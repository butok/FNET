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

#ifndef __MCF54418_XBS_H__
#define __MCF54418_XBS_H__


/*********************************************************************
*
* Cross-Bar Switch Module (XBS)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_XBS_XBS_PRS0                     (*(vuint32*)(0xFC004000))
#define MCF_XBS_XBS_CRS0                     (*(vuint32*)(0xFC004010))
#define MCF_XBS_XBS_PRS1                     (*(vuint32*)(0xFC004100))
#define MCF_XBS_XBS_CRS1                     (*(vuint32*)(0xFC004110))
#define MCF_XBS_XBS_PRS2                     (*(vuint32*)(0xFC004200))
#define MCF_XBS_XBS_CRS2                     (*(vuint32*)(0xFC004210))
#define MCF_XBS_XBS_PRS4                     (*(vuint32*)(0xFC004400))
#define MCF_XBS_XBS_CRS4                     (*(vuint32*)(0xFC004410))
#define MCF_XBS_XBS_PRS6                     (*(vuint32*)(0xFC004600))
#define MCF_XBS_XBS_CRS6                     (*(vuint32*)(0xFC004610))
#define MCF_XBS_XBS_PRS7                     (*(vuint32*)(0xFC004700))
#define MCF_XBS_XBS_CRS7                     (*(vuint32*)(0xFC004710))


/* Bit definitions and macros for MCF_XBS_XBS_PRS0 */
#define MCF_XBS_XBS_PRS0_M0(x)               (((x)&0x7)<<0)
#define MCF_XBS_XBS_PRS0_M1(x)               (((x)&0x7)<<0x4)
#define MCF_XBS_XBS_PRS0_M2(x)               (((x)&0x7)<<0x8)
#define MCF_XBS_XBS_PRS0_M3(x)               (((x)&0x7)<<0xC)
#define MCF_XBS_XBS_PRS0_M4(x)               (((x)&0x7)<<0x10)
#define MCF_XBS_XBS_PRS0_M5(x)               (((x)&0x7)<<0x14)
#define MCF_XBS_XBS_PRS0_M6(x)               (((x)&0x7)<<0x18)
#define MCF_XBS_XBS_PRS0_M7(x)               (((x)&0x7)<<0x1C)

/* Bit definitions and macros for MCF_XBS_XBS_CRS0 */
#define MCF_XBS_XBS_CRS0_PARK(x)             (((x)&0x7)<<0)
#define MCF_XBS_XBS_CRS0_PARK_CORE           (0)
#define MCF_XBS_XBS_CRS0_PARK_EDMA           (0x1)
#define MCF_XBS_XBS_CRS0_PARK_USB_OTG        (0x2)
#define MCF_XBS_XBS_CRS0_PARK_M3_ETH         (0x3)
#define MCF_XBS_XBS_CRS0_PARK_M4_ETH         (0x4)
#define MCF_XBS_XBS_CRS0_PARK_SDHC           (0x5)
#define MCF_XBS_XBS_CRS0_PARK_USB_HOST       (0x6)
#define MCF_XBS_XBS_CRS0_PARK_NANDFLASH      (0x7)
#define MCF_XBS_XBS_CRS0_PCTL(x)             (((x)&0x3)<<0x4)
#define MCF_XBS_XBS_CRS0_PCTL_PARK_FIELD     (0)
#define MCF_XBS_XBS_CRS0_PCTL_PARK_ON_LAST   (0x10)
#define MCF_XBS_XBS_CRS0_PCTL_PARK_NO_MASTER (0x20)
#define MCF_XBS_XBS_CRS0_ARB                 (0x100)
#define MCF_XBS_XBS_CRS0_RO                  (0x80000000)

/* Bit definitions and macros for MCF_XBS_XBS_PRS1 */
#define MCF_XBS_XBS_PRS1_M0(x)               (((x)&0x7)<<0)
#define MCF_XBS_XBS_PRS1_M1(x)               (((x)&0x7)<<0x4)
#define MCF_XBS_XBS_PRS1_M2(x)               (((x)&0x7)<<0x8)
#define MCF_XBS_XBS_PRS1_M3(x)               (((x)&0x7)<<0xC)
#define MCF_XBS_XBS_PRS1_M4(x)               (((x)&0x7)<<0x10)
#define MCF_XBS_XBS_PRS1_M5(x)               (((x)&0x7)<<0x14)
#define MCF_XBS_XBS_PRS1_M6(x)               (((x)&0x7)<<0x18)
#define MCF_XBS_XBS_PRS1_M7(x)               (((x)&0x7)<<0x1C)

