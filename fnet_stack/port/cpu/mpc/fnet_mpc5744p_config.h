/**************************************************************************
*
* Copyright 2015 by Andrey Butok and Gordon Jahn.
*                        Freescale Semiconductor, Inc.
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
* @file fnet_mpc5744p_config.h
*
* @brief MPC574xP specific configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_MPC5744P_CONFIG_H_

#define _FNET_MPC5744P_CONFIG_H_

#define FNET_MPC                                        (1)

/******************************************************************************
 * Maximum Timer number that is avaiable on the used platform.
 ******************************************************************************/
#define  FNET_CFG_CPU_TIMER_NUMBER_MAX                  (4)

/*****************************************************************************
 *  Max priority.
 ******************************************************************************/
#define FNET_CFG_CPU_VECTOR_PRIORITY_MAX                (31)


/* Size of the internal static heap buffer. */
#ifndef FNET_CFG_HEAP_SIZE
    #define FNET_CFG_HEAP_SIZE                          (30 * 1024)
#endif


/* Default system bus frequency in Hz*/
#ifndef FNET_CFG_CPU_CLOCK_HZ
    #define FNET_CFG_CPU_CLOCK_HZ                       (200000000)
#endif

/* MPC Flash Module is not supported.*/
#define FNET_CFG_CPU_FLASH                              (0)

#define FNET_CFG_CPU_FLASH_PAGE_SIZE                    (2*1024)

/* The platform does not have second Ethernet Module.*/
#define FNET_CFG_CPU_ETH1                               (0)

/* Defines the maximum number of incoming frames that may
 *           be buffered by the Ethernet module.*/
#ifndef FNET_CFG_CPU_ETH_RX_BUFS_MAX
    #define FNET_CFG_CPU_ETH_RX_BUFS_MAX                (4)
#endif

/* On-chip Flash size.*/
#define FNET_CFG_CPU_FLASH_SIZE                         (1024 * 1024 * 5 / 2)   /* 2.5 MB*/

/* SRAM size.*/
#define FNET_CFG_CPU_SRAM_SIZE                          (1024 * 384)   /* 384 KB */

/* PIT channel to use as timer */
#ifndef FNET_CFG_CPU_TIMER_NUMBER
    #define FNET_CFG_CPU_TIMER_NUMBER                   (0)   /* PIT channel 0 to 3, defined 0 as default */
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

#endif /* _FNET_MPC5744P_CONFIG_H_ */
