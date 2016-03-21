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
* @file fnet_mk.c
*
* @author Andrey Butok
*
* @brief CPU-specific API implementation.
*
***************************************************************************/
#include "fnet.h"

#if FNET_MK


/************************************************************************
* NAME: fnet_cpu_reset
*
* DESCRIPTION: Initiate software reset.
*************************************************************************/
void fnet_cpu_reset (void)
{
    /* Application Interrupt and Reset Control Register.*/
    /* To write to this register, you must write 0x5FA to the VECTKEY field.*/
    FNET_MK_SCB_AIRCR = FNET_MK_SCB_AIRCR_VECTKEY(0x5fa) | FNET_MK_SCB_AIRCR_SYSRESETREQ_MASK; /* Asserts a signal to the outer system that requests a reset.*/

}

/************************************************************************
* NAME: fnet_cpu_irq_disable
*
* DESCRIPTION: Disable IRQs
*************************************************************************/
fnet_cpu_irq_desc_t fnet_cpu_irq_disable(void)
{
    return fnet_mk_irq_disable();
}

/************************************************************************
* NAME: fnet_cpu_irq_enable
*
* DESCRIPTION: Enables IRQs.
*************************************************************************/
void fnet_cpu_irq_enable(fnet_cpu_irq_desc_t irq_desc)
{
    fnet_mk_irq_enable(irq_desc);
}

/************************************************************************
* NAME: fnet_mk_periph_clk_khz
*
* DESCRIPTION: Kinetis peripheral clock in KHZ.
*************************************************************************/
fnet_uint32_t fnet_mk_periph_clk_khz(void)
{
    return (FNET_CPU_CLOCK_KHZ / (((FNET_MK_SIM_CLKDIV1 & FNET_MK_SIM_CLKDIV1_OUTDIV2_MASK) >> 24) + 1u));
}

#endif /*FNET_MK*/
