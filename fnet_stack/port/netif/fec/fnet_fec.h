/**************************************************************************
*
* Copyright 2008-2018 by Andrey Butok. FNET Community.
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
***************************************************************************
*
*  FEC/ENET module driver definitions.
*
***************************************************************************/
#ifndef _FNET_FEC_H_
#define _FNET_FEC_H_

#include "fnet_config.h"
#if (FNET_MCF || FNET_MK || FNET_MPC || FNET_MIMXRT) && (FNET_CFG_CPU_ETH0 ||FNET_CFG_CPU_ETH1)

#include "fnet.h"
#include "stack/fnet_eth_prv.h"
#include "stack/fnet_error.h"
#include "stack/fnet_debug.h"
#include "stack/fnet_isr.h"
#include "stack/fnet_prot.h"
#include "stack/fnet_arp_prv.h"
#include "stack/fnet_timer_prv.h"
#include "stack/fnet_loop.h"

/* CPU-specific configuration.*/
#if FNET_MCF    /* ColdFire.*/
    #define FNET_FEC_CLOCK_KHZ  FNET_CPU_CLOCK_KHZ
    /* Transmit buffer descriptor queue. This pointer
    * must be 32-bit aligned; however, it is recommended it be
    * made 128-bit aligned (evenly divisible by 16).*/
    #define FNET_FEC_BUF_DESC_DIV       (16U)
    /* The transmit buffer pointer, containing the address
    * of the associated data buffer,
    * must always be evenly divisible by 4.*/
    #define FNET_FEC_TX_BUF_DIV         (4U)
    /* The receive buffer pointer, containing the address
    * of the associated data buffer,
    * must always be evenly divisible by 16.*/
    #define FNET_FEC_RX_BUF_DIV         (16U)
#endif

#if FNET_MK     /* Kinetis.*/
    #define FNET_FEC_CLOCK_KHZ  FNET_MK_PERIPH_CLOCK_KHZ
    /* Transmit buffer descriptor queue. This pointer
    * must be 32-bit aligned; however, it is recommended it be
    * made 128-bit aligned (evenly divisible by 16).*/
    #define FNET_FEC_BUF_DESC_DIV       (16U)
    /* The transmit buffer pointer, containing the address
    * of the associated data buffer,
    * must always be evenly divisible by 8.*/
    #define FNET_FEC_TX_BUF_DIV         (8U)
    /* The receive buffer pointer, containing the address
    * of the associated data buffer,
    * must always be evenly divisible by 16.*/
    #define FNET_FEC_RX_BUF_DIV         (16U)
#endif

#if FNET_MIMXRT     /* i.MX-RT*/
    #define FNET_FEC_CLOCK_KHZ  FNET_CPU_CLOCK_KHZ

    /* This pointer must be 64-bit aligned (bits 2–0 must be zero); however, for
    optimal performance the pointer should be 512-bit aligned, that is, evenly divisible by 64.*/
    #define FNET_FEC_BUF_DESC_DIV       (64U)
    /* For optimal performance, make the transmit buffer pointer evenly
    divisible by 64.*/
    #define FNET_FEC_TX_BUF_DIV         (64U)
    /* The receive buffer pointer, containing the address of the associated data buffer,
    must always be evenly divisible by 64..*/
    #define FNET_FEC_RX_BUF_DIV         (64U)
#endif


