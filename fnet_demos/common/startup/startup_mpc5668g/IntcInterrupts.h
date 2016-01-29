
/*
  FILE: IntcInterrupts.h
 
  DESCRIPTION: Contains defines for utilizing the Interrupt Controller in 
  the MPC551x. 
  There is also the function prototypes for the common interrupt handling 
  routines to be used on the MPC55xx (PowerPC Zen core).
 
 */

#ifndef _INTCINTERRUPTS_H_
#define _INTCINTERRUPTS_H_

/*----------------------------------------------------------------------------*/
/* Types                                                                      */
/*----------------------------------------------------------------------------*/

/** All interrupt handlers should be of this type */
typedef void(*INTCInterruptFn)(void);

/*----------------------------------------------------------------------------*/
/* Function declarations                                                      */
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function will setup the PowerPC Zen core to jump to an Interrupt 
 * Service Routine.
 * This function can be used from user_init() (no stack frame, no memory 
 * access).
 */
__asm void INTC_InitINTCInterrupts(void);

/**
 * This function can be used to install an interrupt handler for a given
 * interrupt vector. It will also set the Priority Status Register for the
 * source to the one given.
 * parameter handlerFn: The function to call when the interrupt occurs.
 * parameter vectoryNum: The number of the INTC Interrupt Request Source we 
 * wish to install the handler for.
 * parameter psrPriority: The priority to set in the Interrupt Controller 
 * Priority Select Register.
 */
void INTC_InstallINTCInterruptHandler(INTCInterruptFn handlerFn, 
                                      unsigned short vectorNum,
                                      unsigned char psrPriority);

#pragma section RX ".__exception_handlers_p0"

/**
 * This function is used to Handle the interrupt source by jumping to the ISR
 * branch table (IACKR)
 */
__declspec (section ".__exception_handlers_p0") void INTC_INTCInterruptHandler(void);

#ifdef __cplusplus
}
#endif

#endif

