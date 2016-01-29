/********************************************************************/
/* Coldfire C Header File
 *
 *     Date      : 2010/07/19
 *     Revision  : 0.4
 *
 *     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 *     http      : www.freescale.com
 *     mail      : support@freescale.com
 */

#ifndef __MCF54418_RTC_H__
#define __MCF54418_RTC_H__


/*********************************************************************
*
* Robust Real-Time Clock (RTC)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_RTC_YEARMON                      (*(vuint16*)(0xFC0A8000))
#define MCF_RTC_DAYS                         (*(vuint16*)(0xFC0A8002))
#define MCF_RTC_HOURMIN                      (*(vuint16*)(0xFC0A8004))
#define MCF_RTC_SECONDS                      (*(vuint16*)(0xFC0A8006))
#define MCF_RTC_ALM_YRMON                    (*(vuint16*)(0xFC0A8008))
#define MCF_RTC_ALM_DAYS                     (*(vuint16*)(0xFC0A800A))
#define MCF_RTC_ALM_HM                       (*(vuint16*)(0xFC0A800C))
#define MCF_RTC_ALM_SEC                      (*(vuint16*)(0xFC0A800E))
#define MCF_RTC_CR                           (*(vuint16*)(0xFC0A8010))
#define MCF_RTC_SR                           (*(vuint16*)(0xFC0A8012))
#define MCF_RTC_ISR                          (*(vuint16*)(0xFC0A8014))
#define MCF_RTC_IER                          (*(vuint16*)(0xFC0A8016))
#define MCF_RTC_COUNT_DN                     (*(vuint16*)(0xFC0A8018))
#define MCF_RTC_CFG_DATA                     (*(vuint16*)(0xFC0A8020))
#define MCF_RTC_DST_HOUR                     (*(vuint16*)(0xFC0A8022))
#define MCF_RTC_DST_MON                      (*(vuint16*)(0xFC0A8024))
#define MCF_RTC_DST_DAY                      (*(vuint16*)(0xFC0A8026))
#define MCF_RTC_COMPEN                       (*(vuint16*)(0xFC0A8028))
#define MCF_RTC_UP_CNTRH                     (*(vuint16*)(0xFC0A8032))
#define MCF_RTC_UP_CNTRL                     (*(vuint16*)(0xFC0A8034))


/* Bit definitions and macros for MCF_RTC_YEARMON */
#define MCF_RTC_YEARMON_MONTH(x)             (((x)&0xFF)<<0)
#define MCF_RTC_YEARMON_YEAR(x)              (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_RTC_DAYS */
#define MCF_RTC_DAYS_DAY(x)                  (((x)&0xFF)<<0)
#define MCF_RTC_DAYS_DAYWEEK(x)              (((x)&0x7)<<0x8)
#define MCF_RTC_DAYS_DAYWEEK_SUN             (0)
#define MCF_RTC_DAYS_DAYWEEK_MON             (0x100)
#define MCF_RTC_DAYS_DAYWEEK_TUE             (0x200)
#define MCF_RTC_DAYS_DAYWEEK_WED             (0x300)
#define MCF_RTC_DAYS_DAYWEEK_THU             (0x400)
#define MCF_RTC_DAYS_DAYWEEK_FRI             (0x500)
#define MCF_RTC_DAYS_DAYWEEK_SAT             (0x600)

/* Bit definitions and macros for MCF_RTC_HOURMIN */
#define MCF_RTC_HOURMIN_MINUTES(x)           (((x)&0xFF)<<0)
#define MCF_RTC_HOURMIN_HOURS(x)             (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_RTC_SECONDS */
#define MCF_RTC_SECONDS_SECONDS(x)           (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_RTC_ALM_YRMON */
#define MCF_RTC_ALM_YRMON_MONTH(x)           (((x)&0xFF)<<0)
#define MCF_RTC_ALM_YRMON_YEAR(x)            (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_RTC_ALM_DAYS */
#define MCF_RTC_ALM_DAYS_DAY(x)              (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_RTC_ALM_HM */
#define MCF_RTC_ALM_HM_MINUTES(x)            (((x)&0xFF)<<0)
#define MCF_RTC_ALM_HM_HOURS(x)              (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_RTC_ALM_SEC */
#define MCF_RTC_ALM_SEC_SECONDS(x)           (((x)&0xFF)<<0)
#define MCF_RTC_ALM_SEC_DEC                  (0x100)
#define MCF_RTC_ALM_SEC_INC                  (0x200)

/* Bit definitions and macros for MCF_RTC_CR */
#define MCF_RTC_CR_WE(x)                     (((x)&0x3)<<0)
#define MCF_RTC_CR_AM(x)                     (((x)&0x3)<<0x2)
#define MCF_RTC_CR_DSTEN                     (0x40)
#define MCF_RTC_CR_BCDEN                     (0x80)
#define MCF_RTC_CR_SWR                       (0x100)

