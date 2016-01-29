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

#ifndef __MCF54418_GPIO_H__
#define __MCF54418_GPIO_H__


/*********************************************************************
*
* General Purpose I/O (GPIO)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_GPIO_PODR_A                      (*(vuint8 *)(0xEC094000))
#define MCF_GPIO_PDDR_A                      (*(vuint8 *)(0xEC09400C))
#define MCF_GPIO_PPDSDR_A                    (*(vuint8 *)(0xEC094018))
#define MCF_GPIO_PCLRR_A                     (*(vuint8 *)(0xEC094024))
#define MCF_GPIO_PCR_A                       (*(vuint16*)(0xEC094030))

#define MCF_GPIO_PODR_B                      (*(vuint8 *)(0xEC094001))
#define MCF_GPIO_PDDR_B                      (*(vuint8 *)(0xEC09400D))
#define MCF_GPIO_PPDSDR_B                    (*(vuint8 *)(0xEC094019))
#define MCF_GPIO_PCLRR_B                     (*(vuint8 *)(0xEC094025))
#define MCF_GPIO_PCR_B                       (*(vuint16*)(0xEC094032))

#define MCF_GPIO_PODR_C                      (*(vuint8 *)(0xEC094002))
#define MCF_GPIO_PDDR_C                      (*(vuint8 *)(0xEC09400E))
#define MCF_GPIO_PPDSDR_C                    (*(vuint8 *)(0xEC09401A))
#define MCF_GPIO_PCLRR_C                     (*(vuint8 *)(0xEC094026))
#define MCF_GPIO_PCR_C                       (*(vuint16*)(0xEC094034))

#define MCF_GPIO_PODR_D                      (*(vuint8 *)(0xEC094003))
#define MCF_GPIO_PDDR_D                      (*(vuint8 *)(0xEC09400F))
#define MCF_GPIO_PPDSDR_D                    (*(vuint8 *)(0xEC09401B))
#define MCF_GPIO_PCLRR_D                     (*(vuint8 *)(0xEC094027))
#define MCF_GPIO_PCR_D                       (*(vuint16*)(0xEC094036))

#define MCF_GPIO_PODR_E                      (*(vuint8 *)(0xEC094004))
#define MCF_GPIO_PDDR_E                      (*(vuint8 *)(0xEC094010))
#define MCF_GPIO_PPDSDR_E                    (*(vuint8 *)(0xEC09401C))
#define MCF_GPIO_PCLRR_E                     (*(vuint8 *)(0xEC094028))
#define MCF_GPIO_PCR_E                       (*(vuint16*)(0xEC094038))

#define MCF_GPIO_PODR_F                      (*(vuint8 *)(0xEC094005))
#define MCF_GPIO_PDDR_F                      (*(vuint8 *)(0xEC094011))
#define MCF_GPIO_PPDSDR_F                    (*(vuint8 *)(0xEC09401D))
#define MCF_GPIO_PCLRR_F                     (*(vuint8 *)(0xEC094029))
#define MCF_GPIO_PCR_F                       (*(vuint16*)(0xEC09403A))

#define MCF_GPIO_PODR_G                      (*(vuint8 *)(0xEC094006))
#define MCF_GPIO_PDDR_G                      (*(vuint8 *)(0xEC094012))
#define MCF_GPIO_PPDSDR_G                    (*(vuint8 *)(0xEC09401E))
#define MCF_GPIO_PCLRR_G                     (*(vuint8 *)(0xEC09402A))
#define MCF_GPIO_PCR_G                       (*(vuint16*)(0xEC09403C))

#define MCF_GPIO_PODR_H                      (*(vuint8 *)(0xEC094007))
#define MCF_GPIO_PDDR_H                      (*(vuint8 *)(0xEC094013))
#define MCF_GPIO_PPDSDR_H                    (*(vuint8 *)(0xEC09401F))
#define MCF_GPIO_PCLRR_H                     (*(vuint8 *)(0xEC09402B))
#define MCF_GPIO_PCR_H                       (*(vuint16*)(0xEC09403E))

#define MCF_GPIO_PODR_I                      (*(vuint8 *)(0xEC094008))
#define MCF_GPIO_PDDR_I                      (*(vuint8 *)(0xEC094014))
#define MCF_GPIO_PPDSDR_I                    (*(vuint8 *)(0xEC094020))
#define MCF_GPIO_PCLRR_I                     (*(vuint8 *)(0xEC09402C))
#define MCF_GPIO_PCR_I                       (*(vuint16*)(0xEC094040))

#define MCF_GPIO_PODR_J                      (*(vuint8 *)(0xEC094009))
#define MCF_GPIO_PDDR_J                      (*(vuint8 *)(0xEC094015))
#define MCF_GPIO_PPDSDR_J                    (*(vuint8 *)(0xEC094021))
#define MCF_GPIO_PCLRR_J                     (*(vuint8 *)(0xEC09402D))
#define MCF_GPIO_PCR_J                       (*(vuint16*)(0xEC094042))

#define MCF_GPIO_PODR_K                      (*(vuint8 *)(0xEC09400A))
#define MCF_GPIO_PDDR_K                      (*(vuint8 *)(0xEC094016))
#define MCF_GPIO_PPDSDR_K                    (*(vuint8 *)(0xEC094022))
#define MCF_GPIO_PCLRR_K                     (*(vuint8 *)(0xEC09402E))
#define MCF_GPIO_PCR_K                       (*(vuint16*)(0xEC094044))



/* Bit definitions and macros for MCF_GPIO_PODR_A */
#define MCF_GPIO_PODR_A_PODR_A0              (0x1)
#define MCF_GPIO_PODR_A_PODR_A1              (0x2)
#define MCF_GPIO_PODR_A_PODR_A2              (0x4)
#define MCF_GPIO_PODR_A_PODR_A3              (0x8)
#define MCF_GPIO_PODR_A_PODR_A4              (0x10)
#define MCF_GPIO_PODR_A_PODR_A5              (0x20)
#define MCF_GPIO_PODR_A_PODR_A6              (0x40)
#define MCF_GPIO_PODR_A_PODR_A7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PDDR_A */
#define MCF_GPIO_PDDR_A_PDDR_A0              (0x1)
#define MCF_GPIO_PDDR_A_PDDR_A1              (0x2)
#define MCF_GPIO_PDDR_A_PDDR_A2              (0x4)
#define MCF_GPIO_PDDR_A_PDDR_A3              (0x8)
#define MCF_GPIO_PDDR_A_PDDR_A4              (0x10)
#define MCF_GPIO_PDDR_A_PDDR_A5              (0x20)
#define MCF_GPIO_PDDR_A_PDDR_A6              (0x40)
#define MCF_GPIO_PDDR_A_PDDR_A7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_A */
#define MCF_GPIO_PPDSDR_A_PPDSDR_A0          (0x1)
#define MCF_GPIO_PPDSDR_A_PPDSDR_A1          (0x2)
#define MCF_GPIO_PPDSDR_A_PPDSDR_A2          (0x4)
#define MCF_GPIO_PPDSDR_A_PPDSDR_A3          (0x8)
#define MCF_GPIO_PPDSDR_A_PPDSDR_A4          (0x10)
#define MCF_GPIO_PPDSDR_A_PPDSDR_A5          (0x20)
#define MCF_GPIO_PPDSDR_A_PPDSDR_A6          (0x40)
#define MCF_GPIO_PPDSDR_A_PPDSDR_A7          (0x80)

