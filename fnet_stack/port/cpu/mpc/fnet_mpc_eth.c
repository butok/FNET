
/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
* Copyright 2011 by Andrey Butok,Gordon Jahn. Freescale Semiconductor, Inc.
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
* @file fnet_mpc_eth.c
*
* @author Andrey Butok
*
* @date Dec-17-2012
*
* @version 0.1.1.0
*
* @brief Ethernet driver interafce.
*
***************************************************************************/


#include "fnet.h"
#if FNET_MPC && (FNET_CFG_CPU_ETH0 ||FNET_CFG_CPU_ETH1)
#include "port/cpu/common/fnet_fec.h"

/************************************************************************
* Ethernet interface structure.
*************************************************************************/
struct fnet_eth_if fnet_mpc_eth0_if =
{
    &fnet_fec0_if
    , 0                            /* MAC module number.*/
    , fnet_fec_output
#if FNET_CFG_MULTICAST
    , fnet_fec_multicast_join
    , fnet_fec_multicast_leave
#endif /* FNET_CFG_MULTICAST */
};

fnet_netif_t fnet_eth0_if =
{
    0,                      /* Pointer to the next net_if structure.*/
    0,                      /* Pointer to the previous net_if structure.*/
    FNET_CFG_CPU_ETH0_NAME, /* Network interface name.*/
    FNET_CFG_CPU_ETH0_MTU,  /* Maximum transmission unit.*/
    &fnet_mpc_eth0_if,      /* Points to interface specific data structure.*/
    &fnet_fec_api           /* Interface API */
};


