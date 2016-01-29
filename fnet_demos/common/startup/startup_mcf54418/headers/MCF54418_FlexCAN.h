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

#ifndef __MCF54418_FlexCAN_H__
#define __MCF54418_FlexCAN_H__


/*********************************************************************
*
* Flex Controller Area Network (FlexCAN)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_FlexCAN0_CANMCR                  (*(vuint32*)(0xFC020000))
#define MCF_FlexCAN0_CANCTRL                 (*(vuint32*)(0xFC020004))
#define MCF_FlexCAN0_TIMER                   (*(vuint32*)(0xFC020008))
#define MCF_FlexCAN0_RXGMASK                 (*(vuint32*)(0xFC020010))
#define MCF_FlexCAN0_RX14MASK                (*(vuint32*)(0xFC020014))
#define MCF_FlexCAN0_RX15MASK                (*(vuint32*)(0xFC020018))
#define MCF_FlexCAN0_ERRCNT                  (*(vuint32*)(0xFC02001C))
#define MCF_FlexCAN0_ERRSTAT                 (*(vuint32*)(0xFC020020))
#define MCF_FlexCAN0_IMASK                   (*(vuint32*)(0xFC020028))
#define MCF_FlexCAN0_IFLAG                   (*(vuint32*)(0xFC020030))
#define MCF_FlexCAN0_RXIMR0                  (*(vuint32*)(0xFC020880))
#define MCF_FlexCAN0_RXIMR1                  (*(vuint32*)(0xFC020884))
#define MCF_FlexCAN0_RXIMR2                  (*(vuint32*)(0xFC020888))
#define MCF_FlexCAN0_RXIMR3                  (*(vuint32*)(0xFC02088C))
#define MCF_FlexCAN0_RXIMR4                  (*(vuint32*)(0xFC020890))
#define MCF_FlexCAN0_RXIMR5                  (*(vuint32*)(0xFC020894))
#define MCF_FlexCAN0_RXIMR6                  (*(vuint32*)(0xFC020898))
#define MCF_FlexCAN0_RXIMR7                  (*(vuint32*)(0xFC02089C))
#define MCF_FlexCAN0_RXIMR8                  (*(vuint32*)(0xFC0208A0))
#define MCF_FlexCAN0_RXIMR9                  (*(vuint32*)(0xFC0208A4))
#define MCF_FlexCAN0_RXIMR10                 (*(vuint32*)(0xFC0208A8))
#define MCF_FlexCAN0_RXIMR11                 (*(vuint32*)(0xFC0208AC))
#define MCF_FlexCAN0_RXIMR12                 (*(vuint32*)(0xFC0208B0))
#define MCF_FlexCAN0_RXIMR13                 (*(vuint32*)(0xFC0208B4))
#define MCF_FlexCAN0_RXIMR14                 (*(vuint32*)(0xFC0208B8))
#define MCF_FlexCAN0_RXIMR15                 (*(vuint32*)(0xFC0208BC))

#define MCF_FlexCAN1_CANMCR                  (*(vuint32*)(0xFC024000))
#define MCF_FlexCAN1_CANCTRL                 (*(vuint32*)(0xFC024004))
#define MCF_FlexCAN1_TIMER                   (*(vuint32*)(0xFC024008))
#define MCF_FlexCAN1_RXGMASK                 (*(vuint32*)(0xFC024010))
#define MCF_FlexCAN1_RX14MASK                (*(vuint32*)(0xFC024014))
#define MCF_FlexCAN1_RX15MASK                (*(vuint32*)(0xFC024018))
#define MCF_FlexCAN1_ERRCNT                  (*(vuint32*)(0xFC02401C))
#define MCF_FlexCAN1_ERRSTAT                 (*(vuint32*)(0xFC024020))
#define MCF_FlexCAN1_IMASK                   (*(vuint32*)(0xFC024028))
#define MCF_FlexCAN1_IFLAG                   (*(vuint32*)(0xFC024030))
#define MCF_FlexCAN1_RXIMR0                  (*(vuint32*)(0xFC024880))
#define MCF_FlexCAN1_RXIMR1                  (*(vuint32*)(0xFC024884))
#define MCF_FlexCAN1_RXIMR2                  (*(vuint32*)(0xFC024888))
#define MCF_FlexCAN1_RXIMR3                  (*(vuint32*)(0xFC02488C))
#define MCF_FlexCAN1_RXIMR4                  (*(vuint32*)(0xFC024890))
#define MCF_FlexCAN1_RXIMR5                  (*(vuint32*)(0xFC024894))
#define MCF_FlexCAN1_RXIMR6                  (*(vuint32*)(0xFC024898))
#define MCF_FlexCAN1_RXIMR7                  (*(vuint32*)(0xFC02489C))
#define MCF_FlexCAN1_RXIMR8                  (*(vuint32*)(0xFC0248A0))
#define MCF_FlexCAN1_RXIMR9                  (*(vuint32*)(0xFC0248A4))
#define MCF_FlexCAN1_RXIMR10                 (*(vuint32*)(0xFC0248A8))
#define MCF_FlexCAN1_RXIMR11                 (*(vuint32*)(0xFC0248AC))
#define MCF_FlexCAN1_RXIMR12                 (*(vuint32*)(0xFC0248B0))
#define MCF_FlexCAN1_RXIMR13                 (*(vuint32*)(0xFC0248B4))
#define MCF_FlexCAN1_RXIMR14                 (*(vuint32*)(0xFC0248B8))
#define MCF_FlexCAN1_RXIMR15                 (*(vuint32*)(0xFC0248BC))

#define MCF_FlexCAN_CANMCR(x)                (*(vuint32*)(0xFC020000 + ((x)*0x4000)))
#define MCF_FlexCAN_CANCTRL(x)               (*(vuint32*)(0xFC020004 + ((x)*0x4000)))
#define MCF_FlexCAN_TIMER(x)                 (*(vuint32*)(0xFC020008 + ((x)*0x4000)))
#define MCF_FlexCAN_RXGMASK(x)               (*(vuint32*)(0xFC020010 + ((x)*0x4000)))
#define MCF_FlexCAN_RX14MASK(x)              (*(vuint32*)(0xFC020014 + ((x)*0x4000)))
#define MCF_FlexCAN_RX15MASK(x)              (*(vuint32*)(0xFC020018 + ((x)*0x4000)))
#define MCF_FlexCAN_ERRCNT(x)                (*(vuint32*)(0xFC02001C + ((x)*0x4000)))
#define MCF_FlexCAN_ERRSTAT(x)               (*(vuint32*)(0xFC020020 + ((x)*0x4000)))
#define MCF_FlexCAN_IMASK(x)                 (*(vuint32*)(0xFC020028 + ((x)*0x4000)))
#define MCF_FlexCAN_IFLAG(x)                 (*(vuint32*)(0xFC020030 + ((x)*0x4000)))
#define MCF_FlexCAN_RXIMR0(x)                (*(vuint32*)(0xFC020880 + ((x)*0x4000)))
#define MCF_FlexCAN_RXIMR1(x)                (*(vuint32*)(0xFC020884 + ((x)*0x4000)))
#define MCF_FlexCAN_RXIMR2(x)                (*(vuint32*)(0xFC020888 + ((x)*0x4000)))
#define MCF_FlexCAN_RXIMR3(x)                (*(vuint32*)(0xFC02088C + ((x)*0x4000)))
#define MCF_FlexCAN_RXIMR4(x)                (*(vuint32*)(0xFC020890 + ((x)*0x4000)))
#define MCF_FlexCAN_RXIMR5(x)                (*(vuint32*)(0xFC020894 + ((x)*0x4000)))
#define MCF_FlexCAN_RXIMR6(x)                (*(vuint32*)(0xFC020898 + ((x)*0x4000)))
#define MCF_FlexCAN_RXIMR7(x)                (*(vuint32*)(0xFC02089C + ((x)*0x4000)))
#define MCF_FlexCAN_RXIMR8(x)                (*(vuint32*)(0xFC0208A0 + ((x)*0x4000)))
#define MCF_FlexCAN_RXIMR9(x)                (*(vuint32*)(0xFC0208A4 + ((x)*0x4000)))
#define MCF_FlexCAN_RXIMR10(x)               (*(vuint32*)(0xFC0208A8 + ((x)*0x4000)))
#define MCF_FlexCAN_RXIMR11(x)               (*(vuint32*)(0xFC0208AC + ((x)*0x4000)))
#define MCF_FlexCAN_RXIMR12(x)               (*(vuint32*)(0xFC0208B0 + ((x)*0x4000)))
#define MCF_FlexCAN_RXIMR13(x)               (*(vuint32*)(0xFC0208B4 + ((x)*0x4000)))
#define MCF_FlexCAN_RXIMR14(x)               (*(vuint32*)(0xFC0208B8 + ((x)*0x4000)))
#define MCF_FlexCAN_RXIMR15(x)               (*(vuint32*)(0xFC0208BC + ((x)*0x4000)))


/* Bit definitions and macros for MCF_FlexCAN_CANMCR */
#define MCF_FlexCAN_CANMCR_MAXMB(x)          (((x)&0xF)<<0)
#define MCF_FlexCAN_CANMCR_BCC               (0x10000)
#define MCF_FlexCAN_CANMCR_LPMACK            (0x100000)
#define MCF_FlexCAN_CANMCR_SUPV              (0x800000)
#define MCF_FlexCAN_CANMCR_FRZACK            (0x1000000)
#define MCF_FlexCAN_CANMCR_SOFTRST           (0x2000000)
#define MCF_FlexCAN_CANMCR_NOTRDY            (0x8000000)
#define MCF_FlexCAN_CANMCR_HALT              (0x10000000)
#define MCF_FlexCAN_CANMCR_FRZ               (0x40000000)
#define MCF_FlexCAN_CANMCR_MDIS              (0x80000000)

