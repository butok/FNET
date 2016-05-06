/**************************************************************************
*
* Copyright 2011-2016 by Andrey Butok. FNET Community.
* Copyright 2011 by Andrey Butok,Gordon Jahn. Freescale Semiconductor, Inc.
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
* @file fnet_mpc_timer.c
*
* @author Andrey Butok
*
* @date Dec-17-2012
*
* @version 0.1.1.0
*
* @brief MPC specific timers implementation.
*
***************************************************************************/
#include "fnet.h"

#if FNET_MPC
#include "stack/fnet_timer_prv.h"
#include "stack/fnet_isr.h"
#include "fnet_mpc.h"

#define FNET_TIMER_VECTOR_NUMBER (FNET_CFG_CPU_TIMER_VECTOR_NUMBER)     /* Number of timer interrupt vector.*/
#define FNET_TIMER_INT_LEVEL     (FNET_CFG_CPU_TIMER_VECTOR_PRIORITY)   /* Timer interrupt level. */
#define FNET_TIMER_NUMBER        (FNET_CFG_CPU_TIMER_NUMBER)            /* Timer number (according to UM) */
#define FNET_TIMER_CLKIN_PER_MS  (FNET_CPU_CLOCK_KHZ)


/************************************************************************
* NAME: fnet_cpu_timer_handler_top
*
* DESCRIPTION: Top interrupt handler. Increment fnet_current_time
*              and interrupt flag.
*************************************************************************/
static void fnet_cpu_timer_handler_top(fnet_uint32_t cookie )
{
    FNET_COMP_UNUSED_ARG(cookie);

    /* Clear timer event condition.*/
#if FNET_CFG_CPU_MPC5566
    FNET_MPC_EMIOS_CSR(FNET_TIMER_NUMBER)   = 0x1;
#else
    FNET_MPC_PITRTI_TFLG(FNET_TIMER_NUMBER) = 0x1;
#endif

    /* Update RTC counter.*/
    fnet_timer_ticks_inc();
}

/************************************************************************
* NAME: fnet_cpu_timer_init
*
* DESCRIPTION: Starts TCP/IP hardware timer. delay_ms - period of timer (ms)
*         e.g. Time-out period = (1/FNET_CFG_SYSTEM_CLOCK_KHZ)x(1)x(124+1)x528x100 = 100 ms
*************************************************************************/
fnet_return_t fnet_cpu_timer_init( fnet_time_t period_ms )
{
    fnet_return_t result;

    /* Install interrupt handler.
     */
    result = fnet_isr_vector_init(FNET_TIMER_VECTOR_NUMBER, fnet_cpu_timer_handler_top,
                                  fnet_timer_handler_bottom, FNET_TIMER_INT_LEVEL, 0);

    if(result == FNET_OK)
    {
#if FNET_CFG_CPU_MPC5566
        FNET_MPC_EMIOS_CADR(FNET_TIMER_NUMBER)  = 0;
        FNET_MPC_EMIOS_CBDR(FNET_TIMER_NUMBER)  = period_ms * FNET_TIMER_CLKIN_PER_MS;
        FNET_MPC_EMIOS_CCR(FNET_TIMER_NUMBER)   = 0x82020658;
#else
#if FNET_CFG_CPU_MPC5744P
        /* FRZ = 1 (stopped in Debug mode), MDIS = 0 */
        FNET_MPC_PITRTI_MCR = 0x01;
#else
        FNET_MPC_PITRTI_MCR = 0x04;
#endif
        FNET_MPC_PITRTI_TCTRL(FNET_TIMER_NUMBER) = 0x0;
#if FNET_CFG_CPU_MPC5744P
        /* assumes SYS_CLK = 200MHz and MC_CGM_SC_DC0 = 4, then PBRIDGE_CLK is /4 */
        FNET_MPC_PITRTI_LDVAL(FNET_TIMER_NUMBER) = period_ms * (FNET_TIMER_CLKIN_PER_MS / 4);
#else
        FNET_MPC_PITRTI_LDVAL(FNET_TIMER_NUMBER) = (period_ms * FNET_TIMER_CLKIN_PER_MS) - 1;
#endif
        FNET_MPC_PITRTI_TCTRL(FNET_TIMER_NUMBER) = 0x3;
#endif
    }

    return result;
}

/************************************************************************
* NAME: fnet_cpu_timer_release
*
* DESCRIPTION: Relaeses TCP/IP hardware timer.
*
*************************************************************************/
void fnet_cpu_timer_release( void )
{
#if FNET_CFG_CPU_MPC5566
    FNET_MPC_EMIOS_CCR(FNET_TIMER_NUMBER)   = 0;
    FNET_MPC_EMIOS_CSR(FNET_TIMER_NUMBER)   = 0x1;
#else
    FNET_MPC_PITRTI_TCTRL(FNET_TIMER_NUMBER) = 0x0;
    FNET_MPC_PITRTI_TFLG(FNET_TIMER_NUMBER) = 0x1;
#endif

    /* Free interrupt handler res.
     */
    fnet_isr_vector_release(FNET_TIMER_VECTOR_NUMBER);
}

#endif /*FNET_MPC*/
