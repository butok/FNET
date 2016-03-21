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
* @file fapp_prv.h
*
* @author Andrey Butok
*
* @brief FNET Application private definitions.
*
***************************************************************************/

#ifndef _FAPP_PRV_H_

#define _FAPP_PRV_H_

#include "fapp.h"
#include "fapp_params_prv.h"


/************************************************************************
*    Definitions.
*************************************************************************/
extern const fnet_char_t FAPP_HELP_STR[];
extern const fnet_char_t FAPP_DELIMITER_STR[];
extern const fnet_char_t FAPP_CANCELLED_STR[];
extern const fnet_char_t FAPP_TOCANCEL_STR[];
extern const fnet_char_t FAPP_COMMAND_RELEASE[]; /* Service release command */
extern const fnet_char_t FAPP_PARAM_ERR[];
extern const fnet_char_t FAPP_INIT_ERR[];
extern const fnet_char_t FAPP_SHELL_INFO_FORMAT_S[];
extern const fnet_char_t FAPP_SHELL_INFO_FORMAT_D[];
extern const fnet_char_t FAPP_SHELL_CANCELED_CTRL_C[];
extern const fnet_char_t *const fapp_enabled_str[];

/* Boot mode structure. */
struct boot_mode
{
    fapp_params_boot_mode_t index;
    fnet_char_t                    *name;
    void(* handler)(fnet_shell_desc_t desc);
};

extern const struct fnet_shell_command fapp_cmd_table [];

/************************************************************************
*     Function Prototypes
*************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

void fapp_help_cmd ( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
void fapp_shell_init( fnet_shell_desc_t desc );
void fapp_print_netif_info( fnet_shell_desc_t desc, fnet_netif_desc_t netif );
void fapp_print_netif_addr(fnet_shell_desc_t desc, fnet_address_family_t family, fnet_netif_desc_t netif, fnet_bool_t print_type);
void fapp_go_cmd ( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
const struct boot_mode *fapp_boot_mode_by_name (fnet_char_t *name);
const struct boot_mode *fapp_boot_mode_by_index (fapp_params_boot_mode_t index);

#if defined(__cplusplus)
}
#endif

#endif
