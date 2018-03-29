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
*  LPC ENET module driver implementation.
*  This is a prototype wrapper for LPC MCUX driver.
*
***************************************************************************/

#include "fnet.h"
#if (FNET_LPC && FNET_CFG_CPU_ETH0)

#if FNET_CFG_DEBUG_ENET && FNET_CFG_DEBUG
    #define FNET_DEBUG_ENET   FNET_DEBUG_PRINTF
#else
    #define FNET_DEBUG_ENET(...)   do{}while(0)
#endif

#include "fnet_enet.h"

/************************************************************************
*     Function Prototypes
*************************************************************************/
static fnet_return_t fnet_enet_init(fnet_netif_t *netif);
static void fnet_enet_release(fnet_netif_t *netif);
static fnet_return_t fnet_enet_get_hw_addr(fnet_netif_t *netif, fnet_uint8_t *hw_addr);
static fnet_return_t fnet_enet_set_hw_addr(fnet_netif_t *netif, fnet_uint8_t *hw_addr);
static fnet_bool_t fnet_enet_is_connected(fnet_netif_t *netif);
static fnet_return_t fnet_enet_get_statistics(fnet_netif_t *netif, struct fnet_netif_statistics *statistics);
static fnet_return_t fnet_enet_phy_init(fnet_enet_if_t *enet_if);
static void fnet_enet_rx_buf_next( fnet_enet_if_t  *enet_if);
#if FNET_CFG_CPU_ETH_PHY_ADDR_DISCOVER
    static void fnet_enet_phy_discover_addr (fnet_enet_if_t *enet_if, fnet_uint8_t phy_addr_start);
#endif

static void fnet_enet_callback(ENET_Type *base, enet_handle_t *handle, enet_event_t event, uint8_t channel, void *param);

/* ENET interrup handler. */
static void fnet_enet_isr_handler_top(void *cookie);
static void fnet_enet_isr_handler_bottom(void *cookie);

static void fnet_enet_get_mac_addr(fnet_enet_if_t *enet_if, fnet_mac_addr_t *mac_addr);

/*****************************************************************************
 * ENET network-interface general API structure.
 ******************************************************************************/
const fnet_netif_api_t fnet_enet_api =
{
    .netif_type = FNET_NETIF_TYPE_ETHERNET,                     /* Data-link type. */
    .netif_hw_addr_size = sizeof(fnet_mac_addr_t),
    .netif_init = fnet_enet_init,                               /* Initialization function.*/
    .netif_release = fnet_enet_release,                         /* Shutdown function.*/
#if FNET_CFG_IP4
    .netif_output_ip4 = fnet_eth_output_ip4,                    /* IPv4 Transmit function.*/
#endif
    .netif_change_addr_notify = fnet_eth_change_addr_notify,    /* Address change notification function.*/
    .netif_drain = fnet_eth_drain,                              /* Drain function.*/
    .netif_get_hw_addr = fnet_enet_get_hw_addr,
    .netif_set_hw_addr = fnet_enet_set_hw_addr,
    .netif_is_connected = fnet_enet_is_connected,
    .netif_get_statistics = fnet_enet_get_statistics
#if FNET_CFG_MULTICAST
#if FNET_CFG_IP4
    , .netif_multicast_join_ip4 = fnet_eth_multicast_join_ip4
    , .netif_multicast_leave_ip4 = fnet_eth_multicast_leave_ip4
#endif
#if FNET_CFG_IP6
    , .netif_multicast_join_ip6 = fnet_eth_multicast_join_ip6
    , .netif_multicast_leave_ip6 = fnet_eth_multicast_leave_ip6
#endif
#endif
#if FNET_CFG_IP6
    , .netif_output_ip6 = fnet_eth_output_ip6                   /* IPv6 Transmit function.*/
#endif
};


//TBD move to the structure
#define FNET_ENET_BUF_SIZE           (((FNET_CFG_CPU_ETH0_MTU>FNET_CFG_CPU_ETH1_MTU)?FNET_CFG_CPU_ETH0_MTU:FNET_CFG_CPU_ETH1_MTU)+FNET_ETH_HDR_SIZE+FNET_ETH_CRC_SIZE) /* Ring Buffer size in bytes.*/

