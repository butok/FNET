/**************************************************************************
*
* Copyright 2012-2015 by Andrey Butok. FNET Community.
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
* @file fnet_mpc_config.h
*
* @author Andrey Butok
*
* @date Dec-17-2012
*
* @version 0.1.1.0
*
* @brief MPC-specific default configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_MPC_CONFIG_H_

    #define _FNET_MPC_CONFIG_H_

    #include "fnet.h"


    #if FNET_MPC

        /******************************************************************************
        *  Vector number of the Ethernet Receive Frame interrupt.
        *  NOTE: User application should not change this parameter.
        ******************************************************************************/
        /*! @cond */
        #ifndef FNET_CFG_CPU_ETH0_VECTOR_NUMBER

            #if FNET_CFG_CPU_MPC564xBC
                #define FNET_CFG_CPU_ETH0_VECTOR_NUMBER      (245)
            #endif
            #if FNET_CFG_CPU_MPC5668G
                #define FNET_CFG_CPU_ETH0_VECTOR_NUMBER      (299)
            #endif
            #if FNET_CFG_CPU_MPC5744P
                #define FNET_CFG_CPU_ETH0_VECTOR_NUMBER      (218)
            #endif
            #if FNET_CFG_CPU_MPC5566
                #define FNET_CFG_CPU_ETH0_VECTOR_NUMBER      (195)
            #endif

        #endif
        /*! @endcond */

        /*****************************************************************************
        *  Max priority.
        ******************************************************************************/
        #ifndef FNET_CFG_CPU_VECTOR_PRIORITY_MAX
            #define FNET_CFG_CPU_VECTOR_PRIORITY_MAX                (15)
        #endif

        /******************************************************************************
        *  Vector number of the timer interrupt.
        *  NOTE: User application should not change this parameter.
        ******************************************************************************/
        /*! @cond */
        #ifndef FNET_CFG_CPU_TIMER_VECTOR_NUMBER
            #if FNET_CFG_CPU_MPC5668G
                #define FNET_CFG_CPU_TIMER_VECTOR_NUMBER    (149+FNET_CFG_CPU_TIMER_NUMBER)

            #elif FNET_CFG_CPU_MPC5744P
                #define FNET_CFG_CPU_TIMER_VECTOR_NUMBER    (226+FNET_CFG_CPU_TIMER_NUMBER)

            #elif FNET_CFG_CPU_MPC564xBC
                #if (FNET_CFG_CPU_TIMER_NUMBER<3)
                    #define FNET_CFG_CPU_TIMER_VECTOR_NUMBER    (59+FNET_CFG_CPU_TIMER_NUMBER)
                #else
                    #define FNET_CFG_CPU_TIMER_VECTOR_NUMBER    (124+FNET_CFG_CPU_TIMER_NUMBER)
                #endif
            #elif FNET_CFG_CPU_MPC5566
                #if (FNET_CFG_CPU_TIMER_NUMBER<16)
                    #define FNET_CFG_CPU_TIMER_VECTOR_NUMBER    (51+FNET_CFG_CPU_TIMER_NUMBER)
                #else
                    #define FNET_CFG_CPU_TIMER_VECTOR_NUMBER    (202-16+FNET_CFG_CPU_TIMER_NUMBER)
                #endif
            #endif
        #endif
        /*! @endcond */

        /*****************************************************************************
        *  On-chip SRAM memory start address.
        ******************************************************************************/
        #ifndef FNET_CFG_CPU_SRAM_ADDRESS
            #define FNET_CFG_CPU_SRAM_ADDRESS   (0x40000000)
        #endif

    #endif /* FNET_MPC */

#endif /* _FNET_MPC_CONFIG_H_ */