#if FNET_MPC
    #if FNET_CFG_CPU_MPC564xBC /* Bolero3M.*/
        #define FNET_FEC_CLOCK_KHZ  (FNET_CPU_CLOCK_KHZ/2)
        /* Transmit buffer descriptor queue. This pointer
        * must be 32-bit aligned; however, it is recommended it be
        * made 128-bit aligned (evenly divisible by 16).*/
        #define FNET_FEC_BUF_DESC_DIV       (16U)
        /* The transmit buffer pointer, containing the address
        * of the associated data buffer,
        * must always be evenly divisible by 4.*/
        #define FNET_FEC_TX_BUF_DIV         (8U)
        /* The receive buffer pointer, containing the address
        * of the associated data buffer,
        * must always be evenly divisible by 16.*/
        #define FNET_FEC_RX_BUF_DIV         (16U)
    #elif FNET_CFG_CPU_MPC5744P || FNET_CFG_CPU_MPC5566
        /* Phanter's and Viper's ENET runs quarter of core max frequency = 200MHz/4 = 50MHz */
        #define FNET_FEC_CLOCK_KHZ  (FNET_CPU_CLOCK_KHZ/4)
        /* Transmit buffer descriptor queue. This pointer
        * must be 64-bit aligned (bits 2–0 must be zero);
        * however, it is recommended to be 128-bit aligned,
        * that is, evenly divisible by 16.*/
        #define FNET_FEC_BUF_DESC_DIV       (16U)
        /* The transmit buffer pointer, containing the address
        * of the associated data buffer,
        * must always be evenly divisible by 8.*/
        #define FNET_FEC_TX_BUF_DIV         (8U)
        /* The receive buffer pointer, containing the address
        * of the associated data buffer,
        * must always be evenly divisible by 16.*/
        #define FNET_FEC_RX_BUF_DIV         (16U)
    #elif FNET_CFG_CPU_S32R274
        /* RRUs ENET = 50MHz */
        #define FNET_FEC_CLOCK_KHZ  50000
        /* Transmit buffer descriptor queue. This pointer
        * must be 64-bit aligned; however, it is recommended it be
        * made 512-bit aligned (evenly divisible by 64).*/
        #define FNET_FEC_BUF_DESC_DIV       (16U)
        /* The transmit buffer pointer, containing the address
        * of the associated data buffer,
        * must always be evenly divisible by 64.*/
        #define FNET_FEC_TX_BUF_DIV         (64U)
        /* The receive buffer pointer, containing the address
        * of the associated data buffer,
        * must always be evenly divisible by 64.*/
        #define FNET_FEC_RX_BUF_DIV         (64U)
    #else
        #define FNET_FEC_CLOCK_KHZ  (FNET_CPU_CLOCK_KHZ)
        /* Transmit buffer descriptor queue. This pointer
        * must be 32-bit aligned; however, it is recommended it be
        * made 128-bit aligned (evenly divisible by 16).*/
        #define FNET_FEC_BUF_DESC_DIV       (16U)
        /* The transmit buffer pointer, containing the address
        * of the associated data buffer,
        * must always be evenly divisible by 4.*/
        #define FNET_FEC_TX_BUF_DIV         (8U)
        /* The receive buffer pointer, containing the address
        * of the associated data buffer,
        * must always be evenly divisible by 16.*/
        #define FNET_FEC_RX_BUF_DIV         (16U)
    #endif
#endif

#define FNET_FEC_BUF_SIZE           (((FNET_CFG_CPU_ETH0_MTU>FNET_CFG_CPU_ETH1_MTU)?FNET_CFG_CPU_ETH0_MTU:FNET_CFG_CPU_ETH1_MTU)+FNET_ETH_HDR_SIZE+FNET_ETH_CRC_SIZE+16U) /* Ring Buffer sizes in bytes.*/
#define FNET_FEC_TX_BUF_NUM         (FNET_CFG_CPU_ETH_TX_BUFS_MAX)
#define FNET_FEC_RX_BUF_NUM         (FNET_CFG_CPU_ETH_RX_BUFS_MAX)

/************************************************************************
*     MII Register Indexes.
*************************************************************************/
#if FNET_CFG_CPU_MPC5744P
    #define FNET_FEC_MII_REG_PHYSTS      (0x0010U)   /* PHY Status Register */
    #define FNET_FEC_MII_REG_ICR         (0x0011U)   /* Interrupt Control Register */
    #define FNET_FEC_MII_REG_ISR         (0x0012U)   /* Interrupt Status Register */
    #define FNET_FEC_MII_REG_FCSCR       (0x0014U)   /* False Carrier Sense Counter Register */
    #define FNET_FEC_MII_REG_RECR        (0x0015U)   /* Receive Error Counter Register */
    #define FNET_FEC_MII_REG_PCSR        (0x0016U)   /* PCS Sub-Layer Configuration and Status Register */
    #define FNET_FEC_MII_REG_RBR         (0x0017U)   /* RMII and Bypass Register */
    #define FNET_FEC_MII_REG_LEDCR       (0x0018U)   /* LED Direct Control Register */
    #define FNET_FEC_MII_REG_PHYCR       (0x0019U)   /* PHY Status Register */
    #define FNET_FEC_MII_REG_10BTSCR     (0x001AU)   /* 10Base-T Status/Control Register */
    #define FNET_FEC_MII_REG_CDCTRL1     (0x001BU)   /* CD Test Control Register and BIST Extensions Register */
    #define FNET_FEC_MII_REG_EDCR        (0x001DU)   /* Energy Detect Control Register */
#elif FNET_CFG_CPU_S32R274
    /* TODO define vendor-specific PHY registers for KSZ9031RNX */
#else
    #define FNET_FEC_MII_REG_ICR         (0x0010U)   /* Interrupt Control Register */
    #define FNET_FEC_MII_REG_PSR         (0x0011U)   /* Proprietary Status Register */
    #define FNET_FEC_MII_REG_PCR         (0x0012U)   /* Proprietary Control Register */
#endif