/* Bit definitions and macros for MCF_GPIO_PCLRR_A */
#define MCF_GPIO_PCLRR_A_PCLRR_A0            (0x1)
#define MCF_GPIO_PCLRR_A_PCLRR_A1            (0x2)
#define MCF_GPIO_PCLRR_A_PCLRR_A2            (0x4)
#define MCF_GPIO_PCLRR_A_PCLRR_A3            (0x8)
#define MCF_GPIO_PCLRR_A_PCLRR_A4            (0x10)
#define MCF_GPIO_PCLRR_A_PCLRR_A5            (0x20)
#define MCF_GPIO_PCLRR_A_PCLRR_A6            (0x40)
#define MCF_GPIO_PCLRR_A_PCLRR_A7            (0x80)

/* Bit definitions and macros for MCF_GPIO_PCR_A */
#define MCF_GPIO_PCR_A_PUS0                  (0x1)
#define MCF_GPIO_PCR_A_PUE0                  (0x2)
#define MCF_GPIO_PCR_A_PUS1                  (0x4)
#define MCF_GPIO_PCR_A_PUE1                  (0x8)
#define MCF_GPIO_PCR_A_PUS2                  (0x10)
#define MCF_GPIO_PCR_A_PUE2                  (0x20)
#define MCF_GPIO_PCR_A_PUS3                  (0x40)
#define MCF_GPIO_PCR_A_PUE3                  (0x80)
#define MCF_GPIO_PCR_A_PUS4                  (0x100)
#define MCF_GPIO_PCR_A_PUE4                  (0x200)
#define MCF_GPIO_PCR_A_PUS5                  (0x400)
#define MCF_GPIO_PCR_A_PUE5                  (0x800)
#define MCF_GPIO_PCR_A_PUS6                  (0x1000)
#define MCF_GPIO_PCR_A_PUE6                  (0x2000)
#define MCF_GPIO_PCR_A_PUS7                  (0x4000)
#define MCF_GPIO_PCR_A_PUE7                  (0x8000)

/* Bit definitions and macros for MCF_GPIO_PODR_B */
#define MCF_GPIO_PODR_B_PODR_B0              (0x1)
#define MCF_GPIO_PODR_B_PODR_B1              (0x2)
#define MCF_GPIO_PODR_B_PODR_B2              (0x4)
#define MCF_GPIO_PODR_B_PODR_B3              (0x8)
#define MCF_GPIO_PODR_B_PODR_B4              (0x10)
#define MCF_GPIO_PODR_B_PODR_B5              (0x20)
#define MCF_GPIO_PODR_B_PODR_B6              (0x40)
#define MCF_GPIO_PODR_B_PODR_B7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PDDR_B */
#define MCF_GPIO_PDDR_B_PDDR_B0              (0x1)
#define MCF_GPIO_PDDR_B_PDDR_B1              (0x2)
#define MCF_GPIO_PDDR_B_PDDR_B2              (0x4)
#define MCF_GPIO_PDDR_B_PDDR_B3              (0x8)
#define MCF_GPIO_PDDR_B_PDDR_B4              (0x10)
#define MCF_GPIO_PDDR_B_PDDR_B5              (0x20)
#define MCF_GPIO_PDDR_B_PDDR_B6              (0x40)
#define MCF_GPIO_PDDR_B_PDDR_B7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_B */
#define MCF_GPIO_PPDSDR_B_PPDSDR_B0          (0x1)
#define MCF_GPIO_PPDSDR_B_PPDSDR_B1          (0x2)
#define MCF_GPIO_PPDSDR_B_PPDSDR_B2          (0x4)
#define MCF_GPIO_PPDSDR_B_PPDSDR_B3          (0x8)
#define MCF_GPIO_PPDSDR_B_PPDSDR_B4          (0x10)
#define MCF_GPIO_PPDSDR_B_PPDSDR_B5          (0x20)
#define MCF_GPIO_PPDSDR_B_PPDSDR_B6          (0x40)
#define MCF_GPIO_PPDSDR_B_PPDSDR_B7          (0x80)

/* Bit definitions and macros for MCF_GPIO_PCLRR_B */
#define MCF_GPIO_PCLRR_B_PCLRR_B0            (0x1)
#define MCF_GPIO_PCLRR_B_PCLRR_B1            (0x2)
#define MCF_GPIO_PCLRR_B_PCLRR_B2            (0x4)
#define MCF_GPIO_PCLRR_B_PCLRR_B3            (0x8)
#define MCF_GPIO_PCLRR_B_PCLRR_B4            (0x10)
#define MCF_GPIO_PCLRR_B_PCLRR_B5            (0x20)
#define MCF_GPIO_PCLRR_B_PCLRR_B6            (0x40)
#define MCF_GPIO_PCLRR_B_PCLRR_B7            (0x80)

