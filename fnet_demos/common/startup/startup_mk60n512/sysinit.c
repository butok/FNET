/*
 * File:        sysinit.c
 * Purpose:     Kinetis Configuration
 *              Initializes processor to a default state
 *
 * Notes:
 *
 */

#include "common.h"
#include "sysinit.h"

/********************************************************************/

/* Actual system clock frequency */
extern int core_clk_khz;
extern int core_clk_mhz;
extern int periph_clk_khz;

/********************************************************************/
void sysinit (void)
{
    /*
    * Enable all of the port clocks. These have to be enabled to configure
    * pin muxing options, so most code will need all of these on anyway.
    */
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                | SIM_SCGC5_PORTB_MASK
                | SIM_SCGC5_PORTC_MASK
                | SIM_SCGC5_PORTD_MASK
                | SIM_SCGC5_PORTE_MASK );

 	/* Ramp up the system clock */
    /* !!!! Be sure that FNET_CFG_CPU_CLOCK_HZ has proper value.!!!!!*/
    pll_init(CORE_CLK_MHZ, REF_CLK);
}

