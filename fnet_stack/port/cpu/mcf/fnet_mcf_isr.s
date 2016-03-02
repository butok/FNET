/**************************************************************************
* 
* Copyright 2011-2015 by Andrey Butok. FNET Community.
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
**********************************************************************//*!
*
* @file fnet_mcf_isr.asm
*
* @author Andrey Butok
*
* @brief Lowest level routines for ColdFire.
*
***************************************************************************/

#include "compiler/fnet_comp_asm.h"

#if FNET_MCF


FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(fnet_isr_handler)

FNET_COMP_ASM_GLOBAL    FNET_COMP_ASM_PREFIX(fnet_cpu_isr)



FNET_COMP_ASM_CODE

/************************************************************************
* NAME: void fnet_cpu_isr(void);
*
* DESCRIPTION: This handler is executed on every FNET interrupt 
*              (from ethernet and timer module).
*              Extructs vector number and calls fnet_isr_handler().
*************************************************************************/
FNET_COMP_ASM_ALIGN 4

FNET_COMP_ASM_PREFIX(fnet_cpu_isr):
	lea      (-60,a7),a7        
	movem.l  d0-d7/a0-a6,(a7)
	
    move.l   (60,a7),d0 /* Extract the irq number from */
    lsr.l    #8,d0      /* The exception stack frame */
    lsr.l    #8,d0     
    lsr.l    #2,d0
    and.l    #0xff,d0
#if FNET_CFG_COMP_GNUC          /* GCC passes parameters on the stack */
    move.l   d0, -(sp)
#endif   
	jsr     (FNET_COMP_ASM_PREFIX(fnet_isr_handler))
#if FNET_CFG_COMP_GNUC          /* GCC passes parameters on the stack */
    addq.l   #4, sp
#endif
	movem.l  (a7),d0-d7/a0-a6
    lea      (60,a7),a7	
	rte

#endif /*FNET_MCF*/


    FNET_COMP_ASM_END
