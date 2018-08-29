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
*  FNET Azure-IOT thread adapter
*
***************************************************************************/
#include "fnet.h"

#if FNET_CFG_AZURE

#include "azure_c_shared_utility/xlogging.h"
#include "azure_c_shared_utility/threadapi.h"

/* Stop for the specified time. ]*/
void ThreadAPI_Sleep(unsigned int milliseconds)
{
    fnet_timer_delay(milliseconds);
}

/* ThreadAPI_Create returns THREADAPI_ERROR. ]*/
THREADAPI_RESULT ThreadAPI_Create(THREAD_HANDLE *threadHandle, THREAD_START_FUNC func, void *arg)
{
    (void)threadHandle;
    (void)func;
    (void)arg;
    return THREADAPI_ERROR;
}

/* ThreadAPI_Join returns THREADAPI_ERROR. ]*/
THREADAPI_RESULT ThreadAPI_Join(THREAD_HANDLE threadHandle, int *res)
{
    (void)threadHandle;
    (void)res;
    return THREADAPI_ERROR;
}

/* ThreadAPI_Exit do nothing. */
void ThreadAPI_Exit(int res)
{
    (void)res;
}

#endif /* FNET_CFG_AZURE */
