/**************************************************************************
*
* Copyright 2011-2018 by Andrey Butok. FNET Community.
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
*  MPC Serial port I/O functions.
*
***************************************************************************/
#include "fnet.h"

#if FNET_MPC

#include "fnet.h"

/********************************************************************/
void fnet_cpu_serial_putchar (fnet_index_t port_number, fnet_char_t character)
{
#if FNET_CFG_CPU_MPC564xBC || FNET_CFG_CPU_MPC5744P || FNET_CFG_CPU_S32R274
    /* Send the character */
    FNET_MPC_LIN_BDRL(port_number) = (fnet_uint8_t)character;

    while(!(FNET_MPC_LIN_UARTSR(port_number) & 0x2))
    {};

    /* Clear Data Send Complete */
    FNET_MPC_LIN_UARTSR(port_number) = 0x2;
#endif

#if FNET_CFG_CPU_MPC5668G
    while(!(FNET_MPC_ESCI_IFSR1(port_number) & 0x8000))
    {};  /* Wait for previous transmissions to finish */

    FNET_MPC_ESCI_IFSR1(port_number) = 0x8000;				/* Clear transmission complete flag */

    FNET_MPC_ESCI_SDR(port_number) = (fnet_uint8_t)character;
#endif

}

/********************************************************************/
fnet_int32_t fnet_cpu_serial_getchar (fnet_index_t port_number)
{

#if FNET_CFG_CPU_MPC5668G
    /* If character received. */
    if(FNET_MPC_ESCI_IFSR1(port_number) & 0x2000)
    {
        /* Clear recieved flag.*/
        FNET_MPC_ESCI_IFSR1(port_number) = 0x2000;  /* Clear flag */
        return FNET_MPC_ESCI_SDR(port_number) & 0xFF;
    }
#endif

#if FNET_CFG_CPU_MPC564xBC || FNET_CFG_CPU_MPC5744P || FNET_CFG_CPU_S32R274
    /* If character received.*/
    if (FNET_MPC_LIN_UARTSR(port_number) & 0x4)
    {
        /* Clear recieved flag.*/
        FNET_MPC_LIN_UARTSR(port_number) = 0x4;
        return (FNET_MPC_LIN_BDRM(port_number) & 0xFF);
    };
#endif

    return FNET_ERR;
}

#if FNET_CFG_CPU_SERIAL_IO_INIT
static inline void fnet_cpu_serial_gpio_init(fnet_int32_t port_number)
{
    /* Enable the proper UART pins */

#if FNET_CFG_CPU_MPC5668G
    switch (port_number)
    {
        case 1:
            FNET_MPC_GPIO_PCR(62) = 0x600;
            FNET_MPC_GPIO_PCR(63) = 0x600;
            break;
        case 0:
        default:
            FNET_MPC_GPIO_PCR(60) = 0x600;
            FNET_MPC_GPIO_PCR(61) = 0x600;
            break;
    }
#endif

#if FNET_CFG_CPU_MPC5744P
    switch (port_number)
    {
        case 1:

            break;
        case 0:
        default:
            /* LINFlex 0 is connected to the MPC5746M MB
            PB[2]    MSCR[18]   TXD      LIN0 O      SRC[1:0]=00    OBE=1    ODE=0    SMC=0    APC=0    IBE=0    HYS=0    PUS=0    PUE=0    INV=0    SSS=0001
            PB[3]    MSCR[19]   RXD      LIN0 O      SRC[1:0]=00    OBE=0    ODE=0    SMC=0    APC=0    IBE=1    HYS=0    PUS=0    PUE=0    INV=0    SSS=0000
            */
            FNET_MPC5744_GPIO_MSCR(18)  = 0x02000001;    /* Set to LIN0_TXD */
            FNET_MPC5744_GPIO_MSCR(19)  = 0x00080000;    /* Set to LIN0_RXD */

            FNET_MPC5744_GPIO_IMCR(165) = 0x00000001;
            break;
    }
#endif
#if FNET_CFG_CPU_S32R274
    switch (port_number)
    {
        case 1:

            break;
        case 0:
        default:
            /* LINFlex 1 is connected to UART->USB converter on S32R274 daughtercard
               NOT CONNECTED ON S32R274-RADAR
            PD[11]    MSCR[59]   TXD      LIN1 O      SRC[1:0]=00    OBE=1    ODE=0    SMC=0    APC=0    IBE=0    HYS=0    PUS=0    PUE=0    INV=0    SSS=0010
            PF[15]    MSCR[95]   RXD      LIN1 I      SRC[1:0]=00    OBE=0    ODE=0    SMC=0    APC=0    IBE=1    HYS=0    PUS=0    PUE=0    INV=0    SSS=0000
            */
            FNET_S32R274_GPIO_MSCR(18)  = 0x02000001;    /* Set to LIN0_TXD */
            FNET_S32R274_GPIO_MSCR(95)  = 0x00080000;    /* Set to LIN0_RXD */

            FNET_S32R274_GPIO_IMCR(63) = 0x00000003;
            break;
    }
#endif
#if FNET_CFG_CPU_MPC564xBC
    switch (port_number)
    {
        case 9:
            FNET_MPC_GPIO_PCR(130) = 0xA00;
            FNET_MPC_GPIO_PCR(131) = 0x100;
            break;
        case 8:
            FNET_MPC_GPIO_PCR(128) = 0xA00;
            FNET_MPC_GPIO_PCR(129) = 0x100;
            break;
        case 7:
            FNET_MPC_GPIO_PCR(104) = 0xA00;
            FNET_MPC_GPIO_PCR(105) = 0x100;
            break;
        case 6:
            FNET_MPC_GPIO_PCR(102) = 0xA00;
            FNET_MPC_GPIO_PCR(103) = 0x100;
            break;
        case 5:
            FNET_MPC_GPIO_PCR(92) = 0xA00;
            FNET_MPC_GPIO_PCR(93) = 0x100;
            break;
        case 4:
            FNET_MPC_GPIO_PCR(90) = 0xA00;
            FNET_MPC_GPIO_PCR(91) = 0x100;
            break;
        case 3:
            FNET_MPC_GPIO_PCR(74) = 0x600;
            FNET_MPC_GPIO_PCR(75) = 0x100;
            break;
        case 2:
            FNET_MPC_GPIO_PCR(40) = 0x600;
            FNET_MPC_GPIO_PCR(41) = 0x100;
            break;
        case 1:
            FNET_MPC_GPIO_PCR(38) = 0x600;
            FNET_MPC_GPIO_PCR(39) = 0x100;
            break;
        case 0:
        default:
            FNET_MPC_GPIO_PCR(18) = 0x600;
            FNET_MPC_GPIO_PCR(19) = 0x100;
            break;
    }
#endif
}
#endif /* FNET_CFG_CPU_SERIAL_IO_INIT */

