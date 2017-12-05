/**************************************************************************
*
* Copyright 2011-2016 by Andrey Butok. FNET Community.
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
*  Main including header for the FNET TCP/IP stack.
*
***************************************************************************/

#ifndef _FNET_STACK_PRV_H_

#define _FNET_STACK_PRV_H_

extern fnet_bool_t _fnet_is_enabled;

#if defined(__cplusplus)
extern "C" {
#endif

#if FNET_CFG_MULTITHREADING
fnet_return_t fnet_stack_mutex_init(void);
void fnet_stack_mutex_free(void);
void fnet_stack_mutex_lock(void);
void fnet_stack_mutex_unlock(void);
#else
#define fnet_stack_mutex_init()        FNET_OK
#define fnet_stack_mutex_free()        do{}while(0)
#define fnet_stack_mutex_lock()        do{}while(0)
#define fnet_stack_mutex_unlock()      do{}while(0)
#endif

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _FNET_STACK_H_ */

