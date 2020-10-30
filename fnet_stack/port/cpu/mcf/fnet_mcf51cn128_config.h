/**************************************************************************
*
* Copyright 2009-2018 by Andrey Butok. FNET Community.
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
*  MCF51CN128 specific configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_MCF51CN_CONFIG_H_

#define _FNET_MCF51CN_CONFIG_H_

#define FNET_MCF                            (1)

/* The platform is ColdFire V1.*/
#define FNET_CFG_MCF_V1                     (1)

/* Maximum Transmission Unit */
#ifndef FNET_CFG_CPU_ETH0_MTU
    #define FNET_CFG_CPU_ETH0_MTU           (800)
#endif

/* System frequency in Hz. */
#ifndef FNET_CFG_CPU_CLOCK_HZ
    #define FNET_CFG_CPU_CLOCK_HZ        	(50331648)
#endif

/* Timer IRQ number */
#undef FNET_CFG_CPU_TIMER_VECTOR_NUMBER
#define FNET_CFG_CPU_TIMER_VECTOR_NUMBER    114

/* Ethernet RX IRQ number */
#undef FNET_CFG_CPU_ETH0_VECTOR_NUMBER
#define FNET_CFG_CPU_ETH0_VECTOR_NUMBER     87

/* No cache. */
#define FNET_CFG_CPU_CACHE                  (0)

/* The platform does not have second Ethernet Module.*/
#define FNET_CFG_CPU_ETH1        			(0)

/* No MIB.*/
#undef FNET_CFG_MCF_ETH_MIB
#define FNET_CFG_MCF_ETH_MIB                (0)

/* No DTIM.*/
#undef FNET_CFG_MCF_TIMER_DTIM
#define FNET_CFG_MCF_TIMER_DTIM             (0)

/* No RCM.*/
#undef FNET_CFG_MCF_RCM
#define FNET_CFG_MCF_RCM                    (0)

/* The platform uses 8 bits Real-Time Counter.*/
#undef FNET_CFG_MCF_TIMER_RTC8
#define FNET_CFG_MCF_TIMER_RTC8             (1)

/* No UART */
#undef FNET_CFG_MCF_UART
#define FNET_CFG_MCF_UART                   (0)

/* The platform uses SCI.*/
#undef FNET_CFG_MCF_SCI
#define FNET_CFG_MCF_SCI                    (1)

/* TWR board use by default port number 1.*/
#ifndef FNET_CFG_CPU_SERIAL_PORT_DEFAULT
    #define FNET_CFG_CPU_SERIAL_PORT_DEFAULT	(1)
#endif

/* The platform has ColdFire Flash Module.*/
#define FNET_CFG_CPU_FLASH                  (1)

#define FNET_CFG_CPU_FLASH_PAGE_SIZE        (1*1024)

/* Flash size.*/
#define FNET_CFG_CPU_FLASH_SIZE         	(1024 * 128)    /* 128 KB */

#endif
