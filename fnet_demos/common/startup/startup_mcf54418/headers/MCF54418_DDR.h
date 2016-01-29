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

#ifndef __MCF54418_DDR_H__
#define __MCF54418_DDR_H__


/*********************************************************************
*
* DDR1/2 SDRAM Memory Controller (DDRMC)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_DDR_CR00                         (*(vuint32*)(0xFC0B8000))
#define MCF_DDR_CR01                         (*(vuint32*)(0xFC0B8004))
#define MCF_DDR_CR02                         (*(vuint32*)(0xFC0B8008))
#define MCF_DDR_CR03                         (*(vuint32*)(0xFC0B800C))
#define MCF_DDR_CR04                         (*(vuint32*)(0xFC0B8010))
#define MCF_DDR_CR05                         (*(vuint32*)(0xFC0B8014))
#define MCF_DDR_CR06                         (*(vuint32*)(0xFC0B8018))
#define MCF_DDR_CR07                         (*(vuint32*)(0xFC0B801C))
#define MCF_DDR_CR08                         (*(vuint32*)(0xFC0B8020))
#define MCF_DDR_CR09                         (*(vuint32*)(0xFC0B8024))
#define MCF_DDR_CR10                         (*(vuint32*)(0xFC0B8028))
#define MCF_DDR_CR11                         (*(vuint32*)(0xFC0B802C))
#define MCF_DDR_CR12                         (*(vuint32*)(0xFC0B8030))
#define MCF_DDR_CR13                         (*(vuint32*)(0xFC0B8034))
#define MCF_DDR_CR14                         (*(vuint32*)(0xFC0B8038))
#define MCF_DDR_CR15                         (*(vuint32*)(0xFC0B803C))
#define MCF_DDR_CR16                         (*(vuint32*)(0xFC0B8040))
#define MCF_DDR_CR17                         (*(vuint32*)(0xFC0B8044))
#define MCF_DDR_CR18                         (*(vuint32*)(0xFC0B8048))
#define MCF_DDR_CR19                         (*(vuint32*)(0xFC0B804C))
#define MCF_DDR_CR20                         (*(vuint32*)(0xFC0B8050))
#define MCF_DDR_CR21                         (*(vuint32*)(0xFC0B8054))
#define MCF_DDR_CR22                         (*(vuint32*)(0xFC0B8058))
#define MCF_DDR_CR23                         (*(vuint32*)(0xFC0B805C))
#define MCF_DDR_CR24                         (*(vuint32*)(0xFC0B8060))
#define MCF_DDR_CR25                         (*(vuint32*)(0xFC0B8064))
#define MCF_DDR_CR26                         (*(vuint32*)(0xFC0B8068))
#define MCF_DDR_CR27                         (*(vuint32*)(0xFC0B806C))
#define MCF_DDR_CR28                         (*(vuint32*)(0xFC0B8070))
#define MCF_DDR_CR29                         (*(vuint32*)(0xFC0B8074))
#define MCF_DDR_CR30                         (*(vuint32*)(0xFC0B8078))
#define MCF_DDR_CR31                         (*(vuint32*)(0xFC0B807C))
#define MCF_DDR_CR32                         (*(vuint32*)(0xFC0B8080))
#define MCF_DDR_CR33                         (*(vuint32*)(0xFC0B8084))
#define MCF_DDR_CR34                         (*(vuint32*)(0xFC0B8088))
#define MCF_DDR_CR35                         (*(vuint32*)(0xFC0B808C))
#define MCF_DDR_CR36                         (*(vuint32*)(0xFC0B8090))
#define MCF_DDR_CR37                         (*(vuint32*)(0xFC0B8094))
#define MCF_DDR_CR38                         (*(vuint32*)(0xFC0B8098))
#define MCF_DDR_CR39                         (*(vuint32*)(0xFC0B809C))
#define MCF_DDR_CR40                         (*(vuint32*)(0xFC0B80A0))
#define MCF_DDR_CR41                         (*(vuint32*)(0xFC0B80A4))
#define MCF_DDR_CR42                         (*(vuint32*)(0xFC0B80A8))
#define MCF_DDR_CR43                         (*(vuint32*)(0xFC0B80AC))
#define MCF_DDR_CR44                         (*(vuint32*)(0xFC0B80B0))
#define MCF_DDR_CR45                         (*(vuint32*)(0xFC0B80B4))
#define MCF_DDR_CR53                         (*(vuint32*)(0xFC0B80D4))
#define MCF_DDR_CR55                         (*(vuint32*)(0xFC0B80DC))
#define MCF_DDR_CR56                         (*(vuint32*)(0xFC0B80E0))
#define MCF_DDR_CR57                         (*(vuint32*)(0xFC0B80E4))
#define MCF_DDR_CR58                         (*(vuint32*)(0xFC0B80E8))
#define MCF_DDR_CR59                         (*(vuint32*)(0xFC0B80EC))
#define MCF_DDR_CR60                         (*(vuint32*)(0xFC0B80F0))
#define MCF_DDR_RCR_CTRL                     (*(vuint32*)(0xFC0B8180))
#define MCF_DDR_PADCR                        (*(vuint32*)(0xFC0B81AC))


/* Bit definitions and macros for MCF_DDR_CR00 */
#define MCF_DDR_CR00_AGE                     (0x1)
#define MCF_DDR_CR00_AGE_DIS                 (0)
#define MCF_DDR_CR00_AGE_EN                  (0x1)
#define MCF_DDR_CR00_ADDCOL                  (0x100)
#define MCF_DDR_CR00_ADDCOL_DIS              (0)
#define MCF_DDR_CR00_ADDCOL_EN               (0x100)
#define MCF_DDR_CR00_RP0                     (0x10000)
#define MCF_DDR_CR00_RP0_HIGH                (0)
#define MCF_DDR_CR00_RP0_LOW                 (0x10000)
#define MCF_DDR_CR00_WP0                     (0x1000000)
#define MCF_DDR_CR00_WP0_HIGH                (0)
#define MCF_DDR_CR00_WP0_LOW                 (0x1000000)

