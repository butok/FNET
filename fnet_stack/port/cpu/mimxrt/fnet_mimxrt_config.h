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
*  i.MX RT specific default configuration.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_MIMXRT_CONFIG_H_

#define _FNET_MIMXRT_CONFIG_H_

#include "fnet_config.h"

#ifndef FNET_MIMXRT
    #define FNET_MIMXRT   (0)
#endif

#if FNET_MIMXRT  || defined(__DOXYGEN__)

    /*! @addtogroup fnet_platform_mimxrt_config  */
    /*! @{ */

    /**************************************************************************/ /*!
    * @def      FNET_CFG_MIMXRT_TIMER_PIT
    * @brief    The Programmable Interrupt Timer (PIT) module:
    *               - @c 1 = The platform uses the Programmable Interrupt Timer (PIT) module
    *                        as the FNET timer.
    *               - @c 0 = The platform does not have or does not use the PIT Timer
    *                        module as the FNET timer.
    ******************************************************************************/
    #ifndef FNET_CFG_MIMXRT_TIMER_PIT
        #define FNET_CFG_MIMXRT_TIMER_PIT              (0)
    #endif

    /**************************************************************************/ /*!
    * @def      FNET_CFG_MIMXRT_TIMER_QTMR
    * @brief    The Quad Timer (TMR) module:
    *               - @c 1 = The platform uses the Quad Timer (TMR) module
    *                        as the FNET timer.
    *               - @c 0 = The platform does not have or does not use the RMR Timer
    *                        module as the FNET timer.
    ******************************************************************************/
    #ifndef FNET_CFG_MIMXRT_TIMER_QTMR
        #define FNET_CFG_MIMXRT_TIMER_QTMR             (0)
    #endif

    #if FNET_CFG_MIMXRT_TIMER_PIT
        #define  FNET_CFG_CPU_TIMER_NUMBER_MAX              (3u) /* Maximum Timer number that is avaiable on the used platform. */
        #ifndef FNET_CFG_CPU_TIMER_VECTOR_NUMBER
            #define FNET_CFG_CPU_TIMER_VECTOR_NUMBER        (16+122/*irq*/) /* Vector number of the timer interrupt.*/
        #endif
    #endif

    #if FNET_CFG_MIMXRT_TIMER_QTMR
        #define  FNET_CFG_CPU_TIMER_NUMBER_MAX              (4u) /* Maximum Timer number that is avaiable on the used platform. */
        #ifndef FNET_CFG_CPU_TIMER_VECTOR_NUMBER
            #define FNET_CFG_CPU_TIMER_VECTOR_NUMBER        (16+133 + (FNET_CFG_CPU_TIMER_NUMBER/*starts from 1*/-1)/*irq*/) /* Vector number of the timer interrupt.*/
        #endif
    #endif

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
        #define FNET_CFG_CPU_SERIAL_PORT_DEFAULT        (1U)
    #endif

    /*******************************************************************************
    * Timer number used by the FNET. It can range from 0 to FNET_CFG_CPU_TIMER_NUMBER_MAX.
    ******************************************************************************/
    #ifndef FNET_CFG_CPU_TIMER_NUMBER
        #define FNET_CFG_CPU_TIMER_NUMBER               (FNET_CFG_CPU_TIMER_NUMBER_MAX)
    #endif

    /******************************************************************************
    *   Vector table address.@n
    *           By default, NVIC vector table register (VTOR).
    ******************************************************************************/
    #ifndef FNET_CFG_CPU_VECTOR_TABLE
        #define FNET_CFG_CPU_VECTOR_TABLE               FNET_MIMXRT_SCB_VTOR
    #endif

    /******************************************************************************
    *  Vector number of the Ethernet Receive Frame vector number.
    *  NOTE: User application should not change this parameter.
    ******************************************************************************/
    #ifndef FNET_CFG_CPU_ETH0_VECTOR_NUMBER
        #define FNET_CFG_CPU_ETH0_VECTOR_NUMBER        (16+114/*irq*/)
    #endif

    /*****************************************************************************
    *  Byte order is little endian.
    ******************************************************************************/
    #define FNET_CFG_CPU_LITTLE_ENDIAN                  (1)

    /*****************************************************************************
    *  There is data cache.
    ******************************************************************************/
    #ifndef FNET_CFG_CPU_CACHE
        #define FNET_CFG_CPU_CACHE                      (1)
    #endif

    /*****************************************************************************
    *  Max priority.
    ******************************************************************************/
    #define FNET_CFG_CPU_VECTOR_PRIORITY_MAX            (15u) /* 4 bits */

    /* i.MX RT start up code does not support vector table in RAM, so far */
    #ifndef FNET_CFG_CPU_VECTOR_TABLE_IS_IN_RAM
        #define FNET_CFG_CPU_VECTOR_TABLE_IS_IN_RAM     (0)
    #endif

    /*! @} */
#endif /* FNET_MIMXRT*/

#endif /* _FNET_MIMXRT_CONFIG_H_ */
