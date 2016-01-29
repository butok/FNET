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

#ifndef __MCF54418_CLOCK_H__
#define __MCF54418_CLOCK_H__


/*********************************************************************
*
* Clock Module (CLOCK)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_CLOCK_PLL_CR                     (*(vuint32*)(0xFC0C0000))
#define MCF_CLOCK_PLL_DR                     (*(vuint32*)(0xFC0C0004))
#define MCF_CLOCK_PLL_SR                     (*(vuint32*)(0xFC0C0008))


/* Bit definitions and macros for MCF_CLOCK_PLL_CR */
#define MCF_CLOCK_PLL_CR_FBKDIV(x)           (((x)&0x3F)<<0)
#define MCF_CLOCK_PLL_CR_REFDIV(x)           (((x)&0x7)<<0x8)
#define MCF_CLOCK_PLL_CR_LOLEN               (0x1000)
#define MCF_CLOCK_PLL_CR_LOLRE               (0x2000)
#define MCF_CLOCK_PLL_CR_LOLIRQ              (0x4000)
#define MCF_CLOCK_PLL_CR_LOCEN               (0x10000)
#define MCF_CLOCK_PLL_CR_LOCRE               (0x20000)
#define MCF_CLOCK_PLL_CR_LOCIRQ              (0x40000)

/* Bit definitions and macros for MCF_CLOCK_PLL_DR */
#define MCF_CLOCK_PLL_DR_OUTDIV1(x)          (((x)&0x1F)<<0)
#define MCF_CLOCK_PLL_DR_OUTDIV2(x)          (((x)&0x1F)<<0x5)
#define MCF_CLOCK_PLL_DR_OUTDIV3(x)          (((x)&0x1F)<<0xA)
#define MCF_CLOCK_PLL_DR_OUTDIV4(x)          (((x)&0x1F)<<0x10)
#define MCF_CLOCK_PLL_DR_OUTDIV5(x)          (((x)&0x1F)<<0x15)

/* Bit definitions and macros for MCF_CLOCK_PLL_SR */
#define MCF_CLOCK_PLL_SR_MODE(x)             (((x)&0x7)<<0)
#define MCF_CLOCK_PLL_SR_LOCK                (0x10)
#define MCF_CLOCK_PLL_SR_LOCKS               (0x20)
#define MCF_CLOCK_PLL_SR_LOLF                (0x40)
#define MCF_CLOCK_PLL_SR_LOC                 (0x100)
#define MCF_CLOCK_PLL_SR_LOCF                (0x200)


#endif /* __MCF54418_CLOCK_H__ */
