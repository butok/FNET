/*
 * File:        rtc.c
 * Purpose:     Provide common RTC routines
 *
 * Notes:       
 *              
 */


#include "common.h"
#include "rtc.h"
#include "arm_cm4.h"

/********************************************************************/
/*
 * Initialize the RTC
 *
 *
 * Parameters:
 *  seconds         Start value of seconds register
 *  alarm           Time in seconds of first alarm. Set to 0xFFFFFFFF to effectively disable alarm
 *  c_interval      Interval at which to apply time compensation can range from 1 second (0x0) to 256 (0xFF)
 *  c_value         Compensation value ranges from -127 32kHz cycles to +128 32 kHz cycles
 *                  80h Time prescaler register overflows every 32896 clock cycles.
 *                  FFh Time prescaler register overflows every 32769 clock cycles.
 *                  00h Time prescaler register overflows every 32768 clock cycles.
 *                  01h Time prescaler register overflows every 32767 clock cycles.
 *                  7Fh Time prescaler register overflows every 32641 clock cycles.
 *  interrupt       TRUE or FALSE
 */

void rtc_init(uint32 seconds, uint32 alarm, uint8 c_interval, uint8 c_value, uint8 interrupt) 
{
  int i;
  
  /*enable the clock to SRTC module register space*/
  SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;
  
  /*Only VBAT_POR has an effect on the SRTC, RESET to the part does not, so you must manually reset the SRTC to make sure everything is in a known state*/
  /*clear the software reset bit*/
  RTC_CR  = RTC_CR_SWR_MASK;
  RTC_CR  &= ~RTC_CR_SWR_MASK;  
  
  /*Enable the interrupt*/
  if(interrupt)
    enable_irq(66);
  
  /*Enable the oscillator*/
  RTC_CR |= RTC_CR_OSCE_MASK;
  
  /*Wait to all the 32 kHz to stabilize, refer to the crystal startup time in the crystal datasheet*/
  for(i=0;i<0x600000;i++);
  
  /*Set time compensation parameters*/
  RTC_TCR = RTC_TCR_CIR(c_interval) | RTC_TCR_TCR(c_value);
  
  /*Configure the timer seconds and alarm registers*/
  RTC_TSR = seconds;
  RTC_TAR = alarm;
  
  /*Enable the counter*/
  RTC_SR |= RTC_SR_TCE_MASK;
  
}

void rtc_isr(void) 
{
   
   if((RTC_SR & RTC_SR_TIF_MASK)== 0x01)
     {
	   fnet_printf("SRTC time invalid interrupt entered...\r\n");
   	   RTC_SR &= 0x07;  //clear TCE, or RTC_TSR can  not be written
   	   RTC_TSR = 0x00000000;  //clear TIF 
     }	
   else if((RTC_SR & RTC_SR_TOF_MASK) == 0x02)
   {
	   fnet_printf("SRTC time overflow interrupt entered...\r\n");
   	   RTC_SR &= 0x07;  //clear TCE, or RTC_TSR can  not be written
   	   RTC_TSR = 0x00000000;  //clear TOF
   }	 	
   else if((RTC_SR & RTC_SR_TAF_MASK) == 0x04)
   {
	   fnet_printf("SRTC alarm interrupt entered...\r\n");
	   fnet_printf("Time Seconds Register value is: %i\n", RTC_TSR);
   	   RTC_TAR += 1;// Write new alarm value, to generate an alarm every second add 1
   }	
   else
   {
	   fnet_printf("No valid Flag was set!\n");
   }
   return;
}
