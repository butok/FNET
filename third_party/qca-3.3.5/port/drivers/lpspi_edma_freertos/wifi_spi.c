/*
 * Copyright (c) 2016, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wifi_common.h"
#include "athdefs.h"

#include "fsl_lpspi.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "fsl_lpspi_edma.h"
#include "wifi_spi.h"

static SemaphoreHandle_t mutex;
static SemaphoreHandle_t event;
static lpspi_master_edma_handle_t g_lpspi_edma_m_handle;
static edma_handle_t lpspiEdmaMasterRxRegToRxDataHandle;
static edma_handle_t lpspiEdmaMasterTxDataToTxRegHandle;
static lpspi_master_handle_t g_m_handle;
static int32_t g_dma_chunk = 500;
static enum IRQn g_dma_irqs[][FSL_FEATURE_EDMA_MODULE_CHANNEL] = DMA_CHN_IRQS;
static enum IRQn g_spi_irqs[] = LPSPI_IRQS;
static LPSPI_Type *g_spi_base = NULL;
static uint32_t g_xfer_cs = 0;
static uint32_t g_irq_threshold = 0;

extern uint32_t LPSPI_GetInstance(LPSPI_Type *base);

/* taken from fsl_intmux.c */
#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
#include "fsl_intmux.h"
static IRQn_Type g_intmux_irqs[][FSL_FEATURE_INTMUX_CHANNEL_COUNT] = INTMUX_IRQS;
static INTMUX_Type *const s_intmuxBases[] = INTMUX_BASE_PTRS;
static uint32_t INTMUX_GetInstance(INTMUX_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_intmuxBases); instance++)
    {
        if (s_intmuxBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_intmuxBases));

    return instance;
}
#endif

/* taken from fsl_edma.c */
static DMA_Type *const s_edmaBases[] = DMA_BASE_PTRS;
static uint32_t EDMA_GetInstance(DMA_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_edmaBases); instance++)
    {
        if (s_edmaBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_edmaBases));

    return instance;
}

/*
 * DMA handler, release transfer semaphore
 */
static void LPSPI_DMA_MasterUserCallback(LPSPI_Type *base, lpspi_master_edma_handle_t *handle, status_t status, void *userData)
{
    /* disable DMA requests before invoke callback */
    BaseType_t reschedule = pdFALSE;
    xSemaphoreGiveFromISR(event, &reschedule);
    portYIELD_FROM_ISR(reschedule);
}

/* 
 * IRQ handler, release transfer semaphore
 */
static void LPSPI_MasterUserCallback(LPSPI_Type *base, lpspi_master_handle_t *handle, status_t status, void *userData)
{
    /* disable IRQ requests before invoke callback */
    BaseType_t reschedule = pdFALSE;
    xSemaphoreGiveFromISR(event, &reschedule);
    portYIELD_FROM_ISR(reschedule);
}

/*
 * Initialize SPI IRQ mode
 */
static A_STATUS WIFIDRVS_SPI_InitIRQMode(WIFIDRVS_SPI_config_t *config)
{
    uint32_t spi_id = LPSPI_GetInstance(config->spi.base);
    enum IRQn spi_irq = g_spi_irqs[spi_id];

#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
    /* SPI interrupt goes through INTMUX */
    if (config->irq_mode.intmux_base && (spi_irq > (FSL_FEATURE_INTERRUPT_IRQ_MAX)))
    {
        uint32_t intmux_id = INTMUX_GetInstance(config->irq_mode.intmux_base);
        INTMUX_Init(config->irq_mode.intmux_base);
        INTMUX_EnableInterrupt(config->irq_mode.intmux_base, config->irq_mode.intmux_chnl, g_spi_irqs[spi_id]);
        NVIC_SetPriority(g_intmux_irqs[intmux_id][config->irq_mode.intmux_chnl], config->irq_mode.spi_irq_prio);
    }
    else
    {
        NVIC_SetPriority(spi_irq, config->irq_mode.spi_irq_prio);
    }
#else
    NVIC_SetPriority(spi_irq, config->irq_mode.spi_irq_prio);
#endif

    /* SPI IRQ non-blocking handle */
    LPSPI_MasterTransferCreateHandle(config->spi.base, &g_m_handle, LPSPI_MasterUserCallback, NULL);

    return A_OK;
}

