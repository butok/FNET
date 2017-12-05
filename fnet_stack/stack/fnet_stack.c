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
*  FNET init/release functions.
*
***************************************************************************/

#include "fnet.h"
#include "fnet_socket_prv.h"
#include "fnet_prot.h"
#include "fnet_stack_prv.h"
#include "fnet_loop.h"

/************************************************************************
*     Global Data Structures
*************************************************************************/
fnet_bool_t _fnet_is_enabled = FNET_FALSE;   /* Flag that the stack is initialized. */
#if FNET_CFG_MULTITHREADING
    const fnet_mutex_api_t  *fnet_mutex_api = FNET_NULL;
    static fnet_mutex_t fnet_stack_mutex = FNET_NULL;
#endif

/************************************************************************
*     Function Prototypes
*************************************************************************/
static fnet_return_t fnet_stack_init( void );
static void fnet_stack_release( void );

/************************************************************************
* DESCRIPTION:
*************************************************************************/
fnet_return_t fnet_init( struct fnet_init_params *init_params )
{
    fnet_return_t result = FNET_ERR;

    if(init_params && init_params->netheap_size)
    {
#if FNET_CFG_MULTITHREADING
        fnet_mutex_api = init_params->mutex_api;
#endif
        if(fnet_stack_mutex_init() == FNET_OK)
        {
            fnet_stack_mutex_lock();

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
            fnet_stack_mutex_unlock();
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION:
*************************************************************************/
void fnet_release(void)
{
    fnet_stack_mutex_lock();

    if(_fnet_is_enabled)
    {
        fnet_stack_release();
        _fnet_is_enabled = FNET_FALSE;
    }

    fnet_stack_mutex_unlock();

    fnet_stack_mutex_free();

#if FNET_CFG_MULTITHREADING
    fnet_mutex_api = NULL;
#endif
}

/************************************************************************
* DESCRIPTION: TCP/IP Stack initialization.
************************************************************************/
static fnet_return_t fnet_stack_init( void )
{
    fnet_isr_init();

    fnet_netif_list = 0;     /* Reset netif list. */
    fnet_netif_default = 0;  /* Reset default interface.*/

    /* Initialize Timer Module */
    if (fnet_timer_init(FNET_TIMER_PERIOD_MS) == FNET_ERR)
    {
        goto ERROR;
    }

#if FNET_CFG_DEBUG_STARTUP_MS && FNET_CFG_DEBUG
    /* Add start-up */
    fnet_println("\n Waiting %d Seconds...", FNET_CFG_DEBUG_STARTUP_MS / 1000);
    fnet_timer_delay(fnet_timer_ms2ticks(FNET_CFG_DEBUG_STARTUP_MS));
#endif

    /* Initialize protocol layer */
    if(fnet_prot_init() == FNET_ERR)
    {
        goto ERROR;
    }

    /* Initialize socket layer.*/
    if(fnet_socket_init() == FNET_ERR)
    {
        goto ERROR;
    }

#if FNET_CFG_LOOPBACK
    /* Initialize Loop-back interface.*/
    if(fnet_netif_init(FNET_LOOP_IF, FNET_NULL, 0u) == FNET_ERR)
    {
        goto ERROR;
    }
#endif

    return (FNET_OK);
ERROR:
    fnet_stack_release();

    return (FNET_ERR);
}

/************************************************************************
* DESCRIPTION: TCP/IP Stack release.
************************************************************************/
static void fnet_stack_release( void )
{
    fnet_netif_release_all();
    fnet_prot_release();
    fnet_timer_release();
    fnet_mem_release();
}

#if FNET_CFG_MULTITHREADING
fnet_return_t fnet_stack_mutex_init(void)
{
    fnet_return_t result;
    if(fnet_mutex_api) /* Check if multithreading is enabled.*/
    {
        if(fnet_mutex_api->mutex_init)
        {
            result = fnet_mutex_api->mutex_init(&fnet_stack_mutex);
        }
        else
        {
            result = FNET_ERR;
        }
    }
    else
    {
        result = FNET_OK; /* OK - it is just disabled.*/
    }

    return result;
}

void fnet_stack_mutex_lock(void)
{
    if(fnet_mutex_api) /* Check if multithreading is enabled.*/
    {
        if(fnet_mutex_api->mutex_lock)
        {
            fnet_mutex_api->mutex_lock(&fnet_stack_mutex);
        }
    }
}

void fnet_stack_mutex_unlock(void)
{
    if(fnet_mutex_api) /* Check if multithreading is enabled.*/
    {
        if(fnet_mutex_api->mutex_unlock)
        {
            fnet_mutex_api->mutex_unlock(&fnet_stack_mutex);
        }
    }
}

void fnet_stack_mutex_free(void)
{
    if(fnet_mutex_api) /* Check if multithreading is enabled.*/
    {
        if(fnet_mutex_api->mutex_free)
        {
            fnet_mutex_api->mutex_free(&fnet_stack_mutex);
        }
    }
}
#endif

