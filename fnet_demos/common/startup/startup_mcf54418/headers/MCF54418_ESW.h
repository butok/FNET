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

#ifndef __MCF54418_ESW_H__
#define __MCF54418_ESW_H__


/*********************************************************************
*
* Ethernet Switch (ESW)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_ESW_REV                          (*(vuint32*)(0xFC0DC000))
#define MCF_ESW_SCR                          (*(vuint32*)(0xFC0DC004))
#define MCF_ESW_PER                          (*(vuint32*)(0xFC0DC008))
#define MCF_ESW_VLANV                        (*(vuint32*)(0xFC0DC010))
#define MCF_ESW_DBCR                         (*(vuint32*)(0xFC0DC014))
#define MCF_ESW_DMCR                         (*(vuint32*)(0xFC0DC018))
#define MCF_ESW_BKLR                         (*(vuint32*)(0xFC0DC01C))
#define MCF_ESW_BMPC                         (*(vuint32*)(0xFC0DC020))
#define MCF_ESW_MODE                         (*(vuint32*)(0xFC0DC024))
#define MCF_ESW_VIMSEL                       (*(vuint32*)(0xFC0DC028))
#define MCF_ESW_VOMSEL                       (*(vuint32*)(0xFC0DC02C))
#define MCF_ESW_VIMEN                        (*(vuint32*)(0xFC0DC030))
#define MCF_ESW_VID                          (*(vuint32*)(0xFC0DC034))
#define MCF_ESW_MCR                          (*(vuint32*)(0xFC0DC040))
#define MCF_ESW_EGMAP                        (*(vuint32*)(0xFC0DC044))
#define MCF_ESW_INGMAP                       (*(vuint32*)(0xFC0DC048))
#define MCF_ESW_INGSAL                       (*(vuint32*)(0xFC0DC04C))
#define MCF_ESW_INGSAH                       (*(vuint32*)(0xFC0DC050))
#define MCF_ESW_INGDAL                       (*(vuint32*)(0xFC0DC054))
#define MCF_ESW_INGDAH                       (*(vuint32*)(0xFC0DC058))
#define MCF_ESW_EGSAL                        (*(vuint32*)(0xFC0DC05C))
#define MCF_ESW_EGSAH                        (*(vuint32*)(0xFC0DC060))
#define MCF_ESW_EGDAL                        (*(vuint32*)(0xFC0DC064))
#define MCF_ESW_EGDAH                        (*(vuint32*)(0xFC0DC068))
#define MCF_ESW_MCVAL                        (*(vuint32*)(0xFC0DC06C))
#define MCF_ESW_MMSR                         (*(vuint32*)(0xFC0DC080))
#define MCF_ESW_LMT                          (*(vuint32*)(0xFC0DC084))
#define MCF_ESW_LFC                          (*(vuint32*)(0xFC0DC088))
#define MCF_ESW_PCSR                         (*(vuint32*)(0xFC0DC08C))
#define MCF_ESW_IOSR                         (*(vuint32*)(0xFC0DC090))
#define MCF_ESW_QWT                          (*(vuint32*)(0xFC0DC094))
#define MCF_ESW_P0BCT                        (*(vuint32*)(0xFC0DC09C))
#define MCF_ESW_FFEN                         (*(vuint32*)(0xFC0DC0BC))
#define MCF_ESW_PSNP1                        (*(vuint32*)(0xFC0DC0C0))
#define MCF_ESW_PSNP2                        (*(vuint32*)(0xFC0DC0C4))
#define MCF_ESW_PSNP3                        (*(vuint32*)(0xFC0DC0C8))
#define MCF_ESW_PSNP4                        (*(vuint32*)(0xFC0DC0CC))
#define MCF_ESW_PSNP5                        (*(vuint32*)(0xFC0DC0D0))
#define MCF_ESW_PSNP6                        (*(vuint32*)(0xFC0DC0D4))
#define MCF_ESW_PSNP7                        (*(vuint32*)(0xFC0DC0D8))
#define MCF_ESW_PSNP8                        (*(vuint32*)(0xFC0DC0DC))
#define MCF_ESW_IPSNP1                       (*(vuint32*)(0xFC0DC0E0))
#define MCF_ESW_IPSNP2                       (*(vuint32*)(0xFC0DC0E4))
#define MCF_ESW_IPSNP3                       (*(vuint32*)(0xFC0DC0E8))
#define MCF_ESW_IPSNP4                       (*(vuint32*)(0xFC0DC0EC))
#define MCF_ESW_IPSNP5                       (*(vuint32*)(0xFC0DC0F0))
#define MCF_ESW_IPSNP6                       (*(vuint32*)(0xFC0DC0F4))
#define MCF_ESW_IPSNP7                       (*(vuint32*)(0xFC0DC0F8))
#define MCF_ESW_IPSNP8                       (*(vuint32*)(0xFC0DC0FC))
#define MCF_ESW_P0VRES                       (*(vuint32*)(0xFC0DC100))
#define MCF_ESW_P1VRES                       (*(vuint32*)(0xFC0DC104))
#define MCF_ESW_P2VRES                       (*(vuint32*)(0xFC0DC108))
#define MCF_ESW_IPRES                        (*(vuint32*)(0xFC0DC140))
#define MCF_ESW_P0RES                        (*(vuint32*)(0xFC0DC180))
#define MCF_ESW_P1RES                        (*(vuint32*)(0xFC0DC184))
#define MCF_ESW_P2RES                        (*(vuint32*)(0xFC0DC188))
#define MCF_ESW_P0ID                         (*(vuint32*)(0xFC0DC200))
#define MCF_ESW_P1ID                         (*(vuint32*)(0xFC0DC204))
#define MCF_ESW_P2ID                         (*(vuint32*)(0xFC0DC208))
#define MCF_ESW_VRES0                        (*(vuint32*)(0xFC0DC280))
#define MCF_ESW_VRES1                        (*(vuint32*)(0xFC0DC284))
#define MCF_ESW_VRES2                        (*(vuint32*)(0xFC0DC288))
#define MCF_ESW_VRES3                        (*(vuint32*)(0xFC0DC28C))
#define MCF_ESW_VRES4                        (*(vuint32*)(0xFC0DC290))
#define MCF_ESW_VRES5                        (*(vuint32*)(0xFC0DC294))
#define MCF_ESW_VRES6                        (*(vuint32*)(0xFC0DC298))
#define MCF_ESW_VRES7                        (*(vuint32*)(0xFC0DC29C))
#define MCF_ESW_VRES8                        (*(vuint32*)(0xFC0DC2A0))
#define MCF_ESW_VRES9                        (*(vuint32*)(0xFC0DC2A4))
#define MCF_ESW_VRES10                       (*(vuint32*)(0xFC0DC2A8))
#define MCF_ESW_VRES11                       (*(vuint32*)(0xFC0DC2AC))
#define MCF_ESW_VRES12                       (*(vuint32*)(0xFC0DC2B0))
#define MCF_ESW_VRES13                       (*(vuint32*)(0xFC0DC2B4))
#define MCF_ESW_VRES14                       (*(vuint32*)(0xFC0DC2B8))
#define MCF_ESW_VRES15                       (*(vuint32*)(0xFC0DC2BC))
#define MCF_ESW_VRES16                       (*(vuint32*)(0xFC0DC2C0))
#define MCF_ESW_VRES17                       (*(vuint32*)(0xFC0DC2C4))
#define MCF_ESW_VRES18                       (*(vuint32*)(0xFC0DC2C8))
#define MCF_ESW_VRES19                       (*(vuint32*)(0xFC0DC2CC))
#define MCF_ESW_VRES20                       (*(vuint32*)(0xFC0DC2D0))
#define MCF_ESW_VRES21                       (*(vuint32*)(0xFC0DC2D4))
#define MCF_ESW_VRES22                       (*(vuint32*)(0xFC0DC2D8))
#define MCF_ESW_VRES23                       (*(vuint32*)(0xFC0DC2DC))
#define MCF_ESW_VRES24                       (*(vuint32*)(0xFC0DC2E0))
#define MCF_ESW_VRES25                       (*(vuint32*)(0xFC0DC2E4))
#define MCF_ESW_VRES26                       (*(vuint32*)(0xFC0DC2E8))
#define MCF_ESW_VRES27                       (*(vuint32*)(0xFC0DC2EC))
#define MCF_ESW_VRES28                       (*(vuint32*)(0xFC0DC2F0))
#define MCF_ESW_VRES29                       (*(vuint32*)(0xFC0DC2F4))
#define MCF_ESW_VRES30                       (*(vuint32*)(0xFC0DC2F8))
#define MCF_ESW_VRES31                       (*(vuint32*)(0xFC0DC2FC))
#define MCF_ESW_DISCN                        (*(vuint32*)(0xFC0DC300))
#define MCF_ESW_DISCB                        (*(vuint32*)(0xFC0DC304))
#define MCF_ESW_NDISCN                       (*(vuint32*)(0xFC0DC308))
#define MCF_ESW_NDISCB                       (*(vuint32*)(0xFC0DC30C))
#define MCF_ESW_P0OQC                        (*(vuint32*)(0xFC0DC310))
#define MCF_ESW_P0MVID                       (*(vuint32*)(0xFC0DC314))
#define MCF_ESW_P0MVTAG                      (*(vuint32*)(0xFC0DC318))
#define MCF_ESW_P0BL                         (*(vuint32*)(0xFC0DC31C))
#define MCF_ESW_P1OQC                        (*(vuint32*)(0xFC0DC320))
#define MCF_ESW_P1MVID                       (*(vuint32*)(0xFC0DC324))
#define MCF_ESW_P1MVTAG                      (*(vuint32*)(0xFC0DC328))
#define MCF_ESW_P1BL                         (*(vuint32*)(0xFC0DC32C))
#define MCF_ESW_P2OQC                        (*(vuint32*)(0xFC0DC330))
#define MCF_ESW_P2MVID                       (*(vuint32*)(0xFC0DC334))
#define MCF_ESW_P2MVTAG                      (*(vuint32*)(0xFC0DC338))
#define MCF_ESW_P2BL                         (*(vuint32*)(0xFC0DC33C))
#define MCF_ESW_ISR                          (*(vuint32*)(0xFC0DC400))
#define MCF_ESW_IMR                          (*(vuint32*)(0xFC0DC404))
#define MCF_ESW_RDSR                         (*(vuint32*)(0xFC0DC408))
#define MCF_ESW_TDSR                         (*(vuint32*)(0xFC0DC40C))
#define MCF_ESW_MRBR                         (*(vuint32*)(0xFC0DC410))
#define MCF_ESW_RDAR                         (*(vuint32*)(0xFC0DC414))
#define MCF_ESW_TDAR                         (*(vuint32*)(0xFC0DC418))
#define MCF_ESW_LREC0                        (*(vuint32*)(0xFC0DC500))
#define MCF_ESW_LREC1                        (*(vuint32*)(0xFC0DC504))
#define MCF_ESW_LSR                          (*(vuint32*)(0xFC0DC508))
#define MCF_ESW_PSNP(x)                      (*(vuint32*)(0xFC0DC0C0 + ((x-1)*0x4)))
#define MCF_ESW_IPSNP(x)                     (*(vuint32*)(0xFC0DC0E0 + ((x-1)*0x4)))
#define MCF_ESW_PVRES(x)                     (*(vuint32*)(0xFC0DC100 + ((x)*0x4)))
#define MCF_ESW_PRES(x)                      (*(vuint32*)(0xFC0DC180 + ((x)*0x4)))
#define MCF_ESW_PID(x)                       (*(vuint32*)(0xFC0DC200 + ((x)*0x4)))
#define MCF_ESW_VRES(x)                      (*(vuint32*)(0xFC0DC280 + ((x)*0x4)))
#define MCF_ESW_POQC(x)                      (*(vuint32*)(0xFC0DC310 + ((x)*0x10)))
#define MCF_ESW_PMVID(x)                     (*(vuint32*)(0xFC0DC314 + ((x)*0x10)))
#define MCF_ESW_PMVTAG(x)                    (*(vuint32*)(0xFC0DC318 + ((x)*0x10)))
#define MCF_ESW_PBL(x)                       (*(vuint32*)(0xFC0DC31C + ((x)*0x10)))


/* Bit definitions and macros for MCF_ESW_REV */
#define MCF_ESW_REV_CoreRevision(x)          (((x)&0xFFFF)<<0)
#define MCF_ESW_REV_CustomerRevision(x)      (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_ESW_SCR */
#define MCF_ESW_SCR_SCRATCH(x)               (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_PER */
#define MCF_ESW_PER_TE0                      (0x1)
#define MCF_ESW_PER_TE1                      (0x2)
#define MCF_ESW_PER_TE2                      (0x4)
#define MCF_ESW_PER_RE0                      (0x10000)
#define MCF_ESW_PER_RE1                      (0x20000)
#define MCF_ESW_PER_RE2                      (0x40000)

