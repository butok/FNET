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

#ifndef __MCF54418_SIM_H__
#define __MCF54418_SIM_H__


/*********************************************************************
*
* Subscriber Identification Module (SIM)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_SIM_CR1                          (*(vuint32*)(0xFC0AC000))
#define MCF_SIM_SETUP                        (*(vuint32*)(0xFC0AC004))
#define MCF_SIM_DETECT1                      (*(vuint32*)(0xFC0AC008))
#define MCF_SIM_TBUF1                        (*(vuint32*)(0xFC0AC00C))
#define MCF_SIM_RBUF1                        (*(vuint32*)(0xFC0AC010))
#define MCF_SIM_CR0                          (*(vuint32*)(0xFC0AC014))
#define MCF_SIM_CR                           (*(vuint32*)(0xFC0AC018))
#define MCF_SIM_PRE                          (*(vuint32*)(0xFC0AC01C))
#define MCF_SIM_RTHR                         (*(vuint32*)(0xFC0AC020))
#define MCF_SIM_EN                           (*(vuint32*)(0xFC0AC024))
#define MCF_SIM_TSR                          (*(vuint32*)(0xFC0AC028))
#define MCF_SIM_RSR                          (*(vuint32*)(0xFC0AC02C))
#define MCF_SIM_IMR                          (*(vuint32*)(0xFC0AC030))
#define MCF_SIM_TBUF0                        (*(vuint32*)(0xFC0AC034))
#define MCF_SIM_RBUF0                        (*(vuint32*)(0xFC0AC038))
#define MCF_SIM_DETECT0                      (*(vuint32*)(0xFC0AC03C))
#define MCF_SIM_FORMAT                       (*(vuint32*)(0xFC0AC040))
#define MCF_SIM_TTHR                         (*(vuint32*)(0xFC0AC044))
#define MCF_SIM_TGCR                         (*(vuint32*)(0xFC0AC048))
#define MCF_SIM_ODCR                         (*(vuint32*)(0xFC0AC04C))
#define MCF_SIM_RCR                          (*(vuint32*)(0xFC0AC050))
#define MCF_SIM_CWTR                         (*(vuint32*)(0xFC0AC054))
#define MCF_SIM_GPCNT                        (*(vuint32*)(0xFC0AC058))
#define MCF_SIM_DIV                          (*(vuint32*)(0xFC0AC05C))
#define MCF_SIM_BWTL                         (*(vuint32*)(0xFC0AC060))
#define MCF_SIM_BGT                          (*(vuint32*)(0xFC0AC064))
#define MCF_SIM_BWTH                         (*(vuint32*)(0xFC0AC068))
#define MCF_SIM_TFSR                         (*(vuint32*)(0xFC0AC06C))
#define MCF_SIM_RFCR                         (*(vuint32*)(0xFC0AC070))
#define MCF_SIM_RFWP                         (*(vuint32*)(0xFC0AC074))
#define MCF_SIM_RFRP                         (*(vuint32*)(0xFC0AC078))


/* Bit definitions and macros for MCF_SIM_CR1 */
#define MCF_SIM_CR1_SAPD                     (0x1)
#define MCF_SIM_CR1_SVEN                     (0x2)
#define MCF_SIM_CR1_STEN                     (0x4)
#define MCF_SIM_CR1_SRST                     (0x8)
#define MCF_SIM_CR1_SCEN                     (0x10)
#define MCF_SIM_CR1_SCSP                     (0x20)
#define MCF_SIM_CR1_3VOLT                    (0x40)
#define MCF_SIM_CR1_SFPD                     (0x80)

/* Bit definitions and macros for MCF_SIM_SETUP */
#define MCF_SIM_SETUP_AMODE                  (0x1)
#define MCF_SIM_SETUP_SPS                    (0x2)

/* Bit definitions and macros for MCF_SIM_DETECT1 */
#define MCF_SIM_DETECT1_SDIM                 (0x1)
#define MCF_SIM_DETECT1_SDI                  (0x2)
#define MCF_SIM_DETECT1_SPDP                 (0x4)
#define MCF_SIM_DETECT1_SPDS                 (0x8)

