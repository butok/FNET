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

#ifndef __MCF54418_USB_HOST_H__
#define __MCF54418_USB_HOST_H__


/*********************************************************************
*
* Universal Serial Bus Interface (USB_HOST)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_USB_HOST_ID                      (*(vuint32*)(0xFC0B4000))
#define MCF_USB_HOST_HWGENERAL               (*(vuint32*)(0xFC0B4004))
#define MCF_USB_HOST_HWHOST                  (*(vuint32*)(0xFC0B4008))
#define MCF_USB_HOST_HWTXBUF                 (*(vuint32*)(0xFC0B4010))
#define MCF_USB_HOST_HWRXBUF                 (*(vuint32*)(0xFC0B4014))
#define MCF_USB_HOST_HCIVERSION              (*(vuint16*)(0xFC0B4100))
#define MCF_USB_HOST_CAPLENGTH               (*(vuint8 *)(0xFC0B4103))
#define MCF_USB_HOST_HCSPARAMS               (*(vuint32*)(0xFC0B4104))
#define MCF_USB_HOST_HCCPARAMS               (*(vuint32*)(0xFC0B4108))
#define MCF_USB_HOST_USBCMD                  (*(vuint32*)(0xFC0B4140))
#define MCF_USB_HOST_USBSTS                  (*(vuint32*)(0xFC0B4144))
#define MCF_USB_HOST_USBINTR                 (*(vuint32*)(0xFC0B4148))
#define MCF_USB_HOST_FRINDEX                 (*(vuint32*)(0xFC0B414C))
#define MCF_USB_HOST_PERIODICLISTBASE        (*(vuint32*)(0xFC0B4154))
#define MCF_USB_HOST_ASYNCLISTADDR           (*(vuint32*)(0xFC0B4158))
#define MCF_USB_HOST_TTCTRL                  (*(vuint32*)(0xFC0B415C))
#define MCF_USB_HOST_BURSTSIZE               (*(vuint32*)(0xFC0B4160))
#define MCF_USB_HOST_TXFILLTUNING            (*(vuint32*)(0xFC0B4164))
#define MCF_USB_HOST_CONFIGFLAG              (*(vuint32*)(0xFC0B4180))
#define MCF_USB_HOST_PORTSC1                 (*(vuint32*)(0xFC0B4184))
#define MCF_USB_HOST_USBMODE                 (*(vuint32*)(0xFC0B41A8))


/* Bit definitions and macros for MCF_USB_HOST_ID */
#define MCF_USB_HOST_ID_RESERVED             (0xC000)
#define MCF_USB_HOST_ID_ID(x)                (((x)&0x3F)<<0|0xC000)
#define MCF_USB_HOST_ID_NID(x)               (((x)&0x3F)<<0x8|0xC000)
#define MCF_USB_HOST_ID_REVISION(x)          (((x)&0xFF)<<0x10|0xC000)

/* Bit definitions and macros for MCF_USB_HOST_HWGENERAL */
#define MCF_USB_HOST_HWGENERAL_PHYW(x)       (((x)&0x3)<<0x4)
#define MCF_USB_HOST_HWGENERAL_PHYM(x)       (((x)&0x7)<<0x6)
#define MCF_USB_HOST_HWGENERAL_SM(x)         (((x)&0x3)<<0x9)

/* Bit definitions and macros for MCF_USB_HOST_HWHOST */
#define MCF_USB_HOST_HWHOST_HC               (0x1)
#define MCF_USB_HOST_HWHOST_NPORT(x)         (((x)&0x7)<<0x1)
#define MCF_USB_HOST_HWHOST_TTASY(x)         (((x)&0xFF)<<0x10)
#define MCF_USB_HOST_HWHOST_TTPER(x)         (((x)&0xFF)<<0x18)

/* Bit definitions and macros for MCF_USB_HOST_HWTXBUF */
#define MCF_USB_HOST_HWTXBUF_TXBURST(x)      (((x)&0xFF)<<0)
#define MCF_USB_HOST_HWTXBUF_TXADD(x)        (((x)&0xFF)<<0x8)
#define MCF_USB_HOST_HWTXBUF_TXCHANADD(x)    (((x)&0xFF)<<0x10)
#define MCF_USB_HOST_HWTXBUF_TXLC            (0x80000000)

