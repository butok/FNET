  
/** 
 * FILE: Exceptions.c
 *
 *  DESCRIPTION: Setup of IVPR to point to the EXCEPTION_HANDLERS memory area 
 *               defined in the linker command file.
 *               Default setup of the IVORxx registers. 
*/


/*----------------------------------------------------------------------------*/
/* Includes                                                                   */
/*----------------------------------------------------------------------------*/

#include "Exceptions.h" /* Implement functions from this file */

/*----------------------------------------------------------------------------*/
/* Function Implementations                                                   */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#pragma push /* Save the current state */
/* Symbol EXCEPTION_HANDLERS is defined in the application linker command file (.lcf) 
   It is defined to the start of the code memory area used for the .__exception_handlers section. 
*/
/*lint -esym(752, EXCEPTION_HANDLERS) */
__declspec (section ".__exception_handlers_p0") extern long EXCEPTION_HANDLERS;  
#pragma force_active on
#pragma function_align 16 /* We use 16 bytes alignment for Exception handlers */
__declspec(interrupt)
__declspec (section ".__exception_handlers_p0")
void EXCEP_DefaultExceptionHandler(void)
{

}
#pragma force_active off
#pragma pop

__asm void EXCEP_InitExceptionHandlers(void)
{
nofralloc

    /* Set the IVPR to the Exception Handlers address defined in the lcf file */
    lis     r3,EXCEPTION_HANDLERS@h
    ori     r3,r3,EXCEPTION_HANDLERS@l
    mtivpr  r3

	addi		r4,r3,0
	mtivor0 r4
	
	addi		r4,r3,0x10
	mtivor1 r4
	
	addi		r4,r3,0x20
	mtivor2 r4
	
	addi		r4,r3,0x30
	mtivor3 r4
	
	addi		r4,r3,0x40
	mtivor4 r4
	
	addi		r4,r3,0x50
	mtivor5 r4
	
	addi		r4,r3,0x60
	mtivor6 r4
	
	addi		r4,r3,0x70
	mtivor7 r4
	
	addi		r4,r3,0x80
	mtivor8 r4
	
	addi		r4,r3,0x90
	mtivor9 r4
	
	addi		r4,r3,0xa0
	mtivor10 r4
	
	addi		r4,r3,0xb0
	mtivor11 r4
	
	addi		r4,r3,0xc0
	mtivor12 r4
	
	addi		r4,r3,0xd0
	mtivor13 r4
	
	addi		r4,r3,0xe0
	mtivor14 r4
	
	addi		r4,r3,0xf0
	mtivor15 r4
    
    blr
}

#ifdef __cplusplus
}
#endif