/* Bit definitions and macros for MCF_DDR_CR01 */
#define MCF_DDR_CR01_RP1                     (0x1)
#define MCF_DDR_CR01_RP1_HIGH                (0)
#define MCF_DDR_CR01_RP1_LOW                 (0x1)
#define MCF_DDR_CR01_WP1                     (0x100)
#define MCF_DDR_CR01_WP1_HIGH                (0)
#define MCF_DDR_CR01_WP1_LOW                 (0x100)
#define MCF_DDR_CR01_AP                      (0x10000)
#define MCF_DDR_CR01_AREF                    (0x1000000)

/* Bit definitions and macros for MCF_DDR_CR02 */
#define MCF_DDR_CR02_AREFMODE                (0x1)
#define MCF_DDR_CR02_BNKSPT                  (0x100)
#define MCF_DDR_CR02_BNKSPT_DIS              (0)
#define MCF_DDR_CR02_BNKSPT_EN               (0x100)
#define MCF_DDR_CR02_BIGEND                  (0x10000)
#define MCF_DDR_CR02_BIGEND_LITTLE           (0)
#define MCF_DDR_CR02_BIGEND_BIG              (0x10000)
#define MCF_DDR_CR02_CCAPEN                  (0x1000000)
#define MCF_DDR_CR02_CCAPEN_DIS              (0)
#define MCF_DDR_CR02_CCAPEN_EN               (0x1000000)

/* Bit definitions and macros for MCF_DDR_CR03 */
#define MCF_DDR_CR03_DLLBYP                  (0x10000)
#define MCF_DDR_CR03_DQSBEN                  (0x1000000)
#define MCF_DDR_CR03_DQSBEN_SINGLE           (0)
#define MCF_DDR_CR03_DQSBEN_DIFF             (0x1000000)

/* Bit definitions and macros for MCF_DDR_CR04 */
#define MCF_DDR_CR04_DRVDQS                  (0x1)
#define MCF_DDR_CR04_DRVDQS_DEASSERT         (0)
#define MCF_DDR_CR04_DRVDQS_DRIVE            (0x1)
#define MCF_DDR_CR04_8BNK                    (0x100)
#define MCF_DDR_CR04_8BNK_4_BANKS            (0)
#define MCF_DDR_CR04_8BNK_8_BANKS            (0x100)
#define MCF_DDR_CR04_QKREF                   (0x10000)
#define MCF_DDR_CR04_QKREF_CONTINUE          (0)
#define MCF_DDR_CR04_QKREF_INTERRUPT         (0x10000)

/* Bit definitions and macros for MCF_DDR_CR05 */
#define MCF_DDR_CR05_FSTWR                   (0x1)
#define MCF_DDR_CR05_INTPRE                  (0x100)
#define MCF_DDR_CR05_INTRD                   (0x10000)
#define MCF_DDR_CR05_INTWR                   (0x1000000)

