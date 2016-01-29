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

#ifndef __MCF54418_I2C_H__
#define __MCF54418_I2C_H__


/*********************************************************************
*
* I2C Module (I2C)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_I2C2_I2ADR                       (*(vuint8 *)(0xEC010000))
#define MCF_I2C2_I2FDR                       (*(vuint8 *)(0xEC010004))
#define MCF_I2C2_I2CR                        (*(vuint8 *)(0xEC010008))
#define MCF_I2C2_I2SR                        (*(vuint8 *)(0xEC01000C))
#define MCF_I2C2_I2DR                        (*(vuint8 *)(0xEC010010))

#define MCF_I2C3_I2ADR                       (*(vuint8 *)(0xEC014000))
#define MCF_I2C3_I2FDR                       (*(vuint8 *)(0xEC014004))
#define MCF_I2C3_I2CR                        (*(vuint8 *)(0xEC014008))
#define MCF_I2C3_I2SR                        (*(vuint8 *)(0xEC01400C))
#define MCF_I2C3_I2DR                        (*(vuint8 *)(0xEC014010))

#define MCF_I2C4_I2ADR                       (*(vuint8 *)(0xEC018000))
#define MCF_I2C4_I2FDR                       (*(vuint8 *)(0xEC018004))
#define MCF_I2C4_I2CR                        (*(vuint8 *)(0xEC018008))
#define MCF_I2C4_I2SR                        (*(vuint8 *)(0xEC01800C))
#define MCF_I2C4_I2DR                        (*(vuint8 *)(0xEC018010))

#define MCF_I2C5_I2ADR                       (*(vuint8 *)(0xEC01C000))
#define MCF_I2C5_I2FDR                       (*(vuint8 *)(0xEC01C004))
#define MCF_I2C5_I2CR                        (*(vuint8 *)(0xEC01C008))
#define MCF_I2C5_I2SR                        (*(vuint8 *)(0xEC01C00C))
#define MCF_I2C5_I2DR                        (*(vuint8 *)(0xEC01C010))

#define MCF_I2C1_I2ADR                       (*(vuint8 *)(0xFC038000))
#define MCF_I2C1_I2FDR                       (*(vuint8 *)(0xFC038004))
#define MCF_I2C1_I2CR                        (*(vuint8 *)(0xFC038008))
#define MCF_I2C1_I2SR                        (*(vuint8 *)(0xFC03800C))
#define MCF_I2C1_I2DR                        (*(vuint8 *)(0xFC038010))

#define MCF_I2C0_I2ADR                       (*(vuint8 *)(0xFC058000))
#define MCF_I2C0_I2FDR                       (*(vuint8 *)(0xFC058004))
#define MCF_I2C0_I2CR                        (*(vuint8 *)(0xFC058008))
#define MCF_I2C0_I2SR                        (*(vuint8 *)(0xFC05800C))
#define MCF_I2C0_I2DR                        (*(vuint8 *)(0xFC058010))


/* Other macros */
#define MCF_I2C_I2ADR(x)                     (*(vuint8 *)(0xEC010000 + ((x<2)? (0x10048000 + ((x)*0x4000) - ((x)*0x24000)) : ((x-2)*0x4000))))))
#define MCF_I2C_I2FDR(x)                     (*(vuint8 *)(0xEC010004 + ((x<2)? (0x10048000 + ((x)*0x4000) - ((x)*0x24000)) : ((x-2)*0x4000))))))
#define MCF_I2C_I2CR(x)                      (*(vuint8 *)(0xEC010008 + ((x<2)? (0x10048000 + ((x)*0x4000) - ((x)*0x24000)) : ((x-2)*0x4000))))))
#define MCF_I2C_I2SR(x)                      (*(vuint8 *)(0xEC01000C + ((x<2)? (0x10048000 + ((x)*0x4000) - ((x)*0x24000)) : ((x-2)*0x4000))))))
#define MCF_I2C_I2DR(x)                      (*(vuint8 *)(0xEC010010 + ((x<2)? (0x10048000 + ((x)*0x4000) - ((x)*0x24000)) : ((x-2)*0x4000))))))


/* Bit definitions and macros for MCF_I2C_I2ADR */
#define MCF_I2C_I2ADR_ADR(x)                 (((x)&0x7F)<<0x1)

/* Bit definitions and macros for MCF_I2C_I2FDR */
#define MCF_I2C_I2FDR_IC(x)                  (((x)&0x3F)<<0)

/* Bit definitions and macros for MCF_I2C_I2CR */
#define MCF_I2C_I2CR_DMAEN                   (0x1)
#define MCF_I2C_I2CR_RSTA                    (0x4)
#define MCF_I2C_I2CR_TXAK                    (0x8)
#define MCF_I2C_I2CR_MTX                     (0x10)
#define MCF_I2C_I2CR_MSTA                    (0x20)
#define MCF_I2C_I2CR_IIEN                    (0x40)
#define MCF_I2C_I2CR_IEN                     (0x80)

/* Bit definitions and macros for MCF_I2C_I2SR */
#define MCF_I2C_I2SR_RXAK                    (0x1)
#define MCF_I2C_I2SR_IIF                     (0x2)
#define MCF_I2C_I2SR_SRW                     (0x4)
#define MCF_I2C_I2SR_IAL                     (0x10)
#define MCF_I2C_I2SR_IBB                     (0x20)
#define MCF_I2C_I2SR_IAAS                    (0x40)
#define MCF_I2C_I2SR_ICF                     (0x80)

/* Bit definitions and macros for MCF_I2C_I2DR */
#define MCF_I2C_I2DR_DATA(x)                 (((x)&0xFF)<<0)


#endif /* __MCF54418_I2C_H__ */
