/*
 * File:    mcg.c
 *
 * Notes:
 * Assumes the MCG mode is in the default FEI mode out of reset
 */

#include "common.h"
#include "mcg.h"
#include "lptmr.h"

// global variables
extern int core_clk_khz;
//extern int slow_irc_freq = 32768; // default slow irc frequency is 32768Hz
//extern int fast_irc_freq = 4000000; // default fast irc frequency is 4MHz

extern char drs_val, dmx32_val;





/*********************************************************************************************/
/* Functon name : pll_init
 *
 * Mode transition: Option to move from FEI to PEE mode or to just initialize the PLL
 *
 * This function initializess either PLL0 or PLL1. Either OSC0 or OSC1 can be selected for the
 * reference clock source. The oscillators can be configured to use a crystal or take in an
 * external square wave clock.
 * NOTE : This driver does not presently (as of Sept 9 2011) support the use of OSC1 as the
 * reference clock for the MCGOUT clock used for the system clocks.
 * The PLL outputs a PLLCLK and PLLCLK2X. PLLCLK2X is the actual PLL frequency and PLLCLK is
 * half this frequency. PLLCLK is used for MCGOUT and is also typically used by the
 * peripherals that can select the PLL as a clock source. So the PLL frequency generated will
 * be twice the desired frequency.
 * Using the function parameter names the PLL frequency is calculated as follows:
 * PLL freq = ((crystal_val / prdiv_val) * vdiv_val)
 * Refer to the readme file in the mcg driver directory for examples of pll_init configurations.
 * All parameters must be provided, for example crystal_val must be provided even if the
 * oscillator associated with that parameter is already initialized.
 * The various passed parameters are checked to ensure they are within the allowed range. If any
 * of these checks fail the driver will exit and return a fail/error code. An error code will
 * also be returned if any error occurs during the PLL initialization sequence. Refer to the
 * readme file in the mcg driver directory for a list of all these codes.
 *
 * Parameters: init_osc    - 0 if oscillator does not need to be initialized, non-zero if the
 *                           oscillator needs to be configured.
 *             osc_select  - 0 to select OSC0, non-zero to select OSC1.
 *             crystal_val - external clock frequency in Hz either from a crystal or square
 *                           wave clock source
 *             hgo_val     - selects whether low power or high gain mode is selected
 *                           for the crystal oscillator. This has no meaning if an
 *                           external clock is used.
 *             erefs_val   - selects external clock (=0) or crystal osc (=1)
 *             pll_select  - 0 to select PLL0, non-zero to select PLL1.
 *             prdiv_val   - value to divide the external clock source by to create the desired
 *                           PLL reference clock frequency
 *             vdiv_val    - value to multiply the PLL reference clock frequency by
 *             mcgout_select  - 0 if the PLL is just to be enabled, non-zero if the PLL is used
 *                              to provide the MCGOUT clock for the system.
 *
 * Return value : PLL frequency (Hz) divided by 2 or error code
 */

int pll_init(unsigned char init_osc, unsigned char osc_select, int crystal_val, unsigned char hgo_val, unsigned char erefs_val, unsigned char pll_select, signed char prdiv_val, signed char vdiv_val, unsigned char mcgout_select)
{
  unsigned char frdiv_val;
  unsigned char temp_reg;
  unsigned char prdiv, vdiv;
  short i;
  int ref_freq;
  int pll_freq;

  // If using the PLL as MCG_OUT must check if the MCG is in FEI mode first
  if (mcgout_select)
  {
    // check if in FEI mode
    if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) && // check CLKS mux has selcted FLL output
        (MCG_S & MCG_S_IREFST_MASK) &&                                  // check FLL ref is internal ref clk
        (!(MCG_S & MCG_S_PLLST_MASK))))                                 // check PLLS mux has selected FLL
    {
      return 0x1;                                                     // return error code
    }
  } // if (mcgout_select)

  // Check if OSC1 is being used as a reference for the MCGOUT PLL
  // This requires a more complicated MCG configuration.
  // At this time (Sept 8th 2011) this driver does not support this option
  if (osc_select && mcgout_select)
  {
    return 0x80; // Driver does not support using OSC1 as the PLL reference for the system clock on MCGOUT
  }

  // check external frequency is less than the maximum frequency
  if  (crystal_val > 60000000) {return 0x21;}

  // check crystal frequency is within spec. if crystal osc is being used as PLL ref
  if (erefs_val)
  {
    if ((crystal_val < 8000000) || (crystal_val > 32000000)) {return 0x22;} // return 1 if one of the available crystal options is not available
  }

  // make sure HGO will never be greater than 1. Could return an error instead if desired.
  if (hgo_val > 0)
  {
    hgo_val = 1; // force hgo_val to 1 if > 0
  }

  // Check PLL divider settings are within spec.
  if ((prdiv_val < 1) || (prdiv_val > 8)) {return 0x41;}
  if ((vdiv_val < 16) || (vdiv_val > 47)) {return 0x42;}

  // Check PLL reference clock frequency is within spec.
  ref_freq = crystal_val / prdiv_val;
  if ((ref_freq < 8000000) || (ref_freq > 16000000)) {return 0x43;}

  // Check PLL output frequency is within spec.
  pll_freq = (crystal_val / prdiv_val) * vdiv_val;
  if ((pll_freq < 180000000) || (pll_freq > 360000000)) {return 0x45;}

  // Determine if oscillator needs to be set up
  if (init_osc)
  {
    // Check if the oscillator needs to be configured
    if (!osc_select)
    {
      // configure the MCG_C2 register
      // the RANGE value is determined by the external frequency. Since the RANGE parameter affects the FRDIV divide value
      // it still needs to be set correctly even if the oscillator is not being used
      
      temp_reg = MCG_C2;
      temp_reg &= ~(MCG_C2_RANGE_MASK | MCG_C2_HGO_MASK | MCG_C2_EREFS_MASK); // clear fields before writing new values
    
      if (crystal_val <= 8000000)
      {
        temp_reg |= (MCG_C2_RANGE(1) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT));
      }
      else
      {
        // On rev. 1.0 of silicon there is an issue where the the input bufferd are enabled when JTAG is connected.
        // This has the affect of sometimes preventing the oscillator from running. To keep the oscillator amplitude
        // low, RANGE = 2 should not be used. This should be removed when fixed silicon is available.
        //temp_reg |= (MCG_C2_RANGE(2) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT));
        temp_reg |= (MCG_C2_RANGE(1) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT));
      }
      MCG_C2 = temp_reg;
    }
    else
    {
      // configure the MCG_C10 register
      // the RANGE value is determined by the external frequency. Since the RANGE parameter affects the FRDIV divide value
      // it still needs to be set correctly even if the oscillator is not being used
      temp_reg = MCG_C10;
      temp_reg &= ~(MCG_C10_RANGE2_MASK | MCG_C10_HGO2_MASK | MCG_C10_EREFS2_MASK); // clear fields before writing new values
      if (crystal_val <= 8000000)
      {
        temp_reg |= MCG_C10_RANGE2(1) | (hgo_val << MCG_C10_HGO2_SHIFT) | (erefs_val << MCG_C10_EREFS2_SHIFT);
      }
      else
      {
        // On rev. 1.0 of silicon there is an issue where the the input bufferd are enabled when JTAG is connected.
        // This has the affect of sometimes preventing the oscillator from running. To keep the oscillator amplitude
        // low, RANGE = 2 should not be used. This should be removed when fixed silicon is available.
        //temp_reg |= MCG_C10_RANGE2(2) | (hgo_val << MCG_C10_HGO2_SHIFT) | (erefs_val << MCG_C10_EREFS2_SHIFT);
        temp_reg |= MCG_C10_RANGE2(1) | (hgo_val << MCG_C10_HGO2_SHIFT) | (erefs_val << MCG_C10_EREFS2_SHIFT);
      }
      MCG_C10 = temp_reg;
    } // if (!osc_select)
  } // if (init_osc)

  if (mcgout_select)
  {
    // determine FRDIV based on reference clock frequency
    // since the external frequency has already been checked only the maximum frequency for each FRDIV value needs to be compared here.
    if (crystal_val <= 1250000) {frdiv_val = 0;}
    else if (crystal_val <= 2500000) {frdiv_val = 1;}
    else if (crystal_val <= 5000000) {frdiv_val = 2;}
    else if (crystal_val <= 10000000) {frdiv_val = 3;}
    else if (crystal_val <= 20000000) {frdiv_val = 4;}
    else {frdiv_val = 5;}

    // Select external oscillator and Reference Divider and clear IREFS to start ext osc
    // If IRCLK is required it must be enabled outside of this driver, existing state will be maintained
    // CLKS=2, FRDIV=frdiv_val, IREFS=0, IRCLKEN=0, IREFSTEN=0
    temp_reg = MCG_C1;
    temp_reg &= ~(MCG_C1_CLKS_MASK | MCG_C1_FRDIV_MASK | MCG_C1_IREFS_MASK); // Clear values in these fields
    temp_reg = MCG_C1_CLKS(2) | MCG_C1_FRDIV(frdiv_val); // Set the required CLKS and FRDIV values
    MCG_C1 = temp_reg;

    // if the external oscillator is used need to wait for OSCINIT to set
    if (erefs_val)
    {
      for (i = 0 ; i < 10000 ; i++)
      {
        if (MCG_S & MCG_S_OSCINIT_MASK) break; // jump out early if OSCINIT sets before loop finishes
      }
      if (!(MCG_S & MCG_S_OSCINIT_MASK)) return 0x23; // check bit is really set and return with error if not set
    }

    // wait for Reference clock Status bit to clear
    for (i = 0 ; i < 2000 ; i++)
    {
      if (!(MCG_S & MCG_S_IREFST_MASK)) break; // jump out early if IREFST clears before loop finishes
    }
    if (MCG_S & MCG_S_IREFST_MASK) return 0x11; // check bit is really clear and return with error if not set

    // Wait for clock status bits to show clock source is ext ref clk
    for (i = 0 ; i < 2000 ; i++)
    {
      if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) break; // jump out early if CLKST shows EXT CLK slected before loop finishes
    }
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2) return 0x1A; // check EXT CLK is really selected and return with error if not

    // Now in FBE
    // It is recommended that the clock monitor is enabled when using an external clock as the clock source/reference.
    // It is enabled here but can be removed if this is not required.
    MCG_C6 |= MCG_C6_CME_MASK;

    // Select which PLL to enable
    if (!pll_select)
    {
      // Configure PLL0
      // Ensure OSC0 is selected as the reference clock
      MCG_C5 &= ~MCG_C5_PLLREFSEL_MASK;
      //Select PLL0 as the source of the PLLS mux
      MCG_C11 &= ~MCG_C11_PLLCS_MASK;
      // Configure MCG_C5
      // If the PLL is to run in STOP mode then the PLLSTEN bit needs to be OR'ed in here or in user code.
      temp_reg = MCG_C5;
      temp_reg &= ~MCG_C5_PRDIV_MASK;
      temp_reg |= MCG_C5_PRDIV(prdiv_val - 1);    //set PLL ref divider
      MCG_C5 = temp_reg;

      // Configure MCG_C6
      // The PLLS bit is set to enable the PLL, MCGOUT still sourced from ext ref clk
      // The loss of lock interrupt can be enabled by seperately OR'ing in the LOLIE bit in MCG_C6
      temp_reg = MCG_C6; // store present C6 value
      temp_reg &= ~MCG_C6_VDIV_MASK; // clear VDIV settings
      temp_reg |= MCG_C6_PLLS_MASK | MCG_C6_VDIV(vdiv_val - 16); // write new VDIV and enable PLL
      MCG_C6 = temp_reg; // update MCG_C6

      // wait for PLLST status bit to set
      for (i = 0 ; i < 2000 ; i++)
      {
        if (MCG_S & MCG_S_PLLST_MASK) break; // jump out early if PLLST sets before loop finishes
      }
      if (!(MCG_S & MCG_S_PLLST_MASK)) return 0x16; // check bit is really set and return with error if not set

      // Wait for LOCK bit to set
      for (i = 0 ; i < 2000 ; i++)
      {
        if (MCG_S & MCG_S_LOCK_MASK) break; // jump out early if LOCK sets before loop finishes
      }
      if (!(MCG_S & MCG_S_LOCK_MASK)) return 0x44; // check bit is really set and return with error if not set

      // Use actual PLL settings to calculate PLL frequency
      prdiv = ((MCG_C5 & MCG_C5_PRDIV_MASK) + 1);
      vdiv = ((MCG_C6 & MCG_C6_VDIV_MASK) + 16);
    }
    else
    {
      // Configure PLL1
      // Ensure OSC0 is selected as the reference clock
      MCG_C11 &= ~MCG_C11_PLLREFSEL2_MASK;
      //Select PLL1 as the source of the PLLS mux
      MCG_C11 |= MCG_C11_PLLCS_MASK;
      // Configure MCG_C11
      // If the PLL is to run in STOP mode then the PLLSTEN2 bit needs to be OR'ed in here or in user code.
      temp_reg = MCG_C11;
      temp_reg &= ~MCG_C11_PRDIV2_MASK;
      temp_reg |= MCG_C11_PRDIV2(prdiv_val - 1);    //set PLL ref divider
      MCG_C11 = temp_reg;

      // Configure MCG_C12
      // The PLLS bit is set to enable the PLL, MCGOUT still sourced from ext ref clk
      // The loss of lock interrupt can be enabled by seperately OR'ing in the LOLIE2 bit in MCG_C12
      temp_reg = MCG_C12; // store present C12 value
      temp_reg &= ~MCG_C12_VDIV2_MASK; // clear VDIV settings
      temp_reg |=  MCG_C12_VDIV2(vdiv_val - 16); // write new VDIV and enable PLL
      MCG_C12 = temp_reg; // update MCG_C12
      // Enable PLL by setting PLLS bit
      MCG_C6 |= MCG_C6_PLLS_MASK;

      // wait for PLLCST status bit to set
      for (i = 0 ; i < 2000 ; i++)
      {
        if (MCG_S2 & MCG_S2_PLLCST_MASK) break; // jump out early if PLLST sets before loop finishes
      }
      if (!(MCG_S2 & MCG_S2_PLLCST_MASK)) return 0x17; // check bit is really set and return with error if not set

      // wait for PLLST status bit to set
      for (i = 0 ; i < 2000 ; i++)
      {
        if (MCG_S & MCG_S_PLLST_MASK) break; // jump out early if PLLST sets before loop finishes
      }
      if (!(MCG_S & MCG_S_PLLST_MASK)) return 0x16; // check bit is really set and return with error if not set

      // Wait for LOCK bit to set
      for (i = 0 ; i < 2000 ; i++)
      {
        if (MCG_S2 & MCG_S2_LOCK2_MASK) break; // jump out early if LOCK sets before loop finishes
      }
      if (!(MCG_S2 & MCG_S2_LOCK2_MASK)) return 0x44; // check bit is really set and return with error if not set

      // Use actual PLL settings to calculate PLL frequency
      prdiv = ((MCG_C11 & MCG_C11_PRDIV2_MASK) + 1);
      vdiv = ((MCG_C12 & MCG_C12_VDIV2_MASK) + 16);
    } // if (!pll_select)

    // now in PBE

    MCG_C1 &= ~MCG_C1_CLKS_MASK; // clear CLKS to switch CLKS mux to select PLL as MCG_OUT

    // Wait for clock status bits to update
    for (i = 0 ; i < 2000 ; i++)
    {
      if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x3) break; // jump out early if CLKST = 3 before loop finishes
    }
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3) return 0x1B; // check CLKST is set correctly and return with error if not

    // Now in PEE
  }
  else
  {
    // Setup PLL for peripheral only use
    if (pll_select)
    {
      // Setup and enable PLL1
      // Select ref source
      if (osc_select)
      {
        MCG_C11 |= MCG_C11_PLLREFSEL2_MASK; // Set select bit to choose OSC1
      }
      else
      {
        MCG_C11 &= ~MCG_C11_PLLREFSEL2_MASK; // Clear select bit to choose OSC0
      }
      // Configure MCG_C11
      // If the PLL is to run in STOP mode then the PLLSTEN2 bit needs to be OR'ed in here or in user code.
      temp_reg = MCG_C11;
      temp_reg &= ~MCG_C11_PRDIV2_MASK;
      temp_reg |= MCG_C11_PRDIV2(prdiv_val - 1);    //set PLL ref divider
      MCG_C11 = temp_reg;

      // Configure MCG_C12
      // The loss of lock interrupt can be enabled by seperately OR'ing in the LOLIE2 bit in MCG_C12
      temp_reg = MCG_C12; // store present C12 value
      temp_reg &= ~MCG_C12_VDIV2_MASK; // clear VDIV settings
      temp_reg |=  MCG_C12_VDIV2(vdiv_val - 16); // write new VDIV and enable PLL
      MCG_C12 = temp_reg; // update MCG_C12
      // Now enable the PLL
      MCG_C11 |= MCG_C11_PLLCLKEN2_MASK; // Set PLLCLKEN2 to enable PLL1

      // Wait for LOCK bit to set
      for (i = 0 ; i < 2000 ; i++)
      {
        if (MCG_S2 & MCG_S2_LOCK2_MASK) break; // jump out early if LOCK sets before loop finishes
      }
      if (!(MCG_S2 & MCG_S2_LOCK2_MASK)) return 0x44; // check bit is really set and return with error if not set

      // Use actual PLL settings to calculate PLL frequency
      prdiv = ((MCG_C11 & MCG_C11_PRDIV2_MASK) + 1);
      vdiv = ((MCG_C12 & MCG_C12_VDIV2_MASK) + 16);
    }
    else
    {
      // Setup and enable PLL0
      // Select ref source
      if (osc_select)
      {
        MCG_C5 |= MCG_C5_PLLREFSEL_MASK; // Set select bit to choose OSC1
      }
      else
      {
        MCG_C5 &= ~MCG_C5_PLLREFSEL_MASK; // Clear select bit to choose OSC0
      }
      // Configure MCG_C5
      // If the PLL is to run in STOP mode then the PLLSTEN bit needs to be OR'ed in here or in user code.
      temp_reg = MCG_C5;
      temp_reg &= ~MCG_C5_PRDIV_MASK;
      temp_reg |= MCG_C5_PRDIV(prdiv_val - 1);    //set PLL ref divider
      MCG_C5 = temp_reg;

      // Configure MCG_C6
      // The loss of lock interrupt can be enabled by seperately OR'ing in the LOLIE bit in MCG_C6
      temp_reg = MCG_C6; // store present C6 value
      temp_reg &= ~MCG_C6_VDIV_MASK; // clear VDIV settings
      temp_reg |=  MCG_C6_VDIV(vdiv_val - 16); // write new VDIV and enable PLL
      MCG_C6 = temp_reg; // update MCG_C6
      // Now enable the PLL
      MCG_C5 |= MCG_C5_PLLCLKEN_MASK; // Set PLLCLKEN to enable PLL0

      // Wait for LOCK bit to set
      for (i = 0 ; i < 2000 ; i++)
      {
        if (MCG_S & MCG_S_LOCK_MASK) break; // jump out early if LOCK sets before loop finishes
      }
      if (!(MCG_S & MCG_S_LOCK_MASK)) return 0x44; // check bit is really set and return with error if not set

      // Use actual PLL settings to calculate PLL frequency
      prdiv = ((MCG_C5 & MCG_C5_PRDIV_MASK) + 1);
      vdiv = ((MCG_C6 & MCG_C6_VDIV_MASK) + 16);
    } // if (pll_select)

  } // if (mcgout_select)

  return (((crystal_val / prdiv) * vdiv) / 2); //MCGOUT equals PLL output frequency/2
} // pll_init


