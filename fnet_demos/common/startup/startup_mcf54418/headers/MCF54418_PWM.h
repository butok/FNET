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

#ifndef __MCF54418_PWM_H__
#define __MCF54418_PWM_H__


/*********************************************************************
*
* Motor Control Pulse Width Modulator (mcPWM)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_PWM_SM0CNT                       (*(vuint16*)(0xEC088000))
#define MCF_PWM_SM0INIT                      (*(vuint16*)(0xEC088002))
#define MCF_PWM_SM0CR2                       (*(vuint16*)(0xEC088004))
#define MCF_PWM_SM0CR1                       (*(vuint16*)(0xEC088006))
#define MCF_PWM_SM0VAL0                      (*(vuint16*)(0xEC088008))
#define MCF_PWM_SM0VAL1                      (*(vuint16*)(0xEC08800A))
#define MCF_PWM_SM0VAL2                      (*(vuint16*)(0xEC08800C))
#define MCF_PWM_SM0VAL3                      (*(vuint16*)(0xEC08800E))
#define MCF_PWM_SM0VAL4                      (*(vuint16*)(0xEC088010))
#define MCF_PWM_SM0VAL5                      (*(vuint16*)(0xEC088012))
#define MCF_PWM_SM0OCR                       (*(vuint16*)(0xEC088018))
#define MCF_PWM_SM0SR                        (*(vuint16*)(0xEC08801A))
#define MCF_PWM_SM0IER                       (*(vuint16*)(0xEC08801C))
#define MCF_PWM_SM0DMAEN                     (*(vuint16*)(0xEC08801E))
#define MCF_PWM_SM0OTCR                      (*(vuint16*)(0xEC088020))
#define MCF_PWM_SM0DISMAP                    (*(vuint16*)(0xEC088022))
#define MCF_PWM_SM0DTCNT0                    (*(vuint16*)(0xEC088024))
#define MCF_PWM_SM0DTCNT1                    (*(vuint16*)(0xEC088026))
#define MCF_PWM_SM0CCRA                      (*(vuint16*)(0xEC088028))
#define MCF_PWM_SM0CCMPA                     (*(vuint16*)(0xEC08802A))
#define MCF_PWM_SM0CCRB                      (*(vuint16*)(0xEC08802C))
#define MCF_PWM_SM0CCMPB                     (*(vuint16*)(0xEC08802E))
#define MCF_PWM_SM0CCRX                      (*(vuint16*)(0xEC088030))
#define MCF_PWM_SM0CCMPX                     (*(vuint16*)(0xEC088032))
#define MCF_PWM_SM0CVAL0                     (*(vuint16*)(0xEC088034))
#define MCF_PWM_SM0CCYC0                     (*(vuint16*)(0xEC088036))
#define MCF_PWM_SM0CVAL1                     (*(vuint16*)(0xEC088038))
#define MCF_PWM_SM0CCYC1                     (*(vuint16*)(0xEC08803A))
#define MCF_PWM_SM0CVAL2                     (*(vuint16*)(0xEC08803C))
#define MCF_PWM_SM0CCYC2                     (*(vuint16*)(0xEC08803E))
#define MCF_PWM_SM0CVAL3                     (*(vuint16*)(0xEC088040))
#define MCF_PWM_SM0CCYC3                     (*(vuint16*)(0xEC088042))
#define MCF_PWM_SM0CVAL4                     (*(vuint16*)(0xEC088044))
#define MCF_PWM_SM0CCYC4                     (*(vuint16*)(0xEC088046))
#define MCF_PWM_SM0CVAL5                     (*(vuint16*)(0xEC088048))
#define MCF_PWM_SM0CCYC5                     (*(vuint16*)(0xEC08804A))
#define MCF_PWM_SM1CNT                       (*(vuint16*)(0xEC088050))
#define MCF_PWM_SM1INIT                      (*(vuint16*)(0xEC088052))
#define MCF_PWM_SM1CR2                       (*(vuint16*)(0xEC088054))
#define MCF_PWM_SM1CR1                       (*(vuint16*)(0xEC088056))
#define MCF_PWM_SM1VAL0                      (*(vuint16*)(0xEC088058))
#define MCF_PWM_SM1VAL1                      (*(vuint16*)(0xEC08805A))
#define MCF_PWM_SM1VAL2                      (*(vuint16*)(0xEC08805C))
#define MCF_PWM_SM1VAL3                      (*(vuint16*)(0xEC08805E))
#define MCF_PWM_SM1VAL4                      (*(vuint16*)(0xEC088060))
#define MCF_PWM_SM1VAL5                      (*(vuint16*)(0xEC088062))
#define MCF_PWM_SM1OCR                       (*(vuint16*)(0xEC088068))
#define MCF_PWM_SM1SR                        (*(vuint16*)(0xEC08806A))
#define MCF_PWM_SM1IER                       (*(vuint16*)(0xEC08806C))
#define MCF_PWM_SM1DMAEN                     (*(vuint16*)(0xEC08806E))
#define MCF_PWM_SM1OTCR                      (*(vuint16*)(0xEC088070))
#define MCF_PWM_SM1DISMAP                    (*(vuint16*)(0xEC088072))
#define MCF_PWM_SM1DTCNT0                    (*(vuint16*)(0xEC088074))
#define MCF_PWM_SM1DTCNT1                    (*(vuint16*)(0xEC088076))
#define MCF_PWM_SM1CCRA                      (*(vuint16*)(0xEC088078))
#define MCF_PWM_SM1CCMPA                     (*(vuint16*)(0xEC08807A))
#define MCF_PWM_SM1CCRB                      (*(vuint16*)(0xEC08807C))
#define MCF_PWM_SM1CCMPB                     (*(vuint16*)(0xEC08807E))
#define MCF_PWM_SM1CCRX                      (*(vuint16*)(0xEC088080))
#define MCF_PWM_SM1CCMPX                     (*(vuint16*)(0xEC088082))
#define MCF_PWM_SM1CVAL0                     (*(vuint16*)(0xEC088084))
#define MCF_PWM_SM1CCYC0                     (*(vuint16*)(0xEC088086))
#define MCF_PWM_SM1CVAL1                     (*(vuint16*)(0xEC088088))
#define MCF_PWM_SM1CCYC1                     (*(vuint16*)(0xEC08808A))
#define MCF_PWM_SM1CVAL2                     (*(vuint16*)(0xEC08808C))
#define MCF_PWM_SM1CCYC2                     (*(vuint16*)(0xEC08808E))
#define MCF_PWM_SM1CVAL3                     (*(vuint16*)(0xEC088090))
#define MCF_PWM_SM1CCYC3                     (*(vuint16*)(0xEC088092))
#define MCF_PWM_SM1CVAL4                     (*(vuint16*)(0xEC088094))
#define MCF_PWM_SM1CCYC4                     (*(vuint16*)(0xEC088096))
#define MCF_PWM_SM1CVAL5                     (*(vuint16*)(0xEC088098))
#define MCF_PWM_SM1CCYC5                     (*(vuint16*)(0xEC08809A))
#define MCF_PWM_SM2CNT                       (*(vuint16*)(0xEC0880A0))
#define MCF_PWM_SM2INIT                      (*(vuint16*)(0xEC0880A2))
#define MCF_PWM_SM2CR2                       (*(vuint16*)(0xEC0880A4))
#define MCF_PWM_SM2CR1                       (*(vuint16*)(0xEC0880A6))
#define MCF_PWM_SM2VAL0                      (*(vuint16*)(0xEC0880A8))
#define MCF_PWM_SM2VAL1                      (*(vuint16*)(0xEC0880AA))
#define MCF_PWM_SM2VAL2                      (*(vuint16*)(0xEC0880AC))
#define MCF_PWM_SM2VAL3                      (*(vuint16*)(0xEC0880AE))
#define MCF_PWM_SM2VAL4                      (*(vuint16*)(0xEC0880B0))
#define MCF_PWM_SM2VAL5                      (*(vuint16*)(0xEC0880B2))
#define MCF_PWM_SM2OCR                       (*(vuint16*)(0xEC0880B8))
#define MCF_PWM_SM2SR                        (*(vuint16*)(0xEC0880BA))
#define MCF_PWM_SM2IER                       (*(vuint16*)(0xEC0880BC))
#define MCF_PWM_SM2DMAEN                     (*(vuint16*)(0xEC0880BE))
#define MCF_PWM_SM2OTCR                      (*(vuint16*)(0xEC0880C0))
#define MCF_PWM_SM2DISMAP                    (*(vuint16*)(0xEC0880C2))
#define MCF_PWM_SM2DTCNT0                    (*(vuint16*)(0xEC0880C4))
#define MCF_PWM_SM2DTCNT1                    (*(vuint16*)(0xEC0880C6))
#define MCF_PWM_SM2CCRA                      (*(vuint16*)(0xEC0880C8))
#define MCF_PWM_SM2CCMPA                     (*(vuint16*)(0xEC0880CA))
#define MCF_PWM_SM2CCRB                      (*(vuint16*)(0xEC0880CC))
#define MCF_PWM_SM2CCMPB                     (*(vuint16*)(0xEC0880CE))
#define MCF_PWM_SM2CCRX                      (*(vuint16*)(0xEC0880D0))
#define MCF_PWM_SM2CCMPX                     (*(vuint16*)(0xEC0880D2))
#define MCF_PWM_SM2CVAL0                     (*(vuint16*)(0xEC0880D4))
#define MCF_PWM_SM2CCYC0                     (*(vuint16*)(0xEC0880D6))
#define MCF_PWM_SM2CVAL1                     (*(vuint16*)(0xEC0880D8))
#define MCF_PWM_SM2CCYC1                     (*(vuint16*)(0xEC0880DA))
#define MCF_PWM_SM2CVAL2                     (*(vuint16*)(0xEC0880DC))
#define MCF_PWM_SM2CCYC2                     (*(vuint16*)(0xEC0880DE))
#define MCF_PWM_SM2CVAL3                     (*(vuint16*)(0xEC0880E0))
#define MCF_PWM_SM2CCYC3                     (*(vuint16*)(0xEC0880E2))
#define MCF_PWM_SM2CVAL4                     (*(vuint16*)(0xEC0880E4))
#define MCF_PWM_SM2CCYC4                     (*(vuint16*)(0xEC0880E6))
#define MCF_PWM_SM2CVAL5                     (*(vuint16*)(0xEC0880E8))
#define MCF_PWM_SM2CCYC5                     (*(vuint16*)(0xEC0880EA))
#define MCF_PWM_SM3CNT                       (*(vuint16*)(0xEC0880F0))
#define MCF_PWM_SM3INIT                      (*(vuint16*)(0xEC0880F2))
#define MCF_PWM_SM3CR2                       (*(vuint16*)(0xEC0880F4))
#define MCF_PWM_SM3CR1                       (*(vuint16*)(0xEC0880F6))
#define MCF_PWM_SM3VAL0                      (*(vuint16*)(0xEC0880F8))
#define MCF_PWM_SM3VAL1                      (*(vuint16*)(0xEC0880FA))
#define MCF_PWM_SM3VAL2                      (*(vuint16*)(0xEC0880FC))
#define MCF_PWM_SM3VAL3                      (*(vuint16*)(0xEC0880FE))
#define MCF_PWM_SM3VAL4                      (*(vuint16*)(0xEC088100))
#define MCF_PWM_SM3VAL5                      (*(vuint16*)(0xEC088102))
#define MCF_PWM_SM3OCR                       (*(vuint16*)(0xEC088108))
#define MCF_PWM_SM3SR                        (*(vuint16*)(0xEC08810A))
#define MCF_PWM_SM3IER                       (*(vuint16*)(0xEC08810C))
#define MCF_PWM_SM3DMAEN                     (*(vuint16*)(0xEC08810E))
#define MCF_PWM_SM3OTCR                      (*(vuint16*)(0xEC088110))
#define MCF_PWM_SM3DISMAP                    (*(vuint16*)(0xEC088112))
#define MCF_PWM_SM3DTCNT0                    (*(vuint16*)(0xEC088114))
#define MCF_PWM_SM3DTCNT1                    (*(vuint16*)(0xEC088116))
#define MCF_PWM_SM3CCRA                      (*(vuint16*)(0xEC088118))
#define MCF_PWM_SM3CCMPA                     (*(vuint16*)(0xEC08811A))
#define MCF_PWM_SM3CCRB                      (*(vuint16*)(0xEC08811C))
#define MCF_PWM_SM3CCMPB                     (*(vuint16*)(0xEC08811E))
#define MCF_PWM_SM3CCRX                      (*(vuint16*)(0xEC088120))
#define MCF_PWM_SM3CCMPX                     (*(vuint16*)(0xEC088122))
#define MCF_PWM_SM3CVAL0                     (*(vuint16*)(0xEC088124))
#define MCF_PWM_SM3CCYC0                     (*(vuint16*)(0xEC088126))
#define MCF_PWM_SM3CVAL1                     (*(vuint16*)(0xEC088128))
#define MCF_PWM_SM3CCYC1                     (*(vuint16*)(0xEC08812A))
#define MCF_PWM_SM3CVAL2                     (*(vuint16*)(0xEC08812C))
#define MCF_PWM_SM3CCYC2                     (*(vuint16*)(0xEC08812E))
#define MCF_PWM_SM3CVAL3                     (*(vuint16*)(0xEC088130))
#define MCF_PWM_SM3CCYC3                     (*(vuint16*)(0xEC088132))
#define MCF_PWM_SM3CVAL4                     (*(vuint16*)(0xEC088134))
#define MCF_PWM_SM3CCYC4                     (*(vuint16*)(0xEC088136))
#define MCF_PWM_SM3CVAL5                     (*(vuint16*)(0xEC088138))
#define MCF_PWM_SM3CCYC5                     (*(vuint16*)(0xEC08813A))
#define MCF_PWM_OUTEN                        (*(vuint16*)(0xEC088140))
#define MCF_PWM_MASK                         (*(vuint16*)(0xEC088142))
#define MCF_PWM_SWCOUT                       (*(vuint16*)(0xEC088144))
#define MCF_PWM_DTSS                         (*(vuint16*)(0xEC088146))
#define MCF_PWM_MCR                          (*(vuint16*)(0xEC088148))
#define MCF_PWM_FCR                          (*(vuint16*)(0xEC08814C))
#define MCF_PWM_FSR                          (*(vuint16*)(0xEC08814E))
#define MCF_PWM_FFILT                        (*(vuint16*)(0xEC088150))


/* Bit definitions and macros for MCF_PWM_SM0CNT */
#define MCF_PWM_SM0CNT_CNT(x)                (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0INIT */
#define MCF_PWM_SM0INIT_INIT(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0CR2 */
#define MCF_PWM_SM0CR2_CLK_SEL(x)            (((x)&0x3)<<0)
#define MCF_PWM_SM0CR2_RELOAD_SEL            (0x4)
#define MCF_PWM_SM0CR2_FORCE_SEL(x)          (((x)&0x7)<<0x3)
#define MCF_PWM_SM0CR2_FORCE                 (0x40)
#define MCF_PWM_SM0CR2_FRCEN                 (0x80)
#define MCF_PWM_SM0CR2_INIT_SEL(x)           (((x)&0x3)<<0x8)
#define MCF_PWM_SM0CR2_PWMX_INIT             (0x400)
#define MCF_PWM_SM0CR2_PWM45_INIT            (0x800)
#define MCF_PWM_SM0CR2_PWM23_INIT            (0x1000)
#define MCF_PWM_SM0CR2_INDEP                 (0x2000)
#define MCF_PWM_SM0CR2_WAITEN                (0x4000)
#define MCF_PWM_SM0CR2_DBGEN                 (0x8000)