/* Bit definitions and macros for MCF_GPIO_PCR_B */
#define MCF_GPIO_PCR_B_PUS0                  (0x1)
#define MCF_GPIO_PCR_B_PUE0                  (0x2)
#define MCF_GPIO_PCR_B_PUS1                  (0x4)
#define MCF_GPIO_PCR_B_PUE1                  (0x8)
#define MCF_GPIO_PCR_B_PUS2                  (0x10)
#define MCF_GPIO_PCR_B_PUE2                  (0x20)
#define MCF_GPIO_PCR_B_PUS3                  (0x40)
#define MCF_GPIO_PCR_B_PUE3                  (0x80)
#define MCF_GPIO_PCR_B_PUS4                  (0x100)
#define MCF_GPIO_PCR_B_PUE4                  (0x200)
#define MCF_GPIO_PCR_B_PUS5                  (0x400)
#define MCF_GPIO_PCR_B_PUE5                  (0x800)
#define MCF_GPIO_PCR_B_PUS6                  (0x1000)
#define MCF_GPIO_PCR_B_PUE6                  (0x2000)
#define MCF_GPIO_PCR_B_PUS7                  (0x4000)
#define MCF_GPIO_PCR_B_PUE7                  (0x8000)

/* Bit definitions and macros for MCF_GPIO_PODR_C */
#define MCF_GPIO_PODR_C_PODR_C0              (0x1)
#define MCF_GPIO_PODR_C_PODR_C1              (0x2)
#define MCF_GPIO_PODR_C_PODR_C2              (0x4)
#define MCF_GPIO_PODR_C_PODR_C3              (0x8)
#define MCF_GPIO_PODR_C_PODR_C4              (0x10)
#define MCF_GPIO_PODR_C_PODR_C5              (0x20)
#define MCF_GPIO_PODR_C_PODR_C6              (0x40)
#define MCF_GPIO_PODR_C_PODR_C7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PDDR_C */
#define MCF_GPIO_PDDR_C_PDDR_C0              (0x1)
#define MCF_GPIO_PDDR_C_PDDR_C1              (0x2)
#define MCF_GPIO_PDDR_C_PDDR_C2              (0x4)
#define MCF_GPIO_PDDR_C_PDDR_C3              (0x8)
#define MCF_GPIO_PDDR_C_PDDR_C4              (0x10)
#define MCF_GPIO_PDDR_C_PDDR_C5              (0x20)
#define MCF_GPIO_PDDR_C_PDDR_C6              (0x40)
#define MCF_GPIO_PDDR_C_PDDR_C7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_C */
#define MCF_GPIO_PPDSDR_C_PPDSDR_C0          (0x1)
#define MCF_GPIO_PPDSDR_C_PPDSDR_C1          (0x2)
#define MCF_GPIO_PPDSDR_C_PPDSDR_C2          (0x4)
#define MCF_GPIO_PPDSDR_C_PPDSDR_C3          (0x8)
#define MCF_GPIO_PPDSDR_C_PPDSDR_C4          (0x10)
#define MCF_GPIO_PPDSDR_C_PPDSDR_C5          (0x20)
#define MCF_GPIO_PPDSDR_C_PPDSDR_C6          (0x40)
#define MCF_GPIO_PPDSDR_C_PPDSDR_C7          (0x80)

/* Bit definitions and macros for MCF_GPIO_PCLRR_C */
#define MCF_GPIO_PCLRR_C_PCLRR_C0            (0x1)
#define MCF_GPIO_PCLRR_C_PCLRR_C1            (0x2)
#define MCF_GPIO_PCLRR_C_PCLRR_C2            (0x4)
#define MCF_GPIO_PCLRR_C_PCLRR_C3            (0x8)
#define MCF_GPIO_PCLRR_C_PCLRR_C4            (0x10)
#define MCF_GPIO_PCLRR_C_PCLRR_C5            (0x20)
#define MCF_GPIO_PCLRR_C_PCLRR_C6            (0x40)
#define MCF_GPIO_PCLRR_C_PCLRR_C7            (0x80)

/* Bit definitions and macros for MCF_GPIO_PCR_C */
#define MCF_GPIO_PCR_C_PUS0                  (0x1)
#define MCF_GPIO_PCR_C_PUE0                  (0x2)
#define MCF_GPIO_PCR_C_PUS1                  (0x4)
#define MCF_GPIO_PCR_C_PUE1                  (0x8)
#define MCF_GPIO_PCR_C_PUS2                  (0x10)
#define MCF_GPIO_PCR_C_PUE2                  (0x20)
#define MCF_GPIO_PCR_C_PUS3                  (0x40)
#define MCF_GPIO_PCR_C_PUE3                  (0x80)
#define MCF_GPIO_PCR_C_PUS4                  (0x100)
#define MCF_GPIO_PCR_C_PUE4                  (0x200)
#define MCF_GPIO_PCR_C_PUS5                  (0x400)
#define MCF_GPIO_PCR_C_PUE5                  (0x800)
#define MCF_GPIO_PCR_C_PUS6                  (0x1000)
#define MCF_GPIO_PCR_C_PUE6                  (0x2000)
#define MCF_GPIO_PCR_C_PUS7                  (0x4000)
#define MCF_GPIO_PCR_C_PUE7                  (0x8000)

/* Bit definitions and macros for MCF_GPIO_PODR_D */
#define MCF_GPIO_PODR_D_PODR_D0              (0x1)
#define MCF_GPIO_PODR_D_PODR_D1              (0x2)
#define MCF_GPIO_PODR_D_PODR_D2              (0x4)
#define MCF_GPIO_PODR_D_PODR_D3              (0x8)
#define MCF_GPIO_PODR_D_PODR_D4              (0x10)
#define MCF_GPIO_PODR_D_PODR_D5              (0x20)
#define MCF_GPIO_PODR_D_PODR_D6              (0x40)
#define MCF_GPIO_PODR_D_PODR_D7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PDDR_D */
#define MCF_GPIO_PDDR_D_PDDR_D0              (0x1)
#define MCF_GPIO_PDDR_D_PDDR_D1              (0x2)
#define MCF_GPIO_PDDR_D_PDDR_D2              (0x4)
#define MCF_GPIO_PDDR_D_PDDR_D3              (0x8)
#define MCF_GPIO_PDDR_D_PDDR_D4              (0x10)
#define MCF_GPIO_PDDR_D_PDDR_D5              (0x20)
#define MCF_GPIO_PDDR_D_PDDR_D6              (0x40)
#define MCF_GPIO_PDDR_D_PDDR_D7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_D */
#define MCF_GPIO_PPDSDR_D_PPDSDR_D0          (0x1)
#define MCF_GPIO_PPDSDR_D_PPDSDR_D1          (0x2)
#define MCF_GPIO_PPDSDR_D_PPDSDR_D2          (0x4)
#define MCF_GPIO_PPDSDR_D_PPDSDR_D3          (0x8)
#define MCF_GPIO_PPDSDR_D_PPDSDR_D4          (0x10)
#define MCF_GPIO_PPDSDR_D_PPDSDR_D5          (0x20)
#define MCF_GPIO_PPDSDR_D_PPDSDR_D6          (0x40)
#define MCF_GPIO_PPDSDR_D_PPDSDR_D7          (0x80)

