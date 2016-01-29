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

#ifndef __MCF54418_eSDHC_H__
#define __MCF54418_eSDHC_H__


/*********************************************************************
*
* Enhanced Secure Digital Host Controller (eSDHC)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_eSDHC_DSADDR                     (*(vuint32*)(0xFC0CC000))
#define MCF_eSDHC_BLKATTR                    (*(vuint32*)(0xFC0CC004))
#define MCF_eSDHC_CMDARG                     (*(vuint32*)(0xFC0CC008))
#define MCF_eSDHC_XFERTYP                    (*(vuint32*)(0xFC0CC00C))
#define MCF_eSDHC_CMDRSP0                    (*(vuint32*)(0xFC0CC010))
#define MCF_eSDHC_CMDRSP1                    (*(vuint32*)(0xFC0CC014))
#define MCF_eSDHC_CMDRSP2                    (*(vuint32*)(0xFC0CC018))
#define MCF_eSDHC_CMDRSP3                    (*(vuint32*)(0xFC0CC01C))
#define MCF_eSDHC_DATPORT                    (*(vuint32*)(0xFC0CC020))
#define MCF_eSDHC_PRSSTAT                    (*(vuint32*)(0xFC0CC024))
#define MCF_eSDHC_PROCTL                     (*(vuint32*)(0xFC0CC028))
#define MCF_eSDHC_SYSCTL                     (*(vuint32*)(0xFC0CC02C))
#define MCF_eSDHC_IRQSTAT                    (*(vuint32*)(0xFC0CC030))
#define MCF_eSDHC_IRQSTATEN                  (*(vuint32*)(0xFC0CC034))
#define MCF_eSDHC_IRQSIGEN                   (*(vuint32*)(0xFC0CC038))
#define MCF_eSDHC_AUTOC12ERR                 (*(vuint32*)(0xFC0CC03C))
#define MCF_eSDHC_HOSTCAPBLT                 (*(vuint32*)(0xFC0CC040))
#define MCF_eSDHC_WML                        (*(vuint32*)(0xFC0CC044))
#define MCF_eSDHC_FEVT                       (*(vuint32*)(0xFC0CC050))
#define MCF_eSDHC_ADMAESR                    (*(vuint32*)(0xFC0CC054))
#define MCF_eSDHC_ADMASAR                    (*(vuint32*)(0xFC0CC058))
#define MCF_eSDHC_VENDOR                     (*(vuint32*)(0xFC0CC0C0))
#define MCF_eSDHC_HOSTVER                    (*(vuint32*)(0xFC0CC0FC))
#define MCF_eSDHC_CMDRSP(x)                  (*(vuint32*)(0xFC0CC010 + ((x)*0x4)))


/* Bit definitions and macros for MCF_eSDHC_DSADDR */
#define MCF_eSDHC_DSADDR_DS_ADDR(x)          (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_eSDHC_BLKATTR */
#define MCF_eSDHC_BLKATTR_BLKSIZE(x)         (((x)&0x1FFF)<<0)
#define MCF_eSDHC_BLKATTR_BLKCNT(x)          (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_eSDHC_CMDARG */
#define MCF_eSDHC_CMDARG_CMDARG(x)           (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_eSDHC_XFERTYP */
#define MCF_eSDHC_XFERTYP_DMAEN              (0x1)
#define MCF_eSDHC_XFERTYP_BCEN               (0x2)
#define MCF_eSDHC_XFERTYP_AC12EN             (0x4)
#define MCF_eSDHC_XFERTYP_DTDSEL             (0x10)
#define MCF_eSDHC_XFERTYP_MSBSEL             (0x20)
#define MCF_eSDHC_XFERTYP_RSPTYP(x)          (((x)&0x3)<<0x10)
#define MCF_eSDHC_XFERTYP_RSPTYP_NORESP      (0)
#define MCF_eSDHC_XFERTYP_RSPTYP_RESP136     (0x10000)
#define MCF_eSDHC_XFERTYP_RSPTYP_RESP48      (0x20000)
#define MCF_eSDHC_XFERTYP_RSPTYP_RESP48_CHECK (0x30000)
#define MCF_eSDHC_XFERTYP_CCCEN              (0x80000)
#define MCF_eSDHC_XFERTYP_CICEN              (0x100000)
#define MCF_eSDHC_XFERTYP_DPSEL              (0x200000)
#define MCF_eSDHC_XFERTYP_CMDTYP(x)          (((x)&0x3)<<0x16)
#define MCF_eSDHC_XFERTYP_CMDTYP_NORMAL      (0)
#define MCF_eSDHC_XFERTYP_CMDTYP_SUSPEND     (0x400000)
#define MCF_eSDHC_XFERTYP_CMDTYP_RESUME      (0x800000)
#define MCF_eSDHC_XFERTYP_CMDTYP_ABORT       (0xC00000)
#define MCF_eSDHC_XFERTYP_CMDINX(x)          (((x)&0x3F)<<0x18)

