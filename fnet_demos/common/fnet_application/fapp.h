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
*  FNET Shell Demo API.
*
***************************************************************************/

#ifndef _FAPP_H_

#define _FAPP_H_

#include "fapp_config.h"
#include "fnet.h"
#include "fapp_freertos.h"

#if defined(__cplusplus)
extern "C" {
#endif

void fapp_hw_init( void ); /* Board-specific HW initialization. Default serial port initialization. Interrupt enabling. */

void fapp_main( void ); /* Main entry point of the bare-metal shell demo. */

void fapp_poll(void);

#if defined(__cplusplus)
}
#endif

#endif
