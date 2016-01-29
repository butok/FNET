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

#ifndef __MCF54418_USB_OTG_H__
#define __MCF54418_USB_OTG_H__


/*********************************************************************
*
* Universal Serial Bus Interface (USB_OTG)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_USB_OTG_ID                       (*(vuint32*)(0xFC0B0000))
#define MCF_USB_OTG_HWGENERAL                (*(vuint32*)(0xFC0B0004))
#define MCF_USB_OTG_HWHOST                   (*(vuint32*)(0xFC0B0008))
#define MCF_USB_OTG_HWDEVICE                 (*(vuint32*)(0xFC0B000C))
#define MCF_USB_OTG_HWTXBUF                  (*(vuint32*)(0xFC0B0010))
#define MCF_USB_OTG_HWRXBUF                  (*(vuint32*)(0xFC0B0014))
#define MCF_USB_OTG_GPTIMER0LD               (*(vuint32*)(0xFC0B0080))
#define MCF_USB_OTG_GPTIMER0CTL              (*(vuint32*)(0xFC0B0084))
#define MCF_USB_OTG_GPTIMER1LD               (*(vuint32*)(0xFC0B0088))
#define MCF_USB_OTG_GPTIMER1CTL              (*(vuint32*)(0xFC0B008C))
#define MCF_USB_OTG_HCIVERSION               (*(vuint16*)(0xFC0B0100))
#define MCF_USB_OTG_CAPLENGTH                (*(vuint8 *)(0xFC0B0103))
#define MCF_USB_OTG_HCSPARAMS                (*(vuint32*)(0xFC0B0104))
#define MCF_USB_OTG_HCCPARAMS                (*(vuint32*)(0xFC0B0108))
#define MCF_USB_OTG_DCIVERSION               (*(vuint16*)(0xFC0B0120))
#define MCF_USB_OTG_DCCPARAMS                (*(vuint32*)(0xFC0B0124))
#define MCF_USB_OTG_USBCMD                   (*(vuint32*)(0xFC0B0140))
#define MCF_USB_OTG_USBSTS                   (*(vuint32*)(0xFC0B0144))
#define MCF_USB_OTG_USBINTR                  (*(vuint32*)(0xFC0B0148))
#define MCF_USB_OTG_FRINDEX                  (*(vuint32*)(0xFC0B014C))
#define MCF_USB_OTG_PERIODICLISTBASE         (*(vuint32*)(0xFC0B0154))
#define MCF_USB_OTG_DEVICEADDR               (*(vuint32*)(0xFC0B0154))
#define MCF_USB_OTG_ASYNCLISTADDR            (*(vuint32*)(0xFC0B0158))
#define MCF_USB_OTG_EPLISTADDR               (*(vuint32*)(0xFC0B0158))
#define MCF_USB_OTG_TTCTRL                   (*(vuint32*)(0xFC0B015C))
#define MCF_USB_OTG_BURSTSIZE                (*(vuint32*)(0xFC0B0160))
#define MCF_USB_OTG_TXFILLTUNING             (*(vuint32*)(0xFC0B0164))
#define MCF_USB_OTG_ULPI_VIEWPORT            (*(vuint32*)(0xFC0B0170))
#define MCF_USB_OTG_CONFIGFLAG               (*(vuint32*)(0xFC0B0180))
#define MCF_USB_OTG_PORTSC1                  (*(vuint32*)(0xFC0B0184))
#define MCF_USB_OTG_OTGSC                    (*(vuint32*)(0xFC0B01A4))
#define MCF_USB_OTG_USBMODE                  (*(vuint32*)(0xFC0B01A8))
#define MCF_USB_OTG_EPSETUPSR                (*(vuint32*)(0xFC0B01AC))
#define MCF_USB_OTG_EPPRIME                  (*(vuint32*)(0xFC0B01B0))
#define MCF_USB_OTG_EPFLUSH                  (*(vuint32*)(0xFC0B01B4))
#define MCF_USB_OTG_EPSR                     (*(vuint32*)(0xFC0B01B8))
#define MCF_USB_OTG_EPCOMPLETE               (*(vuint32*)(0xFC0B01BC))
#define MCF_USB_OTG_EPCR0                    (*(vuint32*)(0xFC0B01C0))
#define MCF_USB_OTG_EPCR1                    (*(vuint32*)(0xFC0B01C4))
#define MCF_USB_OTG_EPCR2                    (*(vuint32*)(0xFC0B01C8))
#define MCF_USB_OTG_EPCR3                    (*(vuint32*)(0xFC0B01CC))
#define MCF_USB_OTG_EPCR(x)                  (*(vuint32*)(0xFC0B01C4 + ((x-1)*0x4)))


/* Bit definitions and macros for MCF_USB_OTG_ID */
#define MCF_USB_OTG_ID_RESERVED              (0xC000)
#define MCF_USB_OTG_ID_ID(x)                 (((x)&0x3F)<<0|0xC000)
#define MCF_USB_OTG_ID_NID(x)                (((x)&0x3F)<<0x8|0xC000)
#define MCF_USB_OTG_ID_REVISION(x)           (((x)&0xFF)<<0x10|0xC000)

