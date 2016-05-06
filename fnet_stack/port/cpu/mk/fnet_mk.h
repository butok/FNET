/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
*
***************************************************************************
*
*  Licensed under the Apache License, Version 2.0 (the "License"); you may
*  not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
*  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
**********************************************************************/
/*!
*
* @file fnet_mk.h
*
* @author Andrey Butok
*
* @brief Private. Kinetis Peripheral Registers definitions.
*
***************************************************************************/

#ifndef _FNET_MK_H_

#define _FNET_MK_H_



#include "fnet.h"

#if FNET_MK

/*********************************************************************
*
* The basic data types.
*
*********************************************************************/
typedef unsigned char fnet_uint8_t;       /*  8 bits */

typedef unsigned short int fnet_uint16_t; /* 16 bits */
typedef unsigned long int fnet_uint32_t;  /* 32 bits */

typedef signed char fnet_int8_t;          /*  8 bits */
typedef signed short int fnet_int16_t;    /* 16 bits */
typedef signed long int fnet_int32_t;     /* 32 bits */

typedef volatile fnet_uint8_t fnet_vuint8_t;     /*  8 bits */
typedef volatile fnet_uint16_t fnet_vuint16_t;   /* 16 bits */
typedef volatile fnet_uint32_t fnet_vuint32_t;   /* 32 bits */


/* Ensure that the Thumb bit is set.*/
#define FNET_CPU_ADDR_TO_INSTRUCTION(addr)    ((fnet_uint32_t)(addr)|0x1u)
#define FNET_CPU_INSTRUCTION_TO_ADDR(addr)    ((fnet_uint32_t)(addr)&(~0x1u))

/************************************************************************
* Kinetis peripheral clock in KHZ.
*************************************************************************/
fnet_uint32_t fnet_mk_periph_clk_khz(void);

#define FNET_MK_PERIPH_CLOCK_KHZ       fnet_mk_periph_clk_khz()
#define FNET_MK_PERIPH_CLOCK_MHZ       (fnet_mk_periph_clk_khz()/1000)


/* ----------------------------------------------------------------------------
   -- UART
   ---------------------------------------------------------------------------- */

/* UART - Peripheral register structure */
typedef struct FNET_MK_UART_MemMap
{
    fnet_uint8_t BDH;                                     /* UART Baud Rate Registers:High, offset: 0x0 */
    fnet_uint8_t BDL;                                     /* UART Baud Rate Registers: Low, offset: 0x1 */
    fnet_uint8_t C1;                                      /* UART Control Register 1, offset: 0x2 */
    fnet_uint8_t C2;                                      /* UART Control Register 2, offset: 0x3 */
    fnet_uint8_t S1;                                      /* UART Status Register 1, offset: 0x4 */
    fnet_uint8_t S2;                                      /* UART Status Register 2, offset: 0x5 */
    fnet_uint8_t C3;                                      /* UART Control Register 3, offset: 0x6 */
    fnet_uint8_t D;                                       /* UART Data Register, offset: 0x7 */
    fnet_uint8_t MA1;                                     /* UART Match Address Registers 1, offset: 0x8 */
    fnet_uint8_t MA2;                                     /* UART Match Address Registers 2, offset: 0x9 */
    fnet_uint8_t C4;                                      /* UART Control Register 4, offset: 0xA */
    fnet_uint8_t C5;                                      /* UART Control Register 5, offset: 0xB */
    fnet_uint8_t ED;                                      /* UART Extended Data Register, offset: 0xC */
    fnet_uint8_t MODEM;                                   /* UART Modem Register, offset: 0xD */
    fnet_uint8_t IR;                                      /* UART Infrared Register, offset: 0xE */
    fnet_uint8_t RESERVED_0[1];
    fnet_uint8_t PFIFO;                                   /* UART FIFO Parameters, offset: 0x10 */
    fnet_uint8_t CFIFO;                                   /* UART FIFO Control Register, offset: 0x11 */
    fnet_uint8_t SFIFO;                                   /* UART FIFO Status Register, offset: 0x12 */
    fnet_uint8_t TWFIFO;                                  /* UART FIFO Transmit Watermark, offset: 0x13 */
    fnet_uint8_t TCFIFO;                                  /* UART FIFO Transmit Count, offset: 0x14 */
    fnet_uint8_t RWFIFO;                                  /* UART FIFO Receive Watermark, offset: 0x15 */
    fnet_uint8_t RCFIFO;                                  /* UART FIFO Receive Count, offset: 0x16 */
    fnet_uint8_t RESERVED_1[1];
    fnet_uint8_t C7816;                                   /* UART 7816 Control Register, offset: 0x18 */
    fnet_uint8_t IE7816;                                  /* UART 7816 Interrupt Enable Register, offset: 0x19 */
    fnet_uint8_t IS7816;                                  /* UART 7816 Interrupt Status Register, offset: 0x1A */
    union                                               /* offset: 0x1B */
    {
        fnet_uint8_t WP7816_T_TYPE0;                          /* UART 7816 Wait Parameter Register, offset: 0x1B */
        fnet_uint8_t WP7816_T_TYPE1;                          /* UART 7816 Wait Parameter Register, offset: 0x1B */
    } WP7816;
    fnet_uint8_t WN7816;                                  /* UART 7816 Wait N Register, offset: 0x1C */
    fnet_uint8_t WF7816;                                  /* UART 7816 Wait FD Register, offset: 0x1D */
    fnet_uint8_t ET7816;                                  /* UART 7816 Error Threshold Register, offset: 0x1E */
    fnet_uint8_t TL7816;                                  /* UART 7816 Transmit Length Register, offset: 0x1F */
} volatile *FNET_MK_UART_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- UART - Register accessor macros
   ---------------------------------------------------------------------------- */

/* UART - Register accessors */
#define FNET_MK_UART_BDH_REG(base)                       ((base)->BDH)
#define FNET_MK_UART_BDL_REG(base)                       ((base)->BDL)
#define FNET_MK_UART_C1_REG(base)                        ((base)->C1)
#define FNET_MK_UART_C2_REG(base)                        ((base)->C2)
#define FNET_MK_UART_S1_REG(base)                        ((base)->S1)
#define FNET_MK_UART_S2_REG(base)                        ((base)->S2)
#define FNET_MK_UART_C3_REG(base)                        ((base)->C3)
#define FNET_MK_UART_D_REG(base)                         ((base)->D)
#define FNET_MK_UART_MA1_REG(base)                       ((base)->MA1)
#define FNET_MK_UART_MA2_REG(base)                       ((base)->MA2)
#define FNET_MK_UART_C4_REG(base)                        ((base)->C4)
#define FNET_MK_UART_C5_REG(base)                        ((base)->C5)
#define FNET_MK_UART_ED_REG(base)                        ((base)->ED)
#define FNET_MK_UART_MODEM_REG(base)                     ((base)->MODEM)
#define FNET_MK_UART_IR_REG(base)                        ((base)->IR)
#define FNET_MK_UART_PFIFO_REG(base)                     ((base)->PFIFO)
#define FNET_MK_UART_CFIFO_REG(base)                     ((base)->CFIFO)
#define FNET_MK_UART_SFIFO_REG(base)                     ((base)->SFIFO)
#define FNET_MK_UART_TWFIFO_REG(base)                    ((base)->TWFIFO)
#define FNET_MK_UART_TCFIFO_REG(base)                    ((base)->TCFIFO)
#define FNET_MK_UART_RWFIFO_REG(base)                    ((base)->RWFIFO)
#define FNET_MK_UART_RCFIFO_REG(base)                    ((base)->RCFIFO)
#define FNET_MK_UART_C7816_REG(base)                     ((base)->C7816)
#define FNET_MK_UART_IE7816_REG(base)                    ((base)->IE7816)
#define FNET_MK_UART_IS7816_REG(base)                    ((base)->IS7816)
#define FNET_MK_UART_WP7816_T_TYPE0_REG(base)            ((base)->WP7816_T_TYPE0)
#define FNET_MK_UART_WP7816_T_TYPE1_REG(base)            ((base)->WP7816_T_TYPE1)
#define FNET_MK_UART_WN7816_REG(base)                    ((base)->WN7816)
#define FNET_MK_UART_WF7816_REG(base)                    ((base)->WF7816)
#define FNET_MK_UART_ET7816_REG(base)                    ((base)->ET7816)
#define FNET_MK_UART_TL7816_REG(base)                    ((base)->TL7816)

/* ----------------------------------------------------------------------------
   -- UART Register Masks
   ---------------------------------------------------------------------------- */

/* BDH Bit Fields */
#define FNET_MK_UART_BDH_SBR_MASK                        0x1Fu
#define FNET_MK_UART_BDH_SBR_SHIFT                       0
#define FNET_MK_UART_BDH_SBR(x)                          (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_BDH_SBR_SHIFT))&FNET_MK_UART_BDH_SBR_MASK)
#define FNET_MK_UART_BDH_RXEDGIE_MASK                    0x40u
#define FNET_MK_UART_BDH_RXEDGIE_SHIFT                   6
#define FNET_MK_UART_BDH_LBKDIE_MASK                     0x80u
#define FNET_MK_UART_BDH_LBKDIE_SHIFT                    7
/* BDL Bit Fields */
#define FNET_MK_UART_BDL_SBR_MASK                        0xFFu
#define FNET_MK_UART_BDL_SBR_SHIFT                       0
#define FNET_MK_UART_BDL_SBR(x)                          (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_BDL_SBR_SHIFT))&FNET_MK_UART_BDL_SBR_MASK)
/* C1 Bit Fields */
#define FNET_MK_UART_C1_PT_MASK                          0x1u
#define FNET_MK_UART_C1_PT_SHIFT                         0
#define FNET_MK_UART_C1_PE_MASK                          0x2u
#define FNET_MK_UART_C1_PE_SHIFT                         1
#define FNET_MK_UART_C1_ILT_MASK                         0x4u
#define FNET_MK_UART_C1_ILT_SHIFT                        2
#define FNET_MK_UART_C1_WAKE_MASK                        0x8u
#define FNET_MK_UART_C1_WAKE_SHIFT                       3
#define FNET_MK_UART_C1_M_MASK                           0x10u
#define FNET_MK_UART_C1_M_SHIFT                          4
#define FNET_MK_UART_C1_RSRC_MASK                        0x20u
#define FNET_MK_UART_C1_RSRC_SHIFT                       5
#define FNET_MK_UART_C1_UARTSWAI_MASK                    0x40u
#define FNET_MK_UART_C1_UARTSWAI_SHIFT                   6
#define FNET_MK_UART_C1_LOOPS_MASK                       0x80u
#define FNET_MK_UART_C1_LOOPS_SHIFT                      7
/* C2 Bit Fields */
#define FNET_MK_UART_C2_SBK_MASK                         0x1u
#define FNET_MK_UART_C2_SBK_SHIFT                        0
#define FNET_MK_UART_C2_RWU_MASK                         0x2u
#define FNET_MK_UART_C2_RWU_SHIFT                        1
#define FNET_MK_UART_C2_RE_MASK                          0x4u
#define FNET_MK_UART_C2_RE_SHIFT                         2
#define FNET_MK_UART_C2_TE_MASK                          0x8u
#define FNET_MK_UART_C2_TE_SHIFT                         3
#define FNET_MK_UART_C2_ILIE_MASK                        0x10u
#define FNET_MK_UART_C2_ILIE_SHIFT                       4
#define FNET_MK_UART_C2_RIE_MASK                         0x20u
#define FNET_MK_UART_C2_RIE_SHIFT                        5
#define FNET_MK_UART_C2_TCIE_MASK                        0x40u
#define FNET_MK_UART_C2_TCIE_SHIFT                       6
#define FNET_MK_UART_C2_TIE_MASK                         0x80u
#define FNET_MK_UART_C2_TIE_SHIFT                        7
/* S1 Bit Fields */
#define FNET_MK_UART_S1_PF_MASK                          0x1u
#define FNET_MK_UART_S1_PF_SHIFT                         0
#define FNET_MK_UART_S1_FE_MASK                          0x2u
#define FNET_MK_UART_S1_FE_SHIFT                         1
#define FNET_MK_UART_S1_NF_MASK                          0x4u
#define FNET_MK_UART_S1_NF_SHIFT                         2
#define FNET_MK_UART_S1_OR_MASK                          0x8u
#define FNET_MK_UART_S1_OR_SHIFT                         3
#define FNET_MK_UART_S1_IDLE_MASK                        0x10u
#define FNET_MK_UART_S1_IDLE_SHIFT                       4
#define FNET_MK_UART_S1_RDRF_MASK                        0x20u
#define FNET_MK_UART_S1_RDRF_SHIFT                       5
#define FNET_MK_UART_S1_TC_MASK                          0x40u
#define FNET_MK_UART_S1_TC_SHIFT                         6
#define FNET_MK_UART_S1_TDRE_MASK                        0x80u
#define FNET_MK_UART_S1_TDRE_SHIFT                       7
/* S2 Bit Fields */
#define FNET_MK_UART_S2_RAF_MASK                         0x1u
#define FNET_MK_UART_S2_RAF_SHIFT                        0
#define FNET_MK_UART_S2_LBKDE_MASK                       0x2u
#define FNET_MK_UART_S2_LBKDE_SHIFT                      1
#define FNET_MK_UART_S2_BRK13_MASK                       0x4u
#define FNET_MK_UART_S2_BRK13_SHIFT                      2
#define FNET_MK_UART_S2_RWUID_MASK                       0x8u
#define FNET_MK_UART_S2_RWUID_SHIFT                      3
#define FNET_MK_UART_S2_RXINV_MASK                       0x10u
#define FNET_MK_UART_S2_RXINV_SHIFT                      4
#define FNET_MK_UART_S2_MSBF_MASK                        0x20u
#define FNET_MK_UART_S2_MSBF_SHIFT                       5
#define FNET_MK_UART_S2_RXEDGIF_MASK                     0x40u
#define FNET_MK_UART_S2_RXEDGIF_SHIFT                    6
#define FNET_MK_UART_S2_LBKDIF_MASK                      0x80u
#define FNET_MK_UART_S2_LBKDIF_SHIFT                     7
/* C3 Bit Fields */
#define FNET_MK_UART_C3_PEIE_MASK                        0x1u
#define FNET_MK_UART_C3_PEIE_SHIFT                       0
#define FNET_MK_UART_C3_FEIE_MASK                        0x2u
#define FNET_MK_UART_C3_FEIE_SHIFT                       1
#define FNET_MK_UART_C3_NEIE_MASK                        0x4u
#define FNET_MK_UART_C3_NEIE_SHIFT                       2
#define FNET_MK_UART_C3_ORIE_MASK                        0x8u
#define FNET_MK_UART_C3_ORIE_SHIFT                       3
#define FNET_MK_UART_C3_TXINV_MASK                       0x10u
#define FNET_MK_UART_C3_TXINV_SHIFT                      4
#define FNET_MK_UART_C3_TXDIR_MASK                       0x20u
#define FNET_MK_UART_C3_TXDIR_SHIFT                      5
#define FNET_MK_UART_C3_T8_MASK                          0x40u
#define FNET_MK_UART_C3_T8_SHIFT                         6
#define FNET_MK_UART_C3_R8_MASK                          0x80u
#define FNET_MK_UART_C3_R8_SHIFT                         7
/* D Bit Fields */
#define FNET_MK_UART_D_RT_MASK                           0xFFu
#define FNET_MK_UART_D_RT_SHIFT                          0
#define FNET_MK_UART_D_RT(x)                             (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_D_RT_SHIFT))&FNET_MK_UART_D_RT_MASK)
/* MA1 Bit Fields */
#define FNET_MK_UART_MA1_MA_MASK                         0xFFu
#define FNET_MK_UART_MA1_MA_SHIFT                        0
#define FNET_MK_UART_MA1_MA(x)                           (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_MA1_MA_SHIFT))&FNET_MK_UART_MA1_MA_MASK)
/* MA2 Bit Fields */
#define FNET_MK_UART_MA2_MA_MASK                         0xFFu
#define FNET_MK_UART_MA2_MA_SHIFT                        0
#define FNET_MK_UART_MA2_MA(x)                           (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_MA2_MA_SHIFT))&FNET_MK_UART_MA2_MA_MASK)
/* C4 Bit Fields */
#define FNET_MK_UART_C4_BRFA_MASK                        0x1Fu
#define FNET_MK_UART_C4_BRFA_SHIFT                       0
#define FNET_MK_UART_C4_BRFA(x)                          (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_C4_BRFA_SHIFT))&FNET_MK_UART_C4_BRFA_MASK)
#define FNET_MK_UART_C4_M10_MASK                         0x20u
#define FNET_MK_UART_C4_M10_SHIFT                        5
#define FNET_MK_UART_C4_MAEN2_MASK                       0x40u
#define FNET_MK_UART_C4_MAEN2_SHIFT                      6
#define FNET_MK_UART_C4_MAEN1_MASK                       0x80u
#define FNET_MK_UART_C4_MAEN1_SHIFT                      7
/* C5 Bit Fields */
#define FNET_MK_UART_C5_RDMAS_MASK                       0x20u
#define FNET_MK_UART_C5_RDMAS_SHIFT                      5
#define FNET_MK_UART_C5_TDMAS_MASK                       0x80u
#define FNET_MK_UART_C5_TDMAS_SHIFT                      7
/* ED Bit Fields */
#define FNET_MK_UART_ED_PARITYE_MASK                     0x40u
#define FNET_MK_UART_ED_PARITYE_SHIFT                    6
#define FNET_MK_UART_ED_NOISY_MASK                       0x80u
#define FNET_MK_UART_ED_NOISY_SHIFT                      7
/* MODEM Bit Fields */
#define FNET_MK_UART_MODEM_TXCTSE_MASK                   0x1u
#define FNET_MK_UART_MODEM_TXCTSE_SHIFT                  0
#define FNET_MK_UART_MODEM_TXRTSE_MASK                   0x2u
#define FNET_MK_UART_MODEM_TXRTSE_SHIFT                  1
#define FNET_MK_UART_MODEM_TXRTSPOL_MASK                 0x4u
#define FNET_MK_UART_MODEM_TXRTSPOL_SHIFT                2
#define FNET_MK_UART_MODEM_RXRTSE_MASK                   0x8u
#define FNET_MK_UART_MODEM_RXRTSE_SHIFT                  3
/* IR Bit Fields */
#define FNET_MK_UART_IR_TNP_MASK                         0x3u
#define FNET_MK_UART_IR_TNP_SHIFT                        0
#define FNET_MK_UART_IR_TNP(x)                           (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_IR_TNP_SHIFT))&FNET_MK_UART_IR_TNP_MASK)
#define FNET_MK_UART_IR_IREN_MASK                        0x4u
#define FNET_MK_UART_IR_IREN_SHIFT                       2
/* PFIFO Bit Fields */
#define FNET_MK_UART_PFIFO_RXFIFOSIZE_MASK               0x7u
#define FNET_MK_UART_PFIFO_RXFIFOSIZE_SHIFT              0
#define FNET_MK_UART_PFIFO_RXFIFOSIZE(x)                 (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_PFIFO_RXFIFOSIZE_SHIFT))&FNET_MK_UART_PFIFO_RXFIFOSIZE_MASK)
#define FNET_MK_UART_PFIFO_RXFE_MASK                     0x8u
#define FNET_MK_UART_PFIFO_RXFE_SHIFT                    3
#define FNET_MK_UART_PFIFO_TXFIFOSIZE_MASK               0x70u
#define FNET_MK_UART_PFIFO_TXFIFOSIZE_SHIFT              4
#define FNET_MK_UART_PFIFO_TXFIFOSIZE(x)                 (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_PFIFO_TXFIFOSIZE_SHIFT))&FNET_MK_UART_PFIFO_TXFIFOSIZE_MASK)
#define FNET_MK_UART_PFIFO_TXFE_MASK                     0x80u
#define FNET_MK_UART_PFIFO_TXFE_SHIFT                    7
/* CFIFO Bit Fields */
#define FNET_MK_UART_CFIFO_RXUFE_MASK                    0x1u
#define FNET_MK_UART_CFIFO_RXUFE_SHIFT                   0
#define FNET_MK_UART_CFIFO_TXOFE_MASK                    0x2u
#define FNET_MK_UART_CFIFO_TXOFE_SHIFT                   1
#define FNET_MK_UART_CFIFO_RXFLUSH_MASK                  0x40u
#define FNET_MK_UART_CFIFO_RXFLUSH_SHIFT                 6
#define FNET_MK_UART_CFIFO_TXFLUSH_MASK                  0x80u
#define FNET_MK_UART_CFIFO_TXFLUSH_SHIFT                 7
/* SFIFO Bit Fields */
#define FNET_MK_UART_SFIFO_RXUF_MASK                     0x1u
#define FNET_MK_UART_SFIFO_RXUF_SHIFT                    0
#define FNET_MK_UART_SFIFO_TXOF_MASK                     0x2u
#define FNET_MK_UART_SFIFO_TXOF_SHIFT                    1
#define FNET_MK_UART_SFIFO_RXEMPT_MASK                   0x40u
#define FNET_MK_UART_SFIFO_RXEMPT_SHIFT                  6
#define FNET_MK_UART_SFIFO_TXEMPT_MASK                   0x80u
#define FNET_MK_UART_SFIFO_TXEMPT_SHIFT                  7
/* TWFIFO Bit Fields */
#define FNET_MK_UART_TWFIFO_TXWATER_MASK                 0xFFu
#define FNET_MK_UART_TWFIFO_TXWATER_SHIFT                0
#define FNET_MK_UART_TWFIFO_TXWATER(x)                   (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_TWFIFO_TXWATER_SHIFT))&FNET_MK_UART_TWFIFO_TXWATER_MASK)
/* TCFIFO Bit Fields */
#define FNET_MK_UART_TCFIFO_TXCOUNT_MASK                 0xFFu
#define FNET_MK_UART_TCFIFO_TXCOUNT_SHIFT                0
#define FNET_MK_UART_TCFIFO_TXCOUNT(x)                   (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_TCFIFO_TXCOUNT_SHIFT))&FNET_MK_UART_TCFIFO_TXCOUNT_MASK)
/* RWFIFO Bit Fields */
#define FNET_MK_UART_RWFIFO_RXWATER_MASK                 0xFFu
#define FNET_MK_UART_RWFIFO_RXWATER_SHIFT                0
#define FNET_MK_UART_RWFIFO_RXWATER(x)                   (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_RWFIFO_RXWATER_SHIFT))&FNET_MK_UART_RWFIFO_RXWATER_MASK)
/* RCFIFO Bit Fields */
#define FNET_MK_UART_RCFIFO_RXCOUNT_MASK                 0xFFu
#define FNET_MK_UART_RCFIFO_RXCOUNT_SHIFT                0
#define FNET_MK_UART_RCFIFO_RXCOUNT(x)                   (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_RCFIFO_RXCOUNT_SHIFT))&FNET_MK_UART_RCFIFO_RXCOUNT_MASK)
/* C7816 Bit Fields */
#define FNET_MK_UART_C7816_ISO_7816E_MASK                0x1u
#define FNET_MK_UART_C7816_ISO_7816E_SHIFT               0
#define FNET_MK_UART_C7816_TTYPE_MASK                    0x2u
#define FNET_MK_UART_C7816_TTYPE_SHIFT                   1
#define FNET_MK_UART_C7816_INIT_MASK                     0x4u
#define FNET_MK_UART_C7816_INIT_SHIFT                    2
#define FNET_MK_UART_C7816_ANACK_MASK                    0x8u
#define FNET_MK_UART_C7816_ANACK_SHIFT                   3
#define FNET_MK_UART_C7816_ONACK_MASK                    0x10u
#define FNET_MK_UART_C7816_ONACK_SHIFT                   4
/* IE7816 Bit Fields */
#define FNET_MK_UART_IE7816_RXTE_MASK                    0x1u
#define FNET_MK_UART_IE7816_RXTE_SHIFT                   0
#define FNET_MK_UART_IE7816_TXTE_MASK                    0x2u
#define FNET_MK_UART_IE7816_TXTE_SHIFT                   1
#define FNET_MK_UART_IE7816_GTVE_MASK                    0x4u
#define FNET_MK_UART_IE7816_GTVE_SHIFT                   2
#define FNET_MK_UART_IE7816_INITDE_MASK                  0x10u
#define FNET_MK_UART_IE7816_INITDE_SHIFT                 4
#define FNET_MK_UART_IE7816_BWTE_MASK                    0x20u
#define FNET_MK_UART_IE7816_BWTE_SHIFT                   5
#define FNET_MK_UART_IE7816_CWTE_MASK                    0x40u
#define FNET_MK_UART_IE7816_CWTE_SHIFT                   6
#define FNET_MK_UART_IE7816_WTE_MASK                     0x80u
#define FNET_MK_UART_IE7816_WTE_SHIFT                    7
/* IS7816 Bit Fields */
#define FNET_MK_UART_IS7816_RXT_MASK                     0x1u
#define FNET_MK_UART_IS7816_RXT_SHIFT                    0
#define FNET_MK_UART_IS7816_TXT_MASK                     0x2u
#define FNET_MK_UART_IS7816_TXT_SHIFT                    1
#define FNET_MK_UART_IS7816_GTV_MASK                     0x4u
#define FNET_MK_UART_IS7816_GTV_SHIFT                    2
#define FNET_MK_UART_IS7816_INITD_MASK                   0x10u
#define FNET_MK_UART_IS7816_INITD_SHIFT                  4
#define FNET_MK_UART_IS7816_BWT_MASK                     0x20u
#define FNET_MK_UART_IS7816_BWT_SHIFT                    5
#define FNET_MK_UART_IS7816_CWT_MASK                     0x40u
#define FNET_MK_UART_IS7816_CWT_SHIFT                    6
#define FNET_MK_UART_IS7816_WT_MASK                      0x80u
#define FNET_MK_UART_IS7816_WT_SHIFT                     7
/* WP7816_T_TYPE0 Bit Fields */
#define FNET_MK_UART_WP7816_T_TYPE0_WI_MASK              0xFFu
#define FNET_MK_UART_WP7816_T_TYPE0_WI_SHIFT             0
#define FNET_MK_UART_WP7816_T_TYPE0_WI(x)                (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_WP7816_T_TYPE0_WI_SHIFT))&FNET_MK_UART_WP7816_T_TYPE0_WI_MASK)
/* WP7816_T_TYPE1 Bit Fields */
#define FNET_MK_UART_WP7816_T_TYPE1_BWI_MASK             0xFu
#define FNET_MK_UART_WP7816_T_TYPE1_BWI_SHIFT            0
#define FNET_MK_UART_WP7816_T_TYPE1_BWI(x)               (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_WP7816_T_TYPE1_BWI_SHIFT))&FNET_MK_UART_WP7816_T_TYPE1_BWI_MASK)
#define FNET_MK_UART_WP7816_T_TYPE1_CWI_MASK             0xF0u
#define FNET_MK_UART_WP7816_T_TYPE1_CWI_SHIFT            4
#define FNET_MK_UART_WP7816_T_TYPE1_CWI(x)               (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_WP7816_T_TYPE1_CWI_SHIFT))&FNET_MK_UART_WP7816_T_TYPE1_CWI_MASK)
/* WN7816 Bit Fields */
#define FNET_MK_UART_WN7816_GTN_MASK                     0xFFu
#define FNET_MK_UART_WN7816_GTN_SHIFT                    0
#define FNET_MK_UART_WN7816_GTN(x)                       (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_WN7816_GTN_SHIFT))&FNET_MK_UART_WN7816_GTN_MASK)
/* WF7816 Bit Fields */
#define FNET_MK_UART_WF7816_GTFD_MASK                    0xFFu
#define FNET_MK_UART_WF7816_GTFD_SHIFT                   0
#define FNET_MK_UART_WF7816_GTFD(x)                      (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_WF7816_GTFD_SHIFT))&FNET_MK_UART_WF7816_GTFD_MASK)
/* ET7816 Bit Fields */
#define FNET_MK_UART_ET7816_RXTHRESHOLD_MASK             0xFu
#define FNET_MK_UART_ET7816_RXTHRESHOLD_SHIFT            0
#define FNET_MK_UART_ET7816_RXTHRESHOLD(x)               (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_ET7816_RXTHRESHOLD_SHIFT))&FNET_MK_UART_ET7816_RXTHRESHOLD_MASK)
#define FNET_MK_UART_ET7816_TXTHRESHOLD_MASK             0xF0u
#define FNET_MK_UART_ET7816_TXTHRESHOLD_SHIFT            4
#define FNET_MK_UART_ET7816_TXTHRESHOLD(x)               (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_ET7816_TXTHRESHOLD_SHIFT))&FNET_MK_UART_ET7816_TXTHRESHOLD_MASK)
/* TL7816 Bit Fields */
#define FNET_MK_UART_TL7816_TLEN_MASK                    0xFFu
#define FNET_MK_UART_TL7816_TLEN_SHIFT                   0
#define FNET_MK_UART_TL7816_TLEN(x)                      (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_MK_UART_TL7816_TLEN_SHIFT))&FNET_MK_UART_TL7816_TLEN_MASK)