/* Bit definitions and macros for MCF_PWM_SM0CR1 */
#define MCF_PWM_SM0CR1_DBLEN                 (0x1)
#define MCF_PWM_SM0CR1_LDMOD                 (0x4)
#define MCF_PWM_SM0CR1_PRSC(x)               (((x)&0x7)<<0x4)
#define MCF_PWM_SM0CR1_DT(x)                 (((x)&0x3)<<0x8)
#define MCF_PWM_SM0CR1_FULL                  (0x400)
#define MCF_PWM_SM0CR1_HALF                  (0x800)
#define MCF_PWM_SM0CR1_LDFQ(x)               (((x)&0xF)<<0xC)

/* Bit definitions and macros for MCF_PWM_SM0VAL0 */
#define MCF_PWM_SM0VAL0_VAL0(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0VAL1 */
#define MCF_PWM_SM0VAL1_VAL1(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0VAL2 */
#define MCF_PWM_SM0VAL2_VAL2(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0VAL3 */
#define MCF_PWM_SM0VAL3_VAL3(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0VAL4 */
#define MCF_PWM_SM0VAL4_VAL4(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0VAL5 */
#define MCF_PWM_SM0VAL5_VAL5(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0OCR */
#define MCF_PWM_SM0OCR_PWMXFS(x)             (((x)&0x3)<<0)
#define MCF_PWM_SM0OCR_PWMBFS(x)             (((x)&0x3)<<0x2)
#define MCF_PWM_SM0OCR_PWMAFS(x)             (((x)&0x3)<<0x4)
#define MCF_PWM_SM0OCR_POLX                  (0x100)
#define MCF_PWM_SM0OCR_POLB                  (0x200)
#define MCF_PWM_SM0OCR_POLA                  (0x400)
#define MCF_PWM_SM0OCR_PWMX_IN               (0x2000)
#define MCF_PWM_SM0OCR_PWMB_IN               (0x4000)
#define MCF_PWM_SM0OCR_PWMA_IN               (0x8000)

