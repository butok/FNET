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

#ifndef __MCF54418_PIT_H__
#define __MCF54418_PIT_H__


/*********************************************************************
*
* Programmable Interrupt Timer (PIT)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_PIT0_PCSR                        (*(vuint16*)(0xFC080000))
#define MCF_PIT0_PMR                         (*(vuint16*)(0xFC080002))
#define MCF_PIT0_PCNTR                       (*(vuint16*)(0xFC080004))

#define MCF_PIT1_PCSR                        (*(vuint16*)(0xFC084000))
#define MCF_PIT1_PMR                         (*(vuint16*)(0xFC084002))
#define MCF_PIT1_PCNTR                       (*(vuint16*)(0xFC084004))

#define MCF_PIT2_PCSR                        (*(vuint16*)(0xFC088000))
#define MCF_PIT2_PMR                         (*(vuint16*)(0xFC088002))
#define MCF_PIT2_PCNTR                       (*(vuint16*)(0xFC088004))

#define MCF_PIT3_PCSR                        (*(vuint16*)(0xFC08C000))
#define MCF_PIT3_PMR                         (*(vuint16*)(0xFC08C002))
#define MCF_PIT3_PCNTR                       (*(vuint16*)(0xFC08C004))

#define MCF_PIT_PCSR(x)                      (*(vuint16*)(0xFC080000 + ((x)*0x4000)))
#define MCF_PIT_PMR(x)                       (*(vuint16*)(0xFC080002 + ((x)*0x4000)))
#define MCF_PIT_PCNTR(x)                     (*(vuint16*)(0xFC080004 + ((x)*0x4000)))


/* Bit definitions and macros for MCF_PIT_PCSR */
#define MCF_PIT_PCSR_EN                      (0x1)
#define MCF_PIT_PCSR_RLD                     (0x2)
#define MCF_PIT_PCSR_PIF                     (0x4)
#define MCF_PIT_PCSR_PIE                     (0x8)
#define MCF_PIT_PCSR_OVW                     (0x10)
#define MCF_PIT_PCSR_DBG                     (0x20)
#define MCF_PIT_PCSR_DOZE                    (0x40)
#define MCF_PIT_PCSR_PRE(x)                  (((x)&0xF)<<0x8)

/* Bit definitions and macros for MCF_PIT_PMR */
#define MCF_PIT_PMR_PM(x)                    (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PIT_PCNTR */
#define MCF_PIT_PCNTR_PC(x)                  (((x)&0xFFFF)<<0)


#endif /* __MCF54418_PIT_H__ */