/* UART - Peripheral instance base addresses */
/* Peripheral UART0 base pointer */
#define FNET_MK_UART0_BASE_PTR                           ((FNET_MK_UART_MemMapPtr)0x4006A000u)
/* Peripheral UART1 base pointer */
#define FNET_MK_UART1_BASE_PTR                           ((FNET_MK_UART_MemMapPtr)0x4006B000u)
/* Peripheral UART2 base pointer */
#define FNET_MK_UART2_BASE_PTR                           ((FNET_MK_UART_MemMapPtr)0x4006C000u)
/* Peripheral UART3 base pointer */
#define FNET_MK_UART3_BASE_PTR                           ((FNET_MK_UART_MemMapPtr)0x4006D000u)
/* Peripheral UART4 base pointer */
#define FNET_MK_UART4_BASE_PTR                           ((FNET_MK_UART_MemMapPtr)0x400EA000u)
/* Peripheral UART5 base pointer */
#define FNET_MK_UART5_BASE_PTR                           ((FNET_MK_UART_MemMapPtr)0x400EB000u)

/* ----------------------------------------------------------------------------
   -- UART - Register accessor macros
   ---------------------------------------------------------------------------- */

/* UART - Register instance definitions */
/* UART0 */
#define FNET_MK_UART0_BDH                                FNET_MK_UART_BDH_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_BDL                                FNET_MK_UART_BDL_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_C1                                 FNET_MK_UART_C1_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_C2                                 FNET_MK_UART_C2_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_S1                                 FNET_MK_UART_S1_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_S2                                 FNET_MK_UART_S2_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_C3                                 FNET_MK_UART_C3_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_D                                  FNET_MK_UART_D_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_MA1                                FNET_MK_UART_MA1_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_MA2                                FNET_MK_UART_MA2_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_C4                                 FNET_MK_UART_C4_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_C5                                 FNET_MK_UART_C5_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_ED                                 FNET_MK_UART_ED_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_MODEM                              FNET_MK_UART_MODEM_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_IR                                 FNET_MK_UART_IR_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_PFIFO                              FNET_MK_UART_PFIFO_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_CFIFO                              FNET_MK_UART_CFIFO_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_SFIFO                              FNET_MK_UART_SFIFO_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_TWFIFO                             FNET_MK_UART_TWFIFO_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_TCFIFO                             FNET_MK_UART_TCFIFO_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_RWFIFO                             FNET_MK_UART_RWFIFO_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_RCFIFO                             FNET_MK_UART_RCFIFO_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_C7816                              FNET_MK_UART_C7816_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_IE7816                             FNET_MK_UART_IE7816_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_IS7816                             FNET_MK_UART_IS7816_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_WP7816T0                           FNET_MK_UART_WP7816_T_TYPE0_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_WP7816T1                           FNET_MK_UART_WP7816_T_TYPE1_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_WN7816                             FNET_MK_UART_WN7816_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_WF7816                             FNET_MK_UART_WF7816_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_ET7816                             FNET_MK_UART_ET7816_REG(FNET_MK_UART0_BASE_PTR)
#define FNET_MK_UART0_TL7816                             FNET_MK_UART_TL7816_REG(FNET_MK_UART0_BASE_PTR)
/* UART1 */
#define FNET_MK_UART1_BDH                                FNET_MK_UART_BDH_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_BDL                                FNET_MK_UART_BDL_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_C1                                 FNET_MK_UART_C1_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_C2                                 FNET_MK_UART_C2_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_S1                                 FNET_MK_UART_S1_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_S2                                 FNET_MK_UART_S2_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_C3                                 FNET_MK_UART_C3_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_D                                  FNET_MK_UART_D_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_MA1                                FNET_MK_UART_MA1_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_MA2                                FNET_MK_UART_MA2_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_C4                                 FNET_MK_UART_C4_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_C5                                 FNET_MK_UART_C5_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_ED                                 FNET_MK_UART_ED_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_MODEM                              FNET_MK_UART_MODEM_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_IR                                 FNET_MK_UART_IR_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_PFIFO                              FNET_MK_UART_PFIFO_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_CFIFO                              FNET_MK_UART_CFIFO_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_SFIFO                              FNET_MK_UART_SFIFO_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_TWFIFO                             FNET_MK_UART_TWFIFO_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_TCFIFO                             FNET_MK_UART_TCFIFO_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_RWFIFO                             FNET_MK_UART_RWFIFO_REG(FNET_MK_UART1_BASE_PTR)
#define FNET_MK_UART1_RCFIFO                             FNET_MK_UART_RCFIFO_REG(FNET_MK_UART1_BASE_PTR)
/* UART2 */
#define FNET_MK_UART2_BDH                                FNET_MK_UART_BDH_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_BDL                                FNET_MK_UART_BDL_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_C1                                 FNET_MK_UART_C1_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_C2                                 FNET_MK_UART_C2_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_S1                                 FNET_MK_UART_S1_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_S2                                 FNET_MK_UART_S2_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_C3                                 FNET_MK_UART_C3_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_D                                  FNET_MK_UART_D_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_MA1                                FNET_MK_UART_MA1_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_MA2                                FNET_MK_UART_MA2_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_C4                                 FNET_MK_UART_C4_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_C5                                 FNET_MK_UART_C5_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_ED                                 FNET_MK_UART_ED_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_MODEM                              FNET_MK_UART_MODEM_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_IR                                 FNET_MK_UART_IR_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_PFIFO                              FNET_MK_UART_PFIFO_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_CFIFO                              FNET_MK_UART_CFIFO_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_SFIFO                              FNET_MK_UART_SFIFO_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_TWFIFO                             FNET_MK_UART_TWFIFO_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_TCFIFO                             FNET_MK_UART_TCFIFO_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_RWFIFO                             FNET_MK_UART_RWFIFO_REG(FNET_MK_UART2_BASE_PTR)
#define FNET_MK_UART2_RCFIFO                             FNET_MK_UART_RCFIFO_REG(FNET_MK_UART2_BASE_PTR)
/* UART3 */
#define FNET_MK_UART3_BDH                                FNET_MK_UART_BDH_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_BDL                                FNET_MK_UART_BDL_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_C1                                 FNET_MK_UART_C1_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_C2                                 FNET_MK_UART_C2_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_S1                                 FNET_MK_UART_S1_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_S2                                 FNET_MK_UART_S2_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_C3                                 FNET_MK_UART_C3_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_D                                  FNET_MK_UART_D_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_MA1                                FNET_MK_UART_MA1_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_MA2                                FNET_MK_UART_MA2_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_C4                                 FNET_MK_UART_C4_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_C5                                 FNET_MK_UART_C5_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_ED                                 FNET_MK_UART_ED_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_MODEM                              FNET_MK_UART_MODEM_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_IR                                 FNET_MK_UART_IR_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_PFIFO                              FNET_MK_UART_PFIFO_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_CFIFO                              FNET_MK_UART_CFIFO_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_SFIFO                              FNET_MK_UART_SFIFO_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_TWFIFO                             FNET_MK_UART_TWFIFO_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_TCFIFO                             FNET_MK_UART_TCFIFO_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_RWFIFO                             FNET_MK_UART_RWFIFO_REG(FNET_MK_UART3_BASE_PTR)
#define FNET_MK_UART3_RCFIFO                             FNET_MK_UART_RCFIFO_REG(FNET_MK_UART3_BASE_PTR)
/* UART4 */
#define FNET_MK_UART4_BDH                                FNET_MK_UART_BDH_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_BDL                                FNET_MK_UART_BDL_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_C1                                 FNET_MK_UART_C1_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_C2                                 FNET_MK_UART_C2_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_S1                                 FNET_MK_UART_S1_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_S2                                 FNET_MK_UART_S2_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_C3                                 FNET_MK_UART_C3_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_D                                  FNET_MK_UART_D_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_MA1                                FNET_MK_UART_MA1_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_MA2                                FNET_MK_UART_MA2_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_C4                                 FNET_MK_UART_C4_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_C5                                 FNET_MK_UART_C5_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_ED                                 FNET_MK_UART_ED_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_MODEM                              FNET_MK_UART_MODEM_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_IR                                 FNET_MK_UART_IR_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_PFIFO                              FNET_MK_UART_PFIFO_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_CFIFO                              FNET_MK_UART_CFIFO_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_SFIFO                              FNET_MK_UART_SFIFO_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_TWFIFO                             FNET_MK_UART_TWFIFO_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_TCFIFO                             FNET_MK_UART_TCFIFO_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_RWFIFO                             FNET_MK_UART_RWFIFO_REG(FNET_MK_UART4_BASE_PTR)
#define FNET_MK_UART4_RCFIFO                             FNET_MK_UART_RCFIFO_REG(FNET_MK_UART4_BASE_PTR)
/* UART5 */
#define FNET_MK_UART5_BDH                                FNET_MK_UART_BDH_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_BDL                                FNET_MK_UART_BDL_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_C1                                 FNET_MK_UART_C1_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_C2                                 FNET_MK_UART_C2_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_S1                                 FNET_MK_UART_S1_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_S2                                 FNET_MK_UART_S2_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_C3                                 FNET_MK_UART_C3_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_D                                  FNET_MK_UART_D_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_MA1                                FNET_MK_UART_MA1_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_MA2                                FNET_MK_UART_MA2_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_C4                                 FNET_MK_UART_C4_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_C5                                 FNET_MK_UART_C5_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_ED                                 FNET_MK_UART_ED_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_MODEM                              FNET_MK_UART_MODEM_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_IR                                 FNET_MK_UART_IR_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_PFIFO                              FNET_MK_UART_PFIFO_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_CFIFO                              FNET_MK_UART_CFIFO_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_SFIFO                              FNET_MK_UART_SFIFO_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_TWFIFO                             FNET_MK_UART_TWFIFO_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_TCFIFO                             FNET_MK_UART_TCFIFO_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_RWFIFO                             FNET_MK_UART_RWFIFO_REG(FNET_MK_UART5_BASE_PTR)
#define FNET_MK_UART5_RCFIFO                             FNET_MK_UART_RCFIFO_REG(FNET_MK_UART5_BASE_PTR)

/* ----------------------------------------------------------------------------
   -- SIM
   ---------------------------------------------------------------------------- */

/* SIM - Peripheral register structure */
typedef struct FNET_MK_SIM_MemMap
{
    fnet_uint32_t SOPT1;                                  /* System Options Register 1, offset: 0x0 */
    fnet_uint8_t RESERVED_0[4096];
    fnet_uint32_t SOPT2;                                  /* System Options Register 2, offset: 0x1004 */
    fnet_uint8_t RESERVED_1[4];
    fnet_uint32_t SOPT4;                                  /* System Options Register 4, offset: 0x100C */
    fnet_uint32_t SOPT5;                                  /* System Options Register 5, offset: 0x1010 */
    fnet_uint32_t SOPT6;                                  /* System Options Register 6, offset: 0x1014 */
    fnet_uint32_t SOPT7;                                  /* System Options Register 7, offset: 0x1018 */
    fnet_uint8_t RESERVED_2[8];
    fnet_uint32_t SDID;                                   /* System Device Identification Register, offset: 0x1024 */
    fnet_uint32_t SCGC1;                                  /* System Clock Gating Control Register 1, offset: 0x1028 */
    fnet_uint32_t SCGC2;                                  /* System Clock Gating Control Register 2, offset: 0x102C */
    fnet_uint32_t SCGC3;                                  /* System Clock Gating Control Register 3, offset: 0x1030 */
    fnet_uint32_t SCGC4;                                  /* System Clock Gating Control Register 4, offset: 0x1034 */
    fnet_uint32_t SCGC5;                                  /* System Clock Gating Control Register 5, offset: 0x1038 */
    fnet_uint32_t SCGC6;                                  /* System Clock Gating Control Register 6, offset: 0x103C */
    fnet_uint32_t SCGC7;                                  /* System Clock Gating Control Register 7, offset: 0x1040 */
    fnet_uint32_t CLKDIV1;                                /* System Clock Divider Register 1, offset: 0x1044 */
    fnet_uint32_t CLKDIV2;                                /* System Clock Divider Register 2, offset: 0x1048 */
    fnet_uint32_t FCFG1;                                  /* Flash Configuration Register 1, offset: 0x104C */
    fnet_uint32_t FCFG2;                                  /* Flash Configuration Register 2, offset: 0x1050 */
    fnet_uint32_t UIDH;                                   /* Unique Identification Register High, offset: 0x1054 */
    fnet_uint32_t UIDMH;                                  /* Unique Identification Register Mid-High, offset: 0x1058 */
    fnet_uint32_t UIDML;                                  /* Unique Identification Register Mid Low, offset: 0x105C */
    fnet_uint32_t UIDL;                                   /* Unique Identification Register Low, offset: 0x1060 */
} volatile *FNET_MK_SIM_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- SIM - Register accessor macros
   ---------------------------------------------------------------------------- */

/* SIM - Register accessors */
#define FNET_MK_SIM_SOPT1_REG(base)                      ((base)->SOPT1)
#define FNET_MK_SIM_SOPT2_REG(base)                      ((base)->SOPT2)
#define FNET_MK_SIM_SOPT4_REG(base)                      ((base)->SOPT4)
#define FNET_MK_SIM_SOPT5_REG(base)                      ((base)->SOPT5)
#define FNET_MK_SIM_SOPT6_REG(base)                      ((base)->SOPT6)
#define FNET_MK_SIM_SOPT7_REG(base)                      ((base)->SOPT7)
#define FNET_MK_SIM_SDID_REG(base)                       ((base)->SDID)
#define FNET_MK_SIM_SCGC1_REG(base)                      ((base)->SCGC1)
#define FNET_MK_SIM_SCGC2_REG(base)                      ((base)->SCGC2)
#define FNET_MK_SIM_SCGC3_REG(base)                      ((base)->SCGC3)
#define FNET_MK_SIM_SCGC4_REG(base)                      ((base)->SCGC4)
#define FNET_MK_SIM_SCGC5_REG(base)                      ((base)->SCGC5)
#define FNET_MK_SIM_SCGC6_REG(base)                      ((base)->SCGC6)
#define FNET_MK_SIM_SCGC7_REG(base)                      ((base)->SCGC7)
#define FNET_MK_SIM_CLKDIV1_REG(base)                    ((base)->CLKDIV1)
#define FNET_MK_SIM_CLKDIV2_REG(base)                    ((base)->CLKDIV2)
#define FNET_MK_SIM_FCFG1_REG(base)                      ((base)->FCFG1)
#define FNET_MK_SIM_FCFG2_REG(base)                      ((base)->FCFG2)
#define FNET_MK_SIM_UIDH_REG(base)                       ((base)->UIDH)
#define FNET_MK_SIM_UIDMH_REG(base)                      ((base)->UIDMH)
#define FNET_MK_SIM_UIDML_REG(base)                      ((base)->UIDML)
#define FNET_MK_SIM_UIDL_REG(base)                       ((base)->UIDL)


/* ----------------------------------------------------------------------------
   -- SIM Register Masks
   ---------------------------------------------------------------------------- */

