/**************************************************************************
*
* Copyright 2005-2015 by Andrey Butok. Freescale Semiconductor, Inc.
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
* @file fnet_mcf.c
*
* @author Andrey Butok
*
* @brief CPU-specific API implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_MCF

#include "cpu/fnet_cpu.h"

/************************************************************************
*     Function Prototypes
*************************************************************************/
static fnet_cpu_irq_desc_t fnet_mcf_set_irqlevel(fnet_uint32_t level);

/************************************************************************
* NAME: fnet_cpu_reset
*
* DESCRIPTION: Initiate software reset.
*************************************************************************/
void fnet_cpu_reset (void)
{
#if FNET_CFG_MCF_RCM
    FNET_MCF_RCM_RCR = FNET_MCF_RCM_RCR_SOFTRST | FNET_MCF_RCM_RCR_FRCRSTOUT;
#else
    fnet_uint32_t address = *((fnet_uint32_t *)(0x4));

    (( void(*)() )address)(); /* Jump. */
#endif

}

/************************************************************************
* NAME: fnet_cpu_reset
*
* DESCRIPTION: Initiate software reset.
*************************************************************************/
static fnet_cpu_irq_desc_t fnet_mcf_set_irqlevel(fnet_uint32_t level)
{
    fnet_uint16_t csr;
    fnet_cpu_irq_desc_t oldlevel;

    csr = fnet_mcf_sr_rd();

    oldlevel = (fnet_cpu_irq_desc_t)((csr & FNET_MCF_SR_IPL) >> 8);

    csr = (fnet_uint16_t)((csr & (~FNET_MCF_SR_IPL)) | FNET_MCF_SR_S | ((level << 8)&FNET_MCF_SR_IPL));

    fnet_mcf_sr_wr(csr);

    return oldlevel;
}


/************************************************************************
* NAME: fnet_cpu_disable_irq
*
* DESCRIPTION: Disable IRQs
*************************************************************************/
fnet_cpu_irq_desc_t fnet_cpu_irq_disable(void)
{
    return fnet_mcf_set_irqlevel(7);
}

/************************************************************************
* NAME: fnet_cpu_disable_irq
*
* DESCRIPTION: Enables IRQs at interrupt level mask value.
*************************************************************************/
void fnet_cpu_irq_enable(fnet_cpu_irq_desc_t irq_desc)
{
    fnet_mcf_set_irqlevel(irq_desc);
}

#endif /*FNET_MCF*/