#define FNET_FEC_MII_TIMEOUT            (0x10000U)   /* Timeout counter for MII communications.*/

/************************************************************************
*     FEC/ENET module registers.
*************************************************************************/
typedef struct
{
    /* Detailed Memory Map (Control/Status Registers)*/
#if FNET_MPC
#if FNET_CFG_CPU_MPC5744P || FNET_CFG_CPU_S32R274
    volatile fnet_uint32_t reserved0;
#else
    volatile fnet_uint8_t  reserved0[4100];    /*Reserved 4100 bytes (Base+0x0000-0x0103)*/
#endif
#endif
#if FNET_CFG_CPU_MIMXRT1052 || FNET_CFG_CPU_MIMXRT1062
    volatile fnet_uint32_t reserved0;
#endif
    volatile fnet_uint32_t EIR;                /* Interrupt even reg. */
    volatile fnet_uint32_t EIMR;               /* Interrupt mask reg. */
    volatile fnet_uint32_t reserved1[1];
    volatile fnet_uint32_t RDAR;               /* Receive descriptor active reg.*/
    volatile fnet_uint32_t TDAR;               /* Transmit descriptor active reg.*/
    volatile fnet_uint32_t reserved2[3];
    volatile fnet_uint32_t ECR;                /* Ethernet control reg.*/
    volatile fnet_uint32_t reserved3[6];
    volatile fnet_uint32_t MMFR;               /* MII management frame reg.*/
    volatile fnet_uint32_t MSCR;               /* MII speed control reg.*/
    volatile fnet_uint32_t reserved4[7];
#if FNET_CFG_CPU_ETH_MIB
    volatile fnet_uint32_t MIBC;               /* MIB Control/Status Register.*/
#else
    volatile fnet_uint32_t reserved4_1;
#endif
    volatile fnet_uint32_t reserved5[7];
    volatile fnet_uint32_t RCR;                /* Receive control reg.*/
    volatile fnet_uint32_t reserved6[15];
    volatile fnet_uint32_t TCR;                /* Transmit Control reg.*/
    volatile fnet_uint32_t reserved7[7];
    volatile fnet_uint32_t PALR;               /* Lower 32-bits of MAC address.*/
    volatile fnet_uint32_t PAUR;               /* Upper 16-bits of MAC address.*/
    volatile fnet_uint32_t OPD;                /* Opcode + Pause Duration.*/
#if FNET_MIMXRT
    volatile fnet_uint32_t TXIC;                /* Transmit Interrupt Coalescing Register */
    volatile fnet_uint32_t reserved8[3];
    volatile fnet_uint32_t RXIC;                /* Receive Interrupt Coalescing Register */
    volatile fnet_uint32_t reserved8_1[5];
#else
    volatile fnet_uint32_t reserved8[10];
#endif
    volatile fnet_uint32_t IAUR;               /* Upper 32-bits of individual hash table.*/
    volatile fnet_uint32_t IALR;               /* Lower 32-bits of individual hash table.*/
    volatile fnet_uint32_t GAUR;               /* Upper 32-bits of group hash table.*/
    volatile fnet_uint32_t GALR;               /* Lower 32-bits of group hash table.*/
    volatile fnet_uint32_t reserved9[7];
    volatile fnet_uint32_t TFWR;               /* FIFO transmit water mark.*/
#if FNET_MK || FNET_MIMXRT
    volatile fnet_uint32_t reserved10[14];
#else /* MCF || MPC */
#if FNET_CFG_CPU_MPC5744P || FNET_CFG_CPU_S32R274
    volatile fnet_uint32_t reserved10[14];
#else
    volatile fnet_uint32_t reserved10[1];
    volatile fnet_uint32_t FRBR;               /* FIFO receive bound reg.*/
    volatile fnet_uint32_t FRSR;               /* FIFO receive start reg.*/
    volatile fnet_uint32_t reserved11[11];
#endif
#endif
    volatile fnet_uint32_t ERDSR;              /* Pointer to receive descriptor ring.*/
    volatile fnet_uint32_t ETDSR;              /* Pointer to transmit descriptor ring.*/
    volatile fnet_uint32_t EMRBR;              /* Maximum receive buffer size.*/

#if FNET_MK || FNET_MIMXRT || FNET_CFG_CPU_MCF54418 || FNET_CFG_CPU_MPC5744P || FNET_CFG_CPU_S32R274
    volatile fnet_uint32_t reserved12[1];
    volatile fnet_uint32_t RSFL;               /* Receive FIFO Section Full Threshold, offset: 0x190 */
    volatile fnet_uint32_t RSEM;               /* Receive FIFO Section Empty Threshold, offset: 0x194 */
    volatile fnet_uint32_t RAEM;               /* Receive FIFO Almost Empty Threshold, offset: 0x198 */
    volatile fnet_uint32_t RAFL;               /* Receive FIFO Almost Full Threshold, offset: 0x19C */
    volatile fnet_uint32_t TSEM;               /* Transmit FIFO Section Empty Threshold, offset: 0x1A0 */
    volatile fnet_uint32_t TAEM;               /* Transmit FIFO Almost Empty Threshold, offset: 0x1A4 */
    volatile fnet_uint32_t TAFL;               /* Transmit FIFO Almost Full Threshold, offset: 0x1A8 */
    volatile fnet_uint32_t TIPG;               /* Transmit Inter-Packet Gap, offset: 0x1AC */
    volatile fnet_uint32_t FTRL;               /* Frame Truncation Length, offset: 0x1B0 */
    volatile fnet_uint32_t reserved13[3];
    volatile fnet_uint32_t TACC;               /* Transmit Accelerator Function Configuration, offset: 0x1C0 */
    volatile fnet_uint32_t RACC;               /* Receive Accelerator Function Configuration, offset: 0x1C4 */
#if FNET_CFG_CPU_MPC5744P || FNET_CFG_CPU_S32R274
    volatile fnet_uint32_t reserved14[15];
#else
    volatile fnet_uint32_t reserved14[14];
#endif
#else /* MCF || MPC */
    volatile fnet_uint32_t reserved14[29];
#endif

#if FNET_CFG_CPU_ETH_MIB
    /* Ethernet Management Information Base (MIB) Block Counters:*/
#if !FNET_CFG_CPU_MPC5744P && !FNET_CFG_CPU_S32R274
    volatile fnet_uint32_t RMON_T_DROP;
#endif
    volatile fnet_uint32_t RMON_T_PACKETS;
    volatile fnet_uint32_t RMON_T_BC_PKT;
    volatile fnet_uint32_t RMON_T_MC_PKT;
    volatile fnet_uint32_t RMON_T_CRC_ALIGN;
    volatile fnet_uint32_t RMON_T_UNDERSIZE;
    volatile fnet_uint32_t RMON_T_OVERSIZE;
    volatile fnet_uint32_t RMON_T_FRAG;
    volatile fnet_uint32_t RMON_T_JAB;
    volatile fnet_uint32_t RMON_T_COL;
    volatile fnet_uint32_t RMON_T_P64;
    volatile fnet_uint32_t RMON_T_P65TO127;
    volatile fnet_uint32_t RMON_T_P128TO255;
    volatile fnet_uint32_t RMON_T_P256TO511;
    volatile fnet_uint32_t RMON_T_P512TO1023;
    volatile fnet_uint32_t RMON_T_P1024TO2047;
    volatile fnet_uint32_t RMON_T_P_GTE2048;
    volatile fnet_uint32_t RMON_T_OCTETS;
#if FNET_CFG_CPU_MPC5744P || FNET_CFG_CPU_S32R274
    volatile fnet_uint32_t reserved15[1];
#else
    volatile fnet_uint32_t IEEE_T_DROP;
#endif
    volatile fnet_uint32_t IEEE_T_FRAME_OK;
    volatile fnet_uint32_t IEEE_T_1COL;
    volatile fnet_uint32_t IEEE_T_MCOL;
    volatile fnet_uint32_t IEEE_T_DEF;
    volatile fnet_uint32_t IEEE_T_LCOL;
    volatile fnet_uint32_t IEEE_T_EXCOL;
    volatile fnet_uint32_t IEEE_T_MACERR;
    volatile fnet_uint32_t IEEE_T_CSERR;
#if FNET_CFG_CPU_MPC5744P || FNET_CFG_CPU_S32R274
    volatile fnet_uint32_t reserved16[1];
#else
    volatile fnet_uint32_t IEEE_T_SQE;
#endif
    volatile fnet_uint32_t IEEE_T_FDXFC;
    volatile fnet_uint32_t IEEE_T_OCTETS_OK;
    volatile fnet_uint32_t reserved17[3];
    volatile fnet_uint32_t RMON_R_PACKETS;
    volatile fnet_uint32_t RMON_R_BC_PKT;
    volatile fnet_uint32_t RMON_R_MC_PKT;
    volatile fnet_uint32_t RMON_R_CRC_ALIGN;
    volatile fnet_uint32_t RMON_R_UNDERSIZE;
    volatile fnet_uint32_t RMON_R_OVERSIZE;
    volatile fnet_uint32_t RMON_R_FRAG;
    volatile fnet_uint32_t RMON_R_JAB;
    volatile fnet_uint32_t RMON_R_RESVD_0;
    volatile fnet_uint32_t RMON_R_P64;
    volatile fnet_uint32_t RMON_R_P65TO127;
    volatile fnet_uint32_t RMON_R_P128TO255;
    volatile fnet_uint32_t RMON_R_P256TO511;
    volatile fnet_uint32_t RMON_R_512TO1023;
    volatile fnet_uint32_t RMON_R_1024TO2047;
    volatile fnet_uint32_t RMON_R_P_GTE2048;
    volatile fnet_uint32_t RMON_R_OCTETS;
    volatile fnet_uint32_t IEEE_R_DROP;
    volatile fnet_uint32_t IEEE_R_FRAME_OK;
    volatile fnet_uint32_t IEEE_R_CRC;
    volatile fnet_uint32_t IEEE_R_ALIGN;
    volatile fnet_uint32_t IEEE_R_MACERR;
    volatile fnet_uint32_t IEEE_R_FDXFC;
    volatile fnet_uint32_t IEEE_R_OCTETS_OK;
#endif
#if 0 /* Not used. Present in Panther, Kinetis, Modelo, i.MX-RT.*/
    volatile fnet_uint32_t RESERVED_18[71];
    volatile fnet_uint32_t ATCR;                   /* Timer Control Register, offset: 0x400 */
    volatile fnet_uint32_t ATVR;                   /* Timer Value Register, offset: 0x404 */
    volatile fnet_uint32_t ATOFF;                  /* Timer Offset Register, offset: 0x408 */
    volatile fnet_uint32_t ATPER;                  /* Timer Period Register, offset: 0x40C */
    volatile fnet_uint32_t ATCOR;                  /* Timer Correction Register, offset: 0x410 */
    volatile fnet_uint32_t ATINC;                  /* Time-Stamping Clock Period Register, offset: 0x414 */
    volatile fnet_uint32_t ATSTMP;                 /* Timestamp of Last Transmitted Frame, offset: 0x418 */
    volatile fnet_uint32_t RESERVED_19[122];
    volatile fnet_uint32_t TGSR;                   /* Timer Global Status Register, offset: 0x604 */
    struct
    {
        /* offset: 0x608, array step: 0x8 */
        volatile fnet_uint32_t TCSR;               /*!< Timer Control Status Register, array offset: 0x608, array step: 0x8 */
        volatile fnet_uint32_t TCCR;               /*!< Timer Compare Capture Register, array offset: 0x60C, array step: 0x8 */
    } CHANNEL[4];
#endif
}
fnet_fec_reg_t;

