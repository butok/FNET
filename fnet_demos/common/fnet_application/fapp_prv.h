/**************************************************************************
*
* Copyright 2011-2017 by Andrey Butok. FNET Community.
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
***************************************************************************
*
*  FNET Application private definitions.
*
***************************************************************************/

#ifndef _FAPP_PRV_H_

#define _FAPP_PRV_H_

#include "fapp.h"
#include "fapp_params_prv.h"
#include "fapp_shell.h"

/************************************************************************
*    Definitions.
*************************************************************************/
/* Boot mode structure. */
struct boot_mode
{
    fapp_params_boot_mode_t index;
    fnet_char_t             *name;
    void(* handler)(fnet_shell_desc_t desc);
};

/************************************************************************
*     Function Prototypes
*************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif
const struct boot_mode *fapp_boot_mode_by_name (fnet_char_t *name);
const struct boot_mode *fapp_boot_mode_by_index (fapp_params_boot_mode_t index);
#if FNET_CFG_IP4 &&((FAPP_CFG_DHCPC_CMD && FNET_CFG_DHCP_CLN) || FNET_CFG_AUTOIP)
void fapp_addr_callback_updated(fnet_shell_desc_t desc, fnet_netif_desc_t netif);
#endif

#if defined(__cplusplus)
}
#endif

#endif