/* Bit definitions and macros for MCF_DDR_CR06 */
#define MCF_DDR_CR06_TACEN                   (0x100)
#define MCF_DDR_CR06_ODTEN                   (0x10000)
#define MCF_DDR_CR06_OORID                   (0x1000000)

/* Bit definitions and macros for MCF_DDR_CR07 */
#define MCF_DDR_CR07_PLEN                    (0x1)
#define MCF_DDR_CR07_PLEN_DIS                (0)
#define MCF_DDR_CR07_PLEN_EN                 (0x1)
#define MCF_DDR_CR07_PROTEN                  (0x100)
#define MCF_DDR_CR07_PROTEN_DIS              (0)
#define MCF_DDR_CR07_PROTEN_EN               (0x100)
#define MCF_DDR_CR07_ERRID                   (0x10000)
#define MCF_DDR_CR07_PD                      (0x1000000)
#define MCF_DDR_CR07_PD_FULL_POWER           (0)
#define MCF_DDR_CR07_PD_POWER_DOWN           (0x1000000)

/* Bit definitions and macros for MCF_DDR_CR08 */
#define MCF_DDR_CR08_PRIEN                   (0x1)
#define MCF_DDR_CR08_PRIEN_DIS               (0)
#define MCF_DDR_CR08_PRIEN_EN                (0x1)
#define MCF_DDR_CR08_PUREF                   (0x100)
#define MCF_DDR_CR08_PUREF_DIS               (0)
#define MCF_DDR_CR08_PUREF_EN                (0x100)
#define MCF_DDR_CR08_R2RTAC                  (0x10000)
#define MCF_DDR_CR08_R2RTAC_DIS              (0)
#define MCF_DDR_CR08_R2RTAC_EN               (0x10000)
#define MCF_DDR_CR08_REDUC                   (0x1000000)
#define MCF_DDR_CR08_REDUC_8BIT              (0x1000000)

/* Bit definitions and macros for MCF_DDR_CR09 */
#define MCF_DDR_CR09_REGDIMM                 (0x1)
#define MCF_DDR_CR09_REGDIMM_NORMAL          (0)
#define MCF_DDR_CR09_REGDIMM_EN              (0x1)
#define MCF_DDR_CR09_RWEN                    (0x100)
#define MCF_DDR_CR09_RWEN_DIS                (0)
#define MCF_DDR_CR09_RWEN_EN                 (0x100)
#define MCF_DDR_CR09_SREF                    (0x10000)
#define MCF_DDR_CR09_SREF_DIS                (0)
#define MCF_DDR_CR09_SREF_EN                 (0x10000)
#define MCF_DDR_CR09_START                   (0x1000000)

/* Bit definitions and macros for MCF_DDR_CR10 */
#define MCF_DDR_CR10_SWPEN                   (0x1)
#define MCF_DDR_CR10_SWPEN_DIS               (0)
#define MCF_DDR_CR10_SWPEN_EN                (0x1)
#define MCF_DDR_CR10_TRAS                    (0x100)
#define MCF_DDR_CR10_TREFEN                  (0x10000)
#define MCF_DDR_CR10_TREFEN_DIS              (0)
#define MCF_DDR_CR10_TREFEN_EN               (0x10000)
#define MCF_DDR_CR10_INTWBR                  (0x1000000)

/* Bit definitions and macros for MCF_DDR_CR11 */
#define MCF_DDR_CR11_WRMD                    (0x1)
#define MCF_DDR_CR11_P0TYP(x)                (((x)&0x3)<<0x8)
#define MCF_DDR_CR11_P0TYP_ASYNC             (0)
#define MCF_DDR_CR11_P0TYP_2_TO_1            (0x100)
#define MCF_DDR_CR11_P0TYP_1_TO_2            (0x200)
#define MCF_DDR_CR11_P0TYP_SYNC              (0x300)
#define MCF_DDR_CR11_P0ATT0(x)               (((x)&0x3)<<0x10)
#define MCF_DDR_CR11_P0ATT0_NO_ACCESS        (0)
#define MCF_DDR_CR11_P0ATT0_READ_ONLY        (0x10000)
#define MCF_DDR_CR11_P0ATT0_WRITE_ONLY       (0x20000)
#define MCF_DDR_CR11_P0ATT0_READ_WRITE       (0x30000)
#define MCF_DDR_CR11_P0ATT1(x)               (((x)&0x3)<<0x18)
#define MCF_DDR_CR11_P0ATT1_NO_ACCESS        (0)
#define MCF_DDR_CR11_P0ATT1_READ_ONLY        (0x1000000)
#define MCF_DDR_CR11_P0ATT1_WRITE_ONLY       (0x2000000)
#define MCF_DDR_CR11_P0ATT1_READ_WRITE       (0x3000000)