unsigned char fll_rtc_init(unsigned char clk_option, unsigned char crystal_val)
{
  unsigned char pll_freq;

  rtc_as_refclk();
  pll_freq = 24;
  return pll_freq;
}

 /*
  * This routine must be placed in RAM. It is a workaround for errata e2448.
  * Flash prefetch must be disabled when the flash clock divider is changed.
  * This cannot be performed while executing out of flash.
  * There must be a short delay after the clock dividers are changed before prefetch
  * can be re-enabled.
  */
#if FNET_CFG_COMP_CW
    __declspec(data) 
#endif
#if FNET_CFG_COMP_IAR 
    __ramfunc
#endif
void set_sys_dividers(uint32 outdiv1, uint32 outdiv2, uint32 outdiv3, uint32 outdiv4)	
{
  uint32 temp_reg;
  uint8 i;

  temp_reg = FMC_PFAPR; // store present value of FMC_PFAPR

  // set M0PFD through M7PFD to 1 to disable prefetch
  FMC_PFAPR |= FMC_PFAPR_M7PFD_MASK | FMC_PFAPR_M6PFD_MASK | FMC_PFAPR_M5PFD_MASK
             | FMC_PFAPR_M4PFD_MASK | FMC_PFAPR_M3PFD_MASK | FMC_PFAPR_M2PFD_MASK
             | FMC_PFAPR_M1PFD_MASK | FMC_PFAPR_M0PFD_MASK;

  // set clock dividers to desired value
  SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(outdiv1) | SIM_CLKDIV1_OUTDIV2(outdiv2)
              | SIM_CLKDIV1_OUTDIV3(outdiv3) | SIM_CLKDIV1_OUTDIV4(outdiv4);

  // wait for dividers to change
  for (i = 0 ; i < outdiv4 ; i++)
  {}

  FMC_PFAPR = temp_reg; // re-store original value of FMC_PFAPR

  return;
} // set_sys_dividers


/********************************************************************/

/********************************************************************/



/********************************************************************/

int pee_pbe(int crystal_val)
{
  short i;
  
// Check MCG is in PEE mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x3) && // check CLKS mux has selcted PLL output
      (!(MCG_S & MCG_S_IREFST_MASK)) &&                               // check FLL ref is external ref clk
      (MCG_S & MCG_S_PLLST_MASK)))                                    // check PLLS mux has selected PLL 
  {
    return 0x8;                                                       // return error code
  } 
  
// As we are running from the PLL by default the PLL and external clock settings are valid
// To move to PBE from PEE simply requires the switching of the CLKS mux to select the ext clock 
// As CLKS is already 0 the CLKS value can simply be OR'ed into the register 
  MCG_C1 |= MCG_C1_CLKS(2); // switch CLKS mux to select external reference clock as MCG_OUT
  
// Wait for clock status bits to update 
  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) break; // jump out early if CLKST shows EXT CLK slected before loop finishes
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2) return 0x1A; // check EXT CLK is really selected and return with error if not

// Now in PBE mode  
return crystal_val; // MCGOUT frequency equals external clock frequency
} // pee_pbe


int pbe_pee(int crystal_val, int pll_select)
{
  unsigned char prdiv, vdiv;
  short i;

  // Check MCG is in PBE mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
      (!(MCG_S & MCG_S_IREFST_MASK)) &&                               // check FLL ref is external ref clk
      (MCG_S & MCG_S_PLLST_MASK) &&                                   // check PLLS mux has selected PLL
      (!(MCG_C2 & MCG_C2_LP_MASK))))                                  // check MCG_C2[LP] bit is not set
  {
    return 0x7;                                                       // return error code
  }

  // As the PLL settings have already been checked when PBE mode was enterred they are not checked here

  // Check the selected PLL state before transitioning to PEE mode
  if (pll_select)
  {
    // Check LOCK bit is set before transitioning MCG to PLL output (already checked in fbe_pbe but good practice
    // to re-check before switch to use PLL)
    for (i = 0 ; i < 2000 ; i++)
    {
      if (MCG_S2 & MCG_S2_LOCK2_MASK) break; // jump out early if LOCK sets before loop finishes
    }
    if (!(MCG_S2 & MCG_S2_LOCK2_MASK)) return 0x44; // check bit is really set and return with error if not set
    // Use actual PLL settings to calculate PLL frequency
    prdiv = ((MCG_C11 & MCG_C11_PRDIV2_MASK) + 1);
    vdiv = ((MCG_C12 & MCG_C12_VDIV2_MASK) + 16);
  }
  else
  {
    // Check LOCK bit is set before transitioning MCG to PLL output (already checked in fbe_pbe but good practice
    // to re-check before switch to use PLL)
    for (i = 0 ; i < 2000 ; i++)
    {
      if (MCG_S & MCG_S_LOCK_MASK) break; // jump out early if LOCK sets before loop finishes
    }
    if (!(MCG_S & MCG_S_LOCK_MASK)) return 0x44; // check bit is really set and return with error if not set
    // Use actual PLL settings to calculate PLL frequency
    prdiv = ((MCG_C5 & MCG_C5_PRDIV_MASK) + 1);
    vdiv = ((MCG_C6 & MCG_C6_VDIV_MASK) + 16);
  }

  MCG_C1 &= ~MCG_C1_CLKS_MASK; // clear CLKS to switch CLKS mux to select PLL as MCG_OUT

  // Wait for clock status bits to update
  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x3) break; // jump out early if CLKST = 3 before loop finishes
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3) return 0x1B; // check CLKST is set correctly and return with error if not

  // Now in PEE
  return (((crystal_val / prdiv) * vdiv) / 2); //MCGOUT equals PLL output frequency/2
} // pbe_pee


