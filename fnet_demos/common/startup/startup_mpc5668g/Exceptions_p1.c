  
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

#include "Exceptions_p1.h" /* Implement functions from this file */

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
__declspec (section ".__exception_handlers_p1") extern long EXCEPTION_HANDLERS_P1;  
#pragma force_active on
#pragma function_align 16 /* We use 16 bytes alignment for Exception handlers */
__declspec(interrupt)
__declspec (section ".__exception_handlers_p1")
void EXCEP_DefaultExceptionHandler_p1(void)
{

}
#pragma force_active off
#pragma pop

__asm void EXCEP_InitExceptionHandlers_P1(void)
{
nofralloc
    /* Set the IVPR to the Exception Handlers address defined in the lcf file */
	lis     r0,EXCEPTION_HANDLERS_P1@h
    ori     r0,r0,EXCEPTION_HANDLERS_P1@l
    mtivpr  r0

    blr
}

#ifdef __cplusplus
}
#endif