/* Bit definitions and macros for MCF_RTC_SR */
#define MCF_RTC_SR_INVAL                     (0x1)
#define MCF_RTC_SR_CDON                      (0x2)
#define MCF_RTC_SR_BERR                      (0x4)
#define MCF_RTC_SR_WPE                       (0x10)
#define MCF_RTC_SR_PORB                      (0x40)

/* Bit definitions and macros for MCF_RTC_ISR */
#define MCF_RTC_ISR_STW                      (0x2)
#define MCF_RTC_ISR_ALM                      (0x4)
#define MCF_RTC_ISR_DAY                      (0x8)
#define MCF_RTC_ISR_HR                       (0x10)
#define MCF_RTC_ISR_MIN                      (0x20)
#define MCF_RTC_ISR_1HZ                      (0x40)
#define MCF_RTC_ISR_2HZ                      (0x80)
#define MCF_RTC_ISR_SAM0                     (0x100)
#define MCF_RTC_ISR_SAM1                     (0x200)
#define MCF_RTC_ISR_SAM2                     (0x400)
#define MCF_RTC_ISR_SAM3                     (0x800)
#define MCF_RTC_ISR_SAM4                     (0x1000)
#define MCF_RTC_ISR_SAM5                     (0x2000)
#define MCF_RTC_ISR_SAM6                     (0x4000)
#define MCF_RTC_ISR_SAM7                     (0x8000)

/* Bit definitions and macros for MCF_RTC_IER */
#define MCF_RTC_IER_STW                      (0x2)
#define MCF_RTC_IER_ALM                      (0x4)
#define MCF_RTC_IER_DAY                      (0x8)
#define MCF_RTC_IER_HR                       (0x10)
#define MCF_RTC_IER_MIN                      (0x20)
#define MCF_RTC_IER_1HZ                      (0x40)
#define MCF_RTC_IER_2HZ                      (0x80)
#define MCF_RTC_IER_SAM0                     (0x100)
#define MCF_RTC_IER_SAM1                     (0x200)
#define MCF_RTC_IER_SAM2                     (0x400)
#define MCF_RTC_IER_SAM3                     (0x800)
#define MCF_RTC_IER_SAM4                     (0x1000)
#define MCF_RTC_IER_SAM5                     (0x2000)
#define MCF_RTC_IER_SAM6                     (0x4000)
#define MCF_RTC_IER_SAM7                     (0x8000)

/* Bit definitions and macros for MCF_RTC_COUNT_DN */
#define MCF_RTC_COUNT_DN_COUNT(x)            (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_RTC_CFG_DATA */
#define MCF_RTC_CFG_DATA_OSCEN               (0x8)
#define MCF_RTC_CFG_DATA_OSCBYP              (0x10)

/* Bit definitions and macros for MCF_RTC_DST_HOUR */
#define MCF_RTC_DST_HOUR_END(x)              (((x)&0xFF)<<0)
#define MCF_RTC_DST_HOUR_START(x)            (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_RTC_DST_MON */
#define MCF_RTC_DST_MON_END(x)               (((x)&0xFF)<<0)
#define MCF_RTC_DST_MON_START(x)             (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_RTC_DST_DAY */
#define MCF_RTC_DST_DAY_END(x)               (((x)&0xFF)<<0)
#define MCF_RTC_DST_DAY_START(x)             (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_RTC_COMPEN */
#define MCF_RTC_COMPEN_VALUE(x)              (((x)&0xFF)<<0)
#define MCF_RTC_COMPEN_INTERVAL(x)           (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_RTC_UP_CNTRH */
#define MCF_RTC_UP_CNTRH_COUNT(x)            (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_RTC_UP_CNTRL */
#define MCF_RTC_UP_CNTRL_COUNT0              (0x1)
#define MCF_RTC_UP_CNTRL_COUNT1              (0x2)
#define MCF_RTC_UP_CNTRL_COUNT2              (0x4)
#define MCF_RTC_UP_CNTRL_COUNT3              (0x8)
#define MCF_RTC_UP_CNTRL_COUNT4              (0x10)
#define MCF_RTC_UP_CNTRL_COUNT5              (0x20)
#define MCF_RTC_UP_CNTRL_COUNT6              (0x40)
#define MCF_RTC_UP_CNTRL_COUNT7              (0x80)
#define MCF_RTC_UP_CNTRL_COUNT8              (0x100)
#define MCF_RTC_UP_CNTRL_COUNT9              (0x200)
#define MCF_RTC_UP_CNTRL_COUNT10             (0x400)
#define MCF_RTC_UP_CNTRL_COUNT11             (0x800)
#define MCF_RTC_UP_CNTRL_COUNT12             (0x1000)
#define MCF_RTC_UP_CNTRL_COUNT13             (0x2000)
#define MCF_RTC_UP_CNTRL_COUNT14             (0x4000)
#define MCF_RTC_UP_CNTRL_COUNT15             (0x8000)


#endif /* __MCF54418_RTC_H__ */
