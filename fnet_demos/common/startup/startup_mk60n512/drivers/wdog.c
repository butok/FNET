/*
 * File:        wdog.c
 * Purpose:     Provide common watchdog module routines
 *
 * Notes:		Need to add more functionality. Right now it
 * 				is just a disable routine since we know almost
 * 				all projects will need that.       
 *              
 */

#include "common.h"
#include "wdog.h"
       
/********************************************************************/
/*
 * Watchdog timer disable routine
 *
 * Parameters:
 * none
 */
void wdog_disable(void)
{
	/* First unlock the watchdog so that we can write to registers */

	/* Write 0xC520 to the unlock register */
	WDOG_UNLOCK = 0xC520;
	
	/* Followed by 0xD928 to complete the unlock */
	WDOG_UNLOCK = 0xD928;
	
	/* Clear the WDOGEN bit to disable the watchdog */
	WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}

