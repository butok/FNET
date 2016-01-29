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

#ifndef __MCF54418_ADC_H__
#define __MCF54418_ADC_H__


/*********************************************************************
*
* Analog-to-Digital Converter (ADC)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_ADC_CR1                          (*(vuint16*)(0xFC094000))
#define MCF_ADC_CR2                          (*(vuint16*)(0xFC094002))
#define MCF_ADC_ZCCR                         (*(vuint16*)(0xFC094004))
#define MCF_ADC_LST1                         (*(vuint16*)(0xFC094006))
#define MCF_ADC_LST2                         (*(vuint16*)(0xFC094008))
#define MCF_ADC_SDIS                         (*(vuint16*)(0xFC09400A))
#define MCF_ADC_SR                           (*(vuint16*)(0xFC09400C))
#define MCF_ADC_LSR                          (*(vuint16*)(0xFC09400E))
#define MCF_ADC_ZCSR                         (*(vuint16*)(0xFC094010))
#define MCF_ADC_RSLT0                        (*(vuint16*)(0xFC094012))
#define MCF_ADC_RSLT1                        (*(vuint16*)(0xFC094014))
#define MCF_ADC_RSLT2                        (*(vuint16*)(0xFC094016))
#define MCF_ADC_RSLT3                        (*(vuint16*)(0xFC094018))
#define MCF_ADC_RSLT4                        (*(vuint16*)(0xFC09401A))
#define MCF_ADC_RSLT5                        (*(vuint16*)(0xFC09401C))
#define MCF_ADC_RSLT6                        (*(vuint16*)(0xFC09401E))
#define MCF_ADC_RSLT7                        (*(vuint16*)(0xFC094020))
#define MCF_ADC_LLMT0                        (*(vuint16*)(0xFC094022))
#define MCF_ADC_LLMT1                        (*(vuint16*)(0xFC094024))
#define MCF_ADC_LLMT2                        (*(vuint16*)(0xFC094026))
#define MCF_ADC_LLMT3                        (*(vuint16*)(0xFC094028))
#define MCF_ADC_LLMT4                        (*(vuint16*)(0xFC09402A))
#define MCF_ADC_LLMT5                        (*(vuint16*)(0xFC09402C))
#define MCF_ADC_LLMT6                        (*(vuint16*)(0xFC09402E))
#define MCF_ADC_LLMT7                        (*(vuint16*)(0xFC094030))
#define MCF_ADC_HLMT0                        (*(vuint16*)(0xFC094032))
#define MCF_ADC_HLMT1                        (*(vuint16*)(0xFC094034))
#define MCF_ADC_HLMT2                        (*(vuint16*)(0xFC094036))
#define MCF_ADC_HLMT3                        (*(vuint16*)(0xFC094038))
#define MCF_ADC_HLMT4                        (*(vuint16*)(0xFC09403A))
#define MCF_ADC_HLMT5                        (*(vuint16*)(0xFC09403C))
#define MCF_ADC_HLMT6                        (*(vuint16*)(0xFC09403E))
#define MCF_ADC_HLMT7                        (*(vuint16*)(0xFC094040))
#define MCF_ADC_OFS0                         (*(vuint16*)(0xFC094042))
#define MCF_ADC_OFS1                         (*(vuint16*)(0xFC094044))
#define MCF_ADC_OFS2                         (*(vuint16*)(0xFC094046))
#define MCF_ADC_OFS3                         (*(vuint16*)(0xFC094048))
#define MCF_ADC_OFS4                         (*(vuint16*)(0xFC09404A))
#define MCF_ADC_OFS5                         (*(vuint16*)(0xFC09404C))
#define MCF_ADC_OFS6                         (*(vuint16*)(0xFC09404E))
#define MCF_ADC_OFS7                         (*(vuint16*)(0xFC094050))
#define MCF_ADC_PWR                          (*(vuint16*)(0xFC094052))
#define MCF_ADC_CAL                          (*(vuint16*)(0xFC094054))
#define MCF_ADC_PWR2                         (*(vuint16*)(0xFC094056))
#define MCF_ADC_DIV                          (*(vuint16*)(0xFC094058))
#define MCF_ADC_ASDIV                        (*(vuint16*)(0xFC09405A))
#define MCF_ADC_RSLT(x)                      (*(vuint16*)(0xFC094012 + ((x)*0x2)))
#define MCF_ADC_LLMT(x)                      (*(vuint16*)(0xFC094022 + ((x)*0x2)))
#define MCF_ADC_HLMT(x)                      (*(vuint16*)(0xFC094032 + ((x)*0x2)))
#define MCF_ADC_OFS(x)                       (*(vuint16*)(0xFC094042 + ((x)*0x2)))


/* Bit definitions and macros for MCF_ADC_CR1 */
#define MCF_ADC_CR1_SMODE(x)                 (((x)&0x7)<<0)
#define MCF_ADC_CR1_CHNCFG(x)                (((x)&0xF)<<0x4)
#define MCF_ADC_CR1_HLMTIE                   (0x100)
#define MCF_ADC_CR1_LLMTIE                   (0x200)
#define MCF_ADC_CR1_ZCIE                     (0x400)
#define MCF_ADC_CR1_EOSIE0                   (0x800)
#define MCF_ADC_CR1_SYNC0                    (0x1000)
#define MCF_ADC_CR1_START0                   (0x2000)
#define MCF_ADC_CR1_STOP0                    (0x4000)
#define MCF_ADC_CR1_DMAEN                    (0x8000)

