/**************************************************************************
*
* Copyright 2011-2017 by Andrey Butok. FNET Community.
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
#if FNET_MK && FNET_CFG_CPU_ETH0

#include "port/netif/fec/fnet_fec.h"

/************************************************************************
* Ethernet interface structure.
*************************************************************************/
static fnet_eth_if_t fnet_mk_eth0_if =
{
    .eth_prv = &fnet_fec0_if,                       /* Points to Ethernet driver-specific control data structure. */
    .eth_mac_number = 0,                            /* MAC module number. */
    .eth_output = fnet_fec_output,                  /* Ethernet driver output.*/
#if FNET_CFG_MULTICAST
    .eth_multicast_join = fnet_fec_multicast_join,  /* Ethernet driver join multicast group.*/
    .eth_multicast_leave = fnet_fec_multicast_leave /* Ethernet driver leave multicast group.*/
#endif
};

fnet_netif_t fnet_cpu_eth0_if =
{
    .netif_name = FNET_CFG_CPU_ETH0_NAME,     /* Network interface name.*/
    .netif_mtu = FNET_CFG_CPU_ETH0_MTU,       /* Maximum transmission unit.*/
    .netif_prv = &fnet_mk_eth0_if,            /* Points to interface specific data structure.*/
    .netif_api = &fnet_fec_api                /* Interface API */
};

