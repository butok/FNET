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
*  LPC specific default configuration.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_LPC_CONFIG_H_

#define _FNET_LPC_CONFIG_H_

#include "fnet_config.h"

#ifndef FNET_LPC
    #define FNET_LPC   (0)
#endif

#if FNET_LPC

    /*****************************************************************************
    *  Byte order is little endian.
    ******************************************************************************/
    #define FNET_CFG_CPU_LITTLE_ENDIAN                  (1)

    /**************************************************************************
    *  Reduced Media Independent Interface (RMII) support.
    ******************************************************************************/
    #ifndef FNET_CFG_CPU_ETH_RMII
        #define FNET_CFG_CPU_ETH_RMII        			(1)
    #endif

    /**************************************************************************
    *  Default serial port number.
    ******************************************************************************/
    #ifndef FNET_CFG_CPU_SERIAL_PORT_DEFAULT
        #define FNET_CFG_CPU_SERIAL_PORT_DEFAULT        (0U)
    #endif

    /**************************************************************************
    *  Maximum Timer number that is avaiable on the used platform.
    ******************************************************************************/
    #ifndef FNET_CFG_CPU_TIMER_NUMBER_MAX
        #define  FNET_CFG_CPU_TIMER_NUMBER_MAX          (4u)
    #endif

    /*******************************************************************************
     * Timer number used by the FNET. It can range from 0 to FNET_CFG_CPU_TIMER_NUMBER_MAX.
     ******************************************************************************/
    #ifndef FNET_CFG_CPU_TIMER_NUMBER
        #define FNET_CFG_CPU_TIMER_NUMBER           (FNET_CFG_CPU_TIMER_NUMBER_MAX)
    #endif

    /**************************************************************************/ /*!
    *   Vector table address.@n
    *           By default, NVIC vector table register (VTOR).
    * @showinitializer
    ******************************************************************************/
    #ifndef FNET_CFG_CPU_VECTOR_TABLE
        #define FNET_CFG_CPU_VECTOR_TABLE               FNET_LPC_SCB_VTOR
    #endif

    /******************************************************************************
    *  Vector number of the timer interrupt.
    ******************************************************************************/
    #ifndef FNET_CFG_CPU_TIMER_VECTOR_NUMBER
        #if FNET_CFG_CPU_TIMER_NUMBER == 0
           #define FNET_CFG_CPU_TIMER_VECTOR_NUMBER        (16U + 10U) /* Standard counter/timer CTIMER0 */
        #elif FNET_CFG_CPU_TIMER_NUMBER == 1
           #define FNET_CFG_CPU_TIMER_VECTOR_NUMBER        (16U + 11U) /* Standard counter/timer CTIMER1 */
        #elif FNET_CFG_CPU_TIMER_NUMBER == 2
           #define FNET_CFG_CPU_TIMER_VECTOR_NUMBER        (16U + 36U) /* Standard counter/timer CTIMER2 */
        #elif FNET_CFG_CPU_TIMER_NUMBER == 3
           #define FNET_CFG_CPU_TIMER_VECTOR_NUMBER        (16U + 13U) /* Standard counter/timer CTIMER3 */
        #elif FNET_CFG_CPU_TIMER_NUMBER == 4
           #define FNET_CFG_CPU_TIMER_VECTOR_NUMBER        (16U + 37U) /* Standard counter/timer CTIMER4 */
        #else
            #error Not suported FNET_CFG_CPU_TIMER_NUMBER
        #endif
    #endif

    /******************************************************************************
    *  Vector number of the Ethernet Receive Frame vector number.
    ******************************************************************************/
    #ifndef FNET_CFG_CPU_ETH0_VECTOR_NUMBER
        #define FNET_CFG_CPU_ETH0_VECTOR_NUMBER        (49U)
    #endif

    /*****************************************************************************
    *  Max priority.
    ******************************************************************************/
    #define FNET_CFG_CPU_VECTOR_PRIORITY_MAX            (15u) /* 4 bits */

    /*****************************************************************************
    *  On-chip Flash memory start address.
    ******************************************************************************/
    #ifndef FNET_CFG_CPU_FLASH_ADDRESS
        #define FNET_CFG_CPU_FLASH_ADDRESS          (0x0U)
    #endif

    #ifndef FNET_CFG_CPU_FLASH_PROGRAM_SIZE
        #define FNET_CFG_CPU_FLASH_PROGRAM_SIZE     (4U)
    #endif

#endif /* FNET_LPC */

#endif /* _FNET_LPC_CONFIG_H_ */