/* Bit definitions and macros for MCF_DDR_CR12 */
#define MCF_DDR_CR12_P1TYP(x)                (((x)&0x3)<<0)
#define MCF_DDR_CR12_P1TYP_ASYNC             (0)
#define MCF_DDR_CR12_P1TYP_2_TO_1            (0x1)
#define MCF_DDR_CR12_P1TYP_1_TO_2            (0x2)
#define MCF_DDR_CR12_P1TYP_SYNC              (0x3)
#define MCF_DDR_CR12_P1ATT0(x)               (((x)&0x3)<<0x8)
#define MCF_DDR_CR12_P1ATT0_NO_ACCESS        (0)
#define MCF_DDR_CR12_P1ATT0_READ_ONLY        (0x100)
#define MCF_DDR_CR12_P1ATT0_WRITE_ONLY       (0x200)
#define MCF_DDR_CR12_P1ATT0_READ_WRITE       (0x300)
#define MCF_DDR_CR12_P1ATT1(x)               (((x)&0x3)<<0x10)
#define MCF_DDR_CR12_P1ATT1_NO_ACCESS        (0)
#define MCF_DDR_CR12_P1ATT1_READ_ONLY        (0x10000)
#define MCF_DDR_CR12_P1ATT1_WRITE_ONLY       (0x20000)
#define MCF_DDR_CR12_P1ATT1_READ_WRITE       (0x30000)
#define MCF_DDR_CR12_CSMAP                   (0x1000000)

/* Bit definitions and macros for MCF_DDR_CR13 */
#define MCF_DDR_CR13_CLKDIS                  (0x1)
#define MCF_DDR_CR13_CLKDIS_EN               (0)
#define MCF_DDR_CR13_CLKDIS_DIS              (0x1)
#define MCF_DDR_CR13_LPREF                   (0x100)
#define MCF_DDR_CR13_LPREF_OCCUR             (0)
#define MCF_DDR_CR13_LPREF_DONT_OCCUR        (0x100)
#define MCF_DDR_CR13_CSMAX(x)                (((x)&0x3)<<0x10)
#define MCF_DDR_CR13_CSMAX_ZERO              (0)
#define MCF_DDR_CR13_CSMAX_ONE               (0x10000)
#define MCF_DDR_CR13_CSMAX_TWO               (0x20000)
#define MCF_DDR_CR13_ODTRDCS                 (0x1000000)

/* Bit definitions and macros for MCF_DDR_CR14 */
#define MCF_DDR_CR14_ODTWRCS                 (0x100)
#define MCF_DDR_CR14_OORTYP(x)               (((x)&0x3)<<0x18)

/* Bit definitions and macros for MCF_DDR_CR15 */
#define MCF_DDR_CR15_QFULL(x)                (((x)&0x3)<<0)
#define MCF_DDR_CR15_ODTRES(x)               (((x)&0x3)<<0x8)
#define MCF_DDR_CR15_ODTRES_DIS              (0)
#define MCF_DDR_CR15_ODTRES_75_OHM           (0x100)
#define MCF_DDR_CR15_ODTRES_150_OHM          (0x200)
#define MCF_DDR_CR15_ADDPINS(x)              (((x)&0x7)<<0x18)

/* Bit definitions and macros for MCF_DDR_CR16 */
#define MCF_DDR_CR16_AGECNT(x)               (((x)&0x7)<<0)
#define MCF_DDR_CR16_CASLAT(x)               (((x)&0x7)<<0x8)
#define MCF_DDR_CR16_CKEDLY(x)               (((x)&0x7)<<0x10)
#define MCF_DDR_CR16_COLSIZ(x)               (((x)&0x7)<<0x18)