/* Bit definitions and macros for MCF_SIM_TBUF1 */
#define MCF_SIM_TBUF1_TXBUF(x)               (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_SIM_RBUF1 */
#define MCF_SIM_RBUF1_RXBUF(x)               (((x)&0xFF)<<0)
#define MCF_SIM_RBUF1_PE                     (0x100)
#define MCF_SIM_RBUF1_FE                     (0x200)
#define MCF_SIM_RBUF1_CWT                    (0x400)

/* Bit definitions and macros for MCF_SIM_CR0 */
#define MCF_SIM_CR0_SAPD                     (0x1)
#define MCF_SIM_CR0_SVEN                     (0x2)
#define MCF_SIM_CR0_STEN                     (0x4)
#define MCF_SIM_CR0_SRST                     (0x8)
#define MCF_SIM_CR0_SCEN                     (0x10)
#define MCF_SIM_CR0_SCSP                     (0x20)
#define MCF_SIM_CR0_3VOLT                    (0x40)
#define MCF_SIM_CR0_SFPD                     (0x80)

/* Bit definitions and macros for MCF_SIM_CR */
#define MCF_SIM_CR_ICM                       (0x2)
#define MCF_SIM_CR_ANACK                     (0x4)
#define MCF_SIM_CR_ONACK                     (0x8)
#define MCF_SIM_CR_SAMPLE12                  (0x10)
#define MCF_SIM_CR_BAUD_SEL(x)               (((x)&0x7)<<0x6)
#define MCF_SIM_CR_GPCNT_CLKSEL(x)           (((x)&0x3)<<0x9)
#define MCF_SIM_CR_CWTEN                     (0x800)
#define MCF_SIM_CR_LRCEN                     (0x1000)
#define MCF_SIM_CR_CRCEN                     (0x2000)
#define MCF_SIM_CR_XMT_CRC_LRC               (0x4000)
#define MCF_SIM_CR_BWTEN                     (0x8000)

/* Bit definitions and macros for MCF_SIM_PRE */
#define MCF_SIM_PRE_PRESCALER(x)             (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_SIM_RTHR */
#define MCF_SIM_RTHR_RDT(x)                  (((x)&0x1F)<<0)
#define MCF_SIM_RTHR_RTH(x)                  (((x)&0xF)<<0x5)

/* Bit definitions and macros for MCF_SIM_EN */
#define MCF_SIM_EN_RXEN                      (0x1)
#define MCF_SIM_EN_TXEN                      (0x2)

/* Bit definitions and macros for MCF_SIM_TSR */
#define MCF_SIM_TSR_XTE                      (0x1)
#define MCF_SIM_TSR_TFE                      (0x8)
#define MCF_SIM_TSR_ETC                      (0x10)
#define MCF_SIM_TSR_TC                       (0x20)
#define MCF_SIM_TSR_TFO                      (0x40)
#define MCF_SIM_TSR_TDTF                     (0x80)
#define MCF_SIM_TSR_GPCNT                    (0x100)

/* Bit definitions and macros for MCF_SIM_RSR */
#define MCF_SIM_RSR_OEF                      (0x1)
#define MCF_SIM_RSR_RFD                      (0x10)
#define MCF_SIM_RSR_RDRF                     (0x20)
#define MCF_SIM_RSR_LRCOK                    (0x40)
#define MCF_SIM_RSR_CRCOK                    (0x80)
#define MCF_SIM_RSR_CWT                      (0x100)
#define MCF_SIM_RSR_RTE                      (0x200)
#define MCF_SIM_RSR_BWT                      (0x400)
#define MCF_SIM_RSR_BGT                      (0x800)

/* Bit definitions and macros for MCF_SIM_IMR */
#define MCF_SIM_IMR_RIM                      (0x1)
#define MCF_SIM_IMR_TCIM                     (0x2)
#define MCF_SIM_IMR_OIM                      (0x4)
#define MCF_SIM_IMR_ETCIM                    (0x8)
#define MCF_SIM_IMR_TFEIM                    (0x10)
#define MCF_SIM_IMR_XTM                      (0x20)
#define MCF_SIM_IMR_TFOM                     (0x40)
#define MCF_SIM_IMR_TDTFM                    (0x80)
#define MCF_SIM_IMR_GPCNTM                   (0x100)
#define MCF_SIM_IMR_CWTM                     (0x200)
#define MCF_SIM_IMR_RTM                      (0x400)
#define MCF_SIM_IMR_BWTM                     (0x800)
#define MCF_SIM_IMR_BGTM                     (0x1000)