int pbe_fbe(int crystal_val)
{
  short i;
  
// Check MCG is in PBE mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
      (!(MCG_S & MCG_S_IREFST_MASK)) &&                               // check FLL ref is external ref clk
      (MCG_S & MCG_S_PLLST_MASK) &&                                   // check PLLS mux has selected PLL
      (!(MCG_C2 & MCG_C2_LP_MASK))))                                  // check MCG_C2[LP] bit is not set   
  {
    return 0x7;                                                       // return error code
  }

// As we are running from the ext clock, by default the external clock settings are valid
// To move to FBE from PBE simply requires the switching of the PLLS mux to disable the PLL 
  
  MCG_C6 &= ~MCG_C6_PLLS_MASK; // clear PLLS to disable PLL, still clocked from ext ref clk
  
// wait for PLLST status bit to set
  for (i = 0 ; i < 2000 ; i++)
  {
    if (!(MCG_S & MCG_S_PLLST_MASK)) break; // jump out early if PLLST clears before loop finishes
  }
  if (MCG_S & MCG_S_PLLST_MASK) return 0x15; // check bit is really clear and return with error if not clear  

// Now in FBE mode  
  return crystal_val; // MCGOUT frequency equals external clock frequency 
} // pbe_fbe


/********************************************************************/
/* Functon name : fbe_pbe
 *
 * Mode transition: FBE to PBE mode
 *
 * This function transitions the MCG from FBE mode to PBE mode. 
 * This function presently only supports OSC0 and PLL0. Support for OSC1 and PLL1 will be added soon 
 * The function requires the desired OSC and PLL be passed in to it for compatibility with the
 * future support of OSC/PLL selection
 *
 * Parameters: osc_select  - Selects which OSC is to be used in PBE mode, 0 or 1
 *             crystal_val - external clock frequency in Hz
 *             pll_select  - 0 to select PLL0, non-zero to select PLL1.
 *             prdiv_val   - value to divide the external clock source by to create the desired
 *                           PLL reference clock frequency
 *             vdiv_val    - value to multiply the PLL reference clock frequency by
 *
 * Return value : MCGCLKOUT frequency (Hz) or error code
 */
int fbe_pbe(unsigned char osc_select, int crystal_val, unsigned char pll_select, signed char prdiv_val, signed char vdiv_val)
{
  unsigned char temp_reg;
  short i;
  int pll_freq;
  
// Check MCG is in FBE mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
      (!(MCG_S & MCG_S_IREFST_MASK)) &&                               // check FLL ref is external ref clk
      (!(MCG_S & MCG_S_PLLST_MASK)) &&                                // check PLLS mux has selected FLL
      (!(MCG_C2 & MCG_C2_LP_MASK))))                                  // check MCG_C2[LP] bit is not set   
  {
    return 0x4;                                                       // return error code
  }
  
// As the external frequency has already been checked when FBE mode was enterred it is not checked here

// Check PLL divider settings are within spec.
  if ((prdiv_val < 1) || (prdiv_val > 8)) {return 0x41;}
  if ((vdiv_val < 16) || (vdiv_val > 47)) {return 0x42;} 
  
// Check PLL reference clock frequency is within spec.
  if (((crystal_val / prdiv_val) < 8000000) || ((crystal_val / prdiv_val) > 32000000)) {return 0x43;}
       
// Check PLL output frequency is within spec.
  pll_freq = (crystal_val / prdiv_val) * vdiv_val;
  if ((pll_freq < 180000000) || (pll_freq > 360000000)) {return 0x45;}

  if (pll_select == 0)
  {
    MCG_C11 &= ~MCG_C11_PLLCS_MASK; // clear the PLLSC bit to select PLL0
  // Configure MCG_C5
  // If the PLL is to run in STOP mode then the PLLSTEN bit needs to be OR'ed in here or in user code.       
    temp_reg = MCG_C5;
    temp_reg &= ~MCG_C5_PRDIV_MASK;
    temp_reg |= MCG_C5_PRDIV(prdiv_val - 1);    //set PLL ref divider
    MCG_C5 = temp_reg;

  // Configure MCG_C6
  // The PLLS bit is set to enable the PLL, MCGOUT still sourced from ext ref clk 
  // The clock monitor is not enabled here as it has likely been enabled previously and so the value of CME
  // is not altered here.
  // The loss of lock interrupt can be enabled by seperately OR'ing in the LOLIE bit in MCG_C6
    temp_reg = MCG_C6; // store present C6 value
    temp_reg &= ~MCG_C6_VDIV_MASK; // clear VDIV settings
    temp_reg |= MCG_C6_PLLS_MASK | MCG_C6_VDIV(vdiv_val - 16); // write new VDIV and enable PLL
    MCG_C6 = temp_reg; // update MCG_C6
  
  // wait for PLLST status bit to set
    for (i = 0 ; i < 2000 ; i++)
    {
      if (MCG_S & MCG_S_PLLST_MASK) break; // jump out early if PLLST sets before loop finishes
    }
    if (!(MCG_S & MCG_S_PLLST_MASK)) return 0x16; // check bit is really set and return with error if not set

  // Wait for LOCK bit to set
    for (i = 0 ; i < 2000 ; i++)
    {
      if (MCG_S & MCG_S_LOCK_MASK) break; // jump out early if LOCK sets before loop finishes
    }
    if (!(MCG_S & MCG_S_LOCK_MASK)) return 0x44; // check bit is really set and return with error if not set
    
  // wait for PLLCST status bit to clear
    for (i = 0 ; i < 2000 ; i++)
    {
      if (!(MCG_S2 & MCG_S2_PLLCST_MASK)) break; // jump out early if PLLST sets before loop finishes
    }
    if (MCG_S2 & MCG_S2_PLLCST_MASK) return 0x17; // check bit is really set and return with error if not set  
  }
  else
  {
    MCG_C11 |= MCG_C11_PLLCS_MASK; // set the PLLSC bit to select PLL1
  // Configure MCG_C11
  // If the PLL is to run in STOP mode then the PLLSTEN bit needs to be OR'ed in here or in user code.       
    temp_reg = MCG_C11;
    temp_reg &= ~MCG_C11_PRDIV2_MASK;
    temp_reg |= MCG_C11_PRDIV2(prdiv_val - 1);    //set PLL ref divider
    MCG_C11 = temp_reg;

  // Configure MCG_C12
  // The PLLS bit is set to enable the PLL, MCGOUT still sourced from ext ref clk 
  // The clock monitor is not enabled here as it has likely been enabled previously and so the value of CME
  // is not altered here.
  // The loss of lock interrupt can be enabled by seperately OR'ing in the LOLIE bit in MCG_C12
    temp_reg = MCG_C12; // store present C6 value
    temp_reg &= ~MCG_C12_VDIV2_MASK; // clear VDIV settings
    temp_reg |= MCG_C12_VDIV2(vdiv_val - 16); // write new VDIV
    MCG_C12 = temp_reg; // update MCG_C12
    MCG_C6 |= MCG_C6_PLLS_MASK; // enable PLL by selecting the PLL with PLLS bit in MCG_C6
  
  // wait for PLLST status bit to set
    for (i = 0 ; i < 2000 ; i++)
    {
      if (MCG_S & MCG_S_PLLST_MASK) break; // jump out early if PLLST sets before loop finishes
    }
    if (!(MCG_S & MCG_S_PLLST_MASK)) return 0x16; // check bit is really set and return with error if not set

  // Wait for LOCK bit to set
    for (i = 0 ; i < 2000 ; i++)
    {
      if (MCG_S2 & MCG_S2_LOCK2_MASK) break; // jump out early if LOCK sets before loop finishes
    }
    if (!(MCG_S2 & MCG_S2_LOCK2_MASK)) return 0x44; // check bit is really set and return with error if not set  
    
  // wait for PLLCST status bit to set
    for (i = 0 ; i < 2000 ; i++)
    {
      if (MCG_S2 & MCG_S2_PLLCST_MASK) break; // jump out early if PLLST sets before loop finishes
    }
    if (!(MCG_S2 & MCG_S2_PLLCST_MASK)) return 0x17; // check bit is really set and return with error if not set  
  }
  
// now in PBE 
  return crystal_val; // MCGOUT frequency equals external clock frequency
} // fbe_pbe


int pbe_blpe(int crystal_val)
{
// Check MCG is in PBE mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
      (!(MCG_S & MCG_S_IREFST_MASK)) &&                               // check FLL ref is external ref clk
      (MCG_S & MCG_S_PLLST_MASK) &&                                   // check PLLS mux has selected PLL
      (!(MCG_C2 & MCG_C2_LP_MASK))))                                  // check MCG_C2[LP] bit is not set   
  {
    return 0x7;                                                       // return error code
  }
  
// To enter BLPE mode the LP bit must be set, disabling the PLL  
  MCG_C2 |= MCG_C2_LP_MASK;
  
// Now in BLPE mode
  return crystal_val;  
} // pbe_blpe


// ************************************************************************************************
// Since PBE mode can be enterred via FBE -> BLPE modes, it cannot be assumed that the PLL has been 
// previously configured correctly. That is why this general purpose driver has the PLL settings as
// passed parameters.
// ************************************************************************************************
int blpe_pbe(int crystal_val, signed char prdiv_val, signed char vdiv_val)
{
  unsigned char temp_reg;
  short i;
  
// Check MCG is in BLPE mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
      (!(MCG_S & MCG_S_IREFST_MASK)) &&                               // check FLL ref is external ref clk
      (MCG_C2 & MCG_C2_LP_MASK)))                                     // check MCG_C2[LP] bit is set   
  {
    return 0x6;                                                       // return error code
  }
  
// As the external frequency has already been checked when FBE mode was enterred it is not checked here

// Check PLL divider settings are within spec.
  if ((prdiv_val < 1) || (prdiv_val > 8)) {return 0x41;}
  if ((vdiv_val < 16) || (vdiv_val > 47)) {return 0x42;} 
  
// Check PLL reference clock frequency is within spec.
  if (((crystal_val / prdiv_val) < 8000000) || ((crystal_val / prdiv_val) > 32000000)) {return 0x43;}
       
// If PRDIV, VDIV and the PLL ref clock are in spec. then the PLL frequency is within spec.

// Configure MCG_C5
// If the PLL is to run in STOP mode then the PLLSTEN bit needs to be OR'ed in here or in user code.       
  temp_reg = MCG_C5;
  temp_reg &= ~MCG_C5_PRDIV_MASK;
  temp_reg |= MCG_C5_PRDIV(prdiv_val - 1);    //set PLL ref divider
  MCG_C5 = temp_reg;

// Configure MCG_C6
// The PLLS bit is set to enable the PLL, MCGOUT still sourced from ext ref clk 
// The clock monitor is not enabled here as it has likely been enabled previously and so the value of CME
// is not altered here.
// The loss of lock interrupt can be enabled by seperately OR'ing in the LOLIE bit in MCG_C6
  temp_reg = MCG_C6; // store present C6 value
  temp_reg &= ~MCG_C6_VDIV_MASK; // clear VDIV settings
  temp_reg |= MCG_C6_PLLS_MASK | MCG_C6_VDIV(vdiv_val - 16); // write new VDIV and enable PLL
  MCG_C6 = temp_reg; // update MCG_C6
  
// Now that PLL is configured, LP is cleared to enable the PLL
  MCG_C2 &= ~MCG_C2_LP_MASK;
  
// wait for PLLST status bit to set
  for (i = 0 ; i < 2000 ; i++)
  {
    if (MCG_S & MCG_S_PLLST_MASK) break; // jump out early if PLLST sets before loop finishes
  }
  if (!(MCG_S & MCG_S_PLLST_MASK)) return 0x16; // check bit is really set and return with error if not set

