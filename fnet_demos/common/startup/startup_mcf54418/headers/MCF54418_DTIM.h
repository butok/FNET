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

#ifndef __MCF54418_DTIM_H__
#define __MCF54418_DTIM_H__


/*********************************************************************
*
* DMA Timers (DTIM)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_DTIM0_DTMR                       (*(vuint16*)(0xFC070000))
#define MCF_DTIM0_DTXMR                      (*(vuint8 *)(0xFC070002))
#define MCF_DTIM0_DTER                       (*(vuint8 *)(0xFC070003))
#define MCF_DTIM0_DTRR                       (*(vuint32*)(0xFC070004))
#define MCF_DTIM0_DTCR                       (*(vuint32*)(0xFC070008))
#define MCF_DTIM0_DTCN                       (*(vuint32*)(0xFC07000C))

#define MCF_DTIM1_DTMR                       (*(vuint16*)(0xFC074000))
#define MCF_DTIM1_DTXMR                      (*(vuint8 *)(0xFC074002))
#define MCF_DTIM1_DTER                       (*(vuint8 *)(0xFC074003))
#define MCF_DTIM1_DTRR                       (*(vuint32*)(0xFC074004))
#define MCF_DTIM1_DTCR                       (*(vuint32*)(0xFC074008))
#define MCF_DTIM1_DTCN                       (*(vuint32*)(0xFC07400C))

#define MCF_DTIM2_DTMR                       (*(vuint16*)(0xFC078000))
#define MCF_DTIM2_DTXMR                      (*(vuint8 *)(0xFC078002))
#define MCF_DTIM2_DTER                       (*(vuint8 *)(0xFC078003))
#define MCF_DTIM2_DTRR                       (*(vuint32*)(0xFC078004))
#define MCF_DTIM2_DTCR                       (*(vuint32*)(0xFC078008))
#define MCF_DTIM2_DTCN                       (*(vuint32*)(0xFC07800C))

#define MCF_DTIM3_DTMR                       (*(vuint16*)(0xFC07C000))
#define MCF_DTIM3_DTXMR                      (*(vuint8 *)(0xFC07C002))
#define MCF_DTIM3_DTER                       (*(vuint8 *)(0xFC07C003))
#define MCF_DTIM3_DTRR                       (*(vuint32*)(0xFC07C004))
#define MCF_DTIM3_DTCR                       (*(vuint32*)(0xFC07C008))
#define MCF_DTIM3_DTCN                       (*(vuint32*)(0xFC07C00C))

#define MCF_DTIM_DTMR(x)                     (*(vuint16*)(0xFC070000 + ((x)*0x4000)))
#define MCF_DTIM_DTXMR(x)                    (*(vuint8 *)(0xFC070002 + ((x)*0x4000)))
#define MCF_DTIM_DTER(x)                     (*(vuint8 *)(0xFC070003 + ((x)*0x4000)))
#define MCF_DTIM_DTRR(x)                     (*(vuint32*)(0xFC070004 + ((x)*0x4000)))
#define MCF_DTIM_DTCR(x)                     (*(vuint32*)(0xFC070008 + ((x)*0x4000)))
#define MCF_DTIM_DTCN(x)                     (*(vuint32*)(0xFC07000C + ((x)*0x4000)))


/* Bit definitions and macros for MCF_DTIM_DTMR */
#define MCF_DTIM_DTMR_RST                    (0x1)
#define MCF_DTIM_DTMR_CLK(x)                 (((x)&0x3)<<0x1)
#define MCF_DTIM_DTMR_CLK_STOP               (0)
#define MCF_DTIM_DTMR_CLK_DIV1               (0x2)
#define MCF_DTIM_DTMR_CLK_DIV16              (0x4)
#define MCF_DTIM_DTMR_CLK_DTIN               (0x6)
#define MCF_DTIM_DTMR_FRR                    (0x8)
#define MCF_DTIM_DTMR_ORRI                   (0x10)
#define MCF_DTIM_DTMR_OM                     (0x20)
#define MCF_DTIM_DTMR_CE(x)                  (((x)&0x3)<<0x6)
#define MCF_DTIM_DTMR_CE_NONE                (0)
#define MCF_DTIM_DTMR_CE_RISE                (0x40)
#define MCF_DTIM_DTMR_CE_FALL                (0x80)
#define MCF_DTIM_DTMR_CE_ANY                 (0xC0)
#define MCF_DTIM_DTMR_PS(x)                  (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_DTIM_DTXMR */
#define MCF_DTIM_DTXMR_MODE16                (0x1)
#define MCF_DTIM_DTXMR_EPD                   (0x10)
#define MCF_DTIM_DTXMR_EN1588                (0x20)
#define MCF_DTIM_DTXMR_HALTED                (0x40)
#define MCF_DTIM_DTXMR_DMAEN                 (0x80)

/* Bit definitions and macros for MCF_DTIM_DTER */
#define MCF_DTIM_DTER_CAP                    (0x1)
#define MCF_DTIM_DTER_REF                    (0x2)

/* Bit definitions and macros for MCF_DTIM_DTRR */
#define MCF_DTIM_DTRR_REF(x)                 (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_DTIM_DTCR */
#define MCF_DTIM_DTCR_CAP(x)                 (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_DTIM_DTCN */
#define MCF_DTIM_DTCN_CNT(x)                 (((x)&0xFFFFFFFF)<<0)


#endif /* __MCF54418_DTIM_H__ */
