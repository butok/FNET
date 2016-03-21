/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
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
* @file fnet_mpc.c
*
* @author Andrey Butok
*
* @date Dec-17-2012
*
* @version 0.1.1.0
*
* @brief CPU-specific API implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_MPC

/************************************************************************
* NAME: fnet_cpu_reset
*
* DESCRIPTION: Initiate software reset.
*************************************************************************/
void fnet_cpu_reset (void)
{
#if !FNET_CFG_CPU_MPC5744P
    FNET_MPC_SIU_SRCR = 0x80000000; /* Software System Reset */
#endif
}

/************************************************************************
* NAME: fnet_cpu_disable_irq
*
* DESCRIPTION: Disable IRQs
*************************************************************************/
fnet_cpu_irq_desc_t fnet_cpu_irq_disable(void)
{
    fnet_cpu_irq_desc_t oldlevel;


#if 0 /* Not the best way to do this... SWT might depend upon it */
    asm("wrteei 0");
    return 0;
#endif



#if FNET_CFG_CPU_INDEX==0
    oldlevel = FNET_MPC_INTC_CPR_PRC0;
    FNET_MPC_INTC_CPR_PRC0 = 15;
#else
    oldlevel = FNET_MPC_INTC_CPR_PRC1;
    FNET_MPC_INTC_CPR_PRC1 = 15;
#endif

    return oldlevel;
}

/************************************************************************
* NAME: fnet_cpu_disable_irq
*
* DESCRIPTION: Enables IRQs at interrupt level mask value.
*************************************************************************/
void fnet_cpu_irq_enable(fnet_cpu_irq_desc_t irq_desc)
{
    /* Enable processor recognition of interrupts. Just in case */
    asm("wrteei 1");                 /* Set MSR[EE]=1  */

#if FNET_CFG_CPU_INDEX==0
    FNET_MPC_INTC_CPR_PRC0 = irq_desc;
#else
    FNET_MPC_INTC_CPR_PRC1 = irq_desc;
#endif

}

#endif /*FNET_MPC*/