/* Bit definitions and macros for MCF_USB_HOST_HWRXBUF */
#define MCF_USB_HOST_HWRXBUF_RXBURST(x)      (((x)&0xFF)<<0)
#define MCF_USB_HOST_HWRXBUF_RXADD(x)        (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_USB_HOST_HCIVERSION */
#define MCF_USB_HOST_HCIVERSION_HCIVERSION(x) (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_USB_HOST_CAPLENGTH */
#define MCF_USB_HOST_CAPLENGTH_CAPLENGTH(x)  (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_USB_HOST_HCSPARAMS */
#define MCF_USB_HOST_HCSPARAMS_N_PORTS(x)    (((x)&0xF)<<0)
#define MCF_USB_HOST_HCSPARAMS_PPC           (0x10)
#define MCF_USB_HOST_HCSPARAMS_N_PCC(x)      (((x)&0xF)<<0x8)
#define MCF_USB_HOST_HCSPARAMS_N_CC(x)       (((x)&0xF)<<0xC)
#define MCF_USB_HOST_HCSPARAMS_PI            (0x10000)
#define MCF_USB_HOST_HCSPARAMS_N_PTT(x)      (((x)&0xF)<<0x14)
#define MCF_USB_HOST_HCSPARAMS_N_TT(x)       (((x)&0xF)<<0x18)

/* Bit definitions and macros for MCF_USB_HOST_HCCPARAMS */
#define MCF_USB_HOST_HCCPARAMS_ADC           (0x1)
#define MCF_USB_HOST_HCCPARAMS_PFL           (0x2)
#define MCF_USB_HOST_HCCPARAMS_ASP           (0x4)
#define MCF_USB_HOST_HCCPARAMS_IST(x)        (((x)&0xF)<<0x4)
#define MCF_USB_HOST_HCCPARAMS_EECP(x)       (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_USB_HOST_USBCMD */
#define MCF_USB_HOST_USBCMD_RS               (0x1)
#define MCF_USB_HOST_USBCMD_RST              (0x2)
#define MCF_USB_HOST_USBCMD_FS0              (0x4)
#define MCF_USB_HOST_USBCMD_FS1              (0x8)
#define MCF_USB_HOST_USBCMD_PSE              (0x10)
#define MCF_USB_HOST_USBCMD_ASE              (0x20)
#define MCF_USB_HOST_USBCMD_IAA              (0x40)
#define MCF_USB_HOST_USBCMD_ASP(x)           (((x)&0x3)<<0x8)
#define MCF_USB_HOST_USBCMD_ASPE             (0x800)
#define MCF_USB_HOST_USBCMD_FS2              (0x8000)
#define MCF_USB_HOST_USBCMD_ITC(x)           (((x)&0xFF)<<0x10)
#define MCF_USB_HOST_USBCMD_ITC_IMM          (0)
#define MCF_USB_HOST_USBCMD_ITC_1            (0x10000)
#define MCF_USB_HOST_USBCMD_ITC_2            (0x20000)
#define MCF_USB_HOST_USBCMD_ITC_4            (0x40000)
#define MCF_USB_HOST_USBCMD_ITC_8            (0x80000)
#define MCF_USB_HOST_USBCMD_ITC_16           (0x100000)
#define MCF_USB_HOST_USBCMD_ITC_32           (0x200000)
#define MCF_USB_HOST_USBCMD_ITC_64           (0x400000)
#define MCF_USB_HOST_USBCMD_FS_1024          (0)
#define MCF_USB_HOST_USBCMD_FS_512           (0x4)
#define MCF_USB_HOST_USBCMD_FS_256           (0x8)
#define MCF_USB_HOST_USBCMD_FS_128           (0xC)
#define MCF_USB_HOST_USBCMD_FS_64            (0x8000)
#define MCF_USB_HOST_USBCMD_FS_32            (0x8004)
#define MCF_USB_HOST_USBCMD_FS_16            (0x8008)
#define MCF_USB_HOST_USBCMD_FS_8             (0x800C)

