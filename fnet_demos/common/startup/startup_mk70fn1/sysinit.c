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

#ifdef DDR_INIT
  #include "sdram.h"
#endif

/********************************************************************/

/* Actual system clock frequency */
int mcg_clk_hz;
int mcg_clk_khz;
int core_clk_khz;
int periph_clk_khz;
int pll_0_clk_khz;
int pll_1_clk_khz;

/********************************************************************/
void sysinit (void)
{
  
/* Check to see if this is an SDRAM target. If so a script that 
 * initializes the port clocks and the PLL should have already 
 * run.
 */  
#ifdef SDRAM 
    /* Initialize clocks to match values from init script */
    mcg_clk_hz = 120000000;
    pll_1_clk_khz = 150000;
    pll_0_clk_khz = mcg_clk_hz / 1000;       
#else  // if not using dram script  
  

    /*
     * Enable all of the port clocks. These have to be enabled to configure
     * pin muxing options, so most code will need all of these on anyway.
     */
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK
                  | SIM_SCGC5_PORTC_MASK
                  | SIM_SCGC5_PORTD_MASK
                  | SIM_SCGC5_PORTE_MASK 
                  | SIM_SCGC5_PORTF_MASK );


    // releases hold with ACKISO:  Only has an effect if recovering from VLLS1, VLLS2, or VLLS3
    // if ACKISO is set you must clear ackiso before calling pll_init 
    //    or pll init hangs waiting for OSC to initialize
    // if osc enabled in low power modes - enable it first before ack
    // if I/O needs to be maintained without glitches enable outputs and modules first before ack.
    if (PMC_REGSC &  PMC_REGSC_ACKISO_MASK)
        PMC_REGSC |= PMC_REGSC_ACKISO_MASK;

    #if defined(NO_PLL_INIT)
        mcg_clk_hz = 21000000; //FEI mode
    #elif defined (ASYNCH_MODE)  
       /* Set the system dividers */
       /* NOTE: The PLL init will not configure the system clock dividers,
        * so they must be configured appropriately before calling the PLL
        * init function to ensure that clocks remain in valid ranges.
        */  
        SIM_CLKDIV1 = ( 0
                        | SIM_CLKDIV1_OUTDIV1(0)
                        | SIM_CLKDIV1_OUTDIV2(1)
                        | SIM_CLKDIV1_OUTDIV3(2)
                        | SIM_CLKDIV1_OUTDIV4(5) );

       /* Initialize PLL0 */
       /* PLL0 will be the source for MCG CLKOUT so the core, system, FlexBus, and flash clocks are derived from it */ 
       mcg_clk_hz = pll_init(OSCINIT,   /* Initialize the oscillator circuit */
                             OSC_0,     /* Use CLKIN0 as the input clock */
                             CLK0_FREQ_HZ,  /* CLKIN0 frequency */
                             LOW_POWER,     /* Set the oscillator for low power mode */
                             CLK0_TYPE,     /* Crystal or canned oscillator clock input */
                             PLL_0,         /* PLL to initialize, in this case PLL0 */
                             PLL0_PRDIV,    /* PLL predivider value */
                             PLL0_VDIV,     /* PLL multiplier */
                             MCGOUT);       /* Use the output from this PLL as the MCGOUT */
    
       /* Check the value returned from pll_init() to make sure there wasn't an error */
      // if (mcg_clk_hz < 0x100)
      //   while(1);

       /* Initialize PLL1 */
       /* PLL1 will be the source for the DDR controller, but NOT the MCGOUT */   
       pll_1_clk_khz = (pll_init(NO_OSCINIT, /* Don't init the osc circuit, already done */
                                 OSC_0,      /* Use CLKIN0 as the input clock */
                                 CLK0_FREQ_HZ,  /* CLKIN0 frequency */
                                 LOW_POWER,     /* Set the oscillator for low power mode */
                                 CLK0_TYPE,     /* Crystal or canned oscillator clock input */
                                 PLL_1,         /* PLL to initialize, in this case PLL1 */
                                 PLL1_PRDIV,    /* PLL predivider value */
                                 PLL1_VDIV,     /* PLL multiplier */
                                 PLL_ONLY) / 1000);   /* Don't use the output from this PLL as the MCGOUT */

       /* Check the value returned from pll_init() to make sure there wasn't an error */
      // if ((pll_1_clk_khz * 1000) < 0x100)
      //   while(1);

       pll_0_clk_khz = mcg_clk_hz / 1000;       
       
    #elif defined (SYNCH_MODE)  
       /* Set the system dividers */
       /* NOTE: The PLL init will not configure the system clock dividers,
        * so they must be configured appropriately before calling the PLL
        * init function to ensure that clocks remain in valid ranges.
        */  
        SIM_CLKDIV1 = ( 0
                        | SIM_CLKDIV1_OUTDIV1(0)
                        | SIM_CLKDIV1_OUTDIV2(2)
                        | SIM_CLKDIV1_OUTDIV3(2)
                        | SIM_CLKDIV1_OUTDIV4(5) );
 
       /* Initialize PLL1 */
       /* PLL1 will be the source MCGOUT and the DDR controller */   
       mcg_clk_hz = pll_init(OSCINIT, /* Don't init the osc circuit, already done */
                                 OSC_0,      /* Use CLKIN0 as the input clock */
                                 CLK0_FREQ_HZ,  /* CLKIN0 frequency */
                                 LOW_POWER,     /* Set the oscillator for low power mode */
                                 CLK0_TYPE,     /* Crystal or canned oscillator clock input */
                                 PLL_1,         /* PLL to initialize, in this case PLL1 */
                                 PLL1_PRDIV,    /* PLL predivider value */
                                 PLL1_VDIV,     /* PLL multiplier */
                                 MCGOUT);   /* Don't use the output from this PLL as the MCGOUT */

       /* Check the value returned from pll_init() to make sure there wasn't an error */
      // if (mcg_clk_hz < 0x100)
      //   while(1);

       /* Initialize PLL0 */
       /* PLL0 is initialized, but not used as the MCGOUT */ 
       pll_0_clk_khz = (pll_init(NO_OSCINIT,   /* Initialize the oscillator circuit */
                             OSC_0,     /* Use CLKIN0 as the input clock */
                             CLK0_FREQ_HZ,  /* CLKIN0 frequency */
                             LOW_POWER,     /* Set the oscillator for low power mode */
                             CLK0_TYPE,     /* Crystal or canned oscillator clock input */
                             PLL_0,         /* PLL to initialize, in this case PLL0 */
                             PLL0_PRDIV,    /* PLL predivider value */
                             PLL0_VDIV,     /* PLL multiplier */
                             PLL_ONLY) / 1000);       /* Use the output from this PLL as the MCGOUT */

       /* Check the value returned from pll_init() to make sure there wasn't an error */
      // if ((pll_0_clk_khz * 1000) < 0x100)
      //   while(1);

       pll_1_clk_khz = mcg_clk_hz / 1000;       
       
    #else
        #error "A PLL configuration for this platform is NOT defined"
    #endif
