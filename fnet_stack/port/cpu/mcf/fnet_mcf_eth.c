/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
* Copyright 2008-2010 by Andrey Butok. Freescale Semiconductor, Inc.
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
**********************************************************************/ /*!
*
* @file fnet_mcf_eth.c
*
* @author Andrey Butok
*
* @brief Ethernet driver implementation.
*
***************************************************************************/

#include "fnet.h"
#if FNET_MCF && (FNET_CFG_CPU_ETH0 ||FNET_CFG_CPU_ETH1)
#include "cpu/common/fnet_fec.h"
//DM #include "stack/fnet_eth_prv.h"


#if FNET_CFG_CPU_ETH0
/************************************************************************
* FEC0 interface structure.
*************************************************************************/
struct fnet_eth_if fnet_mcf_eth0_if =
{
    &fnet_fec0_if
    , 0                            /* MAC module number.*/
    , fnet_fec_output
#if FNET_CFG_MULTICAST
    , fnet_fec_multicast_join
    , fnet_fec_multicast_leave
#endif
};
/************************************************************************
* eth0 Interface structure.
*************************************************************************/
fnet_netif_t fnet_eth0_if =
{
    0,                      /* Pointer to the next net_if structure.*/
    0,                      /* Pointer to the previous net_if structure.*/
    FNET_CFG_CPU_ETH0_NAME, /* Network interface name.*/
    FNET_CFG_CPU_ETH0_MTU,  /* Maximum transmission unit.*/
    &fnet_mcf_eth0_if,      /* Points to interface specific data structure.*/
    &fnet_fec_api           /* Interface API */
};
#endif /*FNET_CFG_CPU_ETH0*/

#if FNET_CFG_CPU_ETH1
/************************************************************************
* FEC1 interface structure.
*************************************************************************/
struct fnet_eth_if fnet_mcf_eth1_if =
{
    &fnet_fec1_if
    , 1                  	/* MAC module number.*/
    , fnet_fec_output
#if FNET_CFG_MULTICAST
    , fnet_fec_multicast_join
    , fnet_fec_multicast_leave
#endif
};
/************************************************************************
* eth1 Interface structure.
*************************************************************************/
fnet_netif_t fnet_eth1_if =
{
    0,                      /* Pointer to the next net_if structure.*/
    0,                      /* Pointer to the previous net_if structure.*/
    FNET_CFG_CPU_ETH1_NAME, /* Network interface name.*/
    FNET_CFG_CPU_ETH1_MTU,  /* Maximum transmission unit.*/
    &fnet_mcf_eth1_if,      /* Points to interface specific data structure.*/
    &fnet_fec_api           /* Interface API */
};
#endif /*FNET_CFG_CPU_ETH0*/