/* Bit definitions and macros for MCF_ESW_VLANV */
#define MCF_ESW_VLANV_VV0                    (0x1)
#define MCF_ESW_VLANV_VV1                    (0x2)
#define MCF_ESW_VLANV_VV2                    (0x4)
#define MCF_ESW_VLANV_DU0                    (0x10000)
#define MCF_ESW_VLANV_DU1                    (0x20000)
#define MCF_ESW_VLANV_DU2                    (0x40000)

/* Bit definitions and macros for MCF_ESW_DBCR */
#define MCF_ESW_DBCR_P0                      (0x1)
#define MCF_ESW_DBCR_P1                      (0x2)
#define MCF_ESW_DBCR_P2                      (0x4)

/* Bit definitions and macros for MCF_ESW_DMCR */
#define MCF_ESW_DMCR_P0                      (0x1)
#define MCF_ESW_DMCR_P1                      (0x2)
#define MCF_ESW_DMCR_P2                      (0x4)

/* Bit definitions and macros for MCF_ESW_BKLR */
#define MCF_ESW_BKLR_BE0                     (0x1)
#define MCF_ESW_BKLR_BE1                     (0x2)
#define MCF_ESW_BKLR_BE2                     (0x4)
#define MCF_ESW_BKLR_LD0                     (0x10000)
#define MCF_ESW_BKLR_LD1                     (0x20000)
#define MCF_ESW_BKLR_LD2                     (0x40000)

