/*
 * File:		m52259evb_sysinit.h
 * Purpose:		Generic Power-on Reset configuration
 *
 * Notes:
 *
 */

#ifndef __M52259EVB_SYSINIT_H__
#define __M52259EVB_SYSINIT_H__

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************/
/*
 * Misc. Defines
 */
#ifdef	FALSE
#undef	FALSE
#endif
#define FALSE	(0)

#ifdef	TRUE
#undef	TRUE
#endif
#define	TRUE	(1)




#define REF_CLOCK_KHZ         (8000)


/* System Clock Info */ 
#define SYSTEM_CLOCK_KHZ           (80000)

/* Serial Port Info */
#define TERMINAL_PORT       (0)
#define TERMINAL_BAUD       (19200)
#undef  HW_FLOW_CONTROL


/********************************************************************/
/* __initialize_hardware Startup code routine
 * 
 * __initialize_hardware is called by the startup code right after reset, 
 * with interrupt disabled and SP pre-set to a valid memory area.
 * Here you should initialize memory and some peripherics;
 * at this point global variables are not initialized yet.
 * The startup code will initialize SP on return of this function.
 */
void __initialize_hardware(void);


#ifdef __cplusplus
}
#endif

#endif /* __M52259EVB_SYSINIT_H__ */


