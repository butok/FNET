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
*  iMX RT specific SW timers implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_MIMXRT
#include "stack/fnet_timer_prv.h"

/*MCUX*/
#include "fsl_clock.h"

#if (!FNET_CFG_MIMXRT_TIMER_PIT && !FNET_CFG_MIMXRT_TIMER_QTMR) || (FNET_CFG_MIMXRT_TIMER_PIT && FNET_CFG_MIMXRT_TIMER_QTMR)
    #error "Choose and set FNET_CFG_MIMXRT_TIMER_PIT or FNET_CFG_MIMXRT_TIMER_QTMR"
#endif

/* ==== PIT Timer =========== */
#if FNET_CFG_MIMXRT_TIMER_PIT
/******************************************************************************
 *  Vector number of the timer interrupt.
 *  NOTE: User application should not change this parameter.
 ******************************************************************************/
#ifndef FNET_CFG_CPU_TIMER_VECTOR_NUMBER
    #define FNET_CFG_CPU_TIMER_VECTOR_NUMBER        (16+122/*irq*/)
#endif

static void fnet_cpu_timer_handler_top(void *cookie);

/************************************************************************
* DESCRIPTION: Top interrupt handler. Increment fnet_current_time
*              and clear interrupt flag.
*************************************************************************/
static void fnet_cpu_timer_handler_top(void *cookie)
{
    /* Clear the PIT timer flag. */
    FNET_MIMXRT_PIT_TFLG(FNET_CFG_CPU_TIMER_NUMBER) |= FNET_MIMXRT_PIT_TFLG_TIF_MASK;

#if 0 /* It looks like it is not needed for i.MX RT PIT */
    /* Read the load value to restart the timer. */
    (void)FNET_MIMXRT_PIT_LDVAL(FNET_CFG_CPU_TIMER_NUMBER);
#endif

    /* Update FNET RTC counter. */
    _fnet_timer_ticks_inc();
}

