/**************************************************************************
*
* Copyright 2018 by Andrey Butok. FNET Community.
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
*  Private. i.MX RT Peripheral Registers definitions.
*
***************************************************************************/

#ifndef _FNET_MIMXRT_H_

#define _FNET_MIMXRT_H_

#if FNET_MIMXRT

/*********************************************************************
*
* The basic data types.
*
*********************************************************************/
typedef unsigned char fnet_uint8_t;         /*  8 bits */
typedef unsigned short int fnet_uint16_t;   /* 16 bits */
typedef unsigned long int fnet_uint32_t;    /* 32 bits */
typedef unsigned long long fnet_uint64_t;   /* 64 bits */

typedef signed char fnet_int8_t;            /*  8 bits */
typedef signed short int fnet_int16_t;      /* 16 bits */
typedef signed long int fnet_int32_t;       /* 32 bits */
typedef signed long long fnet_int64_t;      /* 64 bits */

typedef volatile fnet_uint8_t fnet_vuint8_t;     /*  8 bits */
typedef volatile fnet_uint16_t fnet_vuint16_t;   /* 16 bits */
typedef volatile fnet_uint32_t fnet_vuint32_t;   /* 32 bits */
typedef volatile fnet_uint64_t fnet_vuint64_t;   /* 64 bits */

/* Ensures that all data memory transfers are completed.*/
#define FNET_CPU_DATA_MEMORY_BARRIER    FNET_COMP_ASM("DMB")

/************************************************************************
* i.MX RT peripheral clock in KHZ.
*************************************************************************/
fnet_uint32_t fnet_mimxrt_periph_clk_khz(void);

#define FNET_MIMXRT_PERIPH_CLOCK_KHZ       fnet_mimxrt_periph_clk_khz()


/*********************************************************************
* SCB
*********************************************************************/

/* SCB - Peripheral register structure */
typedef struct FNET_MIMXRT_SCB_MemMap
{
    fnet_vuint32_t CPUID;                                  /* CPUID Base Register, offset: 0xD00 */
    fnet_vuint32_t ICSR;                                   /* Interrupt Control and State Register, offset: 0xD04 */
    fnet_vuint32_t VTOR;                                   /* Vector Table Offset Register, offset: 0xD08 */
    fnet_vuint32_t AIRCR;                                  /* Application Interrupt and Reset Control Register, offset: 0xD0C */
    fnet_vuint32_t SCR;                                    /* System Control Register, offset: 0xD10 */
    fnet_vuint32_t CCR;                                    /* Configuration and Control Register, offset: 0xD14 */
    fnet_vuint8_t SHPR[12];                                /*  System Handlers Priority Registers (4-7, 8-11, 12-15) */
    fnet_vuint32_t SHCSR;                                  /* System Handler Control and State Register, offset: 0xD24 */
    fnet_vuint32_t CFSR;                                   /* Configurable Fault Status Registers, offset: 0xD28 */
    fnet_vuint32_t HFSR;                                   /* HardFault Status register, offset: 0xD2C */
    fnet_vuint32_t DFSR;                                   /* Debug Fault Status Register, offset: 0xD30 */
    fnet_vuint32_t MMFAR;                                  /* MemManage Address Register, offset: 0xD34 */
    fnet_vuint32_t BFAR;                                   /* BusFault Address Register, offset: 0xD38 */
    fnet_vuint32_t AFSR;                                   /* Auxiliary Fault Status Register, offset: 0xD3C */
    /*... TBD */
} volatile *FNET_MIMXRT_SCB_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- SCB - Register accessor macros
   ---------------------------------------------------------------------------- */