// Wait for LOCK bit to set
  for (i = 0 ; i < 2000 ; i++)
  {
    if (MCG_S & MCG_S_LOCK_MASK) break; // jump out early if LOCK sets before loop finishes
  }
  if (!(MCG_S & MCG_S_LOCK_MASK)) return 0x44; // check bit is really set and return with error if not set

// now in PBE 
  return crystal_val; // MCGOUT frequency equals external clock frequency  
} // blpe_pbe


int blpe_fbe(int crystal_val)
{
  short i;
  
// Check MCG is in BLPE mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
      (!(MCG_S & MCG_S_IREFST_MASK)) &&                               // check FLL ref is external ref clk
      (MCG_C2 & MCG_C2_LP_MASK)))                                     // check MCG_C2[LP] bit is set   
  {
    return 0x6;                                                       // return error code
  }
 
// To move from BLPE to FBE the PLLS mux be set to select the FLL output and the LP bit must be cleared
  MCG_C6 &= ~MCG_C6_PLLS_MASK; // clear PLLS to select the FLL
  MCG_C2 &= ~MCG_C2_LP_MASK; // clear LP bit  

// wait for PLLST status bit to set
  for (i = 0 ; i < 2000 ; i++)
  {
    if (!(MCG_S & MCG_S_PLLST_MASK)) break; // jump out early if PLLST clears before loop finishes
  }
  if (MCG_S & MCG_S_PLLST_MASK) return 0x15; // check bit is really clear and return with error if not clear  
  
// now in FBE mode
  return crystal_val; // MCGOUT frequency equals external clock frequency     
} // blpe_fbe


int fbe_blpe(int crystal_val)
{
// Check MCG is in FBE mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
      (!(MCG_S & MCG_S_IREFST_MASK)) &&                               // check FLL ref is external ref clk
      (!(MCG_S & MCG_S_PLLST_MASK)) &&                                // check PLLS mux has selected FLL
      (!(MCG_C2 & MCG_C2_LP_MASK))))                                  // check MCG_C2[LP] bit is not set   
  {
    return 0x4;                                                       // return error code
  }
 
// To move from FBE to BLPE the LP bit must be set
  MCG_C2 |= MCG_C2_LP_MASK; // set LP bit  
 
// now in FBE mode
  return crystal_val; // MCGOUT frequency equals external clock frequency     
} // fbe_blpe


int fbe_fei(int slow_irc_freq)
{
  unsigned char temp_reg;
  short i;
  int mcg_out;
  
// Check MCG is in FBE mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
      (!(MCG_S & MCG_S_IREFST_MASK)) &&                               // check FLL ref is external ref clk
      (!(MCG_S & MCG_S_PLLST_MASK)) &&                                // check PLLS mux has selected FLL
      (!(MCG_C2 & MCG_C2_LP_MASK))))                                  // check MCG_C2[LP] bit is not set   
  {
    return 0x4;                                                       // return error code
  }

// Check IRC frequency is within spec.
  if ((slow_irc_freq < 31250) || (slow_irc_freq > 39063))
  {
    return 0x31;
  }
  
// Check resulting FLL frequency 
  mcg_out = fll_freq(slow_irc_freq); 
  if (mcg_out < 0x5B) {return mcg_out;} // If error code returned, return the code to calling function

// Need to make sure the clockmonitor is disabled before moving to an "internal" clock mode
  MCG_C6 &= ~MCG_C6_CME_MASK; //This assumes OSC0 is used as the external clock source
  
// Move to FEI by setting CLKS to 0 and enabling the slow IRC as the FLL reference clock
  temp_reg = MCG_C1;
  temp_reg &= ~MCG_C1_CLKS_MASK; // clear CLKS to select FLL output
  temp_reg |= MCG_C1_IREFS_MASK; // select internal reference clock
  MCG_C1 = temp_reg; // update MCG_C1 
  
// wait for Reference clock Status bit to set
  for (i = 0 ; i < 2000 ; i++)
  {
    if (MCG_S & MCG_S_IREFST_MASK) break; // jump out early if IREFST sets before loop finishes
  }
  if (!(MCG_S & MCG_S_IREFST_MASK)) return 0x12; // check bit is really set and return with error if not set
  
// Wait for clock status bits to show clock source is ext ref clk
  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) break; // jump out early if CLKST shows EXT CLK slected before loop finishes
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x0) return 0x18; // check EXT CLK is really selected and return with error if not

// Now in FEI mode
  return mcg_out;
} // fbe_fei


/********************************************************************/
/* Functon name : fei_fbe
 *
 * Mode transition: FEI to FBE mode
 *
 * This function transitions the MCG from FEI mode to FBE mode. This is
 * achieved by setting the MCG_C2[LP] bit. There is no status bit to 
 * check so 0 is always returned if the function was called with the MCG
 * in FBI mode. The MCGCLKOUT frequency does not change
 *
 * Parameters: crystal_val - external clock frequency in Hz
 *             hgo_val     - selects whether low power or high gain mode is selected
 *                           for the crystal oscillator. This has no meaning if an 
 *                           external clock is used.
 *             erefs_val   - selects external clock (=0) or crystal osc (=1)
 *
 * Return value : MCGCLKOUT frequency (Hz) or error code
 */
int fei_fbe(int crystal_val, unsigned char hgo_val, unsigned char erefs_val)
{
  unsigned char frdiv_val;
  unsigned char temp_reg;
  short i;
  
// check if in FEI mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) && // check CLKS mux has selcted FLL output
      (MCG_S & MCG_S_IREFST_MASK) &&                                  // check FLL ref is internal ref clk
      (!(MCG_S & MCG_S_PLLST_MASK))))                                 // check PLLS mux has selected FLL
  {
    return 0x1;                                                     // return error code
  }

// check external frequency is less than the maximum frequency
  if  (crystal_val > 60000000) {return 0x21;}
  
// check crystal frequency is within spec. if crystal osc is being used
  if (erefs_val)
  {
    if ((crystal_val < 30000) ||
        ((crystal_val > 40000) && (crystal_val < 3000000)) ||
        (crystal_val > 32000000)) {return 0x22;} // return error if one of the available crystal options is not available
  }

// make sure HGO will never be greater than 1. Could return an error instead if desired.  
  if (hgo_val > 0)
  {
    hgo_val = 1; // force hgo_val to 1 if > 0
  }

// configure the MCG_C2 register
// the RANGE value is determined by the external frequency. Since the RANGE parameter affects the FRDIV divide value
// it still needs to be set correctly even if the oscillator is not being used
  temp_reg = MCG_C2;
  temp_reg &= ~(MCG_C2_RANGE_MASK | MCG_C2_HGO_MASK | MCG_C2_EREFS_MASK); // clear fields before writing new values
  if (crystal_val <= 40000)
  {
    temp_reg |= (MCG_C2_RANGE(0) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT));
  }
  else if (crystal_val <= 8000000)
  {
    temp_reg |= (MCG_C2_RANGE(1) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT));
  }
  else
  {
    temp_reg |= (MCG_C2_RANGE(2) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT));
  }
  MCG_C2 = temp_reg;
// determine FRDIV based on reference clock frequency
// since the external frequency has already been checked only the maximum frequency for each FRDIV value needs to be compared here.
  if (crystal_val <= 1250000) {frdiv_val = 0;}
  else if (crystal_val <= 2500000) {frdiv_val = 1;}
  else if (crystal_val <= 5000000) {frdiv_val = 2;}
  else if (crystal_val <= 10000000) {frdiv_val = 3;}
  else if (crystal_val <= 20000000) {frdiv_val = 4;}
  else {frdiv_val = 5;}
  
// Select external oscilator and Reference Divider and clear IREFS to start ext osc
// If IRCLK is required it must be enabled outside of this driver, existing state will be maintained
// CLKS=2, FRDIV=frdiv_val, IREFS=0, IRCLKEN=0, IREFSTEN=0
  temp_reg = MCG_C1;
  temp_reg &= ~(MCG_C1_CLKS_MASK | MCG_C1_FRDIV_MASK | MCG_C1_IREFS_MASK); // Clear values in these fields
  temp_reg = MCG_C1_CLKS(2) | MCG_C1_FRDIV(frdiv_val); // Set the required CLKS and FRDIV values
  MCG_C1 = temp_reg;

// if the external oscillator is used need to wait for OSCINIT to set
  if (erefs_val)
  {
    for (i = 0 ; i < 10000 ; i++)
    {
      if (MCG_S & MCG_S_OSCINIT_MASK) break; // jump out early if OSCINIT sets before loop finishes
    }
    if (!(MCG_S & MCG_S_OSCINIT_MASK)) return 0x23; // check bit is really set and return with error if not set
  }

// wait for Reference clock Status bit to clear
  for (i = 0 ; i < 2000 ; i++)
  {
    if (!(MCG_S & MCG_S_IREFST_MASK)) break; // jump out early if IREFST clears before loop finishes
  }
  if (MCG_S & MCG_S_IREFST_MASK) return 0x11; // check bit is really clear and return with error if not set
  
// Wait for clock status bits to show clock source is ext ref clk
  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) break; // jump out early if CLKST shows EXT CLK slected before loop finishes
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2) return 0x1A; // check EXT CLK is really selected and return with error if not
 
// Now in FBE  
// It is recommended that the clock monitor is enabled when using an external clock as the clock source/reference.
// It is enabled here but can be removed if this is not required.
  MCG_C6 |= MCG_C6_CME_MASK;
  
  return crystal_val; // MCGOUT frequency equals external clock frequency
} // fei_fbe


int fbe_fee(int crystal_val)
{
  short i, fll_ref_freq;
  int mcg_out;

// Check MCG is in FBE mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
      (!(MCG_S & MCG_S_IREFST_MASK)) &&                               // check FLL ref is external ref clk
      (!(MCG_S & MCG_S_PLLST_MASK)) &&                                // check PLLS mux has selected FLL
      (!(MCG_C2 & MCG_C2_LP_MASK))))                                  // check MCG_C2[LP] bit is not set   
  {
    return 0x4;                                                       // return error code
  }
  
  // The FLL ref clk divide value depends on FRDIV and the RANGE value
  if (((MCG_C2 & MCG_C2_RANGE_MASK) >> MCG_C2_RANGE_SHIFT) > 0)
  {
    fll_ref_freq = (crystal_val / (32 << ((MCG_C1 & MCG_C1_FRDIV_MASK) >> MCG_C1_FRDIV_SHIFT)));
  }
  else
  {
    fll_ref_freq = ((crystal_val) / (1 << (((MCG_C2 & MCG_C2_RANGE_MASK) >> MCG_C2_RANGE_SHIFT))));
  }
  
// Check resulting FLL frequency 
  mcg_out = fll_freq(fll_ref_freq); // FLL reference frequency calculated from ext ref freq and FRDIV
  if (mcg_out < 0x5B) {return mcg_out;} // If error code returned, return the code to calling function
  
// Clear CLKS field to switch CLKS mux to select FLL output
  MCG_C1 &= ~MCG_C1_CLKS_MASK; // clear CLKS to select FLL output

// Wait for clock status bits to show clock source is FLL
  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) break; // jump out early if CLKST shows FLL selected before loop finishes
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x0) return 0x18; // check FLL is really selected and return with error if not
  
// Now in FEE mode
  return mcg_out;
} // fbe_fee


int fee_fbe(int crystal_val)
{ 
  short i;
  
// Check MCG is in FEE mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) && // check CLKS mux has selcted FLL
      (!(MCG_S & MCG_S_IREFST_MASK)) &&                               // check FLL ref is external ref clk
      (!(MCG_S & MCG_S_PLLST_MASK))))                                 // check PLLS mux has selected FLL
  {
    return 0x2;                                                       // return error code
  }
  
// Set CLKS field to 2 to switch CLKS mux to select ext ref clock
// MCG is current in FEE mode so CLKS field = 0 so can just OR in new value
  MCG_C1 |= MCG_C1_CLKS(2); // set CLKS to select ext ref clock

/// Wait for clock status bits to show clock source is ext ref clk
  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) break; // jump out early if CLKST shows EXT CLK slected before loop finishes
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2) return 0x1A; // check EXT CLK is really selected and return with error if not
  
// Now in FBE mode
  return crystal_val;
} // fee_fbe


