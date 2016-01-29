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

#ifndef __MCF54418_SSI_H__
#define __MCF54418_SSI_H__


/*********************************************************************
*
* Synchronous Serial Interface (SSI)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_SSI0_TX0                         (*(vuint32*)(0xFC0BC000))
#define MCF_SSI0_TX1                         (*(vuint32*)(0xFC0BC004))
#define MCF_SSI0_RX0                         (*(vuint32*)(0xFC0BC008))
#define MCF_SSI0_RX1                         (*(vuint32*)(0xFC0BC00C))
#define MCF_SSI0_CR                          (*(vuint32*)(0xFC0BC010))
#define MCF_SSI0_ISR                         (*(vuint32*)(0xFC0BC014))
#define MCF_SSI0_IER                         (*(vuint32*)(0xFC0BC018))
#define MCF_SSI0_TCR                         (*(vuint32*)(0xFC0BC01C))
#define MCF_SSI0_RCR                         (*(vuint32*)(0xFC0BC020))
#define MCF_SSI0_CCR                         (*(vuint32*)(0xFC0BC024))
#define MCF_SSI0_FCSR                        (*(vuint32*)(0xFC0BC02C))
#define MCF_SSI0_ACR                         (*(vuint32*)(0xFC0BC038))
#define MCF_SSI0_ACADD                       (*(vuint32*)(0xFC0BC03C))
#define MCF_SSI0_ACDAT                       (*(vuint32*)(0xFC0BC040))
#define MCF_SSI0_ATAG                        (*(vuint32*)(0xFC0BC044))
#define MCF_SSI0_TMASK                       (*(vuint32*)(0xFC0BC048))
#define MCF_SSI0_RMASK                       (*(vuint32*)(0xFC0BC04C))
#define MCF_SSI0_ACCSR                       (*(vuint32*)(0xFC0BC050))
#define MCF_SSI0_ACCEN                       (*(vuint32*)(0xFC0BC054))
#define MCF_SSI0_ACCDIS                      (*(vuint32*)(0xFC0BC058))
#define MCF_SSI0_TX(x)                       (*(vuint32*)(0xFC0BC000 + ((x)*0x4)))
#define MCF_SSI0_RX(x)                       (*(vuint32*)(0xFC0BC008 + ((x)*0x4)))

#define MCF_SSI1_TX0                         (*(vuint32*)(0xFC0C8000))
#define MCF_SSI1_TX1                         (*(vuint32*)(0xFC0C8004))
#define MCF_SSI1_RX0                         (*(vuint32*)(0xFC0C8008))
#define MCF_SSI1_RX1                         (*(vuint32*)(0xFC0C800C))
#define MCF_SSI1_CR                          (*(vuint32*)(0xFC0C8010))
#define MCF_SSI1_ISR                         (*(vuint32*)(0xFC0C8014))
#define MCF_SSI1_IER                         (*(vuint32*)(0xFC0C8018))
#define MCF_SSI1_TCR                         (*(vuint32*)(0xFC0C801C))
#define MCF_SSI1_RCR                         (*(vuint32*)(0xFC0C8020))
#define MCF_SSI1_CCR                         (*(vuint32*)(0xFC0C8024))
#define MCF_SSI1_FCSR                        (*(vuint32*)(0xFC0C802C))
#define MCF_SSI1_ACR                         (*(vuint32*)(0xFC0C8038))
#define MCF_SSI1_ACADD                       (*(vuint32*)(0xFC0C803C))
#define MCF_SSI1_ACDAT                       (*(vuint32*)(0xFC0C8040))
#define MCF_SSI1_ATAG                        (*(vuint32*)(0xFC0C8044))
#define MCF_SSI1_TMASK                       (*(vuint32*)(0xFC0C8048))
#define MCF_SSI1_RMASK                       (*(vuint32*)(0xFC0C804C))
#define MCF_SSI1_ACCSR                       (*(vuint32*)(0xFC0C8050))
#define MCF_SSI1_ACCEN                       (*(vuint32*)(0xFC0C8054))
#define MCF_SSI1_ACCDIS                      (*(vuint32*)(0xFC0C8058))
#define MCF_SSI1_TX(x)                       (*(vuint32*)(0xFC0C8000 + ((x)*0x4)))
#define MCF_SSI1_RX(x)                       (*(vuint32*)(0xFC0C8008 + ((x)*0x4)))

#define MCF_SSI_TX0(x)                       (*(vuint32*)(0xFC0BC000 + ((x)*0xC000)))
#define MCF_SSI_TX1(x)                       (*(vuint32*)(0xFC0BC004 + ((x)*0xC000)))
#define MCF_SSI_RX0(x)                       (*(vuint32*)(0xFC0BC008 + ((x)*0xC000)))
#define MCF_SSI_RX1(x)                       (*(vuint32*)(0xFC0BC00C + ((x)*0xC000)))
#define MCF_SSI_CR(x)                        (*(vuint32*)(0xFC0BC010 + ((x)*0xC000)))
#define MCF_SSI_ISR(x)                       (*(vuint32*)(0xFC0BC014 + ((x)*0xC000)))
#define MCF_SSI_IER(x)                       (*(vuint32*)(0xFC0BC018 + ((x)*0xC000)))
#define MCF_SSI_TCR(x)                       (*(vuint32*)(0xFC0BC01C + ((x)*0xC000)))
#define MCF_SSI_RCR(x)                       (*(vuint32*)(0xFC0BC020 + ((x)*0xC000)))
#define MCF_SSI_CCR(x)                       (*(vuint32*)(0xFC0BC024 + ((x)*0xC000)))
#define MCF_SSI_FCSR(x)                      (*(vuint32*)(0xFC0BC02C + ((x)*0xC000)))
#define MCF_SSI_ACR(x)                       (*(vuint32*)(0xFC0BC038 + ((x)*0xC000)))
#define MCF_SSI_ACADD(x)                     (*(vuint32*)(0xFC0BC03C + ((x)*0xC000)))
#define MCF_SSI_ACDAT(x)                     (*(vuint32*)(0xFC0BC040 + ((x)*0xC000)))
#define MCF_SSI_ATAG(x)                      (*(vuint32*)(0xFC0BC044 + ((x)*0xC000)))
#define MCF_SSI_TMASK(x)                     (*(vuint32*)(0xFC0BC048 + ((x)*0xC000)))
#define MCF_SSI_RMASK(x)                     (*(vuint32*)(0xFC0BC04C + ((x)*0xC000)))
#define MCF_SSI_ACCSR(x)                     (*(vuint32*)(0xFC0BC050 + ((x)*0xC000)))
#define MCF_SSI_ACCEN(x)                     (*(vuint32*)(0xFC0BC054 + ((x)*0xC000)))
#define MCF_SSI_ACCDIS(x)                    (*(vuint32*)(0xFC0BC058 + ((x)*0xC000)))


/* Bit definitions and macros for MCF_SSI_TX */
#define MCF_SSI_TX_SSI_TX(x)                 (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_SSI_RX */
#define MCF_SSI_RX_SSI_RX(x)                 (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_SSI_CR */
#define MCF_SSI_CR_SSI_EN                    (0x1)
#define MCF_SSI_CR_TE                        (0x2)
#define MCF_SSI_CR_RE                        (0x4)
#define MCF_SSI_CR_NET                       (0x8)
#define MCF_SSI_CR_SYN                       (0x10)
#define MCF_SSI_CR_I2S(x)                    (((x)&0x3)<<0x5)
#define MCF_SSI_CR_I2S_NORMAL                (0)
#define MCF_SSI_CR_I2S_MASTER                (0x20)
#define MCF_SSI_CR_I2S_SLAVE                 (0x40)
#define MCF_SSI_CR_MCE                       (0x80)
#define MCF_SSI_CR_TCH                       (0x100)
#define MCF_SSI_CR_CIS                       (0x200)
#define MCF_SSI_CR_TCD                       (0x400)
#define MCF_SSI_CR_RCD                       (0x800)

