/**************************************************************************
*
* Copyright 2012-2013 by Andrey Butok. FNET Community.
*
***************************************************************************
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
**********************************************************************/
/*!
*
* @file fnet_mcf54418_config.h
*
* @brief MCF54481 specific configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_MCF54481_CONFIG_H_

#define _FNET_MCF54481_CONFIG_H_

/**************************************************************************
 *  MCF family.
 **************************************************************************/
#define FNET_MCF                        	(1)

/**************************************************************************
 *  Default Size of the internal static heap buffer.
 **************************************************************************/
#ifndef FNET_CFG_HEAP_SIZE
    #define FNET_CFG_HEAP_SIZE      		(30 * 1024)
#endif

/**************************************************************************
 *  Modelo does not have on-chip Flash Module.
 **************************************************************************/
#define FNET_CFG_CPU_FLASH            		(0)

/**************************************************************************
 *  Modelo has fixed Internal Peripheral System Base Address.
 **************************************************************************/
#define FNET_CFG_MCF_IPSBAR					((fnet_uint8_t *)0xEC000000)

/**************************************************************************
 *  Default system bus frequency in Hz.
 **************************************************************************/
#ifndef FNET_CFG_CPU_CLOCK_HZ
    #define FNET_CFG_CPU_CLOCK_HZ  			(125000000) /* CPU is 250MHz */
#endif

/**************************************************************************
 *  Reduced Media Independent Interface (RMII) support.
 *  NOTE: The processor contains two Ethernet controllers.
 *        However, due to external pin limitations, if MII mode is
 *        used, only FEC0 can be used. In RMII mode,
 *        both Ethernet controllers are available.
 **************************************************************************/
#ifndef FNET_CFG_CPU_ETH_RMII
    #define FNET_CFG_CPU_ETH_RMII   		(1)
#endif

/**************************************************************************
 *  Default maximum number of incoming frames that may
 *           be buffered by the Ethernet module.
 **************************************************************************/
#ifndef FNET_CFG_CPU_ETH_RX_BUFS_MAX
    #define FNET_CFG_CPU_ETH_RX_BUFS_MAX    (4)
#endif

/* To improve the TX performance.*/
#ifndef FNET_CFG_CPU_ETH_HW_TX_IP_CHECKSUM
    #define FNET_CFG_CPU_ETH_HW_TX_IP_CHECKSUM          (1)
#endif

/* To improve the TX performance.*/
#ifndef FNET_CFG_CPU_ETH_HW_TX_PROTOCOL_CHECKSUM
    #define FNET_CFG_CPU_ETH_HW_TX_PROTOCOL_CHECKSUM    (1)
#endif

/* To improve the RX performance.*/
#ifndef FNET_CFG_CPU_ETH_HW_RX_IP_CHECKSUM
    #define FNET_CFG_CPU_ETH_HW_RX_IP_CHECKSUM          (1)
#endif

/* To improve the RX performance.*/
#ifndef FNET_CFG_CPU_ETH_HW_RX_PROTOCOL_CHECKSUM
    #define FNET_CFG_CPU_ETH_HW_RX_PROTOCOL_CHECKSUM    (1)
#endif

/* Discard of frames with MAC layer errors.*/
#ifndef FNET_CFG_CPU_ETH_HW_RX_MAC_ERR
    #define FNET_CFG_CPU_ETH_HW_RX_MAC_ERR              (1)
#endif


/**************************************************************************
 *  ENET0
 **************************************************************************/
#define FNET_FEC0_BASE_ADDR                 ((fnet_vuint32_t*)(0xFC0D4004))
#define FNET_CFG_CPU_ETH0_VECTOR_NUMBER     (40+0x40)

/**************************************************************************
 *  ENET1
 **************************************************************************/
#define FNET_FEC1_BASE_ADDR                 ((fnet_vuint32_t*)(0xFC0D8004))
#define FNET_CFG_CPU_ETH1_VECTOR_NUMBER     (53+0x40)

/**************************************************************************
 *  There is cache.
 **************************************************************************/
#define FNET_CFG_CPU_CACHE              	(1)

/**************************************************************************
 *  SRAM size.
 **************************************************************************/
#define FNET_CFG_CPU_SRAM_SIZE          	(1024 * 64)     /* 64 KB */

/**************************************************************************
 *  It used DTIM Timer
 **************************************************************************/
#define FNET_CFG_MCF_TIMER_DTIM    			(1)
#define FNET_CFG_CPU_TIMER_VECTOR_NUMBER    (32+FNET_CFG_CPU_TIMER_NUMBER+0x40)

#endif /* _FNET_MCF54481_CONFIG_H_ */