int fbe_fbi(int irc_freq, unsigned char irc_select)
{
  unsigned char temp_reg;
  unsigned char fcrdiv_val;
  short i;
  
// Check MCG is in FBE mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
      (!(MCG_S & MCG_S_IREFST_MASK)) &&                               // check FLL ref is external ref clk
      (!(MCG_S & MCG_S_PLLST_MASK)) &&                                // check PLLS mux has selected FLL
      (!(MCG_C2 & MCG_C2_LP_MASK))))                                  // check MCG_C2[LP] bit is not set   
  {
    return 0x4;                                                       // return error code
  }

// Check that the irc frequency matches the selected IRC 
  if (!(irc_select))
  {    
    if ((irc_freq < 31250) || (irc_freq > 39063)) {return 0x31;}
  }
  else
  {
    if ((irc_freq < 3000000) || (irc_freq > 5000000)) {return 0x32;} // Fast IRC freq
  }
  
// Select the required IRC
  if (irc_select)
  {
    MCG_C2 |= MCG_C2_IRCS_MASK; // select fast IRC by setting IRCS
  }
  else
  {
    MCG_C2 &= ~MCG_C2_IRCS_MASK; // select slow IRC by clearing IRCS
  }
  
// Make sure the clock monitor is disabled before switching modes otherwise it will trigger
  MCG_C6 &= ~MCG_C6_CME_MASK;
  
// Select the IRC as the CLKS mux selection
  temp_reg = MCG_C1;
  temp_reg &= ~MCG_C1_CLKS_MASK;
  temp_reg |= MCG_C1_CLKS(1) | MCG_C1_IREFS_MASK; // select IRC as MCGOUT and enable IREFS
  MCG_C1 = temp_reg; // update MCG_C1
  
// wait until internal reference switches to requested irc.
  if (!(irc_select))
  {
    for (i = 0 ; i < 2000 ; i++)
    {
      if (!(MCG_S & MCG_S_IRCST_MASK)) break; // jump out early if IRCST clears before loop finishes
    }
    if (MCG_S & MCG_S_IRCST_MASK) return 0x13; // check bit is really clear and return with error if set
  }
  else
  {
    for (i = 0 ; i < 2000 ; i++)
    {
      if (MCG_S & MCG_S_IRCST_MASK) break; // jump out early if IRCST sets before loop finishes
    }
    if (!(MCG_S & MCG_S_IRCST_MASK)) return 0x14; // check bit is really set and return with error if not set
  }
 
// Wait for clock status bits to update
  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1) break; // jump out early if CLKST shows IRC slected before loop finishes
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x1) return 0x19; // check IRC is really selected and return with error if not
 
//GPIOC_PSOR = 0x00020000; //set bit 17 of port C  
  // wait for Reference clock Status bit to set
  for (i = 0 ; i < 2000 ; i++)
  {
    if (MCG_S & MCG_S_IREFST_MASK) break; // jump out early if IREFST sets before loop finishes
  }
  if (!(MCG_S & MCG_S_IREFST_MASK)) return 0x12; // check bit is really set and return with error if not set
//GPIOC_PCOR = 0x00020000; // clear bit 17 of port C   
// Now in FBI mode
  
  if (irc_select)
  {
    fcrdiv_val = (1 << ((MCG_SC & MCG_SC_FCIRDIV_MASK) >> MCG_SC_FCIRDIV_SHIFT)); // calculate the fast IRC divder factor
    return (irc_freq / fcrdiv_val); // MCGOUT frequency equals fast IRC frequency divided by FCRDIV factor
  }
  else
  {
    return irc_freq; // MCGOUT frequency equals slow IRC frequency
  }
} //fbe_fbi


int fbi_fbe(int crystal_val, unsigned char hgo_val, unsigned char erefs_val)
{
  unsigned char temp_reg;
  unsigned char frdiv_val;
  short i;
  
// check if in FBI mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1) && // check CLKS mux has selcted int ref clk
      (MCG_S & MCG_S_IREFST_MASK) &&                                  // check FLL ref is internal ref clk
      (!(MCG_S & MCG_S_PLLST_MASK)) &&                                // check PLLS mux has selected FLL
      (!(MCG_C2 & MCG_C2_LP_MASK))))                                  // check LP bit is clear
  {  
    return 0x3;                                                       // MCG not in correct mode return fail code 
  }
 
// check external frequency is less than the maximum frequency
  if  (crystal_val > 60000000) {return 0x21;}
  
// check crystal frequency is within spec. if crystal osc is being used
  if (erefs_val)
  {
    if ((crystal_val < 30000) ||
        ((crystal_val > 40000) && (crystal_val < 3000000)) ||
        (crystal_val > 32000000)) {return 0x22;} // return error if one of the available crystal options is not available
  }

// make sure HGO will never be greater than 1. Could return an error instead if desired.  
  if (hgo_val > 0)
  {
    hgo_val = 1; // force hgo_val to 1 if > 0
  }

// configure the MCG_C2 register
// the RANGE value is determined by the external frequency. Since the RANGE parameter affects the FRDIV divide value
// it still needs to be set correctly even if the oscillator is not being used
  temp_reg = MCG_C2;
  temp_reg &= ~(MCG_C2_RANGE_MASK | MCG_C2_HGO_MASK | MCG_C2_EREFS_MASK); // clear fields before writing new values
  if (crystal_val <= 40000)
  {
    temp_reg |= (MCG_C2_RANGE(0) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT));
  }
  else if (crystal_val <= 8000000)
  {
    temp_reg |= (MCG_C2_RANGE(1) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT));
  }
  else
  {
    temp_reg |= (MCG_C2_RANGE(2) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT));
  }
  MCG_C2 = temp_reg;

// determine FRDIV based on reference clock frequency
// since the external frequency has already been checked only the maximum frequency for each FRDIV value needs to be compared here.
  if (crystal_val <= 1250000) {frdiv_val = 0;}
  else if (crystal_val <= 2500000) {frdiv_val = 1;}
  else if (crystal_val <= 5000000) {frdiv_val = 2;}
  else if (crystal_val <= 10000000) {frdiv_val = 3;}
  else if (crystal_val <= 20000000) {frdiv_val = 4;}
  else {frdiv_val = 5;}
  
// Select external oscilator and Reference Divider and clear IREFS to start ext osc
// If IRCLK is required it must be enabled outside of this driver, existing state will be maintained
// CLKS=2, FRDIV=frdiv_val, IREFS=0, IRCLKEN=0, IREFSTEN=0
  temp_reg = MCG_C1;
  temp_reg &= ~(MCG_C1_CLKS_MASK | MCG_C1_FRDIV_MASK | MCG_C1_IREFS_MASK); // Clear values in these fields
  temp_reg = MCG_C1_CLKS(2) | MCG_C1_FRDIV(frdiv_val); // Set the required CLKS and FRDIV values
  MCG_C1 = temp_reg;

// if the external oscillator is used need to wait for OSCINIT to set
  if (erefs_val)
  {
    for (i = 0 ; i < 10000 ; i++)
    {
      if (MCG_S & MCG_S_OSCINIT_MASK) break; // jump out early if OSCINIT sets before loop finishes
    }
    if (!(MCG_S & MCG_S_OSCINIT_MASK)) return 0x23; // check bit is really set and return with error if not set
  }

// wait for Reference clock Status bit to clear
  for (i = 0 ; i < 2000 ; i++)
  {
    if (!(MCG_S & MCG_S_IREFST_MASK)) break; // jump out early if IREFST clears before loop finishes
  }
  if (MCG_S & MCG_S_IREFST_MASK) return 0x11; // check bit is really clear and return with error if not set
  
// Wait for clock status bits to show clock source is ext ref clk
  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) break; // jump out early if CLKST shows EXT CLK slected before loop finishes
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2) return 0x1A; // check EXT CLK is really selected and return with error if not
 
// Now in FBE  
// It is recommended that the clock monitor is enabled when using an external clock as the clock source/reference.
// It is enabled here but can be removed if this is not required.
  MCG_C6 |= MCG_C6_CME_MASK;
  
  return crystal_val; // MCGOUT frequency equals external clock frequency  
} // fbi_fbe


/********************************************************************/
/* Functon name : fbi_blpi
 *
 * Mode transition: FBI to BLPI mode
 *
 * This function transitions the MCG from FBI mode to BLPI mode. This is
 * achieved by setting the MCG_C2[LP] bit. There is no status bit to 
 * check so 0 is always returned if the function was called with the MCG
 * in FBI mode. 
 *
 * Parameters: irc_freq - internal reference clock frequency
 *             ircs_select - 0 if slow irc, 1 if fast irc
 *
 * Return value : MCGOUT frequency or error code 0x13
 */
int fbi_blpi(int irc_freq, unsigned char irc_select)
{
  unsigned char fcrdiv_val;
  
// check if in FBI mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1) && // check CLKS mux has selcted int ref clk
      (MCG_S & MCG_S_IREFST_MASK) &&                                  // check FLL ref is internal ref clk
      (!(MCG_S & MCG_S_PLLST_MASK)) &&                                // check PLLS mux has selected FLL
      (!(MCG_C2 & MCG_C2_LP_MASK))))                                  // check LP bit is clear
  {  
    return 0x3;                                                       // MCG not in correct mode return fail code 
  }

// Set LP bit to disable the FLL and enter BLPI
  MCG_C2 |= MCG_C2_LP_MASK;
  
// Now in BLPI
  if (irc_select)
  {
    fcrdiv_val = (1 << ((MCG_SC & MCG_SC_FCIRDIV_MASK) >> MCG_SC_FCIRDIV_SHIFT)); // calculate the fast IRC divder factor
    return (irc_freq / fcrdiv_val); // MCGOUT frequency equals fast IRC frequency divided by 2
  }
  else
  {
    return irc_freq; // MCGOUT frequency equals slow IRC frequency
  }   
} // fbi_blpi



/********************************************************************/
/* Functon name : blpi_fbi
 *
 * Mode transition: BLPI to FBI mode
 *
 * This function transitions the MCG from BLPI mode to FBI mode. This is
 * achieved by clearing the MCG_C2[LP] bit. There is no status bit to 
 * check so 0 is always returned if the function was called with the MCG
 * in BLPI mode. 
 *
 * Parameters: irc_freq - internal reference clock frequency
 *             ircs_select - 0 if slow irc, 1 if fast irc
 *
 * Return value : MCGOUT frequency or error code 0x15
 */
int blpi_fbi(int irc_freq, unsigned char irc_select)
{
  unsigned char fcrdiv_val;
  // check if in BLPI mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1) && // check CLKS mux has selcted int ref clk
      (MCG_S & MCG_S_IREFST_MASK) &&                                  // check FLL ref is internal ref clk
      (!(MCG_S & MCG_S_PLLST_MASK)) &&                                // check PLLS mux has selected FLL
      (MCG_C2 & MCG_C2_LP_MASK)))                                     // check LP bit is set
  {
    return 0x5;                                                       // MCG not in correct mode return fail code
  }

// Clear LP bit to enable the FLL and enter FBI mode   
  MCG_C2 &= ~MCG_C2_LP_MASK;
  
// Now in FBI mode
  if (irc_select)
  {
    fcrdiv_val = (1 << ((MCG_SC & MCG_SC_FCIRDIV_MASK) >> MCG_SC_FCIRDIV_SHIFT)); // calculate the fast IRC divder factor
    return (irc_freq / fcrdiv_val); // MCGOUT frequency equals fast IRC frequency divided by 2
  }
  else
  {
    return irc_freq; // MCGOUT frequency equals slow IRC frequency
  }
} // blpi_fbi


int fbi_fee(int crystal_val, unsigned char hgo_val, unsigned char erefs_val)
{
  unsigned char temp_reg;
  unsigned char frdiv_val;
  short i;
  int mcg_out, fll_ref_freq;

// check if in FBI mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1) && // check CLKS mux has selcted int ref clk
      (MCG_S & MCG_S_IREFST_MASK) &&                                  // check FLL ref is internal ref clk
      (!(MCG_S & MCG_S_PLLST_MASK)) &&                                // check PLLS mux has selected FLL
      (!(MCG_C2 & MCG_C2_LP_MASK))))                                  // check LP bit is clear
  {  
    return 0x3;                                                       // MCG not in correct mode return fail code 
  }
  
// check external frequency is less than the maximum frequency
  if  (crystal_val > 60000000) {return 0x21;}
  
// check crystal frequency is within spec. if crystal osc is being used
  if (erefs_val)
  {
    if ((crystal_val < 30000) ||
        ((crystal_val > 40000) && (crystal_val < 3000000)) ||
        (crystal_val > 32000000)) {return 0x22;} // return error if one of the available crystal options is not available
  }

