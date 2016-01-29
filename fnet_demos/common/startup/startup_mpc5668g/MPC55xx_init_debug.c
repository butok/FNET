
/*
 *
 * FILE : MPC55xx_init_debug.c
 *
 * DESCRIPTION:
 *  This file contains the MPC55xx derivative needed initializations. 
 *  MPC55xx_init_debug.c is only used when willing to debug in RAM. Otherwise, 
 *  MPC55xx_init.c shall be used.
 *  usr_init() is called by the startup code of the application at initialization time
 *  You can add needed hardware initializations here.
 */

#include "Exceptions.h"    /* IVPR and default exception handlers setup */
#include "INTCInterrupts.h" /* INTC Interrupts Requests configuration */
#include "MPC5668G_HWInit.h"
#include "MPC5668G.h"

#pragma section code_type ".init"

#define INIT_DERIVATIVE_INTERNAL_SETUP 0
#define INIT_EXTERNAL_BUS_INTERFACE_SETUP 0

#ifndef INIT_DERIVATIVE_INTERNAL_SETUP
#pragma error INIT_DERIVATIVE_INTERNAL_SETUP should be defined !
#endif

#ifndef INIT_EXTERNAL_BUS_INTERFACE_SETUP
#pragma error INIT_EXTERNAL_BUS_INTERFACE_SETUP should be defined !
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern __asm void __start();
__asm void usr_init();
/*lint -esym(752,__start) */

#ifdef __cplusplus
}
#endif

/* Init PLL */
void PLLinit(void)
{  	
	/* Configure the clocks */
	SIU.SYSCLK.B.SYSCLKSEL = 0x0;				/* Select IRC as CLK Source */ 

	FMPLL.ESYNCR1.B.CLKCFG = 0x7; 				/* Configure PLL CTRL Regs. Fsys = 94 MHz from a 40MHz oscillator*/
	FMPLL.ESYNCR2.B.ERFD = 5;					/* Fsys = (Fxtal * (EMFD+16))/((EPREDIV+1)*(ERFD+1)) */
	FMPLL.ESYNCR1.B.EPREDIV = 9;       
	FMPLL.ESYNCR1.B.EMFD = 125;     		

	while(FMPLL.SYNSR.B.LOCK != 1){}			/* Wait for PLL to lock */ 

	SIU.SYSCLK.B.SYSCLKSEL = 0x2;				/* Switch from IRC to PLL */ 

	SIU.SYSCLK.B.LPCLKDIV0 = 2;					/* Peripheral Set 1 divisor (SCI). PC1 = FSYS/4 = 16MHz */
} 

/*****************************************************************/
/* usr_init():                                                   */
/*   Define here the needed hardware initializations at startup  */

__asm void usr_init()
{
    /* Add needed hardware initializations in this function */

    nofralloc

    mflr     r30                            /* Save off return address in NV reg */

#if INIT_DERIVATIVE_INTERNAL_SETUP==1
    bl      INIT_Derivative                 /* Derivative specific hardware initializations */
#endif
#if INIT_EXTERNAL_BUS_INTERFACE_SETUP==1
    bl      INIT_ExternalBusAndMemory       /* Set up access to external memory (inc. chip select and MMU) */
#endif
    bl      EXCEP_InitExceptionHandlers     /* Set up Default Exception handling */
    bl      INTC_InitINTCInterrupts         /* Set up INTC Interrupts Requests handling */
    bl      PLLinit                         /* Init PLL */    

    mtlr    r30                             /* Get saved return address */
    
    blr
}



