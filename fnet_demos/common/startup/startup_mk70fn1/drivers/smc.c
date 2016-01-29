/*
 * File:        smc.c
 * Purpose:     Provides routines for entering low power modes.
 *
 * Notes:	Since the wakeup mechanism for low power modes
 *              will be application specific, these routines
 *              do not include code to setup clocks or interrupts to exit
 *              from the low power modes. The desired means of
 *              low power mode exit should be configured before
 *              calling any of these functions.
 *
 *              These routines do not include protection to
 *              prevent illegal state transitions in the mode
 *              controller, and all routines that write to the
 *              PMPROT register write a value to allow all
 *              possible low power modes (it is write once, so
 *              if only the currently requested mode is enabled
 *              a different mode couldn't be enabled later on).
 *              
 */

#include "common.h"
#include "smc.h"

/********************************************************************/
/* WAIT mode entry routine. Puts the processor into wait mode.
 * In this mode the core clock is disabled (no code executing), but 
 * bus clocks are enabled (peripheral modules are operational). 
 *
 * Mode transitions:
 * RUN -> WAIT
 * VLPR -> VLPW
 *
 * This function can be used to enter normal wait mode or VLPW
 * mode. If you are executing in normal run mode when calling this
 * function, then you will enter normal wait mode. If you are in VLPR
 * mode when calling this function, then you will enter VLPW mode instead.
 *
 * NOTE: Some modules include a programmable option to disable them in 
 * wait mode. If those modules are programmed to disable in wait mode, 
 * they will not be able to generate interrupts to wake up the core.
 *
 * WAIT mode is exited using any enabled interrupt or RESET, so no
 * exit_wait routine is needed. 
 *
 * If in VLPW mode, the statue of the SMC_PMCTRL[LPWUI] bit determines if 
 * the processor exits to VLPR (LPWUI cleared) or normal run mode (LPWUI 
 * set). The enable_lpwui() and disable_lpwui()functions can be used to set
 * this bit to the desired option prior to calling enter_wait().
 * 
 * 
 * Parameters:
 * none
 */
void enter_wait(void)
{
    wait();
}
/********************************************************************/
/* STOP mode entry routine. Puts the processor into normal stop mode.
 * In this mode core, bus and peripheral clocks are disabled.
 *
 * Mode transitions:
 * RUN -> STOP
 * VLPR -> VLPS
 *
 * This function can be used to enter normal stop mode or VLPS
 * mode. If you are executing in normal run mode when calling this
 * function, then you will enter normal stop mode. If you are in VLPR
 * mode when calling this function, then you will enter VLPS mode instead.
 *
 * STOP mode is exited using any enabled interrupt or RESET, so no
 * exit_stop routine is needed.
 *
 * Parameters:
 * none
 */
void enter_stop(void)
{
    /* Set the STOPM field to 0b000 for normal STOP mode - Need to retain state of LPWUI bit 8 */
    SMC_PMCTRL =  SMC_PMCTRL_STOPM(0);           // set STOPM = 0b000
    stop();
}
/********************************************************************/
/* VLPR mode entry routine. Puts the processor into very low power
 * run mode. In this mode all clocks are enabled, but the core, bus,
 * and peripheral clocks are limited to 2MHz or less. The flash 
 * clock is limited to 1MHz or less. 
 *
 * Mode transitions:
 * RUN -> VLPR
 *
 * exit_vlpr() function or an interrupt with LPWUI set can be used 
 * to switch from VLPR back to RUN. The enable_lpwui() and disable_lpwui()
 * functions can be used to set LPWUI to the desired option prior to 
 * calling enter_vlpr().
 *
 * Parameters:
 * none
 */
void enter_vlpr(char lpwui_value)
{
   
  if ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== 4){
	  fnet_printf(" \n[enter_vlpr] Already in VLPR Mode ! ");
         return;
         }
  SMC_PMPROT = SMC_PMPROT_AVLP_MASK;  // write oneif not all set make sure all enabled
                                       //this write-once bit allows the MCU to enter the
                                       //very low power modes: VLPR, VLPW, and VLPS.
    
   if(lpwui_value){
       SMC_PMCTRL = SMC_PMCTRL_LPWUI_MASK | SMC_PMCTRL_RUNM(2);   
   } else {
       SMC_PMCTRL = (~SMC_PMCTRL_LPWUI_MASK) & SMC_PMCTRL_RUNM(2);  
   }    
        
    
      /* Wait for VLPS regulator mode to be confirmed */

   while((PMC_REGSC & PMC_REGSC_REGONS_MASK) ==0x04){
	   fnet_printf(" \n[enter_vlpr] Waiting on REGONS to clear ! ");
   }   // 0 Regulator in stop Reg mode
       // 1 MCU is in Run regulation mode
   fnet_printf("[enter_vlpr] Now in VLPR at 19200 baud  \r\n");
   
         while((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK) != 4){
        	 fnet_printf(" \n[enter_vlpr] HALT PMSTAT does not indicate in VLPR Mode ! ");
         }
}
/********************************************************************/
/* VLPR mode exit routine. Puts the processor into normal run mode
 * from VLPR mode. You can transition from VLPR to normal run using
 * this function or an interrupt with LPWUI set. The enable_lpwui() 
 * and disable_lpwui() functions can be used to set LPWUI to the 
 * desired option prior to calling enter_vlpr().
 *
 * Mode transitions:
 * VLPR -> RUN
 *
 * Parameters:
 * none
 */