// make sure HGO will never be greater than 1. Could return an error instead if desired.  
  if (hgo_val > 0)
  {
    hgo_val = 1; // force hgo_val to 1 if > 0
  }

// configure the MCG_C2 register
// the RANGE value is determined by the external frequency. Since the RANGE parameter affects the FRDIV divide value
// it still needs to be set correctly even if the oscillator is not being used
  temp_reg = MCG_C2;
  temp_reg &= ~(MCG_C2_RANGE_MASK | MCG_C2_HGO_MASK | MCG_C2_EREFS_MASK); // clear fields before writing new values
  if (crystal_val <= 40000)
  {
    temp_reg |= (MCG_C2_RANGE(0) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT));
  }
  else if (crystal_val <= 8000000)
  {
    temp_reg |= (MCG_C2_RANGE(1) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT));
  }
  else
  {
    temp_reg |= (MCG_C2_RANGE(2) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT));
  }
  MCG_C2 = temp_reg;

// determine FRDIV based on reference clock frequency
// since the external frequency has already been checked only the maximum frequency for each FRDIV value needs to be compared here.
  if (crystal_val <= 1250000) {frdiv_val = 0;}
  else if (crystal_val <= 2500000) {frdiv_val = 1;}
  else if (crystal_val <= 5000000) {frdiv_val = 2;}
  else if (crystal_val <= 10000000) {frdiv_val = 3;}
  else if (crystal_val <= 20000000) {frdiv_val = 4;}
  else {frdiv_val = 5;}
// The FLL ref clk divide value depends on FRDIV and the RANGE value
  if (((MCG_C2 & MCG_C2_RANGE_MASK) >> MCG_C2_RANGE_SHIFT) > 0)
  {
    fll_ref_freq = ((crystal_val) / (32 << frdiv_val));
  }
  else
  {
    fll_ref_freq = ((crystal_val) / (1 << frdiv_val));
  }
  
// Check resulting FLL frequency 
  mcg_out = fll_freq(fll_ref_freq); // FLL reference frequency calculated from ext ref freq and FRDIV
  if (mcg_out < 0x5B) {return mcg_out;} // If error code returned, return the code to calling function
  
// Select external oscilator and Reference Divider and clear IREFS to start ext osc
// If IRCLK is required it must be enabled outside of this driver, existing state will be maintained
// CLKS=0, FRDIV=frdiv_val, IREFS=0, IRCLKEN=?, IREFSTEN=?
  temp_reg = MCG_C1;
  temp_reg &= ~(MCG_C1_CLKS_MASK | MCG_C1_FRDIV_MASK | MCG_C1_IREFS_MASK); // Clear CLKS, FRDIV and IREFS fields
  temp_reg = MCG_C1_CLKS(0) | MCG_C1_FRDIV(frdiv_val); // Set the required CLKS and FRDIV values
  MCG_C1 = temp_reg;

// if the external oscillator is used need to wait for OSCINIT to set
  if (erefs_val)
  {
    for (i = 0 ; i < 10000 ; i++)
    {
      if (MCG_S & MCG_S_OSCINIT_MASK) break; // jump out early if OSCINIT sets before loop finishes
    }
    if (!(MCG_S & MCG_S_OSCINIT_MASK)) return 0x23; // check bit is really set and return with error if not set
  }

// wait for Reference clock Status bit to clear
  for (i = 0 ; i < 2000 ; i++)
  {
    if (!(MCG_S & MCG_S_IREFST_MASK)) break; // jump out early if IREFST clears before loop finishes
  }
  if (MCG_S & MCG_S_IREFST_MASK) return 0x11; // check bit is really clear and return with error if not set
  
// Wait for clock status bits to show clock source is ext ref clk
  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) break; // jump out early if CLKST shows FLL selected before loop finishes
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x0) return 0x18; // check FLLK is really selected and return with error if not
 
// Now in FEE  
// It is recommended that the clock monitor is enabled when using an external clock as the clock source/reference.
// It is enabled here but can be removed if this is not required.
// The clock monitor MUST be disabled when returning to a non-external clock mode (FEI, FBI and BLPI)
  MCG_C6 |= MCG_C6_CME_MASK;
  
  return mcg_out; // MCGOUT frequency equals FLL frequency
} //fbi_fee


int fee_fbi(int irc_freq, unsigned char irc_select)
{ 
  unsigned char fcrdiv_val;
  short i;
  
// Check MCG is in FEE mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) && // check CLKS mux has selcted FLL output
      (!(MCG_S & MCG_S_IREFST_MASK)) &&                             // check FLL ref is external ref clk
      (!(MCG_S & MCG_S_PLLST_MASK))))                               // check PLLS mux has selected FLL
  {
    return 0x2;                                                     // return error code
  }
  
  // Check that the irc frequency matches the selected IRC 
  if (!(irc_select))
  {    
    if ((irc_freq < 31250) || (irc_freq > 39063)) {return 0x31;}
  }
  else
  {
    if ((irc_freq < 3000000) || (irc_freq > 5000000)) {return 0x32;} // Fast IRC freq
  }
  
// Select the required IRC
  if (irc_select)
  {
    MCG_C2 |= MCG_C2_IRCS_MASK; // select fast IRC by setting IRCS
  }
  else
  {
    MCG_C2 &= ~MCG_C2_IRCS_MASK; // select slow IRC by clearing IRCS
  }
  
// Make sure the clock monitor is disabled before switching modes otherwise it will trigger
  MCG_C6 &= ~MCG_C6_CME_MASK;
  
// Select the IRC as the CLKS mux selection
  MCG_C1 |= MCG_C1_CLKS(1) | MCG_C1_IREFS_MASK; // set IREFS and select IRC as MCGOUT
 
// wait until internal reference switches to requested irc.
  if (!(irc_select))
  {
    for (i = 0 ; i < 2000 ; i++)
    {
      if (!(MCG_S & MCG_S_IRCST_MASK)) break; // jump out early if IRCST clears before loop finishes
    }
    if (MCG_S & MCG_S_IRCST_MASK) return 0x13; // check bit is really clear and return with error if set
  }
  else
  {
    for (i = 0 ; i < 2000 ; i++)
    {
      if (MCG_S & MCG_S_IRCST_MASK) break; // jump out early if IRCST sets before loop finishes
    }
    if (!(MCG_S & MCG_S_IRCST_MASK)) return 0x14; // check bit is really set and return with error if not set
  }
  
// Wait for clock status bits to update
  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1) break; // jump out early if CLKST shows IRC slected before loop finishes
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x1) return 0x19; // check IRC is really selected and return with error if not
  
// wait for Reference clock Status bit to set
  for (i = 0 ; i < 2000 ; i++)
  {
    if (MCG_S & MCG_S_IREFST_MASK) break; // jump out early if IREFST sets before loop finishes
  }
  if (!(MCG_S & MCG_S_IREFST_MASK)) return 0x12; // check bit is really set and return with error if not set  
  
// Now in FBI mode
  if (irc_select)
  {
    fcrdiv_val = (1 << ((MCG_SC & MCG_SC_FCIRDIV_MASK) >> MCG_SC_FCIRDIV_SHIFT)); // calculate the fast IRC divder factor
    return (irc_freq / fcrdiv_val); // MCGOUT frequency equals fast IRC frequency divided by 2
  }
  else
  {
    return irc_freq; // MCGOUT frequency equals slow IRC frequency
  }
} // fee_fbi 


int fbi_fei(int slow_irc_freq)
{
  unsigned char temp_reg;
  short i;
  int mcg_out;

// check if in FBI mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1) && // check CLKS mux has selcted int ref clk
      (MCG_S & MCG_S_IREFST_MASK) &&                                  // check FLL ref is internal ref clk
      (!(MCG_S & MCG_S_PLLST_MASK)) &&                                // check PLLS mux has selected FLL
      (!(MCG_C2 & MCG_C2_LP_MASK))))                                  // check LP bit is clear
  {  
    return 0x3;                                                       // MCG not in correct mode return fail code 
  }

// Check IRC frequency is within spec.
  if ((slow_irc_freq < 31250) || (slow_irc_freq > 39063))
  {
    return 0x31;
  }

// Check resulting FLL frequency 
  mcg_out = fll_freq(slow_irc_freq); 
  if (mcg_out < 0x5B) {return mcg_out;} // If error code returned, return the code to calling function
  
// Change the CLKS mux to select the FLL output as MCGOUT  
  temp_reg = MCG_C1;
  temp_reg &= ~MCG_C1_CLKS_MASK; // clear CLKS field
  temp_reg |= MCG_C1_CLKS(0); // select FLL as MCGOUT
  temp_reg |= MCG_C1_IREFS_MASK; // make sure IRC is FLL reference
  MCG_C1 = temp_reg; // update MCG_C1
  
// wait for Reference clock Status bit to clear
  for (i = 0 ; i < 2000 ; i++)
  {
    if (MCG_S & MCG_S_IREFST_MASK) break; // jump out early if IREFST clears before loop finishes
  }
  if (!(MCG_S & MCG_S_IREFST_MASK)) return 0x12; // check bit is really set and return with error if not set
  
// Wait for clock status bits to show clock source is ext ref clk
  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) break; // jump out early if CLKST shows FLL slected before loop finishes
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x0) return 0x18; // check FLL is really selected and return with error if not

// Now in FEI mode
  return mcg_out;  
} // fbi_fei


int fei_fbi(int irc_freq, unsigned char irc_select)
{
  unsigned char temp_reg;
  unsigned char fcrdiv_val;
  short i;
  
// Check MCG is in FEI mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) && // check CLKS mux has selcted FLL output
      (MCG_S & MCG_S_IREFST_MASK) &&                                  // check FLL ref is internal ref clk
      (!(MCG_S & MCG_S_PLLST_MASK))))                                 // check PLLS mux has selected FLL
  {
    return 0x1;                                                       // return error code
  } 

// Check that the irc frequency matches the selected IRC 
  if (!(irc_select))
  {    
    if ((irc_freq < 31250) || (irc_freq > 39063)) {return 0x31;}
  }
  else
  {
    if ((irc_freq < 3000000) || (irc_freq > 5000000)) {return 0x32;} // Fast IRC freq
  }
  
// Select the desired IRC
  if (irc_select)
  {
    MCG_C2 |= MCG_C2_IRCS_MASK; // select fast IRCS
  }
  else
  {
    MCG_C2 &= ~MCG_C2_IRCS_MASK; // select slow IRCS
  }
  
// Change the CLKS mux to select the IRC as the MCGOUT
  temp_reg = MCG_C1;
  temp_reg &= ~MCG_C1_CLKS_MASK; // clear CLKS
  temp_reg |= MCG_C1_CLKS(1); // select IRC as the MCG clock sourse
  MCG_C1 = temp_reg;

// wait until internal reference switches to requested irc.
  if (!(irc_select))
  {
    for (i = 0 ; i < 2000 ; i++)
    {
      if (!(MCG_S & MCG_S_IRCST_MASK)) break; // jump out early if IRCST clears before loop finishes
    }
    if (MCG_S & MCG_S_IRCST_MASK) return 0x13; // check bit is really clear and return with error if set
  }
  else
  {
    for (i = 0 ; i < 2000 ; i++)
    {
      if (MCG_S & MCG_S_IRCST_MASK) break; // jump out early if IRCST sets before loop finishes
    }
    if (!(MCG_S & MCG_S_IRCST_MASK)) return 0x14; // check bit is really set and return with error if not set
  }
  
// Wait for clock status bits to update
  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1) break; // jump out early if CLKST shows IRC slected before loop finishes
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x1) return 0x19; // check IRC is really selected and return with error if not
  
// Now in FBI mode
  if (irc_select)
  {
    fcrdiv_val = (1 << ((MCG_SC & MCG_SC_FCIRDIV_MASK) >> MCG_SC_FCIRDIV_SHIFT)); // calculate the fast IRC divder factor
    return (irc_freq / fcrdiv_val); // MCGOUT frequency equals fast IRC frequency divided by 2
  }
  else
  {
    return irc_freq; // MCGOUT frequency equals slow IRC frequency
  }   
} // fei_fbi


