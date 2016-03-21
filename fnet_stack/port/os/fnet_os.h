/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
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
* @file fnet_os.h
*
* @author Andrey Butok
*
* @brief FNET OS API.
*
***************************************************************************/

#ifndef _FNET_OS_H_

#define _FNET_OS_H_

#include "fnet.h"

#if defined(__cplusplus)
extern "C" {
#endif

#if FNET_CFG_OS_MUTEX
fnet_return_t fnet_os_mutex_init(void);
void fnet_os_mutex_lock(void);
void fnet_os_mutex_unlock(void);
void fnet_os_mutex_release(void);
#else
#define fnet_os_mutex_init()        FNET_OK
#define fnet_os_mutex_lock()        do{}while(0)
#define fnet_os_mutex_unlock()      do{}while(0)
#define fnet_os_mutex_release()     do{}while(0)
#endif

#if FNET_CFG_OS_ISR
void fnet_os_isr(void);
#else
#define fnet_os_isr(void)           do{}while(0)
#endif

#if FNET_CFG_OS_EVENT
fnet_return_t fnet_os_event_init(void);
void fnet_os_event_wait(void);
void fnet_os_event_raise(void);
#else
#define fnet_os_event_init()        FNET_OK
#define fnet_os_event_wait()        do{}while(0)
#define fnet_os_event_raise()       do{}while(0)
#endif

fnet_return_t fnet_os_timer_init(fnet_time_t period_ms);
void fnet_os_timer_release(void);

#if defined(__cplusplus)
}
#endif

#endif /* _FNET_OS_H_ */