/************************************************************************
* DESCRIPTION: Starts TCP/IP hardware timer. period_ms - period of timer (ms)
*************************************************************************/
fnet_return_t fnet_cpu_timer_init( fnet_time_t period_ms )
{
    fnet_return_t   result;
    fnet_uint32_t   timeout;

    /* Install interrupt handler and enable interrupt in NVIC. */
    result = _fnet_isr_vector_init(FNET_CFG_CPU_TIMER_VECTOR_NUMBER, fnet_cpu_timer_handler_top,
                                   _fnet_timer_handler_bottom,
                                   FNET_CFG_CPU_TIMER_VECTOR_PRIORITY, 0u);
    if(result == FNET_OK)
    {
        /* Initialize the PIT timer to generate an interrupt every period_ms */

        /* Enable the clock to the PIT module. */
        CLOCK_EnableClock(kCLOCK_Pit);

        /* Enable the PIT timer module. */
        FNET_MIMXRT_PIT_MCR &= ~FNET_MIMXRT_PIT_MCR_MDIS_MASK;
        /* Timer is stopped in Debug mode. */
        FNET_MIMXRT_PIT_MCR |= FNET_MIMXRT_PIT_MCR_FRZ_MASK;

        /* Calculate the timeout value. */
        timeout = period_ms * FNET_MIMXRT_PERIPH_CLOCK_KHZ;
        FNET_MIMXRT_PIT_LDVAL(FNET_CFG_CPU_TIMER_NUMBER) = timeout;

        /* Enable the timer and enable interrupts */
        FNET_MIMXRT_PIT_TCTRL(FNET_CFG_CPU_TIMER_NUMBER) |= FNET_MIMXRT_PIT_TCTRL_TEN_MASK | FNET_MIMXRT_PIT_TCTRL_TIE_MASK;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Release TCP/IP hardware timer.
*************************************************************************/
void fnet_cpu_timer_release( void )
{
    /* Disable the timer and disable interrupts */
    FNET_MIMXRT_PIT_TCTRL(FNET_CFG_CPU_TIMER_NUMBER) &= ~(FNET_MIMXRT_PIT_TCTRL_TEN_MASK | FNET_MIMXRT_PIT_TCTRL_TIE_MASK);

    /* Uninstall interrupt handler. */
    fnet_isr_unregister(FNET_CFG_CPU_TIMER_VECTOR_NUMBER);
}

/* If vector table is in ROM, pre-install FNET ISR for the Timer Event interrupt*/
#if !FNET_CFG_CPU_VECTOR_TABLE_IS_IN_RAM
void PIT_IRQHandler (void)
{
    FNET_ISR_HANDLER();
}
#endif

#endif /* FNET_CFG_MIMXRT_TIMER_PIT */

/* ==== QTMR Timer =========== */
#if FNET_CFG_MIMXRT_TIMER_QTMR

/* MCUXpresso */
#include <fsl_common.h>
#include <fsl_clock.h>
#include <fsl_qtmr.h>

/******************************************************************************
 *  Vector number of the timer interrupt.
 *  NOTE: User application should not change this parameter.
 ******************************************************************************/
#ifndef FNET_CFG_CPU_TIMER_VECTOR_NUMBER
    #define FNET_CFG_CPU_TIMER_VECTOR_NUMBER        (16+133 + (FNET_CFG_CPU_TIMER_NUMBER/*starts from 1*/-1)/*irq*/)
#endif

#define FNET_MIMXRT_QTMR_CHANNEL        kQTMR_Channel_0 /* QTMR channel number 0..3 */

static void fnet_cpu_timer_handler_top(void *cookie);

/* Array of TMR base addresses. */
static TMR_Type *const fnet_mimxrt_qtmr_base_addr[] = TMR_BASE_PTRS;

/************************************************************************
* DESCRIPTION: Top interrupt handler. Increment fnet_current_time
*              and clear interrupt flag.
*************************************************************************/
static void fnet_cpu_timer_handler_top(void *cookie)
{
    TMR_Type             *base;

    base = fnet_mimxrt_qtmr_base_addr[FNET_CFG_CPU_TIMER_NUMBER];

    /* Clear interrupt flag.*/
    QTMR_ClearStatusFlags(base, FNET_MIMXRT_QTMR_CHANNEL, kQTMR_CompareFlag);

    /* Update FNET RTC counter. */
    _fnet_timer_ticks_inc();
}

/************************************************************************
* DESCRIPTION: Starts TCP/IP hardware timer. period_ms - period of timer (ms)
*************************************************************************/
fnet_return_t fnet_cpu_timer_init( fnet_time_t period_ms )
{
    fnet_return_t   result;
    qtmr_config_t   qtmrConfig;
    TMR_Type        *base;

    base = fnet_mimxrt_qtmr_base_addr[FNET_CFG_CPU_TIMER_NUMBER];

    /* Install interrupt handler and enable interrupt in NVIC. */
    result = _fnet_isr_vector_init(FNET_CFG_CPU_TIMER_VECTOR_NUMBER, fnet_cpu_timer_handler_top,
                                   _fnet_timer_handler_bottom,
                                   FNET_CFG_CPU_TIMER_VECTOR_PRIORITY, 0u);
    if(result == FNET_OK)
    {
        /* Initialize the TMR timer to generate an interrupt every period_ms */

        QTMR_GetDefaultConfig(&qtmrConfig);
        qtmrConfig.primarySource = kQTMR_ClockDivide_128; /* IP bus clock divide by 128 prescaler */

        QTMR_Init(base, FNET_MIMXRT_QTMR_CHANNEL, &qtmrConfig);

        /* Set timer period  */
        QTMR_SetTimerPeriod(base, FNET_MIMXRT_QTMR_CHANNEL, MSEC_TO_COUNT(period_ms, (CLOCK_GetFreq(kCLOCK_IpgClk) / 128)));

        /* Enable timer compare interrupt */
        QTMR_EnableInterrupts(base, FNET_MIMXRT_QTMR_CHANNEL, kQTMR_CompareInterruptEnable);

        /* Start the channel to count on rising edge of the primary source clock */
        QTMR_StartTimer(base, FNET_MIMXRT_QTMR_CHANNEL, kQTMR_PriSrcRiseEdge);
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Release TCP/IP hardware timer.
*************************************************************************/
void fnet_cpu_timer_release( void )
{
    TMR_Type        *base;

    base = fnet_mimxrt_qtmr_base_addr[FNET_CFG_CPU_TIMER_NUMBER];

    QTMR_Deinit(base, FNET_MIMXRT_QTMR_CHANNEL);

    /* Uninstall interrupt handler. */
    fnet_isr_unregister(FNET_CFG_CPU_TIMER_VECTOR_NUMBER);
}

/* If vector table is in ROM, pre-install FNET ISR for the Timer Event interrupt*/
#if !FNET_CFG_CPU_VECTOR_TABLE_IS_IN_RAM
#if FNET_CFG_CPU_TIMER_NUMBER == 1
void TMR1_IRQHandler (void)
{
    FNET_ISR_HANDLER();
}
#elif FNET_CFG_CPU_TIMER_NUMBER == 2
void TMR2_IRQHandler (void)
{
    FNET_ISR_HANDLER();
}
#elif FNET_CFG_CPU_TIMER_NUMBER == 3
void TMR3_IRQHandler (void)
{
    FNET_ISR_HANDLER();
}
#elif FNET_CFG_CPU_TIMER_NUMBER == 4
void TMR4_IRQHandler (void)
{
    FNET_ISR_HANDLER();
}
#endif
#endif

#endif /* FNET_CFG_MIMXRT_TIMER_QTMR */


#endif /*FNET_MIMXRT*/