/* Bit definitions and macros for MCF_PWM_SM0SR */
#define MCF_PWM_SM0SR_CMPF(x)                (((x)&0x3F)<<0)
#define MCF_PWM_SM0SR_CFX0                   (0x40)
#define MCF_PWM_SM0SR_CFX1                   (0x80)
#define MCF_PWM_SM0SR_CFB0                   (0x100)
#define MCF_PWM_SM0SR_CFB1                   (0x200)
#define MCF_PWM_SM0SR_CFA0                   (0x400)
#define MCF_PWM_SM0SR_CFA1                   (0x800)
#define MCF_PWM_SM0SR_RF                     (0x1000)
#define MCF_PWM_SM0SR_REF                    (0x2000)
#define MCF_PWM_SM0SR_RUF                    (0x4000)

/* Bit definitions and macros for MCF_PWM_SM0IER */
#define MCF_PWM_SM0IER_CMPIE(x)              (((x)&0x3F)<<0)
#define MCF_PWM_SM0IER_CX0IE                 (0x40)
#define MCF_PWM_SM0IER_CX1IE                 (0x80)
#define MCF_PWM_SM0IER_CB0IE                 (0x100)
#define MCF_PWM_SM0IER_CB1IE                 (0x200)
#define MCF_PWM_SM0IER_CA0IE                 (0x400)
#define MCF_PWM_SM0IER_CA1IE                 (0x800)
#define MCF_PWM_SM0IER_RIE                   (0x1000)
#define MCF_PWM_SM0IER_REIE                  (0x2000)

/* Bit definitions and macros for MCF_PWM_SM0DMAEN */
#define MCF_PWM_SM0DMAEN_CX0DE               (0x1)
#define MCF_PWM_SM0DMAEN_CX1DE               (0x2)
#define MCF_PWM_SM0DMAEN_CB0DE               (0x4)
#define MCF_PWM_SM0DMAEN_CB1DE               (0x8)
#define MCF_PWM_SM0DMAEN_CA0DE               (0x10)
#define MCF_PWM_SM0DMAEN_CA1DE               (0x20)
#define MCF_PWM_SM0DMAEN_CAPTDE(x)           (((x)&0x3)<<0x6)
#define MCF_PWM_SM0DMAEN_FAND                (0x100)
#define MCF_PWM_SM0DMAEN_VALDE               (0x200)

/* Bit definitions and macros for MCF_PWM_SM0OTCR */
#define MCF_PWM_SM0OTCR_OTEN(x)              (((x)&0x3F)<<0)

/* Bit definitions and macros for MCF_PWM_SM0DISMAP */
#define MCF_PWM_SM0DISMAP_DISA(x)            (((x)&0xF)<<0)
#define MCF_PWM_SM0DISMAP_DISB(x)            (((x)&0xF)<<0x4)
#define MCF_PWM_SM0DISMAP_DISX(x)            (((x)&0xF)<<0x8)

/* Bit definitions and macros for MCF_PWM_SM0DTCNT0 */
#define MCF_PWM_SM0DTCNT0_DT(x)              (((x)&0x7FF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0DTCNT1 */
#define MCF_PWM_SM0DTCNT1_DT(x)              (((x)&0x7FF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0CCRA */
#define MCF_PWM_SM0CCRA_ARM                  (0x1)
#define MCF_PWM_SM0CCRA_ONESHOT              (0x2)
#define MCF_PWM_SM0CCRA_EDG0(x)              (((x)&0x3)<<0x2)
#define MCF_PWM_SM0CCRA_EDG1(x)              (((x)&0x3)<<0x4)
#define MCF_PWM_SM0CCRA_INP_SEL              (0x40)
#define MCF_PWM_SM0CCRA_EDGCNT_EN            (0x80)
#define MCF_PWM_SM0CCRA_CFWM(x)              (((x)&0x3)<<0x8)
#define MCF_PWM_SM0CCRA_C0CNT(x)             (((x)&0x7)<<0xA)
#define MCF_PWM_SM0CCRA_C1CNT(x)             (((x)&0x7)<<0xD)

/* Bit definitions and macros for MCF_PWM_SM0CCMPA */
#define MCF_PWM_SM0CCMPA_EDGCMP(x)           (((x)&0xFF)<<0)
#define MCF_PWM_SM0CCMPA_EDGCNT(x)           (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_PWM_SM0CCRB */
#define MCF_PWM_SM0CCRB_ARM                  (0x1)
#define MCF_PWM_SM0CCRB_ONESHOT              (0x2)
#define MCF_PWM_SM0CCRB_EDG0(x)              (((x)&0x3)<<0x2)
#define MCF_PWM_SM0CCRB_EDG1(x)              (((x)&0x3)<<0x4)
#define MCF_PWM_SM0CCRB_INP_SEL              (0x40)
#define MCF_PWM_SM0CCRB_EDGCNT_EN            (0x80)
#define MCF_PWM_SM0CCRB_CFWM(x)              (((x)&0x3)<<0x8)
#define MCF_PWM_SM0CCRB_C0CNT(x)             (((x)&0x7)<<0xA)
#define MCF_PWM_SM0CCRB_C1CNT(x)             (((x)&0x7)<<0xD)

