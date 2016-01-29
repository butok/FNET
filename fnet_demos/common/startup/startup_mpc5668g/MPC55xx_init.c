
/*
 *
 * FILE : MPC55xx_init.c
 *
 * DESCRIPTION:
 *  This file contains the MPC55xx derivative needed initializations. 
 *  usr_init() is called by the startup code of the application at initialization time
 *  You can add needed hardware initializations here.
 *  This file also contains the RCHW and Reset Vector setup:
/*  The chip is by default setup to boot from internal Flash and the watchdog is disabled.
 */

#include "Exceptions.h"    /* IVPR and default exception handlers setup */
#include "INTCInterrupts.h" /* INTC Interrupts Requests configuration */
#include "MPC5668G_HWInit.h"
#include "MPC5668G.h"

#pragma section code_type ".init"

#define INIT_DERIVATIVE_INTERNAL_SETUP 1
#define INIT_EXTERNAL_BUS_INTERFACE_SETUP 1

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

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************/
/* RCHW and Reset Vector setup:                               */
/*   The chip is by default setup to boot from internal Flash */
/*   and the watchdog is disabled.                            */ 

typedef void (*resetfuncptr)(void);

#pragma push /* Save the current state */
#pragma section const_type sconst_type ".__bam_bootarea"
extern const unsigned long bam_rchw;
extern const resetfuncptr bam_resetvector;

/* RCHW_VALUE Flags */
#define RCHW_WTE 0x0400L        /* Enable Watchdog */
#define RCHW_VLE 0x0100L        /* Enable Variable Length Encoding*/
#define RCHW_PS0_32BITS 0x0000L /* Boot from External Bus CS0, 32-bit CS0 port size. */
#define RCHW_PS0_16BITS 0x0200L /* Boot from External Bus CS0, 16-bit CS0 port size. */
#define RCHW_BOOTIDENTIFIER 0x005AL

/* Used RCHW value: boot from internal flash, watchdog disabled */
#if VLE_IS_ON == 1
#define RCHW_VALUE RCHW_BOOTIDENTIFIER|RCHW_PS0_32BITS|RCHW_VLE
#else
#define RCHW_VALUE RCHW_BOOTIDENTIFIER|RCHW_PS0_32BITS 
#endif

const unsigned long bam_rchw = ((RCHW_VALUE)<<16) + (RCHW_VALUE);
const resetfuncptr bam_resetvector = __start;

#pragma pop

#ifdef __cplusplus
}
#endif