/* Bit definitions and macros for MCF_USB_OTG_HWGENERAL */
#define MCF_USB_OTG_HWGENERAL_PHYW(x)        (((x)&0x3)<<0x4)
#define MCF_USB_OTG_HWGENERAL_PHYM(x)        (((x)&0x7)<<0x6)
#define MCF_USB_OTG_HWGENERAL_SM(x)          (((x)&0x3)<<0x9)

/* Bit definitions and macros for MCF_USB_OTG_HWHOST */
#define MCF_USB_OTG_HWHOST_HC                (0x1)
#define MCF_USB_OTG_HWHOST_NPORT(x)          (((x)&0x7)<<0x1)
#define MCF_USB_OTG_HWHOST_TTASY(x)          (((x)&0xFF)<<0x10)
#define MCF_USB_OTG_HWHOST_TTPER(x)          (((x)&0xFF)<<0x18)

/* Bit definitions and macros for MCF_USB_OTG_HWDEVICE */
#define MCF_USB_OTG_HWDEVICE_DC              (0x1)
#define MCF_USB_OTG_HWDEVICE_DEVEP(x)        (((x)&0x1F)<<0x1)

/* Bit definitions and macros for MCF_USB_OTG_HWTXBUF */
#define MCF_USB_OTG_HWTXBUF_TXBURST(x)       (((x)&0xFF)<<0)
#define MCF_USB_OTG_HWTXBUF_TXADD(x)         (((x)&0xFF)<<0x8)
#define MCF_USB_OTG_HWTXBUF_TXCHANADD(x)     (((x)&0xFF)<<0x10)
#define MCF_USB_OTG_HWTXBUF_TXLC             (0x80000000)

/* Bit definitions and macros for MCF_USB_OTG_HWRXBUF */
#define MCF_USB_OTG_HWRXBUF_RXBURST(x)       (((x)&0xFF)<<0)
#define MCF_USB_OTG_HWRXBUF_RXADD(x)         (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_USB_OTG_GPTIMER0LD */
#define MCF_USB_OTG_GPTIMER0LD_GPTLD(x)      (((x)&0xFFFFFF)<<0)

/* Bit definitions and macros for MCF_USB_OTG_GPTIMER0CTL */
#define MCF_USB_OTG_GPTIMER0CTL_GPTCNT(x)    (((x)&0xFFFFFF)<<0)
#define MCF_USB_OTG_GPTIMER0CTL_MODE         (0x1000000)
#define MCF_USB_OTG_GPTIMER0CTL_RST          (0x40000000)
#define MCF_USB_OTG_GPTIMER0CTL_RUN          (0x80000000)

/* Bit definitions and macros for MCF_USB_OTG_GPTIMER1LD */
#define MCF_USB_OTG_GPTIMER1LD_GPTLD(x)      (((x)&0xFFFFFF)<<0)

