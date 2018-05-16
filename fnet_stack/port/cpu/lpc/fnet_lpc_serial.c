/**************************************************************************
*
* Copyright 2017 by Andrey Butok. FNET Community.
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
*  LPC Serial port I/O functions.
*
***************************************************************************/

#include "fnet_config.h"

#if FNET_LPC

#include "fnet.h"
/* MCUXpresso */
#include <fsl_usart.h>
#include <pin_mux.h>
#include <fsl_iocon.h>
#include <fsl_clock.h>

/* Array of USART base addresses. */
static USART_Type *const fnet_lpc_serial_usart_base_addr[] = USART_BASE_PTRS;
/* Array of USART clocks. */
static const clock_name_t fnet_lpc_serial_usart_clock[] = {kCLOCK_Flexcomm0, kCLOCK_Flexcomm1, kCLOCK_Flexcomm2, kCLOCK_Flexcomm3, kCLOCK_Flexcomm4, kCLOCK_Flexcomm5, kCLOCK_Flexcomm6, kCLOCK_Flexcomm7, kCLOCK_Flexcomm8, kCLOCK_Flexcomm9};

#define FNET_LPC_USART_COUNT (sizeof(fnet_lpc_serial_usart_base_addr) / sizeof(fnet_lpc_serial_usart_base_addr[0]))

/********************************************************************/
void fnet_cpu_serial_putchar (fnet_index_t port_number, fnet_char_t character)
{
    USART_Type      *base;

    if(port_number < FNET_LPC_USART_COUNT)
    {
        base = fnet_lpc_serial_usart_base_addr[port_number];

        /* Wait until space is available in the FIFO */
        while (!(base->FIFOSTAT & USART_FIFOSTAT_TXNOTFULL_MASK))
        {};
        /* Send the character */
        base->FIFOWR = character;
        /* Wait to finish transfer */
        /* It looks like optional, but whithout it there are lost symbols */
        while (!(base->STAT & USART_STAT_TXIDLE_MASK))
        {};
    }
}
/********************************************************************/
fnet_int32_t fnet_cpu_serial_getchar (fnet_index_t port_number)
{
    USART_Type      *base;
    uint8_t         ch;

    if(port_number < FNET_LPC_USART_COUNT)
    {
        base = fnet_lpc_serial_usart_base_addr[port_number];

        /* Is RX FIFO has some data to read */
        if(base->FIFOSTAT & USART_FIFOSTAT_RXNOTEMPTY_MASK)
        {
            ch = base->FIFORD;
            return (fnet_int32_t)(ch);
        }
    }

    return FNET_ERR;
}

#if FNET_CFG_CPU_SERIAL_IO_INIT
static inline void fnet_cpu_serial_gpio_init(fnet_index_t port_number)
{
    switch(port_number)
    {
        /*
         * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
        BOARD_InitPins:
        - options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
        - pin_list:
          - {pin_num: B13, peripheral: FLEXCOMM0, signal: RXD_SDA_MOSI, pin_signal: PIO0_29/FC0_RXD_SDA_MOSI/CTIMER2_MAT3/SCT0_OUT8/TRACEDATA(2), mode: inactive, invert: disabled,
            glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
          - {pin_num: A2, peripheral: FLEXCOMM0, signal: TXD_SCL_MISO, pin_signal: PIO0_30/FC0_TXD_SCL_MISO/CTIMER0_MAT0/SCT0_OUT9/TRACEDATA(1), mode: inactive, invert: disabled,
            glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
         * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
         */
        case 0:
        {
            /* attach 12 MHz clock to FLEXCOMM0 */
            CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);

            /* Enables the clock for the IOCON block. 0 = Disable; 1 = Enable.: 0x01u */
            CLOCK_EnableClock(kCLOCK_Iocon);

            const uint32_t port0_pin29_config = (/* Pin is configured as FC0_RXD_SDA_MOSI */
                                                    IOCON_PIO_FUNC1 |
                                                    /* No addition pin function */
                                                    IOCON_PIO_MODE_INACT |
                                                    /* Input function is not inverted */
                                                    IOCON_PIO_INV_DI |
                                                    /* Enables digital function */
                                                    IOCON_PIO_DIGITAL_EN |
                                                    /* Input filter disabled */
                                                    IOCON_PIO_INPFILT_OFF |
                                                    /* Standard mode, output slew rate control is enabled */
                                                    IOCON_PIO_SLEW_STANDARD |
                                                    /* Open drain is disabled */
                                                    IOCON_PIO_OPENDRAIN_DI);
            /* PORT0 PIN29 (coords: B13) is configured as FC0_RXD_SDA_MOSI */
            IOCON_PinMuxSet(IOCON, 0U, 29U, port0_pin29_config);

            const uint32_t port0_pin30_config = (/* Pin is configured as FC0_TXD_SCL_MISO */
                                                    IOCON_PIO_FUNC1 |
                                                    /* No addition pin function */
                                                    IOCON_PIO_MODE_INACT |
                                                    /* Input function is not inverted */
                                                    IOCON_PIO_INV_DI |
                                                    /* Enables digital function */
                                                    IOCON_PIO_DIGITAL_EN |
                                                    /* Input filter disabled */
                                                    IOCON_PIO_INPFILT_OFF |
                                                    /* Standard mode, output slew rate control is enabled */
                                                    IOCON_PIO_SLEW_STANDARD |
                                                    /* Open drain is disabled */
                                                    IOCON_PIO_OPENDRAIN_DI);
            /* PORT0 PIN30 (coords: A2) is configured as FC0_TXD_SCL_MISO */
            IOCON_PinMuxSet(IOCON, 0U, 30U, port0_pin30_config);
        }
        break;
        default:
            //TBD
            break;
    }
}
#endif /* FNET_CFG_CPU_SERIAL_IO_INIT */

/********************************************************************/
fnet_return_t fnet_cpu_serial_init(fnet_index_t port_number, fnet_uint32_t baud_rate)
{
    usart_config_t  config;
    uint32_t        srcClock_Hzl;
    USART_Type      *base;
    fnet_return_t   result = FNET_ERR;

    if(port_number < FNET_LPC_USART_COUNT)
    {
#if FNET_CFG_CPU_SERIAL_IO_INIT
        /* Init GPIO.*/
        fnet_cpu_serial_gpio_init(port_number);
#endif
        base = fnet_lpc_serial_usart_base_addr[port_number];
        srcClock_Hzl = CLOCK_GetFreq(fnet_lpc_serial_usart_clock[port_number]);

        /* Set USART driver parameters. */
        USART_GetDefaultConfig(&config);
        config.baudRate_Bps = baud_rate;
        config.enableTx = true;
        config.enableRx = true;

        USART_Init(base, &config, srcClock_Hzl);

        result = FNET_OK;
    }

    return result;
}

#endif /*FNET_LPC*/