void exit_vlpr(void)
{
    /* check to make sure in VLPR before exiting    */
    if  ((SMC_PMSTAT & SMC_PMSTAT_PMSTAT_MASK)== 4) {
      
       /* Clear RUNM */
       SMC_PMCTRL &= ~(SMC_PMCTRL_RUNM(0x3));
                      
       /* Wait for normal RUN regulation mode to be confirmed */                   
       // 0 MCU is not in run regulation mode
       // 1 MCU is in run regulation mode
       while(!(PMC_REGSC & PMC_REGSC_REGONS_MASK)){
    	   fnet_printf(" \n[exit_vlpr] Waiting on REGONS bit to set to indicate Run regulation mode ! ");
         } 
       
    }  //if in VLPR mode
     // else if not in VLPR ignore call
}
/********************************************************************/
/* VLPS mode entry routine. Puts the processor into VLPS mode directly
 * from normal run mode. 
 *
 * Mode transitions:
 * RUN -> VLPS
 *
 * If VLPS mode is entered directly from normal RUN mode, then the 
 * LPWUI bit is forced to 1 by hardware. This means that when an
 * interrupt occurs you will exit to normal run mode instead of VLPR.
 *
 * If however VLPS mode is entered from VLPR the state of the LPWUI bit
 * determines the state the MCU will return to upon exit from VLPS.If LPWUI is set
 * and an interrupt occurs you will exit to normal run mode instead of VLPR. 
 * If LPWUI is clear and an interrupt occurs you will exit to VLPR.
 *
 * Parameters:  value of LPWUI
 * none
 */
void enter_vlps(char lpwui_value)
{
    /* Write to PMPROT to allow VLPS power modes */
    SMC_PMPROT = SMC_PMPROT_AVLP_MASK;   // write oneif not all set make sure all enabled
                                       //this write-once bit allows the MCU to enter the
                                       //very low power modes: VLPR, VLPW, and VLPS.
        
    /* Set the LPLLSM field to 0b010 for VLPS mode - Need to set state of LPWUI bit 8 */
    if(lpwui_value == 1){
     SMC_PMCTRL = (SMC_PMCTRL & (SMC_PMCTRL_RUNM_MASK))|
                  SMC_PMCTRL_LPWUI_MASK |
                  SMC_PMCTRL_STOPM(0x2) ;
     } else {
     SMC_PMCTRL = (SMC_PMCTRL & (SMC_PMCTRL_RUNM_MASK )) |
                  SMC_PMCTRL_STOPM(0x2) ;
     }  
    /* Now execute the stop instruction to go into VLPS */
    stop();
}
/********************************************************************/
/* LLS mode entry routine. Puts the processor into LLS mode from
 * normal run mode or VLPR. 
 *
 * Mode transitions:
 * RUN -> LLS
 * VLPR -> LLS
 *
 * NOTE: LLS mode will always exit to RUN mode even if you were 
 * in VLPR mode before entering LLS.
 *
 * Wakeup from LLS mode is controlled by the LLWU module. Most
 * modules cannot issue a wakeup interrupt in LLS mode, so make
 * sure to setup the desired wakeup sources in the LLWU before 
 * calling this function.
 *
 * Parameters:
 * none
 */
void enter_lls(void)
{
    /* Write to PMPROT to allow LLS power modes */
    SMC_PMPROT = SMC_PMPROT_ALLS_MASK;   //this write-once bit allows the MCU to enter the
                                       //LLS low power mode
    /* Set the LPLLSM field to 0b011 for LLS mode  */
    SMC_PMCTRL = SMC_PMCTRL_STOPM(0x3) ; 
    /* Now execute the stop instruction to go into LLS */
    stop();
}
/********************************************************************/
/* VLLS3 mode entry routine. Puts the processor into VLLS3 mode from
 * normal run mode or VLPR. 
 *
 * Mode transitions:
 * RUN -> VLLS3
 * VLPR -> VLLS3
 *
 * NOTE: VLLSx modes will always exit to RUN mode even if you were 
 * in VLPR mode before entering VLLSx.
 *
 * Wakeup from VLLSx mode is controlled by the LLWU module. Most
 * modules cannot issue a wakeup interrupt in VLLSx mode, so make
 * sure to setup the desired wakeup sources in the LLWU before 
 * calling this function.
 *
 * Parameters:
 * none  
 */