/* Bit definitions and macros for MCF_eSDHC_CMDRSP */
#define MCF_eSDHC_CMDRSP_CMDRSP(x)           (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_eSDHC_DATPORT */
#define MCF_eSDHC_DATPORT_DATCONT(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_eSDHC_PRSSTAT */
#define MCF_eSDHC_PRSSTAT_CIHB               (0x1)
#define MCF_eSDHC_PRSSTAT_CDIHB              (0x2)
#define MCF_eSDHC_PRSSTAT_DLA                (0x4)
#define MCF_eSDHC_PRSSTAT_SDSTB              (0x8)
#define MCF_eSDHC_PRSSTAT_IPGOFF             (0x10)
#define MCF_eSDHC_PRSSTAT_HCKOFF             (0x20)
#define MCF_eSDHC_PRSSTAT_PEROFF             (0x40)
#define MCF_eSDHC_PRSSTAT_SDOFF              (0x80)
#define MCF_eSDHC_PRSSTAT_WTA                (0x100)
#define MCF_eSDHC_PRSSTAT_RTA                (0x200)
#define MCF_eSDHC_PRSSTAT_BWEN               (0x400)
#define MCF_eSDHC_PRSSTAT_BREN               (0x800)
#define MCF_eSDHC_PRSSTAT_CINS               (0x10000)
#define MCF_eSDHC_PRSSTAT_CLSL               (0x800000)
#define MCF_eSDHC_PRSSTAT_DLSL(x)            (((x)&0xF)<<0x18)

/* Bit definitions and macros for MCF_eSDHC_PROCTL */
#define MCF_eSDHC_PROCTL_DTW(x)              (((x)&0x3)<<0x1)
#define MCF_eSDHC_PROCTL_DTW_1BIT            (0)
#define MCF_eSDHC_PROCTL_DTW_4BIT            (0x2)
#define MCF_eSDHC_PROCTL_DTW_8BIT            (0x4)
#define MCF_eSDHC_PROCTL_D3CD                (0x8)
#define MCF_eSDHC_PROCTL_EMODE(x)            (((x)&0x3)<<0x4)
#define MCF_eSDHC_PROCTL_EMODE_ADDR_INVARIANT (0x20)
#define MCF_eSDHC_PROCTL_CDTL                (0x40)
#define MCF_eSDHC_PROCTL_CDSS                (0x80)
#define MCF_eSDHC_PROCTL_DMAS(x)             (((x)&0x3)<<0x8)
#define MCF_eSDHC_PROCTL_DMAS_SIMPLE         (0)
#define MCF_eSDHC_PROCTL_DMAS_ADVANCED       (0x100)
#define MCF_eSDHC_PROCTL_DMAS_ADVANCED2      (0x200)
#define MCF_eSDHC_PROCTL_SABGREQ             (0x10000)
#define MCF_eSDHC_PROCTL_CREQ                (0x20000)
#define MCF_eSDHC_PROCTL_RWCTL               (0x40000)
#define MCF_eSDHC_PROCTL_IABG                (0x80000)
#define MCF_eSDHC_PROCTL_WECINT              (0x1000000)
#define MCF_eSDHC_PROCTL_WECINS              (0x2000000)
#define MCF_eSDHC_PROCTL_WECRM               (0x4000000)