/* Bit definitions and macros for MCF_DDR_CR17 */
#define MCF_DDR_CR17_CMDAGE(x)               (((x)&0x7)<<0)
#define MCF_DDR_CR17_PCERR0                  (0x100)
#define MCF_DDR_CR17_PCERR1                  (0x200)
#define MCF_DDR_CR17_PCERR2                  (0x400)
#define MCF_DDR_CR17_CKEPW(x)                (((x)&0x7)<<0x10)
#define MCF_DDR_CR17_DFICTLDLY(x)            (((x)&0xF)<<0x18)

/* Bit definitions and macros for MCF_DDR_CR18 */
#define MCF_DDR_CR18_DFICLKDIS(x)            (((x)&0x7)<<0)
#define MCF_DDR_CR18_DFICLKEN(x)             (((x)&0xF)<<0x8)
#define MCF_DDR_CR18_TRRD(x)                 (((x)&0x7)<<0x18)

/* Bit definitions and macros for MCF_DDR_CR19 */
#define MCF_DDR_CR19_TRTP(x)                 (((x)&0x7)<<0)
#define MCF_DDR_CR19_TWTR(x)                 (((x)&0xF)<<0x8)
#define MCF_DDR_CR19_WRLAT(x)                (((x)&0xF)<<0x10)
#define MCF_DDR_CR19_APREBIT(x)              (((x)&0xF)<<0x18)

/* Bit definitions and macros for MCF_DDR_CR20 */
#define MCF_DDR_CR20_LATLIN(x)               (((x)&0xF)<<0)
#define MCF_DDR_CR20_LATGATE(x)              (((x)&0xF)<<0x8)
#define MCF_DDR_CR20_AREFINIT(x)             (((x)&0xF)<<0x10)
#define MCF_DDR_CR20_MAXCOL(x)               (((x)&0xF)<<0x18)

/* Bit definitions and macros for MCF_DDR_CR21 */
#define MCF_DDR_CR21_TDAL(x)                 (((x)&0x1F)<<0)
#define MCF_DDR_CR21_CTRLUPDMIN(x)           (((x)&0xF)<<0x8)
#define MCF_DDR_CR21_PHYRDLAT(x)             (((x)&0xF)<<0x10)
#define MCF_DDR_CR21_PHYWRLAT(x)             (((x)&0xF)<<0x18)

/* Bit definitions and macros for MCF_DDR_CR22 */
#define MCF_DDR_CR22_PHYWRLATBASE(x)         (((x)&0xF)<<0)
#define MCF_DDR_CR22_RDDATAEN(x)             (((x)&0xF)<<0x8)
#define MCF_DDR_CR22_RDDATABASE(x)           (((x)&0xF)<<0x10)
#define MCF_DDR_CR22_TRP(x)                  (((x)&0xF)<<0x18)

/* Bit definitions and macros for MCF_DDR_CR23 */
#define MCF_DDR_CR23_TWR(x)                  (((x)&0x1F)<<0)
#define MCF_DDR_CR23_LPAUTO(x)               (((x)&0x1F)<<0x8)
#define MCF_DDR_CR23_LPAUTO_MODE5_EN         (0x100)
#define MCF_DDR_CR23_LPAUTO_MODE4_EN         (0x200)
#define MCF_DDR_CR23_LPAUTO_MODE3_EN         (0x400)
#define MCF_DDR_CR23_LPAUTO_MODE2_EN         (0x800)
#define MCF_DDR_CR23_LPAUTO_MODE1_EN         (0x1000)
#define MCF_DDR_CR23_LPCTRL(x)               (((x)&0x1F)<<0x10)
#define MCF_DDR_CR23_LPCTRL_MODE5_EN         (0x10000)
#define MCF_DDR_CR23_LPCTRL_MODE4_EN         (0x20000)
#define MCF_DDR_CR23_LPCTRL_MODE3_EN         (0x40000)
#define MCF_DDR_CR23_LPCTRL_MODE2_EN         (0x80000)
#define MCF_DDR_CR23_LPCTRL_MODE1_EN         (0x100000)
#define MCF_DDR_CR23_MAXROW(x)               (((x)&0x1F)<<0x18)

/* Bit definitions and macros for MCF_DDR_CR24 */
#define MCF_DDR_CR24_PDNCS(x)                (((x)&0x1F)<<0)
#define MCF_DDR_CR24_PDNCS_INCREMENT         (0x10)
#define MCF_DDR_CR24_PDNCS_DECREMENT         (0)
#define MCF_DDR_CR24_PUPCS(x)                (((x)&0x1F)<<0x8)
#define MCF_DDR_CR24_PUPCS_INCREMENT         (0x1000)
#define MCF_DDR_CR24_PUPCS_DECREMENT         (0)
#define MCF_DDR_CR24_TFAW(x)                 (((x)&0x1F)<<0x10)
#define MCF_DDR_CR24_TMRD(x)                 (((x)&0x1F)<<0x18)