/* Bit definitions and macros for MCF_ESW_BMPC */
#define MCF_ESW_BMPC_PORT(x)                 (((x)&0xF)<<0)
#define MCF_ESW_BMPC_MSGTX                   (0x20)
#define MCF_ESW_BMPC_EN                      (0x40)
#define MCF_ESW_BMPC_DIS                     (0x80)
#define MCF_ESW_BMPC_PRIORITY(x)             (((x)&0x7)<<0xD)
#define MCF_ESW_BMPC_PORTMASK(x)             (((x)&0x7)<<0x10)
#define MCF_ESW_BMPC_PORTMASK_PORT0          (0x10000)
#define MCF_ESW_BMPC_PORTMASK_PORT1          (0x20000)
#define MCF_ESW_BMPC_PORTMASK_PORT2          (0x40000)

/* Bit definitions and macros for MCF_ESW_MODE */
#define MCF_ESW_MODE_SWRST                   (0x1)
#define MCF_ESW_MODE_SWEN                    (0x2)
#define MCF_ESW_MODE_STOP                    (0x80)
#define MCF_ESW_MODE_CRCTRAN                 (0x100)
#define MCF_ESW_MODE_P0CT                    (0x200)
#define MCF_ESW_MODE_STATRST                 (0x80000000)

/* Bit definitions and macros for MCF_ESW_VIMSEL */
#define MCF_ESW_VIMSEL_IM0(x)                (((x)&0x3)<<0)
#define MCF_ESW_VIMSEL_IM0_MODE1             (0)
#define MCF_ESW_VIMSEL_IM0_MODE2             (0x1)
#define MCF_ESW_VIMSEL_IM0_MODE3             (0x2)
#define MCF_ESW_VIMSEL_IM0_MODE4             (0x3)
#define MCF_ESW_VIMSEL_IM1(x)                (((x)&0x3)<<0x2)
#define MCF_ESW_VIMSEL_IM1_MODE1             (0)
#define MCF_ESW_VIMSEL_IM1_MODE2             (0x4)
#define MCF_ESW_VIMSEL_IM1_MODE3             (0x8)
#define MCF_ESW_VIMSEL_IM1_MODE4             (0xC)
#define MCF_ESW_VIMSEL_IM2(x)                (((x)&0x3)<<0x4)
#define MCF_ESW_VIMSEL_IM2_MODE1             (0)
#define MCF_ESW_VIMSEL_IM2_MODE2             (0x10)
#define MCF_ESW_VIMSEL_IM2_MODE3             (0x20)
#define MCF_ESW_VIMSEL_IM2_MODE4             (0x30)

