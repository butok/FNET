/*
* File:    exceptions.asm
* Purpose: Generic exception handling for ColdFire processors
*
*/
#include    "fnet_comp_asm.h" 

FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(mcf_exception_handler)

FNET_COMP_ASM_GLOBAL    FNET_COMP_ASM_PREFIX(asm_exception_handler)

FNET_COMP_ASM_CODE

FNET_COMP_ASM_ALIGN 4

FNET_COMP_ASM_PREFIX(asm_exception_handler):
   link     a6,#0 
   lea      (-20,sp), sp
   movem.l  d0-d2/a0-a1, (sp)
   lea      (24,sp),a0   /* A0 point to exception stack frame on the stack */
   jsr      (FNET_COMP_ASM_PREFIX(mcf_exception_handler))
   movem.l  (sp), d0-d2/a0-a1
   lea      (20,sp), sp
   unlk a6
   rte

FNET_COMP_ASM_END



