/**************************************************************************
*
* Copyright 2012-2013 by Jon Elliott, Andrey Butok. FNET Community.
*
***************************************************************************
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License Version 3 
* or later (the "LGPL").
*
* As a special exception, the copyright holders of the FNET project give you
* permission to link the FNET sources with independent modules to produce an
* executable, regardless of the license terms of these independent modules,
* and to copy and distribute the resulting executable under terms of your 
* choice, provided that you also meet, for each linked independent module,
* the terms and conditions of the license of that module.
* An independent module is a module which is not derived from or based 
* on this library. 
* If you modify the FNET sources, you may extend this exception 
* to your version of the FNET sources, but you are not obligated 
* to do so. If you do not wish to do so, delete this
* exception statement from your version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* You should have received a copy of the GNU General Public License
* and the GNU Lesser General Public License along with this program.
* If not, see <http://www.gnu.org/licenses/>.
*
**********************************************************************/ /*!
*
* @file fnet_ucosIII_config.h

* Experental. Not supported.
*
***************************************************************************/

#include "compiler/fnet_comp_asm.h"

#if FNET_CFG_OS && FNET_CFG_OS_UCOSIII && FNET_MCF && FNET_CFG_OS_ISR

;*************************************************************************************************
;                                       PUBLIC DECLARATIONS
;*************************************************************************************************

    FNET_COMP_ASM_GLOBAL    FNET_COMP_ASM_PREFIX(fnet_os_isr)
    
;**************************************************************************************************
;                                     EXTERNAL DECLARATIONS
;**************************************************************************************************
    FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(fnet_isr_handler)
    FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(IntpStackTOS)

;DSI.JDE original
;   FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(OSIntNesting)
    FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(OSIntNestingCtr)
    
;DSI.JDE - original
;   FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(OSTCBCur)
    FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(OSTCBCurPtr)           


    FNET_COMP_ASM_CODE
    FNET_COMP_ASM_ALIGN 4
    
/************************************************************************
* NAME: fnet_os_isr
*
* DESCRIPTION: This handler is executed on every FNET interrupt 
*              (from ethernet and timer module).
*              Extracts vector number and calls fnet_isr_handler().
*************************************************************************/  
FNET_COMP_ASM_PREFIX(fnet_os_isr):

    STRLDSR.W   #0x2700                 ;Disable interrupts, but store SR on stack first

    lea         (-60,a7),a7
    movem.l     d0-d7/a0-a6,(a7) 
    MOVEA       A7,A0                   ;Save off original SP for later use
    
    move.l      (64,a7),d0              ; Extract the irq number from 
    lsr.l       #8,d0                   ; The exception stack frame
    lsr.l       #8,d0     
    lsr.l       #2,d0
    and.l       #0xff,d0                ;D0 holds arg for fnet_isr_handler

    MOVEQ.L     #0,D1                   ; OSIntNesting++
;DSI.JDE - changed for OSIII
;   MOVE.B      (FNET_COMP_ASM_PREFIX(OSIntNesting)),D1
    MOVE.B      (FNET_COMP_ASM_PREFIX(OSIntNestingCtr)),D1
    
    ADDQ.L      #1,D1
;DSI.JDE - changed for OSIII
;   MOVE.B      D1,(FNET_COMP_ASM_PREFIX(OSIntNesting))
    MOVE.B      D1,(FNET_COMP_ASM_PREFIX(OSIntNestingCtr))

    CMPI.L      #1, D1                       ; if (OSIntNesting == 1)
    BNE.B       FNET_COMP_ASM_PREFIX(fnet_isr_call)

;----------------------------------------
;   Nesting=1, then we've interrupted a task, not another interrupt
;DSI.JDE - changed for OSIII
;   MOVEA.L     (FNET_COMP_ASM_PREFIX(OSTCBCur)), A1    ; Save SP in OSTCBCur->OSTCBStkPtr
    MOVEA.L     (FNET_COMP_ASM_PREFIX(OSTCBCurPtr)), A1    ; Save SP in OSTCBCur->OSTCBStkPtr
    MOVE.L      A7,(A1)
                                    
    ;To use an interrupt-specific stack frame, must init SP to point to intp stack.
    ;since this is only executed at top level of nested intps, TOS will always be at
    ;beginning of stack space.
;DSI.JDE - changed to allow assembly with CW10.3
;   MOVEA.L     (IntpStackTOS),A1
    MOVEA.L     (FNET_COMP_ASM_PREFIX(IntpStackTOS)),A1
;DSI.JDE - end change   
    LEA.L       (4,A1),A7           ; Point to 1 beyond TOS as it will be pre-decremented when used
;----------------------------------------

FNET_COMP_ASM_PREFIX(fnet_isr_call):
    MOVE.W      (62,A0),D1          ;Get saved status register back from original stack
    MOVE.W      D1,SR               ;to re-enable higher priority interrupts
                                        
    JSR         (FNET_COMP_ASM_PREFIX(fnet_isr_handler))

    MOVE.W      #0x2700,SR          ; Disable interrupts again, will be re-enabled by RTE
    MOVEQ.L     #0,D1               ;if(OSIntNesting > 0) OSIntNesting--

;DSI.JDE - changed for OSIII
;   MOVE.B      (FNET_COMP_ASM_PREFIX(OSIntNesting)),D1
    MOVE.B      (FNET_COMP_ASM_PREFIX(OSIntNestingCtr)),D1
    BEQ          FNET_COMP_ASM_PREFIX(fnet_isr_exit)
    
;DSI.JDE - changed to allow assembly with CW10.3
;   SUBQ        #1,D1   
    SUBQ.L      #1,D1   
;DSI.JDE - end change

;DSI.JDE - change for OSIII
;   MOVE.B      D1,(FNET_COMP_ASM_PREFIX(OSIntNesting))
    MOVE.B      D1,(FNET_COMP_ASM_PREFIX(OSIntNestingCtr))
        
    BNE         FNET_COMP_ASM_PREFIX(fnet_isr_exit)     ;if OSIntNesting now 0,         

;DSI.JDE - changed for OSIII
;   MOVEA.L     (FNET_COMP_ASM_PREFIX(OSTCBCur)),A1     ;Get Task SP back
    MOVEA.L     (FNET_COMP_ASM_PREFIX(OSTCBCurPtr)),A1  ;Get Task SP back
    MOVEA.L     (A1),A7                                 ;into SP

;DSI.JDE - added underscore prefix to label to match FNET_COMP _ASM_PREFIX macro and fix assembler error    
_fnet_isr_exit:
;DSI.JDE - end change
    MOVEM.L     (A7),D0-D7/A0-A6    ; Restore processor registers from stack
    LEA         (64,A7),A7          ; offset of '64' is to discard SR saved on stack by STLDSR
    RTE                             ; Return to task or nested ISR

#endif /*FNET_CFG_OS && FNET_CFG_OS_UCOSIII && FNET_MCF && FNET_CFG_OS_ISR*/

    FNET_COMP_ASM_END