void enter_vlls3(void)
{
    /* Write to PMPROT to allow VLLS3 power modes */
    SMC_PMPROT = SMC_PMPROT_AVLLS_MASK;
        
    /* Set the VLLSM field to 0b100 for VLLS3 mode - Need to retain state of LPWUI bit 8 */
    SMC_PMCTRL = (SMC_PMCTRL & (SMC_PMCTRL_RUNM_MASK |SMC_PMCTRL_LPWUI_MASK)) |
                  SMC_PMCTRL_STOPM(0x4) ; // retain LPWUI
 
    SMC_VLLSCTRL =  SMC_VLLSCTRL_VLLSM(3);           // set VLLSM = 0b11

    /* Now execute the stop instruction to go into VLLS3 */
    stop();
}
/********************************************************************/
/* VLLS2 mode entry routine. Puts the processor into VLLS2 mode from
 * normal run mode or VLPR. 
 *
 * Mode transitions:
 * RUN -> VLLS2
 * VLPR -> VLLS2
 *
 * NOTE: VLLSx modes will always exit to RUN mode even if you were 
 * in VLPR mode before entering VLLSx.
 *
 * Wakeup from VLLSx mode is controlled by the LLWU module. Most
 * modules cannot issue a wakeup interrupt in VLLSx mode, so make
 * sure to setup the desired wakeup sources in the LLWU before 
 * calling this function.
 *
 * Parameters:
 * none  
 */
void enter_vlls2(void)
{
    /* Write to PMPROT to allow VLLS2 power modes */
    SMC_PMPROT = SMC_PMPROT_AVLLS_MASK;
        
    /* Set the VLLSM field to 0b100 for VLLS2 mode - Need to retain state of LPWUI bit 8 */
    SMC_PMCTRL = (SMC_PMCTRL & (SMC_PMCTRL_RUNM_MASK |SMC_PMCTRL_LPWUI_MASK)) |
                  SMC_PMCTRL_STOPM(0x4) ; // retain LPWUI
 
    SMC_VLLSCTRL =  SMC_VLLSCTRL_VLLSM(2);           // set VLLSM = 0b10
        
    /* Now execute the stop instruction to go into VLLS2 */
    stop();
}
/********************************************************************/
/* VLLS1 mode entry routine. Puts the processor into VLLS1 mode from
 * normal run mode or VLPR. 
 *
 * Mode transitions:
 * RUN -> VLLS1
 * VLPR -> VLLS1
 *
 * NOTE: VLLSx modes will always exit to RUN mode even if you were 
 * in VLPR mode before entering VLLSx.
 *
 * Wakeup from VLLSx mode is controlled by the LLWU module. Most
 * modules cannot issue a wakeup interrupt in VLLSx mode, so make
 * sure to setup the desired wakeup sources in the LLWU before 
 * calling this function.
 *
 * Parameters:
 * none  
 */
void enter_vlls1(void)
{
    /* Write to PMPROT to allow all possible power modes */
    /* Set the VLLSM field to 0b100 for VLLS1 mode - Need to retain state of LPWUI bit 8 */
    SMC_PMCTRL = (SMC_PMCTRL & (SMC_PMCTRL_RUNM_MASK |SMC_PMCTRL_LPWUI_MASK)) |
                  SMC_PMCTRL_STOPM(0x4) ; // retain LPWUI

    SMC_VLLSCTRL =  SMC_VLLSCTRL_VLLSM(1);           // set VLLSM = 0b01
       
    /* Now execute the stop instruction to go into VLLS1 */
    stop();
}
/********************************************************************/
/* Enable low power wake up on interrupt. This function can be used
 * to set the LPWUI bit. When this bit is set VLPx modes will exit
 * to normal run mode. When this bit is cleared VLPx modes will exit
 * to VLPR mode.
 *
 * The disable_lpwui() function can be used to clear the LPWUI bit.
 *
 * Parameters:
 * none
 */

   

void enable_lpwui(void)
{
    SMC_PMCTRL |= SMC_PMCTRL_LPWUI_MASK;
    fnet_printf("[enable_lpwui]SMC_PMCTRL   = %#02X \r\n", (SMC_PMCTRL))  ;
}
/********************************************************************/
/* Disable low power wake up on interrupt. This function can be used
 * to clear the LPWUI bit. When this bit is set VLPx modes will exit
 * to normal run mode. When this bit is cleared VLPx modes will exit
 * to VLPR mode.
 *
 * The enable_lpwui() function can be used to set the LPWUI bit.
 *
 * Parameters:
 * none
 */
void disable_lpwui(void)
{
    SMC_PMCTRL &= ~SMC_PMCTRL_LPWUI_MASK;
    fnet_printf("[disable_lpwui]SMC_PMCTRL   = %#02X \r\n", (SMC_PMCTRL))  ;
}