/* Bit definitions and macros for MCF_eSDHC_SYSCTL */
#define MCF_eSDHC_SYSCTL_IPGEN               (0x1)
#define MCF_eSDHC_SYSCTL_HCKEN               (0x2)
#define MCF_eSDHC_SYSCTL_PEREN               (0x4)
#define MCF_eSDHC_SYSCTL_SDCLKEN             (0x8)
#define MCF_eSDHC_SYSCTL_DVS(x)              (((x)&0xF)<<0x4)
#define MCF_eSDHC_SYSCTL_DVS_DIV_BY_1        (0)
#define MCF_eSDHC_SYSCTL_DVS_DIV_BY_2        (0x10)
#define MCF_eSDHC_SYSCTL_DVS_DIV_BY_3        (0x20)
#define MCF_eSDHC_SYSCTL_DVS_DIV_BY_4        (0x30)
#define MCF_eSDHC_SYSCTL_DVS_DIV_BY_5        (0x40)
#define MCF_eSDHC_SYSCTL_DVS_DIV_BY_6        (0x50)
#define MCF_eSDHC_SYSCTL_DVS_DIV_BY_7        (0x60)
#define MCF_eSDHC_SYSCTL_DVS_DIV_BY_8        (0x70)
#define MCF_eSDHC_SYSCTL_DVS_DIV_BY_9        (0x80)
#define MCF_eSDHC_SYSCTL_DVS_DIV_BY_10       (0x90)
#define MCF_eSDHC_SYSCTL_DVS_DIV_BY_11       (0xA0)
#define MCF_eSDHC_SYSCTL_DVS_DIV_BY_12       (0xB0)
#define MCF_eSDHC_SYSCTL_DVS_DIV_BY_13       (0xC0)
#define MCF_eSDHC_SYSCTL_DVS_DIV_BY_14       (0xD0)
#define MCF_eSDHC_SYSCTL_DVS_DIV_BY_15       (0xE0)
#define MCF_eSDHC_SYSCTL_DVS_DIV_BY_16       (0xF0)
#define MCF_eSDHC_SYSCTL_SDCLKFS(x)          (((x)&0xFF)<<0x8)
#define MCF_eSDHC_SYSCTL_SDCLKFS_DIV2        (0x100)
#define MCF_eSDHC_SYSCTL_SDCLKFS_DIV4        (0x200)
#define MCF_eSDHC_SYSCTL_SDCLKFS_DIV8        (0x400)
#define MCF_eSDHC_SYSCTL_SDCLKFS_DIV16       (0x800)
#define MCF_eSDHC_SYSCTL_SDCLKFS_DIV32       (0x1000)
#define MCF_eSDHC_SYSCTL_SDCLKFS_DIV64       (0x2000)
#define MCF_eSDHC_SYSCTL_SDCLKFS_DIV128      (0x4000)
#define MCF_eSDHC_SYSCTL_SDCLKFS_DIV256      (0x8000)
#define MCF_eSDHC_SYSCTL_DTOCV(x)            (((x)&0xF)<<0x10)
#define MCF_eSDHC_SYSCTL_DTOCV_CLK_2_13      (0)
#define MCF_eSDHC_SYSCTL_DTOCV_CLK_2_14      (0x10000)
#define MCF_eSDHC_SYSCTL_DTOCV_CLK_2_15      (0x20000)
#define MCF_eSDHC_SYSCTL_DTOCV_CLK_2_16      (0x30000)
#define MCF_eSDHC_SYSCTL_DTOCV_CLK_2_17      (0x40000)
#define MCF_eSDHC_SYSCTL_DTOCV_CLK_2_18      (0x50000)
#define MCF_eSDHC_SYSCTL_DTOCV_CLK_2_19      (0x60000)
#define MCF_eSDHC_SYSCTL_DTOCV_CLK_2_20      (0x70000)
#define MCF_eSDHC_SYSCTL_DTOCV_CLK_2_21      (0x80000)
#define MCF_eSDHC_SYSCTL_DTOCV_CLK_2_22      (0x90000)
#define MCF_eSDHC_SYSCTL_DTOCV_CLK_2_23      (0xA0000)
#define MCF_eSDHC_SYSCTL_DTOCV_CLK_2_24      (0xB0000)
#define MCF_eSDHC_SYSCTL_DTOCV_CLK_2_25      (0xC0000)
#define MCF_eSDHC_SYSCTL_DTOCV_CLK_2_26      (0xD0000)
#define MCF_eSDHC_SYSCTL_DTOCV_CLK_2_27      (0xE0000)
#define MCF_eSDHC_SYSCTL_RSTA                (0x1000000)
#define MCF_eSDHC_SYSCTL_RSTC                (0x2000000)
#define MCF_eSDHC_SYSCTL_RSTD                (0x4000000)
#define MCF_eSDHC_SYSCTL_INITA               (0x8000000)

