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
*  Ethernet driver interafce.
*
***************************************************************************/

#include "fnet.h"
#if FNET_MIMXRT && FNET_CFG_CPU_ETH0

#include "port/netif/fec/fnet_fec.h"

/* MCUX */
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"

static fnet_return_t fnet_mimxrt_eth_init(fnet_netif_t *netif);
static fnet_return_t fnet_mimxrt_eth_phy_init(fnet_netif_t *netif);

/************************************************************************
* Ethernet interface structure.
*************************************************************************/
static fnet_eth_if_t fnet_mimxrt_eth0_if =
{
    .eth_prv = &fnet_fec0_if,                       /* Points to Ethernet driver-specific control data structure. */
    .eth_mac_number = 0,                            /* MAC module number. */
    .eth_output = fnet_fec_output,                  /* Ethernet driver output.*/
    .eth_phy_addr = FNET_CFG_CPU_ETH0_PHY_ADDR,     /* Set default PHY address */
    .eth_cpu_init = fnet_mimxrt_eth_init,
    .eth_cpu_phy_init = fnet_mimxrt_eth_phy_init,
#if FNET_CFG_MULTICAST
    .eth_multicast_join = fnet_fec_multicast_join,  /* Ethernet driver join multicast group.*/
    .eth_multicast_leave = fnet_fec_multicast_leave /* Ethernet driver leave multicast group.*/
#endif
};

fnet_netif_t fnet_cpu_eth0_if =
{
    .netif_name = FNET_CFG_CPU_ETH0_NAME,     /* Network interface name.*/
    .netif_mtu = FNET_CFG_CPU_ETH0_MTU,       /* Maximum transmission unit.*/
    .netif_prv = &fnet_mimxrt_eth0_if,        /* Points to interface specific data structure.*/
    .netif_api = &fnet_fec_api                /* Interface API */
};

/************************************************************************
* DESCRIPTION: Ethernet IO initialization.
*************************************************************************/
static fnet_return_t fnet_mimxrt_eth_init(fnet_netif_t *netif)
{
#if FNET_CFG_CPU_ETH_IO_INIT
#if FNET_CFG_CPU_MIMXRT1052
    CLOCK_EnableClock(kCLOCK_Iomuxc);          /* iomuxc clock (iomuxc_clk_enable): 0x03u */

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_04_ENET_RX_DATA00,       /* GPIO_B1_04 is configured as ENET_RX_DATA00 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_05_ENET_RX_DATA01,       /* GPIO_B1_05 is configured as ENET_RX_DATA01 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_06_ENET_RX_EN,           /* GPIO_B1_06 is configured as ENET_RX_EN */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_07_ENET_TX_DATA00,       /* GPIO_B1_07 is configured as ENET_TX_DATA00 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_08_ENET_TX_DATA01,       /* GPIO_B1_08 is configured as ENET_TX_DATA01 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_09_ENET_TX_EN,           /* GPIO_B1_09 is configured as ENET_TX_EN */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_10_ENET_REF_CLK,         /* GPIO_B1_10 is configured as ENET_REF_CLK */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_B1_10 */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_11_ENET_RX_ER,           /* GPIO_B1_11 is configured as ENET_RX_ER */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_EMC_40_ENET_MDC,            /* GPIO_EMC_40 is configured as ENET_MDC */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_EMC_41_ENET_MDIO,           /* GPIO_EMC_41 is configured as ENET_MDIO */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */


    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_04_ENET_RX_DATA00,       /* GPIO_B1_04 PAD functional properties : */
        0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_05_ENET_RX_DATA01,       /* GPIO_B1_05 PAD functional properties : */
        0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_06_ENET_RX_EN,           /* GPIO_B1_06 PAD functional properties : */
        0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_07_ENET_TX_DATA00,       /* GPIO_B1_07 PAD functional properties : */
        0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_08_ENET_TX_DATA01,       /* GPIO_B1_08 PAD functional properties : */
        0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_09_ENET_TX_EN,           /* GPIO_B1_09 PAD functional properties : */
        0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_10_ENET_REF_CLK,         /* GPIO_B1_10 PAD functional properties : */
        0x31u);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: low(50MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Disabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_11_ENET_RX_ER,           /* GPIO_B1_11 PAD functional properties : */
        0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_40_ENET_MDC,            /* GPIO_EMC_40 PAD functional properties : */
        0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_41_ENET_MDIO,           /* GPIO_EMC_41 PAD functional properties : */
        0xB829u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: low(50MHz)
                                                 Open Drain Enable Field: Open Drain Enabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B0_09_GPIO1_IO09,        /* GPIO_AD_B0_09 is configured as GPIO1_IO09 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B0_10_GPIO1_IO10,        /* GPIO_AD_B0_10 is configured as GPIO1_IO10 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */

    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_AD_B0_09_GPIO1_IO09,        /* GPIO_AD_B0_09 PAD functional properties : */
        0xB0A9u);                               /* Slew Rate Field: Fast Slew Rate
                                                Drive Strength Field: R0/5
                                                Speed Field: medium(100MHz)
                                                Open Drain Enable Field: Open Drain Disabled
                                                Pull / Keep Enable Field: Pull/Keeper Enabled
                                                Pull / Keep Select Field: Pull
                                                Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_AD_B0_10_GPIO1_IO10,        /* GPIO_AD_B0_10 PAD functional properties : */
        0xB0A9u);                               /* Slew Rate Field: Fast Slew Rate
                                                Drive Strength Field: R0/5
                                                Speed Field: medium(100MHz)
                                                Open Drain Enable Field: Open Drain Disabled
                                                Pull / Keep Enable Field: Pull/Keeper Enabled
                                                Pull / Keep Select Field: Pull
                                                Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                Hyst. Enable Field: Hysteresis Disabled */

