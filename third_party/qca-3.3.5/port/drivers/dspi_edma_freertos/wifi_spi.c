/*
 * The Clear BSD License
 * Copyright (c) 2016, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used tom  endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "wifi_common.h"
#include "athdefs.h"

#include "fsl_dspi.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "fsl_dspi_edma.h"
#include "wifi_spi.h"

static dspi_master_edma_handle_t g_dspi_edma_m_handle;
static edma_handle_t dspiEdmaMasterRxRegToRxDataHandle;
static edma_handle_t dspiEdmaMasterTxDataToIntermediaryHandle;
static edma_handle_t dspiEdmaMasterIntermediaryToTxRegHandle;
static dspi_master_handle_t g_m_handle;

static SemaphoreHandle_t mutex;
static SemaphoreHandle_t event;
static int32_t g_dma_chunk = 500;
static enum IRQn g_dma_irqs[][FSL_FEATURE_EDMA_MODULE_CHANNEL] = DMA_CHN_IRQS;
static enum IRQn g_spi_irqs[] = SPI_IRQS;
static dspi_master_config_t g_spi_config;

static SPI_Type *g_spi_base = NULL;
static uint32_t g_xfer_cs = 0;
static uint32_t g_irq_threshold = 0;

extern uint32_t DSPI_GetInstance(SPI_Type *base);

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
static void DSPI_DMA_MasterUserCallback(SPI_Type *base, dspi_master_edma_handle_t *handle, status_t status, void *userData)
{
    /* disable DMA requests before invoke callback */
    BaseType_t reschedule = pdFALSE;
    xSemaphoreGiveFromISR(event, &reschedule);
    portYIELD_FROM_ISR(reschedule);
}

/* 
 * IRQ handler, release transfer semaphore
 */
static void DSPI_MasterUserCallback(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData)
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
    uint32_t spi_id = DSPI_GetInstance(config->spi.base);
    NVIC_SetPriority(g_spi_irqs[spi_id], config->irq_mode.spi_irq_prio);

    /* DSPI IRQ non-blocking handle */
    DSPI_MasterTransferCreateHandle(config->spi.base, &g_m_handle, DSPI_MasterUserCallback, NULL);

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
    memset(&dspiEdmaMasterRxRegToRxDataHandle, 0, sizeof(dspiEdmaMasterRxRegToRxDataHandle));
    memset(&dspiEdmaMasterTxDataToIntermediaryHandle, 0, sizeof(dspiEdmaMasterTxDataToIntermediaryHandle));
    memset(&dspiEdmaMasterIntermediaryToTxRegHandle, 0, sizeof(dspiEdmaMasterIntermediaryToTxRegHandle));
    EDMA_CreateHandle(&dspiEdmaMasterRxRegToRxDataHandle, config->dma_mode.dma_base, config->dma_mode.dma_rx_chnl);
    EDMA_CreateHandle(&dspiEdmaMasterTxDataToIntermediaryHandle, config->dma_mode.dma_base, config->dma_mode.dma_im_chnl);
    EDMA_CreateHandle(&dspiEdmaMasterIntermediaryToTxRegHandle, config->dma_mode.dma_base, config->dma_mode.dma_tx_chnl);
    DSPI_MasterTransferCreateHandleEDMA(config->spi.base, &g_dspi_edma_m_handle, DSPI_DMA_MasterUserCallback, NULL,
                                        &dspiEdmaMasterRxRegToRxDataHandle, &dspiEdmaMasterTxDataToIntermediaryHandle,
                                        &dspiEdmaMasterIntermediaryToTxRegHandle);

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
    SPI_Type *base,
    uint32_t src_clk_hz,
    uint32_t xfer_cs,
    dspi_master_config_t *user_config
)
{
    assert(NULL != base);
    assert(NULL != user_config);

    mutex = xSemaphoreCreateMutex();
    assert(NULL != mutex);
    event = xSemaphoreCreateBinary();
    assert(NULL != event);

    /* DSPI init */
    DSPI_MasterInit(base, &g_spi_config, src_clk_hz);

    return A_OK;
}

/*
 * Transfer data in DMA mode
 */
static A_STATUS WIFIDRVS_SPI_DMA_Transfer(dspi_transfer_t *transfer)
{
    assert(NULL != transfer);

    status_t result = DSPI_MasterTransferEDMA(g_spi_base, &g_dspi_edma_m_handle, transfer);
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
static A_STATUS WIFIDRVS_SPI_IRQ_Transfer(dspi_transfer_t *transfer)
{
    assert(NULL != transfer);

    status_t result = DSPI_MasterTransferNonBlocking(g_spi_base, &g_m_handle, transfer);
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
static A_STATUS WIFIDRVS_SPI_Transfer(dspi_transfer_t *transfer)
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
            /* DSPI over EDMA can transfer only limited number of bytes
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
    g_spi_config = config->spi.config;

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
    DSPI_Deinit(config->spi.base);

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
A_STATUS WIFIDRVS_SPI_GetSPIConfig(dspi_master_config_t *user_config, uint32_t baudrate, dspi_which_pcs_t cs)
{
    assert(!(NULL == user_config));
    if (NULL == user_config) return A_ERROR;

    memset(user_config, 0, sizeof(*user_config));
    DSPI_MasterGetDefaultConfig(user_config);
    user_config->whichCtar = kDSPI_Ctar0;
    user_config->ctarConfig.baudRate = baudrate;
    user_config->ctarConfig.bitsPerFrame = 8;
    user_config->ctarConfig.cpol = kDSPI_ClockPolarityActiveLow;
    user_config->ctarConfig.cpha = kDSPI_ClockPhaseSecondEdge;
    user_config->ctarConfig.direction = kDSPI_MsbFirst;
    user_config->ctarConfig.pcsToSckDelayInNanoSec = 1000000000U / baudrate;
    user_config->ctarConfig.lastSckToPcsDelayInNanoSec = 1000000000U / baudrate;
    user_config->ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / baudrate;
    user_config->whichPcs = cs;
    user_config->pcsActiveHighOrLow = kDSPI_PcsActiveLow;
    user_config->enableContinuousSCK = false;
    user_config->enableRxFifoOverWrite = false;
    user_config->enableModifiedTimingFormat = false;
    user_config->samplePoint = kDSPI_SckToSin0Clock;

    return A_OK;
}

/*!
 * @brief WiFi SPI transfer SPI
 */
A_STATUS WIFIDRVS_SPI_InOutToken(uint32_t OutToken, uint8_t DataSize, uint32_t *pInToken)
{
    A_STATUS result;
    dspi_transfer_t transfer = {0};

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
    dspi_transfer_t transfer = {0};

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

