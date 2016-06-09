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
***********************************************************************/
/*!
*
* @file fnet_services_config.h
*
* @author Andrey Butok
*
* @brief Services default configuration.
*
***************************************************************************/

/**************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 **************************************************************************/

#ifndef _FNET_SERVICES_CONFIG_H_

#define _FNET_SERVICES_CONFIG_H_

#include "fnet_config.h"

/*! @addtogroup fnet_services_config */
/*! @{ */

/**************************************************************************/ /*!
 * @def     FNET_CFG_POLL_MAX
 * @brief   Maximum number of registered services in the service-polling list.@n
 *          Default value is @b @c 8.
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_POLL_MAX
    #define FNET_CFG_POLL_MAX   (8u)
#endif

/**************************************************************************/ /*!
 * @def      FNET_CFG_FLASH
 * @brief    On-chip Flash driver support:
 *               - @c 1 = is enabled.
 *               - @b @c 0 = is disabled (Default value).
 ******************************************************************************/
#ifndef FNET_CFG_FLASH
    #define FNET_CFG_FLASH      (0)
#endif

#include "services/dhcp/fnet_dhcp_config.h"
#include "services/autoip/fnet_autoip_config.h"
#include "services/ssl/fnet_ssl_config.h"
#include "services/http/fnet_http_config.h"
#include "services/fs/fnet_fs_config.h"
#include "services/tftp/fnet_tftp_config.h"
#include "services/telnet/fnet_telnet_config.h"
#include "services/dns/fnet_dns_config.h"
#include "services/ping/fnet_ping_config.h"
#include "services/serial/fnet_serial_config.h"
#include "services/shell/fnet_shell_config.h"
#include "services/llmnr/fnet_llmnr_config.h"

/*! @} */

#endif /* _FNET_SERVICES_CONFIG_H_ */