/* Bit definitions and macros for MCF_SSI_ISR */
#define MCF_SSI_ISR_TFE0                     (0x1)
#define MCF_SSI_ISR_TFE1                     (0x2)
#define MCF_SSI_ISR_RFF0                     (0x4)
#define MCF_SSI_ISR_RFF1                     (0x8)
#define MCF_SSI_ISR_RLS                      (0x10)
#define MCF_SSI_ISR_TLS                      (0x20)
#define MCF_SSI_ISR_RFS                      (0x40)
#define MCF_SSI_ISR_TFS                      (0x80)
#define MCF_SSI_ISR_TUE0                     (0x100)
#define MCF_SSI_ISR_TUE1                     (0x200)
#define MCF_SSI_ISR_ROE0                     (0x400)
#define MCF_SSI_ISR_ROE1                     (0x800)
#define MCF_SSI_ISR_TDE0                     (0x1000)
#define MCF_SSI_ISR_TDE1                     (0x2000)
#define MCF_SSI_ISR_RDR0                     (0x4000)
#define MCF_SSI_ISR_RDR1                     (0x8000)
#define MCF_SSI_ISR_RXT                      (0x10000)
#define MCF_SSI_ISR_CMDDU                    (0x20000)
#define MCF_SSI_ISR_CMDAU                    (0x40000)
#define MCF_SSI_ISR_TFRC                     (0x800000)
#define MCF_SSI_ISR_RFRC                     (0x1000000)

