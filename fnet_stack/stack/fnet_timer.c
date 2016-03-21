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
* @file fnet_timer.c
*
* @author Andrey Butok
*
* @brief SW timer implementation.
*
***************************************************************************/

#include "fnet.h"
#include "fnet_timer_prv.h"
#include "fnet_netbuf.h"

/* Queue of the software timers*/

struct fnet_net_timer
{
    struct fnet_net_timer *next; /* Next timer in list.*/
    fnet_time_t timer_cnt;     /* Timer counter. */
    fnet_time_t timer_rv;      /* Timer reference value. */
    void (*handler)(fnet_uint32_t cookie);   /* Timer handler. */
    fnet_uint32_t cookie;                /* Handler Cookie. */
};

static struct fnet_net_timer *fnet_tl_head = 0;

volatile static fnet_time_t fnet_current_time;

#if FNET_CFG_DEBUG_TIMER && FNET_CFG_DEBUG
    #define FNET_DEBUG_TIMER   FNET_DEBUG
#else
    #define FNET_DEBUG_TIMER(...)
#endif

/************************************************************************
* NAME: fnet_timer_init
*
* DESCRIPTION: Starts TCP/IP hardware timer. delay_ms - period of timer (ms)
*************************************************************************/
fnet_return_t fnet_timer_init( fnet_time_t period_ms )
{
    fnet_return_t result;

    fnet_current_time = 0u;           /* Reset RTC counter. */
    result = FNET_HW_TIMER_INIT(period_ms);  /* Start HW timer. */

    return result;
}

/************************************************************************
* NAME: fnet_timer_release
*
* DESCRIPTION: Frees the memory, which was allocated for all
*              TCP/IP timers, and removes hardware timer
*************************************************************************/
void fnet_timer_release( void )
{
    struct fnet_net_timer *tmp_tl;

    FNET_HW_TIMER_RELEASE();

    while(fnet_tl_head != 0)
    {
        tmp_tl = fnet_tl_head->next;

        fnet_free(fnet_tl_head);

        fnet_tl_head = tmp_tl;
    }
}

/************************************************************************
* NAME: fnet_timer_ticks
*
* DESCRIPTION: This function returns current value of the timer in ticks.
*************************************************************************/
fnet_time_t fnet_timer_ticks( void )
{
    return fnet_current_time;
}

/************************************************************************
* NAME: fnet_timer_seconds
*
* DESCRIPTION: This function returns current value of the timer in seconds.
*************************************************************************/
fnet_time_t fnet_timer_seconds( void )
{
    return (fnet_current_time / FNET_TIMER_TICK_IN_SEC);
}

/************************************************************************
* NAME: fnet_timer_ms
*
* DESCRIPTION: This function returns current value of the timer
* in milliseconds.
*************************************************************************/
fnet_time_t fnet_timer_ms( void )
{
    return (fnet_current_time * FNET_TIMER_PERIOD_MS);
}

/************************************************************************
* NAME: fnet_timer_ticks_inc
*
* DESCRIPTION: This function increments current value of the RTC counter.
*************************************************************************/
void fnet_timer_ticks_inc( void )
{
    fnet_current_time++;

#if FNET_CFG_DEBUG_TIMER && FNET_CFG_DEBUG
    if((fnet_current_time % 10) == 0)
        FNET_DEBUG_TIMER("!");
#endif
}

/************************************************************************
* NAME: fnet_timer_handler_bottom
*
* DESCRIPTION: Handles timer interrupts
*
*************************************************************************/
void fnet_timer_handler_bottom(fnet_uint32_t cookie)
{
    struct fnet_net_timer *timer = fnet_tl_head;

    FNET_COMP_UNUSED_ARG(cookie);

    while(timer)
    {
        if(fnet_timer_get_interval(timer->timer_cnt, fnet_current_time) >= timer->timer_rv)
        {
            timer->timer_cnt = fnet_current_time;

            if(timer->handler)
            {
                timer->handler(timer->cookie);
            }
        }

        timer = timer->next;
    }
}

/************************************************************************
* NAME: fnet_timer_new
*
* DESCRIPTION: Creates new software timer with the period
*
*************************************************************************/
fnet_timer_desc_t fnet_timer_new( fnet_time_t period_ticks, void (*handler)(fnet_uint32_t cookie), fnet_uint32_t cookie )
{
    struct fnet_net_timer *timer = FNET_NULL;

    if( period_ticks && handler )
    {
        timer = (struct fnet_net_timer *)fnet_malloc_zero(sizeof(struct fnet_net_timer));

        if(timer)
        {
            timer->next = fnet_tl_head;

            fnet_tl_head = timer;

            timer->timer_rv = period_ticks;
            timer->handler = handler;
            timer->cookie = cookie;
        }
    }

    return (fnet_timer_desc_t)timer;
}

/************************************************************************
* NAME: fnet_timer_free
*
* DESCRIPTION: Frees software timer, which is pointed by tl_ptr
*
*************************************************************************/
void fnet_timer_free( fnet_timer_desc_t timer )
{
    struct fnet_net_timer *tl = (struct fnet_net_timer *)timer;
    struct fnet_net_timer *tl_temp;

    if(tl)
    {
        if(tl == fnet_tl_head)
        {
            fnet_tl_head = fnet_tl_head->next;
        }
        else
        {
            tl_temp = fnet_tl_head;

            while(tl_temp->next != tl)
            {
                tl_temp = tl_temp->next;
            }

            tl_temp->next = tl->next;
        }

        fnet_free(tl);
    }
}

/************************************************************************
* NAME: fnet_timer_reset_all
*
* DESCRIPTION: Resets all timers' counters
*
*************************************************************************/
void fnet_timer_reset_all( void )
{
    struct fnet_net_timer *tl;

    tl = fnet_tl_head;

    while(tl != 0)
    {
        tl->timer_cnt = fnet_current_time;
        tl = tl->next;
    }
}

/************************************************************************
* NAME: fnet_timer_get_interval
*
* DESCRIPTION: Calaculates an interval between two moments of time
*
*************************************************************************/
fnet_time_t fnet_timer_get_interval( fnet_time_t start, fnet_time_t end )
{
    if(start <= end)
    {
        return (end - start);
    }
    else
    {
        return (0xffffffffu - start + end + 1u);
    }
}

/************************************************************************
* NAME: fnet_timer_delay
*
* DESCRIPTION: Do delay for a given number of timer ticks.
*
*************************************************************************/
void fnet_timer_delay( fnet_time_t delay_ticks )
{
    fnet_time_t start_ticks = fnet_current_time;

    while(fnet_timer_get_interval(start_ticks, fnet_timer_ticks()) < delay_ticks)
    {}
}

/************************************************************************
* NAME: fnet_timer_ms2ticks
*
* DESCRIPTION: Convert milliseconds to timer ticks.
*
*************************************************************************/
fnet_time_t fnet_timer_ms2ticks( fnet_time_t time_ms )
{
    return time_ms / FNET_TIMER_PERIOD_MS;
}
