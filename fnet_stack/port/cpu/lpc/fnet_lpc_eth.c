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
*  Ethernet driver interface.
*
***************************************************************************/

#include "fnet.h"
#if FNET_LPC && FNET_CFG_CPU_ETH0

#include "port/netif/enet/fnet_enet.h"

/* SDK */
#include "fsl_common.h"
#include "fsl_iocon.h"
#include "pin_mux.h"

static fnet_return_t fnet_lpc_eth_init(fnet_netif_t *netif);

/************************************************************************
* Ethernet interface structure.
*************************************************************************/
static fnet_eth_if_t fnet_lpc_eth0_if =
{
    .eth_prv = &fnet_enet0_if,                      /* Points to Ethernet driver-specific control data structure. */
    .eth_mac_number = 0,                            /* MAC module number. */
    .eth_output = fnet_enet_output,                 /* Ethernet driver output.*/
    .eth_phy_addr = FNET_CFG_CPU_ETH0_PHY_ADDR,     /* Set default PHY address */
    .eth_cpu_init = fnet_lpc_eth_init,
#if FNET_CFG_MULTICAST
    .eth_multicast_join = fnet_enet_multicast_join,  /* Ethernet driver join multicast group.*/
    .eth_multicast_leave = fnet_enet_multicast_leave /* Ethernet driver leave multicast group.*/
#endif
};

fnet_netif_t fnet_cpu_eth0_if =
{
    .netif_name = FNET_CFG_CPU_ETH0_NAME,     /* Network interface name.*/
    .netif_mtu = FNET_CFG_CPU_ETH0_MTU,       /* Maximum transmission unit.*/
    .netif_prv = &fnet_lpc_eth0_if,            /* Points to interface specific data structure.*/
    .netif_api = &fnet_enet_api                /* Interface API */
};


/* If vector table is in ROM, pre-install FNET ISR for ENET interrupt*/
#if !FNET_CFG_CPU_VECTOR_TABLE_IS_IN_RAM
void ETHERNET_IRQHandler (void)
{
    FNET_ISR_HANDLER();
}
#endif