/* Bit definitions and macros for MCF_ESW_VOMSEL */
#define MCF_ESW_VOMSEL_OM0(x)                (((x)&0x3)<<0)
#define MCF_ESW_VOMSEL_OM0_NO                (0)
#define MCF_ESW_VOMSEL_OM0_MODE1             (0x1)
#define MCF_ESW_VOMSEL_OM0_MODE2             (0x2)
#define MCF_ESW_VOMSEL_OM0_MODE3             (0x3)
#define MCF_ESW_VOMSEL_OM1(x)                (((x)&0x3)<<0x2)
#define MCF_ESW_VOMSEL_OM1_NO                (0)
#define MCF_ESW_VOMSEL_OM1_MODE1             (0x4)
#define MCF_ESW_VOMSEL_OM1_MODE2             (0x8)
#define MCF_ESW_VOMSEL_OM1_MODE3             (0xC)
#define MCF_ESW_VOMSEL_OM2(x)                (((x)&0x3)<<0x4)
#define MCF_ESW_VOMSEL_OM2_NO                (0)
#define MCF_ESW_VOMSEL_OM2_MODE1             (0x10)
#define MCF_ESW_VOMSEL_OM2_MODE2             (0x20)
#define MCF_ESW_VOMSEL_OM2_MODE3             (0x30)