/* Bit definitions and macros for MCF_GPIO_PCLRR_D */
#define MCF_GPIO_PCLRR_D_PCLRR_D0            (0x1)
#define MCF_GPIO_PCLRR_D_PCLRR_D1            (0x2)
#define MCF_GPIO_PCLRR_D_PCLRR_D2            (0x4)
#define MCF_GPIO_PCLRR_D_PCLRR_D3            (0x8)
#define MCF_GPIO_PCLRR_D_PCLRR_D4            (0x10)
#define MCF_GPIO_PCLRR_D_PCLRR_D5            (0x20)
#define MCF_GPIO_PCLRR_D_PCLRR_D6            (0x40)
#define MCF_GPIO_PCLRR_D_PCLRR_D7            (0x80)

/* Bit definitions and macros for MCF_GPIO_PCR_D */
#define MCF_GPIO_PCR_D_PUS0                  (0x1)
#define MCF_GPIO_PCR_D_PUE0                  (0x2)
#define MCF_GPIO_PCR_D_PUS1                  (0x4)
#define MCF_GPIO_PCR_D_PUE1                  (0x8)
#define MCF_GPIO_PCR_D_PUS2                  (0x10)
#define MCF_GPIO_PCR_D_PUE2                  (0x20)
#define MCF_GPIO_PCR_D_PUS3                  (0x40)
#define MCF_GPIO_PCR_D_PUE3                  (0x80)
#define MCF_GPIO_PCR_D_PUS4                  (0x100)
#define MCF_GPIO_PCR_D_PUE4                  (0x200)
#define MCF_GPIO_PCR_D_PUS5                  (0x400)
#define MCF_GPIO_PCR_D_PUE5                  (0x800)
#define MCF_GPIO_PCR_D_PUS6                  (0x1000)
#define MCF_GPIO_PCR_D_PUE6                  (0x2000)
#define MCF_GPIO_PCR_D_PUS7                  (0x4000)
#define MCF_GPIO_PCR_D_PUE7                  (0x8000)

/* Bit definitions and macros for MCF_GPIO_PODR_E */
#define MCF_GPIO_PODR_E_PODR_E0              (0x1)
#define MCF_GPIO_PODR_E_PODR_E1              (0x2)
#define MCF_GPIO_PODR_E_PODR_E2              (0x4)
#define MCF_GPIO_PODR_E_PODR_E3              (0x8)
#define MCF_GPIO_PODR_E_PODR_E4              (0x10)
#define MCF_GPIO_PODR_E_PODR_E5              (0x20)
#define MCF_GPIO_PODR_E_PODR_E6              (0x40)
#define MCF_GPIO_PODR_E_PODR_E7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PDDR_E */
#define MCF_GPIO_PDDR_E_PDDR_E0              (0x1)
#define MCF_GPIO_PDDR_E_PDDR_E1              (0x2)
#define MCF_GPIO_PDDR_E_PDDR_E2              (0x4)
#define MCF_GPIO_PDDR_E_PDDR_E3              (0x8)
#define MCF_GPIO_PDDR_E_PDDR_E4              (0x10)
#define MCF_GPIO_PDDR_E_PDDR_E5              (0x20)
#define MCF_GPIO_PDDR_E_PDDR_E6              (0x40)
#define MCF_GPIO_PDDR_E_PDDR_E7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_E */
#define MCF_GPIO_PPDSDR_E_PPDSDR_E0          (0x1)
#define MCF_GPIO_PPDSDR_E_PPDSDR_E1          (0x2)
#define MCF_GPIO_PPDSDR_E_PPDSDR_E2          (0x4)
#define MCF_GPIO_PPDSDR_E_PPDSDR_E3          (0x8)
#define MCF_GPIO_PPDSDR_E_PPDSDR_E4          (0x10)
#define MCF_GPIO_PPDSDR_E_PPDSDR_E5          (0x20)
#define MCF_GPIO_PPDSDR_E_PPDSDR_E6          (0x40)
#define MCF_GPIO_PPDSDR_E_PPDSDR_E7          (0x80)

/* Bit definitions and macros for MCF_GPIO_PCLRR_E */
#define MCF_GPIO_PCLRR_E_PCLRR_E0            (0x1)
#define MCF_GPIO_PCLRR_E_PCLRR_E1            (0x2)
#define MCF_GPIO_PCLRR_E_PCLRR_E2            (0x4)
#define MCF_GPIO_PCLRR_E_PCLRR_E3            (0x8)
#define MCF_GPIO_PCLRR_E_PCLRR_E4            (0x10)
#define MCF_GPIO_PCLRR_E_PCLRR_E5            (0x20)
#define MCF_GPIO_PCLRR_E_PCLRR_E6            (0x40)
#define MCF_GPIO_PCLRR_E_PCLRR_E7            (0x80)