/* Bit definitions and macros for MCF_USB_OTG_GPTIMER1CTL */
#define MCF_USB_OTG_GPTIMER1CTL_GPTCNT(x)    (((x)&0xFFFFFF)<<0)
#define MCF_USB_OTG_GPTIMER1CTL_MODE         (0x1000000)
#define MCF_USB_OTG_GPTIMER1CTL_RST          (0x40000000)
#define MCF_USB_OTG_GPTIMER1CTL_RUN          (0x80000000)

/* Bit definitions and macros for MCF_USB_OTG_HCIVERSION */
#define MCF_USB_OTG_HCIVERSION_HCIVERSION(x) (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_USB_OTG_CAPLENGTH */
#define MCF_USB_OTG_CAPLENGTH_CAPLENGTH(x)   (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_USB_OTG_HCSPARAMS */
#define MCF_USB_OTG_HCSPARAMS_N_PORTS(x)     (((x)&0xF)<<0)
#define MCF_USB_OTG_HCSPARAMS_PPC            (0x10)
#define MCF_USB_OTG_HCSPARAMS_N_PCC(x)       (((x)&0xF)<<0x8)
#define MCF_USB_OTG_HCSPARAMS_N_CC(x)        (((x)&0xF)<<0xC)
#define MCF_USB_OTG_HCSPARAMS_PI             (0x10000)
#define MCF_USB_OTG_HCSPARAMS_N_PTT(x)       (((x)&0xF)<<0x14)
#define MCF_USB_OTG_HCSPARAMS_N_TT(x)        (((x)&0xF)<<0x18)

/* Bit definitions and macros for MCF_USB_OTG_HCCPARAMS */
#define MCF_USB_OTG_HCCPARAMS_ADC            (0x1)
#define MCF_USB_OTG_HCCPARAMS_PFL            (0x2)
#define MCF_USB_OTG_HCCPARAMS_ASP            (0x4)
#define MCF_USB_OTG_HCCPARAMS_IST(x)         (((x)&0xF)<<0x4)
#define MCF_USB_OTG_HCCPARAMS_EECP(x)        (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_USB_OTG_DCIVERSION */
#define MCF_USB_OTG_DCIVERSION_DCIVERSION(x) (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_USB_OTG_DCCPARAMS */
#define MCF_USB_OTG_DCCPARAMS_DEN(x)         (((x)&0x1F)<<0)
#define MCF_USB_OTG_DCCPARAMS_DC             (0x80)
#define MCF_USB_OTG_DCCPARAMS_HC             (0x100)

/* Bit definitions and macros for MCF_USB_OTG_USBCMD */
#define MCF_USB_OTG_USBCMD_RS                (0x1)
#define MCF_USB_OTG_USBCMD_RST               (0x2)
#define MCF_USB_OTG_USBCMD_FS0               (0x4)
#define MCF_USB_OTG_USBCMD_FS1               (0x8)
#define MCF_USB_OTG_USBCMD_PSE               (0x10)
#define MCF_USB_OTG_USBCMD_ASE               (0x20)
#define MCF_USB_OTG_USBCMD_IAA               (0x40)
#define MCF_USB_OTG_USBCMD_ASP(x)            (((x)&0x3)<<0x8)
#define MCF_USB_OTG_USBCMD_ASPE              (0x800)
#define MCF_USB_OTG_USBCMD_SUTW              (0x2000)
#define MCF_USB_OTG_USBCMD_ATDTW             (0x4000)
#define MCF_USB_OTG_USBCMD_FS2               (0x8000)
#define MCF_USB_OTG_USBCMD_ITC(x)            (((x)&0xFF)<<0x10)
#define MCF_USB_OTG_USBCMD_ITC_IMM           (0)
#define MCF_USB_OTG_USBCMD_ITC_1             (0x10000)
#define MCF_USB_OTG_USBCMD_ITC_2             (0x20000)
#define MCF_USB_OTG_USBCMD_ITC_4             (0x40000)
#define MCF_USB_OTG_USBCMD_ITC_8             (0x80000)
#define MCF_USB_OTG_USBCMD_ITC_16            (0x100000)
#define MCF_USB_OTG_USBCMD_ITC_32            (0x200000)
#define MCF_USB_OTG_USBCMD_ITC_64            (0x400000)
#define MCF_USB_OTG_USBCMD_FS_1024           (0)
#define MCF_USB_OTG_USBCMD_FS_512            (0x4)
#define MCF_USB_OTG_USBCMD_FS_256            (0x8)
#define MCF_USB_OTG_USBCMD_FS_128            (0xC)
#define MCF_USB_OTG_USBCMD_FS_64             (0x8000)
#define MCF_USB_OTG_USBCMD_FS_32             (0x8004)
#define MCF_USB_OTG_USBCMD_FS_16             (0x8008)
#define MCF_USB_OTG_USBCMD_FS_8              (0x800C)