/* Bit definitions and macros for MCF_XBS_XBS_CRS1 */
#define MCF_XBS_XBS_CRS1_PARK(x)             (((x)&0x7)<<0)
#define MCF_XBS_XBS_CRS1_PARK_CORE           (0)
#define MCF_XBS_XBS_CRS1_PARK_EDMA           (0x1)
#define MCF_XBS_XBS_CRS1_PARK_USB_OTG        (0x2)
#define MCF_XBS_XBS_CRS1_PARK_M3_ETH         (0x3)
#define MCF_XBS_XBS_CRS1_PARK_M4_ETH         (0x4)
#define MCF_XBS_XBS_CRS1_PARK_SDHC           (0x5)
#define MCF_XBS_XBS_CRS1_PARK_USB_HOST       (0x6)
#define MCF_XBS_XBS_CRS1_PARK_NANDFLASH      (0x7)
#define MCF_XBS_XBS_CRS1_PCTL(x)             (((x)&0x3)<<0x4)
#define MCF_XBS_XBS_CRS1_PCTL_PARK_FIELD     (0)
#define MCF_XBS_XBS_CRS1_PCTL_PARK_ON_LAST   (0x10)
#define MCF_XBS_XBS_CRS1_PCTL_PARK_NO_MASTER (0x20)
#define MCF_XBS_XBS_CRS1_ARB                 (0x100)
#define MCF_XBS_XBS_CRS1_RO                  (0x80000000)

/* Bit definitions and macros for MCF_XBS_XBS_PRS2 */
#define MCF_XBS_XBS_PRS2_M0(x)               (((x)&0x7)<<0)
#define MCF_XBS_XBS_PRS2_M1(x)               (((x)&0x7)<<0x4)
#define MCF_XBS_XBS_PRS2_M2(x)               (((x)&0x7)<<0x8)
#define MCF_XBS_XBS_PRS2_M3(x)               (((x)&0x7)<<0xC)
#define MCF_XBS_XBS_PRS2_M4(x)               (((x)&0x7)<<0x10)
#define MCF_XBS_XBS_PRS2_M5(x)               (((x)&0x7)<<0x14)
#define MCF_XBS_XBS_PRS2_M6(x)               (((x)&0x7)<<0x18)
#define MCF_XBS_XBS_PRS2_M7(x)               (((x)&0x7)<<0x1C)

/* Bit definitions and macros for MCF_XBS_XBS_CRS2 */
#define MCF_XBS_XBS_CRS2_PARK(x)             (((x)&0x7)<<0)
#define MCF_XBS_XBS_CRS2_PARK_CORE           (0)
#define MCF_XBS_XBS_CRS2_PARK_EDMA           (0x1)
#define MCF_XBS_XBS_CRS2_PARK_USB_OTG        (0x2)
#define MCF_XBS_XBS_CRS2_PARK_M3_ETH         (0x3)
#define MCF_XBS_XBS_CRS2_PARK_M4_ETH         (0x4)
#define MCF_XBS_XBS_CRS2_PARK_SDHC           (0x5)
#define MCF_XBS_XBS_CRS2_PARK_USB_HOST       (0x6)
#define MCF_XBS_XBS_CRS2_PARK_NANDFLASH      (0x7)
#define MCF_XBS_XBS_CRS2_PCTL(x)             (((x)&0x3)<<0x4)
#define MCF_XBS_XBS_CRS2_PCTL_PARK_FIELD     (0)
#define MCF_XBS_XBS_CRS2_PCTL_PARK_ON_LAST   (0x10)
#define MCF_XBS_XBS_CRS2_PCTL_PARK_NO_MASTER (0x20)
#define MCF_XBS_XBS_CRS2_ARB                 (0x100)
#define MCF_XBS_XBS_CRS2_RO                  (0x80000000)

/* Bit definitions and macros for MCF_XBS_XBS_PRS4 */
#define MCF_XBS_XBS_PRS4_M0(x)               (((x)&0x7)<<0)
#define MCF_XBS_XBS_PRS4_M1(x)               (((x)&0x7)<<0x4)
#define MCF_XBS_XBS_PRS4_M2(x)               (((x)&0x7)<<0x8)
#define MCF_XBS_XBS_PRS4_M3(x)               (((x)&0x7)<<0xC)
#define MCF_XBS_XBS_PRS4_M4(x)               (((x)&0x7)<<0x10)
#define MCF_XBS_XBS_PRS4_M5(x)               (((x)&0x7)<<0x14)
#define MCF_XBS_XBS_PRS4_M6(x)               (((x)&0x7)<<0x18)
#define MCF_XBS_XBS_PRS4_M7(x)               (((x)&0x7)<<0x1C)

/* Bit definitions and macros for MCF_XBS_XBS_CRS4 */
#define MCF_XBS_XBS_CRS4_PARK(x)             (((x)&0x7)<<0)
#define MCF_XBS_XBS_CRS4_PARK_CORE           (0)
#define MCF_XBS_XBS_CRS4_PARK_EDMA           (0x1)
#define MCF_XBS_XBS_CRS4_PARK_USB_OTG        (0x2)
#define MCF_XBS_XBS_CRS4_PARK_M3_ETH         (0x3)
#define MCF_XBS_XBS_CRS4_PARK_M4_ETH         (0x4)
#define MCF_XBS_XBS_CRS4_PARK_SDHC           (0x5)
#define MCF_XBS_XBS_CRS4_PARK_USB_HOST       (0x6)
#define MCF_XBS_XBS_CRS4_PARK_NANDFLASH      (0x7)
#define MCF_XBS_XBS_CRS4_PCTL(x)             (((x)&0x3)<<0x4)
#define MCF_XBS_XBS_CRS4_PCTL_PARK_FIELD     (0)
#define MCF_XBS_XBS_CRS4_PCTL_PARK_ON_LAST   (0x10)
#define MCF_XBS_XBS_CRS4_PCTL_PARK_NO_MASTER (0x20)
#define MCF_XBS_XBS_CRS4_ARB                 (0x100)
#define MCF_XBS_XBS_CRS4_RO                  (0x80000000)