/* Bit definitions and macros for FNET_FEC_EIR */
#define FNET_FEC_EIR_UN                  (0x00080000U)
#define FNET_FEC_EIR_RL                  (0x00100000U)
#define FNET_FEC_EIR_LC                  (0x00200000U)
#define FNET_FEC_EIR_EBERR               (0x00400000U)
#define FNET_FEC_EIR_MII                 (0x00800000U)
#define FNET_FEC_EIR_RXB                 (0x01000000U)
#define FNET_FEC_EIR_RXF                 (0x02000000U)
#define FNET_FEC_EIR_TXB                 (0x04000000U)
#define FNET_FEC_EIR_TXF                 (0x08000000U)
#define FNET_FEC_EIR_GRA                 (0x10000000U)
#define FNET_FEC_EIR_BABT                (0x20000000U)
#define FNET_FEC_EIR_BABR                (0x40000000U)
#if !FNET_CFG_CPU_MPC5744P && !FNET_CFG_CPU_S32R274
    #define FNET_FEC_EIR_HBERR               (0x80000000U)
#endif

/* Bit definitions and macros for FNET_FEC_EIMR */
#define FNET_FEC_EIMR_UN                 (0x00080000U)
#define FNET_FEC_EIMR_RL                 (0x00100000U)
#define FNET_FEC_EIMR_LC                 (0x00200000U)
#define FNET_FEC_EIMR_EBERR              (0x00400000U)
#define FNET_FEC_EIMR_MII                (0x00800000U)
#define FNET_FEC_EIMR_RXB                (0x01000000U)
#define FNET_FEC_EIMR_RXF                (0x02000000U)
#define FNET_FEC_EIMR_TXB                (0x04000000U)
#define FNET_FEC_EIMR_TXF                (0x08000000U)
#define FNET_FEC_EIMR_GRA                (0x10000000U)
#define FNET_FEC_EIMR_BABT               (0x20000000U)
#define FNET_FEC_EIMR_BABR               (0x40000000U)
#if !FNET_CFG_CPU_MPC5744P && !FNET_CFG_CPU_S32R274
    #define FNET_FEC_EIMR_HBERR              (0x80000000U)