/************************************************************************
* DESCRIPTION: Ethernet IO initialization.
*************************************************************************/
#if FNET_CFG_CPU_ETH_IO_INIT
void fnet_eth_io_init(void)
{
    FNET_MK_PORT_MemMapPtr pctl;
    FNET_MK_SIM_MemMapPtr  sim  = (FNET_MK_SIM_MemMapPtr)FNET_MK_SIM_BASE_PTR;

#if FNET_CFG_CPU_MK64FN1

    /* Enable clock */
    FNET_MK_SIM_SCGC5 |= FNET_MK_SIM_SCGC5_PORTC_MASK | FNET_MK_SIM_SCGC5_PORTA_MASK | FNET_MK_SIM_SCGC5_PORTB_MASK;

    pctl = (FNET_MK_PORT_MemMapPtr)FNET_MK_PORTC_BASE_PTR;
#if 0 /* PTC16-17 may be used also as UART pins */
    pctl->PCR[16] = 0x00000400u;
    pctl->PCR[17] = 0x00000400u;
#endif
    pctl->PCR[18] = 0x00000400u;
    pctl->PCR[19] = 0x00000400u;


    pctl = (FNET_MK_PORT_MemMapPtr)FNET_MK_PORTA_BASE_PTR;
    pctl->PCR[12] = 0x00000400u;     /* PTA12, RMII0_RXD1/MII0_RXD1      */
    pctl->PCR[13] = 0x00000400u;     /* PTA13, RMII0_RXD0/MII0_RXD0      */
    pctl->PCR[14] = 0x00000400u;     /* PTA14, RMII0_CRS_DV/MII0_RXDV    */
    pctl->PCR[15] = 0x00000400u;     /* PTA15, RMII0_TXEN/MII0_TXEN      */
    pctl->PCR[16] = 0x00000400u;     /* PTA16, RMII0_TXD0/MII0_TXD0      */
    pctl->PCR[17] = 0x00000400u;     /* PTA17, RMII0_TXD1/MII0_TXD1      */
    pctl->PCR[5] = 0x00000400u;
    pctl->PCR[28] = 0x00000400u;

    pctl = (FNET_MK_PORT_MemMapPtr)FNET_MK_PORTB_BASE_PTR;
    pctl->PCR[0] = FNET_MK_PORT_PCR_MUX(4) | FNET_MK_PORT_PCR_ODE_MASK | FNET_MK_PORT_PCR_PS_MASK | FNET_MK_PORT_PCR_PE_MASK; /* PTB0, RMII0_MDIO/MII0_MDIO   */
    pctl->PCR[1] = FNET_MK_PORT_PCR_MUX(4);                     /* PTB1, RMII0_MDC/MII0_MDC     */

#elif FNET_CFG_CPU_MK66FN2

    /* Enable clock */
    FNET_MK_SIM_SCGC5 |= FNET_MK_SIM_SCGC5_PORTE_MASK | FNET_MK_SIM_SCGC5_PORTA_MASK | FNET_MK_SIM_SCGC5_PORTB_MASK;

    pctl = (FNET_MK_PORT_MemMapPtr)FNET_MK_PORTE_BASE_PTR;
    pctl->PCR[26] = 0x00000200u;

    pctl = (FNET_MK_PORT_MemMapPtr)FNET_MK_PORTA_BASE_PTR;
    pctl->PCR[5] = 0x00000400u;
    pctl->PCR[12] = 0x00000400u;     /* PTA12, RMII0_RXD1/MII0_RXD1      */
    pctl->PCR[13] = 0x00000400u;     /* PTA13, RMII0_RXD0/MII0_RXD0      */
    pctl->PCR[14] = 0x00000400u;     /* PTA14, RMII0_CRS_DV/MII0_RXDV    */
    pctl->PCR[15] = 0x00000400u;     /* PTA15, RMII0_TXEN/MII0_TXEN      */
    pctl->PCR[16] = 0x00000400u;     /* PTA16, RMII0_TXD0/MII0_TXD0      */
    pctl->PCR[17] = 0x00000400u;     /* PTA17, RMII0_TXD1/MII0_TXD1      */
    pctl->PCR[28] = 0x00000400u;

    pctl = (FNET_MK_PORT_MemMapPtr)FNET_MK_PORTB_BASE_PTR;
    pctl->PCR[0] = FNET_MK_PORT_PCR_MUX(4) | FNET_MK_PORT_PCR_ODE_MASK | FNET_MK_PORT_PCR_PS_MASK | FNET_MK_PORT_PCR_PE_MASK; /* PTB0, RMII0_MDIO/MII0_MDIO   */
    pctl->PCR[1] = FNET_MK_PORT_PCR_MUX(4);                     /* PTB1, RMII0_MDC/MII0_MDC     */

    /* Set RMII clock src. */
    sim->SOPT2 |= FNET_MK_SIM_SOPT2_RMIISRC(1);

#else /* TWR */
    /* Enable clock */
    FNET_MK_SIM_SCGC5 |= FNET_MK_SIM_SCGC5_PORTA_MASK | FNET_MK_SIM_SCGC5_PORTB_MASK;

    pctl = (FNET_MK_PORT_MemMapPtr)FNET_MK_PORTA_BASE_PTR;
    pctl->PCR[12] = 0x00000400u;     /* PTA12, RMII0_RXD1/MII0_RXD1      */
    pctl->PCR[13] = 0x00000400u;     /* PTA13, RMII0_RXD0/MII0_RXD0      */
    pctl->PCR[14] = 0x00000400u;     /* PTA14, RMII0_CRS_DV/MII0_RXDV    */
    pctl->PCR[15] = 0x00000400u;     /* PTA15, RMII0_TXEN/MII0_TXEN      */
    pctl->PCR[16] = 0x00000400u;     /* PTA16, RMII0_TXD0/MII0_TXD0      */
    pctl->PCR[17] = 0x00000400u;     /* PTA17, RMII0_TXD1/MII0_TXD1      */

    pctl = (FNET_MK_PORT_MemMapPtr)FNET_MK_PORTB_BASE_PTR;
    pctl->PCR[0] = FNET_MK_PORT_PCR_MUX(4) | FNET_MK_PORT_PCR_ODE_MASK; /* PTB0, RMII0_MDIO/MII0_MDIO   */
    pctl->PCR[1] = FNET_MK_PORT_PCR_MUX(4);                     /* PTB1, RMII0_MDC/MII0_MDC     */

#endif

    /* Enable clock for ENET module */
    sim->SCGC2 |= FNET_MK_SIM_SCGC2_ENET_MASK;

    /*Allow concurrent access to MPU controller. Example: ENET uDMA to SRAM, otherwise bus error*/
    FNET_MK_MPU_CESR = 0u;  /* MPU is disabled. All accesses from all bus masters are allowed.*/
}
#endif /*!FNET_CFG_CPU_ETH_IO_INIT*/

/************************************************************************
* DESCRIPTION: Ethernet Physical Transceiver initialization and/or reset.
*************************************************************************/
void fnet_eth_phy_init(fnet_fec_if_t *ethif)
{
    FNET_COMP_UNUSED_ARG(ethif);
}

/* If vector table is in ROM, pre-install FNET ISR for ENET Receive Frame interrupt*/
#if !FNET_CFG_CPU_VECTOR_TABLE_IS_IN_RAM
void ENET_Receive_IRQHandler (void)
{
    FNET_ISR_HANDLER();
}
#endif

#endif /* FNET_MK && FNET_CFG_ETH */