/********************************************************************/
/* Functon name : fei_fee
 *
 * Mode transition: FEI to FEE mode
 *
 * This function transitions the MCG from FEI mode to FEE mode. This is
 * achieved by setting the MCG_C2[LP] bit. There is no status bit to 
 * check so 0 is always returned if the function was called with the MCG
 * in FBI mode. The MCGCLKOUT frequency does not change
 *
 * Parameters: crystal_val - external clock frequency in Hz
 *             hgo_val     - selects whether low power or high gain mode is selected
 *                           for the crystal oscillator. This has no meaning if an 
 *                           external clock is used.
 *             erefs_val   - selects external clock (=0) or crystal osc (=1)
 *
 * Return value : MCGCLKOUT frequency (Hz) or error code
 */
int fei_fee(int crystal_val, unsigned char hgo_val, unsigned char erefs_val)
{
  unsigned char frdiv_val;
  unsigned char temp_reg;
 // short i;
  int mcg_out, fll_ref_freq, i;
  
// check if in FEI mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) && // check CLKS mux has selcted FLL output
      (MCG_S & MCG_S_IREFST_MASK) &&                                  // check FLL ref is internal ref clk
      (!(MCG_S & MCG_S_PLLST_MASK))))                                 // check PLLS mux has selected FLL
  {
    return 0x1;                                                     // return error code
  }

// check external frequency is less than the maximum frequency
  if  (crystal_val > 60000000) {return 0x21;}
  
// check crystal frequency is within spec. if crystal osc is being used
  if (erefs_val)
  {
    if ((crystal_val < 30000) ||
        ((crystal_val > 40000) && (crystal_val < 3000000)) ||
        (crystal_val > 32000000)) {return 0x22;} // return error if one of the available crystal options is not available
  }

// make sure HGO will never be greater than 1. Could return an error instead if desired.  
  if (hgo_val > 0)
  {
    hgo_val = 1; // force hgo_val to 1 if > 0
  }

// configure the MCG_C2 register
// the RANGE value is determined by the external frequency. Since the RANGE parameter affects the FRDIV divide value
// it still needs to be set correctly even if the oscillator is not being used
  temp_reg = MCG_C2;
  temp_reg &= ~(MCG_C2_RANGE_MASK | MCG_C2_HGO_MASK | MCG_C2_EREFS_MASK); // clear fields before writing new values
  if (crystal_val <= 40000)
  {
    temp_reg |= (MCG_C2_RANGE(0) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT));
  }
  else if (crystal_val <= 8000000)
  {
    temp_reg |= (MCG_C2_RANGE(1) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT));
  }
  else
  {
    temp_reg |= (MCG_C2_RANGE(2) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT));
  }
  MCG_C2 = temp_reg;

// determine FRDIV based on reference clock frequency
// since the external frequency has already been checked only the maximum frequency for each FRDIV value needs to be compared here.
  if (crystal_val <= 1250000) {frdiv_val = 0;}
  else if (crystal_val <= 2500000) {frdiv_val = 1;}
  else if (crystal_val <= 5000000) {frdiv_val = 2;}
  else if (crystal_val <= 10000000) {frdiv_val = 3;}
  else if (crystal_val <= 20000000) {frdiv_val = 4;}
  else {frdiv_val = 5;}
   
  // The FLL ref clk divide value depends on FRDIV and the RANGE value
  if (((MCG_C2 & MCG_C2_RANGE_MASK) >> MCG_C2_RANGE_SHIFT) > 0)
  {
    fll_ref_freq = ((crystal_val) / (32 << frdiv_val));
  }
  else
  {
    fll_ref_freq = ((crystal_val) / (1 << frdiv_val));
  }
  
// Check resulting FLL frequency 
  mcg_out = fll_freq(fll_ref_freq); // FLL reference frequency calculated from ext ref freq and FRDIV
  if (mcg_out < 0x5B) {return mcg_out;} // If error code returned, return the code to calling function
  
// Select external oscilator and Reference Divider and clear IREFS to start ext osc
// If IRCLK is required it must be enabled outside of this driver, existing state will be maintained
// CLKS=0, FRDIV=frdiv_val, IREFS=0, IRCLKEN=0, IREFSTEN=0
  temp_reg = MCG_C1;
  temp_reg &= ~(MCG_C1_CLKS_MASK | MCG_C1_FRDIV_MASK | MCG_C1_IREFS_MASK); // Clear values in these fields
  temp_reg = MCG_C1_CLKS(0) | MCG_C1_FRDIV(frdiv_val); // Set the required CLKS and FRDIV values
  MCG_C1 = temp_reg;

// if the external oscillator is used need to wait for OSCINIT to set
  if (erefs_val)
  {
    for (i = 0 ; i < 20000000 ; i++)
    {
      if (MCG_S & MCG_S_OSCINIT_MASK) break; // jump out early if OSCINIT sets before loop finishes
    }
    if (!(MCG_S & MCG_S_OSCINIT_MASK)) return 0x23; // check bit is really set and return with error if not set
  }

// wait for Reference clock Status bit to clear
  for (i = 0 ; i < 2000 ; i++)
  {
    if (!(MCG_S & MCG_S_IREFST_MASK)) break; // jump out early if IREFST clears before loop finishes
  }
  if (MCG_S & MCG_S_IREFST_MASK) return 0x11; // check bit is really clear and return with error if not set
  
// Now in FBE  
// It is recommended that the clock monitor is enabled when using an external clock as the clock source/reference.
// It is enabled here but can be removed if this is not required.
  MCG_C6 |= MCG_C6_CME_MASK;
  
  return mcg_out; // MCGOUT frequency equals FLL frequency
} // fei_fee


int fee_fei(int slow_irc_freq)
{
  short i;
  int mcg_out;

// Check MCG is in FEE mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) && // check CLKS mux has selcted FLL
      (!(MCG_S & MCG_S_IREFST_MASK)) &&                             // check FLL ref is external ref clk
      (!(MCG_S & MCG_S_PLLST_MASK))))                               // check PLLS mux has selected FLL
  {
    return 0x2;                                                     // return error code
  } 
      
// Check IRC frequency is within spec.
  if ((slow_irc_freq < 31250) || (slow_irc_freq > 39063))
  {
    return 0x31;
  }

  // Check resulting FLL frequency 
  mcg_out = fll_freq(slow_irc_freq); 
  if (mcg_out < 0x5B) {return mcg_out;} // If error code returned, return the code to calling function
  
// Ensure clock monitor is disabled before switching to FEI otherwise a loss of clock will trigger
  MCG_C6 &= ~MCG_C6_CME_MASK;

// Change FLL reference clock from external to internal by setting IREFS bit
  MCG_C1 |= MCG_C1_IREFS_MASK; // select internal reference
  
// wait for Reference clock to switch to internal reference 
  for (i = 0 ; i < 2000 ; i++)
  {
    if (MCG_S & MCG_S_IREFST_MASK) break; // jump out early if IREFST sets before loop finishes
  }
  if (!(MCG_S & MCG_S_IREFST_MASK)) return 0x12; // check bit is really set and return with error if not set  
  
// Now in FEI mode  
  return mcg_out;  
} // fee_fei



unsigned char atc(unsigned char irc_select, int irc_freq, int mcg_out_freq)
{
  unsigned char mcg_mode;
  unsigned short atcv;
  int bus_clock_freq;
  int  bus_clk_div_val;
  int orig_div;
  int temp_reg;
  
  if (irc_select > 0) // force irc to 1 if greater than 0
  {
    irc_select = 1;
  }
  
  mcg_mode = what_mcg_mode(); // get present MCG mode
  if ((mcg_mode != PEE) && (mcg_mode != PBE) && (mcg_mode != FBE))
  {
    return 1; // return error code if not in PEE, PBE or FBE modes
  }
  
  orig_div = SIM_CLKDIV1; //store present clock divider values
  
  bus_clk_div_val = mcg_out_freq / 16000000; // calculate bus clock divider to generate fastest allowed bus clock for autotrim
  temp_reg = SIM_CLKDIV1;
  temp_reg &= ~(SIM_CLKDIV1_OUTDIV2_MASK | SIM_CLKDIV1_OUTDIV3_MASK | SIM_CLKDIV1_OUTDIV4_MASK); // clear dividers except core
  // set all bus and flash dividers to same value to ensure clocking restrictions are met
  temp_reg |= (SIM_CLKDIV1_OUTDIV2(bus_clk_div_val) | SIM_CLKDIV1_OUTDIV3(bus_clk_div_val) | SIM_CLKDIV1_OUTDIV4(bus_clk_div_val));
  SIM_CLKDIV1 = temp_reg; // set actual dividers
  
  bus_clock_freq = mcg_out_freq / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
  if ((bus_clock_freq < 8000000) || (bus_clock_freq > 16000000))
  {
    SIM_CLKDIV1 = orig_div; // set SIM_CLKDIV1 back to original value
    return 3; // error, bus clock frequency is not within 8MHz to 16MHz
  }
                
  if(!irc_select) //determine if slow or fast IRC to be trimmed
  {
    if (irc_freq < 31250) // check frequency is above min spec.
    {
      SIM_CLKDIV1 = orig_div; // set SIM_CLKDIV1 back to original value
      return 4;
    }
    if (irc_freq > 39062) // check frequency is below max spec.
    {
      SIM_CLKDIV1 = orig_div; // set SIM_CLKDIV1 back to original value
      return 5;
    }         
  }
  else
  {
    if (irc_freq < 3000000) // check frequency is above min spec.
    {
      SIM_CLKDIV1 = orig_div; // set SIM_CLKDIV1 back to original value
      return 6;
    }
    if (irc_freq > 5000000) // check frequency is below max spec.
    {
      SIM_CLKDIV1 = orig_div; // set SIM_CLKDIV1 back to original value
      return 7;
    }            
  } // if
        
// Set up autocal registers, must use floating point calculation
  if (irc_select) 
    atcv = (unsigned short)(128.0f * (21.0f * (bus_clock_freq / (float)irc_freq)));
  else
    atcv = (unsigned short)(21.0f * (bus_clock_freq / (float)irc_freq));
        
  MCG_ATCVL = (atcv & 0xFF); //Set ATCVL to lower 8 bits of count value
  MCG_ATCVH = ((atcv & 0xFF00) >> 8); // Set ATCVH to upper 8 bits of count value

// Enable autocal
  MCG_SC &= ~(MCG_SC_ATME_MASK | MCG_SC_ATMS_MASK |MCG_SC_ATMF_MASK); // clear auto trim settings
  temp_reg = (MCG_SC_ATME_MASK | (irc_select << MCG_SC_ATMS_SHIFT)); //Select IRC to trim and enable trim machine
  MCG_SC |= temp_reg;
        
  while (MCG_SC & MCG_SC_ATME_MASK) {}; //poll for ATME bit to clear
        
  if (MCG_SC & MCG_SC_ATMF_MASK) // check if error flag set
  {
    SIM_CLKDIV1 = orig_div; // set SIM_CLKDIV1 back to original value
    return 8;
  } 
  else 
  {      
    if (!irc_select)
    {
      if ((MCG_C3 == 0xFF) || (MCG_C3 == 0))
      {
        SIM_CLKDIV1 = orig_div; // set SIM_CLKDIV1 back to original value
        return 9;
      }
    }
    else
    {
      if ((((MCG_C4 & MCG_C4_FCTRIM_MASK) >> MCG_C4_FCTRIM_SHIFT) == 0xF) ||
          (((MCG_C4 & MCG_C4_FCTRIM_MASK) >> MCG_C4_FCTRIM_SHIFT) == 0))
      {
        SIM_CLKDIV1 = orig_div; // set SIM_CLKDIV1 back to original value
        return 10;
      }
    }
  }
  SIM_CLKDIV1 = orig_div; // set SIM_CLKDIV1 back to original value
  return 0;
}// atc




