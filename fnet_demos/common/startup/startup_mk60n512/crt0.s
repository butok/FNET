/*
 * File:	crt0.s
 * Purpose:	Lowest level routines for Kinetis.
 *
 * Notes:	
 *
 */
#include "port/compiler/fnet_comp_asm.h"



#if FNET_CFG_COMP_UV /* Keil specific.*/

  AREA KERNEL_DATA_START, DATA, ALIGN=4, READWRITE
  EXPORT __KERNEL_DATA_START
__KERNEL_DATA_START
  DCD 0


/* Stack Size (in Bytes) */
Stack_Size      EQU     0x00001000 ;4KB

  EXPORT __initial_sp

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


/* Heap Size (in Bytes) <0x0-0xFFFFFFFF:8> */
Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


/* User Initial Stack & Heap */

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE
 FNET_COMP_ASM_CODE
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF
  
#endif /* FNET_CFG_COMP_UV */

/******************************************************
*         Entry point
*******************************************************/

#if FNET_CFG_COMP_IAR || FNET_CFG_COMP_UV
 FNET_COMP_ASM_CODE

 FNET_COMP_ASM_EXTERN FNET_COMP_ASM_PREFIX(start)
 FNET_COMP_ASM_GLOBAL FNET_COMP_ASM_PREFIX(__startup)
 
FNET_COMP_ASM_LABEL(__startup)
    MOV     r0,#0                   ; Initialize the GPRs
    MOV     r1,r0
    MOV     r2,r0
    MOV     r3,r0
    MOV     r4,r0
    MOV     r5,r0
    MOV     r6,r0
    MOV     r7,r0
    MOV     r8,r0
    MOV     r9,r0
    MOV     r10,r0
    MOV     r11,r0
    MOV     r12,r0
    CPSIE   i                      ; Unmask interrupts
    BL      start                  ; call the C code

  
#endif /* FNET_CFG_COMP_IAR || FNET_CFG_COMP_UV */

 
     FNET_COMP_ASM_END
	 