/* Bit definitions and macros for MCF_ESW_VIMEN */
#define MCF_ESW_VIMEN_EN0                    (0x1)
#define MCF_ESW_VIMEN_EN1                    (0x2)
#define MCF_ESW_VIMEN_EN2                    (0x4)

/* Bit definitions and macros for MCF_ESW_VID */
#define MCF_ESW_VID_TAG(x)                   (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_MCR */
#define MCF_ESW_MCR_PORT(x)                  (((x)&0xF)<<0)
#define MCF_ESW_MCR_MEN                      (0x10)
#define MCF_ESW_MCR_INGMAP                   (0x20)
#define MCF_ESW_MCR_EGMAP                    (0x40)
#define MCF_ESW_MCR_INGSA                    (0x80)
#define MCF_ESW_MCR_INGDA                    (0x100)
#define MCF_ESW_MCR_EGSA                     (0x200)
#define MCF_ESW_MCR_EGDA                     (0x400)

/* Bit definitions and macros for MCF_ESW_EGMAP */
#define MCF_ESW_EGMAP_EG0                    (0x1)
#define MCF_ESW_EGMAP_EG1                    (0x2)
#define MCF_ESW_EGMAP_EG2                    (0x4)

/* Bit definitions and macros for MCF_ESW_INGMAP */
#define MCF_ESW_INGMAP_ING0                  (0x1)
#define MCF_ESW_INGMAP_ING1                  (0x2)
#define MCF_ESW_INGMAP_ING2                  (0x4)

/* Bit definitions and macros for MCF_ESW_INGSAL */
#define MCF_ESW_INGSAL_ADDLOW(x)             (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_INGSAH */
#define MCF_ESW_INGSAH_ADDHIGH(x)            (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_INGDAL */
#define MCF_ESW_INGDAL_ADDLOW(x)             (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_INGDAH */
#define MCF_ESW_INGDAH_ADDHIGH(x)            (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_EGSAL */
#define MCF_ESW_EGSAL_ADDLOW(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_EGSAH */
#define MCF_ESW_EGSAH_ADDHIGH(x)             (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_EGDAL */
#define MCF_ESW_EGDAL_ADDLOW(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_EGDAH */
#define MCF_ESW_EGDAH_ADDHIGH(x)             (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_MCVAL */
#define MCF_ESW_MCVAL_COUNT(x)               (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_ESW_MMSR */
#define MCF_ESW_MMSR_BUSY                    (0x1)
#define MCF_ESW_MMSR_NOCELL                  (0x2)
#define MCF_ESW_MMSR_MEMFULL                 (0x4)
#define MCF_ESW_MMSR_MFLATCH                 (0x8)
#define MCF_ESW_MMSR_DQGRNT                  (0x40)
#define MCF_ESW_MMSR_CELLS_AVAIL(x)          (((x)&0xFF)<<0x10)