/* SOPT1 Bit Fields */
#define FNET_MK_SIM_SOPT1_RAMSIZE_MASK                   0xF000u
#define FNET_MK_SIM_SOPT1_RAMSIZE_SHIFT                  12
#define FNET_MK_SIM_SOPT1_RAMSIZE(x)                     (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SIM_SOPT1_RAMSIZE_SHIFT))&FNET_MK_SIM_SOPT1_RAMSIZE_MASK)
#define FNET_MK_SIM_SOPT1_OSC32KSEL_MASK                 0x80000u
#define FNET_MK_SIM_SOPT1_OSC32KSEL_SHIFT                19
#define FNET_MK_SIM_SOPT1_MS_MASK                        0x800000u
#define FNET_MK_SIM_SOPT1_MS_SHIFT                       23
#define FNET_MK_SIM_SOPT1_USBSTBY_MASK                   0x40000000u
#define FNET_MK_SIM_SOPT1_USBSTBY_SHIFT                  30
#define FNET_MK_SIM_SOPT1_USBREGEN_MASK                  0x80000000u
#define FNET_MK_SIM_SOPT1_USBREGEN_SHIFT                 31
/* SOPT2 Bit Fields */
#define FNET_MK_SIM_SOPT2_MCGCLKSEL_MASK                 0x1u
#define FNET_MK_SIM_SOPT2_MCGCLKSEL_SHIFT                0
#define FNET_MK_SIM_SOPT2_FBSL_MASK                      0x300u
#define FNET_MK_SIM_SOPT2_FBSL_SHIFT                     8
#define FNET_MK_SIM_SOPT2_FBSL(x)                        (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SIM_SOPT2_FBSL_SHIFT))&FNET_MK_SIM_SOPT2_FBSL_MASK)
#define FNET_MK_SIM_SOPT2_CMTUARTPAD_MASK                0x800u
#define FNET_MK_SIM_SOPT2_CMTUARTPAD_SHIFT               11
#define FNET_MK_SIM_SOPT2_TRACECLKSEL_MASK               0x1000u
#define FNET_MK_SIM_SOPT2_TRACECLKSEL_SHIFT              12
#define FNET_MK_SIM_SOPT2_PLLFLLSEL_MASK                 0x10000u
#define FNET_MK_SIM_SOPT2_PLLFLLSEL_SHIFT                16
#define FNET_MK_SIM_SOPT2_USBSRC_MASK                    0x40000u
#define FNET_MK_SIM_SOPT2_USBSRC_SHIFT                   18
#define FNET_MK_SIM_SOPT2_RMIISRC_MASK                   (0x80000U)
#define FNET_MK_SIM_SOPT2_RMIISRC_SHIFT                  (19U)
#define FNET_MK_SIM_SOPT2_RMIISRC(x)                     (((fnet_uint32_t)(((fnet_uint32_t)(x)) << FNET_MK_SIM_SOPT2_RMIISRC_SHIFT)) & FNET_MK_SIM_SOPT2_RMIISRC_MASK)
#define FNET_MK_SIM_SOPT2_TIMESRC_MASK                   0x300000u
#define FNET_MK_SIM_SOPT2_TIMESRC_SHIFT                  20
#define FNET_MK_SIM_SOPT2_TIMESRC(x)                     (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SIM_SOPT2_TIMESRC_SHIFT))&FNET_MK_SIM_SOPT2_TIMESRC_MASK)
#define FNET_MK_SIM_SOPT2_I2SSRC_MASK                    0x3000000u
#define FNET_MK_SIM_SOPT2_I2SSRC_SHIFT                   24
#define FNET_MK_SIM_SOPT2_I2SSRC(x)                      (((fnet_uint32_t)(((v)(x))<<FNET_MK_SIM_SOPT2_I2SSRC_SHIFT))&FNET_MK_SIM_SOPT2_I2SSRC_MASK)
#define FNET_MK_SIM_SOPT2_SDHCSRC_MASK                   0x30000000u
#define FNET_MK_SIM_SOPT2_SDHCSRC_SHIFT                  28
#define FNET_MK_SIM_SOPT2_SDHCSRC(x)                     (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SIM_SOPT2_SDHCSRC_SHIFT))&FNET_MK_SIM_SOPT2_SDHCSRC_MASK)
/* SOPT4 Bit Fields */
#define FNET_MK_SIM_SOPT4_FTM0FLT0_MASK                  0x1u
#define FNET_MK_SIM_SOPT4_FTM0FLT0_SHIFT                 0
#define FNET_MK_SIM_SOPT4_FTM0FLT1_MASK                  0x2u
#define FNET_MK_SIM_SOPT4_FTM0FLT1_SHIFT                 1
#define FNET_MK_SIM_SOPT4_FTM0FLT2_MASK                  0x4u
#define FNET_MK_SIM_SOPT4_FTM0FLT2_SHIFT                 2
#define FNET_MK_SIM_SOPT4_FTM1FLT0_MASK                  0x10u
#define FNET_MK_SIM_SOPT4_FTM1FLT0_SHIFT                 4
#define FNET_MK_SIM_SOPT4_FTM2FLT0_MASK                  0x100u
#define FNET_MK_SIM_SOPT4_FTM2FLT0_SHIFT                 8
#define FNET_MK_SIM_SOPT4_FTM1CH0SRC_MASK                0xC0000u
#define FNET_MK_SIM_SOPT4_FTM1CH0SRC_SHIFT               18
#define FNET_MK_SIM_SOPT4_FTM1CH0SRC(x)                  (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SIM_SOPT4_FTM1CH0SRC_SHIFT))&FNET_MK_SIM_SOPT4_FTM1CH0SRC_MASK)
#define FNET_MK_SIM_SOPT4_FTM2CH0SRC_MASK                0x300000u
#define FNET_MK_SIM_SOPT4_FTM2CH0SRC_SHIFT               20
#define FNET_MK_SIM_SOPT4_FTM2CH0SRC(x)                  (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SIM_SOPT4_FTM2CH0SRC_SHIFT))&FNET_MK_SIM_SOPT4_FTM2CH0SRC_MASK)
#define FNET_MK_SIM_SOPT4_FTM0CLKSEL_MASK                0x1000000u
#define FNET_MK_SIM_SOPT4_FTM0CLKSEL_SHIFT               24
#define FNET_MK_SIM_SOPT4_FTM1CLKSEL_MASK                0x2000000u
#define FNET_MK_SIM_SOPT4_FTM1CLKSEL_SHIFT               25
#define FNET_MK_SIM_SOPT4_FTM2CLKSEL_MASK                0x4000000u
#define FNET_MK_SIM_SOPT4_FTM2CLKSEL_SHIFT               26
/* SOPT5 Bit Fields */
#define FNET_MK_SIM_SOPT5_UART0TXSRC_MASK                0x3u
#define FNET_MK_SIM_SOPT5_UART0TXSRC_SHIFT               0
#define FNET_MK_SIM_SOPT5_UART0TXSRC(x)                  (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SIM_SOPT5_UART0TXSRC_SHIFT))&FNET_MK_SIM_SOPT5_UART0TXSRC_MASK)
#define FNET_MK_SIM_SOPT5_UART0RXSRC_MASK                0xCu
#define FNET_MK_SIM_SOPT5_UART0RXSRC_SHIFT               2
#define FNET_MK_SIM_SOPT5_UART0RXSRC(x)                  (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SIM_SOPT5_UART0RXSRC_SHIFT))&FNET_MK_SIM_SOPT5_UART0RXSRC_MASK)
#define FNET_MK_SIM_SOPT5_UARTTXSRC_MASK                 0x30u
#define FNET_MK_SIM_SOPT5_UARTTXSRC_SHIFT                4
#define FNET_MK_SIM_SOPT5_UARTTXSRC(x)                   (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SIM_SOPT5_UARTTXSRC_SHIFT))&FNET_MK_SIM_SOPT5_UARTTXSRC_MASK)
#define FNET_MK_SIM_SOPT5_UART1RXSRC_MASK                0xC0u
#define FNET_MK_SIM_SOPT5_UART1RXSRC_SHIFT               6
#define FNET_MK_SIM_SOPT5_UART1RXSRC(x)                  (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SIM_SOPT5_UART1RXSRC_SHIFT))&FNET_MK_SIM_SOPT5_UART1RXSRC_MASK)
/* SOPT6 Bit Fields */
#define FNET_MK_SIM_SOPT6_RSTFLTSEL_MASK                 0x1F000000u
#define FNET_MK_SIM_SOPT6_RSTFLTSEL_SHIFT                24
#define FNET_MK_SIM_SOPT6_RSTFLTSEL(x)                   (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SIM_SOPT6_RSTFLTSEL_SHIFT))&FNET_MK_SIM_SOPT6_RSTFLTSEL_MASK)
#define FNET_MK_SIM_SOPT6_RSTFLTEN_MASK                  0xE0000000u
#define FNET_MK_SIM_SOPT6_RSTFLTEN_SHIFT                 29
#define FNET_MK_SIM_SOPT6_RSTFLTEN(x)                    (((uint32_t)(((uint32_t)(x))<<FNET_MK_SIM_SOPT6_RSTFLTEN_SHIFT))&FNET_MK_SIM_SOPT6_RSTFLTEN_MASK)
/* SOPT7 Bit Fields */
#define FNET_MK_SIM_SOPT7_ADC0TRGSEL_MASK                0xFu
#define FNET_MK_SIM_SOPT7_ADC0TRGSEL_SHIFT               0
#define FNET_MK_SIM_SOPT7_ADC0TRGSEL(x)                  (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SIM_SOPT7_ADC0TRGSEL_SHIFT))&FNET_MK_SIM_SOPT7_ADC0TRGSEL_MASK)
#define FNET_MK_SIM_SOPT7_ADC0PRETRGSEL_MASK             0x10u
#define FNET_MK_SIM_SOPT7_ADC0PRETRGSEL_SHIFT            4
#define FNET_MK_SIM_SOPT7_ADC0ALTTRGEN_MASK              0x80u
#define FNET_MK_SIM_SOPT7_ADC0ALTTRGEN_SHIFT             7
#define FNET_MK_SIM_SOPT7_ADC1TRGSEL_MASK                0xF00u
#define FNET_MK_SIM_SOPT7_ADC1TRGSEL_SHIFT               8
#define FNET_MK_SIM_SOPT7_ADC1TRGSEL(x)                  (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SIM_SOPT7_ADC1TRGSEL_SHIFT))&FNET_MK_SIM_SOPT7_ADC1TRGSEL_MASK)
#define FNET_MK_SIM_SOPT7_ADC1PRETRGSEL_MASK             0x1000u
#define FNET_MK_SIM_SOPT7_ADC1PRETRGSEL_SHIFT            12
#define FNET_MK_SIM_SOPT7_ADC1ALTTRGEN_MASK              0x8000u
#define FNET_MK_SIM_SOPT7_ADC1ALTTRGEN_SHIFT             15
/* SDID Bit Fields */
#define FNET_MK_SIM_SDID_PINID_MASK                      0xFu
#define FNET_MK_SIM_SDID_PINID_SHIFT                     0
#define FNET_MK_SIM_SDID_PINID(x)                        (((fnet_uint32_t)(((v)(x))<<FNET_MK_SIM_SDID_PINID_SHIFT))&FNET_MK_SIM_SDID_PINID_MASK)
#define FNET_MK_SIM_SDID_FAMID_MASK                      0x70u
#define FNET_MK_SIM_SDID_FAMID_SHIFT                     4
#define FNET_MK_SIM_SDID_FAMID(x)                        (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SIM_SDID_FAMID_SHIFT))&FNET_MK_SIM_SDID_FAMID_MASK)
#define FNET_MK_SIM_SDID_REVID_MASK                      0xF000u
#define FNET_MK_SIM_SDID_REVID_SHIFT                     12
#define FNET_MK_SIM_SDID_REVID(x)                        (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SIM_SDID_REVID_SHIFT))&FNET_MK_SIM_SDID_REVID_MASK)
/* SCGC1 Bit Fields */
#define FNET_MK_SIM_SCGC1_UART4_MASK                     0x400u
#define FNET_MK_SIM_SCGC1_UART4_SHIFT                    10
#define FNET_MK_SIM_SCGC1_UART5_MASK                     0x800u
#define FNET_MK_SIM_SCGC1_UART5_SHIFT                    11
/* SCGC2 Bit Fields */
#define FNET_MK_SIM_SCGC2_ENET_MASK                      0x1u
#define FNET_MK_SIM_SCGC2_ENET_SHIFT                     0
#define FNET_MK_SIM_SCGC2_DAC0_MASK                      0x1000u
#define FNET_MK_SIM_SCGC2_DAC0_SHIFT                     12
#define FNET_MK_SIM_SCGC2_DAC1_MASK                      0x2000u
#define FNET_MK_SIM_SCGC2_DAC1_SHIFT                     13
/* SCGC3 Bit Fields */
#define FNET_MK_SIM_SCGC3_RNGB_MASK                      0x1u
#define FNET_MK_SIM_SCGC3_RNGB_SHIFT                     0
#define FNET_MK_SIM_SCGC3_FLEXCAN1_MASK                  0x10u
#define FNET_MK_SIM_SCGC3_FLEXCAN1_SHIFT                 4
#define FNET_MK_SIM_SCGC3_SPI2_MASK                      0x1000u
#define FNET_MK_SIM_SCGC3_SPI2_SHIFT                     12
#define FNET_MK_SIM_SCGC3_SDHC_MASK                      0x20000u
#define FNET_MK_SIM_SCGC3_SDHC_SHIFT                     17
#define FNET_MK_SIM_SCGC3_FTM2_MASK                      0x1000000u
#define FNET_MK_SIM_SCGC3_FTM2_SHIFT                     24
#define FNET_MK_SIM_SCGC3_ADC1_MASK                      0x8000000u
#define FNET_MK_SIM_SCGC3_ADC1_SHIFT                     27
/* SCGC4 Bit Fields */
#define FNET_MK_SIM_SCGC4_EWM_MASK                       0x2u
#define FNET_MK_SIM_SCGC4_EWM_SHIFT                      1
#define FNET_MK_SIM_SCGC4_CMT_MASK                       0x4u
#define FNET_MK_SIM_SCGC4_CMT_SHIFT                      2
#define FNET_MK_SIM_SCGC4_I2C0_MASK                      0x40u
#define FNET_MK_SIM_SCGC4_I2C0_SHIFT                     6
#define FNET_MK_SIM_SCGC4_I2C1_MASK                      0x80u
#define FNET_MK_SIM_SCGC4_I2C1_SHIFT                     7
#define FNET_MK_SIM_SCGC4_UART0_MASK                     0x400u
#define FNET_MK_SIM_SCGC4_UART0_SHIFT                    10
#define FNET_MK_SIM_SCGC4_UART1_MASK                     0x800u
#define FNET_MK_SIM_SCGC4_UART1_SHIFT                    11
#define FNET_MK_SIM_SCGC4_UART2_MASK                     0x1000u
#define FNET_MK_SIM_SCGC4_UART2_SHIFT                    12
#define FNET_MK_SIM_SCGC4_UART3_MASK                     0x2000u
#define FNET_MK_SIM_SCGC4_UART3_SHIFT                    13
#define FNET_MK_SIM_SCGC4_USBOTG_MASK                    0x40000u
#define FNET_MK_SIM_SCGC4_USBOTG_SHIFT                   18
#define FNET_MK_SIM_SCGC4_CMP_MASK                       0x80000u
#define FNET_MK_SIM_SCGC4_CMP_SHIFT                      19
#define FNET_MK_SIM_SCGC4_VREF_MASK                      0x100000u
#define FNET_MK_SIM_SCGC4_VREF_SHIFT                     20
#define FNET_MK_SIM_SCGC4_LLWU_MASK                      0x10000000u
#define FNET_MK_SIM_SCGC4_LLWU_SHIFT                     28
/* SCGC5 Bit Fields */
#define FNET_MK_SIM_SCGC5_LPTIMER_MASK                   0x1u
#define FNET_MK_SIM_SCGC5_LPTIMER_SHIFT                  0
#define FNET_MK_SIM_SCGC5_REGFILE_MASK                   0x2u
#define FNET_MK_SIM_SCGC5_REGFILE_SHIFT                  1
#define FNET_MK_SIM_SCGC5_TSI_MASK                       0x20u
#define FNET_MK_SIM_SCGC5_TSI_SHIFT                      5
#define FNET_MK_SIM_SCGC5_PORTA_MASK                     0x200u
#define FNET_MK_SIM_SCGC5_PORTA_SHIFT                    9
#define FNET_MK_SIM_SCGC5_PORTB_MASK                     0x400u
#define FNET_MK_SIM_SCGC5_PORTB_SHIFT                    10
#define FNET_MK_SIM_SCGC5_PORTC_MASK                     0x800u
#define FNET_MK_SIM_SCGC5_PORTC_SHIFT                    11
#define FNET_MK_SIM_SCGC5_PORTD_MASK                     0x1000u
#define FNET_MK_SIM_SCGC5_PORTD_SHIFT                    12
#define FNET_MK_SIM_SCGC5_PORTE_MASK                     0x2000u
#define FNET_MK_SIM_SCGC5_PORTE_SHIFT                    13
#define FNET_MK_SIM_SCGC5_PORTF_MASK                     0x4000u
#define FNET_MK_SIM_SCGC5_PORTF_SHIFT                    14
/* SCGC6 Bit Fields */
#define FNET_MK_SIM_SCGC6_FTFL_MASK                      0x1u
#define FNET_MK_SIM_SCGC6_FTFL_SHIFT                     0
#define FNET_MK_SIM_SCGC6_DMAMUX_MASK                    0x2u
#define FNET_MK_SIM_SCGC6_DMAMUX_SHIFT                   1
#define FNET_MK_SIM_SCGC6_FLEXCAN0_MASK                  0x10u
#define FNET_MK_SIM_SCGC6_FLEXCAN0_SHIFT                 4
#define FNET_MK_SIM_SCGC6_DSPI0_MASK                     0x1000u
#define FNET_MK_SIM_SCGC6_DSPI0_SHIFT                    12
#define FNET_MK_SIM_SCGC6_SPI1_MASK                      0x2000u
#define FNET_MK_SIM_SCGC6_SPI1_SHIFT                     13
#define FNET_MK_SIM_SCGC6_I2S_MASK                       0x8000u
#define FNET_MK_SIM_SCGC6_I2S_SHIFT                      15
#define FNET_MK_SIM_SCGC6_CRC_MASK                       0x40000u
#define FNET_MK_SIM_SCGC6_CRC_SHIFT                      18
#define FNET_MK_SIM_SCGC6_USBDCD_MASK                    0x200000u
#define FNET_MK_SIM_SCGC6_USBDCD_SHIFT                   21
#define FNET_MK_SIM_SCGC6_PDB_MASK                       0x400000u
#define FNET_MK_SIM_SCGC6_PDB_SHIFT                      22
#define FNET_MK_SIM_SCGC6_PIT_MASK                       0x800000u
#define FNET_MK_SIM_SCGC6_PIT_SHIFT                      23
#define FNET_MK_SIM_SCGC6_FTM0_MASK                      0x1000000u
#define FNET_MK_SIM_SCGC6_FTM0_SHIFT                     24
#define FNET_MK_SIM_SCGC6_FTM1_MASK                      0x2000000u
#define FNET_MK_SIM_SCGC6_FTM1_SHIFT                     25
#define FNET_MK_SIM_SCGC6_ADC0_MASK                      0x8000000u
#define FNET_MK_SIM_SCGC6_ADC0_SHIFT                     27
#define FNET_MK_SIM_SCGC6_RTC_MASK                       0x20000000u
#define FNET_MK_SIM_SCGC6_RTC_SHIFT                      29
/* SCGC7 Bit Fields */
#define FNET_MK_SIM_SCGC7_FLEXBUS_MASK                   0x1u
#define FNET_MK_SIM_SCGC7_FLEXBUS_SHIFT                  0
#define FNET_MK_SIM_SCGC7_DMA_MASK                       0x2u
#define FNET_MK_SIM_SCGC7_DMA_SHIFT                      1
#define FNET_MK_SIM_SCGC7_MPU_MASK                       0x4u
#define FNET_MK_SIM_SCGC7_MPU_SHIFT                      2
/* CLKDIV1 Bit Fields */
#define FNET_MK_SIM_CLKDIV1_OUTDIV4_MASK                 0xF0000u
#define FNET_MK_SIM_CLKDIV1_OUTDIV4_SHIFT                16
#define FNET_MK_SIM_CLKDIV1_OUTDIV4(x)                   (((uint32_t)(((uint32_t)(x))<<FNET_MK_SIM_CLKDIV1_OUTDIV4_SHIFT))&FNET_MK_SIM_CLKDIV1_OUTDIV4_MASK)
#define FNET_MK_SIM_CLKDIV1_OUTDIV3_MASK                 0xF00000u
#define FNET_MK_SIM_CLKDIV1_OUTDIV3_SHIFT                20
#define FNET_MK_SIM_CLKDIV1_OUTDIV3(x)                   (((uint32_t)(((uint32_t)(x))<<FNET_MK_SIM_CLKDIV1_OUTDIV3_SHIFT))&FNET_MK_SIM_CLKDIV1_OUTDIV3_MASK)
#define FNET_MK_SIM_CLKDIV1_OUTDIV2_MASK                 0xF000000u
#define FNET_MK_SIM_CLKDIV1_OUTDIV2_SHIFT                24
#define FNET_MK_SIM_CLKDIV1_OUTDIV2(x)                   (((uint32_t)(((uint32_t)(x))<<FNET_MK_SIM_CLKDIV1_OUTDIV2_SHIFT))&FNET_MK_SIM_CLKDIV1_OUTDIV2_MASK)
#define FNET_MK_SIM_CLKDIV1_OUTDIV1_MASK                 0xF0000000u
#define FNET_MK_SIM_CLKDIV1_OUTDIV1_SHIFT                28
#define FNET_MK_SIM_CLKDIV1_OUTDIV1(x)                   (((uint32_t)(((uint32_t)(x))<<FNET_MK_SIM_CLKDIV1_OUTDIV1_SHIFT))&FNET_MK_SIM_CLKDIV1_OUTDIV1_MASK)
/* CLKDIV2 Bit Fields */
#define FNET_MK_SIM_CLKDIV2_USBFRAC_MASK                 0x1u
#define FNET_MK_SIM_CLKDIV2_USBFRAC_SHIFT                0
#define FNET_MK_SIM_CLKDIV2_USBDIV_MASK                  0xEu
#define FNET_MK_SIM_CLKDIV2_USBDIV_SHIFT                 1
#define FNET_MK_SIM_CLKDIV2_USBDIV(x)                    (((uint32_t)(((uint32_t)(x))<<FNET_MK_SIM_CLKDIV2_USBDIV_SHIFT))&FNET_MK_SIM_CLKDIV2_USBDIV_MASK)
#define FNET_MK_SIM_CLKDIV2_I2SFRAC_MASK                 0xFF00u
#define FNET_MK_SIM_CLKDIV2_I2SFRAC_SHIFT                8
#define FNET_MK_SIM_CLKDIV2_I2SFRAC(x)                   (((uint32_t)(((uint32_t)(x))<<FNET_MK_SIM_CLKDIV2_I2SFRAC_SHIFT))&FNET_MK_SIM_CLKDIV2_I2SFRAC_MASK)
#define FNET_MK_SIM_CLKDIV2_I2SDIV_MASK                  0xFFF00000u
#define FNET_MK_SIM_CLKDIV2_I2SDIV_SHIFT                 20
#define FNET_MK_SIM_CLKDIV2_I2SDIV(x)                    (((uint32_t)(((uint32_t)(x))<<FNET_MK_SIM_CLKDIV2_I2SDIV_SHIFT))&FNET_MK_SIM_CLKDIV2_I2SDIV_MASK)
/* FCFG1 Bit Fields */
#define FNET_MK_SIM_FCFG1_DEPART_MASK                    0xF00u
#define FNET_MK_SIM_FCFG1_DEPART_SHIFT                   8
#define FNET_MK_SIM_FCFG1_DEPART(x)                      (((uint32_t)(((uint32_t)(x))<<FNET_MK_SIM_FCFG1_DEPART_SHIFT))&FNET_MK_SIM_FCFG1_DEPART_MASK)
#define FNET_MK_SIM_FCFG1_EESIZE_MASK                    0xF0000u
#define FNET_MK_SIM_FCFG1_EESIZE_SHIFT                   16
#define FNET_MK_SIM_FCFG1_EESIZE(x)                      (((uint32_t)(((uint32_t)(x))<<FNET_MK_SIM_FCFG1_EESIZE_SHIFT))&FNET_MK_SIM_FCFG1_EESIZE_MASK)
#define FNET_MK_SIM_FCFG1_FSIZE_MASK                     0xFF000000u
#define FNET_MK_SIM_FCFG1_FSIZE_SHIFT                    24
#define FNET_MK_SIM_FCFG1_FSIZE(x)                       (((uint32_t)(((uint32_t)(x))<<FNET_MK_SIM_FCFG1_FSIZE_SHIFT))&FNET_MK_SIM_FCFG1_FSIZE_MASK)
/* FCFG2 Bit Fields */
#define FNET_MK_SIM_FCFG2_MAXADDR1_MASK                  0x3F0000u
#define FNET_MK_SIM_FCFG2_MAXADDR1_SHIFT                 16
#define FNET_MK_SIM_FCFG2_MAXADDR1(x)                    (((uint32_t)(((uint32_t)(x))<<FNET_MK_SIM_FCFG2_MAXADDR1_SHIFT))&FNET_MK_SIM_FCFG2_MAXADDR1_MASK)
#define FNET_MK_SIM_FCFG2_PFLSH_MASK                     0x800000u
#define FNET_MK_SIM_FCFG2_PFLSH_SHIFT                    23
#define FNET_MK_SIM_FCFG2_MAXADDR0_MASK                  0x3F000000u
#define FNET_MK_SIM_FCFG2_MAXADDR0_SHIFT                 24
#define FNET_MK_SIM_FCFG2_MAXADDR0(x)                    (((uint32_t)(((uint32_t)(x))<<FNET_MK_SIM_FCFG2_MAXADDR0_SHIFT))&FNET_MK_SIM_FCFG2_MAXADDR0_MASK)
#define FNET_MK_SIM_FCFG2_SWAPPFLSH_MASK                 0x80000000u
#define FNET_MK_SIM_FCFG2_SWAPPFLSH_SHIFT                31
/* UIDH Bit Fields */
#define FNET_MK_SIM_UIDH_UID_MASK                        0xFFFFFFFFu
#define FNET_MK_SIM_UIDH_UID_SHIFT                       0
#define FNET_MK_SIM_UIDH_UID(x)                          (((uint32_t)(((uint32_t)(x))<<FNET_MK_SIM_UIDH_UID_SHIFT))&FNET_MK_SIM_UIDH_UID_MASK)
/* UIDMH Bit Fields */
#define FNET_MK_SIM_UIDMH_UID_MASK                       0xFFFFFFFFu
#define FNET_MK_SIM_UIDMH_UID_SHIFT                      0
#define FNET_MK_SIM_UIDMH_UID(x)                         (((uint32_t)(((uint32_t)(x))<<FNET_MK_SIM_UIDMH_UID_SHIFT))&FNET_MK_SIM_UIDMH_UID_MASK)
/* UIDML Bit Fields */
#define FNET_MK_SIM_UIDML_UID_MASK                       0xFFFFFFFFu
#define FNET_MK_SIM_UIDML_UID_SHIFT                      0
#define FNET_MK_SIM_UIDML_UID(x)                         (((uint32_t)(((uint32_t)(x))<<FNET_MK_SIM_UIDML_UID_SHIFT))&FNET_MK_SIM_UIDML_UID_MASK)
/* UIDL Bit Fields */
#define FNET_MK_SIM_UIDL_UID_MASK                        0xFFFFFFFFu
#define FNET_MK_SIM_UIDL_UID_SHIFT                       0
#define FNET_MK_SIM_UIDL_UID(x)                          (((uint32_t)(((uint32_t)(x))<<FNET_MK_SIM_UIDL_UID_SHIFT))&FNET_MK_SIM_UIDL_UID_MASK)


/* SIM - Peripheral instance base addresses */
/* Peripheral SIM base pointer */
#define FNET_MK_SIM_BASE_PTR                             ((FNET_MK_SIM_MemMapPtr)0x40047000u)

/* ----------------------------------------------------------------------------
   -- SIM - Register accessor macros
   ---------------------------------------------------------------------------- */

/* SIM - Register instance definitions */
/* SIM */
#define FNET_MK_SIM_SOPT1                                FNET_MK_SIM_SOPT1_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_SOPT2                                FNET_MK_SIM_SOPT2_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_SOPT4                                FNET_MK_SIM_SOPT4_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_SOPT5                                FNET_MK_SIM_SOPT5_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_SOPT6                                FNET_MK_SIM_SOPT6_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_SOPT7                                FNET_MK_SIM_SOPT7_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_SDID                                 FNET_MK_SIM_SDID_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_SCGC1                                FNET_MK_SIM_SCGC1_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_SCGC2                                FNET_MK_SIM_SCGC2_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_SCGC3                                FNET_MK_SIM_SCGC3_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_SCGC4                                FNET_MK_SIM_SCGC4_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_SCGC5                                FNET_MK_SIM_SCGC5_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_SCGC6                                FNET_MK_SIM_SCGC6_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_SCGC7                                FNET_MK_SIM_SCGC7_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_CLKDIV1                              FNET_MK_SIM_CLKDIV1_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_CLKDIV2                              FNET_MK_SIM_CLKDIV2_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_FCFG1                                FNET_MK_SIM_FCFG1_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_FCFG2                                FNET_MK_SIM_FCFG2_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_UIDH                                 FNET_MK_SIM_UIDH_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_UIDMH                                FNET_MK_SIM_UIDMH_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_UIDML                                FNET_MK_SIM_UIDML_REG(FNET_MK_SIM_BASE_PTR)
#define FNET_MK_SIM_UIDL                                 FNET_MK_SIM_UIDL_REG(FNET_MK_SIM_BASE_PTR)

/* ----------------------------------------------------------------------------
   -- PORT
   ---------------------------------------------------------------------------- */

/* PORT - Peripheral register structure */
typedef struct FNET_MK_PORT_MemMap
{
    fnet_uint32_t PCR[32];                                /* Pin Control Register n, array offset: 0x0, array step: 0x4 */
    fnet_uint32_t GPCLR;                                  /* Global Pin Control Low Register, offset: 0x80 */
    fnet_uint32_t GPCHR;                                  /* Global Pin Control High Register, offset: 0x84 */
    fnet_uint8_t RESERVED_0[24];
    fnet_uint32_t ISFR;                                   /* Interrupt Status Flag Register, offset: 0xA0 */
    fnet_uint8_t RESERVED_1[28];
    fnet_uint32_t DFER;                                   /* Digital Filter Enable Register, offset: 0xC0 */
    fnet_uint32_t DFCR;                                   /* Digital Filter Clock Register, offset: 0xC4 */
    fnet_uint32_t DFWR;                                   /* Digital Filter Width Register, offset: 0xC8 */
} volatile *FNET_MK_PORT_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- PORT - Register accessor macros
   ---------------------------------------------------------------------------- */

/* PORT - Register accessors */
#define FNET_MK_PORT_PCR_REG(base,index)                 ((base)->PCR[(index)])
#define FNET_MK_PORT_GPCLR_REG(base)                     ((base)->GPCLR)
#define FNET_MK_PORT_GPCHR_REG(base)                     ((base)->GPCHR)
#define FNET_MK_PORT_ISFR_REG(base)                      ((base)->ISFR)
#define FNET_MK_PORT_DFER_REG(base)                      ((base)->DFER)
#define FNET_MK_PORT_DFCR_REG(base)                      ((base)->DFCR)
#define FNET_MK_PORT_DFWR_REG(base)                      ((base)->DFWR)

/* ----------------------------------------------------------------------------
   -- PORT Register Masks
   ---------------------------------------------------------------------------- */
/* PCR Bit Fields */
#define FNET_MK_PORT_PCR_PS_MASK                         0x1u
#define FNET_MK_PORT_PCR_PS_SHIFT                        0
#define FNET_MK_PORT_PCR_PE_MASK                         0x2u
#define FNET_MK_PORT_PCR_PE_SHIFT                        1
#define FNET_MK_PORT_PCR_SRE_MASK                        0x4u
#define FNET_MK_PORT_PCR_SRE_SHIFT                       2
#define FNET_MK_PORT_PCR_PFE_MASK                        0x10u
#define FNET_MK_PORT_PCR_PFE_SHIFT                       4
#define FNET_MK_PORT_PCR_ODE_MASK                        0x20u
#define FNET_MK_PORT_PCR_ODE_SHIFT                       5
#define FNET_MK_PORT_PCR_DSE_MASK                        0x40u
#define FNET_MK_PORT_PCR_DSE_SHIFT                       6
#define FNET_MK_PORT_PCR_MUX_MASK                        0x700u
#define FNET_MK_PORT_PCR_MUX_SHIFT                       8
#define FNET_MK_PORT_PCR_MUX(x)                          (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_PORT_PCR_MUX_SHIFT))&FNET_MK_PORT_PCR_MUX_MASK)
#define FNET_MK_PORT_PCR_LK_MASK                         0x8000u
#define FNET_MK_PORT_PCR_LK_SHIFT                        15
#define FNET_MK_PORT_PCR_IRQC_MASK                       0xF0000u
#define FNET_MK_PORT_PCR_IRQC_SHIFT                      16
#define FNET_MK_PORT_PCR_IRQC(x)                         (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_PORT_PCR_IRQC_SHIFT))&FNET_MK_PORT_PCR_IRQC_MASK)
#define FNET_MK_PORT_PCR_ISF_MASK                        0x1000000u
#define FNET_MK_PORT_PCR_ISF_SHIFT                       24
/* GPCLR Bit Fields */
#define FNET_MK_PORT_GPCLR_GPWD_MASK                     0xFFFFu
#define FNET_MK_PORT_GPCLR_GPWD_SHIFT                    0
#define FNET_MK_PORT_GPCLR_GPWD(x)                       (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_PORT_GPCLR_GPWD_SHIFT))&FNET_MK_PORT_GPCLR_GPWD_MASK)
#define FNET_MK_PORT_GPCLR_GPWE_MASK                     0xFFFF0000u
#define FNET_MK_PORT_GPCLR_GPWE_SHIFT                    16
#define FNET_MK_PORT_GPCLR_GPWE(x)                       (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_PORT_GPCLR_GPWE_SHIFT))&FNET_MK_PORT_GPCLR_GPWE_MASK)
/* GPCHR Bit Fields */
#define FNET_MK_PORT_GPCHR_GPWD_MASK                     0xFFFFu
#define FNET_MK_PORT_GPCHR_GPWD_SHIFT                    0
#define FNET_MK_PORT_GPCHR_GPWD(x)                       (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_PORT_GPCHR_GPWD_SHIFT))&FNET_MK_PORT_GPCHR_GPWD_MASK)
#define FNET_MK_PORT_GPCHR_GPWE_MASK                     0xFFFF0000u
#define FNET_MK_PORT_GPCHR_GPWE_SHIFT                    16
#define FNET_MK_PORT_GPCHR_GPWE(x)                       (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_PORT_GPCHR_GPWE_SHIFT))&FNET_MK_PORT_GPCHR_GPWE_MASK)
/* ISFR Bit Fields */
#define FNET_MK_PORT_ISFR_ISF_MASK                       0xFFFFFFFFu
#define FNET_MK_PORT_ISFR_ISF_SHIFT                      0
#define FNET_MK_PORT_ISFR_ISF(x)                         (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_PORT_ISFR_ISF_SHIFT))&FNET_MK_PORT_ISFR_ISF_MASK)
/* DFER Bit Fields */
#define FNET_MK_PORT_DFER_DFE_MASK                       0xFFFFFFFFu
#define FNET_MK_PORT_DFER_DFE_SHIFT                      0
#define FNET_MK_PORT_DFER_DFE(x)                         (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_PORT_DFER_DFE_SHIFT))&FNET_MK_PORT_DFER_DFE_MASK)
/* DFCR Bit Fields */
#define FNET_MK_PORT_DFCR_CS_MASK                        0x1u
#define FNET_MK_PORT_DFCR_CS_SHIFT                       0
/* DFWR Bit Fields */
#define FNET_MK_PORT_DFWR_FILT_MASK                      0x1Fu
#define FNET_MK_PORT_DFWR_FILT_SHIFT                     0
#define FNET_MK_PORT_DFWR_FILT(x)                        (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_PORT_DFWR_FILT_SHIFT))&FNET_MK_PORT_DFWR_FILT_MASK)


/* PORT - Peripheral instance base addresses */
/* Peripheral PORTA base pointer */
#define FNET_MK_PORTA_BASE_PTR                           ((FNET_MK_PORT_MemMapPtr)0x40049000u)
/* Peripheral PORTB base pointer */
#define FNET_MK_PORTB_BASE_PTR                           ((FNET_MK_PORT_MemMapPtr)0x4004A000u)
/* Peripheral PORTC base pointer */
#define FNET_MK_PORTC_BASE_PTR                           ((FNET_MK_PORT_MemMapPtr)0x4004B000u)
/* Peripheral PORTD base pointer */
#define FNET_MK_PORTD_BASE_PTR                           ((FNET_MK_PORT_MemMapPtr)0x4004C000u)
/* Peripheral PORTE base pointer */
#define FNET_MK_PORTE_BASE_PTR                           ((FNET_MK_PORT_MemMapPtr)0x4004D000u)
/* Peripheral PORTF base pointer */
#define FNET_MK_PORTF_BASE_PTR                           ((FNET_MK_PORT_MemMapPtr)0x4004E000u)

/* ----------------------------------------------------------------------------
   -- PORT - Register accessor macros
   ---------------------------------------------------------------------------- */

