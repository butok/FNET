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
* @file fnet_mpc_isr_inst.c
*
* @author Andrey Butok
*
* @date Dec-17-2012
*
* @version 0.1.1.0
*
* @brief Interrupt service dispatcher implementation.
*
***************************************************************************/
#include "fnet.h"

#if FNET_MPC
#if !FNET_OS

/* Vector table address. Defined in linker file.*/
extern fnet_uint32_t FNET_CFG_CPU_VECTOR_TABLE [];

/************************************************************************
* NAME: fnet_cpu_isr_install
*
* DESCRIPTION:
*************************************************************************/
fnet_return_t fnet_cpu_isr_install(fnet_uint32_t vector_number, fnet_uint32_t priority)
{
    fnet_return_t result;
    fnet_uint32_t *irq_vec;

    irq_vec = (unsigned long *) (FNET_CFG_CPU_VECTOR_TABLE) + vector_number;

    if(*irq_vec != (unsigned long)fnet_cpu_isr)
    {
        /* It's not installed yet.*/
        *irq_vec = (unsigned long)fnet_cpu_isr;
    }

    if(priority > FNET_CFG_CPU_VECTOR_PRIORITY_MAX)
    {
        priority = FNET_CFG_CPU_VECTOR_PRIORITY_MAX;
    }

    if(*irq_vec == (unsigned long)fnet_cpu_isr)
    {
#if FNET_CFG_CPU_INDEX==0
#if FNET_CFG_CPU_MPC5744P
        FNET_MPC_INTC_PSR(vector_number) = (unsigned short int)(0x8000 | priority);
#else
        FNET_MPC_INTC_PSR(vector_number) = (unsigned char)(0xF & priority);
#endif
#else
        FNET_MPC_INTC_PSR(vector_number) = (unsigned char)(0xC0 | & priority);
#endif

        result = FNET_OK;
    }
    else
        result = FNET_ERR;

    return result;
}

#endif

#endif /*FNET_MPC*/