/* Bit definitions and macros for MCF_ESW_LMT */
#define MCF_ESW_LMT_THRESH(x)                (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_ESW_LFC */
#define MCF_ESW_LFC_COUNT(x)                 (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_PCSR */
#define MCF_ESW_PCSR_PC0                     (0x1)
#define MCF_ESW_PCSR_PC1                     (0x2)
#define MCF_ESW_PCSR_PC2                     (0x4)

/* Bit definitions and macros for MCF_ESW_IOSR */
#define MCF_ESW_IOSR_OR0                     (0x1)
#define MCF_ESW_IOSR_OR1                     (0x2)
#define MCF_ESW_IOSR_OR2                     (0x4)
#define MCF_ESW_IOSR_IR0                     (0x10000)
#define MCF_ESW_IOSR_IR1                     (0x20000)
#define MCF_ESW_IOSR_IR2                     (0x40000)

/* Bit definitions and macros for MCF_ESW_QWT */
#define MCF_ESW_QWT_Q0WT(x)                  (((x)&0x1F)<<0)
#define MCF_ESW_QWT_Q1WT(x)                  (((x)&0x1F)<<0x8)
#define MCF_ESW_QWT_Q2WT(x)                  (((x)&0x1F)<<0x10)
#define MCF_ESW_QWT_Q3WT(x)                  (((x)&0x1F)<<0x18)

/* Bit definitions and macros for MCF_ESW_P0BCT */
#define MCF_ESW_P0BCT_THRESH(x)              (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_ESW_FFEN */
#define MCF_ESW_FFEN_FEN                     (0x1)
#define MCF_ESW_FFEN_FD(x)                   (((x)&0x3)<<0x2)
#define MCF_ESW_FFEN_FD_NOT                  (0)
#define MCF_ESW_FFEN_FD_PORT1                (0x4)
#define MCF_ESW_FFEN_FD_PORT2                (0x8)
#define MCF_ESW_FFEN_FD_BOTH                 (0xC)

/* Bit definitions and macros for MCF_ESW_PSNP */
#define MCF_ESW_PSNP_EN                      (0x1)
#define MCF_ESW_PSNP_MODE(x)                 (((x)&0x3)<<0x1)
#define MCF_ESW_PSNP_MODE_FWD                (0)
#define MCF_ESW_PSNP_MODE_COPY_FWD           (0x2)
#define MCF_ESW_PSNP_MODE_DISCARD            (0x4)
#define MCF_ESW_PSNP_CD                      (0x8)
#define MCF_ESW_PSNP_CS                      (0x10)
#define MCF_ESW_PSNP_PORT_COMPARE(x)         (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_ESW_IPSNP */
#define MCF_ESW_IPSNP_EN                     (0x1)
#define MCF_ESW_IPSNP_MODE(x)                (((x)&0x3)<<0x1)
#define MCF_ESW_IPSNP_MODE_FWD               (0)
#define MCF_ESW_IPSNP_MODE_COPY_FWD          (0x2)
#define MCF_ESW_IPSNP_MODE_DISCARD           (0x4)
#define MCF_ESW_IPSNP_PROTOCOL(x)            (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_ESW_PVRES */
#define MCF_ESW_PVRES_PRI0(x)                (((x)&0x7)<<0)
#define MCF_ESW_PVRES_PRI1(x)                (((x)&0x7)<<0x3)
#define MCF_ESW_PVRES_PRI2(x)                (((x)&0x7)<<0x6)
#define MCF_ESW_PVRES_PRI3(x)                (((x)&0x7)<<0x9)
#define MCF_ESW_PVRES_PRI4(x)                (((x)&0x7)<<0xC)
#define MCF_ESW_PVRES_PRI5(x)                (((x)&0x7)<<0xF)
#define MCF_ESW_PVRES_PRI6(x)                (((x)&0x7)<<0x12)
#define MCF_ESW_PVRES_PRI7(x)                (((x)&0x7)<<0x15)

/* Bit definitions and macros for MCF_ESW_IPRES */
#define MCF_ESW_IPRES_ADDRESS(x)             (((x)&0xFF)<<0)
#define MCF_ESW_IPRES_IPV4SEL                (0x100)
#define MCF_ESW_IPRES_PRI0(x)                (((x)&0x3)<<0x9)
#define MCF_ESW_IPRES_PRI1(x)                (((x)&0x3)<<0xB)
#define MCF_ESW_IPRES_PRI2(x)                (((x)&0x3)<<0xD)
#define MCF_ESW_IPRES_READ                   (0x80000000)

