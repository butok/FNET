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
* @file fnet_mcf_isr_inst.c
*
* @author Andrey Butok
*
* @brief Interrupt service dispatcher implementation.
*
***************************************************************************/

#include "fnet.h"
#if FNET_MCF
#if !FNET_OS

#include "fnet.h"
#include "fnet_isr.h"
#include "fnet_timer.h"
#include "fnet_netbuf.h"
#include "fnet_mcf.h"

/* Vector table address. Defined in linker file.*/
extern fnet_uint32_t FNET_CFG_CPU_VECTOR_TABLE [1];


/************************************************************************
* NAME: fnet_cpu_isr_install
*
* DESCRIPTION:
*************************************************************************/
fnet_return_t fnet_cpu_isr_install(fnet_uint32_t vector_number, fnet_uint32_t priority)
{
    fnet_return_t result;
    fnet_uint32_t *irq_vec;


    irq_vec = (fnet_uint32_t *)(FNET_CFG_CPU_VECTOR_TABLE) + vector_number;

    if(*irq_vec != (fnet_uint32_t)FNET_ISR_HANDLER)
    {
        /* It's not installed yet.*/
        *irq_vec = (fnet_uint32_t) FNET_ISR_HANDLER;
    }

    if(priority > FNET_CFG_CPU_VECTOR_PRIORITY_MAX)
    {
        priority = FNET_CFG_CPU_VECTOR_PRIORITY_MAX;
    }

    if(*irq_vec == (fnet_uint32_t) FNET_ISR_HANDLER)
    {
#if !FNET_CFG_MCF_V1 /* No for MCF Ver.1 */
        /* Set priority. */
        {
            /* Enable interrupt at ColdFire SIM */
            fnet_int32_t irq_number; /* The irq number NOT the vector number.*/
            fnet_int32_t div;

            irq_number = (fnet_int32_t) (vector_number - 0x40);
            div = irq_number / 32;
            if((div >= 0) && (div < 2)) /* So far only upto 64 irq_number (INTC0). TBD*/
            {
                /* Set interrupt level.*/
                FNET_MCF_INTC0_ICR(irq_number) = (fnet_uint8_t)FNET_MCF_INTC0_ICRn_IL(priority) | FNET_MCF_INTC0_ICRn_IP(7);
                /* Clear mask all interrupts.*/
                FNET_MCF_INTC0_IMRL &= ~FNET_MCF_INTC0_IMRL_MASKALL;
                /* Unmask proper interrupt.*/
                FNET_MCF_INTC0_IMR(div) &= (~FNET_MCF_INTC0_IMR_INT_MASK(irq_number));
            }
        }
#else
        FNET_COMP_UNUSED_ARG(priority);
#endif /* !FNET_CFG_MCF_V1 */
        result = FNET_OK;
    }
    else
        result = FNET_ERR;

    return result;
}


#endif

#endif /*FNET_MCF*/
