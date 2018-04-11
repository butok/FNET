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
*  FNET Demo API, FreeRTOS-specific.
*
***************************************************************************/

#ifndef _FAPP_FREERTOS_H_

#define _FAPP_FREERTOS_H_

#include "fapp_config.h"
#include "fnet.h"

#if defined(__cplusplus)
extern "C" {
#endif

#if FAPP_CFG_FREERTOS
void fapp_freertos_main(void); /* Main entry point of the FreeRTOS demo. */

void fapp_freertos_poll(void);
void fapp_freertos_params_init(struct fnet_init_params *init_params);
#endif

#if defined(__cplusplus)
}
#endif

#endif
