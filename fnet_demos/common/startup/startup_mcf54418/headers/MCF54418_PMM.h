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

#ifndef __MCF54418_PMM_H__
#define __MCF54418_PMM_H__


/*********************************************************************
*
* Power Management (PMM)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_PMM_LPCR                         (*(vuint8 *)(0xEC090007))
#define MCF_PMM_MISCCR                       (*(vuint16*)(0xEC09000E))
#define MCF_PMM_WCR                          (*(vuint8 *)(0xFC040013))
#define MCF_PMM_PPMSR0                       (*(vuint8 *)(0xFC04002C))
#define MCF_PMM_PPMCR0                       (*(vuint8 *)(0xFC04002D))
#define MCF_PMM_PPMSR1                       (*(vuint8 *)(0xFC04002E))
#define MCF_PMM_PPMCR1                       (*(vuint8 *)(0xFC04002F))
#define MCF_PMM_PPMHR0                       (*(vuint32*)(0xFC040030))
#define MCF_PMM_PPMLR0                       (*(vuint32*)(0xFC040034))
#define MCF_PMM_PPMHR1                       (*(vuint32*)(0xFC040038))
#define MCF_PMM_PPMLR1                       (*(vuint32*)(0xFC04003C))
#define MCF_PMM_PPMSR(x)                     (*(vuint8 *)(0xFC04002C + ((x)*0x2)))
#define MCF_PMM_PPMCR(x)                     (*(vuint8 *)(0xFC04002D + ((x)*0x2)))


/* Bit definitions and macros for MCF_PMM_LPCR */
#define MCF_PMM_LPCR_STPMD(x)                (((x)&0x3)<<0x3)
#define MCF_PMM_LPCR_STPMD_SYS_DISABLED      (0)
#define MCF_PMM_LPCR_STPMD_SYS_BUSCLK_DISABLED (0x8)
#define MCF_PMM_LPCR_STPMD_ONLY_OSC_ENABLED  (0x10)
#define MCF_PMM_LPCR_STPMD_ALL_DISABLED      (0x18)
#define MCF_PMM_LPCR_FWKUP                   (0x20)

/* Bit definitions and macros for MCF_PMM_MISCCR */
#define MCF_PMM_MISCCR_USBSRC                (0x1)
#define MCF_PMM_MISCCR_USBPUE                (0x2)
#define MCF_PMM_MISCCR_USBOOC                (0x4)
#define MCF_PMM_MISCCR_USBHOC                (0x8)
#define MCF_PMM_MISCCR_SSI0SRC               (0x10)
#define MCF_PMM_MISCCR_SSI1SRC               (0x20)
#define MCF_PMM_MISCCR_SDHCSRC               (0x40)
#define MCF_PMM_MISCCR_BMT(x)                (((x)&0x7)<<0x8)
#define MCF_PMM_MISCCR_BMT_65536             (0)
#define MCF_PMM_MISCCR_BMT_32768             (0x100)
#define MCF_PMM_MISCCR_BMT_16384             (0x200)
#define MCF_PMM_MISCCR_BMT_8192              (0x300)
#define MCF_PMM_MISCCR_BMT_4096              (0x400)
#define MCF_PMM_MISCCR_BMT_2048              (0x500)
#define MCF_PMM_MISCCR_BMT_1024              (0x600)
#define MCF_PMM_MISCCR_BMT_512               (0x700)
#define MCF_PMM_MISCCR_BME                   (0x800)
#define MCF_PMM_MISCCR_LIMP                  (0x1000)
#define MCF_PMM_MISCCR_PLLLOCK               (0x2000)
#define MCF_PMM_MISCCR_PWMEXTCLK(x)          (((x)&0x3)<<0xE)

/* Bit definitions and macros for MCF_PMM_WCR */
#define MCF_PMM_WCR_PRILVL(x)                (((x)&0x7)<<0)
#define MCF_PMM_WCR_LPMD(x)                  (((x)&0x3)<<0x4)
#define MCF_PMM_WCR_LPMD_RUN                 (0)
#define MCF_PMM_WCR_LPMD_DOZE                (0x10)
#define MCF_PMM_WCR_LPMD_WAIT                (0x20)
#define MCF_PMM_WCR_LPMD_STOP                (0x30)
#define MCF_PMM_WCR_ENBWCR                   (0x80)

/* Bit definitions and macros for MCF_PMM_PPMSR */
#define MCF_PMM_PPMSR_SMCD(x)                (((x)&0x3F)<<0)
#define MCF_PMM_PPMSR_SAMCD                  (0x40)

/* Bit definitions and macros for MCF_PMM_PPMCR */
#define MCF_PMM_PPMCR_CMCD(x)                (((x)&0x3F)<<0)
#define MCF_PMM_PPMCR_CAMCD                  (0x40)