/* Bit definitions and macros for MCF_USB_OTG_USBSTS */
#define MCF_USB_OTG_USBSTS_UI                (0x1)
#define MCF_USB_OTG_USBSTS_UEI               (0x2)
#define MCF_USB_OTG_USBSTS_PCI               (0x4)
#define MCF_USB_OTG_USBSTS_FRI               (0x8)
#define MCF_USB_OTG_USBSTS_SEI               (0x10)
#define MCF_USB_OTG_USBSTS_AAI               (0x20)
#define MCF_USB_OTG_USBSTS_URI               (0x40)
#define MCF_USB_OTG_USBSTS_SRI               (0x80)
#define MCF_USB_OTG_USBSTS_SLI               (0x100)
#define MCF_USB_OTG_USBSTS_ULPII             (0x400)
#define MCF_USB_OTG_USBSTS_HCH               (0x1000)
#define MCF_USB_OTG_USBSTS_RCL               (0x2000)
#define MCF_USB_OTG_USBSTS_PS                (0x4000)
#define MCF_USB_OTG_USBSTS_AS                (0x8000)
#define MCF_USB_OTG_USBSTS_NAKI              (0x10000)
#define MCF_USB_OTG_USBSTS_UAI               (0x40000)
#define MCF_USB_OTG_USBSTS_UPI               (0x80000)
#define MCF_USB_OTG_USBSTS_TI0               (0x1000000)
#define MCF_USB_OTG_USBSTS_TI1               (0x2000000)

/* Bit definitions and macros for MCF_USB_OTG_USBINTR */
#define MCF_USB_OTG_USBINTR_UE               (0x1)
#define MCF_USB_OTG_USBINTR_UEE              (0x2)
#define MCF_USB_OTG_USBINTR_PCE              (0x4)
#define MCF_USB_OTG_USBINTR_FRE              (0x8)
#define MCF_USB_OTG_USBINTR_SEE              (0x10)
#define MCF_USB_OTG_USBINTR_AAE              (0x20)
#define MCF_USB_OTG_USBINTR_URE              (0x40)
#define MCF_USB_OTG_USBINTR_SRE              (0x80)
#define MCF_USB_OTG_USBINTR_SLE              (0x100)
#define MCF_USB_OTG_USBINTR_ULPIE            (0x400)
#define MCF_USB_OTG_USBINTR_NAKE             (0x10000)
#define MCF_USB_OTG_USBINTR_UAIE             (0x40000)
#define MCF_USB_OTG_USBINTR_UPIE             (0x80000)
#define MCF_USB_OTG_USBINTR_TIE0             (0x1000000)
#define MCF_USB_OTG_USBINTR_TIE1             (0x2000000)

/* Bit definitions and macros for MCF_USB_OTG_FRINDEX */
#define MCF_USB_OTG_FRINDEX_FRINDEX(x)       (((x)&0x3FFF)<<0)

/* Bit definitions and macros for MCF_USB_OTG_PERIODICLISTBASE */
#define MCF_USB_OTG_PERIODICLISTBASE_PERBASE(x) (((x)&0xFFFFF)<<0xC)

