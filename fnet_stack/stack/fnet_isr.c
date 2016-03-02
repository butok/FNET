/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
* Copyright 2008-2010 by Andrey Butok. Freescale Semiconductor, Inc.
* Copyright 2003 by Andrey Butok. Motorola SPS.
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
* @file fnet_isr.c
*
* @author Andrey Butok
*
* @brief Interrupt service dispatcher implementation.
*
***************************************************************************/

#include "fnet.h"
#include "fnet_isr.h"
#include "fnet_timer.h"
#include "fnet_netbuf.h"

/************************************************************************
*     Interrupt entry.
*************************************************************************/
typedef struct fnet_isr_entry
{
    struct fnet_isr_entry *next;               /* Next isr in chain */
    fnet_uint32_t vector_number;               /* Vector number */
    void (*handler_top)(fnet_uint32_t cookie); /* "Critical handler" - it will
                                        * be called every time on interrupt event,
                                        * (e.g. user can put here clear flags etc.)*/

    void (*handler_bottom)(fnet_uint32_t cookie); /* "Bottom half handler" - it will be called after
                                           *  isr_handler_top() in case NO SW lock
                                           *  or on SW unlock.*/
    fnet_bool_t pended;                           /* indicates interrupt pending */
    fnet_uint32_t cookie;                         /* Handler Cookie. */
} fnet_isr_entry_t;

/************************************************************************
*     Function Prototypes
*************************************************************************/
static fnet_return_t fnet_isr_register(fnet_uint32_t vector_number,
                                       void (*handler_top)(fnet_uint32_t cookie),
                                       void (*handler_bottom)(fnet_uint32_t cookie),
                                       fnet_uint32_t cookie);

/************************************************************************
*     Variables,
*************************************************************************/
static fnet_uint32_t fnet_locked = 0u;
static fnet_isr_entry_t *fnet_isr_table = 0;

static fnet_event_desc_t fnet_event_desc_last = FNET_EVENT_VECTOR_NUMBER;

/************************************************************************
* NAME: fnet_isr_handler
*
* DESCRIPTION: This handler is envoked by fnet_cpu_isr().
*              If fnet_locked == 0 - executes the
*              corresponding handler; else - marks it as pended.
*************************************************************************/
void fnet_isr_handler(fnet_uint32_t vector_number)
{
    fnet_isr_entry_t *isr_cur;

    /* This function operates as follows:
     * Searches through linked list of ISR Descriptor Table entries until
     * match is found with vector_number. Calls handler_top().
     * If local global "fnet_locked" is set, flags this interrupt as "pended" and exits.
     * Else, clears this interrupt's "pended" flag, executes handler_bottom() and exits.
     * NOTE: fnet_locked is incremented by fnet_isr_lock() and
     * decremented by fnet_isr_unlock().
     */

    for (isr_cur = fnet_isr_table; isr_cur; isr_cur = isr_cur->next)
    {
        if (isr_cur->vector_number == vector_number) /* we got it. */
        {
            if (isr_cur->handler_top)
            {
                isr_cur->handler_top(isr_cur->cookie); /* Call "top half" handler. */
            }

            if (fnet_locked)
            {
                isr_cur->pended = FNET_TRUE;
            }
            else
            {
                isr_cur->pended = FNET_FALSE;

                if (isr_cur->handler_bottom)
                {
                    isr_cur->handler_bottom(isr_cur->cookie); /* Call "bottom half" handler.*/
                }
            }

            break;
        }
    }
}

/************************************************************************
* NAME: fnet_isr_vector_init
*
* DESCRIPTION: Sets 'handler' for the interrupt vector with number
*              'vector_number' at the internal vector queue and interrupt
*              handler 'fnet_cpu_isr' at the real vector table
*************************************************************************/
fnet_return_t fnet_isr_vector_init(fnet_uint32_t vector_number,
                                   void (*handler_top)(fnet_uint32_t cookie),
                                   void (*handler_bottom)(fnet_uint32_t cookie),
                                   fnet_uint32_t priority,
                                   fnet_uint32_t cookie)
{
    fnet_return_t result;

    /* Register handler. */
    result = fnet_isr_register(vector_number, handler_top, handler_bottom, cookie);

    if (result == FNET_OK)
    {
        /* CPU-specific initalisation. */
        result = fnet_cpu_isr_install(vector_number, priority);
    }

    return result;
}

/************************************************************************
* NAME: fnet_event_init
*
* DESCRIPTION: Register the event handler.
*************************************************************************/
fnet_event_desc_t fnet_event_init(void (*event_handler)(fnet_uint32_t cookie), fnet_uint32_t cookie)
{
    fnet_uint32_t vector_number = (fnet_uint32_t)(fnet_event_desc_last++);

    if (fnet_isr_register(vector_number, 0, event_handler, cookie) == FNET_ERR)
    {
        return FNET_ERR;
    }
    else
    {
        return (fnet_event_desc_t)vector_number;
    }
}