/* Bit definitions and macros for MCF_PWM_SM0CCMPB */
#define MCF_PWM_SM0CCMPB_EDGCMP(x)           (((x)&0xFF)<<0)
#define MCF_PWM_SM0CCMPB_EDGCNT(x)           (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_PWM_SM0CCRX */
#define MCF_PWM_SM0CCRX_ARM                  (0x1)
#define MCF_PWM_SM0CCRX_ONESHOT              (0x2)
#define MCF_PWM_SM0CCRX_EDG0(x)              (((x)&0x3)<<0x2)
#define MCF_PWM_SM0CCRX_EDG1(x)              (((x)&0x3)<<0x4)
#define MCF_PWM_SM0CCRX_INP_SEL              (0x40)
#define MCF_PWM_SM0CCRX_EDGCNT_EN            (0x80)
#define MCF_PWM_SM0CCRX_CFWM(x)              (((x)&0x3)<<0x8)
#define MCF_PWM_SM0CCRX_C0CNT(x)             (((x)&0x7)<<0xA)
#define MCF_PWM_SM0CCRX_C1CNT(x)             (((x)&0x7)<<0xD)

/* Bit definitions and macros for MCF_PWM_SM0CCMPX */
#define MCF_PWM_SM0CCMPX_EDGCMP(x)           (((x)&0xFF)<<0)
#define MCF_PWM_SM0CCMPX_EDGCNT(x)           (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_PWM_SM0CVAL0 */
#define MCF_PWM_SM0CVAL0_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0CCYC0 */
#define MCF_PWM_SM0CCYC0_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0CVAL1 */
#define MCF_PWM_SM0CVAL1_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0CCYC1 */
#define MCF_PWM_SM0CCYC1_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0CVAL2 */
#define MCF_PWM_SM0CVAL2_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0CCYC2 */
#define MCF_PWM_SM0CCYC2_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0CVAL3 */
#define MCF_PWM_SM0CVAL3_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0CCYC3 */
#define MCF_PWM_SM0CCYC3_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0CVAL4 */
#define MCF_PWM_SM0CVAL4_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0CCYC4 */
#define MCF_PWM_SM0CCYC4_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0CVAL5 */
#define MCF_PWM_SM0CVAL5_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM0CCYC5 */
#define MCF_PWM_SM0CCYC5_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1CNT */
#define MCF_PWM_SM1CNT_CNT(x)                (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1INIT */
#define MCF_PWM_SM1INIT_INIT(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1CR2 */
#define MCF_PWM_SM1CR2_CLK_SEL(x)            (((x)&0x3)<<0)
#define MCF_PWM_SM1CR2_RELOAD_SEL            (0x4)
#define MCF_PWM_SM1CR2_FORCE_SEL(x)          (((x)&0x7)<<0x3)
#define MCF_PWM_SM1CR2_FORCE                 (0x40)
#define MCF_PWM_SM1CR2_FRCEN                 (0x80)
#define MCF_PWM_SM1CR2_INIT_SEL(x)           (((x)&0x3)<<0x8)
#define MCF_PWM_SM1CR2_PWMX_INIT             (0x400)
#define MCF_PWM_SM1CR2_PWM45_INIT            (0x800)
#define MCF_PWM_SM1CR2_PWM23_INIT            (0x1000)
#define MCF_PWM_SM1CR2_INDEP                 (0x2000)
#define MCF_PWM_SM1CR2_WAITEN                (0x4000)
#define MCF_PWM_SM1CR2_DBGEN                 (0x8000)

/* Bit definitions and macros for MCF_PWM_SM1CR1 */
#define MCF_PWM_SM1CR1_DBLEN                 (0x1)
#define MCF_PWM_SM1CR1_LDMOD                 (0x4)
#define MCF_PWM_SM1CR1_PRSC(x)               (((x)&0x7)<<0x4)
#define MCF_PWM_SM1CR1_DT(x)                 (((x)&0x3)<<0x8)
#define MCF_PWM_SM1CR1_FULL                  (0x400)
#define MCF_PWM_SM1CR1_HALF                  (0x800)
#define MCF_PWM_SM1CR1_LDFQ(x)               (((x)&0xF)<<0xC)

/* Bit definitions and macros for MCF_PWM_SM1VAL0 */
#define MCF_PWM_SM1VAL0_VAL0(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1VAL1 */
#define MCF_PWM_SM1VAL1_VAL1(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1VAL2 */
#define MCF_PWM_SM1VAL2_VAL2(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1VAL3 */
#define MCF_PWM_SM1VAL3_VAL3(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1VAL4 */
#define MCF_PWM_SM1VAL4_VAL4(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1VAL5 */
#define MCF_PWM_SM1VAL5_VAL5(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1OCR */
#define MCF_PWM_SM1OCR_PWMXFS(x)             (((x)&0x3)<<0)
#define MCF_PWM_SM1OCR_PWMBFS(x)             (((x)&0x3)<<0x2)
#define MCF_PWM_SM1OCR_PWMAFS(x)             (((x)&0x3)<<0x4)
#define MCF_PWM_SM1OCR_POLX                  (0x100)
#define MCF_PWM_SM1OCR_POLB                  (0x200)
#define MCF_PWM_SM1OCR_POLA                  (0x400)
#define MCF_PWM_SM1OCR_PWMX_IN               (0x2000)
#define MCF_PWM_SM1OCR_PWMB_IN               (0x4000)
#define MCF_PWM_SM1OCR_PWMA_IN               (0x8000)

/* Bit definitions and macros for MCF_PWM_SM1SR */
#define MCF_PWM_SM1SR_CMPF(x)                (((x)&0x3F)<<0)
#define MCF_PWM_SM1SR_CFX0                   (0x40)
#define MCF_PWM_SM1SR_CFX1                   (0x80)
#define MCF_PWM_SM1SR_CFB0                   (0x100)
#define MCF_PWM_SM1SR_CFB1                   (0x200)
#define MCF_PWM_SM1SR_CFA0                   (0x400)
#define MCF_PWM_SM1SR_CFA1                   (0x800)
#define MCF_PWM_SM1SR_RF                     (0x1000)
#define MCF_PWM_SM1SR_REF                    (0x2000)
#define MCF_PWM_SM1SR_RUF                    (0x4000)

/* Bit definitions and macros for MCF_PWM_SM1IER */
#define MCF_PWM_SM1IER_CMPIE(x)              (((x)&0x3F)<<0)
#define MCF_PWM_SM1IER_CX0IE                 (0x40)
#define MCF_PWM_SM1IER_CX1IE                 (0x80)
#define MCF_PWM_SM1IER_CB0IE                 (0x100)
#define MCF_PWM_SM1IER_CB1IE                 (0x200)
#define MCF_PWM_SM1IER_CA0IE                 (0x400)
#define MCF_PWM_SM1IER_CA1IE                 (0x800)
#define MCF_PWM_SM1IER_RIE                   (0x1000)
#define MCF_PWM_SM1IER_REIE                  (0x2000)

/* Bit definitions and macros for MCF_PWM_SM1DMAEN */
#define MCF_PWM_SM1DMAEN_CX0DE               (0x1)
#define MCF_PWM_SM1DMAEN_CX1DE               (0x2)
#define MCF_PWM_SM1DMAEN_CB0DE               (0x4)
#define MCF_PWM_SM1DMAEN_CB1DE               (0x8)
#define MCF_PWM_SM1DMAEN_CA0DE               (0x10)
#define MCF_PWM_SM1DMAEN_CA1DE               (0x20)
#define MCF_PWM_SM1DMAEN_CAPTDE(x)           (((x)&0x3)<<0x6)
#define MCF_PWM_SM1DMAEN_FAND                (0x100)
#define MCF_PWM_SM1DMAEN_VALDE               (0x200)