/* Bit definitions and macros for MCF_SSI_IER */
#define MCF_SSI_IER_TFE0                     (0x1)
#define MCF_SSI_IER_TFE1                     (0x2)
#define MCF_SSI_IER_RFF0                     (0x4)
#define MCF_SSI_IER_RFF1                     (0x8)
#define MCF_SSI_IER_RLS                      (0x10)
#define MCF_SSI_IER_TLS                      (0x20)
#define MCF_SSI_IER_RFS                      (0x40)
#define MCF_SSI_IER_TFS                      (0x80)
#define MCF_SSI_IER_TUE0                     (0x100)
#define MCF_SSI_IER_TUE1                     (0x200)
#define MCF_SSI_IER_ROE0                     (0x400)
#define MCF_SSI_IER_ROE1                     (0x800)
#define MCF_SSI_IER_TDE0                     (0x1000)
#define MCF_SSI_IER_TDE1                     (0x2000)
#define MCF_SSI_IER_RDR0                     (0x4000)
#define MCF_SSI_IER_RDR1                     (0x8000)
#define MCF_SSI_IER_RXT                      (0x10000)
#define MCF_SSI_IER_CMDDU                    (0x20000)
#define MCF_SSI_IER_CMDAU                    (0x40000)
#define MCF_SSI_IER_TIE                      (0x80000)
#define MCF_SSI_IER_TDMAE                    (0x100000)
#define MCF_SSI_IER_RIE                      (0x200000)
#define MCF_SSI_IER_RDMAE                    (0x400000)
#define MCF_SSI_IER_TFRC                     (0x800000)
#define MCF_SSI_IER_RFRC                     (0x1000000)

/* Bit definitions and macros for MCF_SSI_TCR */
#define MCF_SSI_TCR_TEFS                     (0x1)
#define MCF_SSI_TCR_TFSL                     (0x2)
#define MCF_SSI_TCR_TFSI                     (0x4)
#define MCF_SSI_TCR_TSCKP                    (0x8)
#define MCF_SSI_TCR_TSHFD                    (0x10)
#define MCF_SSI_TCR_TXDIR                    (0x20)
#define MCF_SSI_TCR_TFDIR                    (0x40)
#define MCF_SSI_TCR_TFEN0                    (0x80)
#define MCF_SSI_TCR_TFEN1                    (0x100)
#define MCF_SSI_TCR_TXBIT0                   (0x200)