/************************************************************************
* NAME: fnet_eth_io_init
*
* DESCRIPTION: Ethernet IO initialization.
*************************************************************************/
void fnet_eth_io_init()
{

#if FNET_CFG_CPU_MCF52235 /* Kirin2 */

    FNET_MCF5223X_GPIO_PLDPAR = (0
                                 | FNET_MCF5223X_GPIO_PLDPAR_ACTLED_ACTLED
                                 | FNET_MCF5223X_GPIO_PLDPAR_LINKLED_LINKLED
                                 | FNET_MCF5223X_GPIO_PLDPAR_SPDLED_SPDLED
                                 | FNET_MCF5223X_GPIO_PLDPAR_DUPLED_DUPLED
                                 | FNET_MCF5223X_GPIO_PLDPAR_COLLED_COLLED
                                 | FNET_MCF5223X_GPIO_PLDPAR_RXLED_RXLED
                                 | FNET_MCF5223X_GPIO_PLDPAR_TXLED_TXLED);

#endif

#if FNET_CFG_CPU_MCF52259 /* Kirin3 */

    FNET_MCF5225X_GPIO_PNQPAR = 0x4880; /* 0x4880 (for Gamma environment, reported by a customer).*/
    FNET_MCF5225X_GPIO_PTIPAR = 0xFF;
    FNET_MCF5225X_GPIO_PTJPAR = 0xFF;

#endif

#if FNET_CFG_CPU_MCF5282 /* Reindeer */

    FNET_MCF5282_GPIO_PEHLPAR = 0xC0;
    FNET_MCF5282_GPIO_PASPAR |= 0x0f00; /* Initialize PEHLPAR to enable ethernet signals.*/

#endif

#if FNET_CFG_CPU_MCF5235 /* CPUV2 */

    FNET_MCF523x_GPIO_PAR_FECI2C = 0xF0;

#endif

#if FNET_CFG_CPU_MCF51CN128 /* Lasko */

    /* Enable MII pins by setting Pin Mux Control Registers: */
    /* Port A */
    FNET_MCF_PTAPF1 = 0x55;    /* MII_RXD2, MII_RXD1, MII_RXD0, MII_RX_DV */
    FNET_MCF_PTAPF2 = 0x54;    /* MII_MDIO, MII_MDC, MII_RXD3 */
    /* Port B */
    FNET_MCF_PTBPF1 = 0x55;    /* MII_TX_EN, MII_TXD0, MII_TXD1, MII_TXD2 */
    FNET_MCF_PTBPF2 = 0x55;    /* MII_RX_CLK, MII_RX_ER, MII_TX_ER, MII_TX_CLK */
    /* Port C */
    FNET_MCF_PTCPF2 = 0x15;    /* MII_TXD3, MII_COL, MII_CRS */

    /* Set high output drive strength for output pins: */
    FNET_MCF_PTADS = 0x6;  /* Output Drive Enable for Port A*/
    FNET_MCF_PTBDS = 0xF4; /* Output Drive Enable for Port B*/
    FNET_MCF_PTCDS = 0x1;  /* Output Drive Enable for Port C*/

    /* Enable filter. */
    FNET_MCF_PTAIFE = 0x06;
    FNET_MCF_PTBIFE = 0xf4;
    FNET_MCF_PTCIFE = 1;

#endif


#if FNET_CFG_CPU_MCF54418 /* Modelo */

    /* FEC Pin Assignment. */
#if FNET_CFG_CPU_ETH_RMII /* Default, RMII on both MACs */
    FNET_MCF5441X_GPIO_PAR_FEC = FNET_MCF5441X_GPIO_PAR_FEC_PAR_FEC_RMII0_RMII1_FULL;
#else /* In this case only MAC0 works. MAC1 is not functional.*/
    FNET_MCF5441X_GPIO_PAR_FEC = FNET_MCF5441X_GPIO_PAR_FEC_PAR_FEC_MII_FULL;
#endif

    /* Set slew rate to highest, otherwise data will be corrupted (checked on practice).*/
    FNET_MCF5441X_GPIO_SRCR_FEC = FNET_MCF5441X_GPIO_SRCR_FEC_SRE_RMII1_SR_HIGHEST | FNET_MCF5441X_GPIO_SRCR_FEC_SRE_RMII0_SR_HIGHEST;

    /* Needed for RMMI1 */
    FNET_MCF5441X_GPIO_PDDR_G |= FNET_MCF5441X_GPIO_PDDR_G_PDDR_G4; 	/* Set GPIO4 as output.*/
    FNET_MCF5441X_GPIO_PODR_G &= ~FNET_MCF5441X_GPIO_PODR_G_PODR_G4; 	/* Clear GPIO4 pin to enable RMMI1 on the QS3VH16233PAG QUICKSWITCH*/

#endif /* FNET_CFG_CPU_MCF54418 */

}