/* Bit definitions and macros for MCF_PWM_SM1OTCR */
#define MCF_PWM_SM1OTCR_OTEN(x)              (((x)&0x3F)<<0)

/* Bit definitions and macros for MCF_PWM_SM1DISMAP */
#define MCF_PWM_SM1DISMAP_DISA(x)            (((x)&0xF)<<0)
#define MCF_PWM_SM1DISMAP_DISB(x)            (((x)&0xF)<<0x4)
#define MCF_PWM_SM1DISMAP_DISX(x)            (((x)&0xF)<<0x8)

/* Bit definitions and macros for MCF_PWM_SM1DTCNT0 */
#define MCF_PWM_SM1DTCNT0_DT(x)              (((x)&0x7FF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1DTCNT1 */
#define MCF_PWM_SM1DTCNT1_DT(x)              (((x)&0x7FF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1CCRA */
#define MCF_PWM_SM1CCRA_ARM                  (0x1)
#define MCF_PWM_SM1CCRA_ONESHOT              (0x2)
#define MCF_PWM_SM1CCRA_EDG0(x)              (((x)&0x3)<<0x2)
#define MCF_PWM_SM1CCRA_EDG1(x)              (((x)&0x3)<<0x4)
#define MCF_PWM_SM1CCRA_INP_SEL              (0x40)
#define MCF_PWM_SM1CCRA_EDGCNT_EN            (0x80)
#define MCF_PWM_SM1CCRA_CFWM(x)              (((x)&0x3)<<0x8)
#define MCF_PWM_SM1CCRA_C0CNT(x)             (((x)&0x7)<<0xA)
#define MCF_PWM_SM1CCRA_C1CNT(x)             (((x)&0x7)<<0xD)

/* Bit definitions and macros for MCF_PWM_SM1CCMPA */
#define MCF_PWM_SM1CCMPA_EDGCMP(x)           (((x)&0xFF)<<0)
#define MCF_PWM_SM1CCMPA_EDGCNT(x)           (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_PWM_SM1CCRB */
#define MCF_PWM_SM1CCRB_ARM                  (0x1)
#define MCF_PWM_SM1CCRB_ONESHOT              (0x2)
#define MCF_PWM_SM1CCRB_EDG0(x)              (((x)&0x3)<<0x2)
#define MCF_PWM_SM1CCRB_EDG1(x)              (((x)&0x3)<<0x4)
#define MCF_PWM_SM1CCRB_INP_SEL              (0x40)
#define MCF_PWM_SM1CCRB_EDGCNT_EN            (0x80)
#define MCF_PWM_SM1CCRB_CFWM(x)              (((x)&0x3)<<0x8)
#define MCF_PWM_SM1CCRB_C0CNT(x)             (((x)&0x7)<<0xA)
#define MCF_PWM_SM1CCRB_C1CNT(x)             (((x)&0x7)<<0xD)

/* Bit definitions and macros for MCF_PWM_SM1CCMPB */
#define MCF_PWM_SM1CCMPB_EDGCMP(x)           (((x)&0xFF)<<0)
#define MCF_PWM_SM1CCMPB_EDGCNT(x)           (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_PWM_SM1CCRX */
#define MCF_PWM_SM1CCRX_ARM                  (0x1)
#define MCF_PWM_SM1CCRX_ONESHOT              (0x2)
#define MCF_PWM_SM1CCRX_EDG0(x)              (((x)&0x3)<<0x2)
#define MCF_PWM_SM1CCRX_EDG1(x)              (((x)&0x3)<<0x4)
#define MCF_PWM_SM1CCRX_INP_SEL              (0x40)
#define MCF_PWM_SM1CCRX_EDGCNT_EN            (0x80)
#define MCF_PWM_SM1CCRX_CFWM(x)              (((x)&0x3)<<0x8)
#define MCF_PWM_SM1CCRX_C0CNT(x)             (((x)&0x7)<<0xA)
#define MCF_PWM_SM1CCRX_C1CNT(x)             (((x)&0x7)<<0xD)

/* Bit definitions and macros for MCF_PWM_SM1CCMPX */
#define MCF_PWM_SM1CCMPX_EDGCMP(x)           (((x)&0xFF)<<0)
#define MCF_PWM_SM1CCMPX_EDGCNT(x)           (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_PWM_SM1CVAL0 */
#define MCF_PWM_SM1CVAL0_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1CCYC0 */
#define MCF_PWM_SM1CCYC0_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1CVAL1 */
#define MCF_PWM_SM1CVAL1_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1CCYC1 */
#define MCF_PWM_SM1CCYC1_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1CVAL2 */
#define MCF_PWM_SM1CVAL2_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1CCYC2 */
#define MCF_PWM_SM1CCYC2_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1CVAL3 */
#define MCF_PWM_SM1CVAL3_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1CCYC3 */
#define MCF_PWM_SM1CCYC3_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1CVAL4 */
#define MCF_PWM_SM1CVAL4_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1CCYC4 */
#define MCF_PWM_SM1CCYC4_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1CVAL5 */
#define MCF_PWM_SM1CVAL5_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM1CCYC5 */
#define MCF_PWM_SM1CCYC5_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2CNT */
#define MCF_PWM_SM2CNT_CNT(x)                (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2INIT */
#define MCF_PWM_SM2INIT_INIT(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2CR2 */
#define MCF_PWM_SM2CR2_CLK_SEL(x)            (((x)&0x3)<<0)
#define MCF_PWM_SM2CR2_RELOAD_SEL            (0x4)
#define MCF_PWM_SM2CR2_FORCE_SEL(x)          (((x)&0x7)<<0x3)
#define MCF_PWM_SM2CR2_FORCE                 (0x40)
#define MCF_PWM_SM2CR2_FRCEN                 (0x80)
#define MCF_PWM_SM2CR2_INIT_SEL(x)           (((x)&0x3)<<0x8)
#define MCF_PWM_SM2CR2_PWMX_INIT             (0x400)
#define MCF_PWM_SM2CR2_PWM45_INIT            (0x800)
#define MCF_PWM_SM2CR2_PWM23_INIT            (0x1000)
#define MCF_PWM_SM2CR2_INDEP                 (0x2000)
#define MCF_PWM_SM2CR2_WAITEN                (0x4000)
#define MCF_PWM_SM2CR2_DBGEN                 (0x8000)

/* Bit definitions and macros for MCF_PWM_SM2CR1 */
#define MCF_PWM_SM2CR1_DBLEN                 (0x1)
#define MCF_PWM_SM2CR1_LDMOD                 (0x4)
#define MCF_PWM_SM2CR1_PRSC(x)               (((x)&0x7)<<0x4)
#define MCF_PWM_SM2CR1_DT(x)                 (((x)&0x3)<<0x8)
#define MCF_PWM_SM2CR1_FULL                  (0x400)
#define MCF_PWM_SM2CR1_HALF                  (0x800)
#define MCF_PWM_SM2CR1_LDFQ(x)               (((x)&0xF)<<0xC)

