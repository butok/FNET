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

#ifndef __MCF54418_NFC_H__
#define __MCF54418_NFC_H__


/*********************************************************************
*
* NAND Flash Controller (NFC)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_NFC_CMD1                         (*(vuint32*)(0xFC0FFF00))
#define MCF_NFC_CMD2                         (*(vuint32*)(0xFC0FFF04))
#define MCF_NFC_CAR                          (*(vuint32*)(0xFC0FFF08))
#define MCF_NFC_RAR                          (*(vuint32*)(0xFC0FFF0C))
#define MCF_NFC_RPT                          (*(vuint32*)(0xFC0FFF10))
#define MCF_NFC_RAI                          (*(vuint32*)(0xFC0FFF14))
#define MCF_NFC_SR1                          (*(vuint32*)(0xFC0FFF18))
#define MCF_NFC_SR2                          (*(vuint32*)(0xFC0FFF1C))
#define MCF_NFC_DMA1                         (*(vuint32*)(0xFC0FFF20))
#define MCF_NFC_DMACFG                       (*(vuint32*)(0xFC0FFF24))
#define MCF_NFC_SWAP                         (*(vuint32*)(0xFC0FFF28))
#define MCF_NFC_SECSZ                        (*(vuint32*)(0xFC0FFF2C))
#define MCF_NFC_CFG                          (*(vuint32*)(0xFC0FFF30))
#define MCF_NFC_DMA2                         (*(vuint32*)(0xFC0FFF34))
#define MCF_NFC_ISR                          (*(vuint32*)(0xFC0FFF38))


/* Bit definitions and macros for MCF_NFC_CMD1 */
#define MCF_NFC_CMD1_BYTE3(x)                (((x)&0xFF)<<0x10)
#define MCF_NFC_CMD1_BYTE2(x)                (((x)&0xFF)<<0x18)

/* Bit definitions and macros for MCF_NFC_CMD2 */
#define MCF_NFC_CMD2_BUSY_START              (0x1)
#define MCF_NFC_CMD2_BUFNO(x)                (((x)&0x3)<<0x1)
#define MCF_NFC_CMD2_CODE(x)                 (((x)&0xFFFF)<<0x8)
#define MCF_NFC_CMD2_BYTE1(x)                (((x)&0xFF)<<0x18)

/* Bit definitions and macros for MCF_NFC_CAR */
#define MCF_NFC_CAR_BYTE1(x)                 (((x)&0xFF)<<0)
#define MCF_NFC_CAR_BYTE2(x)                 (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_NFC_RAR */
#define MCF_NFC_RAR_BYTE1(x)                 (((x)&0xFF)<<0)
#define MCF_NFC_RAR_BYTE2(x)                 (((x)&0xFF)<<0x8)
#define MCF_NFC_RAR_BYTE3(x)                 (((x)&0xFF)<<0x10)
#define MCF_NFC_RAR_RB                       (0x1000000)
#define MCF_NFC_RAR_CS                       (0x10000000)

/* Bit definitions and macros for MCF_NFC_RPT */
#define MCF_NFC_RPT_COUNT(x)                 (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_NFC_RAI */
#define MCF_NFC_RAI_INC1(x)                  (((x)&0xFF)<<0)
#define MCF_NFC_RAI_INC2(x)                  (((x)&0xFF)<<0x8)
#define MCF_NFC_RAI_INC3(x)                  (((x)&0xFF)<<0x10)

/* Bit definitions and macros for MCF_NFC_SR1 */
#define MCF_NFC_SR1_ID4(x)                   (((x)&0xFF)<<0)
#define MCF_NFC_SR1_ID3(x)                   (((x)&0xFF)<<0x8)
#define MCF_NFC_SR1_ID2(x)                   (((x)&0xFF)<<0x10)
#define MCF_NFC_SR1_ID1(x)                   (((x)&0xFF)<<0x18)

