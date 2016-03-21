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
**********************************************************************/ /*!
*
* @file fnet_mcf_timer.c
*
* @author Andrey Butok
*
* @brief ColdFire specific SW timers implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_MCF
#include "fnet.h"
#include "fnet_timer_prv.h"
#include "fnet_isr.h"
#include "fnet_mcf.h"


#if FNET_CFG_DEBUG_STACK
    extern fnet_uint8_t __SP_INIT[];
    #define SP_INIT    (fnet_uint32_t)__SP_INIT
    fnet_uint32_t fnet_dbg_stack_max = 0;
#endif

#define FNET_TIMER_VECTOR_NUMBER    FNET_CFG_CPU_TIMER_VECTOR_NUMBER   /* Number of timer interrupt vector.*/
#define FNET_TIMER_INT_LEVEL        FNET_CFG_CPU_TIMER_VECTOR_PRIORITY /* Timer interrupt level. Ignored for MCF V1 */

#if FNET_CFG_MCF_TIMER_RTC8     /* 8-bit RTC (Lasko) */

    #if (FNET_TIMER_PERIOD_MS>0xFF)
        #error "FNET_TIMER_PERIOD_MS must be less than 0xFF (HW limitation)."
    #endif

    #define FNET_TIMER_NUMBER    0

#endif

#if FNET_CFG_MCF_TIMER_DTIM     /* DMA Timer (Default for MCF) */
    #define FNET_TIMER_NUMBER       (FNET_CFG_CPU_TIMER_NUMBER)    /* Timer number (according to UM) */

    #if (FNET_TIMER_NUMBER == 3)

        #define FNET_TIMER_INTC0_SETUP(i) i##22

    #endif
    #if (FNET_TIMER_NUMBER == 2)

        #define FNET_TIMER_INTC0_SETUP(i) i##21

    #endif
    #if (FNET_TIMER_NUMBER == 1)

        #define FNET_TIMER_INTC0_SETUP(i) i##20

    #endif
    #if (FNET_TIMER_NUMBER == 0)

        #define FNET_TIMER_INTC0_SETUP(i) i##19

    #endif

    /* Constants for hardware timer initialization:
    * Prescaller value.
    */
    #define FNET_TIMER_TMR_PS_VAL (124)
    /* CLKIN/ms
    */
    #define FNET_TIMER_CLKIN_PER_MS ((FNET_CPU_CLOCK_KHZ/(FNET_TIMER_TMR_PS_VAL+1)))

#endif

#if FNET_CFG_MCF_TIMER_PIT      /* PIT Timer */
    #define FNET_TIMER_NUMBER               (FNET_CFG_CPU_TIMER_NUMBER)    /* Timer number */

    #define FNET_MCF_PIT_PCSR_PRE_VALUE     (7)

#endif  /* FNET_CFG_MCF_TIMER_PIT */

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void fnet_cpu_timer_handler_top(fnet_uint32_t cookie );


/************************************************************************
* NAME: fnet_cpu_timer_handler_top
*
* DESCRIPTION: Top interrupt handler. Increment fnet_current_time
*              and interrupt flag.
*************************************************************************/
static void fnet_cpu_timer_handler_top(fnet_uint32_t cookie )
{
    FNET_COMP_UNUSED_ARG(cookie);

#if FNET_CFG_MCF_TIMER_RTC8     /* 8-bit RTC (Lasko) */
    /* Clear the interrupt flag.
     */
    FNET_MCF_RTCSC |= FNET_MCF_RTCSC_RTIF_MASK;
#endif

#if FNET_CFG_MCF_TIMER_DTIM     /* DMA Timer (Default for MCF) */
    /* Clear timer event condition.
     */
    FNET_MCF_DTIM_DTER(FNET_TIMER_NUMBER) = FNET_MCF_DTIM_DTER_REF;
#endif

#if FNET_CFG_MCF_TIMER_PIT     /* PIT Timer */
    /* Clear PIT interrupt flag.
     */
    FNET_MCF_PIT_PCSR(FNET_TIMER_NUMBER) |= FNET_MCF_PIT_PCSR_PIF;
#endif

    /* Update RTC counter.
     */
    fnet_timer_ticks_inc();



#if FNET_CFG_DEBUG_STACK  /* Only for debugging needs. */
    {
        fnet_uint32_t stack_value;
        fnet_uint32_t stack_usage;

        stack_value = fnet_mcf_sp_rd();

        stack_usage = SP_INIT - stack_value;
        if(stack_usage > fnet_dbg_stack_max)
            fnet_dbg_stack_max = stack_usage;
    }
#endif
}

