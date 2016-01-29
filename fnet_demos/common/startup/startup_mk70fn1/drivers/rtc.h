/*
 * File:		rtc.h
 * Purpose:     Provide common rtc functions
 *
 * Notes:
 */

#ifndef __RTC_H__
#define __RTC_H__
#include "common.h"
/********************************************************************/

void rtc_init(uint32 seconds, uint32 alarm, uint8 c_interval, uint8 c_value, uint8 interrupt);      
void rtc_isr(void);

/********************************************************************/

#endif /* __RTC_H__ */