#endif

/* Bit definitions and macros for FNET_FEC_RDAR */
#define FNET_FEC_RDAR_R_DES_ACTIVE       (0x01000000U)

/* Bit definitions and macros for FNET_MCF_FEC_TDAR */
#define FNET_FEC_TDAR_X_DES_ACTIVE       (0x01000000U)

/* Bit definitions and macros for FNET_MCF_FEC_ECR */
#define FNET_FEC_ECR_RESET               (0x00000001U)
#define FNET_FEC_ECR_ETHER_EN            (0x00000002U)

#if FNET_CFG_CPU_S32R274
    #define FNET_FEC_ECR_MAGICEN        (0x00000004U)
    #define FNET_FEC_ECR_SLEEP          (0x00000008U)
    #define FNET_FEC_ECR_EN1588         (0x00000010U)
    #define FNET_FEC_ECR_SPEED          (0x00000020U)
    #define FNET_FEC_ECR_DBGEN          (0x00000040U)
    #define FNET_FEC_ECR_STOPEN         (0x00000080U)
    #define FNET_FEC_ECR_DBSWP          (0x00000100U)
#endif
/* Bit definitions and macros for FNET_MCF_FEC_MMFR */
#define FNET_FEC_MMFR_DATA(x)            (((x)&0x0000FFFFU)<<0U)
#define FNET_FEC_MMFR_TA(x)              (((x)&0x00000003U)<<16U)
#define FNET_FEC_MMFR_RA(x)              (((x)&0x0000001FU)<<18U)
#define FNET_FEC_MMFR_PA(x)              (((x)&0x0000001FU)<<23U)
#define FNET_FEC_MMFR_OP(x)              (((x)&0x00000003U)<<28U)
#define FNET_FEC_MMFR_ST(x)              (((x)&0x00000003U)<<30U)
#define FNET_FEC_MMFR_ST_01              (0x40000000U)
#define FNET_FEC_MMFR_OP_READ            (0x20000000U)
#define FNET_FEC_MMFR_OP_WRITE           (0x10000000U)
#define FNET_FEC_MMFR_TA_10              (0x00020000U)


