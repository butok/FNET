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

#ifndef __MCF54418_SBF_H__
#define __MCF54418_SBF_H__


/*********************************************************************
*
* Serial Boot Facility (SBF)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_SBF_SBFSR                        (*(vuint16*)(0xFC0A0020))
#define MCF_SBF_SBFCR                        (*(vuint16*)(0xFC0A0022))


/* Bit definitions and macros for MCF_SBF_SBFSR */
#define MCF_SBF_SBFSR_BLL(x)                 (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_SBF_SBFCR */
#define MCF_SBF_SBFCR_BLDIV(x)               (((x)&0xF)<<0)
#define MCF_SBF_SBFCR_BLDIV_1                (0)
#define MCF_SBF_SBFCR_BLDIV_2                (0x1)
#define MCF_SBF_SBFCR_BLDIV_3                (0x2)
#define MCF_SBF_SBFCR_BLDIV_4                (0x3)
#define MCF_SBF_SBFCR_BLDIV_5                (0x4)
#define MCF_SBF_SBFCR_BLDIV_7                (0x5)
#define MCF_SBF_SBFCR_BLDIV_10               (0x6)
#define MCF_SBF_SBFCR_BLDIV_13               (0x7)
#define MCF_SBF_SBFCR_BLDIV_14               (0x8)
#define MCF_SBF_SBFCR_BLDIV_17               (0x9)
#define MCF_SBF_SBFCR_BLDIV_25               (0xA)
#define MCF_SBF_SBFCR_BLDIV_33               (0xB)
#define MCF_SBF_SBFCR_BLDIV_34               (0xC)
#define MCF_SBF_SBFCR_BLDIV_50               (0xD)
#define MCF_SBF_SBFCR_BLDIV_67               (0xE)
#define MCF_SBF_SBFCR_FR                     (0x10)


#endif /* __MCF54418_SBF_H__ */
