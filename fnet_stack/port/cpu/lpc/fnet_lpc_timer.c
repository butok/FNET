/**************************************************************************
*
* Copyright 2017-2018 by Andrey Butok. FNET Community.
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
*  LPC specific SW timers implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_LPC
#include "stack/fnet_timer_prv.h"

/* MCUXpresso */
#include <fsl_common.h>
#include <fsl_clock.h>
#include <fsl_ctimer.h>

static void fnet_cpu_timer_handler_top(void *cookie);

/* Array of CTIMER base addresses. */
static CTIMER_Type *const fnet_lpc_ctimer_base_addr[] = CTIMER_BASE_PTRS;

/************************************************************************
* DESCRIPTION: Top interrupt handler.
*************************************************************************/
static void fnet_cpu_timer_handler_top(void *cookie)
{
    CTIMER_Type             *base;

    base = fnet_lpc_ctimer_base_addr[FNET_CFG_CPU_TIMER_NUMBER];

    /* Get Interrupt status flags and clear it*/
    base->IR = base->IR;

    /* Update FNET timer tick counter.*/
    _fnet_timer_ticks_inc();
}

/************************************************************************
* DESCRIPTION: Starts TCP/IP hardware timer. delay_ms - period of timer (ms)
*         e.g. Time-out period = (1/FNET_CFG_SYSTEM_CLOCK_KHZ)x(1)x(124+1)x528x100 = 100 ms
*************************************************************************/
fnet_return_t fnet_cpu_timer_init( fnet_time_t period_ms )
{
    fnet_return_t           result;
    ctimer_config_t         config;
    ctimer_match_config_t   matchConfig;
    CTIMER_Type             *base;
    clock_name_t            clock_name;

    base = fnet_lpc_ctimer_base_addr[FNET_CFG_CPU_TIMER_NUMBER];


    /* Install interrupt handler and enable interrupt in NVIC.
    */
    result = _fnet_isr_vector_init(FNET_CFG_CPU_TIMER_VECTOR_NUMBER, fnet_cpu_timer_handler_top,
                                   _fnet_timer_handler_bottom,
                                   FNET_CFG_CPU_TIMER_VECTOR_PRIORITY, 0u);
    if(result == FNET_OK)
    {
        /*UM: Set the appropriate bits to enable clocks to timers that will be used:
        * CTIMER0 and CTIMER1, and CTIMER2 in the AHBCLKCTRL1 register,
        * CTIMER3 and CTIMER4 in the ASYNCAPBCLKCTRL register  */
#if (FNET_CFG_CPU_TIMER_NUMBER == 0) || (FNET_CFG_CPU_TIMER_NUMBER == 1) || (FNET_CFG_CPU_TIMER_NUMBER == 2)
        clock_name = kCLOCK_BusClk;
#elif (FNET_CFG_CPU_TIMER_NUMBER == 3) || (FNET_CFG_CPU_TIMER_NUMBER == 4)
        clock_name = kCLOCK_AsyncApbClk;
        /* Use 12 MHz clock.
        Asynchronous APB bridge is enabled inside this function. */
        CLOCK_AttachClk(kFRO12M_to_ASYNC_APB);
#else
#error Not suported FNET_CFG_CPU_TIMER_NUMBER
#endif

        /* Set CTIMER driver parameters. */
        CTIMER_GetDefaultConfig(&config);

        CTIMER_Init(base, &config);

        matchConfig.enableCounterReset = true;
        matchConfig.enableCounterStop = false;
        matchConfig.matchValue = (CLOCK_GetFreq(clock_name) * period_ms) / 1000;
        matchConfig.enableInterrupt = true;

        CTIMER_SetupMatch(base, kCTIMER_Match_0, &matchConfig);
        CTIMER_StartTimer(base);
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Release TCP/IP hardware timer.
*************************************************************************/
void fnet_cpu_timer_release( void )
{
    CTIMER_Type             *base;

    base = fnet_lpc_ctimer_base_addr[FNET_CFG_CPU_TIMER_NUMBER];

    CTIMER_Deinit(base);
}

/* If vector table is in ROM, pre-install FNET ISR for the Timer Event interrupt*/
#if !FNET_CFG_CPU_VECTOR_TABLE_IS_IN_RAM
#if FNET_CFG_CPU_TIMER_NUMBER == 0
void CTIMER0_IRQHandler (void)
{
    FNET_ISR_HANDLER();
}
#elif FNET_CFG_CPU_TIMER_NUMBER == 1
void CTIMER1_IRQHandler (void)
{
    FNET_ISR_HANDLER();
}
#elif FNET_CFG_CPU_TIMER_NUMBER == 2
void CTIMER2_IRQHandler (void)
{
    FNET_ISR_HANDLER();
}
#elif FNET_CFG_CPU_TIMER_NUMBER == 3
void CTIMER3_IRQHandler (void)
{
    FNET_ISR_HANDLER();
}
#elif FNET_CFG_CPU_TIMER_NUMBER == 4
void CTIMER4_IRQHandler (void)
{
    FNET_ISR_HANDLER();
}
#endif
#endif

#endif /*FNET_LPC*/