/* Bit definitions and macros for MCF_FlexCAN_CANCTRL */
#define MCF_FlexCAN_CANCTRL_PROPSEG(x)       (((x)&0x7)<<0)
#define MCF_FlexCAN_CANCTRL_LOM              (0x8)
#define MCF_FlexCAN_CANCTRL_LBUF             (0x10)
#define MCF_FlexCAN_CANCTRL_TSYNC            (0x20)
#define MCF_FlexCAN_CANCTRL_BOFFREC          (0x40)
#define MCF_FlexCAN_CANCTRL_SAMP             (0x80)
#define MCF_FlexCAN_CANCTRL_LPB              (0x1000)
#define MCF_FlexCAN_CANCTRL_CLK_SRC          (0x2000)
#define MCF_FlexCAN_CANCTRL_ERRMSK           (0x4000)
#define MCF_FlexCAN_CANCTRL_BOFFMSK          (0x8000)
#define MCF_FlexCAN_CANCTRL_PSEG2(x)         (((x)&0x7)<<0x10)
#define MCF_FlexCAN_CANCTRL_PSEG1(x)         (((x)&0x7)<<0x13)
#define MCF_FlexCAN_CANCTRL_RJW(x)           (((x)&0x3)<<0x16)
#define MCF_FlexCAN_CANCTRL_PRESDIV(x)       (((x)&0xFF)<<0x18)