/********************************************************************/
static inline void fnet_cpu_timer_gpio_init(long timer_number)
{

#if FNET_CFG_CPU_MCF54418 && FNET_CFG_MCF_TIMER_DTIM
    /* Set GPIO port register to enable PSC(port) signals */
    switch( timer_number )
    {
        case 0:
            FNET_MCF5441X_GPIO_PAR_TIMER &= ~FNET_MCF5441X_GPIO_PAR_TIMER_T0IN;
            FNET_MCF5441X_GPIO_PAR_TIMER |= FNET_MCF5441X_GPIO_PAR_TIMER_T0OUT;
            FNET_MCF_PMM_PPMLR0 &= ~(FNET_MCF_PMM_PPMLR0_CD28);	/* Enable clock */
            break;
        case 1:
            FNET_MCF5441X_GPIO_PAR_TIMER &= ~FNET_MCF5441X_GPIO_PAR_TIMER_T1IN;
            FNET_MCF5441X_GPIO_PAR_TIMER |= FNET_MCF5441X_GPIO_PAR_TIMER_T1OUT;
            FNET_MCF_PMM_PPMLR0 &= ~(FNET_MCF_PMM_PPMLR0_CD29);	/* Enable clock */
            break;
        case 2:
            FNET_MCF5441X_GPIO_PAR_TIMER &= ~FNET_MCF5441X_GPIO_PAR_TIMER_T2IN;
            FNET_MCF5441X_GPIO_PAR_TIMER |= FNET_MCF5441X_GPIO_PAR_TIMER_T2OUT;
            FNET_MCF_PMM_PPMLR0 &= ~(FNET_MCF_PMM_PPMLR0_CD30);	/* Enable clock */
            break;
        case 3:
            FNET_MCF5441X_GPIO_PAR_TIMER &= ~FNET_MCF5441X_GPIO_PAR_TIMER_T3IN;
            FNET_MCF5441X_GPIO_PAR_TIMER |= FNET_MCF5441X_GPIO_PAR_TIMER_T3OUT;
            FNET_MCF_PMM_PPMLR0 &= ~(FNET_MCF_PMM_PPMLR0_CD31);	/* Enable clock */
            break;

    }
#endif	/* FNET_CFG_CPU_MCF54418 && FNET_CFG_MCF_TIMER_DTIM */


    (void)timer_number;
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

    fnet_cpu_timer_gpio_init(FNET_TIMER_NUMBER);

    if(result == FNET_OK)
    {
#if FNET_CFG_MCF_TIMER_RTC8     /* 8-bit RTC (Lasko) */

        /* Clock trim.
         * Note: All MCU devices are factory programmed with a trim value in
         * a reserved memory location.
         */
        FNET_MCF_MCGTRM = FNET_MCF_NVMCGTRM;

        /* Enables the internal reference clock for use as MCGIRCLK (for RTC).
         */
        FNET_MCF_MCGC1_IRCLKEN = 1;

        FNET_MCF_RTCSC_RTCLKS = 0;  /* RTC source is the 1-kHz low power oscillator (LPO).
                                     */
        FNET_MCF_RTCSC_RTCPS = 8;   /* Prescaler = 1.
    	                             */
        FNET_MCF_RTCMOD = (fnet_uint8_t)(period_ms); /* Set period
    	                                               */
        FNET_MCF_RTCSC_RTIE = 1;    /* Enable the RTC to generate interrupts.
    	                             */
#endif

#if FNET_CFG_MCF_TIMER_DTIM     /* DMA Timer (Default for MCF) */

        FNET_MCF_DTIM_DTMR(FNET_TIMER_NUMBER) = FNET_MCF_DTIM_DTMR_CE_NONE;
        FNET_MCF_DTIM_DTRR(FNET_TIMER_NUMBER) = period_ms * FNET_TIMER_CLKIN_PER_MS;
        FNET_MCF_DTIM_DTER(FNET_TIMER_NUMBER) = FNET_MCF_DTIM_DTER_REF | FNET_MCF_DTIM_DTER_CAP;
        FNET_MCF_DTIM_DTCN(FNET_TIMER_NUMBER) = 0;

        FNET_MCF_DTIM_DTMR(FNET_TIMER_NUMBER) = (FNET_TIMER_TMR_PS_VAL << 8)
                                                | FNET_MCF_DTIM_DTMR_RST
                                                | FNET_MCF_DTIM_DTMR_CLK_DIV1
                                                | FNET_MCF_DTIM_DTMR_ORRI
                                                | FNET_MCF_DTIM_DTMR_FRR;
#endif

#if FNET_CFG_MCF_TIMER_PIT     /* PIT Timer */
        FNET_MCF_PIT_PCSR(FNET_TIMER_NUMBER) = (fnet_uint16_t)(FNET_MCF_PIT_PCSR_PRE(FNET_MCF_PIT_PCSR_PRE_VALUE));
        FNET_MCF_PIT_PMR(FNET_TIMER_NUMBER) = (fnet_uint16_t)((FNET_CPU_CLOCK_KHZ >> (FNET_MCF_PIT_PCSR_PRE_VALUE + 1)) * period_ms - 1) ;
        FNET_MCF_PIT_PCSR(FNET_TIMER_NUMBER) |= FNET_MCF_PIT_PCSR_DBG   /* PIT function stopped in debug mode. */
                                                | FNET_MCF_PIT_PCSR_OVW
                                                | FNET_MCF_PIT_PCSR_PIE /* PIF interrupt requests enabled. */
                                                | FNET_MCF_PIT_PCSR_PIF
                                                | FNET_MCF_PIT_PCSR_RLD
                                                | FNET_MCF_PIT_PCSR_EN; /* PIT enabled. */
#endif /* FNET_CFG_MCF_TIMER_PIT */

    }

    return result;
}

/************************************************************************
* NAME: fnet_cpu_timer_release
*
* DESCRIPTION: Releases TCP/IP hardware timer.
*
*************************************************************************/
void fnet_cpu_timer_release( void )
{
#if FNET_CFG_MCF_TIMER_RTC8     /* 8-bit RTC (Lasko) */

    FNET_MCF_RTCSC_RTIE = 0;    /* Disable Interrupts.
                                 */
    FNET_MCF_RTCSC = 0;         /* Disable RTC.
                                 */
#endif

#if FNET_CFG_MCF_TIMER_DTIM     /* DMA Timer (Default for MCF) */

    FNET_MCF_DTIM_DTMR(FNET_TIMER_NUMBER) = FNET_MCF_DTIM_DTMR_CE_NONE; /* Disable timer. */
    FNET_MCF_DTIM_DTER(FNET_TIMER_NUMBER) = FNET_MCF_DTIM_DTER_REF | FNET_MCF_DTIM_DTER_CAP; /* Clear event conditions. */

#endif

    /* Free interrupt handler res.
     */
    fnet_isr_vector_release(FNET_TIMER_VECTOR_NUMBER);

}

#endif /*FNET_MCF*/