/* Bit definitions and macros for MCF_DDR_CR25 */
#define MCF_DDR_CR25_TRC(x)                  (((x)&0x1F)<<0)
#define MCF_DDR_CR25_INTACK(x)               (((x)&0xFF)<<0x10)
#define MCF_DDR_CR25_TRASMIN(x)              (((x)&0xFF)<<0x18)

/* Bit definitions and macros for MCF_DDR_CR26 */
#define MCF_DDR_CR26_TRCD_INT(x)             (((x)&0xFF)<<0)
#define MCF_DDR_CR26_TRFC(x)                 (((x)&0xFF)<<0x8)
#define MCF_DDR_CR26_INTMASK(x)              (((x)&0x1FF)<<0x10)

/* Bit definitions and macros for MCF_DDR_CR27 */
#define MCF_DDR_CR27_INTSTATUS(x)            (((x)&0x1FF)<<0)
#define MCF_DDR_CR27_INTSTATUS_SINGLE_ACCESS (0x1)
#define MCF_DDR_CR27_INTSTATUS_MULTI_ACCESS  (0x2)
#define MCF_DDR_CR27_INTSTATUS_ERROR         (0x4)
#define MCF_DDR_CR27_INTSTATUS_DRAM_INIT     (0x8)
#define MCF_DDR_CR27_INTSTATUS_ADR_CROSS     (0x10)
#define MCF_DDR_CR27_INTSTATUS_LOGIC_OR      (0x100)
#define MCF_DDR_CR27_OORLEN(x)               (((x)&0x3FF)<<0x10)

/* Bit definitions and macros for MCF_DDR_CR28 */
#define MCF_DDR_CR28_P0RDCNT(x)              (((x)&0x7FF)<<0)
#define MCF_DDR_CR28_P0WRCNT(x)              (((x)&0x7FF)<<0x10)

/* Bit definitions and macros for MCF_DDR_CR29 */
#define MCF_DDR_CR29_P1RDCNT(x)              (((x)&0x7FF)<<0)
#define MCF_DDR_CR29_P1WRCNT(x)              (((x)&0x7FF)<<0x10)

/* Bit definitions and macros for MCF_DDR_CR30 */
#define MCF_DDR_CR30_CTRLUPDMAX(x)           (((x)&0x3FFF)<<0)
#define MCF_DDR_CR30_PHYUPDRESP(x)           (((x)&0x3FFF)<<0x10)

/* Bit definitions and macros for MCF_DDR_CR31 */
#define MCF_DDR_CR31_PHYUPD_TYPE0(x)         (((x)&0x3FFF)<<0)
#define MCF_DDR_CR31_TREF(x)                 (((x)&0x3FFF)<<0x10)

/* Bit definitions and macros for MCF_DDR_CR32 */
#define MCF_DDR_CR32_P0ENDAD0(x)             (((x)&0x7FFF)<<0x10)

/* Bit definitions and macros for MCF_DDR_CR33 */
#define MCF_DDR_CR33_P0ENDAD1(x)             (((x)&0x7FFF)<<0)
#define MCF_DDR_CR33_P0STAD0(x)              (((x)&0x7FFF)<<0x10)

/* Bit definitions and macros for MCF_DDR_CR34 */
#define MCF_DDR_CR34_P0STAD1(x)              (((x)&0x7FFF)<<0)
#define MCF_DDR_CR34_P1ENDAD0(x)             (((x)&0x7FFF)<<0x10)

/* Bit definitions and macros for MCF_DDR_CR35 */
#define MCF_DDR_CR35_P1ENDAD1(x)             (((x)&0x7FFF)<<0)
#define MCF_DDR_CR35_P1STAD0(x)              (((x)&0x7FFF)<<0x10)

/* Bit definitions and macros for MCF_DDR_CR36 */
#define MCF_DDR_CR36_P1STAD1(x)              (((x)&0x7FFF)<<0)

