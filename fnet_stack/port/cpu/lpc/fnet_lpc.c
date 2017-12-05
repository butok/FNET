/**************************************************************************
*
* Copyright 2017 by Andrey Butok. FNET Community.
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
* CPU-specific API implementation.
*
***************************************************************************/
#include "fnet.h"

#if FNET_LPC

/************************************************************************
* DESCRIPTION: Initiate software reset.
*************************************************************************/
void fnet_cpu_reset (void)
{
    /* Application Interrupt and Reset Control Register.*/
    /* To write to this register, you must write 0x5FA to the VECTKEY field.*/
    FNET_LPC_SCB_AIRCR = FNET_LPC_SCB_AIRCR_VECTKEY(0x5fa) | FNET_LPC_SCB_AIRCR_SYSRESETREQ_MASK; /* Asserts a signal to the outer system that requests a reset.*/
}

/************************************************************************
* DESCRIPTION: Disable IRQs
*************************************************************************/
fnet_cpu_irq_desc_t fnet_cpu_irq_disable(void)
{
    return fnet_lpc_irq_disable();
}

/************************************************************************
* DESCRIPTION: Enables IRQs.
*************************************************************************/
void fnet_cpu_irq_enable(fnet_cpu_irq_desc_t irq_desc)
{
    fnet_lpc_irq_enable(irq_desc);
}

/************************************************************************
* DESCRIPTION: Kinetis peripheral clock in KHZ.
*************************************************************************/
fnet_uint32_t fnet_mk_periph_clk_khz(void)
{
//TBD
    return (0);
}

#endif /*FNET_MK*/