/* PORT - Register instance definitions */
/* PORTA */
#define FNET_MK_PORTA_ISFR                               FNET_MK_PORT_ISFR_REG(FNET_MK_PORTA_BASE_PTR)
#define FNET_MK_PORTA_PCR0                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,0)
#define FNET_MK_PORTA_PCR1                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,1)
#define FNET_MK_PORTA_PCR2                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,2)
#define FNET_MK_PORTA_PCR3                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,3)
#define FNET_MK_PORTA_PCR4                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,4)
#define FNET_MK_PORTA_PCR5                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,5)
#define FNET_MK_PORTA_PCR6                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,6)
#define FNET_MK_PORTA_PCR7                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,7)
#define FNET_MK_PORTA_PCR8                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,8)
#define FNET_MK_PORTA_PCR9                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,9)
#define FNET_MK_PORTA_PCR10                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,10)
#define FNET_MK_PORTA_PCR11                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,11)
#define FNET_MK_PORTA_PCR12                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,12)
#define FNET_MK_PORTA_PCR13                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,13)
#define FNET_MK_PORTA_PCR14                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,14)
#define FNET_MK_PORTA_PCR15                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,15)
#define FNET_MK_PORTA_PCR16                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,16)
#define FNET_MK_PORTA_PCR17                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,17)
#define FNET_MK_PORTA_PCR18                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,18)
#define FNET_MK_PORTA_PCR19                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,19)
#define FNET_MK_PORTA_PCR20                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,20)
#define FNET_MK_PORTA_PCR21                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,21)
#define FNET_MK_PORTA_PCR22                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,22)
#define FNET_MK_PORTA_PCR23                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,23)
#define FNET_MK_PORTA_PCR24                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,24)
#define FNET_MK_PORTA_PCR25                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,25)
#define FNET_MK_PORTA_PCR26                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,26)
#define FNET_MK_PORTA_PCR27                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,27)
#define FNET_MK_PORTA_PCR28                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,28)
#define FNET_MK_PORTA_PCR29                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,29)
#define FNET_MK_PORTA_PCR30                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,30)
#define FNET_MK_PORTA_PCR31                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,31)
#define FNET_MK_PORTA_GPCLR                              FNET_MK_PORT_GPCLR_REG(FNET_MK_PORTA_BASE_PTR)
#define FNET_MK_PORTA_GPCHR                              FNET_MK_PORT_GPCHR_REG(FNET_MK_PORTA_BASE_PTR)
#define FNET_MK_PORTA_DFER                               FNET_MK_PORT_DFER_REG(FNET_MK_PORTA_BASE_PTR)
#define FNET_MK_PORTA_DFCR                               FNET_MK_PORT_DFCR_REG(FNET_MK_PORTA_BASE_PTR)
#define FNET_MK_PORTA_DFWR                               FNET_MK_PORT_DFWR_REG(FNET_MK_PORTA_BASE_PTR)
/* PORTB */
#define FNET_MK_PORTB_ISFR                               FNET_MK_PORT_ISFR_REG(FNET_MK_PORTB_BASE_PTR)
#define FNET_MK_PORTB_PCR0                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,0)
#define FNET_MK_PORTB_PCR1                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,1)
#define FNET_MK_PORTB_PCR2                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,2)
#define FNET_MK_PORTB_PCR3                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,3)
#define FNET_MK_PORTB_PCR4                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,4)
#define FNET_MK_PORTB_PCR5                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,5)
#define FNET_MK_PORTB_PCR6                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,6)
#define FNET_MK_PORTB_PCR7                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,7)
#define FNET_MK_PORTB_PCR8                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,8)
#define FNET_MK_PORTB_PCR9                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,9)
#define FNET_MK_PORTB_PCR10                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,10)
#define FNET_MK_PORTB_PCR11                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,11)
#define FNET_MK_PORTB_PCR12                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,12)
#define FNET_MK_PORTB_PCR13                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,13)
#define FNET_MK_PORTB_PCR14                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,14)
#define FNET_MK_PORTB_PCR15                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,15)
#define FNET_MK_PORTB_PCR16                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,16)
#define FNET_MK_PORTB_PCR17                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,17)
#define FNET_MK_PORTB_PCR18                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,18)
#define FNET_MK_PORTB_PCR19                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,19)
#define FNET_MK_PORTB_PCR20                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,20)
#define FNET_MK_PORTB_PCR21                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,21)
#define FNET_MK_PORTB_PCR22                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,22)
#define FNET_MK_PORTB_PCR23                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,23)
#define FNET_MK_PORTB_PCR24                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,24)
#define FNET_MK_PORTB_PCR25                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,25)
#define FNET_MK_PORTB_PCR26                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,26)
#define FNET_MK_PORTB_PCR27                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,27)
#define FNET_MK_PORTB_PCR28                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,28)
#define FNET_MK_PORTB_PCR29                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,29)
#define FNET_MK_PORTB_PCR30                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,30)
#define FNET_MK_PORTB_PCR31                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,31)
#define FNET_MK_PORTB_GPCLR                              FNET_MK_PORT_GPCLR_REG(FNET_MK_PORTB_BASE_PTR)
#define FNET_MK_PORTB_GPCHR                              FNET_MK_PORT_GPCHR_REG(FNET_MK_PORTB_BASE_PTR)
#define FNET_MK_PORTB_DFER                               FNET_MK_PORT_DFER_REG(FNET_MK_PORTB_BASE_PTR)
#define FNET_MK_PORTB_DFCR                               FNET_MK_PORT_DFCR_REG(FNET_MK_PORTB_BASE_PTR)
#define FNET_MK_PORTB_DFWR                               FNET_MK_PORT_DFWR_REG(FNET_MK_PORTB_BASE_PTR)
/* PORTC */
#define FNET_MK_PORTC_ISFR                               FNET_MK_PORT_ISFR_REG(FNET_MK_PORTC_BASE_PTR)
#define FNET_MK_PORTC_PCR0                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,0)
#define FNET_MK_PORTC_PCR1                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,1)
#define FNET_MK_PORTC_PCR2                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,2)
#define FNET_MK_PORTC_PCR3                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,3)
#define FNET_MK_PORTC_PCR4                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,4)
#define FNET_MK_PORTC_PCR5                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,5)
#define FNET_MK_PORTC_PCR6                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,6)
#define FNET_MK_PORTC_PCR7                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,7)
#define FNET_MK_PORTC_PCR8                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,8)
#define FNET_MK_PORTC_PCR9                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,9)
#define FNET_MK_PORTC_PCR10                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,10)
#define FNET_MK_PORTC_PCR11                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,11)
#define FNET_MK_PORTC_PCR12                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,12)
#define FNET_MK_PORTC_PCR13                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,13)
#define FNET_MK_PORTC_PCR14                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,14)
#define FNET_MK_PORTC_PCR15                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,15)
#define FNET_MK_PORTC_PCR16                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,16)
#define FNET_MK_PORTC_PCR17                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,17)
#define FNET_MK_PORTC_PCR18                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,18)
#define FNET_MK_PORTC_PCR19                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,19)
#define FNET_MK_PORTC_PCR20                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,20)
#define FNET_MK_PORTC_PCR21                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,21)
#define FNET_MK_PORTC_PCR22                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,22)
#define FNET_MK_PORTC_PCR23                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,23)
#define FNET_MK_PORTC_PCR24                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,24)
#define FNET_MK_PORTC_PCR25                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,25)
#define FNET_MK_PORTC_PCR26                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,26)
#define FNET_MK_PORTC_PCR27                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,27)
#define FNET_MK_PORTC_PCR28                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,28)
#define FNET_MK_PORTC_PCR29                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,29)
#define FNET_MK_PORTC_PCR30                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,30)
#define FNET_MK_PORTC_PCR31                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,31)
#define FNET_MK_PORTC_GPCLR                              FNET_MK_PORT_GPCLR_REG(FNET_MK_PORTC_BASE_PTR)
#define FNET_MK_PORTC_GPCHR                              FNET_MK_PORT_GPCHR_REG(FNET_MK_PORTC_BASE_PTR)
#define FNET_MK_PORTC_DFER                               FNET_MK_PORT_DFER_REG(FNET_MK_PORTC_BASE_PTR)
#define FNET_MK_PORTC_DFCR                               FNET_MK_PORT_DFCR_REG(FNET_MK_PORTC_BASE_PTR)
#define FNET_MK_PORTC_DFWR                               FNET_MK_PORT_DFWR_REG(FNET_MK_PORTC_BASE_PTR)
/* PORTD */
#define FNET_MK_PORTD_ISFR                               FNET_MK_PORT_ISFR_REG(FNET_MK_PORTD_BASE_PTR)
#define FNET_MK_PORTD_PCR0                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,0)
#define FNET_MK_PORTD_PCR1                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,1)
#define FNET_MK_PORTD_PCR2                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,2)
#define FNET_MK_PORTD_PCR3                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,3)
#define FNET_MK_PORTD_PCR4                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,4)
#define FNET_MK_PORTD_PCR5                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,5)
#define FNET_MK_PORTD_PCR6                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,6)
#define FNET_MK_PORTD_PCR7                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,7)
#define FNET_MK_PORTD_PCR8                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,8)
#define FNET_MK_PORTD_PCR9                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,9)
#define FNET_MK_PORTD_PCR10                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,10)
#define FNET_MK_PORTD_PCR11                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,11)
#define FNET_MK_PORTD_PCR12                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,12)
#define FNET_MK_PORTD_PCR13                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,13)
#define FNET_MK_PORTD_PCR14                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,14)
#define FNET_MK_PORTD_PCR15                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,15)
#define FNET_MK_PORTD_PCR16                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,16)
#define FNET_MK_PORTD_PCR17                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,17)
#define FNET_MK_PORTD_PCR18                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,18)
#define FNET_MK_PORTD_PCR19                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,19)
#define FNET_MK_PORTD_PCR20                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,20)
#define FNET_MK_PORTD_PCR21                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,21)
#define FNET_MK_PORTD_PCR22                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,22)
#define FNET_MK_PORTD_PCR23                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,23)
#define FNET_MK_PORTD_PCR24                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,24)
#define FNET_MK_PORTD_PCR25                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,25)
#define FNET_MK_PORTD_PCR26                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,26)
#define FNET_MK_PORTD_PCR27                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,27)
#define FNET_MK_PORTD_PCR28                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,28)
#define FNET_MK_PORTD_PCR29                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,29)
#define FNET_MK_PORTD_PCR30                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,30)
#define FNET_MK_PORTD_PCR31                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,31)
#define FNET_MK_PORTD_GPCLR                              FNET_MK_PORT_GPCLR_REG(FNET_MK_PORTD_BASE_PTR)
#define FNET_MK_PORTD_GPCHR                              FNET_MK_PORT_GPCHR_REG(FNET_MK_PORTD_BASE_PTR)
#define FNET_MK_PORTD_DFER                               FNET_MK_PORT_DFER_REG(FNET_MK_PORTD_BASE_PTR)
#define FNET_MK_PORTD_DFCR                               FNET_MK_PORT_DFCR_REG(FNET_MK_PORTD_BASE_PTR)
#define FNET_MK_PORTD_DFWR                               FNET_MK_PORT_DFWR_REG(FNET_MK_PORTD_BASE_PTR)
/* PORTE */
#define FNET_MK_PORTE_ISFR                               FNET_MK_PORT_ISFR_REG(FNET_MK_PORTE_BASE_PTR)
#define FNET_MK_PORTE_PCR0                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,0)
#define FNET_MK_PORTE_PCR1                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,1)
#define FNET_MK_PORTE_PCR2                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,2)
#define FNET_MK_PORTE_PCR3                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,3)
#define FNET_MK_PORTE_PCR4                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,4)
#define FNET_MK_PORTE_PCR5                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,5)
#define FNET_MK_PORTE_PCR6                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,6)
#define FNET_MK_PORTE_PCR7                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,7)
#define FNET_MK_PORTE_PCR8                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,8)
#define FNET_MK_PORTE_PCR9                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,9)
#define FNET_MK_PORTE_PCR10                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,10)
#define FNET_MK_PORTE_PCR11                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,11)
#define FNET_MK_PORTE_PCR12                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,12)
#define FNET_MK_PORTE_PCR13                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,13)
#define FNET_MK_PORTE_PCR14                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,14)
#define FNET_MK_PORTE_PCR15                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,15)
#define FNET_MK_PORTE_PCR16                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,16)
#define FNET_MK_PORTE_PCR17                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,17)
#define FNET_MK_PORTE_PCR18                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,18)
#define FNET_MK_PORTE_PCR19                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,19)
#define FNET_MK_PORTE_PCR20                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,20)
#define FNET_MK_PORTE_PCR21                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,21)
#define FNET_MK_PORTE_PCR22                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,22)
#define FNET_MK_PORTE_PCR23                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,23)
#define FNET_MK_PORTE_PCR24                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,24)
#define FNET_MK_PORTE_PCR25                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,25)
#define FNET_MK_PORTE_PCR26                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,26)
#define FNET_MK_PORTE_PCR27                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,27)
#define FNET_MK_PORTE_PCR28                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,28)
#define FNET_MK_PORTE_PCR29                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,29)
#define FNET_MK_PORTE_PCR30                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,30)
#define FNET_MK_PORTE_PCR31                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,31)
#define FNET_MK_PORTE_GPCLR                              FNET_MK_PORT_GPCLR_REG(FNET_MK_PORTE_BASE_PTR)
#define FNET_MK_PORTE_GPCHR                              FNET_MK_PORT_GPCHR_REG(FNET_MK_PORTE_BASE_PTR)
#define FNET_MK_PORTE_DFER                               FNET_MK_PORT_DFER_REG(FNET_MK_PORTE_BASE_PTR)
#define FNET_MK_PORTE_DFCR                               FNET_MK_PORT_DFCR_REG(FNET_MK_PORTE_BASE_PTR)
#define FNET_MK_PORTE_DFWR                               FNET_MK_PORT_DFWR_REG(FNET_MK_PORTE_BASE_PTR)
/* PORTF */
#define FNET_MK_PORTF_PCR0                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,0)
#define FNET_MK_PORTF_PCR1                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,1)
#define FNET_MK_PORTF_PCR2                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,2)
#define FNET_MK_PORTF_PCR3                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,3)
#define FNET_MK_PORTF_PCR4                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,4)
#define FNET_MK_PORTF_PCR5                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,5)
#define FNET_MK_PORTF_PCR6                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,6)
#define FNET_MK_PORTF_PCR7                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,7)
#define FNET_MK_PORTF_PCR8                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,8)
#define FNET_MK_PORTF_PCR9                               FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,9)
#define FNET_MK_PORTF_PCR10                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,10)
#define FNET_MK_PORTF_PCR11                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,11)
#define FNET_MK_PORTF_PCR12                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,12)
#define FNET_MK_PORTF_PCR13                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,13)
#define FNET_MK_PORTF_PCR14                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,14)
#define FNET_MK_PORTF_PCR15                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,15)
#define FNET_MK_PORTF_PCR16                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,16)
#define FNET_MK_PORTF_PCR17                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,17)
#define FNET_MK_PORTF_PCR18                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,18)
#define FNET_MK_PORTF_PCR19                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,19)
#define FNET_MK_PORTF_PCR20                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,20)
#define FNET_MK_PORTF_PCR21                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,21)
#define FNET_MK_PORTF_PCR22                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,22)
#define FNET_MK_PORTF_PCR23                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,23)
#define FNET_MK_PORTF_PCR24                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,24)
#define FNET_MK_PORTF_PCR25                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,25)
#define FNET_MK_PORTF_PCR26                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,26)
#define FNET_MK_PORTF_PCR27                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,27)
#define FNET_MK_PORTF_PCR28                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,28)
#define FNET_MK_PORTF_PCR29                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,29)
#define FNET_MK_PORTF_PCR30                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,30)
#define FNET_MK_PORTF_PCR31                              FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,31)
#define FNET_MK_PORTF_GPCLR                              FNET_MK_PORT_GPCLR_REG(FNET_MK_PORTF_BASE_PTR)
#define FNET_MK_PORTF_GPCHR                              FNET_MK_PORT_GPCHR_REG(FNET_MK_PORTF_BASE_PTR)
#define FNET_MK_PORTF_ISFR                               FNET_MK_PORT_ISFR_REG(FNET_MK_PORTF_BASE_PTR)
#define FNET_MK_PORTF_DFER                               FNET_MK_PORT_DFER_REG(FNET_MK_PORTF_BASE_PTR)
#define FNET_MK_PORTF_DFCR                               FNET_MK_PORT_DFCR_REG(FNET_MK_PORTF_BASE_PTR)
#define FNET_MK_PORTF_DFWR                               FNET_MK_PORT_DFWR_REG(FNET_MK_PORTF_BASE_PTR)
/* PORT - Register array accessors */
#define FNET_MK_PORTA_PCR(index)                         FNET_MK_PORT_PCR_REG(FNET_MK_PORTA_BASE_PTR,(index))
#define FNET_MK_PORTB_PCR(index)                         FNET_MK_PORT_PCR_REG(FNET_MK_PORTB_BASE_PTR,(index))
#define FNET_MK_PORTC_PCR(index)                         FNET_MK_PORT_PCR_REG(FNET_MK_PORTC_BASE_PTR,(index))
#define FNET_MK_PORTD_PCR(index)                         FNET_MK_PORT_PCR_REG(FNET_MK_PORTD_BASE_PTR,(index))
#define FNET_MK_PORTE_PCR(index)                         FNET_MK_PORT_PCR_REG(FNET_MK_PORTE_BASE_PTR,(index))
#define FNET_MK_PORTF_PCR(index)                         FNET_MK_PORT_PCR_REG(FNET_MK_PORTF_BASE_PTR,(index))

/* ----------------------------------------------------------------------------
   -- NVIC
   ---------------------------------------------------------------------------- */

/* NVIC - Peripheral register structure */
typedef struct FNET_MK_NVIC_MemMap
{
    fnet_uint32_t ISER[4];                                /* Interrupt Set Enable Register n, array offset: 0x0, array step: 0x4 */
    fnet_uint8_t RESERVED_0[112];
    fnet_uint32_t ICER[4];                                /* Interrupt Clear Enable Register n, array offset: 0x80, array step: 0x4 */
    fnet_uint8_t RESERVED_1[112];
    fnet_uint32_t ISPR[4];                                /* Interrupt Set Pending Register n, array offset: 0x100, array step: 0x4 */
    fnet_uint8_t RESERVED_2[112];
    fnet_uint32_t ICPR[4];                                /* Interrupt Clear Pending Register n, array offset: 0x180, array step: 0x4 */
    fnet_uint8_t RESERVED_3[112];
    fnet_uint32_t IABR[4];                                /* Interrupt Active bit Register n, array offset: 0x200, array step: 0x4 */
    fnet_uint8_t RESERVED_4[240];
    fnet_uint8_t IP[104];                                 /* Interrupt Priority Register n, array offset: 0x300, array step: 0x1 */
    fnet_uint8_t RESERVED_5[2712];
    fnet_uint32_t STIR[1];                                /* Software Trigger Interrupt Register, array offset: 0xE00, array step: 0x4 */
} volatile *FNET_MK_NVIC_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- NVIC - Register accessor macros
   ---------------------------------------------------------------------------- */

/* NVIC - Register accessors */
#define FNET_MK_NVIC_ISER_REG(base,index)                ((base)->ISER[(index)])
#define FNET_MK_NVIC_ICER_REG(base,index)                ((base)->ICER[(index)])
#define FNET_MK_NVIC_ISPR_REG(base,index)                ((base)->ISPR[(index)])
#define FNET_MK_NVIC_ICPR_REG(base,index)                ((base)->ICPR[(index)])
#define FNET_MK_NVIC_IABR_REG(base,index)                ((base)->IABR[(index)])
#define FNET_MK_NVIC_IP_REG(base,index)                  ((base)->IP[(index)])
#define FNET_MK_NVIC_STIR_REG(base,index)                ((base)->STIR[(index)])

/* ----------------------------------------------------------------------------
   -- NVIC Register Masks
   ---------------------------------------------------------------------------- */

/* ISER Bit Fields */
#define FNET_MK_NVIC_ISER_SETENA_MASK                    0xFFFFFFFFu
#define FNET_MK_NVIC_ISER_SETENA_SHIFT                   0
#define FNET_MK_NVIC_ISER_SETENA(x)                      (((uint32_t)(((uint32_t)(x))<<FNET_MK_NVIC_ISER_SETENA_SHIFT))&FNET_MK_NVIC_ISER_SETENA_MASK)
/* ICER Bit Fields */
#define FNET_MK_NVIC_ICER_CLRENA_MASK                    0xFFFFFFFFu
#define FNET_MK_NVIC_ICER_CLRENA_SHIFT                   0
#define FNET_MK_NVIC_ICER_CLRENA(x)                      (((uint32_t)(((uint32_t)(x))<<FNET_MK_NVIC_ICER_CLRENA_SHIFT))&FNET_MK_NVIC_ICER_CLRENA_MASK)
/* ISPR Bit Fields */
#define FNET_MK_NVIC_ISPR_SETPEND_MASK                   0xFFFFFFFFu
#define FNET_MK_NVIC_ISPR_SETPEND_SHIFT                  0
#define FNET_MK_NVIC_ISPR_SETPEND(x)                     (((uint32_t)(((uint32_t)(x))<<FNET_MK_NVIC_ISPR_SETPEND_SHIFT))&FNET_MK_NVIC_ISPR_SETPEND_MASK)
/* ICPR Bit Fields */
#define FNET_MK_NVIC_ICPR_CLRPEND_MASK                   0xFFFFFFFFu
#define FNET_MK_NVIC_ICPR_CLRPEND_SHIFT                  0
#define FNET_MK_NVIC_ICPR_CLRPEND(x)                     (((uint32_t)(((uint32_t)(x))<<FNET_MK_NVIC_ICPR_CLRPEND_SHIFT))&FNET_MK_NVIC_ICPR_CLRPEND_MASK)
/* IABR Bit Fields */
#define FNET_MK_NVIC_IABR_ACTIVE_MASK                    0xFFFFFFFFu
#define FNET_MK_NVIC_IABR_ACTIVE_SHIFT                   0
#define FNET_MK_NVIC_IABR_ACTIVE(x)                      (((uint32_t)(((uint32_t)(x))<<FNET_MK_NVIC_IABR_ACTIVE_SHIFT))&FNET_MK_NVIC_IABR_ACTIVE_MASK)
/* IP Bit Fields */
#define FNET_MK_NVIC_IP_PRI0_MASK                        0xFFu
#define FNET_MK_NVIC_IP_PRI0_SHIFT                       0
#define FNET_MK_NVIC_IP_PRI0(x)                          (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI0_SHIFT))&FNET_MK_NVIC_IP_PRI0_MASK)
#define FNET_MK_NVIC_IP_PRI1_MASK                        0xFFu
#define FNET_MK_NVIC_IP_PRI1_SHIFT                       0
#define FNET_MK_NVIC_IP_PRI1(x)                          (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI1_SHIFT))&FNET_MK_NVIC_IP_PRI1_MASK)
#define FNET_MK_NVIC_IP_PRI2_MASK                        0xFFu
#define FNET_MK_NVIC_IP_PRI2_SHIFT                       0
#define FNET_MK_NVIC_IP_PRI2(x)                          (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI2_SHIFT))&FNET_MK_NVIC_IP_PRI2_MASK)
#define FNET_MK_NVIC_IP_PRI3_MASK                        0xFFu
#define FNET_MK_NVIC_IP_PRI3_SHIFT                       0
#define FNET_MK_NVIC_IP_PRI3(x)                          (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI3_SHIFT))&FNET_MK_NVIC_IP_PRI3_MASK)
#define FNET_MK_NVIC_IP_PRI4_MASK                        0xFFu
#define FNET_MK_NVIC_IP_PRI4_SHIFT                       0
#define FNET_MK_NVIC_IP_PRI4(x)                          (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI4_SHIFT))&FNET_MK_NVIC_IP_PRI4_MASK)
#define FNET_MK_NVIC_IP_PRI5_MASK                        0xFFu
#define FNET_MK_NVIC_IP_PRI5_SHIFT                       0
#define FNET_MK_NVIC_IP_PRI5(x)                          (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI5_SHIFT))&FNET_MK_NVIC_IP_PRI5_MASK)
#define FNET_MK_NVIC_IP_PRI6_MASK                        0xFFu
#define FNET_MK_NVIC_IP_PRI6_SHIFT                       0
#define FNET_MK_NVIC_IP_PRI6(x)                          (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI6_SHIFT))&FNET_MK_NVIC_IP_PRI6_MASK)
#define FNET_MK_NVIC_IP_PRI7_MASK                        0xFFu
#define FNET_MK_NVIC_IP_PRI7_SHIFT                       0
#define FNET_MK_NVIC_IP_PRI7(x)                          (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI7_SHIFT))&FNET_MK_NVIC_IP_PRI7_MASK)
#define FNET_MK_NVIC_IP_PRI8_MASK                        0xFFu
#define FNET_MK_NVIC_IP_PRI8_SHIFT                       0
#define FNET_MK_NVIC_IP_PRI8(x)                          (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI8_SHIFT))&FNET_MK_NVIC_IP_PRI8_MASK)
#define FNET_MK_NVIC_IP_PRI9_MASK                        0xFFu
#define FNET_MK_NVIC_IP_PRI9_SHIFT                       0
#define FNET_MK_NVIC_IP_PRI9(x)                          (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI9_SHIFT))&FNET_MK_NVIC_IP_PRI9_MASK)
#define FNET_MK_NVIC_IP_PRI10_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI10_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI10(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI10_SHIFT))&FNET_MK_NVIC_IP_PRI10_MASK)
#define FNET_MK_NVIC_IP_PRI11_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI11_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI11(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI11_SHIFT))&FNET_MK_NVIC_IP_PRI11_MASK)
#define FNET_MK_NVIC_IP_PRI12_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI12_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI12(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI12_SHIFT))&FNET_MK_NVIC_IP_PRI12_MASK)
#define FNET_MK_NVIC_IP_PRI13_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI13_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI13(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI13_SHIFT))&FNET_MK_NVIC_IP_PRI13_MASK)
#define FNET_MK_NVIC_IP_PRI14_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI14_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI14(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI14_SHIFT))&FNET_MK_NVIC_IP_PRI14_MASK)
#define FNET_MK_NVIC_IP_PRI15_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI15_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI15(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI15_SHIFT))&FNET_MK_NVIC_IP_PRI15_MASK)
#define FNET_MK_NVIC_IP_PRI16_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI16_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI16(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI16_SHIFT))&FNET_MK_NVIC_IP_PRI16_MASK)
#define FNET_MK_NVIC_IP_PRI17_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI17_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI17(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI17_SHIFT))&FNET_MK_NVIC_IP_PRI17_MASK)
#define FNET_MK_NVIC_IP_PRI18_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI18_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI18(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI18_SHIFT))&FNET_MK_NVIC_IP_PRI18_MASK)
#define FNET_MK_NVIC_IP_PRI19_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI19_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI19(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI19_SHIFT))&FNET_MK_NVIC_IP_PRI19_MASK)
#define FNET_MK_NVIC_IP_PRI20_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI20_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI20(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI20_SHIFT))&FNET_MK_NVIC_IP_PRI20_MASK)
#define FNET_MK_NVIC_IP_PRI21_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI21_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI21(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI21_SHIFT))&FNET_MK_NVIC_IP_PRI21_MASK)
#define FNET_MK_NVIC_IP_PRI22_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI22_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI22(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI22_SHIFT))&FNET_MK_NVIC_IP_PRI22_MASK)
#define FNET_MK_NVIC_IP_PRI23_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI23_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI23(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI23_SHIFT))&FNET_MK_NVIC_IP_PRI23_MASK)
#define FNET_MK_NVIC_IP_PRI24_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI24_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI24(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI24_SHIFT))&FNET_MK_NVIC_IP_PRI24_MASK)
#define FNET_MK_NVIC_IP_PRI25_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI25_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI25(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI25_SHIFT))&FNET_MK_NVIC_IP_PRI25_MASK)
#define FNET_MK_NVIC_IP_PRI26_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI26_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI26(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI26_SHIFT))&FNET_MK_NVIC_IP_PRI26_MASK)
#define FNET_MK_NVIC_IP_PRI27_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI27_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI27(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI27_SHIFT))&FNET_MK_NVIC_IP_PRI27_MASK)
#define FNET_MK_NVIC_IP_PRI28_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI28_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI28(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI28_SHIFT))&FNET_MK_NVIC_IP_PRI28_MASK)
#define FNET_MK_NVIC_IP_PRI29_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI29_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI29(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI29_SHIFT))&FNET_MK_NVIC_IP_PRI29_MASK)
#define FNET_MK_NVIC_IP_PRI30_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI30_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI30(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI30_SHIFT))&FNET_MK_NVIC_IP_PRI30_MASK)
#define FNET_MK_NVIC_IP_PRI31_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI31_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI31(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI31_SHIFT))&FNET_MK_NVIC_IP_PRI31_MASK)
#define FNET_MK_NVIC_IP_PRI32_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI32_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI32(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI32_SHIFT))&FNET_MK_NVIC_IP_PRI32_MASK)
#define FNET_MK_NVIC_IP_PRI33_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI33_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI33(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI33_SHIFT))&FNET_MK_NVIC_IP_PRI33_MASK)
#define FNET_MK_NVIC_IP_PRI34_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI34_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI34(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI34_SHIFT))&FNET_MK_NVIC_IP_PRI34_MASK)
#define FNET_MK_NVIC_IP_PRI35_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI35_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI35(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI35_SHIFT))&FNET_MK_NVIC_IP_PRI35_MASK)
#define FNET_MK_NVIC_IP_PRI36_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI36_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI36(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI36_SHIFT))&FNET_MK_NVIC_IP_PRI36_MASK)
#define FNET_MK_NVIC_IP_PRI37_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI37_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI37(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI37_SHIFT))&FNET_MK_NVIC_IP_PRI37_MASK)
#define FNET_MK_NVIC_IP_PRI38_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI38_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI38(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI38_SHIFT))&FNET_MK_NVIC_IP_PRI38_MASK)
#define FNET_MK_NVIC_IP_PRI39_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI39_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI39(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI39_SHIFT))&FNET_MK_NVIC_IP_PRI39_MASK)
#define FNET_MK_NVIC_IP_PRI40_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI40_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI40(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI40_SHIFT))&FNET_MK_NVIC_IP_PRI40_MASK)
#define FNET_MK_NVIC_IP_PRI41_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI41_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI41(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI41_SHIFT))&FNET_MK_NVIC_IP_PRI41_MASK)
#define FNET_MK_NVIC_IP_PRI42_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI42_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI42(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI42_SHIFT))&FNET_MK_NVIC_IP_PRI42_MASK)
#define FNET_MK_NVIC_IP_PRI43_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI43_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI43(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI43_SHIFT))&FNET_MK_NVIC_IP_PRI43_MASK)
#define FNET_MK_NVIC_IP_PRI44_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI44_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI44(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI44_SHIFT))&FNET_MK_NVIC_IP_PRI44_MASK)
#define FNET_MK_NVIC_IP_PRI45_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI45_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI45(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI45_SHIFT))&FNET_MK_NVIC_IP_PRI45_MASK)
#define FNET_MK_NVIC_IP_PRI46_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI46_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI46(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI46_SHIFT))&FNET_MK_NVIC_IP_PRI46_MASK)
#define FNET_MK_NVIC_IP_PRI47_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI47_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI47(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI47_SHIFT))&FNET_MK_NVIC_IP_PRI47_MASK)
#define FNET_MK_NVIC_IP_PRI48_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI48_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI48(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI48_SHIFT))&FNET_MK_NVIC_IP_PRI48_MASK)
#define FNET_MK_NVIC_IP_PRI49_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI49_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI49(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI49_SHIFT))&FNET_MK_NVIC_IP_PRI49_MASK)
#define FNET_MK_NVIC_IP_PRI50_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI50_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI50(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI50_SHIFT))&FNET_MK_NVIC_IP_PRI50_MASK)
#define FNET_MK_NVIC_IP_PRI51_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI51_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI51(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI51_SHIFT))&FNET_MK_NVIC_IP_PRI51_MASK)
#define FNET_MK_NVIC_IP_PRI52_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI52_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI52(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI52_SHIFT))&FNET_MK_NVIC_IP_PRI52_MASK)
#define FNET_MK_NVIC_IP_PRI53_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI53_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI53(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI53_SHIFT))&FNET_MK_NVIC_IP_PRI53_MASK)
#define FNET_MK_NVIC_IP_PRI54_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI54_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI54(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI54_SHIFT))&FNET_MK_NVIC_IP_PRI54_MASK)
#define FNET_MK_NVIC_IP_PRI55_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI55_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI55(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI55_SHIFT))&FNET_MK_NVIC_IP_PRI55_MASK)
#define FNET_MK_NVIC_IP_PRI56_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI56_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI56(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI56_SHIFT))&FNET_MK_NVIC_IP_PRI56_MASK)
#define FNET_MK_NVIC_IP_PRI57_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI57_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI57(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI57_SHIFT))&FNET_MK_NVIC_IP_PRI57_MASK)
#define FNET_MK_NVIC_IP_PRI58_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI58_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI58(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI58_SHIFT))&FNET_MK_NVIC_IP_PRI58_MASK)
#define FNET_MK_NVIC_IP_PRI59_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI59_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI59(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI59_SHIFT))&FNET_MK_NVIC_IP_PRI59_MASK)
#define FNET_MK_NVIC_IP_PRI60_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI60_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI60(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI60_SHIFT))&FNET_MK_NVIC_IP_PRI60_MASK)
#define FNET_MK_NVIC_IP_PRI61_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI61_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI61(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI61_SHIFT))&FNET_MK_NVIC_IP_PRI61_MASK)
#define FNET_MK_NVIC_IP_PRI62_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI62_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI62(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI62_SHIFT))&FNET_MK_NVIC_IP_PRI62_MASK)
#define FNET_MK_NVIC_IP_PRI63_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI63_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI63(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI63_SHIFT))&FNET_MK_NVIC_IP_PRI63_MASK)
#define FNET_MK_NVIC_IP_PRI64_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI64_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI64(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI64_SHIFT))&FNET_MK_NVIC_IP_PRI64_MASK)
#define FNET_MK_NVIC_IP_PRI65_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI65_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI65(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI65_SHIFT))&FNET_MK_NVIC_IP_PRI65_MASK)
#define FNET_MK_NVIC_IP_PRI66_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI66_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI66(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI66_SHIFT))&FNET_MK_NVIC_IP_PRI66_MASK)
#define FNET_MK_NVIC_IP_PRI67_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI67_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI67(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI67_SHIFT))&FNET_MK_NVIC_IP_PRI67_MASK)
#define FNET_MK_NVIC_IP_PRI68_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI68_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI68(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI68_SHIFT))&FNET_MK_NVIC_IP_PRI68_MASK)
#define FNET_MK_NVIC_IP_PRI69_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI69_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI69(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI69_SHIFT))&FNET_MK_NVIC_IP_PRI69_MASK)
#define FNET_MK_NVIC_IP_PRI70_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI70_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI70(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI70_SHIFT))&FNET_MK_NVIC_IP_PRI70_MASK)
#define FNET_MK_NVIC_IP_PRI71_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI71_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI71(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI71_SHIFT))&FNET_MK_NVIC_IP_PRI71_MASK)
#define FNET_MK_NVIC_IP_PRI72_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI72_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI72(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI72_SHIFT))&FNET_MK_NVIC_IP_PRI72_MASK)
#define FNET_MK_NVIC_IP_PRI73_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI73_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI73(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI73_SHIFT))&FNET_MK_NVIC_IP_PRI73_MASK)
#define FNET_MK_NVIC_IP_PRI74_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI74_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI74(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI74_SHIFT))&FNET_MK_NVIC_IP_PRI74_MASK)
#define FNET_MK_NVIC_IP_PRI75_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI75_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI75(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI75_SHIFT))&FNET_MK_NVIC_IP_PRI75_MASK)
#define FNET_MK_NVIC_IP_PRI76_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI76_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI76(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI76_SHIFT))&FNET_MK_NVIC_IP_PRI76_MASK)
#define FNET_MK_NVIC_IP_PRI77_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI77_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI77(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI77_SHIFT))&FNET_MK_NVIC_IP_PRI77_MASK)
#define FNET_MK_NVIC_IP_PRI78_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI78_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI78(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI78_SHIFT))&FNET_MK_NVIC_IP_PRI78_MASK)
#define FNET_MK_NVIC_IP_PRI79_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI79_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI79(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI79_SHIFT))&FNET_MK_NVIC_IP_PRI79_MASK)
#define FNET_MK_NVIC_IP_PRI80_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI80_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI80(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI80_SHIFT))&FNET_MK_NVIC_IP_PRI80_MASK)
#define FNET_MK_NVIC_IP_PRI81_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI81_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI81(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI81_SHIFT))&FNET_MK_NVIC_IP_PRI81_MASK)
#define FNET_MK_NVIC_IP_PRI82_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI82_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI82(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI82_SHIFT))&FNET_MK_NVIC_IP_PRI82_MASK)
#define FNET_MK_NVIC_IP_PRI83_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI83_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI83(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI83_SHIFT))&FNET_MK_NVIC_IP_PRI83_MASK)
#define FNET_MK_NVIC_IP_PRI84_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI84_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI84(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI84_SHIFT))&FNET_MK_NVIC_IP_PRI84_MASK)
#define FNET_MK_NVIC_IP_PRI85_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI85_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI85(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI85_SHIFT))&FNET_MK_NVIC_IP_PRI85_MASK)
#define FNET_MK_NVIC_IP_PRI86_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI86_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI86(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI86_SHIFT))&FNET_MK_NVIC_IP_PRI86_MASK)
#define FNET_MK_NVIC_IP_PRI87_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI87_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI87(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI87_SHIFT))&FNET_MK_NVIC_IP_PRI87_MASK)
#define FNET_MK_NVIC_IP_PRI88_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI88_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI88(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI88_SHIFT))&FNET_MK_NVIC_IP_PRI88_MASK)
#define FNET_MK_NVIC_IP_PRI89_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI89_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI89(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI89_SHIFT))&FNET_MK_NVIC_IP_PRI89_MASK)
#define FNET_MK_NVIC_IP_PRI90_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI90_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI90(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI90_SHIFT))&FNET_MK_NVIC_IP_PRI90_MASK)
#define FNET_MK_NVIC_IP_PRI91_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI91_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI91(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI91_SHIFT))&FNET_MK_NVIC_IP_PRI91_MASK)
#define FNET_MK_NVIC_IP_PRI92_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI92_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI92(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI92_SHIFT))&FNET_MK_NVIC_IP_PRI92_MASK)
#define FNET_MK_NVIC_IP_PRI93_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI93_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI93(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI93_SHIFT))&FNET_MK_NVIC_IP_PRI93_MASK)
#define FNET_MK_NVIC_IP_PRI94_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI94_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI94(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI94_SHIFT))&FNET_MK_NVIC_IP_PRI94_MASK)
#define FNET_MK_NVIC_IP_PRI95_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI95_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI95(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI95_SHIFT))&FNET_MK_NVIC_IP_PRI95_MASK)
#define FNET_MK_NVIC_IP_PRI96_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI96_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI96(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI96_SHIFT))&FNET_MK_NVIC_IP_PRI96_MASK)
#define FNET_MK_NVIC_IP_PRI97_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI97_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI97(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI97_SHIFT))&FNET_MK_NVIC_IP_PRI97_MASK)
#define FNET_MK_NVIC_IP_PRI98_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI98_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI98(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI98_SHIFT))&FNET_MK_NVIC_IP_PRI98_MASK)
#define FNET_MK_NVIC_IP_PRI99_MASK                       0xFFu
#define FNET_MK_NVIC_IP_PRI99_SHIFT                      0
#define FNET_MK_NVIC_IP_PRI99(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI99_SHIFT))&FNET_MK_NVIC_IP_PRI99_MASK)
#define FNET_MK_NVIC_IP_PRI100_MASK                      0xFFu
#define FNET_MK_NVIC_IP_PRI100_SHIFT                     0
#define FNET_MK_NVIC_IP_PRI100(x)                        (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI100_SHIFT))&FNET_MK_NVIC_IP_PRI100_MASK)
#define FNET_MK_NVIC_IP_PRI101_MASK                      0xFFu
#define FNET_MK_NVIC_IP_PRI101_SHIFT                     0
#define FNET_MK_NVIC_IP_PRI101(x)                        (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI101_SHIFT))&FNET_MK_NVIC_IP_PRI101_MASK)
#define FNET_MK_NVIC_IP_PRI102_MASK                      0xFFu
#define FNET_MK_NVIC_IP_PRI102_SHIFT                     0
#define FNET_MK_NVIC_IP_PRI102(x)                        (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI102_SHIFT))&FNET_MK_NVIC_IP_PRI102_MASK)
#define FNET_MK_NVIC_IP_PRI103_MASK                      0xFFu
#define FNET_MK_NVIC_IP_PRI103_SHIFT                     0
#define FNET_MK_NVIC_IP_PRI103(x)                        (((uint8_t)(((uint8_t)(x))<<FNET_MK_NVIC_IP_PRI103_SHIFT))&FNET_MK_NVIC_IP_PRI103_MASK)
/* STIR Bit Fields */
#define FNET_MK_NVIC_STIR_INTID_MASK                     0x1FFu
#define FNET_MK_NVIC_STIR_INTID_SHIFT                    0
#define FNET_MK_NVIC_STIR_INTID(x)                       (((uint32_t)(((uint32_t)(x))<<FNET_MK_NVIC_STIR_INTID_SHIFT))&FNET_MK_NVIC_STIR_INTID_MASK)