/* Bit definitions and macros for MCF_eSDHC_IRQSTAT */
#define MCF_eSDHC_IRQSTAT_CC                 (0x1)
#define MCF_eSDHC_IRQSTAT_TC                 (0x2)
#define MCF_eSDHC_IRQSTAT_BGE                (0x4)
#define MCF_eSDHC_IRQSTAT_DINT               (0x8)
#define MCF_eSDHC_IRQSTAT_BWR                (0x10)
#define MCF_eSDHC_IRQSTAT_BRR                (0x20)
#define MCF_eSDHC_IRQSTAT_CINS               (0x40)
#define MCF_eSDHC_IRQSTAT_CRM                (0x80)
#define MCF_eSDHC_IRQSTAT_CINT               (0x100)
#define MCF_eSDHC_IRQSTAT_CTOE               (0x10000)
#define MCF_eSDHC_IRQSTAT_CCE                (0x20000)
#define MCF_eSDHC_IRQSTAT_CEBE               (0x40000)
#define MCF_eSDHC_IRQSTAT_CIE                (0x80000)
#define MCF_eSDHC_IRQSTAT_DTOE               (0x100000)
#define MCF_eSDHC_IRQSTAT_DCE                (0x200000)
#define MCF_eSDHC_IRQSTAT_DEBE               (0x400000)
#define MCF_eSDHC_IRQSTAT_AC12E              (0x1000000)
#define MCF_eSDHC_IRQSTAT_DMAE               (0x10000000)

/* Bit definitions and macros for MCF_eSDHC_IRQSTATEN */
#define MCF_eSDHC_IRQSTATEN_CCSEN            (0x1)
#define MCF_eSDHC_IRQSTATEN_TCSEN            (0x2)
#define MCF_eSDHC_IRQSTATEN_BGESEN           (0x4)
#define MCF_eSDHC_IRQSTATEN_DINTSEN          (0x8)
#define MCF_eSDHC_IRQSTATEN_BWRSEN           (0x10)
#define MCF_eSDHC_IRQSTATEN_BRRSEN           (0x20)
#define MCF_eSDHC_IRQSTATEN_CINSSEN          (0x40)
#define MCF_eSDHC_IRQSTATEN_CRMSEN           (0x80)
#define MCF_eSDHC_IRQSTATEN_CINTSEN          (0x100)
#define MCF_eSDHC_IRQSTATEN_CTOESEN          (0x10000)
#define MCF_eSDHC_IRQSTATEN_CCESEN           (0x20000)
#define MCF_eSDHC_IRQSTATEN_CEBESEN          (0x40000)
#define MCF_eSDHC_IRQSTATEN_CIESEN           (0x80000)
#define MCF_eSDHC_IRQSTATEN_DTOESEN          (0x100000)
#define MCF_eSDHC_IRQSTATEN_DCESEN           (0x200000)
#define MCF_eSDHC_IRQSTATEN_DEBESEN          (0x400000)
#define MCF_eSDHC_IRQSTATEN_AC12ESEN         (0x1000000)
#define MCF_eSDHC_IRQSTATEN_DMAESEN          (0x10000000)

/* Bit definitions and macros for MCF_eSDHC_IRQSIGEN */
#define MCF_eSDHC_IRQSIGEN_CCIEN             (0x1)
#define MCF_eSDHC_IRQSIGEN_TCIEN             (0x2)
#define MCF_eSDHC_IRQSIGEN_BGEIEN            (0x4)
#define MCF_eSDHC_IRQSIGEN_DINTIEN           (0x8)
#define MCF_eSDHC_IRQSIGEN_BWRIEN            (0x10)
#define MCF_eSDHC_IRQSIGEN_BRRIEN            (0x20)
#define MCF_eSDHC_IRQSIGEN_CINSIEN           (0x40)
#define MCF_eSDHC_IRQSIGEN_CRMIEN            (0x80)
#define MCF_eSDHC_IRQSIGEN_CINTIEN           (0x100)
#define MCF_eSDHC_IRQSIGEN_CTOEIEN           (0x10000)
#define MCF_eSDHC_IRQSIGEN_CCEIEN            (0x20000)
#define MCF_eSDHC_IRQSIGEN_CEBEIEN           (0x40000)
#define MCF_eSDHC_IRQSIGEN_CIEIEN            (0x80000)
#define MCF_eSDHC_IRQSIGEN_DTOEIEN           (0x100000)
#define MCF_eSDHC_IRQSIGEN_DCEIEN            (0x200000)
#define MCF_eSDHC_IRQSIGEN_DEBEIEN           (0x400000)
#define MCF_eSDHC_IRQSIGEN_AC12EIEN          (0x1000000)
#define MCF_eSDHC_IRQSIGEN_DMAEIEN           (0x10000000)

