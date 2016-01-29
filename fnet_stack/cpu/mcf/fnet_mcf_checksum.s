/**************************************************************************
* 
* Copyright 2008-2011 by Andrey Butok. Freescale Semiconductor, Inc.
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
* @file fnet_mcf_checksum.asm
*
* @author Andrey Butok
*
* @brief ColdFire-specific Internet checksum calculation.
*
***************************************************************************/

#include "compiler/fnet_comp_asm.h"

#if FNET_MCF

#if FNET_CFG_OVERLOAD_CHECKSUM_LOW
	FNET_COMP_ASM_GLOBAL FNET_COMP_ASM_PREFIX(fnet_checksum_low) 

	FNET_COMP_ASM_CODE
	FNET_COMP_ASM_ALIGN 4
	
/************************************************************************
* NAME: fnet_checksum_low
*
* DESCRIPTION: Calculates checksum.
*
*************************************************************************
* unsigned long fnet_checksum_low(unsigned long sum, int current_length, unsigned short *d_ptr);
*   Arguments:
*        D0: unsigned long sum
*        D1: int current_length
*        A0: unsigned short *d_ptr
*/
FNET_COMP_ASM_PREFIX(fnet_checksum_low):
#if FNET_CFG_COMP_GNUC          /* GCC passes parameters on the stack */
        move.l  4(sp), d0
        move.l  8(sp), d1
        move.l  12(sp), a0
#endif  
        move.l  a1,-(sp)
        move.l  d2,-(sp)
        move.l  d3,-(sp)

        cmp.l   #3,d1           /* Do we have 3 or less bytes?*/
        ble.w   try_words

        move.l  a0,d2           /* Try to get onto even word.*/
        btst.l  #1,d2
        beq.b   try_longs       /* YES, address is long aligned already.*/
        clr.l   d3              /* NO, do align that. */
        move.w  (a0)+,d3
        add.l   d3,d0
        subq.l  #2,d1

try_longs:
        move.l  d1,d2
        lsr.l   #6,d2           /* 64 bytes summed per loop. */
        movea.l d2,a1

        move.l  d1,d3           /* 1 long processed by 4 bytes.*/
        and.l   #0x3C,d3
        neg.l   d3
        move  #0,ccr            /* Clear X.*/


#if FNET_CFG_COMP_IAR           /* IAR does not understand this command.*/
        DC32   0x4EFB3842
#elif FNET_CFG_COMP_GNUC
        jmp     66(pc,d3.l*1)     
#else
        jmp     (pc,d3.l,66)
#endif
do_longs:
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0

        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0

do_longs_end:
        suba.l  #1,a1
        tst.l   a1
        bge.b     do_longs

do_word:
        btst    #1,d1           /* Sum last word.*/
        beq.b     do_byte
        clr.l   d3
        move.w  (a0)+,d3
        addx.l  d3,d0

do_byte:
        btst    #0,d1           /* Sum last byte.*/
        beq.b   done
        clr.l   d3
        addx.l  d3,d0           /* Add the carry if there is one.*/
        move.b  (a0)+,d3
        asl.l   #8,d3
        add.l   d3,d0

done:   move.l  d0,d1           /* Fold 32 bit sum into 16.*/
        swap    d1
        and.l   #0xFFFF,d0
        and.l   #0xFFFF,d1
        addx.l  d1,d0
        move.l  d0,d1
        swap    d1
        add.l   d1,d0
        and.l   #0xFFFF,d0

        move.l  (sp)+,d3
        move.l  (sp)+,d2
        movea.l  (sp)+,a1
        rts

try_words:
        move  #0,ccr            /* Clear X.*/
        bra.b   do_word
        
#endif  /* FNET_CFG_OVERLOAD_CHECKSUM_LOW */

#endif /*FNET_MCF*/

	FNET_COMP_ASM_END
