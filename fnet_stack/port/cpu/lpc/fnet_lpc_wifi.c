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
*  Wi-Fi driver interface.
*
***************************************************************************/

#include "fnet.h"
#if FNET_LPC && FNET_CFG_CPU_WIFI

#include "port/netif/qca/fnet_qca.h"

/* MCUXpresso */
#include <fsl_common.h>
#include <fsl_iocon.h>
#include <pin_mux.h>

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
#if FNET_CFG_CPU_LPC54628
    /* Enables the clock for the IOCON block. 0 = Disable; 1 = Enable.: 0x01u */
    CLOCK_EnableClock(kCLOCK_Iocon);

    IOCON->PIO[1][22] =
        ((IOCON->PIO[1][22] &
          /* Mask bits to zero which are setting */
          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_MODE_MASK | IOCON_PIO_INVERT_MASK | IOCON_PIO_DIGIMODE_MASK)))

         /* Selects pin function.: PORT122 (pin P11) is configured as PIO1_22 */
         | IOCON_PIO_FUNC(PIO122_FUNC_ALT0)

         /* Selects function mode (on-chip pull-up/pull-down resistor control).: Pull-up. Pull-up resistor enabled. */
         | IOCON_PIO_MODE(PIO122_MODE_PULL_UP)

         /* Input polarity.: Disabled. Input function is not inverted. */
         | IOCON_PIO_INVERT(PIO122_INVERT_DISABLED)

         /* Select Analog/Digital mode.: Digital mode. */
         | IOCON_PIO_DIGIMODE(PIO122_DIGIMODE_DIGITAL));

    IOCON->PIO[3][20] = ((IOCON->PIO[3][20] &
                          /* Mask bits to zero which are setting */
                          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))

                         /* Selects pin function.: PORT320 (pin N2) is configured as FC9_SCK */
                         | IOCON_PIO_FUNC(PIO320_FUNC_ALT1)

                         /* Select Analog/Digital mode.: Digital mode. */
                         | IOCON_PIO_DIGIMODE(PIO320_DIGIMODE_DIGITAL));

    IOCON->PIO[3][21] = ((IOCON->PIO[3][21] &
                          /* Mask bits to zero which are setting */
                          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))

                         /* Selects pin function.: PORT321 (pin P5) is configured as FC9_RXD_SDA_MOSI */
                         | IOCON_PIO_FUNC(PIO321_FUNC_ALT1)

                         /* Select Analog/Digital mode.: Digital mode. */
                         | IOCON_PIO_DIGIMODE(PIO321_DIGIMODE_DIGITAL));

    IOCON->PIO[3][22] = ((IOCON->PIO[3][22] &
                          /* Mask bits to zero which are setting */
                          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))

                         /* Selects pin function.: PORT322 (pin N5) is configured as FC9_TXD_SCL_MISO */
                         | IOCON_PIO_FUNC(PIO322_FUNC_ALT1)

                         /* Select Analog/Digital mode.: Digital mode. */
                         | IOCON_PIO_DIGIMODE(PIO322_DIGIMODE_DIGITAL));

    IOCON->PIO[3][30] = ((IOCON->PIO[3][30] &
                          /* Mask bits to zero which are setting */
                          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))

                         /* Selects pin function.: PORT330 (pin K13) is configured as FC9_CTS_SDA_SSEL0 */
                         | IOCON_PIO_FUNC(PIO330_FUNC_ALT1)

                         /* Select Analog/Digital mode.: Digital mode. */
                         | IOCON_PIO_DIGIMODE(PIO330_DIGIMODE_DIGITAL));

    IOCON->PIO[4][7] =
        ((IOCON->PIO[4][7] &
          /* Mask bits to zero which are setting */
          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_MODE_MASK | IOCON_PIO_INVERT_MASK | IOCON_PIO_DIGIMODE_MASK)))

         /* Selects pin function.: PORT47 (pin A14) is configured as PIO4_7 */
         | IOCON_PIO_FUNC(PIO47_FUNC_ALT0)

         /* Selects function mode (on-chip pull-up/pull-down resistor control).: Pull-down. Pull-down resistor enabled. */
         | IOCON_PIO_MODE(PIO47_MODE_PULL_DOWN)

         /* Input polarity.: Disabled. Input function is not inverted. */
         | IOCON_PIO_INVERT(PIO47_INVERT_DISABLED)

         /* Select Analog/Digital mode.: Digital mode. */
         | IOCON_PIO_DIGIMODE(PIO47_DIGIMODE_DIGITAL));
#endif /* FNET_CFG_CPU_LPC54628 */
}

/************************************************************************
* DESCRIPTION: Silex2401/QCA4004 Wi-Fi board pin initialization.
*************************************************************************/
void BOARD_InitSilex2401Shield(void)
{
    //TBD
}
#if !FNET_CFG_CPU_LPC54628
    #warning "QCA400x Wi-Fi board pin initialization is not tested for your platform."
#endif

#endif /* FNET_CFG_CPU_WIFI_IO_INIT */


#endif /* FNET_LPC && FNET_CFG_CPU_WIFI */
