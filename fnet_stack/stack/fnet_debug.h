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
* @file fnet_debug.h
*
* @brief Private. Debug definitions.
*
***************************************************************************/

#ifndef _FNET_DEBUG_H_

#define _FNET_DEBUG_H_

#include "fnet.h"

#if defined(__cplusplus)
extern "C" {
#endif

#if FNET_CFG_DEBUG

#define FNET_DEBUG(...) fnet_println(__VA_ARGS__)

#if FNET_CFG_DEBUG_STACK && FNET_CFG_DEBUG
extern fnet_size_t fnet_dbg_stack_max;
#endif

#else

#define FNET_DEBUG(...) do{}while(0)

#endif

#if defined(__cplusplus)
}
#endif


#endif