/* Bit definitions and macros for MCF_GPIO_PCR_E */
#define MCF_GPIO_PCR_E_PUS0                  (0x1)
#define MCF_GPIO_PCR_E_PUE0                  (0x2)
#define MCF_GPIO_PCR_E_PUS1                  (0x4)
#define MCF_GPIO_PCR_E_PUE1                  (0x8)
#define MCF_GPIO_PCR_E_PUS2                  (0x10)
#define MCF_GPIO_PCR_E_PUE2                  (0x20)
#define MCF_GPIO_PCR_E_PUS3                  (0x40)
#define MCF_GPIO_PCR_E_PUE3                  (0x80)
#define MCF_GPIO_PCR_E_PUS4                  (0x100)
#define MCF_GPIO_PCR_E_PUE4                  (0x200)
#define MCF_GPIO_PCR_E_PUS5                  (0x400)
#define MCF_GPIO_PCR_E_PUE5                  (0x800)
#define MCF_GPIO_PCR_E_PUS6                  (0x1000)
#define MCF_GPIO_PCR_E_PUE6                  (0x2000)
#define MCF_GPIO_PCR_E_PUS7                  (0x4000)
#define MCF_GPIO_PCR_E_PUE7                  (0x8000)

/* Bit definitions and macros for MCF_GPIO_PODR_F */
#define MCF_GPIO_PODR_F_PODR_F0              (0x1)
#define MCF_GPIO_PODR_F_PODR_F1              (0x2)
#define MCF_GPIO_PODR_F_PODR_F2              (0x4)
#define MCF_GPIO_PODR_F_PODR_F3              (0x8)
#define MCF_GPIO_PODR_F_PODR_F4              (0x10)
#define MCF_GPIO_PODR_F_PODR_F5              (0x20)
#define MCF_GPIO_PODR_F_PODR_F6              (0x40)
#define MCF_GPIO_PODR_F_PODR_F7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PDDR_F */
#define MCF_GPIO_PDDR_F_PDDR_F0              (0x1)
#define MCF_GPIO_PDDR_F_PDDR_F1              (0x2)
#define MCF_GPIO_PDDR_F_PDDR_F2              (0x4)
#define MCF_GPIO_PDDR_F_PDDR_F3              (0x8)
#define MCF_GPIO_PDDR_F_PDDR_F4              (0x10)
#define MCF_GPIO_PDDR_F_PDDR_F5              (0x20)
#define MCF_GPIO_PDDR_F_PDDR_F6              (0x40)
#define MCF_GPIO_PDDR_F_PDDR_F7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_F */
#define MCF_GPIO_PPDSDR_F_PPDSDR_F0          (0x1)
#define MCF_GPIO_PPDSDR_F_PPDSDR_F1          (0x2)
#define MCF_GPIO_PPDSDR_F_PPDSDR_F2          (0x4)
#define MCF_GPIO_PPDSDR_F_PPDSDR_F3          (0x8)
#define MCF_GPIO_PPDSDR_F_PPDSDR_F4          (0x10)
#define MCF_GPIO_PPDSDR_F_PPDSDR_F5          (0x20)
#define MCF_GPIO_PPDSDR_F_PPDSDR_F6          (0x40)
#define MCF_GPIO_PPDSDR_F_PPDSDR_F7          (0x80)

/* Bit definitions and macros for MCF_GPIO_PCLRR_F */
#define MCF_GPIO_PCLRR_F_PCLRR_F0            (0x1)
#define MCF_GPIO_PCLRR_F_PCLRR_F1            (0x2)
#define MCF_GPIO_PCLRR_F_PCLRR_F2            (0x4)
#define MCF_GPIO_PCLRR_F_PCLRR_F3            (0x8)
#define MCF_GPIO_PCLRR_F_PCLRR_F4            (0x10)
#define MCF_GPIO_PCLRR_F_PCLRR_F5            (0x20)
#define MCF_GPIO_PCLRR_F_PCLRR_F6            (0x40)
#define MCF_GPIO_PCLRR_F_PCLRR_F7            (0x80)

/* Bit definitions and macros for MCF_GPIO_PCR_F */
#define MCF_GPIO_PCR_F_PUS0                  (0x1)
#define MCF_GPIO_PCR_F_PUE0                  (0x2)
#define MCF_GPIO_PCR_F_PUS1                  (0x4)
#define MCF_GPIO_PCR_F_PUE1                  (0x8)
#define MCF_GPIO_PCR_F_PUS2                  (0x10)
#define MCF_GPIO_PCR_F_PUE2                  (0x20)
#define MCF_GPIO_PCR_F_PUS3                  (0x40)
#define MCF_GPIO_PCR_F_PUE3                  (0x80)
#define MCF_GPIO_PCR_F_PUS4                  (0x100)
#define MCF_GPIO_PCR_F_PUE4                  (0x200)
#define MCF_GPIO_PCR_F_PUS5                  (0x400)
#define MCF_GPIO_PCR_F_PUE5                  (0x800)
#define MCF_GPIO_PCR_F_PUS6                  (0x1000)
#define MCF_GPIO_PCR_F_PUE6                  (0x2000)
#define MCF_GPIO_PCR_F_PUS7                  (0x4000)
#define MCF_GPIO_PCR_F_PUE7                  (0x8000)

/* Bit definitions and macros for MCF_GPIO_PODR_G */
#define MCF_GPIO_PODR_G_PODR_G0              (0x1)
#define MCF_GPIO_PODR_G_PODR_G1              (0x2)
#define MCF_GPIO_PODR_G_PODR_G2              (0x4)
#define MCF_GPIO_PODR_G_PODR_G3              (0x8)
#define MCF_GPIO_PODR_G_PODR_G4              (0x10)
#define MCF_GPIO_PODR_G_PODR_G5              (0x20)
#define MCF_GPIO_PODR_G_PODR_G6              (0x40)
#define MCF_GPIO_PODR_G_PODR_G7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PDDR_G */
#define MCF_GPIO_PDDR_G_PDDR_G0              (0x1)
#define MCF_GPIO_PDDR_G_PDDR_G1              (0x2)
#define MCF_GPIO_PDDR_G_PDDR_G2              (0x4)
#define MCF_GPIO_PDDR_G_PDDR_G3              (0x8)
#define MCF_GPIO_PDDR_G_PDDR_G4              (0x10)
#define MCF_GPIO_PDDR_G_PDDR_G5              (0x20)
#define MCF_GPIO_PDDR_G_PDDR_G6              (0x40)
#define MCF_GPIO_PDDR_G_PDDR_G7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_G */
#define MCF_GPIO_PPDSDR_G_PPDSDR_G0          (0x1)
#define MCF_GPIO_PPDSDR_G_PPDSDR_G1          (0x2)
#define MCF_GPIO_PPDSDR_G_PPDSDR_G2          (0x4)
#define MCF_GPIO_PPDSDR_G_PPDSDR_G3          (0x8)
#define MCF_GPIO_PPDSDR_G_PPDSDR_G4          (0x10)
#define MCF_GPIO_PPDSDR_G_PPDSDR_G5          (0x20)
#define MCF_GPIO_PPDSDR_G_PPDSDR_G6          (0x40)
#define MCF_GPIO_PPDSDR_G_PPDSDR_G7          (0x80)

