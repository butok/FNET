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
* @file fnet_brtos.c
*
* @author Andrey Butok, Gustavo Denardin
*
* @date Dec-19-2012
*
* @version 0.1.3.0
*
* @brief Default BRTOS-specific functions. @n
*        Experimental. Not supported.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_OS && FNET_CFG_OS_BRTOS

#include "BRTOS.h"

/************************************************************************
*     Global Data Structures
*************************************************************************/
#define	FNET_BRTOS_MUTEX_PRIORITY	    (11)    /* Must be set to a priority 
                                                 * higher than the highest priority 
                                                 * task that will use fnet. */
#define FNET_BRTOS_SEMAPHORE_TIMEOUT    (3)

/************************************************************************
*     Global Data Structures
*************************************************************************/
BRTOS_Sem       *FnetSemaphore;
BRTOS_Mutex     *FnetMutex;

/************************************************************************
* NAME: fnet_os_isr;
*
* DESCRIPTION: This handler is executed on every FNET interrupt
*              (from ethernet and timer module).
*              Extructs vector number and calls fnet_isr_handler().
*************************************************************************/
void fnet_os_isr(void)
{
    /*******************************
     * OS-specific Interrupt Enter.
     *******************************/
    OS_SAVE_ISR();
    OS_INT_ENTER();

    /* Call original CPU handler*/
    fnet_cpu_isr();

    /*******************************
     * Interrupt Exit.
     *******************************/
    OS_INT_EXIT();
    OS_RESTORE_ISR();
}

/************************************************************************
* NAME: fnet_os_event_init
*
* DESCRIPTION:
*************************************************************************/
fnet_return_t fnet_os_event_init(void)
{
    if (OSSemCreate(0, &FnetSemaphore) == ALLOC_EVENT_OK)
        return FNET_OK;
    else
        return FNET_ERR;
}

/************************************************************************
* NAME: fnet_os_event_wait
*
* DESCRIPTION:
*************************************************************************/
void fnet_os_event_wait(void)
{
    (void)OSSemPend(FnetSemaphore, FNET_CFG_OS_SEMAPHORE_TIMEOUT);
}

/************************************************************************
* NAME: fnet_os_event_raise
*
* DESCRIPTION:
*************************************************************************/
void fnet_os_event_raise(void)
{
    (void)OSSemPost(FnetSemaphore);
}

/************************************************************************
* NAME: fnet_os_mutex_init
*
* DESCRIPTION:
*************************************************************************/
fnet_return_t fnet_os_mutex_init(void)
{
    if (OSMutexCreate(&FnetMutex, FNET_BRTOS_MUTEX_PRIORITY) == ALLOC_EVENT_OK)
        return FNET_OK;
    else
        return FNET_ERR;
}

/************************************************************************
* NAME: fnet_os_mutex_lock;
*
* DESCRIPTION:
*************************************************************************/
void fnet_os_mutex_lock(void)
{
    OSMutexAcquire(FnetMutex);
}

/************************************************************************
* NAME: fnet_os_mutex_unlock;
*
* DESCRIPTION:
*************************************************************************/
void fnet_os_mutex_unlock(void)
{
    OSMutexRelease(FnetMutex);
}

/************************************************************************
* NAME: fnet_os_mutex_release;
*
* DESCRIPTION:
*************************************************************************/
void fnet_os_mutex_release(void)
{

}


#endif /* FNET_CFG_OS && FNET_CFG_OS_BRTOS */