/* Bit definitions and macros for MCF_USB_HOST_USBSTS */
#define MCF_USB_HOST_USBSTS_UI               (0x1)
#define MCF_USB_HOST_USBSTS_UEI              (0x2)
#define MCF_USB_HOST_USBSTS_PCI              (0x4)
#define MCF_USB_HOST_USBSTS_FRI              (0x8)
#define MCF_USB_HOST_USBSTS_SEI              (0x10)
#define MCF_USB_HOST_USBSTS_AAI              (0x20)
#define MCF_USB_HOST_USBSTS_URI              (0x40)
#define MCF_USB_HOST_USBSTS_SRI              (0x80)
#define MCF_USB_HOST_USBSTS_SLI              (0x100)
#define MCF_USB_HOST_USBSTS_ULPII            (0x400)
#define MCF_USB_HOST_USBSTS_HCH              (0x1000)
#define MCF_USB_HOST_USBSTS_RCL              (0x2000)
#define MCF_USB_HOST_USBSTS_PS               (0x4000)
#define MCF_USB_HOST_USBSTS_AS               (0x8000)
#define MCF_USB_HOST_USBSTS_NAKI             (0x10000)
#define MCF_USB_HOST_USBSTS_UAI              (0x40000)
#define MCF_USB_HOST_USBSTS_UPI              (0x80000)

/* Bit definitions and macros for MCF_USB_HOST_USBINTR */
#define MCF_USB_HOST_USBINTR_UE              (0x1)
#define MCF_USB_HOST_USBINTR_UEE             (0x2)
#define MCF_USB_HOST_USBINTR_PCE             (0x4)
#define MCF_USB_HOST_USBINTR_FRE             (0x8)
#define MCF_USB_HOST_USBINTR_SEE             (0x10)
#define MCF_USB_HOST_USBINTR_AAE             (0x20)
#define MCF_USB_HOST_USBINTR_SRE             (0x80)
#define MCF_USB_HOST_USBINTR_ULPIE           (0x400)
#define MCF_USB_HOST_USBINTR_NAKE            (0x10000)
#define MCF_USB_HOST_USBINTR_UAIE            (0x40000)
#define MCF_USB_HOST_USBINTR_UPIE            (0x80000)

/* Bit definitions and macros for MCF_USB_HOST_FRINDEX */
#define MCF_USB_HOST_FRINDEX_FRINDEX(x)      (((x)&0x3FFF)<<0)

/* Bit definitions and macros for MCF_USB_HOST_PERIODICLISTBASE */
#define MCF_USB_HOST_PERIODICLISTBASE_PERBASE(x) (((x)&0xFFFFF)<<0xC)

/* Bit definitions and macros for MCF_USB_HOST_ASYNCLISTADDR */
#define MCF_USB_HOST_ASYNCLISTADDR_ASYBASE(x) (((x)&0x7FFFFFF)<<0x5)

/* Bit definitions and macros for MCF_USB_HOST_TTCTRL */
#define MCF_USB_HOST_TTCTRL_TTHA(x)          (((x)&0x7F)<<0x18)

/* Bit definitions and macros for MCF_USB_HOST_BURSTSIZE */
#define MCF_USB_HOST_BURSTSIZE_RXPBURST(x)   (((x)&0xFF)<<0)
#define MCF_USB_HOST_BURSTSIZE_TXPBURST(x)   (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_USB_HOST_TXFILLTUNING */
#define MCF_USB_HOST_TXFILLTUNING_TXSCHOH(x) (((x)&0xFF)<<0)
#define MCF_USB_HOST_TXFILLTUNING_TXSCHHEALTH(x) (((x)&0x1F)<<0x8)
#define MCF_USB_HOST_TXFILLTUNING_TXFIFOTHRES(x) (((x)&0x3F)<<0x10)