/* Bit definitions and macros for MCF_USB_OTG_DEVICEADDR */
#define MCF_USB_OTG_DEVICEADDR_USBADR(x)     (((x)&0x7F)<<0x19)

/* Bit definitions and macros for MCF_USB_OTG_ASYNCLISTADDR */
#define MCF_USB_OTG_ASYNCLISTADDR_ASYBASE(x) (((x)&0x7FFFFFF)<<0x5)

/* Bit definitions and macros for MCF_USB_OTG_EPLISTADDR */
#define MCF_USB_OTG_EPLISTADDR_EPBASE(x)     (((x)&0x1FFFFF)<<0xB)

/* Bit definitions and macros for MCF_USB_OTG_TTCTRL */
#define MCF_USB_OTG_TTCTRL_TTHA(x)           (((x)&0x7F)<<0x18)

/* Bit definitions and macros for MCF_USB_OTG_BURSTSIZE */
#define MCF_USB_OTG_BURSTSIZE_RXPBURST(x)    (((x)&0xFF)<<0)
#define MCF_USB_OTG_BURSTSIZE_TXPBURST(x)    (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_USB_OTG_TXFILLTUNING */
#define MCF_USB_OTG_TXFILLTUNING_TXSCHOH(x)  (((x)&0xFF)<<0)
#define MCF_USB_OTG_TXFILLTUNING_TXSCHHEALTH(x) (((x)&0x1F)<<0x8)
#define MCF_USB_OTG_TXFILLTUNING_TXFIFOTHRES(x) (((x)&0x3F)<<0x10)

/* Bit definitions and macros for MCF_USB_OTG_ULPI_VIEWPORT */
#define MCF_USB_OTG_ULPI_VIEWPORT_ULPI_DATWR(x) (((x)&0xFF)<<0)
#define MCF_USB_OTG_ULPI_VIEWPORT_ULPI_DATRD(x) (((x)&0xFF)<<0x8)
#define MCF_USB_OTG_ULPI_VIEWPORT_ULPI_ADDR(x) (((x)&0xFF)<<0x10)
#define MCF_USB_OTG_ULPI_VIEWPORT_ULPI_PORT(x) (((x)&0x7)<<0x18)
#define MCF_USB_OTG_ULPI_VIEWPORT_ULPI_SS    (0x8000000)
#define MCF_USB_OTG_ULPI_VIEWPORT_ULPI_RW    (0x20000000)
#define MCF_USB_OTG_ULPI_VIEWPORT_ULPI_RUN   (0x40000000)
#define MCF_USB_OTG_ULPI_VIEWPORT_ULPI_WU    (0x80000000)