/* Bit definitions and macros for MCF_PWM_SM2VAL0 */
#define MCF_PWM_SM2VAL0_VAL0(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2VAL1 */
#define MCF_PWM_SM2VAL1_VAL1(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2VAL2 */
#define MCF_PWM_SM2VAL2_VAL2(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2VAL3 */
#define MCF_PWM_SM2VAL3_VAL3(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2VAL4 */
#define MCF_PWM_SM2VAL4_VAL4(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2VAL5 */
#define MCF_PWM_SM2VAL5_VAL5(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2OCR */
#define MCF_PWM_SM2OCR_PWMXFS(x)             (((x)&0x3)<<0)
#define MCF_PWM_SM2OCR_PWMBFS(x)             (((x)&0x3)<<0x2)
#define MCF_PWM_SM2OCR_PWMAFS(x)             (((x)&0x3)<<0x4)
#define MCF_PWM_SM2OCR_POLX                  (0x100)
#define MCF_PWM_SM2OCR_POLB                  (0x200)
#define MCF_PWM_SM2OCR_POLA                  (0x400)
#define MCF_PWM_SM2OCR_PWMX_IN               (0x2000)
#define MCF_PWM_SM2OCR_PWMB_IN               (0x4000)
#define MCF_PWM_SM2OCR_PWMA_IN               (0x8000)

/* Bit definitions and macros for MCF_PWM_SM2SR */
#define MCF_PWM_SM2SR_CMPF(x)                (((x)&0x3F)<<0)
#define MCF_PWM_SM2SR_CFX0                   (0x40)
#define MCF_PWM_SM2SR_CFX1                   (0x80)
#define MCF_PWM_SM2SR_CFB0                   (0x100)
#define MCF_PWM_SM2SR_CFB1                   (0x200)
#define MCF_PWM_SM2SR_CFA0                   (0x400)
#define MCF_PWM_SM2SR_CFA1                   (0x800)
#define MCF_PWM_SM2SR_RF                     (0x1000)
#define MCF_PWM_SM2SR_REF                    (0x2000)
#define MCF_PWM_SM2SR_RUF                    (0x4000)

/* Bit definitions and macros for MCF_PWM_SM2IER */
#define MCF_PWM_SM2IER_CMPIE(x)              (((x)&0x3F)<<0)
#define MCF_PWM_SM2IER_CX0IE                 (0x40)
#define MCF_PWM_SM2IER_CX1IE                 (0x80)
#define MCF_PWM_SM2IER_CB0IE                 (0x100)
#define MCF_PWM_SM2IER_CB1IE                 (0x200)
#define MCF_PWM_SM2IER_CA0IE                 (0x400)
#define MCF_PWM_SM2IER_CA1IE                 (0x800)
#define MCF_PWM_SM2IER_RIE                   (0x1000)
#define MCF_PWM_SM2IER_REIE                  (0x2000)

/* Bit definitions and macros for MCF_PWM_SM2DMAEN */
#define MCF_PWM_SM2DMAEN_CX0DE               (0x1)
#define MCF_PWM_SM2DMAEN_CX1DE               (0x2)
#define MCF_PWM_SM2DMAEN_CB0DE               (0x4)
#define MCF_PWM_SM2DMAEN_CB1DE               (0x8)
#define MCF_PWM_SM2DMAEN_CA0DE               (0x10)
#define MCF_PWM_SM2DMAEN_CA1DE               (0x20)
#define MCF_PWM_SM2DMAEN_CAPTDE(x)           (((x)&0x3)<<0x6)
#define MCF_PWM_SM2DMAEN_FAND                (0x100)
#define MCF_PWM_SM2DMAEN_VALDE               (0x200)

/* Bit definitions and macros for MCF_PWM_SM2OTCR */
#define MCF_PWM_SM2OTCR_OTEN(x)              (((x)&0x3F)<<0)

/* Bit definitions and macros for MCF_PWM_SM2DISMAP */
#define MCF_PWM_SM2DISMAP_DISA(x)            (((x)&0xF)<<0)
#define MCF_PWM_SM2DISMAP_DISB(x)            (((x)&0xF)<<0x4)
#define MCF_PWM_SM2DISMAP_DISX(x)            (((x)&0xF)<<0x8)

/* Bit definitions and macros for MCF_PWM_SM2DTCNT0 */
#define MCF_PWM_SM2DTCNT0_DT(x)              (((x)&0x7FF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2DTCNT1 */
#define MCF_PWM_SM2DTCNT1_DT(x)              (((x)&0x7FF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2CCRA */
#define MCF_PWM_SM2CCRA_ARM                  (0x1)
#define MCF_PWM_SM2CCRA_ONESHOT              (0x2)
#define MCF_PWM_SM2CCRA_EDG0(x)              (((x)&0x3)<<0x2)
#define MCF_PWM_SM2CCRA_EDG1(x)              (((x)&0x3)<<0x4)
#define MCF_PWM_SM2CCRA_INP_SEL              (0x40)
#define MCF_PWM_SM2CCRA_EDGCNT_EN            (0x80)
#define MCF_PWM_SM2CCRA_CFWM(x)              (((x)&0x3)<<0x8)
#define MCF_PWM_SM2CCRA_C0CNT(x)             (((x)&0x7)<<0xA)
#define MCF_PWM_SM2CCRA_C1CNT(x)             (((x)&0x7)<<0xD)

/* Bit definitions and macros for MCF_PWM_SM2CCMPA */
#define MCF_PWM_SM2CCMPA_EDGCMP(x)           (((x)&0xFF)<<0)
#define MCF_PWM_SM2CCMPA_EDGCNT(x)           (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_PWM_SM2CCRB */
#define MCF_PWM_SM2CCRB_ARM                  (0x1)
#define MCF_PWM_SM2CCRB_ONESHOT              (0x2)
#define MCF_PWM_SM2CCRB_EDG0(x)              (((x)&0x3)<<0x2)
#define MCF_PWM_SM2CCRB_EDG1(x)              (((x)&0x3)<<0x4)
#define MCF_PWM_SM2CCRB_INP_SEL              (0x40)
#define MCF_PWM_SM2CCRB_EDGCNT_EN            (0x80)
#define MCF_PWM_SM2CCRB_CFWM(x)              (((x)&0x3)<<0x8)
#define MCF_PWM_SM2CCRB_C0CNT(x)             (((x)&0x7)<<0xA)
#define MCF_PWM_SM2CCRB_C1CNT(x)             (((x)&0x7)<<0xD)

/* Bit definitions and macros for MCF_PWM_SM2CCMPB */
#define MCF_PWM_SM2CCMPB_EDGCMP(x)           (((x)&0xFF)<<0)
#define MCF_PWM_SM2CCMPB_EDGCNT(x)           (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_PWM_SM2CCRX */
#define MCF_PWM_SM2CCRX_ARM                  (0x1)
#define MCF_PWM_SM2CCRX_ONESHOT              (0x2)
#define MCF_PWM_SM2CCRX_EDG0(x)              (((x)&0x3)<<0x2)
#define MCF_PWM_SM2CCRX_EDG1(x)              (((x)&0x3)<<0x4)
#define MCF_PWM_SM2CCRX_INP_SEL              (0x40)
#define MCF_PWM_SM2CCRX_EDGCNT_EN            (0x80)
#define MCF_PWM_SM2CCRX_CFWM(x)              (((x)&0x3)<<0x8)
#define MCF_PWM_SM2CCRX_C0CNT(x)             (((x)&0x7)<<0xA)
#define MCF_PWM_SM2CCRX_C1CNT(x)             (((x)&0x7)<<0xD)

