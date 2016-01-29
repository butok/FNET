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

#ifndef __MCF54418_RGPIO_H__
#define __MCF54418_RGPIO_H__


/*********************************************************************
*
* Rapid GPIO (RGPIO) module
*
*********************************************************************/

/* Register read/write macros */
#define MCF_RGPIO_DIR                        (*(vuint16*)(0x8C000000))
#define MCF_RGPIO_DATA                       (*(vuint16*)(0x8C000002))
#define MCF_RGPIO_ENB                        (*(vuint16*)(0x8C000004))
#define MCF_RGPIO_CLR                        (*(vuint16*)(0x8C000006))
#define MCF_RGPIO_DATA_1                     (*(vuint16*)(0x8C000006))
#define MCF_RGPIO_DIR_1                      (*(vuint16*)(0x8C000008))
#define MCF_RGPIO_SET                        (*(vuint16*)(0x8C00000A))
#define MCF_RGPIO_DATA_2                     (*(vuint16*)(0x8C00000A))
#define MCF_RGPIO_DIR_2                      (*(vuint16*)(0x8C00000C))
#define MCF_RGPIO_TOG                        (*(vuint16*)(0x8C00000E))
#define MCF_RGPIO_DATA_3                     (*(vuint16*)(0x8C00000E))


/* Bit definitions and macros for MCF_RGPIO_DIR */
#define MCF_RGPIO_DIR_DIR0                   (0x1)
#define MCF_RGPIO_DIR_DIR1                   (0x2)
#define MCF_RGPIO_DIR_DIR2                   (0x4)
#define MCF_RGPIO_DIR_DIR3                   (0x8)
#define MCF_RGPIO_DIR_DIR4                   (0x10)
#define MCF_RGPIO_DIR_DIR5                   (0x20)
#define MCF_RGPIO_DIR_DIR6                   (0x40)
#define MCF_RGPIO_DIR_DIR7                   (0x80)
#define MCF_RGPIO_DIR_DIR8                   (0x100)
#define MCF_RGPIO_DIR_DIR9                   (0x200)
#define MCF_RGPIO_DIR_DIR10                  (0x400)
#define MCF_RGPIO_DIR_DIR11                  (0x800)
#define MCF_RGPIO_DIR_DIR12                  (0x1000)
#define MCF_RGPIO_DIR_DIR13                  (0x2000)
#define MCF_RGPIO_DIR_DIR14                  (0x4000)
#define MCF_RGPIO_DIR_DIR15                  (0x8000)

/* Bit definitions and macros for MCF_RGPIO_DATA */
#define MCF_RGPIO_DATA_DATA0                 (0x1)
#define MCF_RGPIO_DATA_DATA1                 (0x2)
#define MCF_RGPIO_DATA_DATA2                 (0x4)
#define MCF_RGPIO_DATA_DATA3                 (0x8)
#define MCF_RGPIO_DATA_DATA4                 (0x10)
#define MCF_RGPIO_DATA_DATA5                 (0x20)
#define MCF_RGPIO_DATA_DATA6                 (0x40)
#define MCF_RGPIO_DATA_DATA7                 (0x80)
#define MCF_RGPIO_DATA_DATA8                 (0x100)
#define MCF_RGPIO_DATA_DATA9                 (0x200)
#define MCF_RGPIO_DATA_DATA10                (0x400)
#define MCF_RGPIO_DATA_DATA11                (0x800)
#define MCF_RGPIO_DATA_DATA12                (0x1000)
#define MCF_RGPIO_DATA_DATA13                (0x2000)
#define MCF_RGPIO_DATA_DATA14                (0x4000)
#define MCF_RGPIO_DATA_DATA15                (0x8000)

/* Bit definitions and macros for MCF_RGPIO_ENB */
#define MCF_RGPIO_ENB_ENB0                   (0x1)
#define MCF_RGPIO_ENB_ENB1                   (0x2)
#define MCF_RGPIO_ENB_ENB2                   (0x4)
#define MCF_RGPIO_ENB_ENB3                   (0x8)
#define MCF_RGPIO_ENB_ENB4                   (0x10)
#define MCF_RGPIO_ENB_ENB5                   (0x20)
#define MCF_RGPIO_ENB_ENB6                   (0x40)
#define MCF_RGPIO_ENB_ENB7                   (0x80)
#define MCF_RGPIO_ENB_ENB8                   (0x100)
#define MCF_RGPIO_ENB_ENB9                   (0x200)
#define MCF_RGPIO_ENB_ENB10                  (0x400)
#define MCF_RGPIO_ENB_ENB11                  (0x800)
#define MCF_RGPIO_ENB_ENB12                  (0x1000)
#define MCF_RGPIO_ENB_ENB13                  (0x2000)
#define MCF_RGPIO_ENB_ENB14                  (0x4000)
#define MCF_RGPIO_ENB_ENB15                  (0x8000)