/* Bit definitions and macros for FNET_MCF_FEC_MSCR */
#define FNET_FEC_MSCR_MII_SPEED(x)       (((x)&0x0000003FU)<<1U)
#define FNET_FEC_MSCR_DIS_PREAMBLE       (0x00000080U)

/* Bit definitions and macros for FNET_MCF_FEC_MIBC */
#define FNET_FEC_MIBC_MIB_IDLE           (0x40000000U)
#define FNET_FEC_MIBC_MIB_DISABLE        (0x80000000U)

/* Bit definitions and macros for FNET_MCF_FEC_RCR */
#define FNET_FEC_RCR_LOOP                (0x00000001U)
#define FNET_FEC_RCR_DRT                 (0x00000002U)
#define FNET_FEC_RCR_MII_MODE            (0x00000004U)
#define FNET_FEC_RCR_PROM                (0x00000008U)
#define FNET_FEC_RCR_BC_REJ              (0x00000010U)
#define FNET_FEC_RCR_FCE                 (0x00000020U)
#if FNET_CFG_CPU_MPC5744P || FNET_CFG_CPU_S32R274
    #define FNET_FEC_RCR_MAX_FL(x)           (((x)&0x00003FFFU)<<16U)
#else
    #define FNET_FEC_RCR_MAX_FL(x)           (((x)&0x000007FFU)<<16U)
#endif
#define FNET_FEC_RCR_RMII_MODE           (0x00000100U)
#define FNET_FEC_RCR_RMII_10T            (0x00000200U)
#if FNET_CFG_CPU_S32R274
    #define FNET_FEC_RCR_RGMII_EN        (0x00000040U)
#endif

/* Bit definitions and macros for FNET_MCF_FEC_TCR */
#define FNET_FEC_TCR_GTS                 (0x00000001U)
#if !FNET_CFG_CPU_MPC5744P && !FNET_CFG_CPU_S32R274
    #define FNET_FEC_TCR_HBC                 (0x00000002U)
#endif
#define FNET_FEC_TCR_FDEN                (0x00000004U)
#define FNET_FEC_TCR_TFC_PAUSE           (0x00000008U)
#define FNET_FEC_TCR_RFC_PAUSE           (0x00000010U)

/* Bit definitions and macros for FNET_MCF_FEC_PAUR */
#define FNET_FEC_PAUR_TYPE(x)           (((x)&0x0000FFFFU)<<0U)
#define FNET_FEC_PAUR_PADDR2(x)         (((x)&0x0000FFFFU)<<16U)

/* Bit definitions and macros for FNET_MCF_FEC_OPD */
#define FNET_FEC_OPD_PAUSE_DUR(x)       (((x)&0x0000FFFFU)<<0U)
#define FNET_FEC_OPD_OPCODE(x)          (((x)&0x0000FFFFU)<<16U)