/* Bit definitions and macros for MCF_GPIO_PCLRR_G */
#define MCF_GPIO_PCLRR_G_PCLRR_G0            (0x1)
#define MCF_GPIO_PCLRR_G_PCLRR_G1            (0x2)
#define MCF_GPIO_PCLRR_G_PCLRR_G2            (0x4)
#define MCF_GPIO_PCLRR_G_PCLRR_G3            (0x8)
#define MCF_GPIO_PCLRR_G_PCLRR_G4            (0x10)
#define MCF_GPIO_PCLRR_G_PCLRR_G5            (0x20)
#define MCF_GPIO_PCLRR_G_PCLRR_G6            (0x40)
#define MCF_GPIO_PCLRR_G_PCLRR_G7            (0x80)

/* Bit definitions and macros for MCF_GPIO_PCR_G */
#define MCF_GPIO_PCR_G_PUS0                  (0x1)
#define MCF_GPIO_PCR_G_PUE0                  (0x2)
#define MCF_GPIO_PCR_G_PUS1                  (0x4)
#define MCF_GPIO_PCR_G_PUE1                  (0x8)
#define MCF_GPIO_PCR_G_PUS2                  (0x10)
#define MCF_GPIO_PCR_G_PUE2                  (0x20)
#define MCF_GPIO_PCR_G_PUS3                  (0x40)
#define MCF_GPIO_PCR_G_PUE3                  (0x80)
#define MCF_GPIO_PCR_G_PUS4                  (0x100)
#define MCF_GPIO_PCR_G_PUE4                  (0x200)
#define MCF_GPIO_PCR_G_PUS5                  (0x400)
#define MCF_GPIO_PCR_G_PUE5                  (0x800)
#define MCF_GPIO_PCR_G_PUS6                  (0x1000)
#define MCF_GPIO_PCR_G_PUE6                  (0x2000)
#define MCF_GPIO_PCR_G_PUS7                  (0x4000)
#define MCF_GPIO_PCR_G_PUE7                  (0x8000)

/* Bit definitions and macros for MCF_GPIO_PODR_H */
#define MCF_GPIO_PODR_H_PODR_H0              (0x1)
#define MCF_GPIO_PODR_H_PODR_H1              (0x2)
#define MCF_GPIO_PODR_H_PODR_H2              (0x4)
#define MCF_GPIO_PODR_H_PODR_H3              (0x8)
#define MCF_GPIO_PODR_H_PODR_H4              (0x10)
#define MCF_GPIO_PODR_H_PODR_H5              (0x20)
#define MCF_GPIO_PODR_H_PODR_H6              (0x40)
#define MCF_GPIO_PODR_H_PODR_H7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PDDR_H */
#define MCF_GPIO_PDDR_H_PDDR_H0              (0x1)
#define MCF_GPIO_PDDR_H_PDDR_H1              (0x2)
#define MCF_GPIO_PDDR_H_PDDR_H2              (0x4)
#define MCF_GPIO_PDDR_H_PDDR_H3              (0x8)
#define MCF_GPIO_PDDR_H_PDDR_H4              (0x10)
#define MCF_GPIO_PDDR_H_PDDR_H5              (0x20)
#define MCF_GPIO_PDDR_H_PDDR_H6              (0x40)
#define MCF_GPIO_PDDR_H_PDDR_H7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_H */
#define MCF_GPIO_PPDSDR_H_PPDSDR_H0          (0x1)
#define MCF_GPIO_PPDSDR_H_PPDSDR_H1          (0x2)
#define MCF_GPIO_PPDSDR_H_PPDSDR_H2          (0x4)
#define MCF_GPIO_PPDSDR_H_PPDSDR_H3          (0x8)
#define MCF_GPIO_PPDSDR_H_PPDSDR_H4          (0x10)
#define MCF_GPIO_PPDSDR_H_PPDSDR_H5          (0x20)
#define MCF_GPIO_PPDSDR_H_PPDSDR_H6          (0x40)
#define MCF_GPIO_PPDSDR_H_PPDSDR_H7          (0x80)

/* Bit definitions and macros for MCF_GPIO_PCLRR_H */
#define MCF_GPIO_PCLRR_H_PCLRR_H0            (0x1)
#define MCF_GPIO_PCLRR_H_PCLRR_H1            (0x2)
#define MCF_GPIO_PCLRR_H_PCLRR_H2            (0x4)
#define MCF_GPIO_PCLRR_H_PCLRR_H3            (0x8)
#define MCF_GPIO_PCLRR_H_PCLRR_H4            (0x10)
#define MCF_GPIO_PCLRR_H_PCLRR_H5            (0x20)
#define MCF_GPIO_PCLRR_H_PCLRR_H6            (0x40)
#define MCF_GPIO_PCLRR_H_PCLRR_H7            (0x80)

/* Bit definitions and macros for MCF_GPIO_PCR_H */
#define MCF_GPIO_PCR_H_PUS0                  (0x1)
#define MCF_GPIO_PCR_H_PUE0                  (0x2)
#define MCF_GPIO_PCR_H_PUS1                  (0x4)
#define MCF_GPIO_PCR_H_PUE1                  (0x8)
#define MCF_GPIO_PCR_H_PUS2                  (0x10)
#define MCF_GPIO_PCR_H_PUE2                  (0x20)
#define MCF_GPIO_PCR_H_PUS3                  (0x40)
#define MCF_GPIO_PCR_H_PUE3                  (0x80)
#define MCF_GPIO_PCR_H_PUS4                  (0x100)
#define MCF_GPIO_PCR_H_PUE4                  (0x200)
#define MCF_GPIO_PCR_H_PUS5                  (0x400)
#define MCF_GPIO_PCR_H_PUE5                  (0x800)
#define MCF_GPIO_PCR_H_PUS6                  (0x1000)
#define MCF_GPIO_PCR_H_PUE6                  (0x2000)
#define MCF_GPIO_PCR_H_PUS7                  (0x4000)
#define MCF_GPIO_PCR_H_PUE7                  (0x8000)

