/*****************************************************************************

FILE
  __start_p1.c
  $Date: Dec-17-2012$
  $Revision: 1.2 $

DESCRIPTION
  A minimal startup function for the e200z0 core of a MPC551x family processor.

  This function initializes the stack to the values defined in the LCF file and
  uses the linker defined _SDA_BASE_ and _SDA2_BASE_ symbols to initialize
  r2 and r13 per (EABI). It then calls main_p1().

  The symbol "__start_p1" can be assigned to CRP.Z0VEC.R to begin executing
  this function. 
*/
#include <__ppc_eabi_init.h>
#include <__ppc_eabi_linker.h>	/* linker-generated symbol declarations */
#include "Exceptions_p1.h"

/* These symbols are defined in the Linker configuration file, but are not in */
/* __ppc_eabi_linker.h, so we'll provide the declarations here.  Note that    */
/* _heap_*_z0 definitions are currently not being used.                       */ 
__declspec(section ".init") extern char	_stack_addr_p1[];
__declspec(section ".init") extern char _stack_end_p1[];
__declspec(section ".init") extern char	_heap_addr_p1[];
__declspec(section ".init") extern char _heap_end_p1[];

extern void main_p1();

#ifdef __cplusplus
extern "C" {
#endif


/* The e200z0 core ONLY supports VLE, so make sure we build VLE code! */

__declspec(vle_on)
#pragma function_align 8
asm extern void __start_p1(void)
{
	nofralloc

	bl EXCEP_InitExceptionHandlers_P1

	/* Initialize stack pointer				*/
	lis	r1, _stack_addr_p1@ha
	addi	r1, r1, _stack_addr_p1@l   

  /* Note that we are assuming small data is shared between the cores */
  /* Initialize small data area pointers (EABI)                       */
	lis   r2, _SDA2_BASE_@ha
	addi  r2, r2, _SDA2_BASE_@l
  
	lis   r13, _SDA_BASE_@ha
	addi  r13, r13, _SDA_BASE_@l
  
  /* now call z0's main program (using short branch)... */ 
	bl main_p1
/*
If main_p1 must be addressed using FAR_ADDRESSING, use:

lis r6,main_p1@h
ori r6,r6,main_p1@l
mtlr r6
blrl

*/

  /* if main returns, just hang here */
here:   b here

	blr
}

#ifdef __cplusplus
}
#endif