/* Bit definitions and macros for MCF_ADC_CR2 */
#define MCF_ADC_CR2_SIMULT                   (0x80)
#define MCF_ADC_CR2_EOSIE1                   (0x800)
#define MCF_ADC_CR2_SYNC1                    (0x1000)
#define MCF_ADC_CR2_START1                   (0x2000)
#define MCF_ADC_CR2_STOP1                    (0x4000)

/* Bit definitions and macros for MCF_ADC_ZCCR */
#define MCF_ADC_ZCCR_ZCE0(x)                 (((x)&0x3)<<0)
#define MCF_ADC_ZCCR_ZCE1(x)                 (((x)&0x3)<<0x2)
#define MCF_ADC_ZCCR_ZCE2(x)                 (((x)&0x3)<<0x4)
#define MCF_ADC_ZCCR_ZCE3(x)                 (((x)&0x3)<<0x6)
#define MCF_ADC_ZCCR_ZCE4(x)                 (((x)&0x3)<<0x8)
#define MCF_ADC_ZCCR_ZCE5(x)                 (((x)&0x3)<<0xA)
#define MCF_ADC_ZCCR_ZCE6(x)                 (((x)&0x3)<<0xC)
#define MCF_ADC_ZCCR_ZCE7(x)                 (((x)&0x3)<<0xE)

/* Bit definitions and macros for MCF_ADC_LST1 */
#define MCF_ADC_LST1_SAMPLE0(x)              (((x)&0x7)<<0)
#define MCF_ADC_LST1_SAMPLE1(x)              (((x)&0x7)<<0x4)
#define MCF_ADC_LST1_SAMPLE2(x)              (((x)&0x7)<<0x8)
#define MCF_ADC_LST1_SAMPLE3(x)              (((x)&0x7)<<0xC)

/* Bit definitions and macros for MCF_ADC_LST2 */
#define MCF_ADC_LST2_SAMPLE4(x)              (((x)&0x7)<<0)
#define MCF_ADC_LST2_SAMPLE5(x)              (((x)&0x7)<<0x4)
#define MCF_ADC_LST2_SAMPLE6(x)              (((x)&0x7)<<0x8)
#define MCF_ADC_LST2_SAMPLE7(x)              (((x)&0x7)<<0xC)

/* Bit definitions and macros for MCF_ADC_SDIS */
#define MCF_ADC_SDIS_DS0                     (0x1)
#define MCF_ADC_SDIS_DS1                     (0x2)
#define MCF_ADC_SDIS_DS2                     (0x4)
#define MCF_ADC_SDIS_DS3                     (0x8)
#define MCF_ADC_SDIS_DS4                     (0x10)
#define MCF_ADC_SDIS_DS5                     (0x20)
#define MCF_ADC_SDIS_DS6                     (0x40)
#define MCF_ADC_SDIS_DS7                     (0x80)

/* Bit definitions and macros for MCF_ADC_SR */
#define MCF_ADC_SR_RDY0                      (0x1)
#define MCF_ADC_SR_RDY1                      (0x2)
#define MCF_ADC_SR_RDY2                      (0x4)
#define MCF_ADC_SR_RDY3                      (0x8)
#define MCF_ADC_SR_RDY4                      (0x10)
#define MCF_ADC_SR_RDY5                      (0x20)
#define MCF_ADC_SR_RDY6                      (0x40)
#define MCF_ADC_SR_RDY7                      (0x80)
#define MCF_ADC_SR_HLMTI                     (0x100)
#define MCF_ADC_SR_LLMTI                     (0x200)
#define MCF_ADC_SR_ZCI                       (0x400)
#define MCF_ADC_SR_EOSI0                     (0x800)
#define MCF_ADC_SR_EOSI1                     (0x1000)
#define MCF_ADC_SR_CIP1                      (0x4000)
#define MCF_ADC_SR_CIP0                      (0x8000)

