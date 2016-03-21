/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
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
* @file fnet_mk_isr_inst.c
*
* @author Andrey Butok
*
* @brief Interrupt service dispatcher implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_MK
#if !FNET_OS

/************************************************************************
* NAME: fnet_cpu_isr_install
*
* DESCRIPTION:
*************************************************************************/
fnet_return_t fnet_cpu_isr_install(fnet_uint32_t vector_number, fnet_uint32_t priority)
{
    fnet_return_t   result;
    fnet_uint32_t   *irq_vec;
    fnet_uint32_t   divider;
    fnet_uint32_t   irq_number; /* The irq number NOT the vector number.*/

    irq_vec = (fnet_uint32_t *)(FNET_CFG_CPU_VECTOR_TABLE) + vector_number;

    if(*irq_vec != (fnet_uint32_t) FNET_ISR_HANDLER)
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
        /* Make sure that the IRQ is an allowable number. */
        irq_number = vector_number - 16u;
        divider = irq_number / 32u;
        if(divider < 3u)
        {
            /* Initialize the NVIC to enable the specified IRQ.*/
            FNET_MK_NVIC_ICPR(divider) |= (fnet_uint32_t)(1u << (irq_number % 32u)); /* Clear-pending. */
            FNET_MK_NVIC_ISER(divider) |= (fnet_uint32_t)(1u << (irq_number % 32u)); /* Set-enable.*/
            /* Set priority.*/
            FNET_MK_NVIC_IP(irq_number) = (fnet_uint8_t)(FNET_CFG_CPU_VECTOR_PRIORITY_MAX - priority) << 4;
        }
        result = FNET_OK;
    }
    else
    {
        result = FNET_ERR;
    }

    return result;
}

#endif /* !FNET_OS */

#endif /*FNET_MK*/
