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
* @file fnet_mk_low.S
*
* @author Andrey Butok
*
* @brief Lowest level routines for Kinetis.
*
***************************************************************************/
#include "port/compiler/fnet_comp_asm.h"

#if FNET_MK

 FNET_COMP_ASM_CODE

 FNET_COMP_ASM_GLOBAL FNET_COMP_ASM_PREFIX(fnet_mk_irq_enable)
 FNET_COMP_ASM_GLOBAL FNET_COMP_ASM_PREFIX(fnet_mk_irq_disable)
 
FNET_COMP_ASM_LABEL(fnet_mk_irq_enable)
    /* cpsie i */    /* Enable the priority configurable interrupts (0=>PRIMASK).*/
    msr primask, r0	 /* Set PRIMASK */
    bx lr

FNET_COMP_ASM_LABEL(fnet_mk_irq_disable)
    mrs r0, primask     /* Save PRIMASK */
    cpsid i     	/* Turn off the priority configurable interrupts (1=>PRIMASK).*/
    bx lr

#endif /*FNET_MK*/ 

    FNET_COMP_ASM_END




