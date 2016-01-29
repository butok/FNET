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

#ifndef __MCF54418_DAC_H__
#define __MCF54418_DAC_H__


/*********************************************************************
*
* Digital to Analog Converter (DAC)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_DAC0_CR                          (*(vuint16*)(0xFC098000))
#define MCF_DAC0_DATA                        (*(vuint16*)(0xFC098002))
#define MCF_DAC0_STEP                        (*(vuint16*)(0xFC098004))
#define MCF_DAC0_MIN                         (*(vuint16*)(0xFC098006))
#define MCF_DAC0_MAX                         (*(vuint16*)(0xFC098008))
#define MCF_DAC0_SR                          (*(vuint16*)(0xFC09800A))
#define MCF_DAC0_FILTCNT                     (*(vuint16*)(0xFC09800C))

#define MCF_DAC1_CR                          (*(vuint16*)(0xFC09C000))
#define MCF_DAC1_DATA                        (*(vuint16*)(0xFC09C002))
#define MCF_DAC1_STEP                        (*(vuint16*)(0xFC09C004))
#define MCF_DAC1_MIN                         (*(vuint16*)(0xFC09C006))
#define MCF_DAC1_MAX                         (*(vuint16*)(0xFC09C008))
#define MCF_DAC1_SR                          (*(vuint16*)(0xFC09C00A))
#define MCF_DAC1_FILTCNT                     (*(vuint16*)(0xFC09C00C))

#define MCF_DAC_CR(x)                        (*(vuint16*)(0xFC098000 + ((x)*0x4000)))
#define MCF_DAC_DATA(x)                      (*(vuint16*)(0xFC098002 + ((x)*0x4000)))
#define MCF_DAC_STEP(x)                      (*(vuint16*)(0xFC098004 + ((x)*0x4000)))
#define MCF_DAC_MIN(x)                       (*(vuint16*)(0xFC098006 + ((x)*0x4000)))
#define MCF_DAC_MAX(x)                       (*(vuint16*)(0xFC098008 + ((x)*0x4000)))
#define MCF_DAC_SR(x)                        (*(vuint16*)(0xFC09800A + ((x)*0x4000)))
#define MCF_DAC_FILTCNT(x)                   (*(vuint16*)(0xFC09800C + ((x)*0x4000)))


/* Bit definitions and macros for MCF_DAC_CR */
#define MCF_DAC_CR_PDN                       (0x1)
#define MCF_DAC_CR_FORMAT                    (0x2)
#define MCF_DAC_CR_SYNC_EN                   (0x4)
#define MCF_DAC_CR_AUTO                      (0x8)
#define MCF_DAC_CR_DOWN                      (0x10)
#define MCF_DAC_CR_UP                        (0x20)
#define MCF_DAC_CR_HSLS                      (0x40)
#define MCF_DAC_CR_DMAEN                     (0x80)
#define MCF_DAC_CR_WMLVL(x)                  (((x)&0x3)<<0x8)
#define MCF_DAC_CR_WMLVL_0                   (0)
#define MCF_DAC_CR_WMLVL_2                   (0x100)
#define MCF_DAC_CR_WMLVL_4                   (0x200)
#define MCF_DAC_CR_WMLVL_6                   (0x300)
#define MCF_DAC_CR_FILT_EN                   (0x1000)

/* Bit definitions and macros for MCF_DAC_DATA */
#define MCF_DAC_DATA_DATA(x)                 (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_DAC_STEP */
#define MCF_DAC_STEP_STEP(x)                 (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_DAC_MIN */
#define MCF_DAC_MIN_MIN(x)                   (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_DAC_MAX */
#define MCF_DAC_MAX_MAX(x)                   (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_DAC_SR */
#define MCF_DAC_SR_EMPTY                     (0x1)
#define MCF_DAC_SR_FULL                      (0x2)

/* Bit definitions and macros for MCF_DAC_FILTCNT */
#define MCF_DAC_FILTCNT_FILTCNT(x)           (((x)&0x3F)<<0)


#endif /* __MCF54418_DAC_H__ */