/* Bit definitions and macros for MCF_ESW_PRES */
#define MCF_ESW_PRES_VLAN                    (0x1)
#define MCF_ESW_PRES_IP                      (0x2)
#define MCF_ESW_PRES_MAC                     (0x4)
#define MCF_ESW_PRES_DFLT_PRI(x)             (((x)&0x7)<<0x4)

/* Bit definitions and macros for MCF_ESW_PID */
#define MCF_ESW_PID_VLANID(x)                (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_VRES */
#define MCF_ESW_VRES_P0                      (0x1)
#define MCF_ESW_VRES_P1                      (0x2)
#define MCF_ESW_VRES_P2                      (0x4)
#define MCF_ESW_VRES_VLANID(x)               (((x)&0x1FFF)<<0x3)

/* Bit definitions and macros for MCF_ESW_DISCN */
#define MCF_ESW_DISCN_COUNT(x)               (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_DISCB */
#define MCF_ESW_DISCB_COUNT(x)               (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_NDISCN */
#define MCF_ESW_NDISCN_COUNT(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_NDISCB */
#define MCF_ESW_NDISCB_COUNT(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_POQC */
#define MCF_ESW_POQC_COUNT(x)                (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_PMVID */
#define MCF_ESW_PMVID_COUNT(x)               (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_PMVTAG */
#define MCF_ESW_PMVTAG_COUNT(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_PBL */
#define MCF_ESW_PBL_COUNT(x)                 (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_ISR */
#define MCF_ESW_ISR_EBERR                    (0x1)
#define MCF_ESW_ISR_RXB                      (0x2)
#define MCF_ESW_ISR_RXF                      (0x4)
#define MCF_ESW_ISR_TXB                      (0x8)
#define MCF_ESW_ISR_TXF                      (0x10)
#define MCF_ESW_ISR_QM                       (0x20)
#define MCF_ESW_ISR_OD0                      (0x40)
#define MCF_ESW_ISR_OD1                      (0x80)
#define MCF_ESW_ISR_OD2                      (0x100)
#define MCF_ESW_ISR_LRN                      (0x200)

/* Bit definitions and macros for MCF_ESW_IMR */
#define MCF_ESW_IMR_EBERR                    (0x1)
#define MCF_ESW_IMR_RXB                      (0x2)
#define MCF_ESW_IMR_RXF                      (0x4)
#define MCF_ESW_IMR_TXB                      (0x8)
#define MCF_ESW_IMR_TXF                      (0x10)
#define MCF_ESW_IMR_QM                       (0x20)
#define MCF_ESW_IMR_OD0                      (0x40)
#define MCF_ESW_IMR_OD1                      (0x80)
#define MCF_ESW_IMR_OD2                      (0x100)
#define MCF_ESW_IMR_LRN                      (0x200)

/* Bit definitions and macros for MCF_ESW_RDSR */
#define MCF_ESW_RDSR_ADDRESS(x)              (((x)&0x3FFFFFFF)<<0x2)

/* Bit definitions and macros for MCF_ESW_TDSR */
#define MCF_ESW_TDSR_ADDRESS(x)              (((x)&0x3FFFFFFF)<<0x2)

/* Bit definitions and macros for MCF_ESW_MRBR */
#define MCF_ESW_MRBR_SIZE(x)                 (((x)&0x3FF)<<0x4)

/* Bit definitions and macros for MCF_ESW_RDAR */
#define MCF_ESW_RDAR_RDAR                    (0x1000000)

/* Bit definitions and macros for MCF_ESW_TDAR */
#define MCF_ESW_TDAR_TDAR                    (0x1000000)

/* Bit definitions and macros for MCF_ESW_LREC0 */
#define MCF_ESW_LREC0_MAC_ADDR0(x)           (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ESW_LREC1 */
#define MCF_ESW_LREC1_MAC_ADDR1(x)           (((x)&0xFFFF)<<0)
#define MCF_ESW_LREC1_HASH(x)                (((x)&0xFF)<<0x10)
#define MCF_ESW_LREC1_SWPORT(x)              (((x)&0x3)<<0x18)

/* Bit definitions and macros for MCF_ESW_LSR */
#define MCF_ESW_LSR_DA                       (0x1)


#endif /* __MCF54418_ESW_H__ */