/* Bit definitions and macros for MCF_RGPIO_CLR */
#define MCF_RGPIO_CLR_CLR0                   (0x1)
#define MCF_RGPIO_CLR_CLR1                   (0x2)
#define MCF_RGPIO_CLR_CLR2                   (0x4)
#define MCF_RGPIO_CLR_CLR3                   (0x8)
#define MCF_RGPIO_CLR_CLR4                   (0x10)
#define MCF_RGPIO_CLR_CLR5                   (0x20)
#define MCF_RGPIO_CLR_CLR6                   (0x40)
#define MCF_RGPIO_CLR_CLR7                   (0x80)
#define MCF_RGPIO_CLR_CLR8                   (0x100)
#define MCF_RGPIO_CLR_CLR9                   (0x200)
#define MCF_RGPIO_CLR_CLR10                  (0x400)
#define MCF_RGPIO_CLR_CLR11                  (0x800)
#define MCF_RGPIO_CLR_CLR12                  (0x1000)
#define MCF_RGPIO_CLR_CLR13                  (0x2000)
#define MCF_RGPIO_CLR_CLR14                  (0x4000)
#define MCF_RGPIO_CLR_CLR15                  (0x8000)

/* Bit definitions and macros for MCF_RGPIO_DATA_1 */
#define MCF_RGPIO_DATA_1_DATA0               (0x1)
#define MCF_RGPIO_DATA_1_DATA1               (0x2)
#define MCF_RGPIO_DATA_1_DATA2               (0x4)
#define MCF_RGPIO_DATA_1_DATA3               (0x8)
#define MCF_RGPIO_DATA_1_DATA4               (0x10)
#define MCF_RGPIO_DATA_1_DATA5               (0x20)
#define MCF_RGPIO_DATA_1_DATA6               (0x40)
#define MCF_RGPIO_DATA_1_DATA7               (0x80)
#define MCF_RGPIO_DATA_1_DATA8               (0x100)
#define MCF_RGPIO_DATA_1_DATA9               (0x200)
#define MCF_RGPIO_DATA_1_DATA10              (0x400)
#define MCF_RGPIO_DATA_1_DATA11              (0x800)
#define MCF_RGPIO_DATA_1_DATA12              (0x1000)
#define MCF_RGPIO_DATA_1_DATA13              (0x2000)
#define MCF_RGPIO_DATA_1_DATA14              (0x4000)
#define MCF_RGPIO_DATA_1_DATA15              (0x8000)

/* Bit definitions and macros for MCF_RGPIO_DIR_1 */
#define MCF_RGPIO_DIR_1_DIR0                 (0x1)
#define MCF_RGPIO_DIR_1_DIR1                 (0x2)
#define MCF_RGPIO_DIR_1_DIR2                 (0x4)
#define MCF_RGPIO_DIR_1_DIR3                 (0x8)
#define MCF_RGPIO_DIR_1_DIR4                 (0x10)
#define MCF_RGPIO_DIR_1_DIR5                 (0x20)
#define MCF_RGPIO_DIR_1_DIR6                 (0x40)
#define MCF_RGPIO_DIR_1_DIR7                 (0x80)
#define MCF_RGPIO_DIR_1_DIR8                 (0x100)
#define MCF_RGPIO_DIR_1_DIR9                 (0x200)
#define MCF_RGPIO_DIR_1_DIR10                (0x400)
#define MCF_RGPIO_DIR_1_DIR11                (0x800)
#define MCF_RGPIO_DIR_1_DIR12                (0x1000)
#define MCF_RGPIO_DIR_1_DIR13                (0x2000)
#define MCF_RGPIO_DIR_1_DIR14                (0x4000)
#define MCF_RGPIO_DIR_1_DIR15                (0x8000)

/* Bit definitions and macros for MCF_RGPIO_SET */
#define MCF_RGPIO_SET_SET0                   (0x1)
#define MCF_RGPIO_SET_SET1                   (0x2)
#define MCF_RGPIO_SET_SET2                   (0x4)
#define MCF_RGPIO_SET_SET3                   (0x8)
#define MCF_RGPIO_SET_SET4                   (0x10)
#define MCF_RGPIO_SET_SET5                   (0x20)
#define MCF_RGPIO_SET_SET6                   (0x40)
#define MCF_RGPIO_SET_SET7                   (0x80)
#define MCF_RGPIO_SET_SET8                   (0x100)
#define MCF_RGPIO_SET_SET9                   (0x200)
#define MCF_RGPIO_SET_SET10                  (0x400)
#define MCF_RGPIO_SET_SET11                  (0x800)
#define MCF_RGPIO_SET_SET12                  (0x1000)
#define MCF_RGPIO_SET_SET13                  (0x2000)
#define MCF_RGPIO_SET_SET14                  (0x4000)
#define MCF_RGPIO_SET_SET15                  (0x8000)