/*
 * Initialize SPI DMA mode
 */
static A_STATUS WIFIDRVS_SPI_InitDMAMode(WIFIDRVS_SPI_config_t *config)
{
    DMAMUX_Init(config->dma_mode.dmamux_base);
    if ((config->dma_mode.dma_rx_chnl >= 0) && (config->dma_mode.dmamux_rx_req >= 0))
    {
        DMAMUX_SetSource(config->dma_mode.dmamux_base, config->dma_mode.dma_rx_chnl, config->dma_mode.dmamux_rx_req);
        DMAMUX_EnableChannel(config->dma_mode.dmamux_base, config->dma_mode.dma_rx_chnl);
    }
    if ((config->dma_mode.dma_tx_chnl >= 0) && (config->dma_mode.dmamux_tx_req >= 0))
    {
        DMAMUX_SetSource(config->dma_mode.dmamux_base, config->dma_mode.dma_tx_chnl, config->dma_mode.dmamux_tx_req);
        DMAMUX_EnableChannel(config->dma_mode.dmamux_base, config->dma_mode.dma_tx_chnl);
    }

    /* Init DMA */
    edma_config_t edma_config = {0};
    EDMA_GetDefaultConfig(&edma_config);
    EDMA_Init(config->dma_mode.dma_base, &edma_config);

    /* Setup DMA/SPI handles */
    memset(&lpspiEdmaMasterRxRegToRxDataHandle, 0, sizeof(lpspiEdmaMasterRxRegToRxDataHandle));
    memset(&lpspiEdmaMasterTxDataToTxRegHandle, 0, sizeof(lpspiEdmaMasterTxDataToTxRegHandle));
    EDMA_CreateHandle(&lpspiEdmaMasterRxRegToRxDataHandle, config->dma_mode.dma_base, config->dma_mode.dma_rx_chnl);
    EDMA_CreateHandle(&lpspiEdmaMasterTxDataToTxRegHandle, config->dma_mode.dma_base, config->dma_mode.dma_tx_chnl);
    LPSPI_MasterTransferCreateHandleEDMA(config->spi.base, &g_lpspi_edma_m_handle, LPSPI_DMA_MasterUserCallback, NULL,
                                         &lpspiEdmaMasterRxRegToRxDataHandle, &lpspiEdmaMasterTxDataToTxRegHandle);

    /* Set up chnl priorities */
    uint32_t edma_id = EDMA_GetInstance(config->dma_mode.dma_base);
    if (config->dma_mode.dma_rx_chnl >= 0)
    {
        NVIC_SetPriority(g_dma_irqs[edma_id][config->dma_mode.dma_rx_chnl], config->dma_mode.dma_irq_prio);
    }
    if (config->dma_mode.dma_im_chnl >= 0)
    {
        NVIC_SetPriority(g_dma_irqs[edma_id][config->dma_mode.dma_im_chnl], config->dma_mode.dma_irq_prio);
    }
    if (config->dma_mode.dma_tx_chnl >= 0)
    {
        NVIC_SetPriority(g_dma_irqs[edma_id][config->dma_mode.dma_tx_chnl], config->dma_mode.dma_irq_prio);
    }

    return A_OK;
}

/*
 * Initialize SPI peripheral
 */
static A_STATUS WIFIDRVS_SPI_InitPeriph(
    LPSPI_Type *base,
    uint32_t src_clk_hz,
    uint32_t xfer_cs,
    lpspi_master_config_t *user_config
)
{
    assert(NULL != base);
    assert(NULL != user_config);

    mutex = xSemaphoreCreateMutex();
    assert(NULL != mutex);
    event = xSemaphoreCreateBinary();
    assert(NULL != event);

    /* LPSPI init */
    LPSPI_MasterInit(base, user_config, src_clk_hz);

    return A_OK;
}

/*
 * Transfer data in DMA mode
 */