/* Bit definitions and macros for MCF_PWM_SM2CCMPX */
#define MCF_PWM_SM2CCMPX_EDGCMP(x)           (((x)&0xFF)<<0)
#define MCF_PWM_SM2CCMPX_EDGCNT(x)           (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_PWM_SM2CVAL0 */
#define MCF_PWM_SM2CVAL0_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2CCYC0 */
#define MCF_PWM_SM2CCYC0_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2CVAL1 */
#define MCF_PWM_SM2CVAL1_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2CCYC1 */
#define MCF_PWM_SM2CCYC1_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2CVAL2 */
#define MCF_PWM_SM2CVAL2_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2CCYC2 */
#define MCF_PWM_SM2CCYC2_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2CVAL3 */
#define MCF_PWM_SM2CVAL3_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2CCYC3 */
#define MCF_PWM_SM2CCYC3_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2CVAL4 */
#define MCF_PWM_SM2CVAL4_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2CCYC4 */
#define MCF_PWM_SM2CCYC4_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2CVAL5 */
#define MCF_PWM_SM2CVAL5_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM2CCYC5 */
#define MCF_PWM_SM2CCYC5_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3CNT */
#define MCF_PWM_SM3CNT_CNT(x)                (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3INIT */
#define MCF_PWM_SM3INIT_INIT(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3CR2 */
#define MCF_PWM_SM3CR2_CLK_SEL(x)            (((x)&0x3)<<0)
#define MCF_PWM_SM3CR2_RELOAD_SEL            (0x4)
#define MCF_PWM_SM3CR2_FORCE_SEL(x)          (((x)&0x7)<<0x3)
#define MCF_PWM_SM3CR2_FORCE                 (0x40)
#define MCF_PWM_SM3CR2_FRCEN                 (0x80)
#define MCF_PWM_SM3CR2_INIT_SEL(x)           (((x)&0x3)<<0x8)
#define MCF_PWM_SM3CR2_PWMX_INIT             (0x400)
#define MCF_PWM_SM3CR2_PWM45_INIT            (0x800)
#define MCF_PWM_SM3CR2_PWM23_INIT            (0x1000)
#define MCF_PWM_SM3CR2_INDEP                 (0x2000)
#define MCF_PWM_SM3CR2_WAITEN                (0x4000)
#define MCF_PWM_SM3CR2_DBGEN                 (0x8000)

/* Bit definitions and macros for MCF_PWM_SM3CR1 */
#define MCF_PWM_SM3CR1_DBLEN                 (0x1)
#define MCF_PWM_SM3CR1_LDMOD                 (0x4)
#define MCF_PWM_SM3CR1_PRSC(x)               (((x)&0x7)<<0x4)
#define MCF_PWM_SM3CR1_DT(x)                 (((x)&0x3)<<0x8)
#define MCF_PWM_SM3CR1_FULL                  (0x400)
#define MCF_PWM_SM3CR1_HALF                  (0x800)
#define MCF_PWM_SM3CR1_LDFQ(x)               (((x)&0xF)<<0xC)

/* Bit definitions and macros for MCF_PWM_SM3VAL0 */
#define MCF_PWM_SM3VAL0_VAL0(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3VAL1 */
#define MCF_PWM_SM3VAL1_VAL1(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3VAL2 */
#define MCF_PWM_SM3VAL2_VAL2(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3VAL3 */
#define MCF_PWM_SM3VAL3_VAL3(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3VAL4 */
#define MCF_PWM_SM3VAL4_VAL4(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3VAL5 */
#define MCF_PWM_SM3VAL5_VAL5(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3OCR */
#define MCF_PWM_SM3OCR_PWMXFS(x)             (((x)&0x3)<<0)
#define MCF_PWM_SM3OCR_PWMBFS(x)             (((x)&0x3)<<0x2)
#define MCF_PWM_SM3OCR_PWMAFS(x)             (((x)&0x3)<<0x4)
#define MCF_PWM_SM3OCR_POLX                  (0x100)
#define MCF_PWM_SM3OCR_POLB                  (0x200)
#define MCF_PWM_SM3OCR_POLA                  (0x400)
#define MCF_PWM_SM3OCR_PWMX_IN               (0x2000)
#define MCF_PWM_SM3OCR_PWMB_IN               (0x4000)
#define MCF_PWM_SM3OCR_PWMA_IN               (0x8000)

/* Bit definitions and macros for MCF_PWM_SM3SR */
#define MCF_PWM_SM3SR_CMPF(x)                (((x)&0x3F)<<0)
#define MCF_PWM_SM3SR_CFX0                   (0x40)
#define MCF_PWM_SM3SR_CFX1                   (0x80)
#define MCF_PWM_SM3SR_CFB0                   (0x100)
#define MCF_PWM_SM3SR_CFB1                   (0x200)
#define MCF_PWM_SM3SR_CFA0                   (0x400)
#define MCF_PWM_SM3SR_CFA1                   (0x800)
#define MCF_PWM_SM3SR_RF                     (0x1000)
#define MCF_PWM_SM3SR_REF                    (0x2000)
#define MCF_PWM_SM3SR_RUF                    (0x4000)

/* Bit definitions and macros for MCF_PWM_SM3IER */
#define MCF_PWM_SM3IER_CMPIE(x)              (((x)&0x3F)<<0)
#define MCF_PWM_SM3IER_CX0IE                 (0x40)
#define MCF_PWM_SM3IER_CX1IE                 (0x80)
#define MCF_PWM_SM3IER_CB0IE                 (0x100)
#define MCF_PWM_SM3IER_CB1IE                 (0x200)
#define MCF_PWM_SM3IER_CA0IE                 (0x400)
#define MCF_PWM_SM3IER_CA1IE                 (0x800)
#define MCF_PWM_SM3IER_RIE                   (0x1000)
#define MCF_PWM_SM3IER_REIE                  (0x2000)

/* Bit definitions and macros for MCF_PWM_SM3DMAEN */
#define MCF_PWM_SM3DMAEN_CX0DE               (0x1)
#define MCF_PWM_SM3DMAEN_CX1DE               (0x2)
#define MCF_PWM_SM3DMAEN_CB0DE               (0x4)
#define MCF_PWM_SM3DMAEN_CB1DE               (0x8)
#define MCF_PWM_SM3DMAEN_CA0DE               (0x10)
#define MCF_PWM_SM3DMAEN_CA1DE               (0x20)
#define MCF_PWM_SM3DMAEN_CAPTDE(x)           (((x)&0x3)<<0x6)
#define MCF_PWM_SM3DMAEN_FAND                (0x100)
#define MCF_PWM_SM3DMAEN_VALDE               (0x200)

/* Bit definitions and macros for MCF_PWM_SM3OTCR */
#define MCF_PWM_SM3OTCR_OTEN(x)              (((x)&0x3F)<<0)

/* Bit definitions and macros for MCF_PWM_SM3DISMAP */
#define MCF_PWM_SM3DISMAP_DISA(x)            (((x)&0xF)<<0)
#define MCF_PWM_SM3DISMAP_DISB(x)            (((x)&0xF)<<0x4)
#define MCF_PWM_SM3DISMAP_DISX(x)            (((x)&0xF)<<0x8)

/* Bit definitions and macros for MCF_PWM_SM3DTCNT0 */
#define MCF_PWM_SM3DTCNT0_DT(x)              (((x)&0x7FF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3DTCNT1 */
#define MCF_PWM_SM3DTCNT1_DT(x)              (((x)&0x7FF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3CCRA */
#define MCF_PWM_SM3CCRA_ARM                  (0x1)
#define MCF_PWM_SM3CCRA_ONESHOT              (0x2)
#define MCF_PWM_SM3CCRA_EDG0(x)              (((x)&0x3)<<0x2)
#define MCF_PWM_SM3CCRA_EDG1(x)              (((x)&0x3)<<0x4)
#define MCF_PWM_SM3CCRA_INP_SEL              (0x40)
#define MCF_PWM_SM3CCRA_EDGCNT_EN            (0x80)
#define MCF_PWM_SM3CCRA_CFWM(x)              (((x)&0x3)<<0x8)
#define MCF_PWM_SM3CCRA_C0CNT(x)             (((x)&0x7)<<0xA)
#define MCF_PWM_SM3CCRA_C1CNT(x)             (((x)&0x7)<<0xD)