/* NVIC - Peripheral instance base addresses */
/* Peripheral NVIC base pointer */
#define FNET_MK_NVIC_BASE_PTR                            ((FNET_MK_NVIC_MemMapPtr)0xE000E100u)

/* ----------------------------------------------------------------------------
   -- NVIC - Register accessor macros
   ---------------------------------------------------------------------------- */

/* NVIC - Register instance definitions */
/* NVIC */
#define FNET_MK_NVICISER0                                FNET_MK_NVIC_ISER_REG(FNET_MK_NVIC_BASE_PTR,0)
#define FNET_MK_NVICISER1                                FNET_MK_NVIC_ISER_REG(FNET_MK_NVIC_BASE_PTR,1)
#define FNET_MK_NVICISER2                                FNET_MK_NVIC_ISER_REG(FNET_MK_NVIC_BASE_PTR,2)
#define FNET_MK_NVICISER3                                FNET_MK_NVIC_ISER_REG(FNET_MK_NVIC_BASE_PTR,3)
#define FNET_MK_NVICICER0                                FNET_MK_NVIC_ICER_REG(FNET_MK_NVIC_BASE_PTR,0)
#define FNET_MK_NVICICER1                                FNET_MK_NVIC_ICER_REG(FNET_MK_NVIC_BASE_PTR,1)
#define FNET_MK_NVICICER2                                FNET_MK_NVIC_ICER_REG(FNET_MK_NVIC_BASE_PTR,2)
#define FNET_MK_NVICICER3                                FNET_MK_NVIC_ICER_REG(FNET_MK_NVIC_BASE_PTR,3)
#define FNET_MK_NVICISPR0                                FNET_MK_NVIC_ISPR_REG(FNET_MK_NVIC_BASE_PTR,0)
#define FNET_MK_NVICISPR1                                FNET_MK_NVIC_ISPR_REG(FNET_MK_NVIC_BASE_PTR,1)
#define FNET_MK_NVICISPR2                                FNET_MK_NVIC_ISPR_REG(FNET_MK_NVIC_BASE_PTR,2)
#define FNET_MK_NVICISPR3                                FNET_MK_NVIC_ISPR_REG(FNET_MK_NVIC_BASE_PTR,3)
#define FNET_MK_NVICICPR0                                FNET_MK_NVIC_ICPR_REG(FNET_MK_NVIC_BASE_PTR,0)
#define FNET_MK_NVICICPR1                                FNET_MK_NVIC_ICPR_REG(FNET_MK_NVIC_BASE_PTR,1)
#define FNET_MK_NVICICPR2                                FNET_MK_NVIC_ICPR_REG(FNET_MK_NVIC_BASE_PTR,2)
#define FNET_MK_NVICICPR3                                FNET_MK_NVIC_ICPR_REG(FNET_MK_NVIC_BASE_PTR,3)
#define FNET_MK_NVICIABR0                                FNET_MK_NVIC_IABR_REG(FNET_MK_NVIC_BASE_PTR,0)
#define FNET_MK_NVICIABR1                                FNET_MK_NVIC_IABR_REG(FNET_MK_NVIC_BASE_PTR,1)
#define FNET_MK_NVICIABR2                                FNET_MK_NVIC_IABR_REG(FNET_MK_NVIC_BASE_PTR,2)
#define FNET_MK_NVICIABR3                                FNET_MK_NVIC_IABR_REG(FNET_MK_NVIC_BASE_PTR,3)
#define FNET_MK_NVICIP0                                  FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,0)
#define FNET_MK_NVICIP1                                  FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,1)
#define FNET_MK_NVICIP2                                  FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,2)
#define FNET_MK_NVICIP3                                  FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,3)
#define FNET_MK_NVICIP4                                  FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,4)
#define FNET_MK_NVICIP5                                  FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,5)
#define FNET_MK_NVICIP6                                  FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,6)
#define FNET_MK_NVICIP7                                  FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,7)
#define FNET_MK_NVICIP8                                  FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,8)
#define FNET_MK_NVICIP9                                  FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,9)
#define FNET_MK_NVICIP10                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,10)
#define FNET_MK_NVICIP11                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,11)
#define FNET_MK_NVICIP12                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,12)
#define FNET_MK_NVICIP13                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,13)
#define FNET_MK_NVICIP14                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,14)
#define FNET_MK_NVICIP15                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,15)
#define FNET_MK_NVICIP16                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,16)
#define FNET_MK_NVICIP17                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,17)
#define FNET_MK_NVICIP18                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,18)
#define FNET_MK_NVICIP19                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,19)
#define FNET_MK_NVICIP20                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,20)
#define FNET_MK_NVICIP21                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,21)
#define FNET_MK_NVICIP22                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,22)
#define FNET_MK_NVICIP23                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,23)
#define FNET_MK_NVICIP24                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,24)
#define FNET_MK_NVICIP25                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,25)
#define FNET_MK_NVICIP26                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,26)
#define FNET_MK_NVICIP27                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,27)
#define FNET_MK_NVICIP28                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,28)
#define FNET_MK_NVICIP29                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,29)
#define FNET_MK_NVICIP30                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,30)
#define FNET_MK_NVICIP31                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,31)
#define FNET_MK_NVICIP32                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,32)
#define FNET_MK_NVICIP33                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,33)
#define FNET_MK_NVICIP34                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,34)
#define FNET_MK_NVICIP35                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,35)
#define FNET_MK_NVICIP36                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,36)
#define FNET_MK_NVICIP37                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,37)
#define FNET_MK_NVICIP38                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,38)
#define FNET_MK_NVICIP39                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,39)
#define FNET_MK_NVICIP40                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,40)
#define FNET_MK_NVICIP41                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,41)
#define FNET_MK_NVICIP42                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,42)
#define FNET_MK_NVICIP43                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,43)
#define FNET_MK_NVICIP44                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,44)
#define FNET_MK_NVICIP45                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,45)
#define FNET_MK_NVICIP46                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,46)
#define FNET_MK_NVICIP47                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,47)
#define FNET_MK_NVICIP48                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,48)
#define FNET_MK_NVICIP49                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,49)
#define FNET_MK_NVICIP50                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,50)
#define FNET_MK_NVICIP51                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,51)
#define FNET_MK_NVICIP52                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,52)
#define FNET_MK_NVICIP53                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,53)
#define FNET_MK_NVICIP54                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,54)
#define FNET_MK_NVICIP55                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,55)
#define FNET_MK_NVICIP56                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,56)
#define FNET_MK_NVICIP57                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,57)
#define FNET_MK_NVICIP58                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,58)
#define FNET_MK_NVICIP59                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,59)
#define FNET_MK_NVICIP60                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,60)
#define FNET_MK_NVICIP61                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,61)
#define FNET_MK_NVICIP62                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,62)
#define FNET_MK_NVICIP63                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,63)
#define FNET_MK_NVICIP64                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,64)
#define FNET_MK_NVICIP65                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,65)
#define FNET_MK_NVICIP66                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,66)
#define FNET_MK_NVICIP67                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,67)
#define FNET_MK_NVICIP68                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,68)
#define FNET_MK_NVICIP69                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,69)
#define FNET_MK_NVICIP70                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,70)
#define FNET_MK_NVICIP71                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,71)
#define FNET_MK_NVICIP72                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,72)
#define FNET_MK_NVICIP73                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,73)
#define FNET_MK_NVICIP74                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,74)
#define FNET_MK_NVICIP75                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,75)
#define FNET_MK_NVICIP76                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,76)
#define FNET_MK_NVICIP77                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,77)
#define FNET_MK_NVICIP78                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,78)
#define FNET_MK_NVICIP79                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,79)
#define FNET_MK_NVICIP80                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,80)
#define FNET_MK_NVICIP81                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,81)
#define FNET_MK_NVICIP82                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,82)
#define FNET_MK_NVICIP83                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,83)
#define FNET_MK_NVICIP84                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,84)
#define FNET_MK_NVICIP85                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,85)
#define FNET_MK_NVICIP86                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,86)
#define FNET_MK_NVICIP87                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,87)
#define FNET_MK_NVICIP88                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,88)
#define FNET_MK_NVICIP89                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,89)
#define FNET_MK_NVICIP90                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,90)
#define FNET_MK_NVICIP91                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,91)
#define FNET_MK_NVICIP92                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,92)
#define FNET_MK_NVICIP93                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,93)
#define FNET_MK_NVICIP94                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,94)
#define FNET_MK_NVICIP95                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,95)
#define FNET_MK_NVICIP96                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,96)
#define FNET_MK_NVICIP97                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,97)
#define FNET_MK_NVICIP98                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,98)
#define FNET_MK_NVICIP99                                 FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,99)
#define FNET_MK_NVICIP100                                FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,100)
#define FNET_MK_NVICIP101                                FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,101)
#define FNET_MK_NVICIP102                                FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,102)
#define FNET_MK_NVICIP103                                FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,103)
#define FNET_MK_NVICSTIR                                 FNET_MK_NVIC_STIR_REG(FNET_MK_NVIC_BASE_PTR,0)

/* NVIC - Register array accessors */
#define FNET_MK_NVIC_ISER(index)                         FNET_MK_NVIC_ISER_REG(FNET_MK_NVIC_BASE_PTR,(index))
#define FNET_MK_NVIC_ICER(index)                         FNET_MK_NVIC_ICER_REG(FNET_MK_NVIC_BASE_PTR,(index))
#define FNET_MK_NVIC_ISPR(index)                         FNET_MK_NVIC_ISPR_REG(FNET_MK_NVIC_BASE_PTR,(index))
#define FNET_MK_NVIC_ICPR(index)                         FNET_MK_NVIC_ICPR_REG(FNET_MK_NVIC_BASE_PTR,(index))
#define FNET_MK_NVIC_IABR(index)                         FNET_MK_NVIC_IABR_REG(FNET_MK_NVIC_BASE_PTR,(index))
#define FNET_MK_NVIC_IP(index)                           FNET_MK_NVIC_IP_REG(FNET_MK_NVIC_BASE_PTR,(index))
#define FNET_MK_NVIC_STIR(index)                         FNET_MK_NVIC_STIR_REG(FNET_MK_NVIC_BASE_PTR,(index))


/* ----------------------------------------------------------------------------
   -- PIT
   ---------------------------------------------------------------------------- */

/* PIT - Peripheral register structure */
typedef struct FNET_MK_PIT_MemMap
{
    fnet_uint32_t MCR;                                      /* PIT Module Control Register, offset: 0x0 */
    fnet_uint8_t RESERVED_0[252];
    struct                                                /* offset: 0x100, array step: 0x10 */
    {
        fnet_uint32_t LDVAL;                                  /* Timer Load Value Register, array offset: 0x100, array step: 0x10 */
        fnet_uint32_t CVAL;                                   /* Current Timer Value Register, array offset: 0x104, array step: 0x10 */
        fnet_uint32_t TCTRL;                                  /* Timer Control Register, array offset: 0x108, array step: 0x10 */
        fnet_uint32_t TFLG;                                   /* Timer Flag Register, array offset: 0x10C, array step: 0x10 */
    } CHANNEL[4];
} volatile *FNET_MK_PIT_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- PIT - Register accessor macros
   ---------------------------------------------------------------------------- */

/* PIT - Register accessors */
#define FNET_MK_PIT_MCR_REG(base)                        ((base)->MCR)
#define FNET_MK_PIT_LDVAL_REG(base,index)                ((base)->CHANNEL[(index)].LDVAL)
#define FNET_MK_PIT_CVAL_REG(base,index)                 ((base)->CHANNEL[(index)].CVAL)
#define FNET_MK_PIT_TCTRL_REG(base,index)                ((base)->CHANNEL[(index)].TCTRL)
#define FNET_MK_PIT_TFLG_REG(base,index)                 ((base)->CHANNEL[(index)].TFLG)

/* ----------------------------------------------------------------------------
   -- PIT Register Masks
   ---------------------------------------------------------------------------- */

/* MCR Bit Fields */
#define FNET_MK_PIT_MCR_FRZ_MASK                         0x1u
#define FNET_MK_PIT_MCR_FRZ_SHIFT                        0
#define FNET_MK_PIT_MCR_MDIS_MASK                        0x2u
#define FNET_MK_PIT_MCR_MDIS_SHIFT                       1
/* LDVAL Bit Fields */
#define FNET_MK_PIT_LDVAL_TSV_MASK                       0xFFFFFFFFu
#define FNET_MK_PIT_LDVAL_TSV_SHIFT                      0
#define FNET_MK_PIT_LDVAL_TSV(x)                         (((uint32_t)(((uint32_t)(x))<<FNET_MK_PIT_LDVAL_TSV_SHIFT))&FNET_MK_PIT_LDVAL_TSV_MASK)
/* CVAL Bit Fields */
#define FNET_MK_PIT_CVAL_TVL_MASK                        0xFFFFFFFFu
#define FNET_MK_PIT_CVAL_TVL_SHIFT                       0
#define FNET_MK_PIT_CVAL_TVL(x)                          (((uint32_t)(((uint32_t)(x))<<FNET_MK_PIT_CVAL_TVL_SHIFT))&FNET_MK_PIT_CVAL_TVL_MASK)
/* TCTRL Bit Fields */
#define FNET_MK_PIT_TCTRL_TEN_MASK                       0x1u
#define FNET_MK_PIT_TCTRL_TEN_SHIFT                      0
#define FNET_MK_PIT_TCTRL_TIE_MASK                       0x2u
#define FNET_MK_PIT_TCTRL_TIE_SHIFT                      1
/* TFLG Bit Fields */
#define FNET_MK_PIT_TFLG_TIF_MASK                        0x1u
#define FNET_MK_PIT_TFLG_TIF_SHIFT                       0

/* PIT - Peripheral instance base addresses */
/* Peripheral PIT base pointer */
#define FNET_MK_PIT_BASE_PTR                             ((FNET_MK_PIT_MemMapPtr)0x40037000u)

/* ----------------------------------------------------------------------------
   -- PIT - Register accessor macros
   ---------------------------------------------------------------------------- */

/* PIT - Register instance definitions */
/* PIT */
#define FNET_MK_PIT_MCR                                  FNET_MK_PIT_MCR_REG(FNET_MK_PIT_BASE_PTR)
#define FNET_MK_PIT_LDVAL0                               FNET_MK_PIT_LDVAL_REG(FNET_MK_PIT_BASE_PTR,0)
#define FNET_MK_PIT_CVAL0                                FNET_MK_PIT_CVAL_REG(FNET_MK_PIT_BASE_PTR,0)
#define FNET_MK_PIT_TCTRL0                               FNET_MK_PIT_TCTRL_REG(FNET_MK_PIT_BASE_PTR,0)
#define FNET_MK_PIT_TFLG0                                FNET_MK_PIT_TFLG_REG(FNET_MK_PIT_BASE_PTR,0)
#define FNET_MK_PIT_LDVAL1                               FNET_MK_PIT_LDVAL_REG(FNET_MK_PIT_BASE_PTR,1)
#define FNET_MK_PIT_CVAL1                                FNET_MK_PIT_CVAL_REG(FNET_MK_PIT_BASE_PTR,1)
#define FNET_MK_PIT_TCTRL1                               FNET_MK_PIT_TCTRL_REG(FNET_MK_PIT_BASE_PTR,1)
#define FNET_MK_PIT_TFLG1                                FNET_MK_PIT_TFLG_REG(FNET_MK_PIT_BASE_PTR,1)
#define FNET_MK_PIT_LDVAL2                               FNET_MK_PIT_LDVAL_REG(FNET_MK_PIT_BASE_PTR,2)
#define FNET_MK_PIT_CVAL2                                FNET_MK_PIT_CVAL_REG(FNET_MK_PIT_BASE_PTR,2)
#define FNET_MK_PIT_TCTRL2                               FNET_MK_PIT_TCTRL_REG(FNET_MK_PIT_BASE_PTR,2)
#define FNET_MK_PIT_TFLG2                                FNET_MK_PIT_TFLG_REG(FNET_MK_PIT_BASE_PTR,2)
#define FNET_MK_PIT_LDVAL3                               FNET_MK_PIT_LDVAL_REG(FNET_MK_PIT_BASE_PTR,3)
#define FNET_MK_PIT_CVAL3                                FNET_MK_PIT_CVAL_REG(FNET_MK_PIT_BASE_PTR,3)
#define FNET_MK_PIT_TCTRL3                               FNET_MK_PIT_TCTRL_REG(FNET_MK_PIT_BASE_PTR,3)
#define FNET_MK_PIT_TFLG3                                FNET_MK_PIT_TFLG_REG(FNET_MK_PIT_BASE_PTR,3)

/* PIT - Register array accessors */
#define FNET_MK_PIT_LDVAL(index)                         FNET_MK_PIT_LDVAL_REG(FNET_MK_PIT_BASE_PTR,(index))
#define FNET_MK_PIT_CVAL(index)                          FNET_MK_PIT_CVAL_REG(FNET_MK_PIT_BASE_PTR,(index))
#define FNET_MK_PIT_TCTRL(index)                         FNET_MK_PIT_TCTRL_REG(FNET_MK_PIT_BASE_PTR,(index))
#define FNET_MK_PIT_TFLG(index)                          FNET_MK_PIT_TFLG_REG(FNET_MK_PIT_BASE_PTR,(index))

/* ----------------------------------------------------------------------------
   -- MPU
   ---------------------------------------------------------------------------- */

/* MPU - Peripheral register structure */
typedef struct FNET_MK_MPU_MemMap
{
    fnet_uint32_t CESR;                                     /* Control/Error Status Register, offset: 0x0 */
    fnet_uint8_t RESERVED_0[12];
    struct                                                /* offset: 0x10, array step: 0x8 */
    {
        fnet_uint32_t EAR;                                    /* Error Address Register, Slave Port n, array offset: 0x10, array step: 0x8 */
        fnet_uint32_t EDR;                                    /* Error Detail Register, Slave Port n, array offset: 0x14, array step: 0x8 */
    } SP[5];
    fnet_uint8_t RESERVED_1[968];
    fnet_uint32_t WORD[12][4];                              /* Region Descriptor n, Word 0..Region Descriptor n, Word 3, array offset: 0x400, array step: index*0x10, index2*0x4 */
    fnet_uint8_t RESERVED_2[832];
    fnet_uint32_t RGDAAC[12];                               /* Region Descriptor Alternate Access Control n, array offset: 0x800, array step: 0x4 */
} volatile *FNET_MK_MPU_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- MPU - Register accessor macros
   ---------------------------------------------------------------------------- */

/* MPU - Register accessors */
#define FNET_MK_MPU_CESR_REG(base)                       ((base)->CESR)
#define FNET_MK_MPU_EAR_REG(base,index)                  ((base)->SP[(index)].EAR)
#define FNET_MK_MPU_EDR_REG(base,index)                  ((base)->SP[(index)].EDR)
#define FNET_MK_MPU_WORD_REG(base,index,index2)          ((base)->WORD[(index)][(index2)])
#define FNET_MK_MPU_RGDAAC_REG(base,index)               ((base)->RGDAAC[(index)])

/* ----------------------------------------------------------------------------
   -- MPU Register Masks
   ---------------------------------------------------------------------------- */