/* Bit definitions and macros for MCF_USB_HOST_CONFIGFLAG */
#define MCF_USB_HOST_CONFIGFLAG_CONFIGFLAG(x) (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_USB_HOST_PORTSC1 */
#define MCF_USB_HOST_PORTSC1_CCS             (0x1)
#define MCF_USB_HOST_PORTSC1_CSC             (0x2)
#define MCF_USB_HOST_PORTSC1_PE              (0x4)
#define MCF_USB_HOST_PORTSC1_PEC             (0x8)
#define MCF_USB_HOST_PORTSC1_OCA             (0x10)
#define MCF_USB_HOST_PORTSC1_OCC             (0x20)
#define MCF_USB_HOST_PORTSC1_FPR             (0x40)
#define MCF_USB_HOST_PORTSC1_SUSP            (0x80)
#define MCF_USB_HOST_PORTSC1_PR              (0x100)
#define MCF_USB_HOST_PORTSC1_HSP             (0x200)
#define MCF_USB_HOST_PORTSC1_LS(x)           (((x)&0x3)<<0xA)
#define MCF_USB_HOST_PORTSC1_LS_SE0          (0)
#define MCF_USB_HOST_PORTSC1_LS_JSTATE       (0x400)
#define MCF_USB_HOST_PORTSC1_LS_KSTATE       (0x800)
#define MCF_USB_HOST_PORTSC1_PP              (0x1000)
#define MCF_USB_HOST_PORTSC1_PO              (0x2000)
#define MCF_USB_HOST_PORTSC1_PIC(x)          (((x)&0x3)<<0xE)
#define MCF_USB_HOST_PORTSC1_PIC_OFF         (0)
#define MCF_USB_HOST_PORTSC1_PIC_AMBER       (0x4000)
#define MCF_USB_HOST_PORTSC1_PIC_GREEN       (0x8000)
#define MCF_USB_HOST_PORTSC1_PTC(x)          (((x)&0xF)<<0x10)
#define MCF_USB_HOST_PORTSC1_PTC_DISABLE     (0)
#define MCF_USB_HOST_PORTSC1_PTC_JSTATE      (0x10000)
#define MCF_USB_HOST_PORTSC1_PTC_KSTATE      (0x20000)
#define MCF_USB_HOST_PORTSC1_PTC_SEQ_NAK     (0x30000)
#define MCF_USB_HOST_PORTSC1_PTC_PACKET      (0x40000)
#define MCF_USB_HOST_PORTSC1_PTC_FORCE_ENABLE_HS (0x50000)
#define MCF_USB_HOST_PORTSC1_PTC_FORCE_ENABLE_FS (0x60000)
#define MCF_USB_HOST_PORTSC1_PTC_FORCE_ENABLE_LS (0x70000)
#define MCF_USB_HOST_PORTSC1_WLCN            (0x100000)
#define MCF_USB_HOST_PORTSC1_WKDS            (0x200000)
#define MCF_USB_HOST_PORTSC1_WKOC            (0x400000)
#define MCF_USB_HOST_PORTSC1_PHCD            (0x800000)
#define MCF_USB_HOST_PORTSC1_PFSC            (0x1000000)
#define MCF_USB_HOST_PORTSC1_PSPD(x)         (((x)&0x3)<<0x1A)
#define MCF_USB_HOST_PORTSC1_PSPD_FULL       (0)
#define MCF_USB_HOST_PORTSC1_PSPD_LOW        (0x4000000)
#define MCF_USB_HOST_PORTSC1_PSPD_HIGH       (0x8000000)
#define MCF_USB_HOST_PORTSC1_PTS(x)          (((x)&0x3)<<0x1E)
#define MCF_USB_HOST_PORTSC1_PTS_ULPI        (0x80000000)
#define MCF_USB_HOST_PORTSC1_PTS_FS_LS       (0xC0000000)

/* Bit definitions and macros for MCF_USB_HOST_USBMODE */
#define MCF_USB_HOST_USBMODE_CM(x)           (((x)&0x3)<<0)
#define MCF_USB_HOST_USBMODE_CM_IDLE         (0)
#define MCF_USB_HOST_USBMODE_CM_DEVICE       (0x2)
#define MCF_USB_HOST_USBMODE_CM_HOST         (0x3)
#define MCF_USB_HOST_USBMODE_ES              (0x4)
#define MCF_USB_HOST_USBMODE_SLOM            (0x8)
#define MCF_USB_HOST_USBMODE_SDIS            (0x10)


#endif /* __MCF54418_USB_HOST_H__ */