/* Bit definitions and macros for MCF_ADC_LSR */
#define MCF_ADC_LSR_LLS0                     (0x1)
#define MCF_ADC_LSR_LLS1                     (0x2)
#define MCF_ADC_LSR_LLS2                     (0x4)
#define MCF_ADC_LSR_LLS3                     (0x8)
#define MCF_ADC_LSR_LLS4                     (0x10)
#define MCF_ADC_LSR_LLS5                     (0x20)
#define MCF_ADC_LSR_LLS6                     (0x40)
#define MCF_ADC_LSR_LLS7                     (0x80)
#define MCF_ADC_LSR_HLS0                     (0x100)
#define MCF_ADC_LSR_HLS1                     (0x200)
#define MCF_ADC_LSR_HLS2                     (0x400)
#define MCF_ADC_LSR_HLS3                     (0x800)
#define MCF_ADC_LSR_HLS4                     (0x1000)
#define MCF_ADC_LSR_HLS5                     (0x2000)
#define MCF_ADC_LSR_HLS6                     (0x4000)
#define MCF_ADC_LSR_HLS7                     (0x8000)

/* Bit definitions and macros for MCF_ADC_ZCSR */
#define MCF_ADC_ZCSR_ZCS0                    (0x1)
#define MCF_ADC_ZCSR_ZCS1                    (0x2)
#define MCF_ADC_ZCSR_ZCS2                    (0x4)
#define MCF_ADC_ZCSR_ZCS3                    (0x8)
#define MCF_ADC_ZCSR_ZCS4                    (0x10)
#define MCF_ADC_ZCSR_ZCS5                    (0x20)
#define MCF_ADC_ZCSR_ZCS6                    (0x40)
#define MCF_ADC_ZCSR_ZCS7                    (0x80)

/* Bit definitions and macros for MCF_ADC_RSLT */
#define MCF_ADC_RSLT_RESULT(x)               (((x)&0xFFF)<<0x3)
#define MCF_ADC_RSLT_SEXT                    (0x8000)

/* Bit definitions and macros for MCF_ADC_LLMT */
#define MCF_ADC_LLMT_LIMIT(x)                (((x)&0xFFF)<<0x3)

/* Bit definitions and macros for MCF_ADC_HLMT */
#define MCF_ADC_HLMT_LIMIT(x)                (((x)&0xFFF)<<0x3)

/* Bit definitions and macros for MCF_ADC_OFS */
#define MCF_ADC_OFS_OFFSET(x)                (((x)&0xFFF)<<0x3)

/* Bit definitions and macros for MCF_ADC_PWR */
#define MCF_ADC_PWR_PD0                      (0x1)
#define MCF_ADC_PWR_PD1                      (0x2)
#define MCF_ADC_PWR_PD2                      (0x4)
#define MCF_ADC_PWR_PD3                      (0x8)
#define MCF_ADC_PWR_PUDELAY(x)               (((x)&0x3F)<<0x4)
#define MCF_ADC_PWR_PSTS0                    (0x400)
#define MCF_ADC_PWR_PSTS1                    (0x800)
#define MCF_ADC_PWR_PSTS2                    (0x1000)
#define MCF_ADC_PWR_PSTS3                    (0x2000)
#define MCF_ADC_PWR_APD                      (0x4000)
#define MCF_ADC_PWR_ASB                      (0x8000)

/* Bit definitions and macros for MCF_ADC_CAL */
#define MCF_ADC_CAL_DAC0                     (0x1)
#define MCF_ADC_CAL_DAC1                     (0x2)
#define MCF_ADC_CAL_TEST0                    (0x4)
#define MCF_ADC_CAL_TEST1                    (0x8)
#define MCF_ADC_CAL_VREFL0                   (0x1000)
#define MCF_ADC_CAL_VREFH0                   (0x2000)
#define MCF_ADC_CAL_VREFL1                   (0x4000)
#define MCF_ADC_CAL_VREFH1                   (0x8000)

/* Bit definitions and macros for MCF_ADC_PWR2 */
#define MCF_ADC_PWR2_SPEEDA(x)               (((x)&0x3)<<0)
#define MCF_ADC_PWR2_SPEEDB(x)               (((x)&0x3)<<0x2)
#define MCF_ADC_PWR2_STNBY                   (0x10)

/* Bit definitions and macros for MCF_ADC_DIV */
#define MCF_ADC_DIV_DIV0(x)                  (((x)&0x7F)<<0)
#define MCF_ADC_DIV_DIV1(x)                  (((x)&0x7F)<<0x8)

/* Bit definitions and macros for MCF_ADC_ASDIV */
#define MCF_ADC_ASDIV_ASDIV(x)               (((x)&0x1FF)<<0)


#endif /* __MCF54418_ADC_H__ */