/************************************************************************
* NAME: fnet_eth_io_init
*
* DESCRIPTION: Ethernet IO initialization.
*************************************************************************/
void fnet_eth_io_init()
{
#if FNET_CFG_CPU_MPC5566  /* Viper */
    /*
     PG[11]  PCR[47] ALT2  RX_CLK      FEC   I
     PH[6]   PCR[58] ALT2  RXD[2]      FEC   I
     PH[5]   PCR[57] ALT2  RXD[1]      FEC   I
     PH[4]   PCR[50] ALT2  RXD[0]      FEC   I
     PH[0]   PCR[52] ALT2  COL         FEC   I
     PH[3]   PCR[49] ALT2  RX_ER       FEC   I
     PG[9]   PCR[45] ALT2  CRS         FEC   I
     PH[7]   PCR[59] ALT2  RXD[3]      FEC   I
     PG[7]   PCR[73] ALT2  MDIO        FEC   I/O
     PH[1]   PCR[53] ALT2  RX_DV       FEC   I
     PG[6]   PCR[72] ALT2  MDC         FEC   O
     PG[8]   PCR[44] ALT2  TX_CLK      FEC   I
     PG[14]  PCR[55] ALT2  TXD[2]      FEC   O
     PG[15]  PCR[51] ALT2  TXD[3]      FEC   O
     PG[13]  PCR[56] ALT2  TXD[1]      FEC   O
     PG[12]  PCR[48] ALT2  TXD[0]      FEC   O
     PH[2]   PCR[54] ALT2  TX_EN       FEC   O
     PG[10]  PCR[46] ALT2  TX_ER       FEC   O
     */
    FNET_MPC_GPIO_PCR(73) = 0xB06;     /* Set to FEC_MDIO   */
    FNET_MPC_GPIO_PCR(72) = 0xA04;     /* Set to FEC_MDC    */
    FNET_MPC_GPIO_PCR(54) = 0xA04;     /* Set to FEC_TX_EN  */
    FNET_MPC_GPIO_PCR(48) = 0xA04;     /* Set to FEC_TXD(0) */
    FNET_MPC_GPIO_PCR(56) = 0xA04;     /* Set to FEC_TXD(1) */
    FNET_MPC_GPIO_PCR(55) = 0xA04;     /* Set to FEC_TXD(2) */
    FNET_MPC_GPIO_PCR(51) = 0xA04;     /* Set to FEC_TXD(3) */
    FNET_MPC_GPIO_PCR(46) = 0xA04;     /* Set to FEC_TX_ER  */
    FNET_MPC_GPIO_PCR(52) = 0x906;     /* Set to FEC_COL    */
    FNET_MPC_GPIO_PCR(44) = 0x906;     /* Set to FEC_TX_CLK */
    FNET_MPC_GPIO_PCR(45) = 0x906;     /* Set to FEC_CRS    */
    FNET_MPC_GPIO_PCR(47) = 0x906;     /* Set to FEC_RX_CLK */
    FNET_MPC_GPIO_PCR(53) = 0x906;     /* Set to FEC_RX_DV  */
    FNET_MPC_GPIO_PCR(50) = 0x906;     /* Set to FEC_RXD(0) */
    FNET_MPC_GPIO_PCR(57) = 0x906;     /* Set to FEC_RXD(1) */
    FNET_MPC_GPIO_PCR(58) = 0x906;     /* Set to FEC_RXD(2) */
    FNET_MPC_GPIO_PCR(59) = 0x906;     /* Set to FEC_RXD(3) */
    FNET_MPC_GPIO_PCR(49) = 0x906;     /* Set to FEC_RX_ER  */
#endif

#if FNET_CFG_CPU_MPC5744P /* Panther */

    /*
    PH[5]    MSCR[117]   COL      FEC I      SRC[1:0]=11    OBE=0    ODE=0    SMC=0    APC=0    IBE=1    HYS=0    PUS=0    PUE=1    INV=0    SSS=0000
    PH[4]    MSCR[116]   CRS      FEC I      SRC[1:0]=11    OBE=0    ODE=0    SMC=0    APC=0    IBE=1    HYS=0    PUS=0    PUE=1    INV=0    SSS=0000
    PF[0]    MSCR[80]    MDC      FEC O      SRC[1:0]=11    OBE=1    ODE=0    SMC=0    APC=0    IBE=0    HYS=0    PUS=0    PUE=1    INV=0    SSS=0011
    PH[7]    MSCR[119]   MDIO     FEC I/O    SRC[1:0]=11    OBE=1    ODE=0    SMC=0    APC=0    IBE=1    HYS=0    PUS=0    PUE=0    INV=0    SSS=0011
    PD[8]    MSCR[56]    RX_CLK   FEC I      SRC[1:0]=11    OBE=0    ODE=0    SMC=0    APC=0    IBE=1    HYS=0    PUS=0    PUE=1    INV=0    SSS=0000
    PD[6]    MSCR[54]    RX_D0    FEC I      SRC[1:0]=11    OBE=0    ODE=0    SMC=0    APC=0    IBE=1    HYS=0    PUS=0    PUE=1    INV=0    SSS=0000
    PD[5]    MSCR[53]    RX_D1    FEC I      SRC[1:0]=11    OBE=0    ODE=0    SMC=0    APC=0    IBE=1    HYS=0    PUS=0    PUE=1    INV=0    SSS=0000
    PH[8]    MSCR[120]   RX_D2    FEC I      SRC[1:0]=11    OBE=0    ODE=0    SMC=0    APC=0    IBE=1    HYS=0    PUS=0    PUE=1    INV=0    SSS=0000
    PJ[9]    MSCR[153]   RX_D3    FEC I      SRC[1:0]=11    OBE=0    ODE=0    SMC=0    APC=0    IBE=1    HYS=0    PUS=0    PUE=1    INV=0    SSS=0000
    PD[7]    MSCR[55]    RX_DV    FEC I      SRC[1:0]=11    OBE=0    ODE=0    SMC=0    APC=0    IBE=1    HYS=0    PUS=0    PUE=1    INV=0    SSS=0000
    PI[1]    MSCR[129]   RX_ER    FEC I      SRC[1:0]=11    OBE=0    ODE=0    SMC=0    APC=0    IBE=1    HYS=0    PUS=0    PUE=1    INV=0    SSS=0000
    PG[8]    MSCR[104]   TX_CLK   FEC I      SRC[1:0]=11    OBE=0    ODE=0    SMC=0    APC=0    IBE=1    HYS=0    PUS=0    PUE=1    INV=0    SSS=0000
    PG[9]    MSCR[105]   TX_D0    FEC O      SRC[1:0]=11    OBE=1    ODE=0    SMC=0    APC=0    IBE=0    HYS=0    PUS=0    PUE=0    INV=0    SSS=0011
    PG[10]   MSCR[106]   TX_D1    FEC O      SRC[1:0]=11    OBE=1    ODE=0    SMC=0    APC=0    IBE=0    HYS=0    PUS=0    PUE=0    INV=0    SSS=0011
    PD[10]   MSCR[58]    TX_D2    FEC O      SRC[1:0]=11    OBE=1    ODE=0    SMC=0    APC=0    IBE=0    HYS=0    PUS=0    PUE=0    INV=0    SSS=0011
    PG[11]   MSCR[107]   TX_D3    FEC O      SRC[1:0]=11    OBE=1    ODE=0    SMC=0    APC=0    IBE=0    HYS=0    PUS=0    PUE=0    INV=0    SSS=0011
    PG[4]    MSCR[101]   TX_EN    FEC O      SRC[1:0]=11    OBE=1    ODE=0    SMC=0    APC=0    IBE=0    HYS=0    PUS=0    PUE=0    INV=0    SSS=0011
    */
    FNET_MPC5744_GPIO_MSCR(117) = 0x30090000;    /* Set to FEC_COL    */
    FNET_MPC5744_GPIO_MSCR(116) = 0x30090000;    /* Set to FEC_CRS    */
    FNET_MPC5744_GPIO_MSCR(80)  = 0x32010003;    /* Set to FEC_MDC    */
    FNET_MPC5744_GPIO_MSCR(119) = 0x32080003;    /* Set to FEC_MDIO   */
    FNET_MPC5744_GPIO_MSCR(56)  = 0x30090000;    /* Set to FEC_RX_CLK */
    FNET_MPC5744_GPIO_MSCR(54)  = 0x30090000;    /* Set to FEC_RX_D0  */
    FNET_MPC5744_GPIO_MSCR(53)  = 0x30090000;    /* Set to FEC_RX_D1  */
    FNET_MPC5744_GPIO_MSCR(120) = 0x30090000;    /* Set to FEC_RX_D2  */
    FNET_MPC5744_GPIO_MSCR(153) = 0x30090000;    /* Set to FEC_RX_D3  */
    FNET_MPC5744_GPIO_MSCR(55)  = 0x30090000;    /* Set to FEC_RX_DV  */
    FNET_MPC5744_GPIO_MSCR(129) = 0x30090000;    /* Set to FEC_RX_ER  */
    FNET_MPC5744_GPIO_MSCR(104) = 0x30090000;    /* Set to FEC_TX_CLK */
    FNET_MPC5744_GPIO_MSCR(105) = 0x32000003;    /* Set to FEC_TX_D0  */
    FNET_MPC5744_GPIO_MSCR(106) = 0x32000003;    /* Set to FEC_TX_D1  */
    FNET_MPC5744_GPIO_MSCR(58)  = 0x32000003;    /* Set to FEC_TX_D2  */
    FNET_MPC5744_GPIO_MSCR(107) = 0x32000003;    /* Set to FEC_TX_D3  */
    FNET_MPC5744_GPIO_MSCR(101) = 0x32000003;    /* Set to FEC_TX_EN  */


    FNET_MPC5744_GPIO_IMCR(230) = 0x00000001;
    FNET_MPC5744_GPIO_IMCR(231) = 0x00000001;
    FNET_MPC5744_GPIO_IMCR(224) = 0x00000001;
    FNET_MPC5744_GPIO_IMCR(226) = 0x00000001;
    FNET_MPC5744_GPIO_IMCR(227) = 0x00000001;
    FNET_MPC5744_GPIO_IMCR(228) = 0x00000001;
    FNET_MPC5744_GPIO_IMCR(225) = 0x00000001;
    FNET_MPC5744_GPIO_IMCR(229) = 0x00000001;
    FNET_MPC5744_GPIO_IMCR(232) = 0x00000001;
    FNET_MPC5744_GPIO_IMCR(233) = 0x00000001;
#endif


#if FNET_CFG_CPU_MPC5668G /* FADO */
    /*
     PG[11]	PCR[107]	ALT2	RX_CLK		FEC	I
     PH[6]	PCR[118]	ALT2	RXD[2]		FEC	I
     PH[5]	PCR[117]	ALT2	RXD[1]		FEC	I
     PH[4]	PCR[116]	ALT2	RXD[0]		FEC	I
     PH[0]	PCR[112]	ALT2	COL			FEC	I
     PH[3]	PCR[115]	ALT2	RX_ER		FEC	I
     PG[9]	PCR[105]	ALT2	CRS			FEC	I
     PH[7]	PCR[119]	ALT2	RXD[3]		FEC	I
     PG[7]	PCR[103]	ALT2	MDIO		FEC	I/O
     PH[1]	PCR[113]	ALT2	RX_DV		FEC	I
     PG[6]	PCR[102]	ALT2	MDC			FEC	O
     PG[8]	PCR[104]	ALT2	TX_CLK		FEC	I
     PG[14]	PCR[110]	ALT2	TXD[2]		FEC	O
     PG[15]	PCR[111]	ALT2	TXD[3]		FEC	O
     PG[13]	PCR[109]	ALT2	TXD[1]		FEC	O
     PG[12]	PCR[108]	ALT2	TXD[0]		FEC	O
     PH[2]	PCR[114]	ALT2	TX_EN		FEC	O
     PG[10]	PCR[106]	ALT2	TX_ER		FEC	O
     */
    FNET_MPC_GPIO_PCR(103) = 0xB06;     /* Set to FEC_MDIO	  */
    FNET_MPC_GPIO_PCR(102) = 0xA04;     /* Set to FEC_MDC 	  */
    FNET_MPC_GPIO_PCR(114) = 0xA04;     /* Set to FEC_TX_EN  */
    FNET_MPC_GPIO_PCR(108) = 0xA04;     /* Set to FEC_TXD(0)	*/
    FNET_MPC_GPIO_PCR(109) = 0xA04;     /* Set to FEC_TXD(1) */
    FNET_MPC_GPIO_PCR(110) = 0xA04;     /* Set to FEC_TXD(2) */
    FNET_MPC_GPIO_PCR(111) = 0xA04;     /* Set to FEC_TXD(3) */
    FNET_MPC_GPIO_PCR(106) = 0xA04 ;    /* Set to FEC_TX_ER  */
    FNET_MPC_GPIO_PCR(112) = 0x906;     /* Set to FEC_COL	  */
    FNET_MPC_GPIO_PCR(104) = 0x906;     /* Set to FEC_TX_CLK */
    FNET_MPC_GPIO_PCR(105) = 0x906;     /* Set to FEC_CRS	  */
    FNET_MPC_GPIO_PCR(107) = 0x906;     /* Set to FEC_RX_CLK */
    FNET_MPC_GPIO_PCR(113) = 0x906;     /* Set to FEC_RX_DV  */
    FNET_MPC_GPIO_PCR(116) = 0x906;     /* Set to FEC_RXD(0) */
    FNET_MPC_GPIO_PCR(117) = 0x906;     /* Set to FEC_RXD(1) */
    FNET_MPC_GPIO_PCR(118) = 0x906;     /* Set to FEC_RXD(2) */
    FNET_MPC_GPIO_PCR(119) = 0x906;     /* Set to FEC_RXD(3) */
    FNET_MPC_GPIO_PCR(115) = 0x906;     /* Set to FEC_RX_ER  */
#endif

#if FNET_CFG_CPU_MPC564xBC /* Bolero3M */

    /*
    PA[3]	PCR[3]				RX_CLK	FEC	I
    PA[7]	PCR[7]				RXD[2]		FEC	I
    PA[8]	PCR[8]				RXD[1]		FEC	I
    PA[9]	PCR[9]				RXD[0]		FEC	I
    PA[10]	PCR[10]				COL			FEC	I
    PA[11]	PCR[11]				RX_ER		FEC	I
    PE[12]	PCR[76]				CRS			FEC	I
    PE[13]	PCR[77]				RXD[3]		FEC	I
    PF[14]	PCR[94]		ALT4	MDIO		FEC	I/O
    PF[15]	PCR[95]				RX_DV		FEC	I
    PG[0]	PCR[96]		ALT4	MDC			FEC	O
    PG[1]	PCR[97]				TX_CLK		FEC	I
    PG[12]	PCR[108]	ALT4	TXD[2]		FEC	O
    PG[13]	PCR[109]	ALT4	TXD[3]		FEC	O
    PH[0]	PCR[112]	ALT4	TXD[1]		FEC	O
    PH[1]	PCR[113]	ALT4	TXD[0]		FEC	O
    PH[2)	PCR[114)	ALT4	TX_EN		FEC	O
    PH(3)	PCR(115)	ALT4	TX_ER		FEC	O
    */
    FNET_MPC_GPIO_PCR(94) = 0x1306;     /* Set to FEC_MDIO	  */
    FNET_MPC_GPIO_PCR(96) = 0x1202;     /* Set to FEC_MDC 	  */
    FNET_MPC_GPIO_PCR(114) = 0x1204;    /* Set to FEC_TX_EN  */
    FNET_MPC_GPIO_PCR(113) = 0x1204;    /* Set to FEC_TXD(0)	*/
    FNET_MPC_GPIO_PCR(112) = 0x1204;    /* Set to FEC_TXD(1) */
    FNET_MPC_GPIO_PCR(108) = 0x1204;    /* Set to FEC_TXD(2) */
    FNET_MPC_GPIO_PCR(109) = 0x1204;    /* Set to FEC_TXD(3) */
    FNET_MPC_GPIO_PCR(115) = 0x1204;    /* Set to FEC_TX_ER  */
    FNET_MPC_GPIO_PCR(10) = 0x106;      /* Set to FEC_COL	  */
    FNET_MPC_GPIO_PCR(97) = 0x102;      /* Set to FEC_TX_CLK */
    FNET_MPC_GPIO_PCR(76) = 0x106;      /* Set to FEC_CRS	  */
    FNET_MPC_GPIO_PCR(3) = 0x106;       /* Set to FEC_RX_CLK */
    FNET_MPC_GPIO_PCR(95) = 0x102;      /* Set to FEC_RX_DV  */
    FNET_MPC_GPIO_PCR(9) = 0x106;       /* Set to FEC_RXD(0) */
    FNET_MPC_GPIO_PCR(8) = 0x106;       /* Set to FEC_RXD(1) */
    FNET_MPC_GPIO_PCR(7) = 0x106;       /* Set to FEC_RXD(2) */
    FNET_MPC_GPIO_PCR(77) = 0x106;      /* Set to FEC_RXD(3) */
    FNET_MPC_GPIO_PCR(11) = 0x106;      /* Set to FEC_RX_ER  */
#endif
}

/************************************************************************
* NAME: fnet_eth_phy_init
*
* DESCRIPTION: Ethernet Physical Transceiver initialization and/or reset.
*************************************************************************/
void fnet_eth_phy_init(fnet_fec_if_t *ethif)
{
    fnet_uint16_t reg_value;
    fnet_uint16_t status_value = 0;

    fnet_fec_mii_read(ethif, FNET_FEC_MII_REG_CR, &reg_value);

    /* ANE ENABLED:*/
    fnet_fec_mii_write(ethif, FNET_FEC_MII_REG_CR, (fnet_uint16_t)(reg_value | FNET_FEC_MII_REG_CR_ANE | FNET_FEC_MII_REG_CR_ANE_RESTART));

    while (status_value != 0x0040)
    {
        fnet_fec_mii_read(ethif, FNET_FEC_MII_REG_SR, &status_value);
        status_value &= 0x0040;
    }
}

#endif /* FNET_MPC && FNET_CFG_ETH */


