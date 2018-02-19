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
*  FNET Shell Demo API.
*
***************************************************************************/

#ifndef _FAPP_H_

#define _FAPP_H_

#include "fapp_config.h"
#include "fnet.h"

#if defined(__cplusplus)
extern "C" {
#endif

void fapp_main( void ); /* Main entry point of the bare-metal shell demo. */
#if FAPP_CFG_FREERTOS
void fapp_main_freertos( void ); /* Main entry point of the FreeRTOS shell demo. */
#endif

void fapp_poll(void);

#if defined(__cplusplus)
}
#endif

#endif