/* Bit definitions and macros for MCF_SSI_RCR */
#define MCF_SSI_RCR_REFS                     (0x1)
#define MCF_SSI_RCR_RFSL                     (0x2)
#define MCF_SSI_RCR_RFSI                     (0x4)
#define MCF_SSI_RCR_RSCKP                    (0x8)
#define MCF_SSI_RCR_RSHFD                    (0x10)
#define MCF_SSI_RCR_RXDIR                    (0x20)
#define MCF_SSI_RCR_RFEN0                    (0x80)
#define MCF_SSI_RCR_RFEN1                    (0x100)
#define MCF_SSI_RCR_RXBIT0                   (0x200)
#define MCF_SSI_RCR_RXEXT                    (0x400)

/* Bit definitions and macros for MCF_SSI_CCR */
#define MCF_SSI_CCR_PM(x)                    (((x)&0xFF)<<0)
#define MCF_SSI_CCR_DC(x)                    (((x)&0x1F)<<0x8)
#define MCF_SSI_CCR_WL(x)                    (((x)&0xF)<<0xD)
#define MCF_SSI_CCR_PSR                      (0x20000)
#define MCF_SSI_CCR_DIV2                     (0x40000)

/* Bit definitions and macros for MCF_SSI_FCSR */
#define MCF_SSI_FCSR_TFWM0(x)                (((x)&0xF)<<0)
#define MCF_SSI_FCSR_RFWM0(x)                (((x)&0xF)<<0x4)
#define MCF_SSI_FCSR_TFCNT0(x)               (((x)&0xF)<<0x8)
#define MCF_SSI_FCSR_RFCNT0(x)               (((x)&0xF)<<0xC)
#define MCF_SSI_FCSR_TFWM1(x)                (((x)&0xF)<<0x10)
#define MCF_SSI_FCSR_RFWM1(x)                (((x)&0xF)<<0x14)
#define MCF_SSI_FCSR_TFCNT1(x)               (((x)&0xF)<<0x18)
#define MCF_SSI_FCSR_RFCNT1(x)               (((x)&0xF)<<0x1C)

/* Bit definitions and macros for MCF_SSI_ACR */
#define MCF_SSI_ACR_AC97EN                   (0x1)
#define MCF_SSI_ACR_FV                       (0x2)
#define MCF_SSI_ACR_TIF                      (0x4)
#define MCF_SSI_ACR_RD                       (0x8)
#define MCF_SSI_ACR_WR                       (0x10)
#define MCF_SSI_ACR_FRDIV(x)                 (((x)&0x3F)<<0x5)

/* Bit definitions and macros for MCF_SSI_ACADD */
#define MCF_SSI_ACADD_SSI_ACADD(x)           (((x)&0x7FFFF)<<0)

/* Bit definitions and macros for MCF_SSI_ACDAT */
#define MCF_SSI_ACDAT_SSI_ACDAT(x)           (((x)&0xFFFFF)<<0)

/* Bit definitions and macros for MCF_SSI_ATAG */
#define MCF_SSI_ATAG_SSI_ATAG(x)             (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_SSI_TMASK */
#define MCF_SSI_TMASK_SSI_TMASK(x)           (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_SSI_RMASK */
#define MCF_SSI_RMASK_SSI_RMASK(x)           (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_SSI_ACCSR */
#define MCF_SSI_ACCSR_ACCSR(x)               (((x)&0x3FF)<<0)

/* Bit definitions and macros for MCF_SSI_ACCEN */
#define MCF_SSI_ACCEN_ACCEN(x)               (((x)&0x3FF)<<0)

/* Bit definitions and macros for MCF_SSI_ACCDIS */
#define MCF_SSI_ACCDIS_ACCDIS(x)             (((x)&0x3FF)<<0)


#endif /* __MCF54418_SSI_H__ */
