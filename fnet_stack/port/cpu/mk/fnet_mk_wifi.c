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
*  Wi-Fi driver interafce.
*
***************************************************************************/

#include "fnet.h"
#if FNET_MK && FNET_CFG_CPU_WIFI

#include "port/cpu/netif/qca/fnet_qca.h"

/************************************************************************
* Wi-Fi interface structures.
*************************************************************************/
static fnet_eth_if_t fnet_mk_wifi_if =
{
    .eth_prv = &fnet_qca_if,                    /* Points to Wi-Fi driver-specific control data structure. */
    .eth_output = fnet_qca_output,              /* Ethernet driver output.*/
};

fnet_netif_t fnet_cpu_wifi_if =
{
    .netif_name = FNET_CFG_CPU_WIFI_NAME,       /* Network interface name.*/
    .netif_mtu = FNET_ETH_MTU,                  /* Maximum transmission unit.*/
    .netif_prv = &fnet_mk_wifi_if,              /* Points to interface specific data structure.*/
    .netif_api = &fnet_qca_api                  /* Interface API */
};

/************************************************************************
* Wi-Fi IO initialization.
*************************************************************************/
#if FNET_CFG_CPU_WIFI_IO_INIT
/************************************************************************
* DESCRIPTION: GT202/QCA4002 Wi-Fi board pin initialization.
*************************************************************************/
void BOARD_InitGT202Shield(void)
{
#if FNET_CFG_CPU_MK64FN1
    FNET_MK_PORT_MemMapPtr pctl;

    /*- {pin_num: '93', peripheral: SPI0, signal: PCS0_SS, pin_signal: PTD0/LLWU_P12/SPI0_PCS0/UART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b}
    - {pin_num: '94', peripheral: SPI0, signal: SCK, pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/UART2_CTS_b/FTM3_CH1/FB_CS0_b}
    - {pin_num: '95', peripheral: SPI0, signal: SOUT, pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/I2C0_SCL}
    - {pin_num: '96', peripheral: SPI0, signal: SIN, pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/I2C0_SDA}
    - {pin_num: '73', peripheral: GPIOC, signal: 'GPIO, 3', pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK, direction: INPUT, pull_select: up,
    pull_enable: enable}
    - {pin_num: '84', peripheral: GPIOC, signal: 'GPIO, 12', pin_signal: PTC12/UART4_RTS_b/FB_AD27/FTM3_FLT0, direction: OUTPUT, pull_enable: enable}*/

    /* Enable clock for Port C and D*/
    FNET_MK_SIM_SCGC5 |= FNET_MK_SIM_SCGC5_PORTC_MASK | FNET_MK_SIM_SCGC5_PORTD_MASK;

    pctl = (FNET_MK_PORT_MemMapPtr)FNET_MK_PORTC_BASE_PTR;
    pctl->PCR[12] = FNET_MK_PORT_PCR_MUX(1)|FNET_MK_PORT_PCR_PE(1);     /* PORTC12 (pin 84) is configured as PTC12 (GPIO). Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input.*/
    pctl->PCR[3] = FNET_MK_PORT_PCR_MUX(1)|FNET_MK_PORT_PCR_PE(1)|FNET_MK_PORT_PCR_PS(1);   /* PPORTC3 (pin 73) is configured as PTC3 (GPIO). 
                                                                                            Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input.
                                                                                            Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */

    pctl = (FNET_MK_PORT_MemMapPtr)FNET_MK_PORTD_BASE_PTR;
    pctl->PCR[0] = FNET_MK_PORT_PCR_MUX(2);                 /* PORTD0 (pin 93) is configured as SPI0_PCS0 */
    pctl->PCR[1] = FNET_MK_PORT_PCR_MUX(2);                 /* PORTD1 (pin 94) is configured as SPI0_SCK */
    pctl->PCR[2] = FNET_MK_PORT_PCR_MUX(2);                 /* PORTD2 (pin 95) is configured as SPI0_SOUT */
    pctl->PCR[3] = FNET_MK_PORT_PCR_MUX(2);                 /* PORTD3 (pin 96) is configured as SPI0_SIN */
#endif /* FNET_CFG_CPU_MK64FN1 */
}

/************************************************************************
* DESCRIPTION: Silex2401/QCA4002 Wi-Fi board pin initialization.
*************************************************************************/
void BOARD_InitSilex2401Shield(void) 
{
#if FNET_CFG_CPU_MK64FN1
    FNET_MK_PORT_MemMapPtr pctl;

    /*  - {pin_num: '96', peripheral: SPI0, signal: SIN, pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/I2C0_SDA}
    - {pin_num: '95', peripheral: SPI0, signal: SOUT, pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/I2C0_SCL}
    - {pin_num: '94', peripheral: SPI0, signal: SCK, pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/UART2_CTS_b/FTM3_CH1/FB_CS0_b}
    - {pin_num: '93', peripheral: SPI0, signal: PCS0_SS, pin_signal: PTD0/LLWU_P12/SPI0_PCS0/UART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b}
    - {pin_num: '57', peripheral: GPIOB, signal: 'GPIO, 9', pin_signal: PTB9/SPI1_PCS1/UART3_CTS_b/FB_AD20, direction: INPUT, pull_select: up, pull_enable: enable}
    - {pin_num: '69', peripheral: GPIOB, signal: 'GPIO, 23', pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28, direction: OUTPUT, pull_enable: enable}*/

    /* Enable clock for Port C and D*/
    FNET_MK_SIM_SCGC5 |= FNET_MK_SIM_SCGC5_PORTB_MASK | FNET_MK_SIM_SCGC5_PORTD_MASK;

    pctl = (FNET_MK_PORT_MemMapPtr)FNET_MK_PORTB_BASE_PTR;
    pctl->PCR[23] = FNET_MK_PORT_PCR_MUX(1)|FNET_MK_PORT_PCR_PE(1);     /* PORTB23 (pin 69) is configured as PTB23 (GPIO). 
                                                                        Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input.*/
    pctl->PCR[9] = FNET_MK_PORT_PCR_MUX(1)|FNET_MK_PORT_PCR_PE(1)|FNET_MK_PORT_PCR_PS(1);   /* PORTB9 (pin 57) is configured as PTB9 (GPIO). 
                                                                                            Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input.
                                                                                            Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */

    pctl = (FNET_MK_PORT_MemMapPtr)FNET_MK_PORTD_BASE_PTR;
    pctl->PCR[0] = FNET_MK_PORT_PCR_MUX(2);                 /* PORTD0 (pin 93) is configured as SPI0_PCS0 */
    pctl->PCR[1] = FNET_MK_PORT_PCR_MUX(2);                 /* PORTD1 (pin 94) is configured as SPI0_SCK */
    pctl->PCR[2] = FNET_MK_PORT_PCR_MUX(2);                 /* PORTD2 (pin 95) is configured as SPI0_SOUT */
    pctl->PCR[3] = FNET_MK_PORT_PCR_MUX(2);                 /* PORTD3 (pin 96) is configured as SPI0_SIN */
#endif /* FNET_CFG_CPU_MK64FN1 */
}
#if !FNET_CFG_CPU_MK64FN1
 #warning "QCA400x Wi-Fi board pin initialization is not tested for your platform."
#endif

#endif /* FNET_CFG_CPU_WIFI_IO_INIT */

#endif /* FNET_MK && FNET_CFG_CPU_WIFI */