/* SCB - Register accessors */
#define FNET_MIMXRT_SCB_CPUID_REG(base)                      ((base)->CPUID)
#define FNET_MIMXRT_SCB_ICSR_REG(base)                       ((base)->ICSR)
#define FNET_MIMXRT_SCB_VTOR_REG(base)                       ((base)->VTOR)
#define FNET_MIMXRT_SCB_AIRCR_REG(base)                      ((base)->AIRCR)
#define FNET_MIMXRT_SCB_SCR_REG(base)                        ((base)->SCR)
#define FNET_MIMXRT_SCB_CCR_REG(base)                        ((base)->CCR)
#define FNET_MIMXRT_SCB_SHPR_REG(base)                      ((base)->SHPR)
#define FNET_MIMXRT_SCB_SHCSR_REG(base)                      ((base)->SHCSR)
#define FNET_MIMXRT_SCB_CFSR_REG(base)                       ((base)->CFSR)
#define FNET_MIMXRT_SCB_HFSR_REG(base)                       ((base)->HFSR)
#define FNET_MIMXRT_SCB_DFSR_REG(base)                       ((base)->DFSR)
#define FNET_MIMXRT_SCB_MMFAR_REG(base)                      ((base)->MMFAR)
#define FNET_MIMXRT_SCB_BFAR_REG(base)                       ((base)->BFAR)
#define FNET_MIMXRT_SCB_AFSR_REG(base)                       ((base)->AFSR)

/* SCB - Peripheral instance base addresses */
/* Peripheral SystemControl base pointer */
#define FNET_MIMXRT_SystemControl_BASE_PTR                   ((FNET_MIMXRT_SCB_MemMapPtr)0xE000ED00u)

/* ----------------------------------------------------------------------------
   -- SCB - Register accessor macros
   ---------------------------------------------------------------------------- */

/* SCB - Register instance definitions */
/* SystemControl */
#define FNET_MIMXRT_SCB_CPUID                                FNET_MIMXRT_SCB_CPUID_REG(FNET_MIMXRT_SystemControl_BASE_PTR)
#define FNET_MIMXRT_SCB_ICSR                                 FNET_MIMXRT_SCB_ICSR_REG(FNET_MIMXRT_SystemControl_BASE_PTR)
#define FNET_MIMXRT_SCB_AIRCR                                FNET_MIMXRT_SCB_AIRCR_REG(FNET_MIMXRT_SystemControl_BASE_PTR)
#define FNET_MIMXRT_SCB_VTOR                                 FNET_MIMXRT_SCB_VTOR_REG(FNET_MIMXRT_SystemControl_BASE_PTR)
#define FNET_MIMXRT_SCB_SCR                                  FNET_MIMXRT_SCB_SCR_REG(FNET_MIMXRT_SystemControl_BASE_PTR)
#define FNET_MIMXRT_SCB_CCR                                  FNET_MIMXRT_SCB_CCR_REG(FNET_MIMXRT_SystemControl_BASE_PTR)
#define FNET_MIMXRT_SCB_SHPR1                                FNET_MIMXRT_SCB_SHPR1_REG(FNET_MIMXRT_SystemControl_BASE_PTR)
#define FNET_MIMXRT_SCB_SHPR2                                FNET_MIMXRT_SCB_SHPR2_REG(FNET_MIMXRT_SystemControl_BASE_PTR)
#define FNET_MIMXRT_SCB_SHPR3                                FNET_MIMXRT_SCB_SHPR3_REG(FNET_MIMXRT_SystemControl_BASE_PTR)
#define FNET_MIMXRT_SCB_SHCSR                                FNET_MIMXRT_SCB_SHCSR_REG(FNET_MIMXRT_SystemControl_BASE_PTR)
#define FNET_MIMXRT_SCB_CFSR                                 FNET_MIMXRT_SCB_CFSR_REG(FNET_MIMXRT_SystemControl_BASE_PTR)
#define FNET_MIMXRT_SCB_HFSR                                 FNET_MIMXRT_SCB_HFSR_REG(FNET_MIMXRT_SystemControl_BASE_PTR)
#define FNET_MIMXRT_SCB_DFSR                                 FNET_MIMXRT_SCB_DFSR_REG(FNET_MIMXRT_SystemControl_BASE_PTR)
#define FNET_MIMXRT_SCB_MMFAR                                FNET_MIMXRT_SCB_MMFAR_REG(FNET_MIMXRT_SystemControl_BASE_PTR)
#define FNET_MIMXRT_SCB_BFAR                                 FNET_MIMXRT_SCB_BFAR_REG(FNET_MIMXRT_SystemControl_BASE_PTR)
#define FNET_MIMXRT_SCB_AFSR                                 FNET_MIMXRT_SCB_AFSR_REG(FNET_MIMXRT_SystemControl_BASE_PTR)