/* Bit definitions and macros for MCF_GPIO_PODR_I */
#define MCF_GPIO_PODR_I_PODR_I0              (0x1)
#define MCF_GPIO_PODR_I_PODR_I1              (0x2)
#define MCF_GPIO_PODR_I_PODR_I2              (0x4)
#define MCF_GPIO_PODR_I_PODR_I3              (0x8)
#define MCF_GPIO_PODR_I_PODR_I4              (0x10)
#define MCF_GPIO_PODR_I_PODR_I5              (0x20)
#define MCF_GPIO_PODR_I_PODR_I6              (0x40)
#define MCF_GPIO_PODR_I_PODR_I7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PDDR_I */
#define MCF_GPIO_PDDR_I_PDDR_I0              (0x1)
#define MCF_GPIO_PDDR_I_PDDR_I1              (0x2)
#define MCF_GPIO_PDDR_I_PDDR_I2              (0x4)
#define MCF_GPIO_PDDR_I_PDDR_I3              (0x8)
#define MCF_GPIO_PDDR_I_PDDR_I4              (0x10)
#define MCF_GPIO_PDDR_I_PDDR_I5              (0x20)
#define MCF_GPIO_PDDR_I_PDDR_I6              (0x40)
#define MCF_GPIO_PDDR_I_PDDR_I7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_I */
#define MCF_GPIO_PPDSDR_I_PPDSDR_I0          (0x1)
#define MCF_GPIO_PPDSDR_I_PPDSDR_I1          (0x2)
#define MCF_GPIO_PPDSDR_I_PPDSDR_I2          (0x4)
#define MCF_GPIO_PPDSDR_I_PPDSDR_I3          (0x8)
#define MCF_GPIO_PPDSDR_I_PPDSDR_I4          (0x10)
#define MCF_GPIO_PPDSDR_I_PPDSDR_I5          (0x20)
#define MCF_GPIO_PPDSDR_I_PPDSDR_I6          (0x40)
#define MCF_GPIO_PPDSDR_I_PPDSDR_I7          (0x80)

/* Bit definitions and macros for MCF_GPIO_PCLRR_I */
#define MCF_GPIO_PCLRR_I_PCLRR_I0            (0x1)
#define MCF_GPIO_PCLRR_I_PCLRR_I1            (0x2)
#define MCF_GPIO_PCLRR_I_PCLRR_I2            (0x4)
#define MCF_GPIO_PCLRR_I_PCLRR_I3            (0x8)
#define MCF_GPIO_PCLRR_I_PCLRR_I4            (0x10)
#define MCF_GPIO_PCLRR_I_PCLRR_I5            (0x20)
#define MCF_GPIO_PCLRR_I_PCLRR_I6            (0x40)
#define MCF_GPIO_PCLRR_I_PCLRR_I7            (0x80)

/* Bit definitions and macros for MCF_GPIO_PCR_I */
#define MCF_GPIO_PCR_I_PUS0                  (0x1)
#define MCF_GPIO_PCR_I_PUE0                  (0x2)
#define MCF_GPIO_PCR_I_PUS1                  (0x4)
#define MCF_GPIO_PCR_I_PUE1                  (0x8)
#define MCF_GPIO_PCR_I_PUS2                  (0x10)
#define MCF_GPIO_PCR_I_PUE2                  (0x20)
#define MCF_GPIO_PCR_I_PUS3                  (0x40)
#define MCF_GPIO_PCR_I_PUE3                  (0x80)
#define MCF_GPIO_PCR_I_PUS4                  (0x100)
#define MCF_GPIO_PCR_I_PUE4                  (0x200)
#define MCF_GPIO_PCR_I_PUS5                  (0x400)
#define MCF_GPIO_PCR_I_PUE5                  (0x800)
#define MCF_GPIO_PCR_I_PUS6                  (0x1000)
#define MCF_GPIO_PCR_I_PUE6                  (0x2000)
#define MCF_GPIO_PCR_I_PUS7                  (0x4000)
#define MCF_GPIO_PCR_I_PUE7                  (0x8000)

/* Bit definitions and macros for MCF_GPIO_PODR_J */
#define MCF_GPIO_PODR_J_PODR_J0              (0x1)
#define MCF_GPIO_PODR_J_PODR_J1              (0x2)
#define MCF_GPIO_PODR_J_PODR_J2              (0x4)
#define MCF_GPIO_PODR_J_PODR_J3              (0x8)
#define MCF_GPIO_PODR_J_PODR_J4              (0x10)
#define MCF_GPIO_PODR_J_PODR_J5              (0x20)
#define MCF_GPIO_PODR_J_PODR_J6              (0x40)
#define MCF_GPIO_PODR_J_PODR_J7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PDDR_J */
#define MCF_GPIO_PDDR_J_PDDR_J0              (0x1)
#define MCF_GPIO_PDDR_J_PDDR_J1              (0x2)
#define MCF_GPIO_PDDR_J_PDDR_J2              (0x4)
#define MCF_GPIO_PDDR_J_PDDR_J3              (0x8)
#define MCF_GPIO_PDDR_J_PDDR_J4              (0x10)
#define MCF_GPIO_PDDR_J_PDDR_J5              (0x20)
#define MCF_GPIO_PDDR_J_PDDR_J6              (0x40)
#define MCF_GPIO_PDDR_J_PDDR_J7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_J */
#define MCF_GPIO_PPDSDR_J_PPDSDR_J0          (0x1)
#define MCF_GPIO_PPDSDR_J_PPDSDR_J1          (0x2)
#define MCF_GPIO_PPDSDR_J_PPDSDR_J2          (0x4)
#define MCF_GPIO_PPDSDR_J_PPDSDR_J3          (0x8)
#define MCF_GPIO_PPDSDR_J_PPDSDR_J4          (0x10)
#define MCF_GPIO_PPDSDR_J_PPDSDR_J5          (0x20)
#define MCF_GPIO_PPDSDR_J_PPDSDR_J6          (0x40)
#define MCF_GPIO_PPDSDR_J_PPDSDR_J7          (0x80)