typedef uint8_t fnet_enet_buffer_t[SDK_SIZEALIGN(FNET_ENET_BUF_SIZE, ENET_BUFF_ALIGNMENT)];
AT_NONCACHEABLE_SECTION_ALIGN(static enet_rx_bd_struct_t rxBuffDescrip[FNET_CFG_CPU_ETH_RX_BUFS_MAX], ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(static enet_tx_bd_struct_t txBuffDescrip[FNET_CFG_CPU_ETH_TX_BUFS_MAX], ENET_BUFF_ALIGNMENT);
SDK_ALIGN(static fnet_enet_buffer_t rxDataBuff[FNET_CFG_CPU_ETH_RX_BUFS_MAX], ENET_BUFF_ALIGNMENT);
SDK_ALIGN(static fnet_enet_buffer_t txDataBuff[FNET_CFG_CPU_ETH_TX_BUFS_MAX], ENET_BUFF_ALIGNMENT);

/************************************************************************
*     Global Data Structures
*************************************************************************/

fnet_enet_if_t fnet_enet0_if = {.base = ENET, /* Peripheral ENET base pointer */
                                .vector_number = FNET_CFG_CPU_ETH0_VECTOR_NUMBER,
                                .phy_addr = FNET_CFG_CPU_ETH0_PHY_ADDR
                               };

/************************************************************************
* DESCRIPTION: ENET Ethernet module initialization.
*************************************************************************/
static fnet_return_t fnet_enet_init(fnet_netif_t *netif)
{
    fnet_return_t           result = FNET_ERR;
    fnet_enet_if_t          *enet_if;
    enet_config_t           enet_config;        /* ENET driver configuration */
    enet_buffer_config_t    enet_buffer_config; /* ENET driver buffer configuration */
    uint32_t                rxBufferStartAddr[FNET_CFG_CPU_ETH_RX_BUFS_MAX]; /* Start addresses of the rx buffers. */
    fnet_index_t            i;

    enet_if = (fnet_enet_if_t *)(((fnet_eth_if_t *)(netif->netif_prv))->eth_prv);

    /* Clear statistics. */
    fnet_memset_zero(&enet_if->statistics, sizeof(struct fnet_netif_statistics));

#if FNET_CFG_CPU_ETH_IO_INIT
    fnet_eth_io_init();
#endif
    /* Clear descriptors */
    fnet_memset_zero(rxBuffDescrip, sizeof(rxBuffDescrip));
    fnet_memset_zero(txBuffDescrip, sizeof(rxBuffDescrip));
    /* Phy initialization */
    if (fnet_enet_phy_init(enet_if) == FNET_OK)
    {
        /* Init start address of the rx buffers.*/
        for (i = 0; i < FNET_CFG_CPU_ETH_RX_BUFS_MAX; i++)
        {
            rxBufferStartAddr[i] = (uint32_t) & (rxDataBuff[i][0]);
        }

        /* Init buffer descriptor configure structure. */
        enet_buffer_config.rxRingLen = FNET_CFG_CPU_ETH_RX_BUFS_MAX;           /* The length of receive buffer descriptor ring. */
        enet_buffer_config.txRingLen = FNET_CFG_CPU_ETH_TX_BUFS_MAX;           /* The length of transmit buffer descriptor ring. */
        enet_buffer_config.txDescStartAddrAlign = &txBuffDescrip[0]; /* Aligned transmit descriptor start address. */
        enet_buffer_config.txDescTailAddrAlign = &txBuffDescrip[0];  /* Aligned transmit descriptor tail address. */
        enet_buffer_config.rxDescStartAddrAlign = &rxBuffDescrip[0]; /* Aligned receive descriptor start address. */
        enet_buffer_config.rxDescTailAddrAlign = &rxBuffDescrip[FNET_CFG_CPU_ETH_RX_BUFS_MAX]; /* Aligned receive descriptor tail address. */
        enet_buffer_config.rxBufferStartAddr = rxBufferStartAddr;              /* Start addresses of the rx buffers. */
        enet_buffer_config.rxBuffSizeAlign = sizeof(fnet_enet_buffer_t);       /* Aligned receive data buffer size. */

        ENET_GetDefaultConfig(&enet_config);

#if FNET_CFG_CPU_ETH_PROMISCUOUS
        enet_config.specialControl |= kENET_PromiscuousEnable; /* Receive all */
#else
        enet_config.specialControl |= kENET_MulticastAllEnable; /* Pass all multicast. */
#endif

        enet_if->txIdx = 0;

        /* Install ENET interrupt handler.*/
        result = fnet_isr_vector_init(enet_if->vector_number, fnet_enet_isr_handler_top, fnet_enet_isr_handler_bottom, FNET_CFG_CPU_ETH_VECTOR_PRIORITY, netif);

        /* Initializes the ENET module. */
        ENET_Init(enet_if->base, &enet_config, FNET_NULL, kCLOCK_CoreSysClk);

        /* Enable ENET interrupt */
        ENET_EnableInterrupts(enet_if->base, kENET_DmaRx | kENET_DmaTx);
        /* Create handler. */
        ENET_CreateHandler(enet_if->base, &enet_if->handle, &enet_config, &enet_buffer_config, fnet_enet_callback, 0);

        /* Initialize for all ENET descriptors.
         * the descriptor initialization should be called after ENET_Init(), ENET_EnableInterrupts()*/
        if(ENET_DescriptorInit(enet_if->base, &enet_config, &enet_buffer_config) != kStatus_Success)
        {
            FNET_DEBUG_ENET("[ENET] Descriptor init is failed");
        }
        else
        {
            /* Starts the ENET rx/tx. */
            ENET_StartRxTx(enet_if->base, 1, 1);

            result = FNET_OK;
        }
    }
    else
    {
        FNET_DEBUG_ENET("[ENET] PHY init is failed");
    }

    return result;
}

static void fnet_enet_callback(ENET_Type *base, enet_handle_t *handle, enet_event_t event, uint8_t channel, void *param)
{
    /*DUMB function to avoid assert in SDK ENET driver */
}

/************************************************************************
* DESCRIPTION: ENET Ethernet module release.
*************************************************************************/
static void fnet_enet_release(fnet_netif_t *netif)
{
    fnet_enet_if_t *enet_if = (fnet_enet_if_t *)((fnet_eth_if_t *)(netif->netif_prv))->eth_prv;

    ENET_Deinit(enet_if->base);

    fnet_isr_unregister(enet_if->vector_number);

    fnet_eth_release(netif); /* Common Ethernet-interface release.*/
}

/************************************************************************
* DESCRIPTION: Ethernet input function.
*************************************************************************/
static void fnet_enet_input(fnet_netif_t *netif)
{
    fnet_enet_if_t      *enet_if = (fnet_enet_if_t *)((fnet_eth_if_t *)(netif->netif_prv))->eth_prv;
    uint32_t            length = 0;
    fnet_uint8_t        *frame;
    enet_handle_t       *handle = &enet_if->handle;

    enet_rx_bd_ring_t   *rxBdRing = &handle->rxBdRing[0];
    enet_rx_bd_struct_t *rxDesc = rxBdRing->rxBdBase + rxBdRing->rxGenIdx; /* Get current RX descriptor */

#if FNET_CFG_CPU_CACHE_INVALIDATE
    fnet_cpu_cache_invalidate();
#endif

    /* While buffer NOT(empty or rx in progress)*/
    while((rxDesc->control & ENET_RXDESCRIP_WR_OWN_MASK) == 0u)
    {
        /* If NOT(buffer is last in the frame) */
        if ((rxDesc->control & ENET_RXDESCRIP_WR_LD_MASK) == 0u)
        {
            /* Skip the splitted frame. */
            do /* Keep going until we find the last one. */
            {
                fnet_enet_rx_buf_next(enet_if);
                rxDesc = rxBdRing->rxBdBase + rxBdRing->rxGenIdx;
            }
            while ((rxDesc->control & ENET_RXDESCRIP_WR_LD_MASK) == 0u);
        }
        else
        {
            if (rxDesc->control & ENET_RXDESCRIP_WR_ERRSUM_MASK) /* Any Error. */
            {
                /* Drop packet */
                goto NEXT_FRAME;
            }

            length = rxDesc->control & ENET_RXDESCRIP_WR_PACKETLEN_MASK;
            frame = (void *)rxDesc->buff1Addr;

            /* Ethernet input.*/
            fnet_eth_input( netif, frame, length);
        }

    NEXT_FRAME:
        fnet_enet_rx_buf_next(enet_if);
        rxDesc = rxBdRing->rxBdBase + rxBdRing->rxGenIdx;
    }
}

/************************************************************************
* DESCRIPTION: Goes not the next Rx buffer.
*************************************************************************/
static void fnet_enet_rx_buf_next( fnet_enet_if_t  *enet_if)
{
    ENET_Type           *base = enet_if->base;
    enet_handle_t       *handle = &enet_if->handle;

    enet_rx_bd_ring_t   *rxBdRing = &handle->rxBdRing[0];
    enet_rx_bd_struct_t *rxDesc = rxBdRing->rxBdBase + rxBdRing->rxGenIdx; /* Get current RX descriptor */

    /* Increment current available receive buffer descriptor pointer to the next one. */
    rxBdRing->rxGenIdx++;
    if (rxBdRing->rxGenIdx >= rxBdRing->rxRingLen)
    {
        rxBdRing->rxGenIdx = 0;
    }
    /* Updates the receive buffer descriptor. */
    rxDesc->control =  ENET_RXDESCRIP_RD_OWN_MASK |            /* Own bit -  Ethernet block's DMA owns the descriptor */
                       ENET_RXDESCRIP_RD_BUFF1VALID_MASK |     /* Buffer 1 address valid */
                       ENET_RXDESCRIP_RD_IOC_MASK;             /* Interrupt enabled on completion */
    rxDesc->buff2Addr = 0; /* Just in case. We support only Buffer 1. */

    /* The Rx process is suspended. To resume processing Rx data, the application
    should change the ownership and issue a receive poll demand command. If this command is
    not issued, the Rx process resumes when the next recognized incoming packet is received.
    In ring mode, the application should advance the receive tail pointer register of a channel.*/
    if (base->DMA_CH[0].DMA_CHX_STAT & ENET_DMA_CH_DMA_CHX_STAT_RBU_MASK)
    {
        /* Set command for rx. */
        base->DMA_CH[0].DMA_CHX_RXDESC_TAIL_PTR = base->DMA_CH[0].DMA_CHX_RXDESC_TAIL_PTR;
    }
}

/************************************************************************
* DESCRIPTION: Ethernet low-level output function.
*************************************************************************/
void fnet_enet_output(fnet_netif_t *netif, fnet_netbuf_t *nb)
{
    fnet_enet_if_t      *enet_if = (fnet_enet_if_t *)((fnet_eth_if_t *)(netif->netif_prv))->eth_prv;
    fnet_uint8_t        *tx_buffer;

    if(nb && (nb->total_length >= FNET_ETH_HDR_SIZE))
    {
        /* Buffer descriptors will be updated by DMA, so we need to invalidate cache */
        fnet_cpu_cache_invalidate();

        /* Wait for a free TX descriptor */
        while(ENET_IsTxDescriptorDmaOwn(&txBuffDescrip[enet_if->txIdx]))
        {} /* If you are blocked here forever, there is a communication issue between ENET and PHY.*/

        tx_buffer = txDataBuff[enet_if->txIdx];
        enet_if->txIdx = (enet_if->txIdx + 1U) % FNET_CFG_CPU_ETH_TX_BUFS_MAX;

        fnet_netbuf_to_buf(nb, 0u, FNET_NETBUF_COPYALL, tx_buffer);

        if (ENET_SendFrame(enet_if->base, &enet_if->handle, tx_buffer, nb->total_length) != kStatus_Success)
        {
            FNET_DEBUG_ENET("[ENET] TX is failed");
        }

        enet_if->statistics.tx_packet++;
    }

    fnet_netbuf_free_chain(nb);
}

/************************************************************************
* DESCRIPTION: This function polls ENET driver.
* !!!! Used only for debug needs. !!!!!
*************************************************************************/
void fnet_enet_poll(fnet_netif_desc_t netif_desc)
{
    fnet_netif_t    *netif = (fnet_netif_t *)netif_desc;

    fnet_isr_lock();

    fnet_enet_input(netif);

    fnet_isr_unlock();
}

/************************************************************************
* DESCRIPTION: Sets MAC address.
*************************************************************************/
static fnet_return_t fnet_enet_set_hw_addr(fnet_netif_t *netif, fnet_uint8_t *hw_addr)
{
    fnet_enet_if_t  *enet_if;
    fnet_return_t   result;

    /* Set the source address for the controller. */
    if(netif
       && (netif->netif_api->netif_type == FNET_NETIF_TYPE_ETHERNET)
       && ((enet_if = (fnet_enet_if_t *)((fnet_eth_if_t *)(netif->netif_prv))->eth_prv) != 0)
       && hw_addr
       && (fnet_memcmp(hw_addr, fnet_eth_null_addr, sizeof(fnet_mac_addr_t)))
       && (fnet_memcmp(hw_addr, fnet_eth_broadcast, sizeof(fnet_mac_addr_t)))
       && ((hw_addr[0] & 0x01U) == 0x00U)) /* Most significant nibble should be always even.*/
    {
        /* Set Macaddr */
        ENET_SetMacAddr(enet_if->base, hw_addr);

        fnet_eth_change_addr_notify(netif);

        result = FNET_OK;
    }
    else
    {
        result = FNET_ERR;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: This function reads HW address.
*************************************************************************/
static fnet_return_t fnet_enet_get_hw_addr(fnet_netif_t *netif, fnet_uint8_t *hw_addr)
{
    fnet_enet_if_t  *enet_if;
    fnet_return_t   result;

    if(netif && (netif->netif_api->netif_type == FNET_NETIF_TYPE_ETHERNET)
       && ((enet_if = (fnet_enet_if_t *)((fnet_eth_if_t *)(netif->netif_prv))->eth_prv) != FNET_NULL)
       && (hw_addr) )
    {
        fnet_enet_get_mac_addr(enet_if, (fnet_mac_addr_t *) hw_addr);
        result = FNET_OK;
    }
    else
    {
        result = FNET_ERR;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: This function reads MAC address.
*************************************************************************/
static void fnet_enet_get_mac_addr(fnet_enet_if_t *enet_if, fnet_mac_addr_t *mac_addr)
{
#if 0 /* Driver function returns swapped/wrong address */
    ENET_GetMacAddr(enet_if->base, (uint8_t *)mac_addr);
#else
    fnet_uint32_t   address;
    uint8_t         *hw_addr = (uint8_t *)mac_addr;

    /* Physical address lower register. */
    address = enet_if->base->MAC_ADDR_LOW;
    hw_addr[3] = 0xFFU & (address >> 24U);
    hw_addr[2] = 0xFFU & (address >> 16U);
    hw_addr[1] = 0xFFU & (address >> 8U);
    hw_addr[0] = 0xFFU & address;

    /* Physical address high register. */
    address = enet_if->base->MAC_ADDR_HIGH;
    hw_addr[5] = 0xFFU & (address >> 8U);
    hw_addr[4] = 0xFFU & address;
#endif
}

/************************************************************************
* DESCRIPTION: Returns Ethernet statistics information
*************************************************************************/
static fnet_return_t fnet_enet_get_statistics(fnet_netif_t *netif, struct fnet_netif_statistics *statistics)
{
    fnet_enet_if_t  *enet_if;
    fnet_return_t   result = FNET_ERR;

    if(netif && (netif->netif_api->netif_type == FNET_NETIF_TYPE_ETHERNET))
    {
        enet_if = (fnet_enet_if_t *)((fnet_eth_if_t *)(netif->netif_prv))->eth_prv;

        *statistics = enet_if->statistics;

        result = FNET_OK;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Top Ethernet receive frame interrupt handler.
*              Clear event flag
*************************************************************************/
static void fnet_enet_isr_handler_top (void *cookie)
{
    fnet_enet_if_t *enet_if = (fnet_enet_if_t *)((fnet_eth_if_t *)(((fnet_netif_t *)cookie)->netif_prv))->eth_prv;

    ENET_IRQHandler(enet_if->base, &enet_if->handle); /* SDK ENET driver IRQ */
}

/************************************************************************
* DESCRIPTION: This function implements the Ethernet receive
*              frame interrupt handler.
*************************************************************************/
static void fnet_enet_isr_handler_bottom (void *cookie)
{
    fnet_netif_t *netif = (fnet_netif_t *)cookie;

    fnet_isr_lock();

    fnet_enet_input(netif);

    fnet_isr_unlock();
}

/************************************************************************
* PHY/MII Staff
*************************************************************************/

/************************************************************************
* DESCRIPTION: PHY initialization/reset.
*************************************************************************/
static fnet_return_t fnet_enet_phy_init(fnet_enet_if_t *enet_if)
{
    fnet_uint16_t       reg_value = 0;
    uint32_t            counter;
    fnet_return_t       result = FNET_ERR;

    /* Enable ENET SMI(serial management interface)- MII management interface clock. */
    CLOCK_EnableClock(kCLOCK_Eth);

    /* Set the ENET SMI(serial management interface)- MII management interface.*/
    ENET_SetSMI(enet_if->base);

    /* Looking for a valid PHY address. */
#if FNET_CFG_CPU_ETH_PHY_ADDR_DISCOVER
    fnet_enet_phy_discover_addr (enet_if, enet_if->phy_addr);
#endif

    /* == Reset PHY, wait for completion. == */
    fnet_enet_mii_write(enet_if, FNET_ETH_MII_REG_CR, FNET_ETH_MII_REG_CR_RESET);
    for (counter = FNET_ENET_MII_TIMEOUT; counter > 0; counter--)
    {
        fnet_enet_mii_read(enet_if, FNET_ETH_MII_REG_CR, &reg_value);
        if(reg_value & FNET_ETH_MII_REG_CR_RESET)
        {
            break; /* Completed */
        }
    }

    if(counter) /* No timeout */
    {
        /*== Start auto-negotiation  == */
        fnet_enet_mii_read(enet_if, FNET_ETH_MII_REG_CR, &reg_value);
        fnet_enet_mii_write(enet_if, FNET_ETH_MII_REG_CR, (fnet_uint16_t)(reg_value | FNET_ETH_MII_REG_CR_ANE | FNET_ETH_MII_REG_CR_ANE_RESTART));

#if FNET_CFG_CPU_ETH_ATONEGOTIATION_TIMEOUT
        /* Wait for auto-negotiation completion. */
        {
            fnet_time_t     last_time =  fnet_timer_get_ms();

            do
            {
                /* Read Basic Mode Status Register*/
                if(fnet_enet_mii_read(enet_if, FNET_ETH_MII_REG_SR, &reg_value) == FNET_ERR)
                {
                    break;
                }

                /* Auto-Negotiation Complete */
                if(reg_value & FNET_ETH_MII_REG_SR_AN_COMPLETE)
                {
                    break;
                }
            }
            while((fnet_timer_get_ms() - last_time) < FNET_CFG_CPU_ETH_ATONEGOTIATION_TIMEOUT);
        }
#endif /* FNET_CFG_CPU_ETH_ATONEGOTIATION_TIMEOUT */
        result = FNET_OK;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Looking for a valid PHY address.
*************************************************************************/
#if FNET_CFG_CPU_ETH_PHY_ADDR_DISCOVER
static void fnet_enet_phy_discover_addr (fnet_enet_if_t *enet_if, fnet_uint8_t phy_addr_start)
{
    fnet_uint8_t i;
    fnet_uint8_t phy_addr = enet_if->phy_addr; /* Save old value just in case the discover
                                               * is failed, in case communication with
                                               * the PHY via MDIO is not possible.*/

    for (i = (fnet_uint8_t)phy_addr_start; i < 32U; i++)
    {
        fnet_uint16_t id;

        enet_if->phy_addr = i;
        fnet_enet_mii_read(enet_if, FNET_ETH_MII_REG_IDR1, &id);

        if (!((id == 0U) || (id == 0xffffU) || (id == 0x7fffU)))
        {
            return; /* FHY address is discovered.*/
        }
    }
    enet_if->phy_addr = phy_addr;
}
#endif

/************************************************************************
* DESCRIPTION: Read a value from a PHY's MII register.
* reg_addr < address of the register in the PHY
* data < Pointer to storage for the Data to be read from the PHY register (passed by reference)
* Return FNET_ERR on failure, FNET_OK on success
*************************************************************************/
fnet_return_t fnet_enet_mii_read(fnet_enet_if_t *enet_if, fnet_uint32_t reg_addr, fnet_uint16_t *data)
{
    fnet_return_t   result = FNET_ERR;
    uint32_t        counter;

    ENET_StartSMIRead(enet_if->base, enet_if->phy_addr, reg_addr);

    /* Wait for MII complete. */
    for (counter = FNET_ENET_MII_TIMEOUT; counter > 0; counter--)
    {
        if(ENET_IsSMIBusy(enet_if->base) == false)
        {
            break; /* Result is ready */
        }
    }

    if(counter)
    {
        *data = ENET_ReadSMIData(enet_if->base);
        result = FNET_OK;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Write a value to a PHY's MII register.
* reg_addr = address of the register in the PHY
* data = Data to be writen to the PHY register (passed by reference)
* Return FNET_ERR on failure (timeout), FNET_OK on success
*************************************************************************/
fnet_return_t fnet_enet_mii_write(fnet_enet_if_t *enet_if, fnet_uint32_t reg_addr, fnet_uint16_t data)
{
    fnet_return_t   result = FNET_ERR;
    fnet_uint32_t   counter;

    ENET_StartSMIWrite(enet_if->base, enet_if->phy_addr, reg_addr, data);

    /* Wait for MII complete. */
    for (counter = FNET_ENET_MII_TIMEOUT; counter > 0; counter--)
    {
        if(ENET_IsSMIBusy(enet_if->base) == false)
        {
            break; /* Completed. */
        }
    }

    if(counter)
    {
        result = FNET_OK;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Link status.
*************************************************************************/
static fnet_bool_t fnet_enet_is_connected(fnet_netif_t *netif)
{
    fnet_uint16_t   data;
    fnet_enet_if_t  *enet_if;
    fnet_bool_t     res = FNET_FALSE;

    enet_if = (fnet_enet_if_t *)((fnet_eth_if_t *)(netif->netif_prv))->eth_prv;

    /* Some PHY (e.g.DP8340) returns "unconnected" and than "connected" state
     *  just to show that was transition event from one state to another.
     *  As we need only curent state,  read 2 times and returtn
     *  the current/latest state.
     */
    fnet_enet_mii_read(enet_if, FNET_ETH_MII_REG_SR, &data);

    if (fnet_enet_mii_read(enet_if, FNET_ETH_MII_REG_SR, &data) == FNET_OK)
    {
        res = (((data & FNET_ETH_MII_REG_SR_LINK_STATUS) != 0u) ? FNET_TRUE : FNET_FALSE);
    }
    else /* Return previous value in case read PHY error. */
    {
        res = netif->is_connected;
    }

    return res;
}

/************************************************************************
* DESCRIPTION: Compute the CRC-32 polynomial on the multicast group
*************************************************************************/
#if FNET_CFG_MULTICAST

/************************************************************************
* DESCRIPTION: Joins a multicast group on ENET interface.
*************************************************************************/
void fnet_enet_multicast_join(fnet_netif_t *netif, fnet_mac_addr_t multicast_addr )
{
    //TBD
}

/************************************************************************
* DESCRIPTION: Leavess a multicast group on ENET interface.
*************************************************************************/
void fnet_enet_multicast_leave(fnet_netif_t *netif, fnet_mac_addr_t multicast_addr )
{
    //TBD
}

#endif /*FNET_CFG_MULTICAST*/

/************************************************************************
* DESCRIPTION: Prints all MII register.
* !!!! Used only for debug needs. !!!!!
*************************************************************************/
void fnet_enet_debug_mii_print_regs(fnet_netif_t *netif)
{
    fnet_enet_if_t  *enet_if;
    fnet_uint16_t   reg_value;

    if(netif->netif_api->netif_type == FNET_NETIF_TYPE_ETHERNET)
    {
        enet_if = (fnet_enet_if_t *)((fnet_eth_if_t *)(netif->netif_prv))->eth_prv;

        fnet_printf(" === MII registers ===\r\n");
        fnet_enet_mii_read(enet_if, FNET_ETH_MII_REG_CR, &reg_value);
        fnet_printf("\tCR = 0x%04X\n", reg_value );
        fnet_enet_mii_read(enet_if, FNET_ETH_MII_REG_SR, &reg_value);
        fnet_printf("\tSR = 0x%04X\n", reg_value );
        fnet_enet_mii_read(enet_if, FNET_ETH_MII_REG_IDR1, &reg_value);
        fnet_printf("\tIDR1 = 0x%04X\n", reg_value );
        fnet_enet_mii_read(enet_if, FNET_ETH_MII_REG_IDR2, &reg_value);
        fnet_printf("\tIDR2 = 0x%04X\n", reg_value );
        fnet_enet_mii_read(enet_if, FNET_ETH_MII_REG_ANAR, &reg_value);
        fnet_printf("\tANAR = 0x%04X\n", reg_value );
        fnet_enet_mii_read(enet_if, FNET_ETH_MII_REG_ANLPAR, &reg_value);
        fnet_printf("\tANLPAR = 0x%04X\n", reg_value );
        fnet_enet_mii_read(enet_if, FNET_ETH_MII_REG_ANER, &reg_value);
        fnet_printf("\tANER = 0x%04X\n", reg_value );
        fnet_enet_mii_read(enet_if, FNET_ETH_MII_REG_ANNPTR, &reg_value);
        fnet_printf("\tANNPTR = 0x%04X\n", reg_value );
    }
}

#endif /* FNET_LPC && FNET_CFG_CPU_ETH0 */
