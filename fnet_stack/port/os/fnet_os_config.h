/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
* Copyright 2008-2010 by Andrey Butok. Freescale Semiconductor, Inc.
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
**********************************************************************/
/*!
*
* @file fnet_os_config.h
*
* @author Andrey Butok
*
* @brief Default OS-specific configuration. @n
*        Experimental. Not supported.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/
#ifndef _FNET_OS_CONFIG_H_

#define _FNET_OS_CONFIG_H_

/*! @addtogroup fnet_os_config  */
/*! @{ */

/**************************************************************************/ /*!
 * @def      FNET_CFG_OS
 * @brief     Operation System support:
 *               - @c 1 = is enabled. The OS type is defined by the @ref FNET_CFG_OS_operation_system_type.
 *               - @b @c 0 = is disabled. It is used bare-metal FNET stack (Default value).
 ******************************************************************************/
#ifndef FNET_CFG_OS
    #define FNET_CFG_OS    (0)
#endif

/**************************************************************************/ /*!
 * @def      FNET_CFG_OS_operation_system_type
 * @brief    This is the set of the @c FNET_CFG_OS_[operation_system_type] definitions that
 *           define a currently used operation system. @n
 *           Current version of the FNET supports the following OS definitions:
 *            - @c FNET_CFG_OS_BRTOS    = Used OS is the BRTOS (http://code.google.com/p/brtos/).
 *            - @c FNET_CFG_OS_FREERTOS = Used OS is the FreeRTOS.
 *            @n @n
 *            Selected OS definition should be only one and must be defined as 1.
 *            All others may be defined but must have the 0 value.
 *
 ******************************************************************************/
#define FNET_CFG_OS_operation_system_type /* Ignore it. Just only for Doxygen documentation */

#if FNET_CFG_OS

    /*-----------*/

    #ifndef FNET_CFG_OS_BRTOS
        #define FNET_CFG_OS_BRTOS    (0)
    #endif

    #ifndef FNET_CFG_OS_FREERTOS
        #define FNET_CFG_OS_FREERTOS (0)
    #endif

    /*-----------*/
    #if FNET_CFG_OS_BRTOS /* BRTOS */
        #ifdef FNET_OS_STR
            #error "More than one OS selected FNET_OS_XXXX"
        #endif

        #include "fnet_brtos_config.h"
        #define FNET_OS_STR    "BRTOS"
    #endif

    #if FNET_CFG_OS_FREERTOS /* FREERTOS */
        #ifdef FNET_OS_STR
            #error "More than one OS selected FNET_OS_XXXX"
        #endif

        #include "fnet_freertos_config.h"
        #define FNET_OS_STR    "FreeRTOS"
    #endif
#else
    #define FNET_CFG_OS_BRTOS    (0)
    #define FNET_CFG_OS_FREERTOS (0)
#endif /* FNET_CFG_OS*/

/*-----------*/
#ifndef FNET_OS_STR
    #define FNET_OS_STR    "NONE"
#endif

/**************************************************************************/ /*!
 * @def      FNET_CFG_OS_MUTEX
 * @brief    OS Mutex support:
 *               - @c 1 = is enabled.
 *               - @b @c 0 = is disabled (Default value).
 ******************************************************************************/
#ifndef FNET_CFG_OS_MUTEX
    #define FNET_CFG_OS_MUTEX   (0)
#endif

/**************************************************************************/ /*!
 * @def      FNET_CFG_OS_ISR
 * @brief    OS-specific ISR handler:
 *               - @c 1 = is enabled. The fnet_os_isr() handler is used instead
 *                        of the fnet_cpu_isr() handler.
 *               - @b @c 0 = is disabled (Default value).
 ******************************************************************************/
#ifndef FNET_CFG_OS_ISR
    #define FNET_CFG_OS_ISR     (0)
#endif

/**************************************************************************/ /*!
 * @def      FNET_CFG_OS_EVENT
 * @brief    OS-specific event:
 *               - @c 1 = is enabled. It is raised when new data are arrived to
 *                        the Socket layer.
 *               - @b @c 0 = is disabled (Default value).
 ******************************************************************************/
#ifndef FNET_CFG_OS_EVENT
    #define FNET_CFG_OS_EVENT   (0)
#endif

/**************************************************************************/ /*!
 * @def      FNET_CFG_OS_TIMER
 * @brief    OS-specific timer initialization/release:
 *               - @c 1 = is enabled.@n
 *                        The fnet_os_timer_init() initialization is called instead of fnet_cpu_timer_init(),
 *                        and fnet_os_timer_relaese() is called instead of fnet_cpu_timer_release() .
 *               - @b @c 0 = is disabled (Default value).
 ******************************************************************************/
#ifndef FNET_CFG_OS_TIMER
    #define FNET_CFG_OS_TIMER   (0)
#endif

/*! @} */

#endif /* _FNET_OS_CONFIG_H_ */
