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
* FNET Azure-IOT client example.
*
***************************************************************************/

#ifndef _FAPP_AZURE_H_

#define _FAPP_AZURE_H_

#include "fapp_config.h"

#if FAPP_CFG_AZURE_CMD && FNET_CFG_AZURE

/* Number of Azure messages to be transmitted by "azure" command.
    Set to (0xFFFFFFFF) for infinit sending
    WARNING: The Azure free subscription supports only 8000 messages per day.*/
#ifndef FAPP_CFG_AZURE_CMD_MESSAGE_COUNT
    #define FAPP_CFG_AZURE_CMD_MESSAGE_COUNT    (0xFFFFFFFF)
#endif

/* Period between Azure messages.
    WARNING: The Azure free subscription supports only 8000 messages per day.*/
#ifndef FAPP_CFG_AZURE_CMD_MESSAGE_PERIOD
    #define FAPP_CFG_AZURE_CMD_MESSAGE_PERIOD   (15) /* sec */
#endif

/* Azure log support */
#ifndef FAPP_CFG_AZURE_CMD_LOG
    #define FAPP_CFG_AZURE_CMD_LOG              (0)
#endif

/* Azure packet trace support */
#ifndef FAPP_CFG_AZURE_CMD_TRACE
    #define FAPP_CFG_AZURE_CMD_TRACE            (0)
#endif


#if defined(__cplusplus)
extern "C" {
#endif


void fapp_azure_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );

#if defined(__cplusplus)
}
#endif

#endif

#endif