/************************************************************************
* DESCRIPTION: Ethernet IO initialization.
*************************************************************************/
static fnet_return_t fnet_lpc_eth_init(fnet_netif_t *netif)
{
#if FNET_CFG_CPU_ETH_IO_INIT

#if FNET_CFG_CPU_LPC54628
    /*
      - {pin_num: B14, peripheral: ENET, signal: 'ENET_TXD, 0', pin_signal: PIO4_8/ENET_TXD0/FC2_SCK/USB0_OVERCURRENTN/USB0_UP_LED/SCT0_GPI1, mode: inactive, invert: disabled,
        glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
      - {pin_num: B9, peripheral: ENET, signal: ENET_RX_DV, pin_signal: PIO4_10/ENET_RX_DV/FC2_TXD_SCL_MISO/USB1_OVERCURRENTN/USB1_UP_LED/SCT0_GPI3, mode: inactive, invert: disabled,
        glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
      - {pin_num: A9, peripheral: ENET, signal: 'ENET_RXD, 0', pin_signal: PIO4_11/ENET_RXD0/FC2_CTS_SDA_SSEL0/USB0_IDVALUE/SCT0_GPI4, mode: inactive, invert: disabled,
        glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
      - {pin_num: A6, peripheral: ENET, signal: 'ENET_RXD, 1', pin_signal: PIO4_12/ENET_RXD1/FC2_RTS_SCL_SSEL1/SCT0_GPI5, mode: inactive, invert: disabled, glitch_filter: disabled,
        slew_rate: standard, open_drain: disabled}
      - {pin_num: B6, peripheral: ENET, signal: ENET_TX_EN, pin_signal: PIO4_13/ENET_TX_EN/CTIMER4_MAT0/SCT0_GPI6, mode: inactive, invert: disabled, glitch_filter: disabled,
        slew_rate: standard, open_drain: disabled}
      - {pin_num: B5, peripheral: ENET, signal: ENET_RX_CLK, pin_signal: PIO4_14/ENET_RX_CLK/CTIMER4_MAT1/FC9_SCK/SCT0_GPI7, mode: inactive, invert: disabled, glitch_filter: disabled,
        slew_rate: standard, open_drain: disabled}
      - {pin_num: A4, peripheral: ENET, signal: ENET_MDC, pin_signal: PIO4_15/ENET_MDC/CTIMER4_MAT2/FC9_RXD_SDA_MOSI, mode: inactive, invert: disabled, glitch_filter: disabled,
        slew_rate: standard, open_drain: disabled}
      - {pin_num: C4, peripheral: ENET, signal: ENET_MDIO, pin_signal: PIO4_16/ENET_MDIO/CTIMER4_MAT3/FC9_TXD_SCL_MISO, mode: inactive, invert: disabled, glitch_filter: disabled,
        slew_rate: standard, open_drain: disabled}
      - {pin_num: E14, peripheral: ENET, signal: 'ENET_TXD, 1', pin_signal: PIO0_17/FC4_SSEL2/SD_CARD_DET_N/SCT0_GPI7/SCT0_OUT0/EMC_OEN/ENET_TXD1, mode: inactive, invert: disabled,
        glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
     */
    /* Enables the clock for the IOCON block. 0 = Disable; 1 = Enable.: 0x01u */
    CLOCK_EnableClock(kCLOCK_Iocon);

    const uint32_t port0_pin17_config = (/* Pin is configured as ENET_TXD1 */
                                            IOCON_PIO_FUNC7 |
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
    /* PORT0 PIN17 (coords: E14) is configured as ENET_TXD1 */
    IOCON_PinMuxSet(IOCON, 0U, 17U, port0_pin17_config);

    const uint32_t port4_pin10_config = (/* Pin is configured as ENET_RX_DV */
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
    /* PORT4 PIN10 (coords: B9) is configured as ENET_RX_DV */
    IOCON_PinMuxSet(IOCON, 4U, 10U, port4_pin10_config);

    const uint32_t port4_pin11_config = (/* Pin is configured as ENET_RXD0 */
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
    /* PORT4 PIN11 (coords: A9) is configured as ENET_RXD0 */
    IOCON_PinMuxSet(IOCON, 4U, 11U, port4_pin11_config);

    const uint32_t port4_pin12_config = (/* Pin is configured as ENET_RXD1 */
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
    /* PORT4 PIN12 (coords: A6) is configured as ENET_RXD1 */
    IOCON_PinMuxSet(IOCON, 4U, 12U, port4_pin12_config);

    const uint32_t port4_pin13_config = (/* Pin is configured as ENET_TX_EN */
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
    /* PORT4 PIN13 (coords: B6) is configured as ENET_TX_EN */
    IOCON_PinMuxSet(IOCON, 4U, 13U, port4_pin13_config);

    const uint32_t port4_pin14_config = (/* Pin is configured as ENET_RX_CLK */
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
    /* PORT4 PIN14 (coords: B5) is configured as ENET_RX_CLK */
    IOCON_PinMuxSet(IOCON, 4U, 14U, port4_pin14_config);

    const uint32_t port4_pin15_config = (/* Pin is configured as ENET_MDC */
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
    /* PORT4 PIN15 (coords: A4) is configured as ENET_MDC */
    IOCON_PinMuxSet(IOCON, 4U, 15U, port4_pin15_config);

    const uint32_t port4_pin16_config = (/* Pin is configured as ENET_MDIO */
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
    /* PORT4 PIN16 (coords: C4) is configured as ENET_MDIO */
    IOCON_PinMuxSet(IOCON, 4U, 16U, port4_pin16_config);

    const uint32_t port4_pin8_config = (/* Pin is configured as ENET_TXD0 */
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
    /* PORT4 PIN8 (coords: B14) is configured as ENET_TXD0 */
    IOCON_PinMuxSet(IOCON, 4U, 8U, port4_pin8_config);
#endif
#endif /*!FNET_CFG_CPU_ETH_IO_INIT*/
    return FNET_OK;
}

#endif /* FNET_LPC && FNET_CFG_CPU_ETH0 */