#if FNET_CFG_CPU_MPC5744P || FNET_CFG_CPU_S32R274
    /* Bit definitions and macros for FNET_MPC_FEC_TFWR */
    #define FNET_FEC_TFWR_X_WMRK(x)         (((x)&0x0000003FU)<<0U)

    /* Bit definitions and macros for FNET_MPC_FEC_ERDSR */
    #define FNET_FEC_ERDSR_R_DES_START(x)   (((x)&0x1FFFFFFFU)<<3U)

    /* Bit definitions and macros for FNET_MPC_FEC_ETDSR */
    #define FNET_FEC_ETDSR_X_DES_START(x)   (((x)&0x1FFFFFFFU)<<3U)

#else
    /* Bit definitions and macros for FNET_MCF_FEC_TFWR */
    #define FNET_FEC_TFWR_X_WMRK(x)         (((x)&0x00000003U)<<0U)

    /* Bit definitions and macros for FNET_MCF_FEC_FRBR */
    #define FNET_FEC_FRBR_R_BOUND(x)        (((x)&0x000000FFU)<<2U)

    /* Bit definitions and macros for FNET_MCF_FEC_FRSR */
    #define FNET_FEC_FRSR_R_FSTART(x)       (((x)&0x000000FFU)<<2U)

    /* Bit definitions and macros for FNET_MCF_FEC_ERDSR */
    #define FNET_FEC_ERDSR_R_DES_START(x)   (((x)&0x3FFFFFFFU)<<2U)

    /* Bit definitions and macros for FNET_MCF_FEC_ETDSR */
    #define FNET_FEC_ETDSR_X_DES_START(x)   (((x)&0x3FFFFFFFU)<<2U)

#endif

/* Bit definitions and macros for FNET_MCF_FEC_EMRBR */
#define FNET_FEC_EMRBR_R_BUF_SIZE(x)    (((x)&0x0000007FU)<<4U)

/*  Bit definitions and macros for TACC */
#define FNET_FEC_TACC_SHIFT16           (0x00000001U)
#define FNET_FEC_TACC_IPCHK             (0x00000008U)
#define FNET_FEC_TACC_PROCHK            (0x00000010U)

/* Bit definitions and macros for RACC */
#define FNET_FEC_RACC_PADREM            (0x00000001U)
#define FNET_FEC_RACC_IPDIS             (0x00000002U)
#define FNET_FEC_RACC_PRODIS            (0x00000004U)
#define FNET_FEC_RACC_LINEDIS           (0x00000040U)
#define FNET_FEC_RACC_SHIFT16           (0x00000080U)


/*  Bit definitions and macros for TFWR */
#define FNET_FEC_TFWR_STRFWD             (0x00000100U)

/************************************************************************
*     Ethernet Buffer Descriptor definitions (status field)
*************************************************************************/
#define FNET_FEC_RX_BD_E        (0x8000U)
#define FNET_FEC_RX_BD_R01      (0x4000U)
#define FNET_FEC_RX_BD_W        (0x2000U)
#define FNET_FEC_RX_BD_R02      (0x1000U)
#define FNET_FEC_RX_BD_L        (0x0800U)
#define FNET_FEC_RX_BD_M        (0x0100U)
#define FNET_FEC_RX_BD_BC       (0x0080U)
#define FNET_FEC_RX_BD_MC       (0x0040U)
#define FNET_FEC_RX_BD_LG       (0x0020U)
#define FNET_FEC_RX_BD_NO       (0x0010U)
#if FNET_CFG_CPU_MPC5744P || FNET_CFG_CPU_S32R274
    #define FNET_FEC_RX_BD_SH       (0x0U)
#else
    #define FNET_FEC_RX_BD_SH       (0x0008U)
#endif
#define FNET_FEC_RX_BD_CR       (0x0004U)
#define FNET_FEC_RX_BD_OV       (0x0002U)
#define FNET_FEC_RX_BD_TR       (0x0001U)

#define FNET_FEC_TX_BD_R        (0x8000U)
#define FNET_FEC_TX_BD_TO1      (0x4000U)
#define FNET_FEC_TX_BD_W        (0x2000U)
#define FNET_FEC_TX_BD_TO2      (0x1000U)
#define FNET_FEC_TX_BD_L        (0x0800U)
#define FNET_FEC_TX_BD_TC       (0x0400U)
#if FNET_CFG_CPU_MPC5744P || FNET_CFG_CPU_S32R274
    #define FNET_FEC_TX_BD_ABC      (0x0200U)