/* Bit definitions and macros for MCF_USB_OTG_CONFIGFLAG */
#define MCF_USB_OTG_CONFIGFLAG_CONFIGFLAG(x) (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_USB_OTG_PORTSC1 */
#define MCF_USB_OTG_PORTSC1_CCS              (0x1)
#define MCF_USB_OTG_PORTSC1_CSC              (0x2)
#define MCF_USB_OTG_PORTSC1_PE               (0x4)
#define MCF_USB_OTG_PORTSC1_PEC              (0x8)
#define MCF_USB_OTG_PORTSC1_OCA              (0x10)
#define MCF_USB_OTG_PORTSC1_OCC              (0x20)
#define MCF_USB_OTG_PORTSC1_FPR              (0x40)
#define MCF_USB_OTG_PORTSC1_SUSP             (0x80)
#define MCF_USB_OTG_PORTSC1_PR               (0x100)
#define MCF_USB_OTG_PORTSC1_HSP              (0x200)
#define MCF_USB_OTG_PORTSC1_LS(x)            (((x)&0x3)<<0xA)
#define MCF_USB_OTG_PORTSC1_LS_SE0           (0)
#define MCF_USB_OTG_PORTSC1_LS_JSTATE        (0x400)
#define MCF_USB_OTG_PORTSC1_LS_KSTATE        (0x800)
#define MCF_USB_OTG_PORTSC1_PP               (0x1000)
#define MCF_USB_OTG_PORTSC1_PO               (0x2000)
#define MCF_USB_OTG_PORTSC1_PIC(x)           (((x)&0x3)<<0xE)
#define MCF_USB_OTG_PORTSC1_PIC_OFF          (0)
#define MCF_USB_OTG_PORTSC1_PIC_AMBER        (0x4000)
#define MCF_USB_OTG_PORTSC1_PIC_GREEN        (0x8000)
#define MCF_USB_OTG_PORTSC1_PTC(x)           (((x)&0xF)<<0x10)
#define MCF_USB_OTG_PORTSC1_PTC_DISABLE      (0)
#define MCF_USB_OTG_PORTSC1_PTC_JSTATE       (0x10000)
#define MCF_USB_OTG_PORTSC1_PTC_KSTATE       (0x20000)
#define MCF_USB_OTG_PORTSC1_PTC_SEQ_NAK      (0x30000)
#define MCF_USB_OTG_PORTSC1_PTC_PACKET       (0x40000)
#define MCF_USB_OTG_PORTSC1_PTC_FORCE_ENABLE_HS (0x50000)
#define MCF_USB_OTG_PORTSC1_PTC_FORCE_ENABLE_FS (0x60000)
#define MCF_USB_OTG_PORTSC1_PTC_FORCE_ENABLE_LS (0x70000)
#define MCF_USB_OTG_PORTSC1_WLCN             (0x100000)
#define MCF_USB_OTG_PORTSC1_WKDS             (0x200000)
#define MCF_USB_OTG_PORTSC1_WKOC             (0x400000)
#define MCF_USB_OTG_PORTSC1_PHCD             (0x800000)
#define MCF_USB_OTG_PORTSC1_PFSC             (0x1000000)
#define MCF_USB_OTG_PORTSC1_PSPD(x)          (((x)&0x3)<<0x1A)
#define MCF_USB_OTG_PORTSC1_PSPD_FULL        (0)
#define MCF_USB_OTG_PORTSC1_PSPD_LOW         (0x4000000)
#define MCF_USB_OTG_PORTSC1_PSPD_HIGH        (0x8000000)
#define MCF_USB_OTG_PORTSC1_PTS(x)           (((x)&0x3)<<0x1E)
#define MCF_USB_OTG_PORTSC1_PTS_ULPI         (0x80000000)
#define MCF_USB_OTG_PORTSC1_PTS_FS_LS        (0xC0000000)

/* Bit definitions and macros for MCF_USB_OTG_OTGSC */
#define MCF_USB_OTG_OTGSC_VD                 (0x1)
#define MCF_USB_OTG_OTGSC_VC                 (0x2)
#define MCF_USB_OTG_OTGSC_OT                 (0x8)
#define MCF_USB_OTG_OTGSC_DP                 (0x10)
#define MCF_USB_OTG_OTGSC_IDPU               (0x20)
#define MCF_USB_OTG_OTGSC_ID                 (0x100)
#define MCF_USB_OTG_OTGSC_AVV                (0x200)
#define MCF_USB_OTG_OTGSC_ASV                (0x400)
#define MCF_USB_OTG_OTGSC_BSV                (0x800)
#define MCF_USB_OTG_OTGSC_BSE                (0x1000)
#define MCF_USB_OTG_OTGSC_1MST               (0x2000)
#define MCF_USB_OTG_OTGSC_DPS                (0x4000)
#define MCF_USB_OTG_OTGSC_IDIS               (0x10000)
#define MCF_USB_OTG_OTGSC_AVVIS              (0x20000)
#define MCF_USB_OTG_OTGSC_ASVIS              (0x40000)
#define MCF_USB_OTG_OTGSC_BSVIS              (0x80000)
#define MCF_USB_OTG_OTGSC_BSEIS              (0x100000)
#define MCF_USB_OTG_OTGSC_1MSS               (0x200000)
#define MCF_USB_OTG_OTGSC_DPIS               (0x400000)
#define MCF_USB_OTG_OTGSC_IDIE               (0x1000000)
#define MCF_USB_OTG_OTGSC_AVVIE              (0x2000000)
#define MCF_USB_OTG_OTGSC_ASVIE              (0x4000000)
#define MCF_USB_OTG_OTGSC_BSVIE              (0x8000000)
#define MCF_USB_OTG_OTGSC_BSEIE              (0x10000000)
#define MCF_USB_OTG_OTGSC_1MSE               (0x20000000)
#define MCF_USB_OTG_OTGSC_DPIE               (0x40000000)
#define MCF_USB_OTG_OTGSC_CLEAR              (0x7F0000)
#define MCF_USB_OTG_OTGSC_ENABLE_ALL         (0x7F000000)

