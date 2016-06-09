/**************************************************************************
*
* Copyright 2016 by Andrey Butok. FNET Community.
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
#ifndef _FNET_AUTOIP_CONFIG_H_

#define _FNET_AUTOIP_CONFIG_H_

/*! @addtogroup fnet_autoip_config */
/*! @{ */

/**************************************************************************/ /*!
 * @def      FNET_CFG_AUTOIP
 * @brief    Auto-IP service support (RFC3927 "Dynamic Configuration of IPv4 Link-Local Addresses"):
 *               - @c 1 = is enabled.
 *               - @b @c 0 = is disabled (Default value).
 ******************************************************************************/
#ifndef FNET_CFG_AUTOIP
    #define FNET_CFG_AUTOIP       (0)
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_AUTOIP_MAX
 * @brief   Maximum number of the Auto-IP services that can be run simultaneously. @n
 *          Default value is @b @c 1.
 *          @note It is not posible to run several Auto-IP servers on the same networking interface.
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_AUTOIP_MAX
    #define FNET_CFG_AUTOIP_MAX     (1u)
#endif


/*! @} */

#endif /*_FNET_AUTOIP_CONFIG_H_ */
