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

#ifndef __MCF54418_CCM_H__
#define __MCF54418_CCM_H__


/*********************************************************************
*
* Chip Configuration Module (CCM)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_CCM_CCR                          (*(vuint16*)(0xEC090004))
#define MCF_CCM_RCON                         (*(vuint16*)(0xEC090008))
#define MCF_CCM_CIR                          (*(vuint16*)(0xEC09000A))
#define MCF_CCM_MISCCR                       (*(vuint16*)(0xEC09000E))
#define MCF_CCM_CDRH                         (*(vuint16*)(0xEC090010))
#define MCF_CCM_CDRL                         (*(vuint16*)(0xEC090012))
#define MCF_CCM_UOCSR                        (*(vuint16*)(0xEC090014))
#define MCF_CCM_UHCSR                        (*(vuint16*)(0xEC090016))
#define MCF_CCM_MISCCR3                      (*(vuint16*)(0xEC090018))
#define MCF_CCM_MISCCR2                      (*(vuint16*)(0xEC09001A))
#define MCF_CCM_ADCTSR                       (*(vuint16*)(0xEC09001C))
#define MCF_CCM_DACTSR                       (*(vuint16*)(0xEC09001E))
#define MCF_CCM_FNACR                        (*(vuint32*)(0xEC090024))


/* Bit definitions and macros for MCF_CCM_CCR */
#define MCF_CCM_CCR_RESERVED                 (0x1000)
#define MCF_CCM_CCR_BOOTMEM                  (0x1001)
#define MCF_CCM_CCR_PLLMODE                  (0x1002)
#define MCF_CCM_CCR_OSCMODE                  (0x1004)
#define MCF_CCM_CCR_ALESEL                   (0x1008)
#define MCF_CCM_CCR_BOOTPS(x)                (((x)&0x3)<<0x4|0x1000)
#define MCF_CCM_CCR_PLLMULT(x)               (((x)&0x3F)<<0x6|0x1000)
#define MCF_CCM_CCR_BOOTMOD(x)               (((x)&0x3)<<0xE|0x1000)

/* Bit definitions and macros for MCF_CCM_RCON */
#define MCF_CCM_RCON_RESERVED                (0x1000)
#define MCF_CCM_RCON_BOOTMEM                 (0x1001)
#define MCF_CCM_RCON_PLLMODE                 (0x1002)
#define MCF_CCM_RCON_OSCMODE                 (0x1004)
#define MCF_CCM_RCON_ALESEL                  (0x1008)
#define MCF_CCM_RCON_BOOTPS(x)               (((x)&0x3)<<0x4|0x1000)
#define MCF_CCM_RCON_PLLMULT(x)              (((x)&0x3F)<<0x6|0x1000)

/* Bit definitions and macros for MCF_CCM_CIR */
#define MCF_CCM_CIR_PRN(x)                   (((x)&0x3F)<<0)
#define MCF_CCM_CIR_PIN(x)                   (((x)&0x3FF)<<0x6)

/* Bit definitions and macros for MCF_CCM_MISCCR */
#define MCF_CCM_MISCCR_USBSRC                (0x1)
#define MCF_CCM_MISCCR_USBPUE                (0x2)
#define MCF_CCM_MISCCR_USBOOC                (0x4)
#define MCF_CCM_MISCCR_USBHOC                (0x8)
#define MCF_CCM_MISCCR_SSI0SRC               (0x10)
#define MCF_CCM_MISCCR_SSI1SRC               (0x20)
#define MCF_CCM_MISCCR_SDHCSRC               (0x40)
#define MCF_CCM_MISCCR_BMT(x)                (((x)&0x7)<<0x8)
#define MCF_CCM_MISCCR_BMT_65536             (0)
#define MCF_CCM_MISCCR_BMT_32768             (0x100)
#define MCF_CCM_MISCCR_BMT_16384             (0x200)
#define MCF_CCM_MISCCR_BMT_8192              (0x300)
#define MCF_CCM_MISCCR_BMT_4096              (0x400)
#define MCF_CCM_MISCCR_BMT_2048              (0x500)
#define MCF_CCM_MISCCR_BMT_1024              (0x600)
#define MCF_CCM_MISCCR_BMT_512               (0x700)
#define MCF_CCM_MISCCR_BME                   (0x800)
#define MCF_CCM_MISCCR_LIMP                  (0x1000)
#define MCF_CCM_MISCCR_PLLLOCK               (0x2000)
#define MCF_CCM_MISCCR_PWMEXTCLK(x)          (((x)&0x3)<<0xE)