#endif          

	/*
         * Use the value obtained from the pll_init function to define variables
	 * for the core clock in kHz and also the peripheral clock. These
	 * variables can be used by other functions that need awareness of the
	 * system frequency.
	 */
    mcg_clk_khz = mcg_clk_hz / 1000;
  	core_clk_khz = mcg_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> 28)+ 1);
  	periph_clk_khz = mcg_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);

  	/* For debugging purposes, enable the trace clock and/or CLKOUT so that
  	 * we'll be able to monitor clocks and know the PLL is at the frequency
  	 * that we expect.
  	 */
	trace_clk_init();

  	clkout_init();
       
    /* Initialize the DDR if the project option if defined */
    #ifdef DDR_INIT
      twr_ddr2_script_init();
    #endif
}
/********************************************************************/
void trace_clk_init(void)
{
	/* Set the trace clock to the core clock frequency */
	SIM_SOPT2 |= SIM_SOPT2_TRACECLKSEL_MASK;

	/* Enable the TRACE_CLKOUT pin function on PTF23 (alt6 function) */
	PORTF_PCR23 = ( PORT_PCR_MUX(0x6));
}
/********************************************************************/
void clkout_init(void)
{
        /* Due to errata #4218 the FlexBus should not be clocked
         * without initializing at least one chip select. So we'll
         * configure the CLKOUT pin to drive the Flash clock instead
         * of the default FlexBus clock.
         */
        
        /* Disable the FlexBus clock gating for now (err #4218 workaround).
         * If you want to use the FlexBus in your code you must re-enable
         * the clock gate and initialize at least one chip select.
         */
        SIM_SCGC7 &= ~SIM_SCGC7_FLEXBUS_MASK;
        
        /* Select the flash clock output option for the CLKOUT pin */
        SIM_SOPT2 |= SIM_SOPT2_CLKOUTSEL(0x2);

 	/* Enable the CLKOUT function on PTC3 (alt5 function) */
	PORTC_PCR3 = ( PORT_PCR_MUX(0x5));
}
/********************************************************************/