/* Bit definitions and macros for MCF_USB_OTG_USBMODE */
#define MCF_USB_OTG_USBMODE_CM(x)            (((x)&0x3)<<0)
#define MCF_USB_OTG_USBMODE_CM_IDLE          (0)
#define MCF_USB_OTG_USBMODE_CM_DEVICE        (0x2)
#define MCF_USB_OTG_USBMODE_CM_HOST          (0x3)
#define MCF_USB_OTG_USBMODE_ES               (0x4)
#define MCF_USB_OTG_USBMODE_SLOM             (0x8)
#define MCF_USB_OTG_USBMODE_SDIS             (0x10)

/* Bit definitions and macros for MCF_USB_OTG_EPSETUPSR */
#define MCF_USB_OTG_EPSETUPSR_EPSETUPSTAT0   (0x1)
#define MCF_USB_OTG_EPSETUPSR_EPSETUPSTAT1   (0x2)
#define MCF_USB_OTG_EPSETUPSR_EPSETUPSTAT2   (0x4)
#define MCF_USB_OTG_EPSETUPSR_EPSETUPSTAT3   (0x8)
#define MCF_USB_OTG_EPSETUPSR_EPSETUPSTAT(x) (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_USB_OTG_EPPRIME */
#define MCF_USB_OTG_EPPRIME_PERB0            (0x1)
#define MCF_USB_OTG_EPPRIME_PERB1            (0x2)
#define MCF_USB_OTG_EPPRIME_PERB2            (0x4)
#define MCF_USB_OTG_EPPRIME_PERB3            (0x8)
#define MCF_USB_OTG_EPPRIME_PETB0            (0x10000)
#define MCF_USB_OTG_EPPRIME_PETB1            (0x20000)
#define MCF_USB_OTG_EPPRIME_PETB2            (0x40000)
#define MCF_USB_OTG_EPPRIME_PETB3            (0x80000)
#define MCF_USB_OTG_EPPRIME_PERB(x)          (((x)&0x0F)<<0)
#define MCF_USB_OTG_EPPRIME_PETB(x)          (((x)&0x0F)<<0x10)

/* Bit definitions and macros for MCF_USB_OTG_EPFLUSH */
#define MCF_USB_OTG_EPFLUSH_FERB0            (0x1)
#define MCF_USB_OTG_EPFLUSH_FERB1            (0x2)
#define MCF_USB_OTG_EPFLUSH_FERB2            (0x4)
#define MCF_USB_OTG_EPFLUSH_FERB3            (0x8)
#define MCF_USB_OTG_EPFLUSH_FETB0            (0x10000)
#define MCF_USB_OTG_EPFLUSH_FETB1            (0x20000)
#define MCF_USB_OTG_EPFLUSH_FETB2            (0x40000)
#define MCF_USB_OTG_EPFLUSH_FETB3            (0x80000)
#define MCF_USB_OTG_EPFLUSH_FERB(x)          (((x)&0x0F)<<0)
#define MCF_USB_OTG_EPFLUSH_FETB(x)          (((x)&0x0F)<<0x10)

