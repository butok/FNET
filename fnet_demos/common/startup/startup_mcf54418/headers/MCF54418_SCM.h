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

#ifndef __MCF54418_SCM_H__
#define __MCF54418_SCM_H__


/*********************************************************************
*
* System Control Module (SCM)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_SCM_CWCR                         (*(vuint16*)(0xFC040016))
#define MCF_SCM_CWSR                         (*(vuint8 *)(0xFC04001B))
#define MCF_SCM_SCMISR                       (*(vuint8 *)(0xFC04001F))
#define MCF_SCM_BCR                          (*(vuint32*)(0xFC040024))
#define MCF_SCM_CFADR                        (*(vuint32*)(0xFC040070))
#define MCF_SCM_CFIER                        (*(vuint8 *)(0xFC040075))
#define MCF_SCM_CFLOC                        (*(vuint8 *)(0xFC040076))
#define MCF_SCM_CFATR                        (*(vuint8 *)(0xFC040077))
#define MCF_SCM_CFDTR                        (*(vuint32*)(0xFC04007C))


/* Bit definitions and macros for MCF_SCM_CWCR */
#define MCF_SCM_CWCR_CWT(x)                  (((x)&0x1F)<<0)
#define MCF_SCM_CWCR_CWRI(x)                 (((x)&0x3)<<0x5)
#define MCF_SCM_CWCR_CWRI_INT                (0)
#define MCF_SCM_CWCR_CWRI_INT_THEN_RESET     (0x20)
#define MCF_SCM_CWCR_CWRI_RESET              (0x40)
#define MCF_SCM_CWCR_CWRI_WINDOW             (0x60)
#define MCF_SCM_CWCR_CWE                     (0x80)
#define MCF_SCM_CWCR_CWRWH                   (0x100)
#define MCF_SCM_CWCR_RO                      (0x8000)

/* Bit definitions and macros for MCF_SCM_CWSR */
#define MCF_SCM_CWSR_CWSR(x)                 (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_SCM_SCMISR */
#define MCF_SCM_SCMISR_CWIC                  (0x1)
#define MCF_SCM_SCMISR_CFEI                  (0x2)

/* Bit definitions and macros for MCF_SCM_BCR */
#define MCF_SCM_BCR_SBE(x)                   (((x)&0xFF)<<0)
#define MCF_SCM_BCR_SBE_BURST_DIS            (0)
#define MCF_SCM_BCR_SBE_BURST_EN             (0xFF)
#define MCF_SCM_BCR_GBW                      (0x100)
#define MCF_SCM_BCR_GBR                      (0x200)

/* Bit definitions and macros for MCF_SCM_CFADR */
#define MCF_SCM_CFADR_ADDR(x)                (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_SCM_CFIER */
#define MCF_SCM_CFIER_ECFEI                  (0x1)

/* Bit definitions and macros for MCF_SCM_CFLOC */
#define MCF_SCM_CFLOC_LOC                    (0x80)

/* Bit definitions and macros for MCF_SCM_CFATR */
#define MCF_SCM_CFATR_TYPE                   (0x1)
#define MCF_SCM_CFATR_MODE                   (0x2)
#define MCF_SCM_CFATR_CACHE                  (0x8)
#define MCF_SCM_CFATR_SIZE(x)                (((x)&0x7)<<0x4)
#define MCF_SCM_CFATR_WRITE                  (0x80)

/* Bit definitions and macros for MCF_SCM_CFDTR */
#define MCF_SCM_CFDTR_CFDTR(x)               (((x)&0xFFFFFFFF)<<0)


#endif /* __MCF54418_SCM_H__ */