/* CESR Bit Fields */
#define FNET_MK_MPU_CESR_VLD_MASK                        0x1u
#define FNET_MK_MPU_CESR_VLD_SHIFT                       0
#define FNET_MK_MPU_CESR_NRGD_MASK                       0xF00u
#define FNET_MK_MPU_CESR_NRGD_SHIFT                      8
#define FNET_MK_MPU_CESR_NRGD(x)                         (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_CESR_NRGD_SHIFT))&FNET_MK_MPU_CESR_NRGD_MASK)
#define FNET_MK_MPU_CESR_NSP_MASK                        0xF000u
#define FNET_MK_MPU_CESR_NSP_SHIFT                       12
#define FNET_MK_MPU_CESR_NSP(x)                          (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_CESR_NSP_SHIFT))&FNET_MK_MPU_CESR_NSP_MASK)
#define FNET_MK_MPU_CESR_HRL_MASK                        0xF0000u
#define FNET_MK_MPU_CESR_HRL_SHIFT                       16
#define FNET_MK_MPU_CESR_HRL(x)                          (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_CESR_HRL_SHIFT))&FNET_MK_MPU_CESR_HRL_MASK)
#define FNET_MK_MPU_CESR_SPERR_MASK                      0xF8000000u
#define FNET_MK_MPU_CESR_SPERR_SHIFT                     27
#define FNET_MK_MPU_CESR_SPERR(x)                        (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_CESR_SPERR_SHIFT))&FNET_MK_MPU_CESR_SPERR_MASK)
/* EAR Bit Fields */
#define FNET_MK_MPU_EAR_EADDR_MASK                       0xFFFFFFFFu
#define FNET_MK_MPU_EAR_EADDR_SHIFT                      0
#define FNET_MK_MPU_EAR_EADDR(x)                         (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_EAR_EADDR_SHIFT))&FNET_MK_MPU_EAR_EADDR_MASK)
/* EDR Bit Fields */
#define FNET_MK_MPU_EDR_ERW_MASK                         0x1u
#define FNET_MK_MPU_EDR_ERW_SHIFT                        0
#define FNET_MK_MPU_EDR_EATTR_MASK                       0xEu
#define FNET_MK_MPU_EDR_EATTR_SHIFT                      1
#define FNET_MK_MPU_EDR_EATTR(x)                         (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_EDR_EATTR_SHIFT))&FNET_MK_MPU_EDR_EATTR_MASK)
#define FNET_MK_MPU_EDR_EMN_MASK                         0xF0u
#define FNET_MK_MPU_EDR_EMN_SHIFT                        4
#define FNET_MK_MPU_EDR_EMN(x)                           (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_EDR_EMN_SHIFT))&FNET_MK_MPU_EDR_EMN_MASK)
#define FNET_MK_MPU_EDR_EACD_MASK                        0xFFFF0000u
#define FNET_MK_MPU_EDR_EACD_SHIFT                       16
#define FNET_MK_MPU_EDR_EACD(x)                          (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_EDR_EACD_SHIFT))&FNET_MK_MPU_EDR_EACD_MASK)
/* WORD Bit Fields */
#define FNET_MK_MPU_WORD_M0UM_MASK                       0x7u
#define FNET_MK_MPU_WORD_M0UM_SHIFT                      0
#define FNET_MK_MPU_WORD_M0UM(x)                         (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_WORD_M0UM_SHIFT))&FNET_MK_MPU_WORD_M0UM_MASK)
#define FNET_MK_MPU_WORD_VLD_MASK                        0x1u
#define FNET_MK_MPU_WORD_VLD_SHIFT                       0
#define FNET_MK_MPU_WORD_M0SM_MASK                       0x18u
#define FNET_MK_MPU_WORD_M0SM_SHIFT                      3
#define FNET_MK_MPU_WORD_M0SM(x)                         (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_WORD_M0SM_SHIFT))&FNET_MK_MPU_WORD_M0SM_MASK)
#define FNET_MK_MPU_WORD_ENDADDR_MASK                    0xFFFFFFE0u
#define FNET_MK_MPU_WORD_ENDADDR_SHIFT                   5
#define FNET_MK_MPU_WORD_ENDADDR(x)                      (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_WORD_ENDADDR_SHIFT))&FNET_MK_MPU_WORD_ENDADDR_MASK)
#define FNET_MK_MPU_WORD_SRTADDR_MASK                    0xFFFFFFE0u
#define FNET_MK_MPU_WORD_SRTADDR_SHIFT                   5
#define FNET_MK_MPU_WORD_SRTADDR(x)                      (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_WORD_SRTADDR_SHIFT))&FNET_MK_MPU_WORD_SRTADDR_MASK)
#define FNET_MK_MPU_WORD_M1UM_MASK                       0x1C0u
#define FNET_MK_MPU_WORD_M1UM_SHIFT                      6
#define FNET_MK_MPU_WORD_M1UM(x)                         (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_WORD_M1UM_SHIFT))&FNET_MK_MPU_WORD_M1UM_MASK)
#define FNET_MK_MPU_WORD_M1SM_MASK                       0x600u
#define FNET_MK_MPU_WORD_M1SM_SHIFT                      9
#define FNET_MK_MPU_WORD_M1SM(x)                         (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_WORD_M1SM_SHIFT))&FNET_MK_MPU_WORD_M1SM_MASK)
#define FNET_MK_MPU_WORD_M2UM_MASK                       0x7000u
#define FNET_MK_MPU_WORD_M2UM_SHIFT                      12
#define FNET_MK_MPU_WORD_M2UM(x)                         (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_WORD_M2UM_SHIFT))&FNET_MK_MPU_WORD_M2UM_MASK)
#define FNET_MK_MPU_WORD_M2SM_MASK                       0x18000u
#define FNET_MK_MPU_WORD_M2SM_SHIFT                      15
#define FNET_MK_MPU_WORD_M2SM(x)                         (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_WORD_M2SM_SHIFT))&FNET_MK_MPU_WORD_M2SM_MASK)
#define FNET_MK_MPU_WORD_M3UM_MASK                       0x1C0000u
#define FNET_MK_MPU_WORD_M3UM_SHIFT                      18
#define FNET_MK_MPU_WORD_M3UM(x)                         (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_WORD_M3UM_SHIFT))&FNET_MK_MPU_WORD_M3UM_MASK)
#define FNET_MK_MPU_WORD_M3SM_MASK                       0x600000u
#define FNET_MK_MPU_WORD_M3SM_SHIFT                      21
#define FNET_MK_MPU_WORD_M3SM(x)                         (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_WORD_M3SM_SHIFT))&FNET_MK_MPU_WORD_M3SM_MASK)
#define FNET_MK_MPU_WORD_M4WE_MASK                       0x1000000u
#define FNET_MK_MPU_WORD_M4WE_SHIFT                      24
#define FNET_MK_MPU_WORD_M4RE_MASK                       0x2000000u
#define FNET_MK_MPU_WORD_M4RE_SHIFT                      25
#define FNET_MK_MPU_WORD_M5WE_MASK                       0x4000000u
#define FNET_MK_MPU_WORD_M5WE_SHIFT                      26
#define FNET_MK_MPU_WORD_M5RE_MASK                       0x8000000u
#define FNET_MK_MPU_WORD_M5RE_SHIFT                      27
#define FNET_MK_MPU_WORD_M6WE_MASK                       0x10000000u
#define FNET_MK_MPU_WORD_M6WE_SHIFT                      28
#define FNET_MK_MPU_WORD_M6RE_MASK                       0x20000000u
#define FNET_MK_MPU_WORD_M6RE_SHIFT                      29
#define FNET_MK_MPU_WORD_M7WE_MASK                       0x40000000u
#define FNET_MK_MPU_WORD_M7WE_SHIFT                      30
#define FNET_MK_MPU_WORD_M7RE_MASK                       0x80000000u
#define FNET_MK_MPU_WORD_M7RE_SHIFT                      31
/* RGDAAC Bit Fields */
#define FNET_MK_MPU_RGDAAC_M0UM_MASK                     0x7u
#define FNET_MK_MPU_RGDAAC_M0UM_SHIFT                    0
#define FNET_MK_MPU_RGDAAC_M0UM(x)                       (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_RGDAAC_M0UM_SHIFT))&FNET_MK_MPU_RGDAAC_M0UM_MASK)
#define FNET_MK_MPU_RGDAAC_M0SM_MASK                     0x18u
#define FNET_MK_MPU_RGDAAC_M0SM_SHIFT                    3
#define FNET_MK_MPU_RGDAAC_M0SM(x)                       (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_RGDAAC_M0SM_SHIFT))&FNET_MK_MPU_RGDAAC_M0SM_MASK)
#define FNET_MK_MPU_RGDAAC_M1UM_MASK                     0x1C0u
#define FNET_MK_MPU_RGDAAC_M1UM_SHIFT                    6
#define FNET_MK_MPU_RGDAAC_M1UM(x)                       (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_RGDAAC_M1UM_SHIFT))&FNET_MK_MPU_RGDAAC_M1UM_MASK)
#define FNET_MK_MPU_RGDAAC_M1SM_MASK                     0x600u
#define FNET_MK_MPU_RGDAAC_M1SM_SHIFT                    9
#define FNET_MK_MPU_RGDAAC_M1SM(x)                       (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_RGDAAC_M1SM_SHIFT))&FNET_MK_MPU_RGDAAC_M1SM_MASK)
#define FNET_MK_MPU_RGDAAC_M2UM_MASK                     0x7000u
#define FNET_MK_MPU_RGDAAC_M2UM_SHIFT                    12
#define FNET_MK_MPU_RGDAAC_M2UM(x)                       (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_RGDAAC_M2UM_SHIFT))&FNET_MK_MPU_RGDAAC_M2UM_MASK)
#define FNET_MK_MPU_RGDAAC_M2SM_MASK                     0x18000u
#define FNET_MK_MPU_RGDAAC_M2SM_SHIFT                    15
#define FNET_MK_MPU_RGDAAC_M2SM(x)                       (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_RGDAAC_M2SM_SHIFT))&FNET_MK_MPU_RGDAAC_M2SM_MASK)
#define FNET_MK_MPU_RGDAAC_M3UM_MASK                     0x1C0000u
#define FNET_MK_MPU_RGDAAC_M3UM_SHIFT                    18
#define FNET_MK_MPU_RGDAAC_M3UM(x)                       (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_RGDAAC_M3UM_SHIFT))&FNET_MK_MPU_RGDAAC_M3UM_MASK)
#define FNET_MK_MPU_RGDAAC_M3SM_MASK                     0x600000u
#define FNET_MK_MPU_RGDAAC_M3SM_SHIFT                    21
#define FNET_MK_MPU_RGDAAC_M3SM(x)                       (((uint32_t)(((uint32_t)(x))<<FNET_MK_MPU_RGDAAC_M3SM_SHIFT))&FNET_MK_MPU_RGDAAC_M3SM_MASK)
#define FNET_MK_MPU_RGDAAC_M4WE_MASK                     0x1000000u
#define FNET_MK_MPU_RGDAAC_M4WE_SHIFT                    24
#define FNET_MK_MPU_RGDAAC_M4RE_MASK                     0x2000000u
#define FNET_MK_MPU_RGDAAC_M4RE_SHIFT                    25
#define FNET_MK_MPU_RGDAAC_M5WE_MASK                     0x4000000u
#define FNET_MK_MPU_RGDAAC_M5WE_SHIFT                    26
#define FNET_MK_MPU_RGDAAC_M5RE_MASK                     0x8000000u
#define FNET_MK_MPU_RGDAAC_M5RE_SHIFT                    27
#define FNET_MK_MPU_RGDAAC_M6WE_MASK                     0x10000000u
#define FNET_MK_MPU_RGDAAC_M6WE_SHIFT                    28
#define FNET_MK_MPU_RGDAAC_M6RE_MASK                     0x20000000u
#define FNET_MK_MPU_RGDAAC_M6RE_SHIFT                    29
#define FNET_MK_MPU_RGDAAC_M7WE_MASK                     0x40000000u
#define FNET_MK_MPU_RGDAAC_M7WE_SHIFT                    30
#define FNET_MK_MPU_RGDAAC_M7RE_MASK                     0x80000000u
#define FNET_MK_MPU_RGDAAC_M7RE_SHIFT                    31

/* MPU - Peripheral instance base addresses */
/* Peripheral MPU base pointer */
#define FNET_MK_MPU_BASE_PTR                             ((FNET_MK_MPU_MemMapPtr)0x4000D000u)

/* ----------------------------------------------------------------------------
   -- MPU - Register accessor macros
   ---------------------------------------------------------------------------- */

/* MPU - Register instance definitions */
/* MPU */
#define FNET_MK_MPU_CESR                                 FNET_MK_MPU_CESR_REG(FNET_MK_MPU_BASE_PTR)
#define FNET_MK_MPU_EAR0                                 FNET_MK_MPU_EAR_REG(FNET_MK_MPU_BASE_PTR,0)
#define FNET_MK_MPU_EDR0                                 FNET_MK_MPU_EDR_REG(FNET_MK_MPU_BASE_PTR,0)
#define FNET_MK_MPU_EAR1                                 FNET_MK_MPU_EAR_REG(FNET_MK_MPU_BASE_PTR,1)
#define FNET_MK_MPU_EDR1                                 FNET_MK_MPU_EDR_REG(FNET_MK_MPU_BASE_PTR,1)
#define FNET_MK_MPU_EAR2                                 FNET_MK_MPU_EAR_REG(FNET_MK_MPU_BASE_PTR,2)
#define FNET_MK_MPU_EDR2                                 FNET_MK_MPU_EDR_REG(FNET_MK_MPU_BASE_PTR,2)
#define FNET_MK_MPU_EAR3                                 FNET_MK_MPU_EAR_REG(FNET_MK_MPU_BASE_PTR,3)
#define FNET_MK_MPU_EDR3                                 FNET_MK_MPU_EDR_REG(FNET_MK_MPU_BASE_PTR,3)
#define FNET_MK_MPU_EAR4                                 FNET_MK_MPU_EAR_REG(FNET_MK_MPU_BASE_PTR,4)
#define FNET_MK_MPU_EDR4                                 FNET_MK_MPU_EDR_REG(FNET_MK_MPU_BASE_PTR,4)
#define FNET_MK_MPU_RGD0_WORD0                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,0,0)
#define FNET_MK_MPU_RGD0_WORD1                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,0,1)
#define FNET_MK_MPU_RGD0_WORD2                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,0,2)
#define FNET_MK_MPU_RGD0_WORD3                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,0,3)
#define FNET_MK_MPU_RGD1_WORD0                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,1,0)
#define FNET_MK_MPU_RGD1_WORD1                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,1,1)
#define FNET_MK_MPU_RGD1_WORD2                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,1,2)
#define FNET_MK_MPU_RGD1_WORD3                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,1,3)
#define FNET_MK_MPU_RGD2_WORD0                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,2,0)
#define FNET_MK_MPU_RGD2_WORD1                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,2,1)
#define FNET_MK_MPU_RGD2_WORD2                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,2,2)
#define FNET_MK_MPU_RGD2_WORD3                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,2,3)
#define FNET_MK_MPU_RGD3_WORD0                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,3,0)
#define FNET_MK_MPU_RGD3_WORD1                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,3,1)
#define FNET_MK_MPU_RGD3_WORD2                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,3,2)
#define FNET_MK_MPU_RGD3_WORD3                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,3,3)
#define FNET_MK_MPU_RGD4_WORD0                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,4,0)
#define FNET_MK_MPU_RGD4_WORD1                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,4,1)
#define FNET_MK_MPU_RGD4_WORD2                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,4,2)
#define FNET_MK_MPU_RGD4_WORD3                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,4,3)
#define FNET_MK_MPU_RGD5_WORD0                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,5,0)
#define FNET_MK_MPU_RGD5_WORD1                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,5,1)
#define FNET_MK_MPU_RGD5_WORD2                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,5,2)
#define FNET_MK_MPU_RGD5_WORD3                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,5,3)
#define FNET_MK_MPU_RGD6_WORD0                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,6,0)
#define FNET_MK_MPU_RGD6_WORD1                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,6,1)
#define FNET_MK_MPU_RGD6_WORD2                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,6,2)
#define FNET_MK_MPU_RGD6_WORD3                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,6,3)
#define FNET_MK_MPU_RGD7_WORD0                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,7,0)
#define FNET_MK_MPU_RGD7_WORD1                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,7,1)
#define FNET_MK_MPU_RGD7_WORD2                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,7,2)
#define FNET_MK_MPU_RGD7_WORD3                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,7,3)
#define FNET_MK_MPU_RGD8_WORD0                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,8,0)
#define FNET_MK_MPU_RGD8_WORD1                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,8,1)
#define FNET_MK_MPU_RGD8_WORD2                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,8,2)
#define FNET_MK_MPU_RGD8_WORD3                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,8,3)
#define FNET_MK_MPU_RGD9_WORD0                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,9,0)
#define FNET_MK_MPU_RGD9_WORD1                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,9,1)
#define FNET_MK_MPU_RGD9_WORD2                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,9,2)
#define FNET_MK_MPU_RGD9_WORD3                           FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,9,3)
#define FNET_MK_MPU_RGD10_WORD0                          FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,10,0)
#define FNET_MK_MPU_RGD10_WORD1                          FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,10,1)
#define FNET_MK_MPU_RGD10_WORD2                          FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,10,2)
#define FNET_MK_MPU_RGD10_WORD3                          FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,10,3)
#define FNET_MK_MPU_RGD11_WORD0                          FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,11,0)
#define FNET_MK_MPU_RGD11_WORD1                          FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,11,1)
#define FNET_MK_MPU_RGD11_WORD2                          FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,11,2)
#define FNET_MK_MPU_RGD11_WORD3                          FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,11,3)
#define FNET_MK_MPU_RGDAAC0                              FNET_MK_MPU_RGDAAC_REG(FNET_MK_MPU_BASE_PTR,0)
#define FNET_MK_MPU_RGDAAC1                              FNET_MK_MPU_RGDAAC_REG(FNET_MK_MPU_BASE_PTR,1)
#define FNET_MK_MPU_RGDAAC2                              FNET_MK_MPU_RGDAAC_REG(FNET_MK_MPU_BASE_PTR,2)
#define FNET_MK_MPU_RGDAAC3                              FNET_MK_MPU_RGDAAC_REG(FNET_MK_MPU_BASE_PTR,3)
#define FNET_MK_MPU_RGDAAC4                              FNET_MK_MPU_RGDAAC_REG(FNET_MK_MPU_BASE_PTR,4)
#define FNET_MK_MPU_RGDAAC5                              FNET_MK_MPU_RGDAAC_REG(FNET_MK_MPU_BASE_PTR,5)
#define FNET_MK_MPU_RGDAAC6                              FNET_MK_MPU_RGDAAC_REG(FNET_MK_MPU_BASE_PTR,6)
#define FNET_MK_MPU_RGDAAC7                              FNET_MK_MPU_RGDAAC_REG(FNET_MK_MPU_BASE_PTR,7)
#define FNET_MK_MPU_RGDAAC8                              FNET_MK_MPU_RGDAAC_REG(FNET_MK_MPU_BASE_PTR,8)
#define FNET_MK_MPU_RGDAAC9                              FNET_MK_MPU_RGDAAC_REG(FNET_MK_MPU_BASE_PTR,9)
#define FNET_MK_MPU_RGDAAC10                             FNET_MK_MPU_RGDAAC_REG(FNET_MK_MPU_BASE_PTR,10)
#define FNET_MK_MPU_RGDAAC11                             FNET_MK_MPU_RGDAAC_REG(FNET_MK_MPU_BASE_PTR,11)

/* MPU - Register array accessors */
#define FNET_MK_MPU_EAR(index)                           FNET_MK_MPU_EAR_REG(FNET_MK_MPU_BASE_PTR,(index))
#define FNET_MK_MPU_EDR(index)                           FNET_MK_MPU_EDR_REG(FNET_MK_MPU_BASE_PTR,(index))
#define FNET_MK_MPU_WORD(index,index2)                   FNET_MK_MPU_WORD_REG(FNET_MK_MPU_BASE_PTR,(index),(index2))
#define FNET_MK_MPU_RGDAAC(index)                        FNET_MK_MPU_RGDAAC_REG(FNET_MK_MPU_BASE_PTR,(index))


/* ----------------------------------------------------------------------------
   -- FMC
   ---------------------------------------------------------------------------- */

/* FMC - Peripheral register structure */
typedef struct FNET_MK_FMC_MemMap
{
    fnet_uint32_t PFAPR;                                    /* Flash Access Protection Register, offset: 0x0 */
    fnet_uint32_t PFB0CR;                                   /* Flash Bank 0 Control Register, offset: 0x4 */
    fnet_uint32_t PFB1CR;                                   /* Flash Bank 1 Control Register, offset: 0x8 */
    fnet_uint8_t RESERVED_0[244];
    fnet_uint32_t TAGVD[4][8];                              /* Cache Directory Storage, array offset: 0x100, array step: index*0x20, index2*0x4 */
    fnet_uint8_t RESERVED_1[128];
    struct                                                /* offset: 0x200, array step: index*0x40, index2*0x8 */
    {
        fnet_uint32_t DATA_U;                                 /* Cache Data Storage (upper word), array offset: 0x200, array step: index*0x40, index2*0x8 */
        fnet_uint32_t DATA_L;                                 /* Cache Data Storage (lower word), array offset: 0x204, array step: index*0x40, index2*0x8 */
    } SET[4][8];
} volatile *FNET_MK_FMC_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- FMC - Register accessor macros
   ---------------------------------------------------------------------------- */

/* FMC - Register accessors */
#define FNET_MK_FMC_PFAPR_REG(base)                      ((base)->PFAPR)
#define FNET_MK_FMC_PFB0CR_REG(base)                     ((base)->PFB0CR)
#define FNET_MK_FMC_PFB1CR_REG(base)                     ((base)->PFB1CR)
#define FNET_MK_FMC_TAGVD_REG(base,index,index2)         ((base)->TAGVD[(index)][(index2)])
#define FNET_MK_FMC_DATA_U_REG(base,index,index2)        ((base)->SET[(index)][(index2)].DATA_U)
#define FNET_MK_FMC_DATA_L_REG(base,index,index2)        ((base)->SET[(index)][(index2)].DATA_L)

/* ----------------------------------------------------------------------------
   -- FMC Register Masks
   ---------------------------------------------------------------------------- */

/* PFAPR Bit Fields */
#define FNET_MK_FMC_PFAPR_M0AP_MASK                      0x3u
#define FNET_MK_FMC_PFAPR_M0AP_SHIFT                     0
#define FNET_MK_FMC_PFAPR_M0AP(x)                        (((uint32_t)(((uint32_t)(x))<<FNET_MK_FMC_PFAPR_M0AP_SHIFT))&FNET_MK_FMC_PFAPR_M0AP_MASK)
#define FNET_MK_FMC_PFAPR_M1AP_MASK                      0xCu
#define FNET_MK_FMC_PFAPR_M1AP_SHIFT                     2
#define FNET_MK_FMC_PFAPR_M1AP(x)                        (((uint32_t)(((uint32_t)(x))<<FNET_MK_FMC_PFAPR_M1AP_SHIFT))&FNET_MK_FMC_PFAPR_M1AP_MASK)
#define FNET_MK_FMC_PFAPR_M2AP_MASK                      0x30u
#define FNET_MK_FMC_PFAPR_M2AP_SHIFT                     4
#define FNET_MK_FMC_PFAPR_M2AP(x)                        (((uint32_t)(((uint32_t)(x))<<FNET_MK_FMC_PFAPR_M2AP_SHIFT))&FNET_MK_FMC_PFAPR_M2AP_MASK)
#define FNET_MK_FMC_PFAPR_M3AP_MASK                      0xC0u
#define FNET_MK_FMC_PFAPR_M3AP_SHIFT                     6
#define FNET_MK_FMC_PFAPR_M3AP(x)                        (((uint32_t)(((uint32_t)(x))<<FNET_MK_FMC_PFAPR_M3AP_SHIFT))&FNET_MK_FMC_PFAPR_M3AP_MASK)
#define FNET_MK_FMC_PFAPR_M4AP_MASK                      0x300u
#define FNET_MK_FMC_PFAPR_M4AP_SHIFT                     8
#define FNET_MK_FMC_PFAPR_M4AP(x)                        (((uint32_t)(((uint32_t)(x))<<FNET_MK_FMC_PFAPR_M4AP_SHIFT))&FNET_MK_FMC_PFAPR_M4AP_MASK)
#define FNET_MK_FMC_PFAPR_M5AP_MASK                      0xC00u
#define FNET_MK_FMC_PFAPR_M5AP_SHIFT                     10
#define FNET_MK_FMC_PFAPR_M5AP(x)                        (((uint32_t)(((uint32_t)(x))<<FNET_MK_FMC_PFAPR_M5AP_SHIFT))&FNET_MK_FMC_PFAPR_M5AP_MASK)
#define FNET_MK_FMC_PFAPR_M6AP_MASK                      0x3000u
#define FNET_MK_FMC_PFAPR_M6AP_SHIFT                     12
#define FNET_MK_FMC_PFAPR_M6AP(x)                        (((uint32_t)(((uint32_t)(x))<<FNET_MK_FMC_PFAPR_M6AP_SHIFT))&FNET_MK_FMC_PFAPR_M6AP_MASK)
#define FNET_MK_FMC_PFAPR_M7AP_MASK                      0xC000u
#define FNET_MK_FMC_PFAPR_M7AP_SHIFT                     14
#define FNET_MK_FMC_PFAPR_M7AP(x)                        (((uint32_t)(((uint32_t)(x))<<FNET_MK_FMC_PFAPR_M7AP_SHIFT))&FNET_MK_FMC_PFAPR_M7AP_MASK)
#define FNET_MK_FMC_PFAPR_M0PFD_MASK                     0x10000u
#define FNET_MK_FMC_PFAPR_M0PFD_SHIFT                    16
#define FNET_MK_FMC_PFAPR_M1PFD_MASK                     0x20000u
#define FNET_MK_FMC_PFAPR_M1PFD_SHIFT                    17
#define FNET_MK_FMC_PFAPR_M2PFD_MASK                     0x40000u
#define FNET_MK_FMC_PFAPR_M2PFD_SHIFT                    18
#define FNET_MK_FMC_PFAPR_M3PFD_MASK                     0x80000u
#define FNET_MK_FMC_PFAPR_M3PFD_SHIFT                    19
#define FNET_MK_FMC_PFAPR_M4PFD_MASK                     0x100000u
#define FNET_MK_FMC_PFAPR_M4PFD_SHIFT                    20
#define FNET_MK_FMC_PFAPR_M5PFD_MASK                     0x200000u
#define FNET_MK_FMC_PFAPR_M5PFD_SHIFT                    21
#define FNET_MK_FMC_PFAPR_M6PFD_MASK                     0x400000u
#define FNET_MK_FMC_PFAPR_M6PFD_SHIFT                    22
#define FNET_MK_FMC_PFAPR_M7PFD_MASK                     0x800000u
#define FNET_MK_FMC_PFAPR_M7PFD_SHIFT                    23
/* PFB0CR Bit Fields */
#define FNET_MK_FMC_PFB0CR_B0SEBE_MASK                   0x1u
#define FNET_MK_FMC_PFB0CR_B0SEBE_SHIFT                  0
#define FNET_MK_FMC_PFB0CR_B0IPE_MASK                    0x2u
#define FNET_MK_FMC_PFB0CR_B0IPE_SHIFT                   1
#define FNET_MK_FMC_PFB0CR_B0DPE_MASK                    0x4u
#define FNET_MK_FMC_PFB0CR_B0DPE_SHIFT                   2
#define FNET_MK_FMC_PFB0CR_B0ICE_MASK                    0x8u
#define FNET_MK_FMC_PFB0CR_B0ICE_SHIFT                   3
#define FNET_MK_FMC_PFB0CR_B0DCE_MASK                    0x10u
#define FNET_MK_FMC_PFB0CR_B0DCE_SHIFT                   4
#define FNET_MK_FMC_PFB0CR_CRC_MASK                      0xE0u
#define FNET_MK_FMC_PFB0CR_CRC_SHIFT                     5
#define FNET_MK_FMC_PFB0CR_CRC(x)                        (((uint32_t)(((uint32_t)(x))<<FNET_MK_FMC_PFB0CR_CRC_SHIFT))&FNET_MK_FMC_PFB0CR_CRC_MASK)
#define FNET_MK_FMC_PFB0CR_B0MW_MASK                     0x60000u
#define FNET_MK_FMC_PFB0CR_B0MW_SHIFT                    17
#define FNET_MK_FMC_PFB0CR_B0MW(x)                       (((uint32_t)(((uint32_t)(x))<<FNET_MK_FMC_PFB0CR_B0MW_SHIFT))&FNET_MK_FMC_PFB0CR_B0MW_MASK)
#define FNET_MK_FMC_PFB0CR_S_B_INV_MASK                  0x80000u
#define FNET_MK_FMC_PFB0CR_S_B_INV_SHIFT                 19
#define FNET_MK_FMC_PFB0CR_CINV_WAY_MASK                 0xF00000u
#define FNET_MK_FMC_PFB0CR_CINV_WAY_SHIFT                20
#define FNET_MK_FMC_PFB0CR_CINV_WAY(x)                   (((uint32_t)(((uint32_t)(x))<<FNET_MK_FMC_PFB0CR_CINV_WAY_SHIFT))&FNET_MK_FMC_PFB0CR_CINV_WAY_MASK)
#define FNET_MK_FMC_PFB0CR_CLCK_WAY_MASK                 0xF000000u
#define FNET_MK_FMC_PFB0CR_CLCK_WAY_SHIFT                24
#define FNET_MK_FMC_PFB0CR_CLCK_WAY(x)                   (((uint32_t)(((uint32_t)(x))<<FNET_MK_FMC_PFB0CR_CLCK_WAY_SHIFT))&FNET_MK_FMC_PFB0CR_CLCK_WAY_MASK)
#define FNET_MK_FMC_PFB0CR_B0RWSC_MASK                   0xF0000000u
#define FNET_MK_FMC_PFB0CR_B0RWSC_SHIFT                  28
#define FNET_MK_FMC_PFB0CR_B0RWSC(x)                     (((uint32_t)(((uint32_t)(x))<<FNET_MK_FMC_PFB0CR_B0RWSC_SHIFT))&FNET_MK_FMC_PFB0CR_B0RWSC_MASK)
/* PFB1CR Bit Fields */
#define FNET_MK_FMC_PFB1CR_B1SEBE_MASK                   0x1u
#define FNET_MK_FMC_PFB1CR_B1SEBE_SHIFT                  0
#define FNET_MK_FMC_PFB1CR_B1IPE_MASK                    0x2u
#define FNET_MK_FMC_PFB1CR_B1IPE_SHIFT                   1
#define FNET_MK_FMC_PFB1CR_B1DPE_MASK                    0x4u
#define FNET_MK_FMC_PFB1CR_B1DPE_SHIFT                   2
#define FNET_MK_FMC_PFB1CR_B1ICE_MASK                    0x8u
#define FNET_MK_FMC_PFB1CR_B1ICE_SHIFT                   3
#define FNET_MK_FMC_PFB1CR_B1DCE_MASK                    0x10u
#define FNET_MK_FMC_PFB1CR_B1DCE_SHIFT                   4
#define FNET_MK_FMC_PFB1CR_B1MW_MASK                     0x60000u
#define FNET_MK_FMC_PFB1CR_B1MW_SHIFT                    17
#define FNET_MK_FMC_PFB1CR_B1MW(x)                       (((uint32_t)(((uint32_t)(x))<<FNET_MK_FMC_PFB1CR_B1MW_SHIFT))&FNET_MK_FMC_PFB1CR_B1MW_MASK)
#define FNET_MK_FMC_PFB1CR_B1RWSC_MASK                   0xF0000000u
#define FNET_MK_FMC_PFB1CR_B1RWSC_SHIFT                  28
#define FNET_MK_FMC_PFB1CR_B1RWSC(x)                     (((uint32_t)(((uint32_t)(x))<<FNET_MK_FMC_PFB1CR_B1RWSC_SHIFT))&FNET_MK_FMC_PFB1CR_B1RWSC_MASK)
/* TAGVD Bit Fields */
#define FNET_MK_FMC_TAGVD_valid_MASK                     0x1u
#define FNET_MK_FMC_TAGVD_valid_SHIFT                    0
#define FNET_MK_FMC_TAGVD_tag_MASK                       0x7FFC0u
#define FNET_MK_FMC_TAGVD_tag_SHIFT                      6
#define FNET_MK_FMC_TAGVD_tag(x)                         (((uint32_t)(((uint32_t)(x))<<FNET_MK_FMC_TAGVD_tag_SHIFT))&FNET_MK_FMC_TAGVD_tag_MASK)
/* DATA_U Bit Fields */
#define FNET_MK_FMC_DATA_U_data_MASK                     0xFFFFFFFFu
#define FNET_MK_FMC_DATA_U_data_SHIFT                    0
#define FNET_MK_FMC_DATA_U_data(x)                       (((uint32_t)(((uint32_t)(x))<<FNET_MK_FMC_DATA_U_data_SHIFT))&FNET_MK_FMC_DATA_U_data_MASK)
/* DATA_L Bit Fields */
#define FNET_MK_FMC_DATA_L_data_MASK                     0xFFFFFFFFu
#define FNET_MK_FMC_DATA_L_data_SHIFT                    0
#define FNET_MK_FMC_DATA_L_data(x)                       (((uint32_t)(((uint32_t)(x))<<FNET_MK_FMC_DATA_L_data_SHIFT))&FNET_MK_FMC_DATA_L_data_MASK)


