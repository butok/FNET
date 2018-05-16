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
*  Wi-Fi driver interafce.
*
***************************************************************************/

#include "fnet.h"
#if FNET_MIMXRT && FNET_CFG_CPU_WIFI

#include "port/netif/qca/fnet_qca.h"

/*MCUX*/
#include "fsl_iomuxc.h"
/************************************************************************
* Wi-Fi interface structures.
*************************************************************************/
static fnet_eth_if_t fnet_mimxrt_wifi_if =
{
    .eth_prv = &fnet_qca_if,                    /* Points to Wi-Fi driver-specific control data structure. */
    .eth_output = fnet_qca_output,              /* Ethernet driver output.*/
};

fnet_netif_t fnet_cpu_wifi_if =
{
    .netif_name = FNET_CFG_CPU_WIFI_NAME,       /* Network interface name.*/
    .netif_mtu = FNET_ETH_MTU,                  /* Maximum transmission unit.*/
    .netif_prv = &fnet_mimxrt_wifi_if,              /* Points to interface specific data structure.*/
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
#if FNET_CFG_CPU_MIMXRT1052
    CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03u */

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B0_03_GPIO1_IO03,        /* GPIO_AD_B0_03 is configured as GPIO1_IO03 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_03_GPIO1_IO19,        /* GPIO_AD_B1_03 is configured as GPIO1_IO19 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B0_00_LPSPI1_SCK,        /* GPIO_SD_B0_00 is configured as LPSPI1_SCK */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B0_00 */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B0_01_LPSPI1_PCS0,       /* GPIO_SD_B0_01 is configured as LPSPI1_PCS0 */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B0_01 */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B0_02_LPSPI1_SDO,        /* GPIO_SD_B0_02 is configured as LPSPI1_SDO */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B0_02 */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B0_03_LPSPI1_SDI,        /* GPIO_SD_B0_03 is configured as LPSPI1_SDI */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B0_03 */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_AD_B0_03_GPIO1_IO03,        /* GPIO_AD_B0_03 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_AD_B1_03_GPIO1_IO19,        /* GPIO_AD_B1_03 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_SD_B0_00_LPSPI1_SCK,        /* GPIO_SD_B0_00 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_SD_B0_01_LPSPI1_PCS0,       /* GPIO_SD_B0_01 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_SD_B0_02_LPSPI1_SDO,        /* GPIO_SD_B0_02 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_SD_B0_03_LPSPI1_SDI,        /* GPIO_SD_B0_03 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
#endif /* FNET_CFG_CPU_MK64FN1 */
}

#if !FNET_CFG_CPU_MIMXRT1052
    #warning "QCA400x Wi-Fi board pin initialization is not tested for your platform."
#endif

#endif /* FNET_CFG_CPU_WIFI_IO_INIT */

#endif /* FNET_MIMXRT && FNET_CFG_CPU_WIFI */