#else
    #define FNET_FEC_TX_BD_DEF      (0x0200U)
    #define FNET_FEC_TX_BD_HB       (0x0100U)
    #define FNET_FEC_TX_BD_LC       (0x0080U)
    #define FNET_FEC_TX_BD_RL       (0x0040U)
    #define FNET_FEC_TX_BD_UN       (0x0002U)
    #define FNET_FEC_TX_BD_CSL      (0x0001U)
#endif

/* Ethernet Buffer descriptor */
FNET_COMP_PACKED_BEGIN
typedef struct
{
    volatile fnet_uint16_t  status FNET_COMP_PACKED;     /* Control and status info.*/
    volatile fnet_uint16_t  length FNET_COMP_PACKED;     /* Data length.*/
    volatile fnet_uint8_t   *buf_ptr FNET_COMP_PACKED;   /* Buffer pointer.*/
}
fnet_fec_buf_desc_t;
FNET_COMP_PACKED_END

/* FEC/ENET driver buffer placeholder.
* It have to be located in noncacheable region.*/
typedef struct
{
    volatile fnet_uint8_t tx_buf_desc_buf[(FNET_FEC_TX_BUF_NUM * sizeof(fnet_fec_buf_desc_t)) + (FNET_FEC_BUF_DESC_DIV - 1U)];
    volatile fnet_uint8_t rx_buf_desc_buf[(FNET_FEC_RX_BUF_NUM * sizeof(fnet_fec_buf_desc_t)) + (FNET_FEC_BUF_DESC_DIV - 1U)];
    volatile fnet_uint8_t tx_buf[FNET_FEC_TX_BUF_NUM][FNET_FEC_BUF_SIZE + (FNET_FEC_TX_BUF_DIV - 1U)];
    volatile fnet_uint8_t rx_buf[FNET_FEC_RX_BUF_NUM][FNET_FEC_BUF_SIZE + (FNET_FEC_RX_BUF_DIV - 1U)];
}
fnet_fec_buf_t;

/* FEC/ENET Module control data structure */
typedef struct
{
    volatile fnet_fec_reg_t     *reg;               /* Pointer to the eth registers. */
    volatile fnet_fec_reg_t     *reg_phy;           /* Pointer to the eth registers, used for comunication with phy. */
    fnet_uint32_t               vector_number;      /* Vector number of the Ethernet Receive Frame interrupt.*/

    fnet_fec_buf_desc_t         *tx_buf_desc;       /* Tx Buffer Descriptors.*/
    fnet_fec_buf_desc_t         *rx_buf_desc;       /* Rx Buffer Descriptors.*/
    fnet_uint8_t                tx_buf_desc_num;    /* Number of allocated Tx Buffer Descriptors.*/
    fnet_uint8_t                rx_buf_desc_num;    /* Number of allocated Tx Buffer Descriptors.*/
    volatile fnet_fec_buf_desc_t         *rx_buf_desc_cur;   /* Points to the descriptor of the current incoming buffer.*/

    volatile fnet_fec_buf_desc_t         *tx_buf_desc_cur;   /* Points to the descriptor of the current outcoming buffer.*/
#if FNET_CFG_MULTICAST
    fnet_uint32_t               GALR_double;
    fnet_uint32_t               GAUR_double;
#endif
#if !FNET_CFG_CPU_ETH_MIB
    struct fnet_netif_statistics    statistics;     /* Statistic counters.*/
#endif
    fnet_fec_buf_t              *buf;
} fnet_fec_if_t;

/* FEC driver API */
extern const fnet_netif_api_t fnet_fec_api;
/* Ethernet specific control data structure.*/
#if FNET_CFG_CPU_ETH0
    extern fnet_fec_if_t fnet_fec0_if;
#endif
#if FNET_CFG_CPU_ETH1
    extern fnet_fec_if_t fnet_fec1_if;
#endif

/************************************************************************
*     Function Prototypes
*************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

void fnet_fec_output(fnet_netif_t *netif, fnet_netbuf_t *nb);
#if FNET_CFG_MULTICAST
void fnet_fec_multicast_join(fnet_netif_t *netif, fnet_mac_addr_t multicast_addr);
void fnet_fec_multicast_leave(fnet_netif_t *netif, fnet_mac_addr_t multicast_addr);
#endif /* FNET_CFG_MULTICAST */

/* For debug needs.*/
void fnet_fec_poll(fnet_netif_desc_t netif_desc);
void fnet_fec_stop(fnet_netif_t *netif);
void fnet_fec_resume(fnet_netif_t *netif);

#if defined(__cplusplus)
}
#endif

#endif /* (FNET_MCF || FNET_MK || FNET_MPC) && FNET_CFG_ETH */

#endif /*_FNET_FEC_H_*/