/* AIRCR Bit Fields */
#define FNET_MIMXRT_SCB_AIRCR_VECTRESET_MASK                 0x1u
#define FNET_MIMXRT_SCB_AIRCR_VECTRESET_SHIFT                0
#define FNET_MIMXRT_SCB_AIRCR_VECTCLRACTIVE_MASK             0x2u
#define FNET_MIMXRT_SCB_AIRCR_VECTCLRACTIVE_SHIFT            1
#define FNET_MIMXRT_SCB_AIRCR_SYSRESETREQ_MASK               0x4u
#define FNET_MIMXRT_SCB_AIRCR_SYSRESETREQ_SHIFT              2
#define FNET_MIMXRT_SCB_AIRCR_PRIGROUP_MASK                  0x700u
#define FNET_MIMXRT_SCB_AIRCR_PRIGROUP_SHIFT                 8
#define FNET_MIMXRT_SCB_AIRCR_PRIGROUP(x)                    (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MIMXRT_SCB_AIRCR_PRIGROUP_SHIFT))&FNET_MIMXRT_SCB_AIRCR_PRIGROUP_MASK)
#define FNET_MIMXRT_SCB_AIRCR_ENDIANNESS_MASK                0x8000u
#define FNET_MIMXRT_SCB_AIRCR_ENDIANNESS_SHIFT               15
#define FNET_MIMXRT_SCB_AIRCR_VECTKEY_MASK                   0xFFFF0000u
#define FNET_MIMXRT_SCB_AIRCR_VECTKEY_SHIFT                  16
#define FNET_MIMXRT_SCB_AIRCR_VECTKEY(x)                     (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MIMXRT_SCB_AIRCR_VECTKEY_SHIFT))&FNET_MIMXRT_SCB_AIRCR_VECTKEY_MASK)

/*********************************************************************
* NVIC
*********************************************************************/

/* NVIC - Peripheral register structure */
typedef struct FNET_MIMXRT_NVIC_MemMap
{
    fnet_vuint32_t ISER[8U];                                /* Interrupt Set Enable Register n, array offset: 0x0, array step: 0x4 */
    fnet_uint32_t RESERVED_0[24U];
    fnet_vuint32_t ICER[8U];                                /* Interrupt Clear Enable Register n, array offset: 0x80, array step: 0x4 */
    fnet_uint32_t RESERVED_1[24U];
    fnet_vuint32_t ISPR[8U];                                /* Interrupt Set Pending Register n, array offset: 0x100, array step: 0x4 */
    fnet_uint32_t RESERVED_2[24U];
    fnet_vuint32_t ICPR[8U];                                /* Interrupt Clear Pending Register n, array offset: 0x180, array step: 0x4 */
    fnet_uint32_t RESERVED_3[24U];
    fnet_vuint32_t IABR[8U];                                /* Interrupt Active bit Register n, array offset: 0x200, array step: 0x4 */
    fnet_uint32_t RESERVED_4[56U];
    fnet_vuint8_t IP[240U];                                 /* Interrupt Priority Register n, array offset: 0x300, array step: 0x1 */
    fnet_uint32_t RESERVED_5[644U];
    fnet_vuint32_t STIR[1];                                /* Software Trigger Interrupt Register, array offset: 0xE00, array step: 0x4 */
} volatile *FNET_MIMXRT_NVIC_MemMapPtr;


