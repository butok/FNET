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
* @file fapp_params_prv.h
*
* @author Andrey Butok
*
* @brief FNET Application parameters private API.
*
***************************************************************************/

#ifndef _FAPP_PARAMS_PRV_H_

#define _FAPP_PARAMS_PRV_H_

#include "fapp_params.h"

/************************************************************************
*    Definitions.
*************************************************************************/
extern fnet_char_t fapp_params_host_name[FAPP_PARAMS_HOST_NAME_SIZE];

#if FAPP_CFG_PARAMS_BOOT
    extern struct fapp_params_boot fapp_params_boot_config;
#endif
#if FAPP_CFG_PARAMS_TFTP
    extern struct fapp_params_tftp fapp_params_tftp_config;
#endif

#if defined(__cplusplus)
extern "C" {
#endif

fnet_return_t fapp_params_to_flash(void);
fnet_return_t fapp_params_from_flash(void);

#if defined(__cplusplus)
}
#endif


#endif /* _FAPP_PARAMS_PRV_H_ */
