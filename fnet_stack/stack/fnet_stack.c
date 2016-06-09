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
* @file fnet_stack.c
*
* @brief FNET init/release functions.
*
***************************************************************************/

#include "fnet.h"
#include "fnet_socket_prv.h"
#include "fnet_prot.h"

/************************************************************************
*     Function Prototypes
*************************************************************************/
static fnet_return_t fnet_stack_init( void );
static void fnet_stack_release( void );

/************************************************************************
* NAME: fnet_init
*
* DESCRIPTION:
*************************************************************************/
fnet_return_t fnet_init( struct fnet_init_params *init_params )
{
    fnet_return_t result = FNET_ERR;

    if(init_params
       && (fnet_os_mutex_init() == FNET_OK)
       && (fnet_os_event_init() == FNET_OK))
    {
        fnet_os_mutex_lock();

        if(_fnet_is_enabled == FNET_FALSE) /* Is enabled already?. */
        {
            if((result = fnet_heap_init(init_params->netheap_ptr, init_params->netheap_size)) == FNET_OK )
            {
                if((result = fnet_stack_init()) == FNET_OK)
                {
                    _fnet_is_enabled = FNET_TRUE; /* Mark the stack is enabled. */
                }
            }
        }

        fnet_os_mutex_unlock();
    }

    return result;
}

/************************************************************************
* NAME: fnet_init_static
*
* DESCRIPTION:
*************************************************************************/
fnet_return_t fnet_init_static(void)
{
    static fnet_uint8_t heap[FNET_CFG_HEAP_SIZE];
    struct fnet_init_params init_params;

    init_params.netheap_ptr = heap;
    init_params.netheap_size = FNET_CFG_HEAP_SIZE;

    return fnet_init(&init_params);
}

/************************************************************************
* NAME: fnet_release
*
* DESCRIPTION:
*************************************************************************/
void fnet_release(void)
{
    fnet_os_mutex_lock();

    if(_fnet_is_enabled)
    {
        fnet_stack_release();
        _fnet_is_enabled = FNET_FALSE;
    }

    fnet_os_mutex_unlock();

    fnet_os_mutex_release();
}

/************************************************************************
* NAME: fnet_stack_init
*
* DESCRIPTION: TCP/IP Stack initialization.
************************************************************************/
static fnet_return_t fnet_stack_init( void )
{
    fnet_isr_init();

    if (fnet_timer_init(FNET_TIMER_PERIOD_MS) == FNET_ERR)
    {
        goto ERROR;
    }

#if FNET_CFG_DEBUG_STARTUP_MS && FNET_CFG_DEBUG
    fnet_println("\n Waiting %d Seconds...", FNET_CFG_DEBUG_STARTUP_MS / 1000);
    fnet_timer_delay(fnet_timer_ms2ticks(FNET_CFG_DEBUG_STARTUP_MS));
#endif

    if(fnet_prot_init() == FNET_ERR)
    {
        goto ERROR;
    }
    fnet_socket_init();

    if(fnet_netif_init_all() == FNET_ERR)
    {
        goto ERROR;
    }

    return (FNET_OK);
ERROR:
    fnet_stack_release();

    return (FNET_ERR);
}

/************************************************************************
* NAME: fnet_stack_release
*
* DESCRIPTION: TCP/IP Stack release.
************************************************************************/
static void fnet_stack_release( void )
{
    fnet_netif_release_all();
    fnet_prot_release();
    fnet_timer_release();
    fnet_mem_release();
}