/* NVIC - Peripheral instance base addresses */
/* Peripheral NVIC base pointer */
#define FNET_MIMXRT_NVIC_BASE_PTR                            ((FNET_MIMXRT_NVIC_MemMapPtr)0xE000E100UL)

/* ----------------------------------------------------------------------------
   -- NVIC - Register accessor macros
   ---------------------------------------------------------------------------- */

/* NVIC - Register accessors */
#define FNET_MIMXRT_NVIC_ISER_REG(base,index)                ((base)->ISER[(index)])
#define FNET_MIMXRT_NVIC_ICER_REG(base,index)                ((base)->ICER[(index)])
#define FNET_MIMXRT_NVIC_ISPR_REG(base,index)                ((base)->ISPR[(index)])
#define FNET_MIMXRT_NVIC_ICPR_REG(base,index)                ((base)->ICPR[(index)])
#define FNET_MIMXRT_NVIC_IABR_REG(base,index)                ((base)->IABR[(index)])
#define FNET_MIMXRT_NVIC_IP_REG(base,index)                  ((base)->IP[(index)])
#define FNET_MIMXRT_NVIC_STIR_REG(base,index)                ((base)->STIR[(index)])

/* NVIC - Register array accessors */
#define FNET_MIMXRT_NVIC_ISER(index)                         FNET_MIMXRT_NVIC_ISER_REG(FNET_MIMXRT_NVIC_BASE_PTR,(index))
#define FNET_MIMXRT_NVIC_ICER(index)                         FNET_MIMXRT_NVIC_ICER_REG(FNET_MIMXRT_NVIC_BASE_PTR,(index))
#define FNET_MIMXRT_NVIC_ISPR(index)                         FNET_MIMXRT_NVIC_ISPR_REG(FNET_MIMXRT_NVIC_BASE_PTR,(index))
#define FNET_MIMXRT_NVIC_ICPR(index)                         FNET_MIMXRT_NVIC_ICPR_REG(FNET_MIMXRT_NVIC_BASE_PTR,(index))
#define FNET_MIMXRT_NVIC_IABR(index)                         FNET_MIMXRT_NVIC_IABR_REG(FNET_MIMXRT_NVIC_BASE_PTR,(index))
#define FNET_MIMXRT_NVIC_IP(index)                           FNET_MIMXRT_NVIC_IP_REG(FNET_MIMXRT_NVIC_BASE_PTR,(index))
#define FNET_MIMXRT_NVIC_STIR(index)                         FNET_MIMXRT_NVIC_STIR_REG(FNET_MIMXRT_NVIC_BASE_PTR,(index))




/* ----------------------------------------------------------------------------
   -- PIT
   ---------------------------------------------------------------------------- */

/* PIT - Peripheral register structure */
typedef struct FNET_MIMXRT_PIT_MemMap
{
    fnet_uint32_t MCR;                                      /* PIT Module Control Register, offset: 0x0 */
    fnet_uint8_t RESERVED_0[220];
    fnet_uint32_t LTMR64H;                                  /* PIT Upper Lifetime Timer Register, offset: 0xE0 */
    fnet_uint32_t LTMR64L;                                  /* PIT Lower Lifetime Timer Register, offset: 0xE4 */
    fnet_uint8_t RESERVED_1[24];
    struct                                                /* offset: 0x100, array step: 0x10 */
    {
        fnet_uint32_t LDVAL;                                  /* Timer Load Value Register, array offset: 0x100, array step: 0x10 */
        fnet_uint32_t CVAL;                                   /* Current Timer Value Register, array offset: 0x104, array step: 0x10 */
        fnet_uint32_t TCTRL;                                  /* Timer Control Register, array offset: 0x108, array step: 0x10 */
        fnet_uint32_t TFLG;                                   /* Timer Flag Register, array offset: 0x10C, array step: 0x10 */
    } CHANNEL[4];
} volatile *FNET_MIMXRT_PIT_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- PIT - Register accessor macros
   ---------------------------------------------------------------------------- */

