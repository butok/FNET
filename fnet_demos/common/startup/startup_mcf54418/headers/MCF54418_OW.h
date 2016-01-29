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

#ifndef __MCF54418_OW_H__
#define __MCF54418_OW_H__


/*********************************************************************
*
* 1-Wire Module (OneWire)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_OW_CR                            (*(vuint8 *)(0xEC008000))
#define MCF_OW_DIV                           (*(vuint8 *)(0xEC008004))
#define MCF_OW_RST                           (*(vuint8 *)(0xEC008008))
#define MCF_OW_CMD                           (*(vuint8 *)(0xEC00800C))
#define MCF_OW_TXRX                          (*(vuint8 *)(0xEC008010))
#define MCF_OW_ISR                           (*(vuint8 *)(0xEC008014))
#define MCF_OW_IER                           (*(vuint8 *)(0xEC008018))



/* Bit definitions and macros for MCF_OW_CR */
#define MCF_OW_CR_DMAEN                      (0x1)
#define MCF_OW_CR_RDST                       (0x8)
#define MCF_OW_CR_WR1                        (0x10)
#define MCF_OW_CR_WR0                        (0x20)
#define MCF_OW_CR_PST                        (0x40)
#define MCF_OW_CR_RPP                        (0x80)

/* Bit definitions and macros for MCF_OW_DIV */
#define MCF_OW_DIV_DVDR(x)                   (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_OW_RST */
#define MCF_OW_RST_RST                       (0x1)

/* Bit definitions and macros for MCF_OW_CMD */
#define MCF_OW_CMD_SRA                       (0x2)

/* Bit definitions and macros for MCF_OW_TXRX */
#define MCF_OW_TXRX_DATA(x)                  (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_OW_ISR */
#define MCF_OW_ISR_PD                        (0x1)
#define MCF_OW_ISR_PDR                       (0x2)
#define MCF_OW_ISR_TBE                       (0x4)
#define MCF_OW_ISR_TSRE                      (0x8)
#define MCF_OW_ISR_RBF                       (0x10)
#define MCF_OW_ISR_RSRF                      (0x20)

/* Bit definitions and macros for MCF_OW_IER */
#define MCF_OW_IER_EPD                       (0x1)
#define MCF_OW_IER_IAS                       (0x2)
#define MCF_OW_IER_ETBE                      (0x4)
#define MCF_OW_IER_ETSE                      (0x8)
#define MCF_OW_IER_ERBF                      (0x10)
#define MCF_OW_IER_ERSF                      (0x20)


#endif /* __MCF54418_OW_H__ */
