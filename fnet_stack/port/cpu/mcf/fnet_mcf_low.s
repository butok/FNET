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
* @file fnet_mcf_low.asm
*
* @author Andrey Butok
*
* @brief Lowest level routines for ColdFire.
*
***************************************************************************/
#include "compiler/fnet_comp_asm.h"

#if FNET_MCF

	FNET_COMP_ASM_GLOBAL    FNET_COMP_ASM_PREFIX(fnet_mcf_vbr_wr)

#if FNET_CFG_CPU_CACHE
	FNET_COMP_ASM_GLOBAL    FNET_COMP_ASM_PREFIX(fnet_mcf_cacr_wr)
#endif

	FNET_COMP_ASM_GLOBAL    FNET_COMP_ASM_PREFIX(fnet_mcf_sp_rd)

	FNET_COMP_ASM_GLOBAL    FNET_COMP_ASM_PREFIX(fnet_mcf_sr_rd)


	FNET_COMP_ASM_GLOBAL    FNET_COMP_ASM_PREFIX(fnet_mcf_sr_wr)


	FNET_COMP_ASM_GLOBAL    FNET_COMP_ASM_PREFIX(fnet_mcf_illegal)

	FNET_COMP_ASM_GLOBAL    FNET_COMP_ASM_PREFIX(fnet_mcf_nop)


	FNET_COMP_ASM_CODE


/* Set VBR */
/* void fnet_mcf_vbr_wr(fnet_uint32_t) */
FNET_COMP_ASM_PREFIX(fnet_mcf_vbr_wr):
#if FNET_CFG_COMP_GNUC
	move.l  4(sp), d0	/* get parameter from stack */
#endif
	movec d0,VBR 
	nop
	rts	

#if FNET_CFG_CPU_CACHE
/* Set CACR */
/* void fnet_mcf_cacr_wr(fnet_uint32_t) */
FNET_COMP_ASM_PREFIX(fnet_mcf_cacr_wr):
#if FNET_CFG_COMP_GNUC
	move.l  4(sp), d0	/* get parameter from stack */
#endif
	movec d0, cacr 
	nop
	rts
#endif	

/* Read SP */
/* fnet_uint32_t fnet_mcf_sp_rd() */
FNET_COMP_ASM_PREFIX(fnet_mcf_sp_rd):
	move.l     sp,d0
    rts  


/* Read SR */
/* fnet_uint16_t fnet_mcf_sr_rd() */
FNET_COMP_ASM_PREFIX(fnet_mcf_sr_rd):
	move.w     sr,d0
	rts    

/* Write SR */
/* void fnet_mcf_sr_wr(fnet_uint16_t) */
FNET_COMP_ASM_PREFIX(fnet_mcf_sr_wr):
#if FNET_CFG_COMP_GNUC
	move.l  4(sp), d0	/* get parameter from stack */
#endif
	move.w     d0,sr
	rts


/* Generates an illegal instruction exception */
FNET_COMP_ASM_PREFIX(fnet_mcf_illegal):
	FNET_COMP_ASM_DC16 0x4afc ;illegal
	rts


/* NOP */
FNET_COMP_ASM_PREFIX(fnet_mcf_nop):
	nop
	rts

#endif /*FNET_MCF*/

	FNET_COMP_ASM_END