/************************************************************************
* NAME: fnet_isr_register
*
* DESCRIPTION: Register 'handler' at the isr table.
*************************************************************************/
static fnet_return_t fnet_isr_register(fnet_uint32_t vector_number,
                                       void (*handler_top)(fnet_uint32_t handler_top_cookie),
                                       void (*handler_bottom)(fnet_uint32_t handler_bottom_cookie),
                                       fnet_uint32_t cookie)
{
    fnet_return_t result;
    fnet_isr_entry_t *isr_temp;

    isr_temp = (fnet_isr_entry_t *)fnet_malloc_zero(sizeof(fnet_isr_entry_t));

    if (isr_temp)
    {
        isr_temp->vector_number = vector_number;
        isr_temp->handler_top = (void (*)(fnet_uint32_t handler_top_cookie))handler_top;
        isr_temp->handler_bottom = (void (*)(fnet_uint32_t handler_bottom_cookie))handler_bottom;
        isr_temp->next = fnet_isr_table;
        isr_temp->cookie = cookie;
        fnet_isr_table = isr_temp;

        result = FNET_OK;
    }
    else
    {
        result = FNET_ERR;
    }

    return result;
}

/************************************************************************
* NAME: fnet_isr_vector_release
*
* DESCRIPTION: Sets the interrupt handler 'handler' for the interrupt vector
*              with number 'vector_number' at the exception vector table but
*              destroys info about old interrupt handler and removes
*              information from 'fnet_isr_table' queue
*************************************************************************/
void fnet_isr_vector_release(fnet_uint32_t vector_number)
{
    fnet_isr_entry_t *isr_temp;

    isr_temp = fnet_isr_table;

    while (isr_temp != 0)
    {
        if (isr_temp->vector_number == vector_number)
        {
            break;
        }

        isr_temp = isr_temp->next;
    }

    if (isr_temp != 0) /* if handler wasn't registered in queue */
    {
        fnet_free(isr_temp);

        if (fnet_isr_table->vector_number == vector_number)
        {
            fnet_isr_table = fnet_isr_table->next;
        }
        else
        {
            isr_temp = fnet_isr_table;

            while (isr_temp->next != 0)
            {
                if (isr_temp->next->vector_number == vector_number)
                {
                    isr_temp->next = isr_temp->next->next;
                    break;
                }

                isr_temp = isr_temp->next;
            }
        }
    }
}

/************************************************************************
* NAME: fnet_isr_lock
*
* DESCRIPTION: After execution of this routine all interrupts will be
*              pended
*************************************************************************/
void fnet_isr_lock(void)
{
    fnet_locked++;
}

/************************************************************************
* NAME: fnet_isr_unlock
*
* DESCRIPTION: Executes all pending interrupt handlers and
*              enables hardware interrupts processing
*************************************************************************/
#if 1 /* Old version.*/
void fnet_isr_unlock(void)
{
    fnet_isr_entry_t *isr_temp;

    /* This function operates as follows:
       * If local global "fnet_locked" == 0 then it simply returns.
       * Else, if fnet_locked == 1 (at topmost lock level),
       * index through ISR Descriptor Table until 1st pended interrupt is found.
       * Clear pended status and call associated handler_bottom().
       * Continue to do this until all pended interrupts have been handled.
       *Always exits by decrementing fnet_locked so as to bump up a lock level.
    */

    if (fnet_locked == 1u)
    {
        isr_temp = fnet_isr_table;

        while (isr_temp != 0)
        {
            if (isr_temp->pended == FNET_TRUE)
            {
                isr_temp->pended = FNET_FALSE;

                if (isr_temp->handler_bottom)
                {
                    isr_temp->handler_bottom(isr_temp->cookie);
                }
            }
            isr_temp = isr_temp->next;
        }
    }

    --fnet_locked;
}
#else /* new one. - but it looks like it is not too stable. */
void fnet_isr_unlock(void)
{
    fnet_isr_entry_t *isr_temp;

    /* This function operates as follows:
       * If local global "fnet_locked" == 0 then it simply returns.
       * Else, if fnet_locked == 1 (at topmost lock level),
       * index through ISR Descriptor Table until 1st pended interrupt is found.
       * Clear pended status and call associated handler_bottom().
       * Continue to do this until all pended interrupts have been handled.
       *Always exits by decrementing fnet_locked so as to bump up a lock level.
    */

    --fnet_locked;

    if (fnet_locked == 0)
    {
        isr_temp = fnet_isr_table;

        while (isr_temp != 0)
        {
            if (isr_temp->pended == FNET_TRUE)
            {
                isr_temp->pended = FNET_FALSE;

                if (isr_temp->handler_bottom)
                    isr_temp->handler_bottom(isr_temp->cookie);
            }

            isr_temp = isr_temp->next;
        }
    }
}
#endif

/************************************************************************
* NAME: fnet_event_raise
*
* DESCRIPTION: This function raise registerted event.
*************************************************************************/
void fnet_event_raise(fnet_event_desc_t event_number)
{
    fnet_uint32_t vector_number = (fnet_uint32_t)(event_number);
    fnet_isr_entry_t *isr_temp;

    isr_temp = fnet_isr_table;

    fnet_isr_lock();

    while (isr_temp != 0)
    {
        if (isr_temp->vector_number == vector_number)
        {
            if (isr_temp->handler_top)
            {
                isr_temp->handler_top(isr_temp->cookie);
            }

            if (fnet_locked == 1u)
            {
                isr_temp->pended = FNET_FALSE;

                if (isr_temp->handler_bottom)
                {
                    isr_temp->handler_bottom(isr_temp->cookie);
                }
            }
            else
            {
                isr_temp->pended = FNET_TRUE;
            }

            break;
        }

        isr_temp = isr_temp->next;
    }

    fnet_isr_unlock();
}

/************************************************************************
* NAME: fnet_isr_init
*
* DESCRIPTION: This function inits ISR module
*
*************************************************************************/
void fnet_isr_init(void)
{
    fnet_locked = 0u;
    fnet_isr_table = 0;
}