/* FMC - Peripheral instance base addresses */
/* Peripheral FMC base pointer */
#define FNET_MK_FMC_BASE_PTR                             ((FNET_MK_FMC_MemMapPtr)0x4001F000u)

/* ----------------------------------------------------------------------------
   -- FMC - Register accessor macros
   ---------------------------------------------------------------------------- */

/* FMC - Register instance definitions */
/* FMC */
#define FNET_MK_FMC_PFAPR                                FNET_MK_FMC_PFAPR_REG(FNET_MK_FMC_BASE_PTR)
#define FNET_MK_FMC_PFB0CR                               FNET_MK_FMC_PFB0CR_REG(FNET_MK_FMC_BASE_PTR)
#define FNET_MK_FMC_PFB1CR                               FNET_MK_FMC_PFB1CR_REG(FNET_MK_FMC_BASE_PTR)
#define FNET_MK_FMC_TAGVDW0S0                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,0,0)
#define FNET_MK_FMC_TAGVDW0S1                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,0,1)
#define FNET_MK_FMC_TAGVDW0S2                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,0,2)
#define FNET_MK_FMC_TAGVDW0S3                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,0,3)
#define FNET_MK_FMC_TAGVDW0S4                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,0,4)
#define FNET_MK_FMC_TAGVDW0S5                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,0,5)
#define FNET_MK_FMC_TAGVDW0S6                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,0,6)
#define FNET_MK_FMC_TAGVDW0S7                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,0,7)
#define FNET_MK_FMC_TAGVDW1S0                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,1,0)
#define FNET_MK_FMC_TAGVDW1S1                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,1,1)
#define FNET_MK_FMC_TAGVDW1S2                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,1,2)
#define FNET_MK_FMC_TAGVDW1S3                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,1,3)
#define FNET_MK_FMC_TAGVDW1S4                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,1,4)
#define FNET_MK_FMC_TAGVDW1S5                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,1,5)
#define FNET_MK_FMC_TAGVDW1S6                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,1,6)
#define FNET_MK_FMC_TAGVDW1S7                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,1,7)
#define FNET_MK_FMC_TAGVDW2S0                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,2,0)
#define FNET_MK_FMC_TAGVDW2S1                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,2,1)
#define FNET_MK_FMC_TAGVDW2S2                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,2,2)
#define FNET_MK_FMC_TAGVDW2S3                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,2,3)
#define FNET_MK_FMC_TAGVDW2S4                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,2,4)
#define FNET_MK_FMC_TAGVDW2S5                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,2,5)
#define FNET_MK_FMC_TAGVDW2S6                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,2,6)
#define FNET_MK_FMC_TAGVDW2S7                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,2,7)
#define FNET_MK_FMC_TAGVDW3S0                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,3,0)
#define FNET_MK_FMC_TAGVDW3S1                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,3,1)
#define FNET_MK_FMC_TAGVDW3S2                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,3,2)
#define FNET_MK_FMC_TAGVDW3S3                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,3,3)
#define FNET_MK_FMC_TAGVDW3S4                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,3,4)
#define FNET_MK_FMC_TAGVDW3S5                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,3,5)
#define FNET_MK_FMC_TAGVDW3S6                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,3,6)
#define FNET_MK_FMC_TAGVDW3S7                            FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,3,7)
#define FNET_MK_FMC_DATAW0S0U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,0,0)
#define FNET_MK_FMC_DATAW0S0L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,0,0)
#define FNET_MK_FMC_DATAW0S1U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,0,1)
#define FNET_MK_FMC_DATAW0S1L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,0,1)
#define FNET_MK_FMC_DATAW0S2U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,0,2)
#define FNET_MK_FMC_DATAW0S2L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,0,2)
#define FNET_MK_FMC_DATAW0S3U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,0,3)
#define FNET_MK_FMC_DATAW0S3L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,0,3)
#define FNET_MK_FMC_DATAW0S4U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,0,4)
#define FNET_MK_FMC_DATAW0S4L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,0,4)
#define FNET_MK_FMC_DATAW0S5U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,0,5)
#define FNET_MK_FMC_DATAW0S5L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,0,5)
#define FNET_MK_FMC_DATAW0S6U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,0,6)
#define FNET_MK_FMC_DATAW0S6L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,0,6)
#define FNET_MK_FMC_DATAW0S7U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,0,7)
#define FNET_MK_FMC_DATAW0S7L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,0,7)
#define FNET_MK_FMC_DATAW1S0U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,1,0)
#define FNET_MK_FMC_DATAW1S0L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,1,0)
#define FNET_MK_FMC_DATAW1S1U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,1,1)
#define FNET_MK_FMC_DATAW1S1L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,1,1)
#define FNET_MK_FMC_DATAW1S2U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,1,2)
#define FNET_MK_FMC_DATAW1S2L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,1,2)
#define FNET_MK_FMC_DATAW1S3U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,1,3)
#define FNET_MK_FMC_DATAW1S3L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,1,3)
#define FNET_MK_FMC_DATAW1S4U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,1,4)
#define FNET_MK_FMC_DATAW1S4L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,1,4)
#define FNET_MK_FMC_DATAW1S5U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,1,5)
#define FNET_MK_FMC_DATAW1S5L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,1,5)
#define FNET_MK_FMC_DATAW1S6U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,1,6)
#define FNET_MK_FMC_DATAW1S6L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,1,6)
#define FNET_MK_FMC_DATAW1S7U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,1,7)
#define FNET_MK_FMC_DATAW1S7L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,1,7)
#define FNET_MK_FMC_DATAW2S0U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,2,0)
#define FNET_MK_FMC_DATAW2S0L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,2,0)
#define FNET_MK_FMC_DATAW2S1U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,2,1)
#define FNET_MK_FMC_DATAW2S1L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,2,1)
#define FNET_MK_FMC_DATAW2S2U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,2,2)
#define FNET_MK_FMC_DATAW2S2L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,2,2)
#define FNET_MK_FMC_DATAW2S3U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,2,3)
#define FNET_MK_FMC_DATAW2S3L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,2,3)
#define FNET_MK_FMC_DATAW2S4U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,2,4)
#define FNET_MK_FMC_DATAW2S4L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,2,4)
#define FNET_MK_FMC_DATAW2S5U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,2,5)
#define FNET_MK_FMC_DATAW2S5L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,2,5)
#define FNET_MK_FMC_DATAW2S6U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,2,6)
#define FNET_MK_FMC_DATAW2S6L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,2,6)
#define FNET_MK_FMC_DATAW2S7U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,2,7)
#define FNET_MK_FMC_DATAW2S7L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,2,7)
#define FNET_MK_FMC_DATAW3S0U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,3,0)
#define FNET_MK_FMC_DATAW3S0L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,3,0)
#define FNET_MK_FMC_DATAW3S1U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,3,1)
#define FNET_MK_FMC_DATAW3S1L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,3,1)
#define FNET_MK_FMC_DATAW3S2U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,3,2)
#define FNET_MK_FMC_DATAW3S2L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,3,2)
#define FNET_MK_FMC_DATAW3S3U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,3,3)
#define FNET_MK_FMC_DATAW3S3L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,3,3)
#define FNET_MK_FMC_DATAW3S4U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,3,4)
#define FNET_MK_FMC_DATAW3S4L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,3,4)
#define FNET_MK_FMC_DATAW3S5U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,3,5)
#define FNET_MK_FMC_DATAW3S5L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,3,5)
#define FNET_MK_FMC_DATAW3S6U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,3,6)
#define FNET_MK_FMC_DATAW3S6L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,3,6)
#define FNET_MK_FMC_DATAW3S7U                            FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,3,7)
#define FNET_MK_FMC_DATAW3S7L                            FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,3,7)

/* FMC - Register array accessors */
#define FNET_MK_FMC_TAGVD(index,index2)                  FNET_MK_FMC_TAGVD_REG(FNET_MK_FMC_BASE_PTR,(index),(index2))
#define FNET_MK_FMC_DATA_U(index,index2)                 FNET_MK_FMC_DATA_U_REG(FNET_MK_FMC_BASE_PTR,(index),(index2))
#define FNET_MK_FMC_DATA_L(index,index2)                 FNET_MK_FMC_DATA_L_REG(FNET_MK_FMC_BASE_PTR,(index),(index2))

/* ----------------------------------------------------------------------------
   -- FTFL
   ---------------------------------------------------------------------------- */

/* FTFL - Peripheral register structure */
typedef struct FNET_MK_FTFL_MemMap
{
    fnet_vuint8_t FSTAT;                                   /* Flash Status Register, offset: 0x0 */
    fnet_vuint8_t FCNFG;                                   /* Flash Configuration Register, offset: 0x1 */
    fnet_vuint8_t FSEC;                                    /* Flash Security Register, offset: 0x2 */
    fnet_vuint8_t FOPT;                                    /* Flash Option Register, offset: 0x3 */
    fnet_vuint8_t FCCOB3;                                  /* Flash Common Command Object Registers, offset: 0x4 */
    fnet_vuint8_t FCCOB2;                                  /* Flash Common Command Object Registers, offset: 0x5 */
    fnet_vuint8_t FCCOB1;                                  /* Flash Common Command Object Registers, offset: 0x6 */
    fnet_vuint8_t FCCOB0;                                  /* Flash Common Command Object Registers, offset: 0x7 */
    fnet_vuint8_t FCCOB7;                                  /* Flash Common Command Object Registers, offset: 0x8 */
    fnet_vuint8_t FCCOB6;                                  /* Flash Common Command Object Registers, offset: 0x9 */
    fnet_vuint8_t FCCOB5;                                  /* Flash Common Command Object Registers, offset: 0xA */
    fnet_vuint8_t FCCOB4;                                  /* Flash Common Command Object Registers, offset: 0xB */
    fnet_vuint8_t FCCOBB;                                  /* Flash Common Command Object Registers, offset: 0xC */
    fnet_vuint8_t FCCOBA;                                  /* Flash Common Command Object Registers, offset: 0xD */
    fnet_vuint8_t FCCOB9;                                  /* Flash Common Command Object Registers, offset: 0xE */
    fnet_vuint8_t FCCOB8;                                  /* Flash Common Command Object Registers, offset: 0xF */
    fnet_vuint8_t FPROT3;                                  /* Program Flash Protection Registers, offset: 0x10 */
    fnet_vuint8_t FPROT2;                                  /* Program Flash Protection Registers, offset: 0x11 */
    fnet_vuint8_t FPROT1;                                  /* Program Flash Protection Registers, offset: 0x12 */
    fnet_vuint8_t FPROT0;                                  /* Program Flash Protection Registers, offset: 0x13 */
    fnet_vuint8_t RESERVED_0[2];
    fnet_vuint8_t FEPROT;                                  /* EEPROM Protection Register, offset: 0x16 */
    fnet_vuint8_t FDPROT;                                  /* Data Flash Protection Register, offset: 0x17 */
} volatile *FNET_MK_FTFL_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- FTFL - Register accessor macros
   ---------------------------------------------------------------------------- */

/* FTFL - Register accessors */
#define FNET_MK_FTFL_FSTAT_REG(base)                     ((base)->FSTAT)
#define FNET_MK_FTFL_FCNFG_REG(base)                     ((base)->FCNFG)
#define FNET_MK_FTFL_FSEC_REG(base)                      ((base)->FSEC)
#define FNET_MK_FTFL_FOPT_REG(base)                      ((base)->FOPT)
#define FNET_MK_FTFL_FCCOB3_REG(base)                    ((base)->FCCOB3)
#define FNET_MK_FTFL_FCCOB2_REG(base)                    ((base)->FCCOB2)
#define FNET_MK_FTFL_FCCOB1_REG(base)                    ((base)->FCCOB1)
#define FNET_MK_FTFL_FCCOB0_REG(base)                    ((base)->FCCOB0)
#define FNET_MK_FTFL_FCCOB7_REG(base)                    ((base)->FCCOB7)
#define FNET_MK_FTFL_FCCOB6_REG(base)                    ((base)->FCCOB6)
#define FNET_MK_FTFL_FCCOB5_REG(base)                    ((base)->FCCOB5)
#define FNET_MK_FTFL_FCCOB4_REG(base)                    ((base)->FCCOB4)
#define FNET_MK_FTFL_FCCOBB_REG(base)                    ((base)->FCCOBB)
#define FNET_MK_FTFL_FCCOBA_REG(base)                    ((base)->FCCOBA)
#define FNET_MK_FTFL_FCCOB9_REG(base)                    ((base)->FCCOB9)
#define FNET_MK_FTFL_FCCOB8_REG(base)                    ((base)->FCCOB8)
#define FNET_MK_FTFL_FPROT3_REG(base)                    ((base)->FPROT3)
#define FNET_MK_FTFL_FPROT2_REG(base)                    ((base)->FPROT2)
#define FNET_MK_FTFL_FPROT1_REG(base)                    ((base)->FPROT1)
#define FNET_MK_FTFL_FPROT0_REG(base)                    ((base)->FPROT0)
#define FNET_MK_FTFL_FEPROT_REG(base)                    ((base)->FEPROT)
#define FNET_MK_FTFL_FDPROT_REG(base)                    ((base)->FDPROT)


/* ----------------------------------------------------------------------------
   -- FTFL Register Masks
   ---------------------------------------------------------------------------- */

/* FSTAT Bit Fields */
#define FNET_MK_FTFL_FSTAT_MGSTAT0_MASK                  0x1u
#define FNET_MK_FTFL_FSTAT_MGSTAT0_SHIFT                 0
#define FNET_MK_FTFL_FSTAT_FPVIOL_MASK                   0x10u
#define FNET_MK_FTFL_FSTAT_FPVIOL_SHIFT                  4
#define FNET_MK_FTFL_FSTAT_ACCERR_MASK                   0x20u
#define FNET_MK_FTFL_FSTAT_ACCERR_SHIFT                  5
#define FNET_MK_FTFL_FSTAT_RDCOLERR_MASK                 0x40u
#define FNET_MK_FTFL_FSTAT_RDCOLERR_SHIFT                6
#define FNET_MK_FTFL_FSTAT_CCIF_MASK                     0x80u
#define FNET_MK_FTFL_FSTAT_CCIF_SHIFT                    7
/* FCNFG Bit Fields */
#define FNET_MK_FTFL_FCNFG_EEERDY_MASK                   0x1u
#define FNET_MK_FTFL_FCNFG_EEERDY_SHIFT                  0
#define FNET_MK_FTFL_FCNFG_RAMRDY_MASK                   0x2u
#define FNET_MK_FTFL_FCNFG_RAMRDY_SHIFT                  1
#define FNET_MK_FTFL_FCNFG_PFLSH_MASK                    0x4u
#define FNET_MK_FTFL_FCNFG_PFLSH_SHIFT                   2
#define FNET_MK_FTFL_FCNFG_SWAP_MASK                     0x8u
#define FNET_MK_FTFL_FCNFG_SWAP_SHIFT                    3
#define FNET_MK_FTFL_FCNFG_ERSSUSP_MASK                  0x10u
#define FNET_MK_FTFL_FCNFG_ERSSUSP_SHIFT                 4
#define FNET_MK_FTFL_FCNFG_ERSAREQ_MASK                  0x20u
#define FNET_MK_FTFL_FCNFG_ERSAREQ_SHIFT                 5
#define FNET_MK_FTFL_FCNFG_RDCOLLIE_MASK                 0x40u
#define FNET_MK_FTFL_FCNFG_RDCOLLIE_SHIFT                6
#define FNET_MK_FTFL_FCNFG_CCIE_MASK                     0x80u
#define FNET_MK_FTFL_FCNFG_CCIE_SHIFT                    7
/* FSEC Bit Fields */
#define FNET_MK_FTFL_FSEC_SEC_MASK                       0x3u
#define FNET_MK_FTFL_FSEC_SEC_SHIFT                      0
#define FNET_MK_FTFL_FSEC_SEC(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FSEC_SEC_SHIFT))&FNET_MK_FTFL_FSEC_SEC_MASK)
#define FNET_MK_FTFL_FSEC_FSLACC_MASK                    0xCu
#define FNET_MK_FTFL_FSEC_FSLACC_SHIFT                   2
#define FNET_MK_FTFL_FSEC_FSLACC(x)                      (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FSEC_FSLACC_SHIFT))&FNET_MK_FTFL_FSEC_FSLACC_MASK)
#define FNET_MK_FTFL_FSEC_MEEN_MASK                      0x30u
#define FNET_MK_FTFL_FSEC_MEEN_SHIFT                     4
#define FNET_MK_FTFL_FSEC_MEEN(x)                        (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FSEC_MEEN_SHIFT))&FNET_MK_FTFL_FSEC_MEEN_MASK)
#define FNET_MK_FTFL_FSEC_KEYEN_MASK                     0xC0u
#define FNET_MK_FTFL_FSEC_KEYEN_SHIFT                    6
#define FNET_MK_FTFL_FSEC_KEYEN(x)                       (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FSEC_KEYEN_SHIFT))&FNET_MK_FTFL_FSEC_KEYEN_MASK)
/* FOPT Bit Fields */
#define FNET_MK_FTFL_FOPT_OPT_MASK                       0xFFu
#define FNET_MK_FTFL_FOPT_OPT_SHIFT                      0
#define FNET_MK_FTFL_FOPT_OPT(x)                         (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FOPT_OPT_SHIFT))&FNET_MK_FTFL_FOPT_OPT_MASK)
/* FCCOB3 Bit Fields */
#define FNET_MK_FTFL_FCCOB3_CCOBn_MASK                   0xFFu
#define FNET_MK_FTFL_FCCOB3_CCOBn_SHIFT                  0
#define FNET_MK_FTFL_FCCOB3_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FCCOB3_CCOBn_SHIFT))&FNET_MK_FTFL_FCCOB3_CCOBn_MASK)
/* FCCOB2 Bit Fields */
#define FNET_MK_FTFL_FCCOB2_CCOBn_MASK                   0xFFu
#define FNET_MK_FTFL_FCCOB2_CCOBn_SHIFT                  0
#define FNET_MK_FTFL_FCCOB2_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FCCOB2_CCOBn_SHIFT))&FNET_MK_FTFL_FCCOB2_CCOBn_MASK)
/* FCCOB1 Bit Fields */
#define FNET_MK_FTFL_FCCOB1_CCOBn_MASK                   0xFFu
#define FNET_MK_FTFL_FCCOB1_CCOBn_SHIFT                  0
#define FNET_MK_FTFL_FCCOB1_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FCCOB1_CCOBn_SHIFT))&FNET_MK_FTFL_FCCOB1_CCOBn_MASK)
/* FCCOB0 Bit Fields */
#define FNET_MK_FTFL_FCCOB0_CCOBn_MASK                   0xFFu
#define FNET_MK_FTFL_FCCOB0_CCOBn_SHIFT                  0
#define FNET_MK_FTFL_FCCOB0_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FCCOB0_CCOBn_SHIFT))&FNET_MK_FTFL_FCCOB0_CCOBn_MASK)
/* FCCOB7 Bit Fields */
#define FNET_MK_FTFL_FCCOB7_CCOBn_MASK                   0xFFu
#define FNET_MK_FTFL_FCCOB7_CCOBn_SHIFT                  0
#define FNET_MK_FTFL_FCCOB7_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FCCOB7_CCOBn_SHIFT))&FNET_MK_FTFL_FCCOB7_CCOBn_MASK)
/* FCCOB6 Bit Fields */
#define FNET_MK_FTFL_FCCOB6_CCOBn_MASK                   0xFFu
#define FNET_MK_FTFL_FCCOB6_CCOBn_SHIFT                  0
#define FNET_MK_FTFL_FCCOB6_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FCCOB6_CCOBn_SHIFT))&FNET_MK_FTFL_FCCOB6_CCOBn_MASK)
/* FCCOB5 Bit Fields */
#define FNET_MK_FTFL_FCCOB5_CCOBn_MASK                   0xFFu
#define FNET_MK_FTFL_FCCOB5_CCOBn_SHIFT                  0
#define FNET_MK_FTFL_FCCOB5_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FCCOB5_CCOBn_SHIFT))&FNET_MK_FTFL_FCCOB5_CCOBn_MASK)
/* FCCOB4 Bit Fields */
#define FNET_MK_FTFL_FCCOB4_CCOBn_MASK                   0xFFu
#define FNET_MK_FTFL_FCCOB4_CCOBn_SHIFT                  0
#define FNET_MK_FTFL_FCCOB4_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FCCOB4_CCOBn_SHIFT))&FNET_MK_FTFL_FCCOB4_CCOBn_MASK)
/* FCCOBB Bit Fields */
#define FNET_MK_FTFL_FCCOBB_CCOBn_MASK                   0xFFu
#define FNET_MK_FTFL_FCCOBB_CCOBn_SHIFT                  0
#define FNET_MK_FTFL_FCCOBB_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FCCOBB_CCOBn_SHIFT))&FNET_MK_FTFL_FCCOBB_CCOBn_MASK)
/* FCCOBA Bit Fields */
#define FNET_MK_FTFL_FCCOBA_CCOBn_MASK                   0xFFu
#define FNET_MK_FTFL_FCCOBA_CCOBn_SHIFT                  0
#define FNET_MK_FTFL_FCCOBA_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FCCOBA_CCOBn_SHIFT))&FNET_MK_FTFL_FCCOBA_CCOBn_MASK)
/* FCCOB9 Bit Fields */
#define FNET_MK_FTFL_FCCOB9_CCOBn_MASK                   0xFFu
#define FNET_MK_FTFL_FCCOB9_CCOBn_SHIFT                  0
#define FNET_MK_FTFL_FCCOB9_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FCCOB9_CCOBn_SHIFT))&FNET_MK_FTFL_FCCOB9_CCOBn_MASK)
/* FCCOB8 Bit Fields */
#define FNET_MK_FTFL_FCCOB8_CCOBn_MASK                   0xFFu
#define FNET_MK_FTFL_FCCOB8_CCOBn_SHIFT                  0
#define FNET_MK_FTFL_FCCOB8_CCOBn(x)                     (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FCCOB8_CCOBn_SHIFT))&FNET_MK_FTFL_FCCOB8_CCOBn_MASK)
/* FPROT3 Bit Fields */
#define FNET_MK_FTFL_FPROT3_PROT_MASK                    0xFFu
#define FNET_MK_FTFL_FPROT3_PROT_SHIFT                   0
#define FNET_MK_FTFL_FPROT3_PROT(x)                      (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FPROT3_PROT_SHIFT))&FNET_MK_FTFL_FPROT3_PROT_MASK)
/* FPROT2 Bit Fields */
#define FNET_MK_FTFL_FPROT2_PROT_MASK                    0xFFu
#define FNET_MK_FTFL_FPROT2_PROT_SHIFT                   0
#define FNET_MK_FTFL_FPROT2_PROT(x)                      (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FPROT2_PROT_SHIFT))&FNET_MK_FTFL_FPROT2_PROT_MASK)
/* FPROT1 Bit Fields */
#define FNET_MK_FTFL_FPROT1_PROT_MASK                    0xFFu
#define FNET_MK_FTFL_FPROT1_PROT_SHIFT                   0
#define FNET_MK_FTFL_FPROT1_PROT(x)                      (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FPROT1_PROT_SHIFT))&FNET_MK_FTFL_FPROT1_PROT_MASK)
/* FPROT0 Bit Fields */
#define FNET_MK_FTFL_FPROT0_PROT_MASK                    0xFFu
#define FNET_MK_FTFL_FPROT0_PROT_SHIFT                   0
#define FNET_MK_FTFL_FPROT0_PROT(x)                      (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FPROT0_PROT_SHIFT))&FNET_MK_FTFL_FPROT0_PROT_MASK)
/* FEPROT Bit Fields */
#define FNET_MK_FTFL_FEPROT_EPROT_MASK                   0xFFu
#define FNET_MK_FTFL_FEPROT_EPROT_SHIFT                  0
#define FNET_MK_FTFL_FEPROT_EPROT(x)                     (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FEPROT_EPROT_SHIFT))&FNET_MK_FTFL_FEPROT_EPROT_MASK)
/* FDPROT Bit Fields */
#define FNET_MK_FTFL_FDPROT_DPROT_MASK                   0xFFu
#define FNET_MK_FTFL_FDPROT_DPROT_SHIFT                  0
#define FNET_MK_FTFL_FDPROT_DPROT(x)                     (((uint8_t)(((uint8_t)(x))<<FNET_MK_FTFL_FDPROT_DPROT_SHIFT))&FNET_MK_FTFL_FDPROT_DPROT_MASK)


/* FTFL - Peripheral instance base addresses */
/* Peripheral FTFL base pointer */
#define FNET_MK_FTFL_BASE_PTR                            ((FNET_MK_FTFL_MemMapPtr)0x40020000u)

/* ----------------------------------------------------------------------------
   -- FTFL - Register accessor macros
   ---------------------------------------------------------------------------- */

/* FTFL - Register instance definitions */
/* FTFL */
#define FNET_MK_FTFL_FOPT                                FNET_MK_FTFL_FOPT_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FSTAT                               FNET_MK_FTFL_FSTAT_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FCNFG                               FNET_MK_FTFL_FCNFG_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FSEC                                FNET_MK_FTFL_FSEC_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FCCOB3                              FNET_MK_FTFL_FCCOB3_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FCCOB2                              FNET_MK_FTFL_FCCOB2_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FCCOB1                              FNET_MK_FTFL_FCCOB1_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FCCOB0                              FNET_MK_FTFL_FCCOB0_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FCCOB7                              FNET_MK_FTFL_FCCOB7_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FCCOB6                              FNET_MK_FTFL_FCCOB6_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FCCOB5                              FNET_MK_FTFL_FCCOB5_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FCCOB4                              FNET_MK_FTFL_FCCOB4_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FCCOBB                              FNET_MK_FTFL_FCCOBB_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FCCOBA                              FNET_MK_FTFL_FCCOBA_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FCCOB9                              FNET_MK_FTFL_FCCOB9_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FCCOB8                              FNET_MK_FTFL_FCCOB8_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FPROT3                              FNET_MK_FTFL_FPROT3_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FPROT2                              FNET_MK_FTFL_FPROT2_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FPROT1                              FNET_MK_FTFL_FPROT1_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FPROT0                              FNET_MK_FTFL_FPROT0_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FDPROT                              FNET_MK_FTFL_FDPROT_REG(FNET_MK_FTFL_BASE_PTR)
#define FNET_MK_FTFL_FEPROT                              FNET_MK_FTFL_FEPROT_REG(FNET_MK_FTFL_BASE_PTR)


