/**************************************************************************
*
* Copyright 2008-2018 by Andrey Butok. FNET Community.
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
* @brief FNET Shell Demo API (Ping).
*
***************************************************************************/

#ifndef _FAPP_PING_H_

#define _FAPP_PING_H_

#include "fapp_config.h"

#if FNET_CFG_PING && (FAPP_CFG_PING_CMD || FAPP_CFG_PING6_CMD)

/* Default parameters.*/
#define FAPP_PING_DEFAULT_SIZE      (63U)
#define FAPP_PING_DEFAULT_NUMBER    (4U)
#define FAPP_PING_DEFAULT_TIMEOUT   (1000U) /* ms */
#define FAPP_PING_DEFAULT_PATTERN   (0x00U)
#define FAPP_PING_DEFAULT_HOP_LIMIT (64U)

#if defined(__cplusplus)
extern "C" {
#endif

void fapp_ping_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );

#if defined(__cplusplus)
}
#endif

#endif /* FNET_CFG_PING && (FAPP_CFG_PING_CMD || FAPP_CFG_PING6_CMD) */

#endif /* _FAPP_PING_H_ */