/************************************************************************
* NAME: fnet_eth_phy_init
*
* DESCRIPTION: Ethernet Physical Transceiver initialization and/or reset.
*************************************************************************/
void fnet_eth_phy_init(fnet_fec_if_t *ethif)
{

#if FNET_CFG_CPU_MCF52235
#if 1
    fnet_uint16_t reg_value;
    /* Enable EPHY module, Enable auto_neg at start-up, Let PHY PLLs be determined by PHY.*/
    FNET_MCF_EPHY_EPHYCTL0 = (fnet_uint8_t)(FNET_MCF_EPHY_EPHYCTL0_EPHYEN);

    /* Start-up Delay for Kirin2 = 350uS */
    fnet_timer_delay(1); /* Delay for 1 timer tick (100ms) */

    /* Disable ANE that causes problems with some routers.  Enable full-duplex and 100Mbps */
    fnet_fec_mii_read(ethif, FNET_FEC_MII_REG_CR, &reg_value);
    fnet_fec_mii_write(ethif, FNET_FEC_MII_REG_CR, (fnet_uint16_t)(reg_value & (~FNET_FEC_MII_REG_CR_ANE) | FNET_FEC_MII_REG_CR_DPLX | FNET_FEC_MII_REG_CR_DATARATE));

#else /* Old version, just in case.*/
    fnet_uint16_t reg_value;

    /* Set phy address */
    FNET_MCF_EPHY_EPHYCTL1 = (fnet_uint8)FNET_MCF_EPHY_EPHYCTL1_PHYADD(ethif->phy_addr);

    /* Disable device, PHY clocks disabled, Enable autonigatiation, Turn on leds.*/
    FNET_MCF_EPHY_EPHYCTL0 = FNET_MCF_EPHY_EPHYCTL0_DIS100 | FNET_MCF_EPHY_EPHYCTL0_DIS10 | FNET_MCF_EPHY_EPHYCTL0_LEDEN ;

    /* Enable EPHY module.*/
    FNET_MCF_EPHY_EPHYCTL0 |= FNET_MCF_EPHY_EPHYCTL0_EPHYEN;

    /* Reset PHY.*/
    fnet_timer_delay(10);
    fnet_fec_mii_write(ethif, FNET_FEC_MII_REG_CR, FNET_ETH_MII_REG_CR_RESET);

    /* SECF128: EPHY Incorrectly Advertises It Can Receive Next Pages
    * Description:The EPHY from reset incorrectly advertises that it can receive next pages
    *    from the link partner. These next pages are most often used to send gigabit
    *    Ethernet ability information between link partners. This device is 10/100 Mbit
    *    only, so there is no need to advertise this capability. In fact if advertised this
    *    additional pages of information must be handled in a special manor not typical
    *    of 10/100 Ethernet drivers.
    * Workaround: The NXTP bit in the auto-negotiate (A/N) advertisement register (4.15) should
    *    be cleared as soon as possible after reset; ideally before enabling
    *    auto-negotiation.
    */
    if (fnet_fec_mii_read(ethif, FNET_FEC_MII_REG_ANAR, &reg_value) == FNET_OK)
    {
        anar_value &= (~ ( FNET_ETH_MII_REG_ANAR_NEXT_PAGE));/* Turn off next page mode.*/
        fnet_fec_mii_write(ethif, FNET_FEC_MII_REG_ANAR, reg_value);
    }

    /* Turns clocks on. Let PHY PLLs be determined by PHY. */
    FNET_MCF_EPHY_EPHYCTL0 &= (~(FNET_MCF_EPHY_EPHYCTL0_DIS100 | FNET_MCF_EPHY_EPHYCTL0_DIS10));
#endif
#endif

#if FNET_CFG_CPU_MCF52259
    fnet_uint8_t tmp_ptipar;
    fnet_uint8_t tmp_ptjpar;

    fnet_timer_delay(3);
    /* Workaround for PHY reset */
    tmp_ptipar = FNET_MCF5225X_GPIO_PTIPAR; /* Save current state */
    tmp_ptjpar = FNET_MCF5225X_GPIO_PTJPAR;
    FNET_MCF5225X_GPIO_PTIPAR = 0x00;		/* Ethernet signals now GPIO*/
    FNET_MCF5225X_GPIO_PTJPAR = 0x00;
    FNET_MCF5225X_GPIO_DDRTI = 0xFF;		/* GPIO output.*/
    FNET_MCF5225X_GPIO_DDRTJ = 0xFF;
    FNET_MCF5225X_GPIO_PORTTI = 0x00;		/* Force Ethernet signals low.*/
    FNET_MCF5225X_GPIO_PORTTJ = 0x00;

    /* Reset PHY.*/
    fnet_fec_mii_write(ethif, FNET_FEC_MII_REG_CR, FNET_FEC_MII_REG_CR_RESET);
    fnet_timer_delay(5);

    /* RSTO signal.*/
    FNET_MCF_RCM_RCR |= FNET_MCF_RCM_RCR_FRCRSTOUT;     /* Assert RSTO.*/
    fnet_timer_delay(5);
    FNET_MCF_RCM_RCR &= ~(FNET_MCF_RCM_RCR_FRCRSTOUT);  /* Negate RSTO.*/

    FNET_MCF5225X_GPIO_PTIPAR = tmp_ptipar;		/* Restore Ethernet signals.*/
    FNET_MCF5225X_GPIO_PTJPAR = tmp_ptjpar;
#endif

    {
        /* Check if the PHY is powered down or isolated, before using it.*/
        fnet_uint16_t reg_value;

        if (fnet_fec_mii_read(ethif, FNET_FEC_MII_REG_CR, &reg_value) == FNET_OK)
        {
            if(reg_value & (FNET_FEC_MII_REG_CR_PDWN | FNET_FEC_MII_REG_CR_ISOL))
            {
                reg_value &= ~(FNET_FEC_MII_REG_CR_PDWN | FNET_FEC_MII_REG_CR_ISOL);
                fnet_fec_mii_write(ethif, FNET_FEC_MII_REG_CR, reg_value);
            }
        }
    }

}


#endif /* FNET_MCF && FNET_CFG_ETH */