/* Bit definitions and macros for MCF_FlexCAN_TIMER */
#define MCF_FlexCAN_TIMER_TIMER(x)           (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_RXGMASK */
#define MCF_FlexCAN_RXGMASK_MI(x)            (((x)&0x1FFFFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_RX14MASK */
#define MCF_FlexCAN_RX14MASK_MI(x)           (((x)&0x1FFFFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_RX15MASK */
#define MCF_FlexCAN_RX15MASK_MI(x)           (((x)&0x1FFFFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_ERRCNT */
#define MCF_FlexCAN_ERRCNT_TXECTR(x)         (((x)&0xFF)<<0)
#define MCF_FlexCAN_ERRCNT_RXECTR(x)         (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_FlexCAN_ERRSTAT */
#define MCF_FlexCAN_ERRSTAT_ERRINT           (0x2)
#define MCF_FlexCAN_ERRSTAT_BOFFINT          (0x4)
#define MCF_FlexCAN_ERRSTAT_FLTCONF(x)       (((x)&0x3)<<0x4)
#define MCF_FlexCAN_ERRSTAT_FLTCONF_ACTIVE   (0)
#define MCF_FlexCAN_ERRSTAT_FLTCONF_PASSIVE  (0x10)
#define MCF_FlexCAN_ERRSTAT_FLTCONF_BUSOFF   (0x20)
#define MCF_FlexCAN_ERRSTAT_TXRX             (0x40)
#define MCF_FlexCAN_ERRSTAT_IDLE             (0x80)
#define MCF_FlexCAN_ERRSTAT_RXWRN            (0x100)
#define MCF_FlexCAN_ERRSTAT_TXWRN            (0x200)
#define MCF_FlexCAN_ERRSTAT_STFERR           (0x400)
#define MCF_FlexCAN_ERRSTAT_FRMERR           (0x800)
#define MCF_FlexCAN_ERRSTAT_CRCERR           (0x1000)
#define MCF_FlexCAN_ERRSTAT_ACKERR           (0x2000)
#define MCF_FlexCAN_ERRSTAT_BIT0ERR          (0x4000)
#define MCF_FlexCAN_ERRSTAT_BIT1ERR          (0x8000)