/* Bit definitions and macros for MCF_SIM_TBUF0 */
#define MCF_SIM_TBUF0_TXBUF(x)               (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_SIM_RBUF0 */
#define MCF_SIM_RBUF0_RXBUF(x)               (((x)&0xFF)<<0)
#define MCF_SIM_RBUF0_PE                     (0x100)
#define MCF_SIM_RBUF0_FE                     (0x200)
#define MCF_SIM_RBUF0_CWT                    (0x400)

/* Bit definitions and macros for MCF_SIM_DETECT0 */
#define MCF_SIM_DETECT0_SDIM                 (0x1)
#define MCF_SIM_DETECT0_SDI                  (0x2)
#define MCF_SIM_DETECT0_SPDP                 (0x4)
#define MCF_SIM_DETECT0_SPDS                 (0x8)

/* Bit definitions and macros for MCF_SIM_FORMAT */
#define MCF_SIM_FORMAT_IC                    (0x1)

/* Bit definitions and macros for MCF_SIM_TTHR */
#define MCF_SIM_TTHR_TDT(x)                  (((x)&0xF)<<0)
#define MCF_SIM_TTHR_XTH(x)                  (((x)&0xF)<<0x4)

/* Bit definitions and macros for MCF_SIM_TGCR */
#define MCF_SIM_TGCR_GETU(x)                 (((x)&0xFF)<<0)
#define MCF_SIM_TGCR_RCVR11                  (0x100)

/* Bit definitions and macros for MCF_SIM_ODCR */
#define MCF_SIM_ODCR_ODP0                    (0x1)
#define MCF_SIM_ODCR_ODP1                    (0x2)

/* Bit definitions and macros for MCF_SIM_RCR */
#define MCF_SIM_RCR_FLUSH_RCV                (0x1)
#define MCF_SIM_RCR_FLUSH_XMT                (0x2)
#define MCF_SIM_RCR_SOFT_RST                 (0x4)
#define MCF_SIM_RCR_KILL_CLOCK               (0x8)
#define MCF_SIM_RCR_DOZE                     (0x10)
#define MCF_SIM_RCR_STOP                     (0x20)
#define MCF_SIM_RCR_DBUG                     (0x40)

/* Bit definitions and macros for MCF_SIM_CWTR */
#define MCF_SIM_CWTR_CWT(x)                  (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_SIM_GPCNT */
#define MCF_SIM_GPCNT_GPCNT(x)               (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_SIM_DIV */
#define MCF_SIM_DIV_DIVISOR(x)               (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_SIM_BWTL */
#define MCF_SIM_BWTL_BWT(x)                  (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_SIM_BGT */
#define MCF_SIM_BGT_BGT(x)                   (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_SIM_BWTH */
#define MCF_SIM_BWTH_BWT(x)                  (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_SIM_TFSR */
#define MCF_SIM_TFSR_RPTR(x)                 (((x)&0xF)<<0)
#define MCF_SIM_TFSR_WPTR(x)                 (((x)&0xF)<<0x4)
#define MCF_SIM_TFSR_CNT(x)                  (((x)&0xF)<<0x8)

/* Bit definitions and macros for MCF_SIM_RFCR */
#define MCF_SIM_RFCR_CNT(x)                  (((x)&0x1FF)<<0)

/* Bit definitions and macros for MCF_SIM_RFWP */
#define MCF_SIM_RFWP_WPTR(x)                 (((x)&0x1FF)<<0)

/* Bit definitions and macros for MCF_SIM_RFRP */
#define MCF_SIM_RFRP_RPTR(x)                 (((x)&0x1FF)<<0)


#endif /* __MCF54418_SIM_H__ */