/* Bit definitions and macros for MCF_XBS_XBS_PRS6 */
#define MCF_XBS_XBS_PRS6_M0(x)               (((x)&0x7)<<0)
#define MCF_XBS_XBS_PRS6_M1(x)               (((x)&0x7)<<0x4)
#define MCF_XBS_XBS_PRS6_M2(x)               (((x)&0x7)<<0x8)
#define MCF_XBS_XBS_PRS6_M3(x)               (((x)&0x7)<<0xC)
#define MCF_XBS_XBS_PRS6_M4(x)               (((x)&0x7)<<0x10)
#define MCF_XBS_XBS_PRS6_M5(x)               (((x)&0x7)<<0x14)
#define MCF_XBS_XBS_PRS6_M6(x)               (((x)&0x7)<<0x18)
#define MCF_XBS_XBS_PRS6_M7(x)               (((x)&0x7)<<0x1C)

/* Bit definitions and macros for MCF_XBS_XBS_CRS6 */
#define MCF_XBS_XBS_CRS6_PARK(x)             (((x)&0x7)<<0)
#define MCF_XBS_XBS_CRS6_PARK_CORE           (0)
#define MCF_XBS_XBS_CRS6_PARK_EDMA           (0x1)
#define MCF_XBS_XBS_CRS6_PARK_USB_OTG        (0x2)
#define MCF_XBS_XBS_CRS6_PARK_M3_ETH         (0x3)
#define MCF_XBS_XBS_CRS6_PARK_M4_ETH         (0x4)
#define MCF_XBS_XBS_CRS6_PARK_SDHC           (0x5)
#define MCF_XBS_XBS_CRS6_PARK_USB_HOST       (0x6)
#define MCF_XBS_XBS_CRS6_PARK_NANDFLASH      (0x7)
#define MCF_XBS_XBS_CRS6_PCTL(x)             (((x)&0x3)<<0x4)
#define MCF_XBS_XBS_CRS6_PCTL_PARK_FIELD     (0)
#define MCF_XBS_XBS_CRS6_PCTL_PARK_ON_LAST   (0x10)
#define MCF_XBS_XBS_CRS6_PCTL_PARK_NO_MASTER (0x20)
#define MCF_XBS_XBS_CRS6_ARB                 (0x100)
#define MCF_XBS_XBS_CRS6_RO                  (0x80000000)

/* Bit definitions and macros for MCF_XBS_XBS_PRS7 */
#define MCF_XBS_XBS_PRS7_M0(x)               (((x)&0x7)<<0)
#define MCF_XBS_XBS_PRS7_M1(x)               (((x)&0x7)<<0x4)
#define MCF_XBS_XBS_PRS7_M2(x)               (((x)&0x7)<<0x8)
#define MCF_XBS_XBS_PRS7_M3(x)               (((x)&0x7)<<0xC)
#define MCF_XBS_XBS_PRS7_M4(x)               (((x)&0x7)<<0x10)
#define MCF_XBS_XBS_PRS7_M5(x)               (((x)&0x7)<<0x14)
#define MCF_XBS_XBS_PRS7_M6(x)               (((x)&0x7)<<0x18)
#define MCF_XBS_XBS_PRS7_M7(x)               (((x)&0x7)<<0x1C)

/* Bit definitions and macros for MCF_XBS_XBS_CRS7 */
#define MCF_XBS_XBS_CRS7_PARK(x)             (((x)&0x7)<<0)
#define MCF_XBS_XBS_CRS7_PARK_CORE           (0)
#define MCF_XBS_XBS_CRS7_PARK_EDMA           (0x1)
#define MCF_XBS_XBS_CRS7_PARK_USB_OTG        (0x2)
#define MCF_XBS_XBS_CRS7_PARK_M3_ETH         (0x3)
#define MCF_XBS_XBS_CRS7_PARK_M4_ETH         (0x4)
#define MCF_XBS_XBS_CRS7_PARK_SDHC           (0x5)
#define MCF_XBS_XBS_CRS7_PARK_USB_HOST       (0x6)
#define MCF_XBS_XBS_CRS7_PARK_NANDFLASH      (0x7)
#define MCF_XBS_XBS_CRS7_PCTL(x)             (((x)&0x3)<<0x4)
#define MCF_XBS_XBS_CRS7_PCTL_PARK_FIELD     (0)
#define MCF_XBS_XBS_CRS7_PCTL_PARK_ON_LAST   (0x10)
#define MCF_XBS_XBS_CRS7_PCTL_PARK_NO_MASTER (0x20)
#define MCF_XBS_XBS_CRS7_ARB                 (0x100)
#define MCF_XBS_XBS_CRS7_RO                  (0x80000000)


#endif /* __MCF54418_XBS_H__ */
