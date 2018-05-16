/**************************************************************************
*
* Copyright 2018 by Andrey Butok. FNET Community.
*
***************************************************************************
*
*  Licensed under the Apache License, Version 2.0 (the "License"); you may
*  not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
*  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
***************************************************************************
*
*  iMX RT Serial port I/O functions.
*
***************************************************************************/

#include "fnet_config.h"

#if FNET_MIMXRT

#include "fnet.h"

/*MCUx*/
#include <fsl_lpuart.h>
#include <fsl_iomuxc.h>

static LPUART_Type *fnet_mimxrt_get_uart_port_ptr[] = LPUART_BASE_PTRS;

#if FNET_CFG_CPU_SERIAL_IO_INIT
static inline fnet_return_t fnet_cpu_serial_gpio_init(fnet_index_t port_number)
{
    fnet_return_t result = FNET_ERR;

    /* Enable the pins for and clock to the selected UART */
    switch(port_number)
    {
        /* LPUART startting from number 1*/
        case 1:
            CLOCK_EnableClock(kCLOCK_Iomuxc);          /* iomuxc clock (iomuxc_clk_enable): 0x03u */

            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_12_LPUART1_TX,        /* GPIO_AD_B0_12 is configured as LPUART1_TX */
                0U);                                    /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_SetPinMux(
                IOMUXC_GPIO_AD_B0_13_LPUART1_RX,        /* GPIO_AD_B0_13 is configured as LPUART1_RX */
                0U);                                    /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_SetPinConfig(
                IOMUXC_GPIO_AD_B0_12_LPUART1_TX,        /* GPIO_AD_B0_12 PAD functional properties : */
                0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                        Drive Strength Field: R0/6
                                                        Speed Field: medium(100MHz)
                                                        Open Drain Enable Field: Open Drain Disabled
                                                        Pull / Keep Enable Field: Pull/Keeper Enabled
                                                        Pull / Keep Select Field: Keeper
                                                        Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                        Hyst. Enable Field: Hysteresis Disabled */
            IOMUXC_SetPinConfig(
                IOMUXC_GPIO_AD_B0_13_LPUART1_RX,        /* GPIO_AD_B0_13 PAD functional properties : */
                0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                        Drive Strength Field: R0/6
                                                        Speed Field: medium(100MHz)
                                                        Open Drain Enable Field: Open Drain Disabled
                                                        Pull / Keep Enable Field: Pull/Keeper Enabled
                                                        Pull / Keep Select Field: Keeper
                                                        Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                        Hyst. Enable Field: Hysteresis Disabled */
            result = FNET_OK;
            break;
        default:
            break;
    }

    return result;
}
#endif /* FNET_CFG_CPU_SERIAL_IO_INIT */

/********************************************************************/
fnet_return_t fnet_cpu_serial_init(fnet_index_t port_number, fnet_uint32_t baud_rate)
{
    fnet_return_t   result = FNET_ERR;
    lpuart_config_t lpuart_config;
    LPUART_Type     *lpuart_base;
    uint32_t        lpuart_src_clock;

    if(port_number < sizeof(fnet_mimxrt_get_uart_port_ptr))
    {
#if FNET_CFG_CPU_SERIAL_IO_INIT
        /* Init GPIO.*/
        result = fnet_cpu_serial_gpio_init(port_number);

        if(result != FNET_OK)
        {
            goto EXIT;
        }
#endif
        lpuart_base = fnet_mimxrt_get_uart_port_ptr[port_number];
        if(lpuart_base)
        {
            /* PLL3 source or OSC source */
            if (CLOCK_GetMux(kCLOCK_UartMux) == 0)
            {
                lpuart_src_clock = (CLOCK_GetPllFreq(kCLOCK_PllUsb1) / 6U) / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);/* PLL3 div6 80M */
            }
            else
            {
                lpuart_src_clock = CLOCK_GetOscFreq() / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
            }

            LPUART_GetDefaultConfig(&lpuart_config);
            lpuart_config.baudRate_Bps = baud_rate;
            /* Enable clock and initial UART module follow user configure structure. */
            if(LPUART_Init(lpuart_base, &lpuart_config, lpuart_src_clock) == kStatus_Success)
            {
                LPUART_EnableTx(lpuart_base, true);
                LPUART_EnableRx(lpuart_base, true);
                result = FNET_OK;
            }
        }
    }

EXIT:
    return result;
}

/********************************************************************/
void fnet_cpu_serial_putchar (fnet_index_t port_number, fnet_char_t character)
{
    LPUART_Type     *lpuart_base;

    if(port_number < sizeof(fnet_mimxrt_get_uart_port_ptr))
    {
        lpuart_base = fnet_mimxrt_get_uart_port_ptr[port_number];
        if(lpuart_base)
        {
            /* Wait until space is available in the FIFO */
            while (!(lpuart_base->STAT & LPUART_STAT_TDRE_MASK))
            {}
            /* Send the character */
            lpuart_base->DATA = character;
        }
    }
}
/********************************************************************/
fnet_int32_t fnet_cpu_serial_getchar (fnet_index_t port_number)
{
    LPUART_Type     *lpuart_base;

    if(port_number < sizeof(fnet_mimxrt_get_uart_port_ptr))
    {
        lpuart_base = fnet_mimxrt_get_uart_port_ptr[port_number];
        if(lpuart_base)
        {
            if((lpuart_base->STAT & LPUART_STAT_RDRF_MASK) != 0u)
            {
                /* Return the 8-bit data from the receiver */
                return lpuart_base->DATA & 0xFF;
            }
        }
    }

    return FNET_ERR;
}

#endif /*FNET_MIMXRT*/