/* Bit definitions and macros for MCF_DDR_CR37 */
#define MCF_DDR_CR37_EMRS2D(x)               (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_DDR_CR38 */
#define MCF_DDR_CR38_LPEXTCNT(x)             (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_DDR_CR39 */
#define MCF_DDR_CR39_LPINTCNT(x)             (((x)&0xFFFF)<<0)
#define MCF_DDR_CR39_LPPDCNT(x)              (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_DDR_CR40 */
#define MCF_DDR_CR40_LPSREFCNT(x)            (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_DDR_CR41 */
#define MCF_DDR_CR41_TCPD(x)                 (((x)&0xFFFF)<<0)
#define MCF_DDR_CR41_TDLL(x)                 (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_DDR_CR42 */
#define MCF_DDR_CR42_TPDEX(x)                (((x)&0xFFFF)<<0)
#define MCF_DDR_CR42_TRASMAX(x)              (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_DDR_CR43 */
#define MCF_DDR_CR43_TXSNR(x)                (((x)&0xFFFF)<<0)
#define MCF_DDR_CR43_TXSR(x)                 (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_DDR_CR44 */
#define MCF_DDR_CR44_VERSION(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_DDR_CR45 */
#define MCF_DDR_CR45_TINIT(x)                (((x)&0xFFFFFF)<<0)

/* Bit definitions and macros for MCF_DDR_CR53 */
#define MCF_DDR_CR53_PCERRAD(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_DDR_CR55 */
#define MCF_DDR_CR55_OORAD(x)                (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_DDR_CR56 */
#define MCF_DDR_CR56_OORAD32                 (0x1)
#define MCF_DDR_CR56_W2RSAME(x)              (((x)&0x7)<<0x10)
#define MCF_DDR_CR56_DRAMCLASS(x)            (((x)&0xF)<<0x18)
#define MCF_DDR_CR56_DRAMCLASS_DDR1          (0)
#define MCF_DDR_CR56_DRAMCLASS_DDR2          (0x4000000)

/* Bit definitions and macros for MCF_DDR_CR57 */
#define MCF_DDR_CR57_RDLATADJ(x)             (((x)&0xF)<<0)
#define MCF_DDR_CR57_WRLATADJ(x)             (((x)&0xF)<<0x8)
#define MCF_DDR_CR57_TMOD(x)                 (((x)&0xFF)<<0x18)

/* Bit definitions and macros for MCF_DDR_CR58 */
#define MCF_DDR_CR58_EMRS1D(x)               (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_DDR_CR59 */
#define MCF_DDR_CR59_EMRS1D_B(x)             (((x)&0xFFFF)<<0)
#define MCF_DDR_CR59_EMRS3D(x)               (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_DDR_CR60 */
#define MCF_DDR_CR60_EMRS3D_B(x)             (((x)&0xFFFF)<<0)
#define MCF_DDR_CR60_MRSD(x)                 (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_DDR_RCR_CTRL */
#define MCF_DDR_RCR_CTRL_REF_UPDATE_EN       (0x10000000)
#define MCF_DDR_RCR_CTRL_HW_UPDATE_OFF       (0x20000000)
#define MCF_DDR_RCR_CTRL_RCR_RST             (0x40000000)
#define MCF_DDR_RCR_CTRL_RCR_OFF             (0x80000000)

/* Bit definitions and macros for MCF_DDR_PADCR */
#define MCF_DDR_PADCR_SPARE_DLY_CTRL1(x)     (((x)&0x3)<<0)
#define MCF_DDR_PADCR_SPARE_DLY_CTRL1_HIGH   (0x3)
#define MCF_DDR_PADCR_OFFSET_VALUE(x)        (((x)&0x3)<<0x10)
#define MCF_DDR_PADCR_OFFSET_VALUE_400_200_MV (0)
#define MCF_DDR_PADCR_OFFSET_VALUE_500_250_MV (0x10000)
#define MCF_DDR_PADCR_OFFSET_VALUE_600_300_MV (0x20000)
#define MCF_DDR_PADCR_OFFSET_VALUE_700_350_MV (0x30000)
#define MCF_DDR_PADCR_PAD_ODT_CS(x)          (((x)&0x3)<<0x18)
#define MCF_DDR_PADCR_PAD_ODT_CS_DIS         (0)
#define MCF_DDR_PADCR_PAD_ODT_CS_75_OHM      (0x1000000)
#define MCF_DDR_PADCR_PAD_ODT_CS_150_OHM     (0x2000000)
#define MCF_DDR_PADCR_PAD_ODT_CS_50_OHM      (0x3000000)


#endif /* __MCF54418_DDR_H__ */
