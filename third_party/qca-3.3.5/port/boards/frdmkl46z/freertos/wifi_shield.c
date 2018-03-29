/*
 * The Clear BSD License
 * Copyright (c) 2017, NXP Semiconductor, Inc.
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

#include <stdint.h>

#include "wifi_common.h"
#include "fsl_gpio.h"
#include "driver_cxt.h"
#include "wifi_spi.h"
#include "wifi_shield.h"


/*!
 * @brief Low level initialization, RTOS does not have to run yet
 */
A_STATUS WIFISHIELD_Init(void)
{
    gpio_pin_config_t config;
    memset(&config, 0, sizeof(config));

    /* initialize pinmux */
#ifdef WIFISHIELD_PINMUX_INIT
    WIFISHIELD_PINMUX_INIT();
#endif

    /* Set up WLAN_PWRON signal */
    config.pinDirection = (gpio_pin_direction_t)WIFISHIELD_WLAN_PWRON_DIRECTION;
    GPIO_PinInit(WIFISHIELD_WLAN_PWRON_GPIO, WIFISHIELD_WLAN_PWRON_PIN, &config);

    /* Set up WLAN_IRQ signal */
    config.pinDirection = (gpio_pin_direction_t)WIFISHIELD_WLAN_IRQ_DIRECTION;
    PORT_SetPinInterruptConfig(WIFISHIELD_WLAN_IRQ_PORT, WIFISHIELD_WLAN_IRQ_PIN, kPORT_InterruptFallingEdge);
    GPIO_PinInit(WIFISHIELD_WLAN_IRQ_GPIO, WIFISHIELD_WLAN_IRQ_PIN, &config);

    /* Enable NVIC interrupt for WLAN_IRQ */
    NVIC_EnableIRQ(WIFISHIELD_WLAN_IRQn);

    /* Set NVIC priority if is required by Freertos */
    NVIC_SetPriority(WIFISHIELD_WLAN_IRQn, WIFISHIELD_WLAN_IRQ_PRIORITY);

    return A_OK;
}

/*!
 * @brief Initialization is maintained by WIFI stack
 */
A_STATUS WIFISHIELD_InitDrivers(void *param)
{
    /* expose capabilities of SPI transfer to upper layer TODO: remove in future */
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(param);

    /* Force WIFI stack to use this SPI settings */
    pDCxt->spi_hcd.PowerUpDelay = 1;
    pDCxt->spi_hcd.SpiHWCapabilitiesFlags = (HW_SPI_FRAME_WIDTH_8 | HW_SPI_NO_DMA | HW_SPI_INT_EDGE_DETECT);

    /* Complex DMAMUX/DMA/SPI config structure */
    WIFIDRVS_SPI_config_t spi_config = {0};

    /* load default settings */
    WIFIDRVS_SPI_GetDefaultConfig(&spi_config);

#if defined(WIFISHIELD_DMA) && defined(WIFISHIELD_DMAMUX)
    /* configure dma_mode */
    spi_config.dma_mode.enabled = true;
    spi_config.dma_mode.dmamux_base = (void*)WIFISHIELD_DMAMUX;
    spi_config.dma_mode.dmamux_rx_req = WIFISHIELD_DMAMUX_RX_REQ;
    spi_config.dma_mode.dmamux_tx_req = WIFISHIELD_DMAMUX_TX_REQ;
    spi_config.dma_mode.dma_base = (void*)WIFISHIELD_DMA;
    spi_config.dma_mode.dma_rx_chnl = WIFISHIELD_DMA_RX_CHNL;
    spi_config.dma_mode.dma_tx_chnl = WIFISHIELD_DMA_TX_CHNL;
    spi_config.dma_mode.dma_irq_prio = WIFISHIELD_DMA_IRQ_PRIORITY;
#endif

#if defined(WIFISHIELD_SPI_IRQ_PRIORITY)
    /* configure irq_mode */
    spi_config.irq_mode.enabled = true;
    spi_config.irq_mode.spi_irq_prio = WIFISHIELD_SPI_IRQ_PRIORITY;
#endif

    /* configure spi */
    spi_config.spi.base = (void*)WIFISHIELD_SPI;
    spi_config.spi.clk_hz = CLOCK_GetFreq(WIFISHIELD_SPI_CLOCKSRC);
    spi_config.spi.baudrate = WIFISHIELD_SPI_BAUDRATE;
    spi_config.spi.irq_threshold = WIFISHIELD_SPI_THRESHOLD;

    /* load recommended SPI settings */
    WIFIDRVS_SPI_GetSPIConfig(&spi_config.spi.config, WIFISHIELD_SPI_BAUDRATE);

    /* initialize driver */
    A_STATUS result = WIFIDRVS_SPI_Init(&spi_config);
    assert(A_OK == result);

    return result;
}

/*!
 * @brief Deinitialization is maintained by WIFI stack
 */
A_STATUS WIFISHIELD_DeinitDrivers(void *param)
{
    // WIFI_Spi_Deinit();
    return A_OK;
}

/*!
 * @brief Power up WiFi shield, RTOS does not have to run yet
 */
A_STATUS WIFISHIELD_PowerUp(uint32_t enable)
{
    if (enable)
    {
        GPIO_PortSet(WIFISHIELD_WLAN_PWRON_GPIO, (1 << (WIFISHIELD_WLAN_PWRON_PIN)));
    }
    else
    {
        GPIO_PortClear(WIFISHIELD_WLAN_PWRON_GPIO, (1 << (WIFISHIELD_WLAN_PWRON_PIN)));
    }
    return A_OK;
}

/*!
 * @brief Fn post DriverTask semaphore, can be called only from WLAN_IRQ ISR
 */
void WIFISHIELD_NotifyDriverTask(void *param)
{
    extern QCA_CONTEXT_STRUCT wifiCtx;
    void HW_InterruptHandler(void *pCxt);

    void *pCxt = wifiCtx.MAC_CONTEXT_PTR;
    if (pCxt)
    {
        HW_InterruptHandler(pCxt);
    }
}

#ifdef WIFISHIELD_WLAN_ISR

/*!
 * @brief GPIO IRQ handler, drop request line, notify driver
 */
void WIFISHIELD_WLAN_ISR(void)
{
    uint32_t pin_flags = GPIO_PortGetInterruptFlags(WIFISHIELD_WLAN_IRQ_GPIO);
    GPIO_PortClearInterruptFlags(WIFISHIELD_WLAN_IRQ_GPIO, pin_flags);

    if (pin_flags & (1 << WIFISHIELD_WLAN_IRQ_PIN))
    {
        WIFISHIELD_NotifyDriverTask(NULL);
    }
}

#endif

