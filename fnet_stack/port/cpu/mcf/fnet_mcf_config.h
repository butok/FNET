/**************************************************************************
*
* Copyright 2012-2015 by Andrey Butok.
* Copyright 2008-2011 by Andrey Butok. Freescale Semiconductor, Inc.
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
* @file fnet_mcf_config.h
*
* @author Andrey Butok
*
* @brief ColdFire specific default configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_MCF_CONFIG_H_

#define _FNET_MCF_CONFIG_H_


#include "fnet_config.h"


#if FNET_MCF || defined(__DOXYGEN__)

    /*! @addtogroup fnet_platform_mcf_config  */
    /*! @{ */

    /**************************************************************************/ /*!
    * @def      FNET_CFG_MCF_V1
    * @brief     ColdFire V1 (Flexis):
    *               - @c 1 = The platform is ColdFire V1.
    *                        For example MCF51CN128.
    *               - @c 0 = The platform is ColdFire V2-V4.
    *  @n @n NOTE: User application should not change this parameter.
    ******************************************************************************/
    #ifndef FNET_CFG_MCF_V1
        #define FNET_CFG_MCF_V1                      (0)
    #endif

    /*****************************************************************************
    *  Max priority.
    ******************************************************************************/
    #define FNET_CFG_CPU_VECTOR_PRIORITY_MAX                (7)

    /******************************************************************************
    *  Vector number of the Ethernet Receive Frame interrupt.
    *  NOTE: User application should not change this parameter.
    ******************************************************************************/
    #ifndef FNET_CFG_CPU_ETH0_VECTOR_NUMBER
        #define FNET_CFG_CPU_ETH0_VECTOR_NUMBER     (27+0x40)
    #endif

    /**************************************************************************/ /*!
    * @def      FNET_CFG_MCF_TIMER_DTIM
    * @brief    The DMA Timer (DTIM) module:
    *               - @c 1 = The platform uses the DMA Timer (DTIM) module
    *                        as the FNET timer.
    *               - @c 0 = The platform does not have or does not use the DMA Timer (DTIM)
    *                        module as the FNET timer.
    ******************************************************************************/
    #ifndef FNET_CFG_MCF_TIMER_DTIM
        #define FNET_CFG_MCF_TIMER_DTIM             (1)
    #endif

    /**************************************************************************/ /*!
    * @def      FNET_CFG_MCF_TIMER_PIT
    * @brief    The Programmable Interrupt Timer (PIT) module:
    *               - @c 1 = The platform uses the Programmable Interrupt Timer (PIT) module
    *                        as the FNET timer.
    *               - @c 0 = The platform does not have or does not use the PIT Timer
    *                        module as the FNET timer.
    ******************************************************************************/
    #ifndef FNET_CFG_MCF_TIMER_PIT
        #define FNET_CFG_MCF_TIMER_PIT              (0)
    #endif

    #if FNET_CFG_MCF_TIMER_PIT /* Disable DTIM if used PIT. */
        #undef FNET_CFG_MCF_TIMER_DTIM
        #define FNET_CFG_MCF_TIMER_DTIM             (0)
    #endif

    /**************************************************************************/ /*!
    * @def      FNET_CFG_MCF_TIMER_RTC8
    * @brief    The 8-bit Real-Time Counter (RTC) module:
    *               - @c 1 = The platform uses the 8-bit Real-Time Counter (RTC)
    *                        module as the FNET timer.
    *               - @c 0 = The platform does not have the 8-bit Real-Time
    *                        Counter (RTC) module.
    *           @n @n NOTE: User application should not change this parameter.
    ******************************************************************************/
    #ifndef FNET_CFG_MCF_TIMER_RTC8
        #define FNET_CFG_MCF_TIMER_RTC8             (0)
    #endif

    /******************************************************************************
    *  Timer number used by the FNET. It can range from 0 to 3 for DTIM timer
    *  and from 0 to 1 for PIT timer.
    *  NOTE: It's ignored for MCF V1.
    ******************************************************************************/
    #if FNET_CFG_MCF_TIMER_DTIM
        #define  FNET_CFG_CPU_TIMER_NUMBER_MAX  (3)
    #endif

    #if FNET_CFG_MCF_TIMER_PIT
        #define  FNET_CFG_CPU_TIMER_NUMBER_MAX  (1)
    #endif

    /******************************************************************************
    *  Vector number of the timer interrupt.
    *  NOTE: User application should not change this parameter.
    ******************************************************************************/
    #ifndef FNET_CFG_CPU_TIMER_VECTOR_NUMBER
        #if FNET_CFG_MCF_TIMER_DTIM
            #if FNET_CFG_CPU_MCF54418

            #else /* Other MCFs */
                #define FNET_CFG_CPU_TIMER_VECTOR_NUMBER    (19+FNET_CFG_CPU_TIMER_NUMBER+0x40)
            #endif
        #endif
        #if FNET_CFG_MCF_TIMER_PIT
            #define FNET_CFG_CPU_TIMER_VECTOR_NUMBER    (55+FNET_CFG_CPU_TIMER_NUMBER+0x40)
        #endif
    #endif


    /**************************************************************************/ /*!
    * @def      FNET_CFG_MCF_IPSBAR
    * @brief    Internal Peripheral System Base Address.
    *           @n @n NOTE: It's ignored for MCF V1.
    * @showinitializer
    ******************************************************************************/

    #ifndef __FNET_ASM_CODE
        #ifndef FNET_CFG_MCF_IPSBAR
            /* The following symbol should be defined in the lcf */
            extern fnet_uint8_t __IPSBAR [];

            #define FNET_CFG_MCF_IPSBAR                 __IPSBAR

        #endif
    #endif

    /* Overload checksum functions.*/
    #ifndef FNET_CFG_OVERLOAD_CHECKSUM_LOW
        #define FNET_CFG_OVERLOAD_CHECKSUM_LOW  (1)
    #endif

    /**************************************************************************/ /*!
    * @def      FNET_CFG_MCF_CACHE_CACR
    * @brief    Cache Control Register (CACR) default value,
    *           used during cache invalidation.@n
    *           It is used only when FNET_CFG_CPU_CACHE is enabled.
    * @showinitializer
    ******************************************************************************/
    #ifndef FNET_CFG_MCF_CACHE_CACR
        #define FNET_CFG_MCF_CACHE_CACR             (0)
    #endif

    /*****************************************************************************
    *  On-chip Flash memory start address.
    ******************************************************************************/

    #ifndef __FNET_ASM_CODE
        #if FNET_CFG_CPU_FLASH
            #ifndef FNET_CFG_CPU_FLASH_ADDRESS
                /* The following symbol should be defined in the linker file. */

                #if FNET_CFG_MCF_V1
                    #define FNET_CFG_CPU_FLASH_ADDRESS  (0x00000000)
                #else
                    extern fnet_uint8_t __FLASHBAR [];
                    #define FNET_CFG_CPU_FLASH_ADDRESS  ((fnet_uint32_t)__FLASHBAR)
                #endif

            #endif /* FNET_CFG_CPU_FLASH_ADDRESS */
        #endif /* FNET_CFG_CPU_FLASH */
    #endif /* __FNET_ASM_CODE */

    /*****************************************************************************
    *  On-chip SRAM memory start address.
    ******************************************************************************/

    #ifndef __FNET_ASM_CODE
        #ifndef FNET_CFG_CPU_SRAM_ADDRESS
            /* The following symbol should be defined in the linker file.*/

            #if FNET_CFG_MCF_V1
                #define FNET_CFG_CPU_SRAM_ADDRESS   (0x00800000)
            #else
                extern fnet_uint8_t __RAMBAR [];
                #define FNET_CFG_CPU_SRAM_ADDRESS   ((fnet_uint32_t)__RAMBAR)
            #endif

        #endif
    #endif

    /**************************************************************************/ /*!
    * @def      FNET_CFG_MCF_RCM
    * @brief    Reset Controller Module (RCM):
    *               - @c 1 = Current platform has the Reset Controller Module.
    *               - @c 0 = Current platform does not have the Reset Controller Module.
    *              @n @n NOTE: User application should not change this parameter.
    ******************************************************************************/
    #ifndef FNET_CFG_MCF_RCM
        #define FNET_CFG_MCF_RCM                    (1)
    #endif

    /**************************************************************************/ /*!
    * @def      FNET_CFG_MCF_UART
    * @brief    The Universal Asynchronous Receiver/Transmitters (UART) module:
    *               - @c 1 = The platform uses the UART module
    *                        for serial-port input/output functions.
    *               - @c 0 = The platform does not have the UART module.
    *           @n @n NOTE: User application should not change this parameter.
    ******************************************************************************/
    #ifndef FNET_CFG_MCF_UART
        #define FNET_CFG_MCF_UART                   (1)
    #endif

    /**************************************************************************/ /*!
    * @def      FNET_CFG_MCF_SCI
    * @brief    The Serial Communication Interface (SCI) module:
    *               - @c 1 = The platform uses the SCI module
    *                        for serial-port input/output functions.
    *               - @c 0 = The platform does not have the SCI module.
    *           @n @n NOTE: User application should not change this parameter.
    ******************************************************************************/
    #ifndef FNET_CFG_MCF_SCI
        #define FNET_CFG_MCF_SCI                    (0)
    #endif

    #define FNET_CFG_CPU_FLASH_PROGRAM_SIZE         (4) /* Bytes.*/


    /*! @} */

#endif /* FNET_MCF */

#endif
