/*
 * File:	start.c
 * Purpose:	Kinetis start up routines. 
 *
 * Notes:		
 */

#include "start.h"
#include "common.h"
#include "wdog.h"
#include "sysinit.h"

#include "fnet.h"

void start(void);

#if FNET_CFG_COMP_CW
extern char __SP_INIT[];
asm void __startup(void)
{
			//	setup the stack before we attempt anything else
			lda	r0,__SP_INIT
			// skip stack setup if __SP_INIT is 0
			// assume sp is already setup.
			cmp	r0,#0
			beq	skip_sp
			mov	sp,r0
			sub	sp,#4
			mov	r0,#0
			mvn	r0,r0
			str	r0,[sp,#0]
			add	sp,#4
	skip_sp:
			bl start
}
#endif /* FNET_CFG_COMP_CW */


#if FNET_CFG_COMP_UV
extern void __main(void); 
#endif /* FNET_CFG_COMP_UV */

/********************************************************************/
/*!
 * \brief   Kinetis Start
 * \return  None
 *
 * This function calls all of the needed starup routines and then 
 * branches to the main process.
 */
void start(void)
{ 
	
    /* Disable the watchdog timer */
    wdog_disable();

    /* Copy any vector or data sections that need to be in RAM */
    common_startup();

    /* Perform processor initialization */
    sysinit();

    /* Jump to main process */
#if FNET_CFG_COMP_UV
    __main(); /* Keil-specific initialisation => main().*/
#else
    main();
#endif

	/* No actions to perform after this so wait forever */
	while(1);

}



/********************************************************************/

