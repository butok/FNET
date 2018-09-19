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
*  FNET Azure-IOT tick counter adapter
*
***************************************************************************/
#include "fnet.h"

#if FNET_CFG_AZURE

#include "azure_c_shared_utility/gballoc.h"
#include "azure_c_shared_utility/tickcounter.h"
#include "azure_c_shared_utility/optimize_size.h"
#include "azure_c_shared_utility/xlogging.h"

typedef struct TICK_COUNTER_INSTANCE_TAG
{
    fnet_time_t original_ms;
} TICK_COUNTER_INSTANCE;

TICK_COUNTER_HANDLE tickcounter_create(void)
{
    TICK_COUNTER_INSTANCE *result = (TICK_COUNTER_INSTANCE *)malloc(sizeof(TICK_COUNTER_INSTANCE));
    if (result == NULL)
    {
        LogError("Failed creating tick counter");
    }
    else
    {
        result->original_ms = fnet_timer_get_ms();
    }
    return result;
}

void tickcounter_destroy(TICK_COUNTER_HANDLE tick_counter)
{
    if (tick_counter != NULL)
    {
        free(tick_counter);
    }
}

int tickcounter_get_current_ms(TICK_COUNTER_HANDLE tick_counter, tickcounter_ms_t *current_ms)
{
    int result;

    if (tick_counter == NULL || current_ms == NULL)
    {
        LogError("Invalid Arguments.");
        result = __FAILURE__;
    }
    else
    {
        *current_ms = (tickcounter_ms_t)( fnet_timer_get_ms() - tick_counter->original_ms);
        result = 0;
    }

    return result;
}

#endif /* FNET_CFG_AZURE */