/* Bit definitions and macros for MCF_PWM_SM3CCMPA */
#define MCF_PWM_SM3CCMPA_EDGCMP(x)           (((x)&0xFF)<<0)
#define MCF_PWM_SM3CCMPA_EDGCNT(x)           (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_PWM_SM3CCRB */
#define MCF_PWM_SM3CCRB_ARM                  (0x1)
#define MCF_PWM_SM3CCRB_ONESHOT              (0x2)
#define MCF_PWM_SM3CCRB_EDG0(x)              (((x)&0x3)<<0x2)
#define MCF_PWM_SM3CCRB_EDG1(x)              (((x)&0x3)<<0x4)
#define MCF_PWM_SM3CCRB_INP_SEL              (0x40)
#define MCF_PWM_SM3CCRB_EDGCNT_EN            (0x80)
#define MCF_PWM_SM3CCRB_CFWM(x)              (((x)&0x3)<<0x8)
#define MCF_PWM_SM3CCRB_C0CNT(x)             (((x)&0x7)<<0xA)
#define MCF_PWM_SM3CCRB_C1CNT(x)             (((x)&0x7)<<0xD)

/* Bit definitions and macros for MCF_PWM_SM3CCMPB */
#define MCF_PWM_SM3CCMPB_EDGCMP(x)           (((x)&0xFF)<<0)
#define MCF_PWM_SM3CCMPB_EDGCNT(x)           (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_PWM_SM3CCRX */
#define MCF_PWM_SM3CCRX_ARM                  (0x1)
#define MCF_PWM_SM3CCRX_ONESHOT              (0x2)
#define MCF_PWM_SM3CCRX_EDG0(x)              (((x)&0x3)<<0x2)
#define MCF_PWM_SM3CCRX_EDG1(x)              (((x)&0x3)<<0x4)
#define MCF_PWM_SM3CCRX_INP_SEL              (0x40)
#define MCF_PWM_SM3CCRX_EDGCNT_EN            (0x80)
#define MCF_PWM_SM3CCRX_CFWM(x)              (((x)&0x3)<<0x8)
#define MCF_PWM_SM3CCRX_C0CNT(x)             (((x)&0x7)<<0xA)
#define MCF_PWM_SM3CCRX_C1CNT(x)             (((x)&0x7)<<0xD)

/* Bit definitions and macros for MCF_PWM_SM3CCMPX */
#define MCF_PWM_SM3CCMPX_EDGCMP(x)           (((x)&0xFF)<<0)
#define MCF_PWM_SM3CCMPX_EDGCNT(x)           (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_PWM_SM3CVAL0 */
#define MCF_PWM_SM3CVAL0_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3CCYC0 */
#define MCF_PWM_SM3CCYC0_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3CVAL1 */
#define MCF_PWM_SM3CVAL1_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3CCYC1 */
#define MCF_PWM_SM3CCYC1_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3CVAL2 */
#define MCF_PWM_SM3CVAL2_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3CCYC2 */
#define MCF_PWM_SM3CCYC2_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3CVAL3 */
#define MCF_PWM_SM3CVAL3_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3CCYC3 */
#define MCF_PWM_SM3CCYC3_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3CVAL4 */
#define MCF_PWM_SM3CVAL4_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3CCYC4 */
#define MCF_PWM_SM3CCYC4_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3CVAL5 */
#define MCF_PWM_SM3CVAL5_CAPTVAL(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PWM_SM3CCYC5 */
#define MCF_PWM_SM3CCYC5_CYC(x)              (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PWM_OUTEN */
#define MCF_PWM_OUTEN_PWMX_EN(x)             (((x)&0xF)<<0)
#define MCF_PWM_OUTEN_PWMB_EN(x)             (((x)&0xF)<<0x4)
#define MCF_PWM_OUTEN_PWMA_EN(x)             (((x)&0xF)<<0x8)

/* Bit definitions and macros for MCF_PWM_MASK */
#define MCF_PWM_MASK_MASKX(x)                (((x)&0xF)<<0)
#define MCF_PWM_MASK_MASKB(x)                (((x)&0xF)<<0x4)
#define MCF_PWM_MASK_MASKA(x)                (((x)&0xF)<<0x8)

/* Bit definitions and macros for MCF_PWM_SWCOUT */
#define MCF_PWM_SWCOUT_OUT45_0               (0x1)
#define MCF_PWM_SWCOUT_OUT23_0               (0x2)
#define MCF_PWM_SWCOUT_OUT45_1               (0x4)
#define MCF_PWM_SWCOUT_OUT23_1               (0x8)
#define MCF_PWM_SWCOUT_OUT45_2               (0x10)
#define MCF_PWM_SWCOUT_OUT23_2               (0x20)
#define MCF_PWM_SWCOUT_OUT45_3               (0x40)
#define MCF_PWM_SWCOUT_OUT23_3               (0x80)

/* Bit definitions and macros for MCF_PWM_DTSS */
#define MCF_PWM_DTSS_SEL45_0(x)              (((x)&0x3)<<0)
#define MCF_PWM_DTSS_SEL23_0(x)              (((x)&0x3)<<0x2)
#define MCF_PWM_DTSS_SEL45_1(x)              (((x)&0x3)<<0x4)
#define MCF_PWM_DTSS_SEL23_1(x)              (((x)&0x3)<<0x6)
#define MCF_PWM_DTSS_SEL45_2(x)              (((x)&0x3)<<0x8)
#define MCF_PWM_DTSS_SEL23_2(x)              (((x)&0x3)<<0xA)
#define MCF_PWM_DTSS_SEL45_3(x)              (((x)&0x3)<<0xC)
#define MCF_PWM_DTSS_SEL23_3(x)              (((x)&0x3)<<0xE)

/* Bit definitions and macros for MCF_PWM_MCR */
#define MCF_PWM_MCR_LDOK(x)                  (((x)&0xF)<<0)
#define MCF_PWM_MCR_CLDOK(x)                 (((x)&0xF)<<0x4)
#define MCF_PWM_MCR_RUN(x)                   (((x)&0xF)<<0x8)
#define MCF_PWM_MCR_IPOL(x)                  (((x)&0xF)<<0xC)

/* Bit definitions and macros for MCF_PWM_FCR */
#define MCF_PWM_FCR_FIE(x)                   (((x)&0x7)<<0)
#define MCF_PWM_FCR_FSAFE(x)                 (((x)&0x7)<<0x4)
#define MCF_PWM_FCR_FAUTO(x)                 (((x)&0x7)<<0x8)
#define MCF_PWM_FCR_FLVL(x)                  (((x)&0x7)<<0xC)

/* Bit definitions and macros for MCF_PWM_FSR */
#define MCF_PWM_FSR_FFLAG(x)                 (((x)&0x7)<<0)
#define MCF_PWM_FSR_FFULL(x)                 (((x)&0x7)<<0x4)
#define MCF_PWM_FSR_FFPIN(x)                 (((x)&0x7)<<0x8)
#define MCF_PWM_FSR_FTEST                    (0x1000)

/* Bit definitions and macros for MCF_PWM_FFILT */
#define MCF_PWM_FFILT_PER(x)                 (((x)&0xFF)<<0)
#define MCF_PWM_FFILT_CNT(x)                 (((x)&0x7)<<0x8)
#define MCF_PWM_FFILT_GSTR                   (0x8000)


#endif /* __MCF54418_PWM_H__ */