/* Bit definitions and macros for MCF_FlexCAN_IMASK */
#define MCF_FlexCAN_IMASK_BUF0M              (0x1)
#define MCF_FlexCAN_IMASK_BUF1M              (0x2)
#define MCF_FlexCAN_IMASK_BUF2M              (0x4)
#define MCF_FlexCAN_IMASK_BUF3M              (0x8)
#define MCF_FlexCAN_IMASK_BUF4M              (0x10)
#define MCF_FlexCAN_IMASK_BUF5M              (0x20)
#define MCF_FlexCAN_IMASK_BUF6M              (0x40)
#define MCF_FlexCAN_IMASK_BUF7M              (0x80)
#define MCF_FlexCAN_IMASK_BUF8M              (0x100)
#define MCF_FlexCAN_IMASK_BUF9M              (0x200)
#define MCF_FlexCAN_IMASK_BUF10M             (0x400)
#define MCF_FlexCAN_IMASK_BUF11M             (0x800)
#define MCF_FlexCAN_IMASK_BUF12M             (0x1000)
#define MCF_FlexCAN_IMASK_BUF13M             (0x2000)
#define MCF_FlexCAN_IMASK_BUF14M             (0x4000)
#define MCF_FlexCAN_IMASK_BUF15M             (0x8000)
#define MCF_FlexCAN_IMASK_BUF(x)             (0x1<<(x))

/* Bit definitions and macros for MCF_FlexCAN_IFLAG */
#define MCF_FlexCAN_IFLAG_BUF0I              (0x1)
#define MCF_FlexCAN_IFLAG_BUF1I              (0x2)
#define MCF_FlexCAN_IFLAG_BUF2I              (0x4)
#define MCF_FlexCAN_IFLAG_BUF3I              (0x8)
#define MCF_FlexCAN_IFLAG_BUF4I              (0x10)
#define MCF_FlexCAN_IFLAG_BUF5I              (0x20)
#define MCF_FlexCAN_IFLAG_BUF6I              (0x40)
#define MCF_FlexCAN_IFLAG_BUF7I              (0x80)
#define MCF_FlexCAN_IFLAG_BUF8I              (0x100)
#define MCF_FlexCAN_IFLAG_BUF9I              (0x200)
#define MCF_FlexCAN_IFLAG_BUF10I             (0x400)
#define MCF_FlexCAN_IFLAG_BUF11I             (0x800)
#define MCF_FlexCAN_IFLAG_BUF12I             (0x1000)
#define MCF_FlexCAN_IFLAG_BUF13I             (0x2000)
#define MCF_FlexCAN_IFLAG_BUF14I             (0x4000)
#define MCF_FlexCAN_IFLAG_BUF15I             (0x8000)
#define MCF_FlexCAN_IFLAG_BUF(x)             (0x1<<(x))

/* Bit definitions and macros for MCF_FlexCAN_RXIMR0 */
#define MCF_FlexCAN_RXIMR0_MI(x)             (((x)&0x1FFFFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_RXIMR1 */
#define MCF_FlexCAN_RXIMR1_MI(x)             (((x)&0x1FFFFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_RXIMR2 */
#define MCF_FlexCAN_RXIMR2_MI(x)             (((x)&0x1FFFFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_RXIMR3 */
#define MCF_FlexCAN_RXIMR3_MI(x)             (((x)&0x1FFFFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_RXIMR4 */
#define MCF_FlexCAN_RXIMR4_MI(x)             (((x)&0x1FFFFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_RXIMR5 */
#define MCF_FlexCAN_RXIMR5_MI(x)             (((x)&0x1FFFFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_RXIMR6 */
#define MCF_FlexCAN_RXIMR6_MI(x)             (((x)&0x1FFFFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_RXIMR7 */
#define MCF_FlexCAN_RXIMR7_MI(x)             (((x)&0x1FFFFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_RXIMR8 */
#define MCF_FlexCAN_RXIMR8_MI(x)             (((x)&0x1FFFFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_RXIMR9 */
#define MCF_FlexCAN_RXIMR9_MI(x)             (((x)&0x1FFFFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_RXIMR10 */
#define MCF_FlexCAN_RXIMR10_MI(x)            (((x)&0x1FFFFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_RXIMR11 */
#define MCF_FlexCAN_RXIMR11_MI(x)            (((x)&0x1FFFFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_RXIMR12 */
#define MCF_FlexCAN_RXIMR12_MI(x)            (((x)&0x1FFFFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_RXIMR13 */
#define MCF_FlexCAN_RXIMR13_MI(x)            (((x)&0x1FFFFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_RXIMR14 */
#define MCF_FlexCAN_RXIMR14_MI(x)            (((x)&0x1FFFFFFF)<<0)

/* Bit definitions and macros for MCF_FlexCAN_RXIMR15 */
#define MCF_FlexCAN_RXIMR15_MI(x)            (((x)&0x1FFFFFFF)<<0)


#endif /* __MCF54418_FlexCAN_H__ */
