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

#ifndef __MCF54418_GIACR_H__
#define __MCF54418_GIACR_H__


/*********************************************************************
*
* Global Interrupt Acknowledge Control Registers Module (GIACR)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_GIACR_GSWIACK                    (*(vuint8 *)(0xFC0540E0))
#define MCF_GIACR_GL1IACK                    (*(vuint8 *)(0xFC0540E4))
#define MCF_GIACR_GL2IACK                    (*(vuint8 *)(0xFC0540E8))
#define MCF_GIACR_GL3IACK                    (*(vuint8 *)(0xFC0540EC))
#define MCF_GIACR_GL4IACK                    (*(vuint8 *)(0xFC0540F0))
#define MCF_GIACR_GL5IACK                    (*(vuint8 *)(0xFC0540F4))
#define MCF_GIACR_GL6IACK                    (*(vuint8 *)(0xFC0540F8))
#define MCF_GIACR_GL7IACK                    (*(vuint8 *)(0xFC0540FC))
#define MCF_GIACR_GLIACK(x)                  (*(vuint8 *)(0xFC0540E4 + ((x-1)*0x4)))


/* Bit definitions and macros for MCF_GIACR_GSWIACK */
#define MCF_GIACR_GSWIACK_VECTOR(x)          (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_GIACR_GLIACK */
#define MCF_GIACR_GLIACK_VECTOR(x)           (((x)&0xFF)<<0)


#endif /* __MCF54418_GIACR_H__ */