/* Command definitions for FNET_FTFL_FCCOB0 */
#define FNET_MK_FNET_FTFL_FCCOB0_CMD_READ_BLOCK             (0x00u)
#define FNET_MK_FNET_FTFL_FCCOB0_CMD_READ_SECTION           (0x01u)
#define FNET_MK_FNET_FTFL_FCCOB0_CMD_PROGRAM_CHECK          (0x02u)
#define FNET_MK_FNET_FTFL_FCCOB0_CMD_READ_RESOURCE          (0x03u)
#define FNET_MK_FNET_FTFL_FCCOB0_CMD_PROGRAM_LONGWORD       (0x06u)
#define FNET_MK_FNET_FTFL_FCCOB0_CMD_PROGRAM_PHRASE         (0x07u) /*Program 8 bytes in a program flash block or a data flash block. FTFE K70 */
#define FNET_MK_FNET_FTFL_FCCOB0_CMD_ERASE_BLOCK            (0x08u)
#define FNET_MK_FNET_FTFL_FCCOB0_CMD_ERASE_SECTOR           (0x09u)
#define FNET_MK_FNET_FTFL_FCCOB0_CMD_PROGRAM_SECTION        (0x0Bu)
#define FNET_MK_FNET_FTFL_FCCOB0_CMD_READ_BLOCKS            (0x40u)
#define FNET_MK_FNET_FTFL_FCCOB0_CMD_READ_ONCE              (0x41u)
#define FNET_MK_FNET_FTFL_FCCOB0_CMD_PROGRAM_ONCE           (0x43u)
#define FNET_MK_FNET_FTFL_FCCOB0_CMD_ERASE_BLOCKS           (0x44u)
#define FNET_MK_FNET_FTFL_FCCOB0_CMD_VERIFY_KEY             (0x45u)
#define FNET_MK_FNET_FTFL_FCCOB0_CMD_PROGRAM_PARTITION      (0x80u)

/* ----------------------------------------------------------------------------
   -- SCB
   ---------------------------------------------------------------------------- */

/* SCB - Peripheral register structure */
typedef struct FNET_MK_SCB_MemMap
{
    fnet_vuint8_t RESERVED_0[8];
    fnet_vuint32_t ACTLR;                                  /* Auxiliary Control Register,, offset: 0x8 */
    fnet_vuint8_t RESERVED_1[3316];
    fnet_vuint32_t CPUID;                                  /* CPUID Base Register, offset: 0xD00 */
    fnet_vuint32_t ICSR;                                   /* Interrupt Control and State Register, offset: 0xD04 */
    fnet_vuint32_t VTOR;                                   /* Vector Table Offset Register, offset: 0xD08 */
    fnet_vuint32_t AIRCR;                                  /* Application Interrupt and Reset Control Register, offset: 0xD0C */
    fnet_vuint32_t SCR;                                    /* System Control Register, offset: 0xD10 */
    fnet_vuint32_t CCR;                                    /* Configuration and Control Register, offset: 0xD14 */
    fnet_vuint32_t SHPR1;                                  /* System Handler Priority Register 1, offset: 0xD18 */
    fnet_vuint32_t SHPR2;                                  /* System Handler Priority Register 2, offset: 0xD1C */
    fnet_vuint32_t SHPR3;                                  /* System Handler Priority Register 3, offset: 0xD20 */
    fnet_vuint32_t SHCSR;                                  /* System Handler Control and State Register, offset: 0xD24 */
    fnet_vuint32_t CFSR;                                   /* Configurable Fault Status Registers, offset: 0xD28 */
    fnet_vuint32_t HFSR;                                   /* HardFault Status register, offset: 0xD2C */
    fnet_vuint32_t DFSR;                                   /* Debug Fault Status Register, offset: 0xD30 */
    fnet_vuint32_t MMFAR;                                  /* MemManage Address Register, offset: 0xD34 */
    fnet_vuint32_t BFAR;                                   /* BusFault Address Register, offset: 0xD38 */
    fnet_vuint32_t AFSR;                                   /* Auxiliary Fault Status Register, offset: 0xD3C */
} volatile *FNET_MK_SCB_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- SCB - Register accessor macros
   ---------------------------------------------------------------------------- */

/* SCB - Register accessors */
#define FNET_MK_SCB_ACTLR_REG(base)                      ((base)->ACTLR)
#define FNET_MK_SCB_CPUID_REG(base)                      ((base)->CPUID)
#define FNET_MK_SCB_ICSR_REG(base)                       ((base)->ICSR)
#define FNET_MK_SCB_VTOR_REG(base)                       ((base)->VTOR)
#define FNET_MK_SCB_AIRCR_REG(base)                      ((base)->AIRCR)
#define FNET_MK_SCB_SCR_REG(base)                        ((base)->SCR)
#define FNET_MK_SCB_CCR_REG(base)                        ((base)->CCR)
#define FNET_MK_SCB_SHPR1_REG(base)                      ((base)->SHPR1)
#define FNET_MK_SCB_SHPR2_REG(base)                      ((base)->SHPR2)
#define FNET_MK_SCB_SHPR3_REG(base)                      ((base)->SHPR3)
#define FNET_MK_SCB_SHCSR_REG(base)                      ((base)->SHCSR)
#define FNET_MK_SCB_CFSR_REG(base)                       ((base)->CFSR)
#define FNET_MK_SCB_HFSR_REG(base)                       ((base)->HFSR)
#define FNET_MK_SCB_DFSR_REG(base)                       ((base)->DFSR)
#define FNET_MK_SCB_MMFAR_REG(base)                      ((base)->MMFAR)
#define FNET_MK_SCB_BFAR_REG(base)                       ((base)->BFAR)
#define FNET_MK_SCB_AFSR_REG(base)                       ((base)->AFSR)

/* ----------------------------------------------------------------------------
   -- SCB Register Masks
   ---------------------------------------------------------------------------- */

/* ACTLR Bit Fields */
#define FNET_MK_SCB_ACTLR_DISMCYCINT_MASK                0x1u
#define FNET_MK_SCB_ACTLR_DISMCYCINT_SHIFT               0
#define FNET_MK_SCB_ACTLR_DISDEFWBUF_MASK                0x2u
#define FNET_MK_SCB_ACTLR_DISDEFWBUF_SHIFT               1
#define FNET_MK_SCB_ACTLR_DISFOLD_MASK                   0x4u
#define FNET_MK_SCB_ACTLR_DISFOLD_SHIFT                  2
/* CPUID Bit Fields */
#define FNET_MK_SCB_CPUID_REVISION_MASK                  0xFu
#define FNET_MK_SCB_CPUID_REVISION_SHIFT                 0
#define FNET_MK_SCB_CPUID_REVISION(x)                    (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SCB_CPUID_REVISION_SHIFT))&FNET_MK_SCB_CPUID_REVISION_MASK)
#define FNET_MK_SCB_CPUID_PARTNO_MASK                    0xFFF0u
#define FNET_MK_SCB_CPUID_PARTNO_SHIFT                   4
#define FNET_MK_SCB_CPUID_PARTNO(x)                      (((fnet_uint32_t)(((fnrt_uint32)(x))<<FNET_MK_SCB_CPUID_PARTNO_SHIFT))&FNET_MK_SCB_CPUID_PARTNO_MASK)
#define FNET_MK_SCB_CPUID_VARIANT_MASK                   0xF00000u
#define FNET_MK_SCB_CPUID_VARIANT_SHIFT                  20
#define FNET_MK_SCB_CPUID_VARIANT(x)                     (((fnrt_uint32)(((fnet_uint32_t)(x))<<FNET_MK_SCB_CPUID_VARIANT_SHIFT))&FNET_MK_SCB_CPUID_VARIANT_MASK)
#define FNET_MK_SCB_CPUID_IMPLEMENTER_MASK               0xFF000000u
#define FNET_MK_SCB_CPUID_IMPLEMENTER_SHIFT              24
#define FNET_MK_SCB_CPUID_IMPLEMENTER(x)                 (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SCB_CPUID_IMPLEMENTER_SHIFT))&FNET_MK_SCB_CPUID_IMPLEMENTER_MASK)
/* ICSR Bit Fields */
#define FNET_MK_SCB_ICSR_VECTACTIVE_MASK                 0x1FFu
#define FNET_MK_SCB_ICSR_VECTACTIVE_SHIFT                0
#define FNET_MK_SCB_ICSR_VECTACTIVE(x)                   (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SCB_ICSR_VECTACTIVE_SHIFT))&FNET_MK_SCB_ICSR_VECTACTIVE_MASK)
#define FNET_MK_SCB_ICSR_RETTOBASE_MASK                  0x800u
#define FNET_MK_SCB_ICSR_RETTOBASE_SHIFT                 11
#define FNET_MK_SCB_ICSR_VECTPENDING_MASK                0x3F000u
#define FNET_MK_SCB_ICSR_VECTPENDING_SHIFT               12
#define FNET_MK_SCB_ICSR_VECTPENDING(x)                  (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SCB_ICSR_VECTPENDING_SHIFT))&FNET_MK_SCB_ICSR_VECTPENDING_MASK)
#define FNET_MK_SCB_ICSR_ISRPENDING_MASK                 0x400000u
#define FNET_MK_SCB_ICSR_ISRPENDING_SHIFT                22
#define FNET_MK_SCB_ICSR_ISRPREEMPT_MASK                 0x800000u
#define FNET_MK_SCB_ICSR_ISRPREEMPT_SHIFT                23
#define FNET_MK_SCB_ICSR_PENDSTCLR_MASK                  0x2000000u
#define FNET_MK_SCB_ICSR_PENDSTCLR_SHIFT                 25
#define FNET_MK_SCB_ICSR_PENDSTSET_MASK                  0x4000000u
#define FNET_MK_SCB_ICSR_PENDSTSET_SHIFT                 26
#define FNET_MK_SCB_ICSR_PENDSVCLR_MASK                  0x8000000u
#define FNET_MK_SCB_ICSR_PENDSVCLR_SHIFT                 27
#define FNET_MK_SCB_ICSR_PENDSVSET_MASK                  0x10000000u
#define FNET_MK_SCB_ICSR_PENDSVSET_SHIFT                 28
#define FNET_MK_SCB_ICSR_NMIPENDSET_MASK                 0x80000000u
#define FNET_MK_SCB_ICSR_NMIPENDSET_SHIFT                31
/* VTOR Bit Fields */
#define FNET_MK_SCB_VTOR_TBLOFF_MASK                     0xFFFFFF80u
#define FNET_MK_SCB_VTOR_TBLOFF_SHIFT                    7
#define FNET_MK_SCB_VTOR_TBLOFF(x)                       (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SCB_VTOR_TBLOFF_SHIFT))&FNET_MK_SCB_VTOR_TBLOFF_MASK)
/* AIRCR Bit Fields */
#define FNET_MK_SCB_AIRCR_VECTRESET_MASK                 0x1u
#define FNET_MK_SCB_AIRCR_VECTRESET_SHIFT                0
#define FNET_MK_SCB_AIRCR_VECTCLRACTIVE_MASK             0x2u
#define FNET_MK_SCB_AIRCR_VECTCLRACTIVE_SHIFT            1
#define FNET_MK_SCB_AIRCR_SYSRESETREQ_MASK               0x4u
#define FNET_MK_SCB_AIRCR_SYSRESETREQ_SHIFT              2
#define FNET_MK_SCB_AIRCR_PRIGROUP_MASK                  0x700u
#define FNET_MK_SCB_AIRCR_PRIGROUP_SHIFT                 8
#define FNET_MK_SCB_AIRCR_PRIGROUP(x)                    (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SCB_AIRCR_PRIGROUP_SHIFT))&FNET_MK_SCB_AIRCR_PRIGROUP_MASK)
#define FNET_MK_SCB_AIRCR_ENDIANNESS_MASK                0x8000u
#define FNET_MK_SCB_AIRCR_ENDIANNESS_SHIFT               15
#define FNET_MK_SCB_AIRCR_VECTKEY_MASK                   0xFFFF0000u
#define FNET_MK_SCB_AIRCR_VECTKEY_SHIFT                  16
#define FNET_MK_SCB_AIRCR_VECTKEY(x)                     (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SCB_AIRCR_VECTKEY_SHIFT))&FNET_MK_SCB_AIRCR_VECTKEY_MASK)
/* SCR Bit Fields */
#define FNET_MK_SCB_SCR_SLEEPONEXIT_MASK                 0x2u
#define FNET_MK_SCB_SCR_SLEEPONEXIT_SHIFT                1
#define FNET_MK_SCB_SCR_SLEEPDEEP_MASK                   0x4u
#define FNET_MK_SCB_SCR_SLEEPDEEP_SHIFT                  2
#define FNET_MK_SCB_SCR_SEVONPEND_MASK                   0x10u
#define FNET_MK_SCB_SCR_SEVONPEND_SHIFT                  4
/* CCR Bit Fields */
#define FNET_MK_SCB_CCR_NONBASETHRDENA_MASK              0x1u
#define FNET_MK_SCB_CCR_NONBASETHRDENA_SHIFT             0
#define FNET_MK_SCB_CCR_USERSETMPEND_MASK                0x2u
#define FNET_MK_SCB_CCR_USERSETMPEND_SHIFT               1
#define FNET_MK_SCB_CCR_UNALIGN_TRP_MASK                 0x8u
#define FNET_MK_SCB_CCR_UNALIGN_TRP_SHIFT                3
#define FNET_MK_SCB_CCR_DIV_0_TRP_MASK                   0x10u
#define FNET_MK_SCB_CCR_DIV_0_TRP_SHIFT                  4
#define FNET_MK_SCB_CCR_BFHFNMIGN_MASK                   0x100u
#define FNET_MK_SCB_CCR_BFHFNMIGN_SHIFT                  8
#define FNET_MK_SCB_CCR_STKALIGN_MASK                    0x200u
#define FNET_MK_SCB_CCR_STKALIGN_SHIFT                   9
/* SHPR1 Bit Fields */
#define FNET_MK_SCB_SHPR1_PRI_4_MASK                     0xFFu
#define FNET_MK_SCB_SHPR1_PRI_4_SHIFT                    0
#define FNET_MK_SCB_SHPR1_PRI_4(x)                       (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SCB_SHPR1_PRI_4_SHIFT))&FNET_MK_SCB_SHPR1_PRI_4_MASK)
#define FNET_MK_SCB_SHPR1_PRI_5_MASK                     0xFF00u
#define FNET_MK_SCB_SHPR1_PRI_5_SHIFT                    8
#define FNET_MK_SCB_SHPR1_PRI_5(x)                       (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SCB_SHPR1_PRI_5_SHIFT))&FNET_MK_SCB_SHPR1_PRI_5_MASK)
#define FNET_MK_SCB_SHPR1_PRI_6_MASK                     0xFF0000u
#define FNET_MK_SCB_SHPR1_PRI_6_SHIFT                    16
#define FNET_MK_SCB_SHPR1_PRI_6(x)                       (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SCB_SHPR1_PRI_6_SHIFT))&FNET_MK_SCB_SHPR1_PRI_6_MASK)
/* SHPR2 Bit Fields */
#define FNET_MK_SCB_SHPR2_PRI_11_MASK                    0xFF000000u
#define FNET_MK_SCB_SHPR2_PRI_11_SHIFT                   24
#define FNET_MK_SCB_SHPR2_PRI_11(x)                      (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SCB_SHPR2_PRI_11_SHIFT))&FNET_MK_SCB_SHPR2_PRI_11_MASK)
/* SHPR3 Bit Fields */
#define FNET_MK_SCB_SHPR3_PRI_14_MASK                    0xFF0000u
#define FNET_MK_SCB_SHPR3_PRI_14_SHIFT                   16
#define FNET_MK_SCB_SHPR3_PRI_14(x)                      (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SCB_SHPR3_PRI_14_SHIFT))&FNET_MK_SCB_SHPR3_PRI_14_MASK)
#define FNET_MK_SCB_SHPR3_PRI_15_MASK                    0xFF000000u
#define FNET_MK_SCB_SHPR3_PRI_15_SHIFT                   24
#define FNET_MK_SCB_SHPR3_PRI_15(x)                      (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SCB_SHPR3_PRI_15_SHIFT))&FNET_MK_SCB_SHPR3_PRI_15_MASK)
/* SHCSR Bit Fields */
#define FNET_MK_SCB_SHCSR_MEMFAULTACT_MASK               0x1u
#define FNET_MK_SCB_SHCSR_MEMFAULTACT_SHIFT              0
#define FNET_MK_SCB_SHCSR_BUSFAULTACT_MASK               0x2u
#define FNET_MK_SCB_SHCSR_BUSFAULTACT_SHIFT              1
#define FNET_MK_SCB_SHCSR_USGFAULTACT_MASK               0x8u
#define FNET_MK_SCB_SHCSR_USGFAULTACT_SHIFT              3
#define FNET_MK_SCB_SHCSR_SVCALLACT_MASK                 0x80u
#define FNET_MK_SCB_SHCSR_SVCALLACT_SHIFT                7
#define FNET_MK_SCB_SHCSR_MONITORACT_MASK                0x100u
#define FNET_MK_SCB_SHCSR_MONITORACT_SHIFT               8
#define FNET_MK_SCB_SHCSR_PENDSVACT_MASK                 0x400u
#define FNET_MK_SCB_SHCSR_PENDSVACT_SHIFT                10
#define FNET_MK_SCB_SHCSR_SYSTICKACT_MASK                0x800u
#define FNET_MK_SCB_SHCSR_SYSTICKACT_SHIFT               11
#define FNET_MK_SCB_SHCSR_USGFAULTPENDED_MASK            0x1000u
#define FNET_MK_SCB_SHCSR_USGFAULTPENDED_SHIFT           12
#define FNET_MK_SCB_SHCSR_MEMFAULTPENDED_MASK            0x2000u
#define FNET_MK_SCB_SHCSR_MEMFAULTPENDED_SHIFT           13
#define FNET_MK_SCB_SHCSR_BUSFAULTPENDED_MASK            0x4000u
#define FNET_MK_SCB_SHCSR_BUSFAULTPENDED_SHIFT           14
#define FNET_MK_SCB_SHCSR_SVCALLPENDED_MASK              0x8000u
#define FNET_MK_SCB_SHCSR_SVCALLPENDED_SHIFT             15
#define FNET_MK_SCB_SHCSR_MEMFAULTENA_MASK               0x10000u
#define FNET_MK_SCB_SHCSR_MEMFAULTENA_SHIFT              16
#define FNET_MK_SCB_SHCSR_BUSFAULTENA_MASK               0x20000u
#define FNET_MK_SCB_SHCSR_BUSFAULTENA_SHIFT              17
#define FNET_MK_SCB_SHCSR_USGFAULTENA_MASK               0x40000u
#define FNET_MK_SCB_SHCSR_USGFAULTENA_SHIFT              18
/* CFSR Bit Fields */
#define FNET_MK_SCB_CFSR_IACCVIOL_MASK                   0x1u
#define FNET_MK_SCB_CFSR_IACCVIOL_SHIFT                  0
#define FNET_MK_SCB_CFSR_DACCVIOL_MASK                   0x2u
#define FNET_MK_SCB_CFSR_DACCVIOL_SHIFT                  1
#define FNET_MK_SCB_CFSR_MUNSTKERR_MASK                  0x8u
#define FNET_MK_SCB_CFSR_MUNSTKERR_SHIFT                 3
#define FNET_MK_SCB_CFSR_MSTKERR_MASK                    0x10u
#define FNET_MK_SCB_CFSR_MSTKERR_SHIFT                   4
#define FNET_MK_SCB_CFSR_MLSPERR_MASK                    0x20u
#define FNET_MK_SCB_CFSR_MLSPERR_SHIFT                   5
#define FNET_MK_SCB_CFSR_MMARVALID_MASK                  0x80u
#define FNET_MK_SCB_CFSR_MMARVALID_SHIFT                 7
#define FNET_MK_SCB_CFSR_IBUSERR_MASK                    0x100u
#define FNET_MK_SCB_CFSR_IBUSERR_SHIFT                   8
#define FNET_MK_SCB_CFSR_PRECISERR_MASK                  0x200u
#define FNET_MK_SCB_CFSR_PRECISERR_SHIFT                 9
#define FNET_MK_SCB_CFSR_IMPRECISERR_MASK                0x400u
#define FNET_MK_SCB_CFSR_IMPRECISERR_SHIFT               10
#define FNET_MK_SCB_CFSR_UNSTKERR_MASK                   0x800u
#define FNET_MK_SCB_CFSR_UNSTKERR_SHIFT                  11
#define FNET_MK_SCB_CFSR_STKERR_MASK                     0x1000u
#define FNET_MK_SCB_CFSR_STKERR_SHIFT                    12
#define FNET_MK_SCB_CFSR_LSPERR_MASK                     0x2000u
#define FNET_MK_SCB_CFSR_LSPERR_SHIFT                    13
#define FNET_MK_SCB_CFSR_BFARVALID_MASK                  0x8000u
#define FNET_MK_SCB_CFSR_BFARVALID_SHIFT                 15
#define FNET_MK_SCB_CFSR_UNDEFINSTR_MASK                 0x10000u
#define FNET_MK_SCB_CFSR_UNDEFINSTR_SHIFT                16
#define FNET_MK_SCB_CFSR_INVSTATE_MASK                   0x20000u
#define FNET_MK_SCB_CFSR_INVSTATE_SHIFT                  17
#define FNET_MK_SCB_CFSR_INVPC_MASK                      0x40000u
#define FNET_MK_SCB_CFSR_INVPC_SHIFT                     18
#define FNET_MK_SCB_CFSR_NOCP_MASK                       0x80000u
#define FNET_MK_SCB_CFSR_NOCP_SHIFT                      19
#define FNET_MK_SCB_CFSR_UNALIGNED_MASK                  0x1000000u
#define FNET_MK_SCB_CFSR_UNALIGNED_SHIFT                 24
#define FNET_MK_SCB_CFSR_DIVBYZERO_MASK                  0x2000000u
#define FNET_MK_SCB_CFSR_DIVBYZERO_SHIFT                 25
/* HFSR Bit Fields */
#define FNET_MK_SCB_HFSR_VECTTBL_MASK                    0x2u
#define FNET_MK_SCB_HFSR_VECTTBL_SHIFT                   1
#define FNET_MK_SCB_HFSR_FORCED_MASK                     0x40000000u
#define FNET_MK_SCB_HFSR_FORCED_SHIFT                    30
#define FNET_MK_SCB_HFSR_DEBUGEVT_MASK                   0x80000000u
#define FNET_MK_SCB_HFSR_DEBUGEVT_SHIFT                  31
/* DFSR Bit Fields */
#define FNET_MK_SCB_DFSR_HALTED_MASK                     0x1u
#define FNET_MK_SCB_DFSR_HALTED_SHIFT                    0
#define FNET_MK_SCB_DFSR_BKPT_MASK                       0x2u
#define FNET_MK_SCB_DFSR_BKPT_SHIFT                      1
#define FNET_MK_SCB_DFSR_DWTTRAP_MASK                    0x4u
#define FNET_MK_SCB_DFSR_DWTTRAP_SHIFT                   2
#define FNET_MK_SCB_DFSR_VCATCH_MASK                     0x8u
#define FNET_MK_SCB_DFSR_VCATCH_SHIFT                    3
#define FNET_MK_SCB_DFSR_EXTERNAL_MASK                   0x10u
#define FNET_MK_SCB_DFSR_EXTERNAL_SHIFT                  4
/* MMFAR Bit Fields */
#define FNET_MK_SCB_MMFAR_ADDRESS_MASK                   0xFFFFFFFFu
#define FNET_MK_SCB_MMFAR_ADDRESS_SHIFT                  0
#define FNET_MK_SCB_MMFAR_ADDRESS(x)                     (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SCB_MMFAR_ADDRESS_SHIFT))&FNET_MK_SCB_MMFAR_ADDRESS_MASK)
/* BFAR Bit Fields */
#define FNET_MK_SCB_BFAR_ADDRESS_MASK                    0xFFFFFFFFu
#define FNET_MK_SCB_BFAR_ADDRESS_SHIFT                   0
#define FNET_MK_SCB_BFAR_ADDRESS(x)                      (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SCB_BFAR_ADDRESS_SHIFT))&FNET_MK_SCB_BFAR_ADDRESS_MASK)
/* AFSR Bit Fields */
#define FNET_MK_SCB_AFSR_AUXFAULT_MASK                   0xFFFFFFFFu
#define FNET_MK_SCB_AFSR_AUXFAULT_SHIFT                  0
#define FNET_MK_SCB_AFSR_AUXFAULT(x)                     (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MK_SCB_AFSR_AUXFAULT_SHIFT))&FNET_MK_SCB_AFSR_AUXFAULT_MASK)


/* SCB - Peripheral instance base addresses */
/* Peripheral SystemControl base pointer */
#define FNET_MK_SystemControl_BASE_PTR                   ((FNET_MK_SCB_MemMapPtr)0xE000E000u)

/* ----------------------------------------------------------------------------
   -- SCB - Register accessor macros
   ---------------------------------------------------------------------------- */

/* SCB - Register instance definitions */
/* SystemControl */
#define FNET_MK_SCB_ACTLR                                FNET_MK_SCB_ACTLR_REG(FNET_MK_SystemControl_BASE_PTR)
#define FNET_MK_SCB_CPUID                                FNET_MK_SCB_CPUID_REG(FNET_MK_SystemControl_BASE_PTR)
#define FNET_MK_SCB_ICSR                                 FNET_MK_SCB_ICSR_REG(FNET_MK_SystemControl_BASE_PTR)
#define FNET_MK_SCB_AIRCR                                FNET_MK_SCB_AIRCR_REG(FNET_MK_SystemControl_BASE_PTR)
#define FNET_MK_SCB_VTOR                                 FNET_MK_SCB_VTOR_REG(FNET_MK_SystemControl_BASE_PTR)
#define FNET_MK_SCB_SCR                                  FNET_MK_SCB_SCR_REG(FNET_MK_SystemControl_BASE_PTR)
#define FNET_MK_SCB_CCR                                  FNET_MK_SCB_CCR_REG(FNET_MK_SystemControl_BASE_PTR)
#define FNET_MK_SCB_SHPR1                                FNET_MK_SCB_SHPR1_REG(FNET_MK_SystemControl_BASE_PTR)
#define FNET_MK_SCB_SHPR2                                FNET_MK_SCB_SHPR2_REG(FNET_MK_SystemControl_BASE_PTR)
#define FNET_MK_SCB_SHPR3                                FNET_MK_SCB_SHPR3_REG(FNET_MK_SystemControl_BASE_PTR)
#define FNET_MK_SCB_SHCSR                                FNET_MK_SCB_SHCSR_REG(FNET_MK_SystemControl_BASE_PTR)
#define FNET_MK_SCB_CFSR                                 FNET_MK_SCB_CFSR_REG(FNET_MK_SystemControl_BASE_PTR)
#define FNET_MK_SCB_HFSR                                 FNET_MK_SCB_HFSR_REG(FNET_MK_SystemControl_BASE_PTR)
#define FNET_MK_SCB_DFSR                                 FNET_MK_SCB_DFSR_REG(FNET_MK_SystemControl_BASE_PTR)
#define FNET_MK_SCB_MMFAR                                FNET_MK_SCB_MMFAR_REG(FNET_MK_SystemControl_BASE_PTR)
#define FNET_MK_SCB_BFAR                                 FNET_MK_SCB_BFAR_REG(FNET_MK_SystemControl_BASE_PTR)
#define FNET_MK_SCB_AFSR                                 FNET_MK_SCB_AFSR_REG(FNET_MK_SystemControl_BASE_PTR)


/*********************************************************************
* Fast Ethernet Controller (FEC)
*********************************************************************/
#define FNET_FEC0_BASE_ADDR                             (0x400C0004u)

#if defined(__cplusplus)
extern "C" {
#endif

void fnet_mk_irq_enable(fnet_uint32_t irq_desc);
fnet_uint32_t fnet_mk_irq_disable(void);

#if defined(__cplusplus)
}
#endif

#endif /* FNET_MK */

#endif /*_FNET_MK_H_*/
