/* Bit definitions and macros for MCF_eSDHC_AUTOC12ERR */
#define MCF_eSDHC_AUTOC12ERR_AC12NE          (0x1)
#define MCF_eSDHC_AUTOC12ERR_AC12TOE         (0x2)
#define MCF_eSDHC_AUTOC12ERR_AC12EBE         (0x4)
#define MCF_eSDHC_AUTOC12ERR_AC12CE          (0x8)
#define MCF_eSDHC_AUTOC12ERR_AC12IE          (0x10)
#define MCF_eSDHC_AUTOC12ERR_CNIBAC12E       (0x80)

/* Bit definitions and macros for MCF_eSDHC_HOSTCAPBLT */
#define MCF_eSDHC_HOSTCAPBLT_MBL(x)          (((x)&0x7)<<0x10)
#define MCF_eSDHC_HOSTCAPBLT_MBL_512         (0)
#define MCF_eSDHC_HOSTCAPBLT_MBL_1024        (0x10000)
#define MCF_eSDHC_HOSTCAPBLT_MBL_2048        (0x20000)
#define MCF_eSDHC_HOSTCAPBLT_MBL_4096        (0x30000)
#define MCF_eSDHC_HOSTCAPBLT_ADMAS           (0x100000)
#define MCF_eSDHC_HOSTCAPBLT_HSS             (0x200000)
#define MCF_eSDHC_HOSTCAPBLT_DMAS            (0x400000)
#define MCF_eSDHC_HOSTCAPBLT_SRS             (0x800000)
#define MCF_eSDHC_HOSTCAPBLT_VS33            (0x1000000)
#define MCF_eSDHC_HOSTCAPBLT_VS30            (0x2000000)
#define MCF_eSDHC_HOSTCAPBLT_VS18            (0x4000000)

/* Bit definitions and macros for MCF_eSDHC_WML */
#define MCF_eSDHC_WML_RD_WML(x)              (((x)&0xFF)<<0)
#define MCF_eSDHC_WML_WR_WML(x)              (((x)&0xFF)<<0x10)

/* Bit definitions and macros for MCF_eSDHC_FEVT */
#define MCF_eSDHC_FEVT_FEVTAC12NE            (0x1)
#define MCF_eSDHC_FEVT_FEVTAC12TOE           (0x2)
#define MCF_eSDHC_FEVT_FEVTAC12CE            (0x4)
#define MCF_eSDHC_FEVT_FEVTAC12EBE           (0x8)
#define MCF_eSDHC_FEVT_FEVTAC12IE            (0x10)
#define MCF_eSDHC_FEVT_FEVTCNIBAC12E         (0x80)
#define MCF_eSDHC_FEVT_FEVTCTOE              (0x10000)
#define MCF_eSDHC_FEVT_FEVTCCE               (0x20000)
#define MCF_eSDHC_FEVT_FEVTCEBE              (0x40000)
#define MCF_eSDHC_FEVT_FEVTCIE               (0x80000)
#define MCF_eSDHC_FEVT_FEVTDTOE              (0x100000)
#define MCF_eSDHC_FEVT_FEVTDCE               (0x200000)
#define MCF_eSDHC_FEVT_FEVTDEBE              (0x400000)
#define MCF_eSDHC_FEVT_FEVTAC12E             (0x1000000)
#define MCF_eSDHC_FEVT_FEVTDMAE              (0x10000000)
#define MCF_eSDHC_FEVT_FEVTCINT              (0x80000000)

/* Bit definitions and macros for MCF_eSDHC_ADMAESR */
#define MCF_eSDHC_ADMAESR_ES(x)              (((x)&0x3)<<0)
#define MCF_eSDHC_ADMAESR_ES_ST_STOP         (0)
#define MCF_eSDHC_ADMAESR_ES_ST_FDS          (0x1)
#define MCF_eSDHC_ADMAESR_ES_ST_CADR         (0x2)
#define MCF_eSDHC_ADMAESR_ES_ST_TFR          (0x3)
#define MCF_eSDHC_ADMAESR_LME                (0x4)
#define MCF_eSDHC_ADMAESR_DCE                (0x8)

/* Bit definitions and macros for MCF_eSDHC_ADMASAR */
#define MCF_eSDHC_ADMASAR_ADDR(x)            (((x)&0x3FFFFFFF)<<0x2)

/* Bit definitions and macros for MCF_eSDHC_VENDOR */
#define MCF_eSDHC_VENDOR_EXT_DMA_EN          (0x1)

/* Bit definitions and macros for MCF_eSDHC_HOSTVER */
#define MCF_eSDHC_HOSTVER_SVN(x)             (((x)&0xFF)<<0)
#define MCF_eSDHC_HOSTVER_VVN(x)             (((x)&0xFF)<<0x8)


#endif /* __MCF54418_eSDHC_H__ */