/* Bit definitions and macros for MCF_USB_OTG_EPSR */
#define MCF_USB_OTG_EPSR_ERBR0               (0x1)
#define MCF_USB_OTG_EPSR_ERBR1               (0x2)
#define MCF_USB_OTG_EPSR_ERBR2               (0x4)
#define MCF_USB_OTG_EPSR_ERBR3               (0x8)
#define MCF_USB_OTG_EPSR_ETBR0               (0x10000)
#define MCF_USB_OTG_EPSR_ETBR1               (0x20000)
#define MCF_USB_OTG_EPSR_ETBR2               (0x40000)
#define MCF_USB_OTG_EPSR_ETBR3               (0x80000)
#define MCF_USB_OTG_EPSR_ERBR(x)             (((x)&0x0F)<<0)
#define MCF_USB_OTG_EPSR_ETBR(x)             (((x)&0x0F)<<0x10)

/* Bit definitions and macros for MCF_USB_OTG_EPCOMPLETE */
#define MCF_USB_OTG_EPCOMPLETE_ERCE0         (0x1)
#define MCF_USB_OTG_EPCOMPLETE_ERCE1         (0x2)
#define MCF_USB_OTG_EPCOMPLETE_ERCE2         (0x4)
#define MCF_USB_OTG_EPCOMPLETE_ERCE3         (0x8)
#define MCF_USB_OTG_EPCOMPLETE_ETCE0         (0x10000)
#define MCF_USB_OTG_EPCOMPLETE_ETCE1         (0x20000)
#define MCF_USB_OTG_EPCOMPLETE_ETCE2         (0x40000)
#define MCF_USB_OTG_EPCOMPLETE_ETCE3         (0x80000)
#define MCF_USB_OTG_EPCOMPLETE_ERCE(x)       (((x)&0xF)<<0)
#define MCF_USB_OTG_EPCOMPLETE_ETCE(x)       (((x)&0xF)<<0x10)

/* Bit definitions and macros for MCF_USB_OTG_EPCR0 */
#define MCF_USB_OTG_EPCR0_RXS                (0x1)
#define MCF_USB_OTG_EPCR0_RXT(x)             (((x)&0x3)<<0x2)
#define MCF_USB_OTG_EPCR0_RXE                (0x80)
#define MCF_USB_OTG_EPCR0_TXS                (0x10000)
#define MCF_USB_OTG_EPCR0_TXT(x)             (((x)&0x3)<<0x12)
#define MCF_USB_OTG_EPCR0_TXE                (0x800000)

/* Bit definitions and macros for MCF_USB_OTG_EPCR */
#define MCF_USB_OTG_EPCR_RXS                 (0x1)
#define MCF_USB_OTG_EPCR_RXD                 (0x2)
#define MCF_USB_OTG_EPCR_RXT(x)              (((x)&0x3)<<0x2)
#define MCF_USB_OTG_EPCR_RXT_CONTROL         (0)
#define MCF_USB_OTG_EPCR_RXT_ISO             (0x4)
#define MCF_USB_OTG_EPCR_RXT_BULK            (0x8)
#define MCF_USB_OTG_EPCR_RXT_INT             (0xC)
#define MCF_USB_OTG_EPCR_RXI                 (0x20)
#define MCF_USB_OTG_EPCR_RXR                 (0x40)
#define MCF_USB_OTG_EPCR_RXE                 (0x80)
#define MCF_USB_OTG_EPCR_TXS                 (0x10000)
#define MCF_USB_OTG_EPCR_TXD                 (0x20000)
#define MCF_USB_OTG_EPCR_TXT(x)              (((x)&0x3)<<0x12)
#define MCF_USB_OTG_EPCR_TXT_CONTROL         (0)
#define MCF_USB_OTG_EPCR_TXT_ISO             (0x40000)
#define MCF_USB_OTG_EPCR_TXT_BULK            (0x80000)
#define MCF_USB_OTG_EPCR_TXT_INT             (0xC0000)
#define MCF_USB_OTG_EPCR_TXI                 (0x200000)
#define MCF_USB_OTG_EPCR_TXR                 (0x400000)
#define MCF_USB_OTG_EPCR_TXE                 (0x800000)


#endif /* __MCF54418_USB_OTG_H__ */
