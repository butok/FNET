
/**
 * FILE: Exceptions.h
 *
 *  DESCRIPTION: Setup of IVPR to point to the EXCEPTION_HANDLERS memory area 
 *               defined in the linker command file.
 *
 */

#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

/*----------------------------------------------------------------------------*/
/* Function declarations                                                      */
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#pragma section RX ".__exception_handlers_p0"

/**
 * This function is used as default exceptions handler
 */
__declspec (section ".__exception_handlers_p0") void EXCEP_DefaultExceptionHandler(void);


/**
 * This function will setup the PowerPC Zen core IVPR and IVORxx registers.
 * IVPR will be set to the EXCEPTION_HANDLERS memory area defined in the 
 * linker command file (.lcf of the current build target)  
 * IVORxx will be set by default to the exception handler function: 
 *     __DefaultExceptionHandler__.
 *
 * If an Exception is used in the application code, the exception handler routine
 * should be defined like the EXCEP_DefaultExceptionHandler function 
 * (i.e. interrupt function, forced active and placed in the 
 * ".__exception_handlers_z1" code section). 
 * The corresponding entries in the ivor_branch_table should then be set to 
 * branch to this address.
 *    
 */
__asm void EXCEP_InitExceptionHandlers(void);

#ifdef __cplusplus
}
#endif

#endif