/* Bit definitions and macros for MCF_NFC_SR2 */
#define MCF_NFC_SR2_STATUS1(x)               (((x)&0xFF)<<0)
#define MCF_NFC_SR2_ID5(x)                   (((x)&0xFF)<<0x18)

/* Bit definitions and macros for MCF_NFC_DMA1 */
#define MCF_NFC_DMA1_ADDRESS(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_NFC_DMACFG */
#define MCF_NFC_DMACFG_ACT2                  (0x1)
#define MCF_NFC_DMACFG_ACT1                  (0x2)
#define MCF_NFC_DMACFG_OFFSET2(x)            (((x)&0x7FF)<<0x2)
#define MCF_NFC_DMACFG_COUNT2(x)             (((x)&0x7F)<<0xD)
#define MCF_NFC_DMACFG_COUNT1(x)             (((x)&0xFFF)<<0x14)

/* Bit definitions and macros for MCF_NFC_SWAP */
#define MCF_NFC_SWAP_ADDR1(x)                (((x)&0x7FF)<<0x1)
#define MCF_NFC_SWAP_ADDR2(x)                (((x)&0x7FF)<<0x11)

/* Bit definitions and macros for MCF_NFC_SECSZ */
#define MCF_NFC_SECSZ_SIZE(x)                (((x)&0x1FFF)<<0)

/* Bit definitions and macros for MCF_NFC_CFG */
#define MCF_NFC_CFG_PAGECNT(x)               (((x)&0xF)<<0)
#define MCF_NFC_CFG_AIBN                     (0x10)
#define MCF_NFC_CFG_AIAD                     (0x20)
#define MCF_NFC_CFG_BTMD                     (0x40)
#define MCF_NFC_CFG_16BIT                    (0x80)
#define MCF_NFC_CFG_TIMEOUT(x)               (((x)&0x1F)<<0x8)
#define MCF_NFC_CFG_IDCNT(x)                 (((x)&0x7)<<0xD)
#define MCF_NFC_CFG_FAST                     (0x10000)
#define MCF_NFC_CFG_ECCMODE(x)               (((x)&0x7)<<0x11)
#define MCF_NFC_CFG_DMAREQ                   (0x100000)
#define MCF_NFC_CFG_ECCSRAM                  (0x200000)
#define MCF_NFC_CFG_ECCAD(x)                 (((x)&0x1FF)<<0x16)
#define MCF_NFC_CFG_STOPWERR                 (0x80000000)

/* Bit definitions and macros for MCF_NFC_DMA2 */
#define MCF_NFC_DMA2_ADDRESS(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_NFC_ISR */
#define MCF_NFC_ISR_DMABN(x)                 (((x)&0x3)<<0)
#define MCF_NFC_ISR_ECCBN(x)                 (((x)&0x3)<<0x2)
#define MCF_NFC_ISR_RESBN(x)                 (((x)&0x3)<<0x4)
#define MCF_NFC_ISR_IDLECLR                  (0x20000)
#define MCF_NFC_ISR_DONECLR                  (0x40000)
#define MCF_NFC_ISR_WERRCLR                  (0x80000)
#define MCF_NFC_ISR_IDLEEN                   (0x100000)
#define MCF_NFC_ISR_DONEEN                   (0x200000)
#define MCF_NFC_ISR_WERREN                   (0x400000)
#define MCF_NFC_ISR_DMABUSY                  (0x800000)
#define MCF_NFC_ISR_ECCBUSY                  (0x1000000)
#define MCF_NFC_ISR_RESBUSY                  (0x2000000)
#define MCF_NFC_ISR_CMDBUSY                  (0x4000000)
#define MCF_NFC_ISR_WERRNS                   (0x8000000)
#define MCF_NFC_ISR_IDLE                     (0x20000000)
#define MCF_NFC_ISR_DONE                     (0x40000000)
#define MCF_NFC_ISR_WERR                     (0x80000000)


#endif /* __MCF54418_NFC_H__ */
