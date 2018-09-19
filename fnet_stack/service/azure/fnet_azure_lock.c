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
*  FNET Azure-IOT Lock adapter
*
***************************************************************************/
#include "fnet.h"

#if FNET_CFG_AZURE

#include "stack/fnet_stack_prv.h"

#include "azure_c_shared_utility/lock.h"
#include "azure_c_shared_utility/xlogging.h"

LOCK_HANDLE Lock_Init(void)
{
#if FNET_CFG_MULTITHREADING
    fnet_mutex_t result = FNET_NULL;

    _fnet_mutex_init(&result);

    return (LOCK_HANDLE)result;
#else
    return (LOCK_HANDLE)1; /* To avoid error */
#endif
}

LOCK_RESULT Lock_Deinit(LOCK_HANDLE handle)
{
#if FNET_CFG_MULTITHREADING
    LOCK_RESULT result;

    if(handle)
    {
        _fnet_mutex_release(&handle);
        result = LOCK_OK;
    }
    else
    {
        result = LOCK_ERROR;
    }

    return result;
#else
    return LOCK_OK;
#endif
}

LOCK_RESULT Lock(LOCK_HANDLE handle)
{
#if FNET_CFG_MULTITHREADING
    LOCK_RESULT result;

    if(handle)
    {
        _fnet_mutex_lock(&handle);
        result = LOCK_OK;
    }
    else
    {
        result = LOCK_ERROR;
    }
    return result;
#else
    return LOCK_OK;
#endif
}

LOCK_RESULT Unlock(LOCK_HANDLE handle)
{
#if FNET_CFG_MULTITHREADING
    LOCK_RESULT result;

    if(handle)
    {
        _fnet_mutex_unlock(&handle);
        result = LOCK_OK;
    }
    else
    {
        result = LOCK_ERROR;
    }
    return result;
#else
    return LOCK_OK;
#endif
}

#endif /* FNET_CFG_AZURE */
