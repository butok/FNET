/**************************************************************************
*
* Copyright 2012-2016 by Andrey Butok. FNET Community.
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
**********************************************************************/ /*!
*
* @file fnet_freertos.c
*
* @author Christophe Graulle, Andrey Butok
*
* @brief Default FreeRTOS-specific functions. @n
*        Experimental. Not supported.
*
***************************************************************************/

#include	"fnet.h"

#if FNET_CFG_OS && FNET_CFG_OS_FREERTOS

#include	"FreeRTOS.h"
#include	"semphr.h"

#if FNET_CFG_OS_EVENT

xSemaphoreHandle FNetSemaphore = NULL;
portBASE_TYPE l = pdFALSE;

fnet_return_t fnet_os_event_init(void)
{
    if (FNetSemaphore == NULL)
    {
        vSemaphoreCreateBinary( FNetSemaphore);

        if (FNetSemaphore == NULL)
            return FNET_ERR;
        else
            return FNET_OK;
    }
    else
    {
        return FNET_OK;
    }
}

void fnet_os_event_wait(void)
{
    xSemaphoreTake( FNetSemaphore, portMAX_DELAY);
}

void fnet_os_event_raise(void)
{
    portBASE_TYPE    ret;

    ret = xSemaphoreGiveFromISR( FNetSemaphore, &l);


    portEND_SWITCHING_ISR( l );

    if ( ret != pdTRUE )
    {
        ret = 1;
    }
}
#endif /* FNET_CFG_OS_EVENT */


#if FNET_CFG_OS_MUTEX

/************************************************************************
*     Globals
*************************************************************************/
xSemaphoreHandle FNetMutex;

/************************************************************************
* NAME: fnet_os_mutex_init
*
* DESCRIPTION:
*************************************************************************/
fnet_return_t fnet_os_mutex_init(void)
{
    FNetMutex = xSemaphoreCreateRecursiveMutex();
    if ( FNetMutex == NULL )
        return FNET_ERR;
    else
        return FNET_OK;
}

/************************************************************************
* NAME: fnet_os_mutex_lock;
*
* DESCRIPTION:
*************************************************************************/
void fnet_os_mutex_lock(void)
{
    xSemaphoreTakeRecursive( FNetMutex, portMAX_DELAY );
}

/************************************************************************
* NAME: fnet_os_mutex_unlock;
*
* DESCRIPTION:
*************************************************************************/
void fnet_os_mutex_unlock(void)
{
    xSemaphoreGiveRecursive( FNetMutex );
}

/************************************************************************
* NAME: fnet_os_mutex_release;
*
* DESCRIPTION:
*************************************************************************/
void fnet_os_mutex_release(void)
{
    vSemaphoreDelete( FNetMutex );
}
#endif /* FNET_CFG_OS_MUTEX */

#endif