/* PIT - Register accessors */
#define FNET_MIMXRT_PIT_MCR_REG(base)                        ((base)->MCR)
#define FNET_MIMXRT_PIT_LDVAL_REG(base,index)                ((base)->CHANNEL[(index)].LDVAL)
#define FNET_MIMXRT_PIT_CVAL_REG(base,index)                 ((base)->CHANNEL[(index)].CVAL)
#define FNET_MIMXRT_PIT_TCTRL_REG(base,index)                ((base)->CHANNEL[(index)].TCTRL)
#define FNET_MIMXRT_PIT_TFLG_REG(base,index)                 ((base)->CHANNEL[(index)].TFLG)

/* ----------------------------------------------------------------------------
   -- PIT Register Masks
   ---------------------------------------------------------------------------- */

/* MCR Bit Fields */
#define FNET_MIMXRT_PIT_MCR_FRZ_MASK                         0x1u
#define FNET_MIMXRT_PIT_MCR_FRZ_SHIFT                        0
#define FNET_MIMXRT_PIT_MCR_MDIS_MASK                        0x2u
#define FNET_MIMXRT_PIT_MCR_MDIS_SHIFT                       1
/* LDVAL Bit Fields */
#define FNET_MIMXRT_PIT_LDVAL_TSV_MASK                       0xFFFFFFFFu
#define FNET_MIMXRT_PIT_LDVAL_TSV_SHIFT                      0
#define FNET_MIMXRT_PIT_LDVAL_TSV(x)                         (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MIMXRT_PIT_LDVAL_TSV_SHIFT))&FNET_MIMXRT_PIT_LDVAL_TSV_MASK)
/* CVAL Bit Fields */
#define FNET_MIMXRT_PIT_CVAL_TVL_MASK                        0xFFFFFFFFu
#define FNET_MIMXRT_PIT_CVAL_TVL_SHIFT                       0
#define FNET_MIMXRT_PIT_CVAL_TVL(x)                          (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_MIMXRT_PIT_CVAL_TVL_SHIFT))&FNET_MIMXRT_PIT_CVAL_TVL_MASK)
/* TCTRL Bit Fields */
#define FNET_MIMXRT_PIT_TCTRL_TEN_MASK                       0x1u
#define FNET_MIMXRTPIT_TCTRL_TEN_SHIFT                      0
#define FNET_MIMXRT_PIT_TCTRL_TIE_MASK                       0x2u
#define FNET_MIMXRT_PIT_TCTRL_TIE_SHIFT                      1
/* TFLG Bit Fields */
#define FNET_MIMXRT_PIT_TFLG_TIF_MASK                        0x1u
#define FNET_MIMXRT_PIT_TFLG_TIF_SHIFT                       0

/* PIT - Peripheral instance base addresses */
/* Peripheral PIT base pointer */
#define FNET_MIMXRT_PIT_BASE_PTR                             ((FNET_MIMXRT_PIT_MemMapPtr)0x40084000u)

/* ----------------------------------------------------------------------------
   -- PIT - Register accessor macros
   ---------------------------------------------------------------------------- */