/* Bit definitions and macros for MCF_CCM_CDRH */
#define MCF_CCM_CDRH_SSI1DIV(x)              (((x)&0xFF)<<0)
#define MCF_CCM_CDRH_SSI0DIV(x)              (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_CCM_CDRL */
#define MCF_CCM_CDRL_LPDIV(x)                (((x)&0xF)<<0x8)

/* Bit definitions and macros for MCF_CCM_UOCSR */
#define MCF_CCM_UOCSR_XPDE                   (0x1)
#define MCF_CCM_UOCSR_UOMIE                  (0x2)
#define MCF_CCM_UOCSR_WKUP                   (0x4)
#define MCF_CCM_UOCSR_PWRFLT                 (0x8)
#define MCF_CCM_UOCSR_SEND                   (0x10)
#define MCF_CCM_UOCSR_VVLD                   (0x20)
#define MCF_CCM_UOCSR_BVLD                   (0x40)
#define MCF_CCM_UOCSR_AVLD                   (0x80)
#define MCF_CCM_UOCSR_DPPU                   (0x100)
#define MCF_CCM_UOCSR_DCR_VBUS               (0x200)
#define MCF_CCM_UOCSR_CRG_VBUS               (0x400)
#define MCF_CCM_UOCSR_DRV_VBUS               (0x800)
#define MCF_CCM_UOCSR_DMPD                   (0x1000)
#define MCF_CCM_UOCSR_DPPD                   (0x2000)

/* Bit definitions and macros for MCF_CCM_UHCSR */
#define MCF_CCM_UHCSR_XPDE                   (0x1)
#define MCF_CCM_UHCSR_UHMIE                  (0x2)
#define MCF_CCM_UHCSR_WKUP                   (0x4)
#define MCF_CCM_UHCSR_PWRFLT                 (0x8)
#define MCF_CCM_UHCSR_DRVVBUS                (0x10)
#define MCF_CCM_UHCSR_PIC(x)                 (((x)&0x3)<<0xE)

/* Bit definitions and macros for MCF_CCM_MISCCR3 */
#define MCF_CCM_MISCCR3_ENETCLK(x)           (((x)&0x7)<<0x8)
#define MCF_CCM_MISCCR3_TMRENET              (0x1000)

/* Bit definitions and macros for MCF_CCM_MISCCR2 */
#define MCF_CCM_MISCCR2_ULPI                 (0x1)
#define MCF_CCM_MISCCR2_FBHALF               (0x2)
#define MCF_CCM_MISCCR2_ADC3EN               (0x4)
#define MCF_CCM_MISCCR2_ADC7EN               (0x8)
#define MCF_CCM_MISCCR2_ADCEN                (0x10)
#define MCF_CCM_MISCCR2_DAC0SEL              (0x20)
#define MCF_CCM_MISCCR2_DAC1SEL              (0x40)
#define MCF_CCM_MISCCR2_DCCBYP               (0x80)
#define MCF_CCM_MISCCR2_PLLMODE(x)           (((x)&0x7)<<0x8)
#define MCF_CCM_MISCCR2_SWTSCR               (0x1000)
#define MCF_CCM_MISCCR2_RGPIOHALF            (0x2000)
#define MCF_CCM_MISCCR2_DDR2CLK              (0x4000)
#define MCF_CCM_MISCCR2_EXTCLKBYP            (0x8000)

/* Bit definitions and macros for MCF_CCM_ADCTSR */
#define MCF_CCM_ADCTSR_ADC0SRC(x)            (((x)&0x7)<<0)
#define MCF_CCM_ADCTSR_ADC0CH(x)             (((x)&0x3)<<0x3)
#define MCF_CCM_ADCTSR_ADC1SRC(x)            (((x)&0x7)<<0x8)
#define MCF_CCM_ADCTSR_ADC1CH(x)             (((x)&0x3)<<0xB)

/* Bit definitions and macros for MCF_CCM_DACTSR */
#define MCF_CCM_DACTSR_DAC0SRC(x)            (((x)&0x7)<<0)
#define MCF_CCM_DACTSR_DAC0CH(x)             (((x)&0x3)<<0x3)
#define MCF_CCM_DACTSR_DAC1SRC(x)            (((x)&0x7)<<0x8)
#define MCF_CCM_DACTSR_DAC1CH(x)             (((x)&0x3)<<0xB)

/* Bit definitions and macros for MCF_CCM_FNACR */
#define MCF_CCM_FNACR_MCC(x)                 (((x)&0xFFFF)<<0)
#define MCF_CCM_FNACR_PCR(x)                 (((x)&0xF)<<0x18)


#endif /* __MCF54418_CCM_H__ */
