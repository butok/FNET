/**************************************************************************
*
* Copyright 2005-2011 by Andrey Butok and Gordon Jahn.
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
* @file fnet_mpc564xbc_config.h

*
* @brief MPC564xBC specific configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_MPC564XBC_CONFIG_H_

#define _FNET_MPC564XBC_CONFIG_H_

#define FNET_MPC                        (1)

/******************************************************************************
 * Maximum Timer number that is avaiable on the used platform.
 ******************************************************************************/
#define  FNET_CFG_CPU_TIMER_NUMBER_MAX  (7)

#ifndef FNET_CFG_CPU_TIMER_NUMBER
    #define FNET_CFG_CPU_TIMER_NUMBER           (FNET_CFG_CPU_TIMER_NUMBER_MAX)
#endif

/* MPC Flash Module is not supported.*/
#define FNET_CFG_CPU_FLASH              (0)
#define FNET_CFG_CPU_FLASH_PAGE_SIZE    (2*1024)

/* The platform does not have second Ethernet Module.*/
#define FNET_CFG_CPU_ETH1               (0)

/* Default system bus frequency in Hz */
#ifndef FNET_CFG_CPU_CLOCK_HZ
    #define FNET_CFG_CPU_CLOCK_HZ           (120000000)
#endif

/* Defines the maximum number of incoming frames that may
 *           be buffered by the Ethernet module.*/
#ifndef FNET_CFG_CPU_ETH_RX_BUFS_MAX
    #define FNET_CFG_CPU_ETH_RX_BUFS_MAX    (4)
#endif

/* Flash size.*/
#define FNET_CFG_CPU_FLASH_SIZE         (1024 * 512)    /* 512 KB */


#endif /* _FNET_MPC564XBC_CONFIG_H_ */