/* PIT - Register instance definitions */
/* PIT */
#define FNET_MIMXRT_PIT_MCR                                  FNET_MIMXRT_PIT_MCR_REG(FNET_MIMXRT_PIT_BASE_PTR)
#define FNET_MIMXRT_PIT_LDVAL0                               FNET_MIMXRT_PIT_LDVAL_REG(FNET_MIMXRT_PIT_BASE_PTR,0)
#define FNET_MIMXRT_PIT_CVAL0                                FNET_MIMXRT_PIT_CVAL_REG(FNET_MIMXRT_PIT_BASE_PTR,0)
#define FNET_MIMXRT_PIT_TCTRL0                               FNET_MIMXRT_PIT_TCTRL_REG(FNET_MIMXRT_PIT_BASE_PTR,0)
#define FNET_MIMXRT_PIT_TFLG0                                FNET_MIMXRT_PIT_TFLG_REG(FNET_MIMXRT_PIT_BASE_PTR,0)
#define FNET_MIMXRT_PIT_LDVAL1                               FNET_MIMXRT_PIT_LDVAL_REG(FNET_MIMXRT_PIT_BASE_PTR,1)
#define FNET_MIMXRT_PIT_CVAL1                                FNET_MIMXRT_PIT_CVAL_REG(FNET_MIMXRT_PIT_BASE_PTR,1)
#define FNET_MIMXRT_PIT_TCTRL1                               FNET_MIMXRT_PIT_TCTRL_REG(FNET_MIMXRT_PIT_BASE_PTR,1)
#define FNET_MIMXRT_PIT_TFLG1                                FNET_MIMXRT_PIT_TFLG_REG(FNET_MIMXRT_PIT_BASE_PTR,1)
#define FNET_MIMXRT_PIT_LDVAL2                               FNET_MIMXRT_PIT_LDVAL_REG(FNET_MIMXRT_PIT_BASE_PTR,2)
#define FNET_MIMXRT_PIT_CVAL2                                FNET_MIMXRT_PIT_CVAL_REG(FNET_MIMXRT_PIT_BASE_PTR,2)
#define FNET_MIMXRT_PIT_TCTRL2                               FNET_MIMXRT_PIT_TCTRL_REG(FNET_MIMXRT_PIT_BASE_PTR,2)
#define FNET_MIMXRT_PIT_TFLG2                                FNET_MIMXRT_PIT_TFLG_REG(FNET_MIMXRT_PIT_BASE_PTR,2)
#define FNET_MIMXRT_PIT_LDVAL3                               FNET_MIMXRT_PIT_LDVAL_REG(FNET_MIMXRT_PIT_BASE_PTR,3)
#define FNET_MIMXRT_PIT_CVAL3                                FNET_MIMXRT_PIT_CVAL_REG(FNET_MIMXRT_PIT_BASE_PTR,3)
#define FNET_MIMXRT_PIT_TCTRL3                               FNET_MIMXRT_PIT_TCTRL_REG(FNET_MIMXRT_PIT_BASE_PTR,3)
#define FNET_MIMXRT_PIT_TFLG3                                FNET_MIMXRT_PIT_TFLG_REG(FNET_MIMXRT_PIT_BASE_PTR,3)

/* PIT - Register array accessors */
#define FNET_MIMXRT_PIT_LDVAL(index)                         FNET_MIMXRT_PIT_LDVAL_REG(FNET_MIMXRT_PIT_BASE_PTR,(index))
#define FNET_MIMXRT_PIT_CVAL(index)                          FNET_MIMXRT_PIT_CVAL_REG(FNET_MIMXRT_PIT_BASE_PTR,(index))
#define FNET_MIMXRT_PIT_TCTRL(index)                         FNET_MIMXRT_PIT_TCTRL_REG(FNET_MIMXRT_PIT_BASE_PTR,(index))
#define FNET_MIMXRT_PIT_TFLG(index)                          FNET_MIMXRT_PIT_TFLG_REG(FNET_MIMXRT_PIT_BASE_PTR,(index))

/*********************************************************************
* Fast Ethernet Controller (FEC)
*********************************************************************/
#define FNET_FEC0_BASE_ADDR                             (0x402D8000u)


#if defined(__cplusplus)
extern "C" {
#endif

void fnet_mimxrt_irq_enable(fnet_uint32_t irq_desc);
fnet_uint32_t fnet_mimxrt_irq_disable(void);

#if defined(__cplusplus)
}
#endif

#endif /* FNET_MK */

#endif /*_FNET_MK_H_*/