int fll_freq(int fll_ref)
{
  int fll_freq_hz;
  
  if (MCG_C4 & MCG_C4_DMX32_MASK) // if DMX32 set
  {
    switch ((MCG_C4 & MCG_C4_DRST_DRS_MASK) >> MCG_C4_DRST_DRS_SHIFT) // determine multiplier based on DRS
    {
    case 0:
      fll_freq_hz = (fll_ref * 732);
      if (fll_freq_hz < 20000000) {return 0x33;}
      else if (fll_freq_hz > 25000000) {return 0x34;}
      break;
    case 1:
      fll_freq_hz = (fll_ref * 1464);
      if (fll_freq_hz < 40000000) {return 0x35;}
      else if (fll_freq_hz > 50000000) {return 0x36;}
      break;
    case 2:
      fll_freq_hz = (fll_ref * 2197);
      if (fll_freq_hz < 60000000) {return 0x37;}
      else if (fll_freq_hz > 75000000) {return 0x38;}
      break;
    case 3:
      fll_freq_hz = (fll_ref * 2929);
      if (fll_freq_hz < 80000000) {return 0x39;}
      else if (fll_freq_hz > 100000000) {return 0x3A;}
      break;
    }
  }
  else // if DMX32 = 0
  {
    switch ((MCG_C4 & MCG_C4_DRST_DRS_MASK) >> MCG_C4_DRST_DRS_SHIFT) // determine multiplier based on DRS
    {
    case 0:
      fll_freq_hz = (fll_ref * 640);
      if (fll_freq_hz < 20000000) {return 0x33;}
      else if (fll_freq_hz > 25000000) {return 0x34;}
      break;
    case 1:
      fll_freq_hz = (fll_ref * 1280);
      if (fll_freq_hz < 40000000) {return 0x35;}
      else if (fll_freq_hz > 50000000) {return 0x36;}
      break;
    case 2:
      fll_freq_hz = (fll_ref * 1920);
      if (fll_freq_hz < 60000000) {return 0x37;}
      else if (fll_freq_hz > 75000000) {return 0x38;}
      break;
    case 3:
      fll_freq_hz = (fll_ref * 2560);
      if (fll_freq_hz < 80000000) {return 0x39;}
      else if (fll_freq_hz > 100000000) {return 0x3A;}
      break;
    }
  }    
  return fll_freq_hz;
} // fll_freq


unsigned char what_mcg_mode(void)
{
  // check if in FEI mode
  if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) &&      // check CLKS mux has selcted FLL output
      (MCG_S & MCG_S_IREFST_MASK) &&                                     // check FLL ref is internal ref clk
      (!(MCG_S & MCG_S_PLLST_MASK)))                                     // check PLLS mux has selected FLL
  {
    return FEI;                                                          // return FEI code
  }
  // Check MCG is in PEE mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x3) && // check CLKS mux has selcted PLL output
          (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
          (MCG_S & MCG_S_PLLST_MASK))                                    // check PLLS mux has selected PLL 
  {
    return PEE;                                                          // return PEE code
  }
  // Check MCG is in PBE mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
          (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
          (MCG_S & MCG_S_PLLST_MASK) &&                                  // check PLLS mux has selected PLL
          (!(MCG_C2 & MCG_C2_LP_MASK)))                                  // check MCG_C2[LP] bit is not set
  {
    return PBE;                                                          // return PBE code
  }
  // Check MCG is in FBE mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
          (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
          (!(MCG_S & MCG_S_PLLST_MASK)) &&                               // check PLLS mux has selected FLL
          (!(MCG_C2 & MCG_C2_LP_MASK)))                                  // check MCG_C2[LP] bit is not set   
  {
    return FBE;                                                          // return FBE code
  }
  // Check MCG is in BLPE mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) && // check CLKS mux has selcted external reference
          (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
          (MCG_C2 & MCG_C2_LP_MASK))                                     // check MCG_C2[LP] bit is set   
  {
    return BLPE;                                                         // return BLPE code
  }
  // check if in BLPI mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1) && // check CLKS mux has selcted int ref clk
          (MCG_S & MCG_S_IREFST_MASK) &&                                 // check FLL ref is internal ref clk
          (!(MCG_S & MCG_S_PLLST_MASK)) &&                               // check PLLS mux has selected FLL
          (MCG_C2 & MCG_C2_LP_MASK))                                     // check LP bit is set
  {
    return BLPI;                                                         // return BLPI code
  }
  // check if in FBI mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1) && // check CLKS mux has selcted int ref clk
          (MCG_S & MCG_S_IREFST_MASK) &&                                 // check FLL ref is internal ref clk
          (!(MCG_S & MCG_S_PLLST_MASK)) &&                               // check PLLS mux has selected FLL
          (!(MCG_C2 & MCG_C2_LP_MASK)))                                  // check LP bit is clear
  {  
    return FBI;                                                          // return FBI code 
  }
  // Check MCG is in FEE mode
  else if ((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) && // check CLKS mux has selcted FLL
          (!(MCG_S & MCG_S_IREFST_MASK)) &&                              // check FLL ref is external ref clk
          (!(MCG_S & MCG_S_PLLST_MASK)))                                 // check PLLS mux has selected FLL
  {
    return FEE;                                                          // return FEE code
  }
  else
  {
    return 0;                                                            // error condition
  }
} // what_mcg_mode


/********************************************************************/
/* Functon name : clk_monitor_0
 *
 * This function simply enables or disables the OSC 0 clock monitor. This is
 * achieved by setting or clearing the MCG_C6[CME] bit. It is recommended to  
 * enable this monitor in external clock modes (FEE, FBE, BLPE, PBE and PEE.
 * It MUST be disabled in all other modes or a reset may be generated. It must
 * also be disabled if it is desired to enter VLPR from BLPE mode.
 *
 * Parameters: en_dis - enables (= 1) or disables (= 0) the OSC 0 clock monitor
 *
 * Return value : none
 */
void clk_monitor_0(unsigned char en_dis)
{         
  if (en_dis)
  {
    MCG_C6 |= MCG_C6_CME_MASK;   
  }
  else
  {
    MCG_C6 &= ~MCG_C6_CME_MASK;
  }
}    // end clk_monitor_0

/*
int fei_pee(int crystal_val, unsigned char hgo_val, unsigned char erefs_val, signed char prdiv_val, signed char vdiv_val)
{
  unsigned char frdiv_val;
  unsigned char temp_reg;
  unsigned char prdiv, vdiv;
  short i;
  int ref_freq;
  int pll_freq;

// check if in FEI mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) && // check CLKS mux has selcted FLL output
      (MCG_S & MCG_S_IREFST_MASK) &&                                  // check FLL ref is internal ref clk
      (!(MCG_S & MCG_S_PLLST_MASK))))                                 // check PLLS mux has selected FLL
  {
    return 0x1;                                                     // return error code
  }

// check external frequency is less than the maximum frequency
  if  (crystal_val > 50000000) {return 0x21;}

// check crystal frequency is within spec. if crystal osc is being used as PLL ref
  if (erefs_val)
  {
    if ((crystal_val < 8000000) || (crystal_val > 32000000)) {return 0x22;} // return 1 if one of the available crystal options is not available
  }

// make sure HGO will never be greater than 1. Could return an error instead if desired.
  if (hgo_val > 0)
  {
    hgo_val = 1; // force hgo_val to 1 if > 0
  }

// Check PLL divider settings are within spec.
  if ((prdiv_val < 1) || (prdiv_val > 8)) {return 0x41;}
  if ((vdiv_val < 16) || (vdiv_val > 47)) {return 0x42;}

// Check PLL reference clock frequency is within spec.
  ref_freq = crystal_val / prdiv_val;
  if ((ref_freq < 8000000) || (ref_freq > 32000000)) {return 0x43;}

// Check PLL output frequency is within spec.
  pll_freq = (crystal_val / prdiv_val) * vdiv_val;
  if ((pll_freq < 180000000) || (pll_freq > 360000000)) {return 0x45;}

// configure the MCG_C2 register
// the RANGE value is determined by the external frequency. Since the RANGE parameter affects the FRDIV divide value
// it still needs to be set correctly even if the oscillator is not being used
  if (crystal_val <= 40000)
  {
    MCG_C2 = MCG_C2_RANGE(0) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT);
  }
  else if (crystal_val <= 8000000)
  {
    MCG_C2 = MCG_C2_RANGE(1) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT);
  }
  else
  {
    MCG_C2 = MCG_C2_RANGE(2) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT);
  }

// determine FRDIV based on reference clock frequency
// since the external frequency has already been checked only the maximum frequency for each FRDIV value needs to be compared here.
  if (crystal_val <= 1250000) {frdiv_val = 0;}
  else if (crystal_val <= 2500000) {frdiv_val = 1;}
  else if (crystal_val <= 5000000) {frdiv_val = 2;}
  else if (crystal_val <= 10000000) {frdiv_val = 3;}
  else if (crystal_val <= 20000000) {frdiv_val = 4;}
  else {frdiv_val = 5;}

// Select external oscillator and Reference Divider and clear IREFS to start ext osc
// If IRCLK is required it must be enabled outside of this driver, existing state will be maintained
// CLKS=2, FRDIV=frdiv_val, IREFS=0, IRCLKEN=0, IREFSTEN=0
  temp_reg = MCG_C1;
  temp_reg &= ~(MCG_C1_CLKS_MASK | MCG_C1_FRDIV_MASK | MCG_C1_IREFS_MASK); // Clear values in these fields
  temp_reg = MCG_C1_CLKS(2) | MCG_C1_FRDIV(frdiv_val); // Set the required CLKS and FRDIV values
  MCG_C1 = temp_reg;

// if the external oscillator is used need to wait for OSCINIT to set
  if (erefs_val)
  {
    for (i = 0 ; i < 10000 ; i++)
    {
      if (MCG_S & MCG_S_OSCINIT_MASK) break; // jump out early if OSCINIT sets before loop finishes
    }
    if (!(MCG_S & MCG_S_OSCINIT_MASK)) return 0x23; // check bit is really set and return with error if not set
  }

// wait for Reference clock Status bit to clear
  for (i = 0 ; i < 2000 ; i++)
  {
    if (!(MCG_S & MCG_S_IREFST_MASK)) break; // jump out early if IREFST clears before loop finishes
  }
  if (MCG_S & MCG_S_IREFST_MASK) return 0x11; // check bit is really clear and return with error if not set

// Wait for clock status bits to show clock source is ext ref clk
  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) break; // jump out early if CLKST shows EXT CLK slected before loop finishes
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2) return 0x1A; // check EXT CLK is really selected and return with error if not

// Now in FBE
// It is recommended that the clock monitor is enabled when using an external clock as the clock source/reference.
// It is enabled here but can be removed if this is not required.
  MCG_C6 |= MCG_C6_CME_MASK;

// Configure MCG_C5
// If the PLL is to run in STOP mode then the PLLSTEN bit needs to be OR'ed in here or in user code.
  temp_reg = MCG_C5;
  temp_reg &= ~MCG_C5_PRDIV_MASK;
  temp_reg |= MCG_C5_PRDIV(prdiv_val - 1);    //set PLL ref divider
  MCG_C5 = temp_reg;

// Configure MCG_C6
// The PLLS bit is set to enable the PLL, MCGOUT still sourced from ext ref clk
// The loss of lock interrupt can be enabled by seperately OR'ing in the LOLIE bit in MCG_C6
  temp_reg = MCG_C6; // store present C6 value
  temp_reg &= ~MCG_C6_VDIV_MASK; // clear VDIV settings
  temp_reg |= MCG_C6_PLLS_MASK | MCG_C6_VDIV(vdiv_val - 16); // write new VDIV and enable PLL
  MCG_C6 = temp_reg; // update MCG_C6

// wait for PLLST status bit to set
  for (i = 0 ; i < 2000 ; i++)
  {
    if (MCG_S & MCG_S_PLLST_MASK) break; // jump out early if PLLST sets before loop finishes
  }
  if (!(MCG_S & MCG_S_PLLST_MASK)) return 0x16; // check bit is really set and return with error if not set

// Wait for LOCK bit to set
  for (i = 0 ; i < 2000 ; i++)
  {
    if (MCG_S & MCG_S_LOCK_MASK) break; // jump out early if LOCK sets before loop finishes
  }
  if (!(MCG_S & MCG_S_LOCK_MASK)) return 0x44; // check bit is really set and return with error if not set

// now in PBE

  MCG_C1 &= ~MCG_C1_CLKS_MASK; // clear CLKS to switch CLKS mux to select PLL as MCG_OUT

// Wait for clock status bits to update
  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x3) break; // jump out early if CLKST = 3 before loop finishes
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3) return 0x1B; // check CLKST is set correctly and return with error if not

// Use actual PLL settings to calculate PLL frequency
  prdiv = ((MCG_C5 & MCG_C5_PRDIV_MASK) + 1);
  vdiv = ((MCG_C6 & MCG_C6_VDIV_MASK) + 16);

// Now in PEE
  return (((crystal_val / prdiv) * vdiv) / 2); //MCGOUT equals PLL output frequency/2
} // fei_pee
*/