/* Bit definitions and macros for MCF_PMM_PPMHR0 */
#define MCF_PMM_PPMHR0_CD32                  (0x1)
#define MCF_PMM_PPMHR0_CD33                  (0x2)
#define MCF_PMM_PPMHR0_CD34                  (0x4)
#define MCF_PMM_PPMHR0_CD35                  (0x8)
#define MCF_PMM_PPMHR0_CD36                  (0x10)
#define MCF_PMM_PPMHR0_CD37                  (0x20)
#define MCF_PMM_PPMHR0_CD38                  (0x40)
#define MCF_PMM_PPMHR0_CD39                  (0x80)
#define MCF_PMM_PPMHR0_CD42                  (0x400)
#define MCF_PMM_PPMHR0_CD43                  (0x800)
#define MCF_PMM_PPMHR0_CD44                  (0x1000)
#define MCF_PMM_PPMHR0_CD45                  (0x2000)
#define MCF_PMM_PPMHR0_CD46                  (0x4000)
#define MCF_PMM_PPMHR0_CD47                  (0x8000)
#define MCF_PMM_PPMHR0_CD48                  (0x10000)
#define MCF_PMM_PPMHR0_CD49                  (0x20000)
#define MCF_PMM_PPMHR0_CD50                  (0x40000)
#define MCF_PMM_PPMHR0_CD51                  (0x80000)
#define MCF_PMM_PPMHR0_CD53                  (0x200000)
#define MCF_PMM_PPMHR0_CD54                  (0x400000)
#define MCF_PMM_PPMHR0_CD55                  (0x800000)
#define MCF_PMM_PPMHR0_CD56                  (0x1000000)
#define MCF_PMM_PPMHR0_CD63                  (0x80000000)

/* Bit definitions and macros for MCF_PMM_PPMLR0 */
#define MCF_PMM_PPMLR0_CD2                   (0x4)
#define MCF_PMM_PPMLR0_CD8                   (0x100)
#define MCF_PMM_PPMLR0_CD9                   (0x200)
#define MCF_PMM_PPMLR0_CD14                  (0x4000)
#define MCF_PMM_PPMLR0_CD15                  (0x8000)
#define MCF_PMM_PPMLR0_CD17                  (0x20000)
#define MCF_PMM_PPMLR0_CD18                  (0x40000)
#define MCF_PMM_PPMLR0_CD19                  (0x80000)
#define MCF_PMM_PPMLR0_CD20                  (0x100000)
#define MCF_PMM_PPMLR0_CD22                  (0x400000)
#define MCF_PMM_PPMLR0_CD23                  (0x800000)
#define MCF_PMM_PPMLR0_CD24                  (0x1000000)
#define MCF_PMM_PPMLR0_CD25                  (0x2000000)
#define MCF_PMM_PPMLR0_CD26                  (0x4000000)
#define MCF_PMM_PPMLR0_CD27                  (0x8000000)
#define MCF_PMM_PPMLR0_CD28                  (0x10000000)
#define MCF_PMM_PPMLR0_CD29                  (0x20000000)
#define MCF_PMM_PPMLR0_CD30                  (0x40000000)
#define MCF_PMM_PPMLR0_CD31                  (0x80000000)

/* Bit definitions and macros for MCF_PMM_PPMHR1 */
#define MCF_PMM_PPMHR1_CD34                  (0x4)
#define MCF_PMM_PPMHR1_CD36                  (0x10)
#define MCF_PMM_PPMHR1_CD37                  (0x20)

/* Bit definitions and macros for MCF_PMM_PPMLR1 */
#define MCF_PMM_PPMLR1_CD2                   (0x4)
#define MCF_PMM_PPMLR1_CD4                   (0x10)
#define MCF_PMM_PPMLR1_CD5                   (0x20)
#define MCF_PMM_PPMLR1_CD6                   (0x40)
#define MCF_PMM_PPMLR1_CD7                   (0x80)
#define MCF_PMM_PPMLR1_CD14                  (0x4000)
#define MCF_PMM_PPMLR1_CD15                  (0x8000)
#define MCF_PMM_PPMLR1_CD24                  (0x1000000)
#define MCF_PMM_PPMLR1_CD25                  (0x2000000)
#define MCF_PMM_PPMLR1_CD26                  (0x4000000)
#define MCF_PMM_PPMLR1_CD27                  (0x8000000)
#define MCF_PMM_PPMLR1_CD28                  (0x10000000)
#define MCF_PMM_PPMLR1_CD29                  (0x20000000)


#endif /* __MCF54418_PMM_H__ */