static A_STATUS WIFIDRVS_SPI_DMA_Transfer(lpspi_transfer_t *transfer)
{
    assert(NULL != transfer);

    status_t result = LPSPI_MasterTransferEDMA(g_spi_base, &g_lpspi_edma_m_handle, transfer);
    if (kStatus_Success != result)
    {
        assert(0);
        return A_ERROR;
    }
    /* semaphore is released in callback fn */
    if (pdTRUE != xSemaphoreTake(event, portMAX_DELAY))
    {
        assert(0);
        return A_ERROR;
    }
    return A_OK;
}

/*
 * Transfer data in IRQ mode
 */
static A_STATUS WIFIDRVS_SPI_IRQ_Transfer(lpspi_transfer_t *transfer)
{
    assert(NULL != transfer);

    status_t result = LPSPI_MasterTransferNonBlocking(g_spi_base, &g_m_handle, transfer);
    if (kStatus_Success != result)
    {
        assert(0);
        return A_ERROR;
    }
    if (pdTRUE != xSemaphoreTake(event, portMAX_DELAY))
    {
        assert(0);
        result = A_ERROR;
    }
    return A_OK;
}

/*
 * Transfer data
 */
static A_STATUS WIFIDRVS_SPI_Transfer(lpspi_transfer_t *transfer)
{
    A_STATUS result = A_OK;

    /* NOTE: following code expects that SDK drivers do not
     * modify members of 'transfer' argument */
    for (int32_t to_transfer = transfer->dataSize; to_transfer;)
    {
        if (to_transfer < g_irq_threshold)
        {
            /* DMA is unefficient for small amount of data, so use IRQ mode.
             * IRQ mode can transfer unlimited number of data */
            transfer->dataSize = to_transfer;
            result = WIFIDRVS_SPI_IRQ_Transfer(transfer);
            if (A_OK != result)
                break;
            to_transfer = 0;
        }
        else
        {
            /* LPSPI over EDMA can transfer only limited number of bytes
             * so we have to split transfer into chunks */
            transfer->dataSize = to_transfer < g_dma_chunk ? to_transfer : g_dma_chunk;
            result = WIFIDRVS_SPI_DMA_Transfer(transfer);
            if (A_OK != result)
                break;
            to_transfer -= transfer->dataSize;
            /* recalculate rx/rx offsets */
            if (NULL != transfer->txData)
            {
                transfer->txData += transfer->dataSize;
            }
            if (NULL != transfer->rxData)
            {
                transfer->rxData += transfer->dataSize;
            }
        }
    }

    return result;
}

/*!
 * @brief Initialize SPI driver
 */
A_STATUS WIFIDRVS_SPI_Init(WIFIDRVS_SPI_config_t *config)
{
    /* No SPI base address, invalid config*/
    assert(!((NULL == config) || (NULL == config->spi.base)));
    if ((NULL == config) || (NULL == config->spi.base)) return A_ERROR;

    /* IRQ mode only - set threshold to max value */
    if ((config->irq_mode.enabled) && (!config->dma_mode.enabled))
    {
        g_irq_threshold = (uint32_t)-1;
    }
    /* DMA mode only - set threshold to 0 */
    else if ((!config->irq_mode.enabled) && (config->dma_mode.enabled))
    {
        g_irq_threshold = 0;
    }
    /* DMA and IRQ mode - set user defined value  */
    else if ((config->irq_mode.enabled) && (config->dma_mode.enabled))
    {
        g_irq_threshold = config->spi.irq_threshold;
    }
    /* Neither of modes is enabled, return error  */
    else
    {
        return A_ERROR;
    }

    /* Prepare driver internal context */
    g_spi_base = config->spi.base;
    g_xfer_cs = config->spi.xfer_cs;

    /* Initialize SPI peripheral */
    WIFIDRVS_SPI_InitPeriph(config->spi.base, config->spi.clk_hz, config->spi.xfer_cs, &config->spi.config);

    /* Enable IRQ mode */
    if (config->irq_mode.enabled)
    {
        WIFIDRVS_SPI_InitIRQMode(config);
    }

    /* Enable DMA mode */
    if (config->dma_mode.enabled)
    {
        WIFIDRVS_SPI_InitDMAMode(config);
    }

    return A_OK;
}