#endif /* FNET_CFG_CPU_MIMXRT1052 */
#endif /*!FNET_CFG_CPU_ETH_IO_INIT*/

    /* KSZ8081RNB  PHY */
    /*  RMII – 50 MHZ CLOCK MODE */
    /*• An external 50 MHz clock source (oscillator) connected to XI (Pin 9).
    • Register 1Fh, Bit [7] is set to 1 to select 50 MHz clock mode.*/
    /* For 50 MHz clock mode, the KSZ8081RNB takes in the 50 MHz RMII REF_CLK from the MAC or system board at XI
    (Pin 9) and leaves the REF_CLK (Pin 19) as a no connect.*/
    {
        const clock_enet_pll_config_t config = {true, false, 1/* 50MHz*/};
        CLOCK_InitEnetPll(&config);

        IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);
    }

    /* Enable clock for ENET module */
    CLOCK_EnableClock(kCLOCK_Enet);

#if !FNET_CFG_CPU_CACHE
    /* To be sure that data cache is disabled. */
    SCB_DisableDCache();
#endif

    return FNET_OK;
}

/************************************************************************
* DESCRIPTION: Ethernet Physical Transceiver initialization and/or reset.
*************************************************************************/
static fnet_return_t fnet_mimxrt_eth_phy_init(fnet_netif_t *netif)
{
#if FNET_CFG_CPU_MIMXRT1052 /* & KSZ8081RNB PHY */
    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    GPIO_PinInit(GPIO1, 9, &gpio_config); /* ENET_RST (active is low)*/
    GPIO_PinInit(GPIO1, 10, &gpio_config); /* ENET_INT/NAND_TREE*/
    /* Disable NAND tree mode. Pull up the ENET_INT/NAND_TREE# before RESET.
    At the de-assertion of reset, this pin value is latched by the chip*/
    GPIO_WritePinOutput(GPIO1, 10, 1);
    /* UM: For warm reset, the reset (RST#) pin should be asserted low for a minimum of 500 µs. The strap-in pin values are read
    and updated at the de-assertion of reset.*/
    GPIO_WritePinOutput(GPIO1, 9, 0);
    fnet_timer_delay(100);
    GPIO_WritePinOutput(GPIO1, 9, 1);
    /* UM: After the de-assertion of reset, wait a minimum of 100 µs before starting programming on the MIIM (MDC/MDIO) interface.*/
    fnet_timer_delay(100);

    /* KSZ8081RNB PHY Register 1Fh, Bit [7] is set to 1 to select 50 MHz clock mode.*/
    {
#define KSZ8081RNB_PHY_CONTROL2_REG                 0x1FU   /* PHY Control 2 register. */
#define KSZ8081RNB_PHY_CONTROL2_REFCLK_SELECT_MASK  0x0080U /* PHY RMII reference clock select. 1 = RMII 50 MHz clock mode; 
                                                                    clock input to XI (Pin 9) is 50 MHz  */
        fnet_uint16_t   reg_value;
        while (_fnet_eth_phy_read(netif, KSZ8081RNB_PHY_CONTROL2_REG, &reg_value) != FNET_OK)
        {} /* If locked here, there is problem between ENET and PHY. */

        _fnet_eth_phy_write(netif, KSZ8081RNB_PHY_CONTROL2_REG, (reg_value | KSZ8081RNB_PHY_CONTROL2_REFCLK_SELECT_MASK));
    }
#endif /* FNET_CFG_CPU_MIMXRT1052 & KSZ8081RNB PHY */

    return FNET_OK;
}

/* If vector table is in ROM, pre-install FNET ISR for ENET Receive Frame interrupt*/
#if !FNET_CFG_CPU_VECTOR_TABLE_IS_IN_RAM
void ENET_IRQHandler (void)
{
    FNET_ISR_HANDLER();
}
#endif

#endif /* FNET_MIMXRT && FNET_CFG_CPU_ETH0 */