/* Bit definitions and macros for MCF_GPIO_PCLRR_J */
#define MCF_GPIO_PCLRR_J_PCLRR_J0            (0x1)
#define MCF_GPIO_PCLRR_J_PCLRR_J1            (0x2)
#define MCF_GPIO_PCLRR_J_PCLRR_J2            (0x4)
#define MCF_GPIO_PCLRR_J_PCLRR_J3            (0x8)
#define MCF_GPIO_PCLRR_J_PCLRR_J4            (0x10)
#define MCF_GPIO_PCLRR_J_PCLRR_J5            (0x20)
#define MCF_GPIO_PCLRR_J_PCLRR_J6            (0x40)
#define MCF_GPIO_PCLRR_J_PCLRR_J7            (0x80)

/* Bit definitions and macros for MCF_GPIO_PCR_J */
#define MCF_GPIO_PCR_J_PUS0                  (0x1)
#define MCF_GPIO_PCR_J_PUE0                  (0x2)
#define MCF_GPIO_PCR_J_PUS1                  (0x4)
#define MCF_GPIO_PCR_J_PUE1                  (0x8)
#define MCF_GPIO_PCR_J_PUS2                  (0x10)
#define MCF_GPIO_PCR_J_PUE2                  (0x20)
#define MCF_GPIO_PCR_J_PUS3                  (0x40)
#define MCF_GPIO_PCR_J_PUE3                  (0x80)
#define MCF_GPIO_PCR_J_PUS4                  (0x100)
#define MCF_GPIO_PCR_J_PUE4                  (0x200)
#define MCF_GPIO_PCR_J_PUS5                  (0x400)
#define MCF_GPIO_PCR_J_PUE5                  (0x800)
#define MCF_GPIO_PCR_J_PUS6                  (0x1000)
#define MCF_GPIO_PCR_J_PUE6                  (0x2000)
#define MCF_GPIO_PCR_J_PUS7                  (0x4000)
#define MCF_GPIO_PCR_J_PUE7                  (0x8000)

/* Bit definitions and macros for MCF_GPIO_PODR_K */
#define MCF_GPIO_PODR_K_PODR_K0              (0x1)
#define MCF_GPIO_PODR_K_PODR_K1              (0x2)
#define MCF_GPIO_PODR_K_PODR_K2              (0x4)
#define MCF_GPIO_PODR_K_PODR_K3              (0x8)
#define MCF_GPIO_PODR_K_PODR_K4              (0x10)
#define MCF_GPIO_PODR_K_PODR_K5              (0x20)
#define MCF_GPIO_PODR_K_PODR_K6              (0x40)
#define MCF_GPIO_PODR_K_PODR_K7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PDDR_K */
#define MCF_GPIO_PDDR_K_PDDR_K0              (0x1)
#define MCF_GPIO_PDDR_K_PDDR_K1              (0x2)
#define MCF_GPIO_PDDR_K_PDDR_K2              (0x4)
#define MCF_GPIO_PDDR_K_PDDR_K3              (0x8)
#define MCF_GPIO_PDDR_K_PDDR_K4              (0x10)
#define MCF_GPIO_PDDR_K_PDDR_K5              (0x20)
#define MCF_GPIO_PDDR_K_PDDR_K6              (0x40)
#define MCF_GPIO_PDDR_K_PDDR_K7              (0x80)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_K */
#define MCF_GPIO_PPDSDR_K_PPDSDR_K0          (0x1)
#define MCF_GPIO_PPDSDR_K_PPDSDR_K1          (0x2)
#define MCF_GPIO_PPDSDR_K_PPDSDR_K2          (0x4)
#define MCF_GPIO_PPDSDR_K_PPDSDR_K3          (0x8)
#define MCF_GPIO_PPDSDR_K_PPDSDR_K4          (0x10)
#define MCF_GPIO_PPDSDR_K_PPDSDR_K5          (0x20)
#define MCF_GPIO_PPDSDR_K_PPDSDR_K6          (0x40)
#define MCF_GPIO_PPDSDR_K_PPDSDR_K7          (0x80)

/* Bit definitions and macros for MCF_GPIO_PCLRR_K */
#define MCF_GPIO_PCLRR_K_PCLRR_K0            (0x1)
#define MCF_GPIO_PCLRR_K_PCLRR_K1            (0x2)
#define MCF_GPIO_PCLRR_K_PCLRR_K2            (0x4)
#define MCF_GPIO_PCLRR_K_PCLRR_K3            (0x8)
#define MCF_GPIO_PCLRR_K_PCLRR_K4            (0x10)
#define MCF_GPIO_PCLRR_K_PCLRR_K5            (0x20)
#define MCF_GPIO_PCLRR_K_PCLRR_K6            (0x40)
#define MCF_GPIO_PCLRR_K_PCLRR_K7            (0x80)

/* Bit definitions and macros for MCF_GPIO_PCR_K */
#define MCF_GPIO_PCR_K_PUS0                  (0x1)
#define MCF_GPIO_PCR_K_PUE0                  (0x2)
#define MCF_GPIO_PCR_K_PUS1                  (0x4)
#define MCF_GPIO_PCR_K_PUE1                  (0x8)
#define MCF_GPIO_PCR_K_PUS2                  (0x10)
#define MCF_GPIO_PCR_K_PUE2                  (0x20)
#define MCF_GPIO_PCR_K_PUS3                  (0x40)
#define MCF_GPIO_PCR_K_PUE3                  (0x80)
#define MCF_GPIO_PCR_K_PUS4                  (0x100)
#define MCF_GPIO_PCR_K_PUE4                  (0x200)
#define MCF_GPIO_PCR_K_PUS5                  (0x400)
#define MCF_GPIO_PCR_K_PUE5                  (0x800)
#define MCF_GPIO_PCR_K_PUS6                  (0x1000)
#define MCF_GPIO_PCR_K_PUE6                  (0x2000)
#define MCF_GPIO_PCR_K_PUS7                  (0x4000)
#define MCF_GPIO_PCR_K_PUE7                  (0x8000)


#endif /* __MCF54418_GPIO_H__ */