/*!
 * @brief Deinitialize SPI driver
 */
A_STATUS WIFIDRVS_SPI_Deinit(WIFIDRVS_SPI_config_t *config)
{
    assert(!(NULL == config));
    if (NULL == config) return A_ERROR;

    if (NULL == config->spi.base) return A_ERROR;
    LPSPI_Deinit(config->spi.base);

    return A_OK;
}

/*!
 * @brief Return default configuration
 */
A_STATUS WIFIDRVS_SPI_GetDefaultConfig(WIFIDRVS_SPI_config_t *config)
{
    assert(!(NULL == config));
    if (NULL == config) return A_ERROR;

    memset(config, 0, sizeof(*config));
    config->dma_mode.dma_rx_chnl = -1;
    config->dma_mode.dma_im_chnl = -1;
    config->dma_mode.dma_tx_chnl = -1;

    return A_OK;
}

/*!
 * @brief Return default SPI peripheral settings
 */
A_STATUS WIFIDRVS_SPI_GetSPIConfig(lpspi_master_config_t *user_config, uint32_t baudrate, lpspi_which_pcs_t cs)
{
    assert(!(NULL == user_config));
    if (NULL == user_config) return A_ERROR;

    /* setup SPI */
    memset(user_config, 0, sizeof(*user_config));
    LPSPI_MasterGetDefaultConfig(user_config);

    user_config->baudRate = baudrate;
    user_config->bitsPerFrame = 8;
    user_config->cpol = kLPSPI_ClockPolarityActiveLow;
    user_config->cpha = kLPSPI_ClockPhaseSecondEdge;
    user_config->direction = kLPSPI_MsbFirst;
    user_config->pcsToSckDelayInNanoSec = 1000000000 / baudrate;
    user_config->lastSckToPcsDelayInNanoSec = 1000000000 / baudrate;
    user_config->betweenTransferDelayInNanoSec = 1000000000 / baudrate;
    user_config->whichPcs = cs;
    user_config->pcsActiveHighOrLow = kLPSPI_PcsActiveLow;
    user_config->pinCfg = kLPSPI_SdiInSdoOut;
    user_config->dataOutConfig = kLpspiDataOutRetained;

    return A_OK;
}

/*!
 * @brief WiFi SPI transfer SPI
 */
A_STATUS WIFIDRVS_SPI_InOutToken(uint32_t OutToken, uint8_t DataSize, uint32_t *pInToken)
{
    A_STATUS result;
    lpspi_transfer_t transfer = {0};

    transfer.txData = (uint8_t *)&OutToken;
    transfer.rxData = (uint8_t *)pInToken;
    transfer.dataSize = DataSize;
    transfer.configFlags = g_xfer_cs;

    /* Protect transmit by mutex */
    if (pdTRUE != xSemaphoreTake(mutex, portMAX_DELAY))
    {
        return A_ERROR;
    }
    result = WIFIDRVS_SPI_Transfer(&transfer);
    xSemaphoreGive(mutex);
    return result;
}

/*!
 * @brief WiFi SPI transfer SPI
 */
A_STATUS WIFIDRVS_SPI_InOutBuffer(uint8_t *pBuffer, uint16_t length, uint8_t doRead, boolean sync)
{
    A_STATUS result;
    lpspi_transfer_t transfer = {0};

    if (doRead)
    {
        transfer.txData = NULL;
        transfer.rxData = pBuffer;
    }
    else
    {
        transfer.txData = pBuffer;
        transfer.rxData = NULL;
    }
    transfer.dataSize = length;
    transfer.configFlags = g_xfer_cs;

    /* Protect transmit by mutex */
    if (pdTRUE != xSemaphoreTake(mutex, portMAX_DELAY))
    {
        return A_ERROR;
    }
    result = WIFIDRVS_SPI_Transfer(&transfer);
    xSemaphoreGive(mutex);
    return result;
}