/* Bit definitions and macros for MCF_RGPIO_DATA_2 */
#define MCF_RGPIO_DATA_2_DATA0               (0x1)
#define MCF_RGPIO_DATA_2_DATA1               (0x2)
#define MCF_RGPIO_DATA_2_DATA2               (0x4)
#define MCF_RGPIO_DATA_2_DATA3               (0x8)
#define MCF_RGPIO_DATA_2_DATA4               (0x10)
#define MCF_RGPIO_DATA_2_DATA5               (0x20)
#define MCF_RGPIO_DATA_2_DATA6               (0x40)
#define MCF_RGPIO_DATA_2_DATA7               (0x80)
#define MCF_RGPIO_DATA_2_DATA8               (0x100)
#define MCF_RGPIO_DATA_2_DATA9               (0x200)
#define MCF_RGPIO_DATA_2_DATA10              (0x400)
#define MCF_RGPIO_DATA_2_DATA11              (0x800)
#define MCF_RGPIO_DATA_2_DATA12              (0x1000)
#define MCF_RGPIO_DATA_2_DATA13              (0x2000)
#define MCF_RGPIO_DATA_2_DATA14              (0x4000)
#define MCF_RGPIO_DATA_2_DATA15              (0x8000)

/* Bit definitions and macros for MCF_RGPIO_DIR_2 */
#define MCF_RGPIO_DIR_2_DIR0                 (0x1)
#define MCF_RGPIO_DIR_2_DIR1                 (0x2)
#define MCF_RGPIO_DIR_2_DIR2                 (0x4)
#define MCF_RGPIO_DIR_2_DIR3                 (0x8)
#define MCF_RGPIO_DIR_2_DIR4                 (0x10)
#define MCF_RGPIO_DIR_2_DIR5                 (0x20)
#define MCF_RGPIO_DIR_2_DIR6                 (0x40)
#define MCF_RGPIO_DIR_2_DIR7                 (0x80)
#define MCF_RGPIO_DIR_2_DIR8                 (0x100)
#define MCF_RGPIO_DIR_2_DIR9                 (0x200)
#define MCF_RGPIO_DIR_2_DIR10                (0x400)
#define MCF_RGPIO_DIR_2_DIR11                (0x800)
#define MCF_RGPIO_DIR_2_DIR12                (0x1000)
#define MCF_RGPIO_DIR_2_DIR13                (0x2000)
#define MCF_RGPIO_DIR_2_DIR14                (0x4000)
#define MCF_RGPIO_DIR_2_DIR15                (0x8000)

/* Bit definitions and macros for MCF_RGPIO_TOG */
#define MCF_RGPIO_TOG_TOG0                   (0x1)
#define MCF_RGPIO_TOG_TOG1                   (0x2)
#define MCF_RGPIO_TOG_TOG2                   (0x4)
#define MCF_RGPIO_TOG_TOG3                   (0x8)
#define MCF_RGPIO_TOG_TOG4                   (0x10)
#define MCF_RGPIO_TOG_TOG5                   (0x20)
#define MCF_RGPIO_TOG_TOG6                   (0x40)
#define MCF_RGPIO_TOG_TOG7                   (0x80)
#define MCF_RGPIO_TOG_TOG8                   (0x100)
#define MCF_RGPIO_TOG_TOG9                   (0x200)
#define MCF_RGPIO_TOG_TOG10                  (0x400)
#define MCF_RGPIO_TOG_TOG11                  (0x800)
#define MCF_RGPIO_TOG_TOG12                  (0x1000)
#define MCF_RGPIO_TOG_TOG13                  (0x2000)
#define MCF_RGPIO_TOG_TOG14                  (0x4000)
#define MCF_RGPIO_TOG_TOG15                  (0x8000)

/* Bit definitions and macros for MCF_RGPIO_DATA_3 */
#define MCF_RGPIO_DATA_3_DATA0               (0x1)
#define MCF_RGPIO_DATA_3_DATA1               (0x2)
#define MCF_RGPIO_DATA_3_DATA2               (0x4)
#define MCF_RGPIO_DATA_3_DATA3               (0x8)
#define MCF_RGPIO_DATA_3_DATA4               (0x10)
#define MCF_RGPIO_DATA_3_DATA5               (0x20)
#define MCF_RGPIO_DATA_3_DATA6               (0x40)
#define MCF_RGPIO_DATA_3_DATA7               (0x80)
#define MCF_RGPIO_DATA_3_DATA8               (0x100)
#define MCF_RGPIO_DATA_3_DATA9               (0x200)
#define MCF_RGPIO_DATA_3_DATA10              (0x400)
#define MCF_RGPIO_DATA_3_DATA11              (0x800)
#define MCF_RGPIO_DATA_3_DATA12              (0x1000)
#define MCF_RGPIO_DATA_3_DATA13              (0x2000)
#define MCF_RGPIO_DATA_3_DATA14              (0x4000)
#define MCF_RGPIO_DATA_3_DATA15              (0x8000)


#endif /* __MCF54418_RGPIO_H__ */