/********************************************************************/
void fnet_cpu_serial_init(fnet_index_t port_number, fnet_uint32_t baud_rate)
{

    /*
     * Initialize UART for serial communications
     */

    /* Init GPIO.*/
#if FNET_CFG_CPU_SERIAL_IO_INIT
    fnet_cpu_serial_gpio_init(port_number);
#endif

#if FNET_CFG_CPU_MPC5668G

    FNET_MPC_ESCI_BRR(port_number) = (fnet_uint16_t)((FNET_CFG_CPU_CLOCK_HZ / 64 + baud_rate / 2) / baud_rate);  /* Assuming peripheral clock divided by 8 */
    FNET_MPC_ESCI_CR2(port_number) = 0x2000;                             /* Enable module */
    FNET_MPC_ESCI_CR1(port_number) = 0xC;                                /* Tx & Rx Enabled */

#endif

#if FNET_CFG_CPU_MPC564xBC || FNET_CFG_CPU_MPC5744P || FNET_CFG_CPU_S32R274
    {
        fnet_int32_t lfdivx16;
        /*
         * Reset Transmitter - set sleep = 0 and init = 1
         */
        FNET_MPC_LIN_CR1(port_number) = (FNET_MPC_LIN_CR1(port_number) & 0x0000FFFC) | 0x00000001;
        FNET_MPC_LIN_UARTCR(port_number) = 0x0001;	/* Turn on UART mode so settings can be... set.*/
        FNET_MPC_LIN_UARTCR(port_number) = 0x0033;

#if FNET_CFG_CPU_MPC5744P
        /* Calculate LINIBRR and LINFBRR based on baud rate, assumes 200MHz and /2 on HALFSYS_CLK on Panther

        */
        lfdivx16 = (FNET_CFG_CPU_CLOCK_HZ / 2) / baud_rate;
#elif FNET_CFG_CPU_S32R274
        lfdivx16 = 80000000 / baud_rate;
#else
        /* Calculate LINIBRR and LINFBRR based on baud rate, assumes 120MHz and /4 on Peripheral Set 1 on B3M

        */
        lfdivx16 = (FNET_CFG_CPU_CLOCK_HZ / 4) / baud_rate;
#endif

        FNET_MPC_LIN_LINIBRR(port_number) = lfdivx16 / 16;
        FNET_MPC_LIN_LINFBRR(port_number) = lfdivx16 % 16;

        FNET_MPC_LIN_CR1(port_number) = FNET_MPC_LIN_CR1(port_number) & 0x0000FFFE;
    }
#endif
}

#endif /*FNET_MPC*/
