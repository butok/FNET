/**************************************************************************
*
* Copyright 2017 by Andrey Butok. FNET Community.
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
*  Private. LPC definitions.
*
***************************************************************************/

#ifndef _FNET_LPC_H_

#define _FNET_LPC_H_

#if FNET_LPC

/*********************************************************************
* The basic data types.
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

/*********************************************************************
* SCB
*********************************************************************/

/* SCB - Peripheral register structure */
typedef struct FNET_LPC_SCB_MemMap
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
} volatile *FNET_LPC_SCB_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- SCB - Register accessor macros
   ---------------------------------------------------------------------------- */

/* SCB - Register accessors */
#define FNET_LPC_SCB_ACTLR_REG(base)                      ((base)->ACTLR)
#define FNET_LPC_SCB_CPUID_REG(base)                      ((base)->CPUID)
#define FNET_LPC_SCB_ICSR_REG(base)                       ((base)->ICSR)
#define FNET_LPC_SCB_VTOR_REG(base)                       ((base)->VTOR)
#define FNET_LPC_SCB_AIRCR_REG(base)                      ((base)->AIRCR)
#define FNET_LPC_SCB_SCR_REG(base)                        ((base)->SCR)
#define FNET_LPC_SCB_CCR_REG(base)                        ((base)->CCR)
#define FNET_LPC_SCB_SHPR1_REG(base)                      ((base)->SHPR1)
#define FNET_LPC_SCB_SHPR2_REG(base)                      ((base)->SHPR2)
#define FNET_LPC_SCB_SHPR3_REG(base)                      ((base)->SHPR3)
#define FNET_LPC_SCB_SHCSR_REG(base)                      ((base)->SHCSR)
#define FNET_LPC_SCB_CFSR_REG(base)                       ((base)->CFSR)
#define FNET_LPC_SCB_HFSR_REG(base)                       ((base)->HFSR)
#define FNET_LPC_SCB_DFSR_REG(base)                       ((base)->DFSR)
#define FNET_LPC_SCB_MMFAR_REG(base)                      ((base)->MMFAR)
#define FNET_LPC_SCB_BFAR_REG(base)                       ((base)->BFAR)
#define FNET_LPC_SCB_AFSR_REG(base)                       ((base)->AFSR)

/* ----------------------------------------------------------------------------
   -- SCB Register Masks
   ---------------------------------------------------------------------------- */

/* ACTLR Bit Fields */
#define FNET_LPC_SCB_ACTLR_DISMCYCINT_MASK                0x1u
#define FNET_LPC_SCB_ACTLR_DISMCYCINT_SHIFT               0
#define FNET_LPC_SCB_ACTLR_DISDEFWBUF_MASK                0x2u
#define FNET_LPC_SCB_ACTLR_DISDEFWBUF_SHIFT               1
#define FNET_LPC_SCB_ACTLR_DISFOLD_MASK                   0x4u
#define FNET_LPC_SCB_ACTLR_DISFOLD_SHIFT                  2
/* CPUID Bit Fields */
#define FNET_LPC_SCB_CPUID_REVISION_MASK                  0xFu
#define FNET_LPC_SCB_CPUID_REVISION_SHIFT                 0
#define FNET_LPC_SCB_CPUID_REVISION(x)                    (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_SCB_CPUID_REVISION_SHIFT))&FNET_LPC_SCB_CPUID_REVISION_MASK)
#define FNET_LPC_SCB_CPUID_PARTNO_MASK                    0xFFF0u
#define FNET_LPC_SCB_CPUID_PARTNO_SHIFT                   4
#define FNET_LPC_SCB_CPUID_PARTNO(x)                      (((fnet_uint32_t)(((fnrt_uint32)(x))<<FNET_LPC_SCB_CPUID_PARTNO_SHIFT))&FNET_LPC_SCB_CPUID_PARTNO_MASK)
#define FNET_LPC_SCB_CPUID_VARIANT_MASK                   0xF00000u
#define FNET_LPC_SCB_CPUID_VARIANT_SHIFT                  20
#define FNET_LPC_SCB_CPUID_VARIANT(x)                     (((fnrt_uint32)(((fnet_uint32_t)(x))<<FNET_LPC_SCB_CPUID_VARIANT_SHIFT))&FNET_LPC_SCB_CPUID_VARIANT_MASK)
#define FNET_LPC_SCB_CPUID_IMPLEMENTER_MASK               0xFF000000u
#define FNET_LPC_SCB_CPUID_IMPLEMENTER_SHIFT              24
#define FNET_LPC_SCB_CPUID_IMPLEMENTER(x)                 (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_SCB_CPUID_IMPLEMENTER_SHIFT))&FNET_LPC_SCB_CPUID_IMPLEMENTER_MASK)
/* ICSR Bit Fields */
#define FNET_LPC_SCB_ICSR_VECTACTIVE_MASK                 0x1FFu
#define FNET_LPC_SCB_ICSR_VECTACTIVE_SHIFT                0
#define FNET_LPC_SCB_ICSR_VECTACTIVE(x)                   (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_SCB_ICSR_VECTACTIVE_SHIFT))&FNET_LPC_SCB_ICSR_VECTACTIVE_MASK)
#define FNET_LPC_SCB_ICSR_RETTOBASE_MASK                  0x800u
#define FNET_LPC_SCB_ICSR_RETTOBASE_SHIFT                 11
#define FNET_LPC_SCB_ICSR_VECTPENDING_MASK                0x3F000u
#define FNET_LPC_SCB_ICSR_VECTPENDING_SHIFT               12
#define FNET_LPC_SCB_ICSR_VECTPENDING(x)                  (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_SCB_ICSR_VECTPENDING_SHIFT))&FNET_LPC_SCB_ICSR_VECTPENDING_MASK)
#define FNET_LPC_SCB_ICSR_ISRPENDING_MASK                 0x400000u
#define FNET_LPC_SCB_ICSR_ISRPENDING_SHIFT                22
#define FNET_LPC_SCB_ICSR_ISRPREEMPT_MASK                 0x800000u
#define FNET_LPC_SCB_ICSR_ISRPREEMPT_SHIFT                23
#define FNET_LPC_SCB_ICSR_PENDSTCLR_MASK                  0x2000000u
#define FNET_LPC_SCB_ICSR_PENDSTCLR_SHIFT                 25
#define FNET_LPC_SCB_ICSR_PENDSTSET_MASK                  0x4000000u
#define FNET_LPC_SCB_ICSR_PENDSTSET_SHIFT                 26
#define FNET_LPC_SCB_ICSR_PENDSVCLR_MASK                  0x8000000u
#define FNET_LPC_SCB_ICSR_PENDSVCLR_SHIFT                 27
#define FNET_LPC_SCB_ICSR_PENDSVSET_MASK                  0x10000000u
#define FNET_LPC_SCB_ICSR_PENDSVSET_SHIFT                 28
#define FNET_LPC_SCB_ICSR_NMIPENDSET_MASK                 0x80000000u
#define FNET_LPC_SCB_ICSR_NMIPENDSET_SHIFT                31
/* VTOR Bit Fields */
#define FNET_LPC_SCB_VTOR_TBLOFF_MASK                     0xFFFFFF80u
#define FNET_LPC_SCB_VTOR_TBLOFF_SHIFT                    7
#define FNET_LPC_SCB_VTOR_TBLOFF(x)                       (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_SCB_VTOR_TBLOFF_SHIFT))&FNET_LPC_SCB_VTOR_TBLOFF_MASK)
/* AIRCR Bit Fields */
#define FNET_LPC_SCB_AIRCR_VECTRESET_MASK                 0x1u
#define FNET_LPC_SCB_AIRCR_VECTRESET_SHIFT                0
#define FNET_LPC_SCB_AIRCR_VECTCLRACTIVE_MASK             0x2u
#define FNET_LPC_SCB_AIRCR_VECTCLRACTIVE_SHIFT            1
#define FNET_LPC_SCB_AIRCR_SYSRESETREQ_MASK               0x4u
#define FNET_LPC_SCB_AIRCR_SYSRESETREQ_SHIFT              2
#define FNET_LPC_SCB_AIRCR_PRIGROUP_MASK                  0x700u
#define FNET_LPC_SCB_AIRCR_PRIGROUP_SHIFT                 8
#define FNET_LPC_SCB_AIRCR_PRIGROUP(x)                    (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_SCB_AIRCR_PRIGROUP_SHIFT))&FNET_LPC_SCB_AIRCR_PRIGROUP_MASK)
#define FNET_LPC_SCB_AIRCR_ENDIANNESS_MASK                0x8000u
#define FNET_LPC_SCB_AIRCR_ENDIANNESS_SHIFT               15
#define FNET_LPC_SCB_AIRCR_VECTKEY_MASK                   0xFFFF0000u
#define FNET_LPC_SCB_AIRCR_VECTKEY_SHIFT                  16
#define FNET_LPC_SCB_AIRCR_VECTKEY(x)                     (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_SCB_AIRCR_VECTKEY_SHIFT))&FNET_LPC_SCB_AIRCR_VECTKEY_MASK)
/* SCR Bit Fields */
#define FNET_LPC_SCB_SCR_SLEEPONEXIT_MASK                 0x2u
#define FNET_LPC_SCB_SCR_SLEEPONEXIT_SHIFT                1
#define FNET_LPC_SCB_SCR_SLEEPDEEP_MASK                   0x4u
#define FNET_LPC_SCB_SCR_SLEEPDEEP_SHIFT                  2
#define FNET_LPC_SCB_SCR_SEVONPEND_MASK                   0x10u
#define FNET_LPC_SCB_SCR_SEVONPEND_SHIFT                  4
/* CCR Bit Fields */
#define FNET_LPC_SCB_CCR_NONBASETHRDENA_MASK              0x1u
#define FNET_LPC_SCB_CCR_NONBASETHRDENA_SHIFT             0
#define FNET_LPC_SCB_CCR_USERSETMPEND_MASK                0x2u
#define FNET_LPC_SCB_CCR_USERSETMPEND_SHIFT               1
#define FNET_LPC_SCB_CCR_UNALIGN_TRP_MASK                 0x8u
#define FNET_LPC_SCB_CCR_UNALIGN_TRP_SHIFT                3
#define FNET_LPC_SCB_CCR_DIV_0_TRP_MASK                   0x10u
#define FNET_LPC_SCB_CCR_DIV_0_TRP_SHIFT                  4
#define FNET_LPC_SCB_CCR_BFHFNMIGN_MASK                   0x100u
#define FNET_LPC_SCB_CCR_BFHFNMIGN_SHIFT                  8
#define FNET_LPC_SCB_CCR_STKALIGN_MASK                    0x200u
#define FNET_LPC_SCB_CCR_STKALIGN_SHIFT                   9
/* SHPR1 Bit Fields */
#define FNET_LPC_SCB_SHPR1_PRI_4_MASK                     0xFFu
#define FNET_LPC_SCB_SHPR1_PRI_4_SHIFT                    0
#define FNET_LPC_SCB_SHPR1_PRI_4(x)                       (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_SCB_SHPR1_PRI_4_SHIFT))&FNET_LPC_SCB_SHPR1_PRI_4_MASK)
#define FNET_LPC_SCB_SHPR1_PRI_5_MASK                     0xFF00u
#define FNET_LPC_SCB_SHPR1_PRI_5_SHIFT                    8
#define FNET_LPC_SCB_SHPR1_PRI_5(x)                       (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_SCB_SHPR1_PRI_5_SHIFT))&FNET_LPC_SCB_SHPR1_PRI_5_MASK)
#define FNET_LPC_SCB_SHPR1_PRI_6_MASK                     0xFF0000u
#define FNET_LPC_SCB_SHPR1_PRI_6_SHIFT                    16
#define FNET_LPC_SCB_SHPR1_PRI_6(x)                       (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_SCB_SHPR1_PRI_6_SHIFT))&FNET_LPC_SCB_SHPR1_PRI_6_MASK)
/* SHPR2 Bit Fields */
#define FNET_LPC_SCB_SHPR2_PRI_11_MASK                    0xFF000000u
#define FNET_LPC_SCB_SHPR2_PRI_11_SHIFT                   24
#define FNET_LPC_SCB_SHPR2_PRI_11(x)                      (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_SCB_SHPR2_PRI_11_SHIFT))&FNET_LPC_SCB_SHPR2_PRI_11_MASK)
/* SHPR3 Bit Fields */
#define FNET_LPC_SCB_SHPR3_PRI_14_MASK                    0xFF0000u
#define FNET_LPC_SCB_SHPR3_PRI_14_SHIFT                   16
#define FNET_LPC_SCB_SHPR3_PRI_14(x)                      (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_SCB_SHPR3_PRI_14_SHIFT))&FNET_LPC_SCB_SHPR3_PRI_14_MASK)
#define FNET_LPC_SCB_SHPR3_PRI_15_MASK                    0xFF000000u
#define FNET_LPC_SCB_SHPR3_PRI_15_SHIFT                   24
#define FNET_LPC_SCB_SHPR3_PRI_15(x)                      (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_SCB_SHPR3_PRI_15_SHIFT))&FNET_LPC_SCB_SHPR3_PRI_15_MASK)
/* SHCSR Bit Fields */
#define FNET_LPC_SCB_SHCSR_MEMFAULTACT_MASK               0x1u
#define FNET_LPC_SCB_SHCSR_MEMFAULTACT_SHIFT              0
#define FNET_LPC_SCB_SHCSR_BUSFAULTACT_MASK               0x2u
#define FNET_LPC_SCB_SHCSR_BUSFAULTACT_SHIFT              1
#define FNET_LPC_SCB_SHCSR_USGFAULTACT_MASK               0x8u
#define FNET_LPC_SCB_SHCSR_USGFAULTACT_SHIFT              3
#define FNET_LPC_SCB_SHCSR_SVCALLACT_MASK                 0x80u
#define FNET_LPC_SCB_SHCSR_SVCALLACT_SHIFT                7
#define FNET_LPC_SCB_SHCSR_MONITORACT_MASK                0x100u
#define FNET_LPC_SCB_SHCSR_MONITORACT_SHIFT               8
#define FNET_LPC_SCB_SHCSR_PENDSVACT_MASK                 0x400u
#define FNET_LPC_SCB_SHCSR_PENDSVACT_SHIFT                10
#define FNET_LPC_SCB_SHCSR_SYSTICKACT_MASK                0x800u
#define FNET_LPC_SCB_SHCSR_SYSTICKACT_SHIFT               11
#define FNET_LPC_SCB_SHCSR_USGFAULTPENDED_MASK            0x1000u
#define FNET_LPC_SCB_SHCSR_USGFAULTPENDED_SHIFT           12
#define FNET_LPC_SCB_SHCSR_MEMFAULTPENDED_MASK            0x2000u
#define FNET_LPC_SCB_SHCSR_MEMFAULTPENDED_SHIFT           13
#define FNET_LPC_SCB_SHCSR_BUSFAULTPENDED_MASK            0x4000u
#define FNET_LPC_SCB_SHCSR_BUSFAULTPENDED_SHIFT           14
#define FNET_LPC_SCB_SHCSR_SVCALLPENDED_MASK              0x8000u
#define FNET_LPC_SCB_SHCSR_SVCALLPENDED_SHIFT             15
#define FNET_LPC_SCB_SHCSR_MEMFAULTENA_MASK               0x10000u
#define FNET_LPC_SCB_SHCSR_MEMFAULTENA_SHIFT              16
#define FNET_LPC_SCB_SHCSR_BUSFAULTENA_MASK               0x20000u
#define FNET_LPC_SCB_SHCSR_BUSFAULTENA_SHIFT              17
#define FNET_LPC_SCB_SHCSR_USGFAULTENA_MASK               0x40000u
#define FNET_LPC_SCB_SHCSR_USGFAULTENA_SHIFT              18
/* CFSR Bit Fields */
#define FNET_LPC_SCB_CFSR_IACCVIOL_MASK                   0x1u
#define FNET_LPC_SCB_CFSR_IACCVIOL_SHIFT                  0
#define FNET_LPC_SCB_CFSR_DACCVIOL_MASK                   0x2u
#define FNET_LPC_SCB_CFSR_DACCVIOL_SHIFT                  1
#define FNET_LPC_SCB_CFSR_MUNSTKERR_MASK                  0x8u
#define FNET_LPC_SCB_CFSR_MUNSTKERR_SHIFT                 3
#define FNET_LPC_SCB_CFSR_MSTKERR_MASK                    0x10u
#define FNET_LPC_SCB_CFSR_MSTKERR_SHIFT                   4
#define FNET_LPC_SCB_CFSR_MLSPERR_MASK                    0x20u
#define FNET_LPC_SCB_CFSR_MLSPERR_SHIFT                   5
#define FNET_LPC_SCB_CFSR_MMARVALID_MASK                  0x80u
#define FNET_LPC_SCB_CFSR_MMARVALID_SHIFT                 7
#define FNET_LPC_SCB_CFSR_IBUSERR_MASK                    0x100u
#define FNET_LPC_SCB_CFSR_IBUSERR_SHIFT                   8
#define FNET_LPC_SCB_CFSR_PRECISERR_MASK                  0x200u
#define FNET_LPC_SCB_CFSR_PRECISERR_SHIFT                 9
#define FNET_LPC_SCB_CFSR_IMPRECISERR_MASK                0x400u
#define FNET_LPC_SCB_CFSR_IMPRECISERR_SHIFT               10
#define FNET_LPC_SCB_CFSR_UNSTKERR_MASK                   0x800u
#define FNET_LPC_SCB_CFSR_UNSTKERR_SHIFT                  11
#define FNET_LPC_SCB_CFSR_STKERR_MASK                     0x1000u
#define FNET_LPC_SCB_CFSR_STKERR_SHIFT                    12
#define FNET_LPC_SCB_CFSR_LSPERR_MASK                     0x2000u
#define FNET_LPC_SCB_CFSR_LSPERR_SHIFT                    13
#define FNET_LPC_SCB_CFSR_BFARVALID_MASK                  0x8000u
#define FNET_LPC_SCB_CFSR_BFARVALID_SHIFT                 15
#define FNET_LPC_SCB_CFSR_UNDEFINSTR_MASK                 0x10000u
#define FNET_LPC_SCB_CFSR_UNDEFINSTR_SHIFT                16
#define FNET_LPC_SCB_CFSR_INVSTATE_MASK                   0x20000u
#define FNET_LPC_SCB_CFSR_INVSTATE_SHIFT                  17
#define FNET_LPC_SCB_CFSR_INVPC_MASK                      0x40000u
#define FNET_LPC_SCB_CFSR_INVPC_SHIFT                     18
#define FNET_LPC_SCB_CFSR_NOCP_MASK                       0x80000u
#define FNET_LPC_SCB_CFSR_NOCP_SHIFT                      19
#define FNET_LPC_SCB_CFSR_UNALIGNED_MASK                  0x1000000u
#define FNET_LPC_SCB_CFSR_UNALIGNED_SHIFT                 24
#define FNET_LPC_SCB_CFSR_DIVBYZERO_MASK                  0x2000000u
#define FNET_LPC_SCB_CFSR_DIVBYZERO_SHIFT                 25
/* HFSR Bit Fields */
#define FNET_LPC_SCB_HFSR_VECTTBL_MASK                    0x2u
#define FNET_LPC_SCB_HFSR_VECTTBL_SHIFT                   1
#define FNET_LPC_SCB_HFSR_FORCED_MASK                     0x40000000u
#define FNET_LPC_SCB_HFSR_FORCED_SHIFT                    30
#define FNET_LPC_SCB_HFSR_DEBUGEVT_MASK                   0x80000000u
#define FNET_LPC_SCB_HFSR_DEBUGEVT_SHIFT                  31
/* DFSR Bit Fields */
#define FNET_LPC_SCB_DFSR_HALTED_MASK                     0x1u
#define FNET_LPC_SCB_DFSR_HALTED_SHIFT                    0
#define FNET_LPC_SCB_DFSR_BKPT_MASK                       0x2u
#define FNET_LPC_SCB_DFSR_BKPT_SHIFT                      1
#define FNET_LPC_SCB_DFSR_DWTTRAP_MASK                    0x4u
#define FNET_LPC_SCB_DFSR_DWTTRAP_SHIFT                   2
#define FNET_LPC_SCB_DFSR_VCATCH_MASK                     0x8u
#define FNET_LPC_SCB_DFSR_VCATCH_SHIFT                    3
#define FNET_LPC_SCB_DFSR_EXTERNAL_MASK                   0x10u
#define FNET_LPC_SCB_DFSR_EXTERNAL_SHIFT                  4
/* MMFAR Bit Fields */
#define FNET_LPC_SCB_MMFAR_ADDRESS_MASK                   0xFFFFFFFFu
#define FNET_LPC_SCB_MMFAR_ADDRESS_SHIFT                  0
#define FNET_LPC_SCB_MMFAR_ADDRESS(x)                     (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_SCB_MMFAR_ADDRESS_SHIFT))&FNET_LPC_SCB_MMFAR_ADDRESS_MASK)
/* BFAR Bit Fields */
#define FNET_LPC_SCB_BFAR_ADDRESS_MASK                    0xFFFFFFFFu
#define FNET_LPC_SCB_BFAR_ADDRESS_SHIFT                   0
#define FNET_LPC_SCB_BFAR_ADDRESS(x)                      (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_SCB_BFAR_ADDRESS_SHIFT))&FNET_LPC_SCB_BFAR_ADDRESS_MASK)
/* AFSR Bit Fields */
#define FNET_LPC_SCB_AFSR_AUXFAULT_MASK                   0xFFFFFFFFu
#define FNET_LPC_SCB_AFSR_AUXFAULT_SHIFT                  0
#define FNET_LPC_SCB_AFSR_AUXFAULT(x)                     (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_SCB_AFSR_AUXFAULT_SHIFT))&FNET_LPC_SCB_AFSR_AUXFAULT_MASK)


/* SCB - Peripheral instance base addresses */
/* Peripheral SystemControl base pointer */
#define FNET_LPC_SystemControl_BASE_PTR                   ((FNET_LPC_SCB_MemMapPtr)0xE000E000u)

/* ----------------------------------------------------------------------------
   -- SCB - Register accessor macros
   ---------------------------------------------------------------------------- */

/* SCB - Register instance definitions */
/* SystemControl */
#define FNET_LPC_SCB_ACTLR                                FNET_LPC_SCB_ACTLR_REG(FNET_LPC_SystemControl_BASE_PTR)
#define FNET_LPC_SCB_CPUID                                FNET_LPC_SCB_CPUID_REG(FNET_LPC_SystemControl_BASE_PTR)
#define FNET_LPC_SCB_ICSR                                 FNET_LPC_SCB_ICSR_REG(FNET_LPC_SystemControl_BASE_PTR)
#define FNET_LPC_SCB_AIRCR                                FNET_LPC_SCB_AIRCR_REG(FNET_LPC_SystemControl_BASE_PTR)
#define FNET_LPC_SCB_VTOR                                 FNET_LPC_SCB_VTOR_REG(FNET_LPC_SystemControl_BASE_PTR)
#define FNET_LPC_SCB_SCR                                  FNET_LPC_SCB_SCR_REG(FNET_LPC_SystemControl_BASE_PTR)
#define FNET_LPC_SCB_CCR                                  FNET_LPC_SCB_CCR_REG(FNET_LPC_SystemControl_BASE_PTR)
#define FNET_LPC_SCB_SHPR1                                FNET_LPC_SCB_SHPR1_REG(FNET_LPC_SystemControl_BASE_PTR)
#define FNET_LPC_SCB_SHPR2                                FNET_LPC_SCB_SHPR2_REG(FNET_LPC_SystemControl_BASE_PTR)
#define FNET_LPC_SCB_SHPR3                                FNET_LPC_SCB_SHPR3_REG(FNET_LPC_SystemControl_BASE_PTR)
#define FNET_LPC_SCB_SHCSR                                FNET_LPC_SCB_SHCSR_REG(FNET_LPC_SystemControl_BASE_PTR)
#define FNET_LPC_SCB_CFSR                                 FNET_LPC_SCB_CFSR_REG(FNET_LPC_SystemControl_BASE_PTR)
#define FNET_LPC_SCB_HFSR                                 FNET_LPC_SCB_HFSR_REG(FNET_LPC_SystemControl_BASE_PTR)
#define FNET_LPC_SCB_DFSR                                 FNET_LPC_SCB_DFSR_REG(FNET_LPC_SystemControl_BASE_PTR)
#define FNET_LPC_SCB_MMFAR                                FNET_LPC_SCB_MMFAR_REG(FNET_LPC_SystemControl_BASE_PTR)
#define FNET_LPC_SCB_BFAR                                 FNET_LPC_SCB_BFAR_REG(FNET_LPC_SystemControl_BASE_PTR)
#define FNET_LPC_SCB_AFSR                                 FNET_LPC_SCB_AFSR_REG(FNET_LPC_SystemControl_BASE_PTR)

/*********************************************************************
* NVIC
*********************************************************************/

/* NVIC - Peripheral register structure */
typedef struct FNET_LPC_NVIC_MemMap
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
} volatile *FNET_LPC_NVIC_MemMapPtr;

/* ----------------------------------------------------------------------------
   -- NVIC - Register accessor macros
   ---------------------------------------------------------------------------- */

/* NVIC - Register accessors */
#define FNET_LPC_NVIC_ISER_REG(base,index)                ((base)->ISER[(index)])
#define FNET_LPC_NVIC_ICER_REG(base,index)                ((base)->ICER[(index)])
#define FNET_LPC_NVIC_ISPR_REG(base,index)                ((base)->ISPR[(index)])
#define FNET_LPC_NVIC_ICPR_REG(base,index)                ((base)->ICPR[(index)])
#define FNET_LPC_NVIC_IABR_REG(base,index)                ((base)->IABR[(index)])
#define FNET_LPC_NVIC_IP_REG(base,index)                  ((base)->IP[(index)])
#define FNET_LPC_NVIC_STIR_REG(base,index)                ((base)->STIR[(index)])

/* ----------------------------------------------------------------------------
   -- NVIC Register Masks
   ---------------------------------------------------------------------------- */

/* ISER Bit Fields */
#define FNET_LPC_NVIC_ISER_SETENA_MASK                    0xFFFFFFFFu
#define FNET_LPC_NVIC_ISER_SETENA_SHIFT                   0
#define FNET_LPC_NVIC_ISER_SETENA(x)                      (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_NVIC_ISER_SETENA_SHIFT))&FNET_LPC_NVIC_ISER_SETENA_MASK)
/* ICER Bit Fields */
#define FNET_LPC_NVIC_ICER_CLRENA_MASK                    0xFFFFFFFFu
#define FNET_LPC_NVIC_ICER_CLRENA_SHIFT                   0
#define FNET_LPC_NVIC_ICER_CLRENA(x)                      (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_NVIC_ICER_CLRENA_SHIFT))&FNET_LPC_NVIC_ICER_CLRENA_MASK)
/* ISPR Bit Fields */
#define FNET_LPC_NVIC_ISPR_SETPEND_MASK                   0xFFFFFFFFu
#define FNET_LPC_NVIC_ISPR_SETPEND_SHIFT                  0
#define FNET_LPC_NVIC_ISPR_SETPEND(x)                     (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_NVIC_ISPR_SETPEND_SHIFT))&FNET_LPC_NVIC_ISPR_SETPEND_MASK)
/* ICPR Bit Fields */
#define FNET_LPC_NVIC_ICPR_CLRPEND_MASK                   0xFFFFFFFFu
#define FNET_LPC_NVIC_ICPR_CLRPEND_SHIFT                  0
#define FNET_LPC_NVIC_ICPR_CLRPEND(x)                     (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_NVIC_ICPR_CLRPEND_SHIFT))&FNET_LPC_NVIC_ICPR_CLRPEND_MASK)
/* IABR Bit Fields */
#define FNET_LPC_NVIC_IABR_ACTIVE_MASK                    0xFFFFFFFFu
#define FNET_LPC_NVIC_IABR_ACTIVE_SHIFT                   0
#define FNET_LPC_NVIC_IABR_ACTIVE(x)                      (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_NVIC_IABR_ACTIVE_SHIFT))&FNET_LPC_NVIC_IABR_ACTIVE_MASK)
/* IP Bit Fields */
#define FNET_LPC_NVIC_IP_PRI0_MASK                        0xFFu
#define FNET_LPC_NVIC_IP_PRI0_SHIFT                       0
#define FNET_LPC_NVIC_IP_PRI0(x)                          (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI0_SHIFT))&FNET_LPC_NVIC_IP_PRI0_MASK)
#define FNET_LPC_NVIC_IP_PRI1_MASK                        0xFFu
#define FNET_LPC_NVIC_IP_PRI1_SHIFT                       0
#define FNET_LPC_NVIC_IP_PRI1(x)                          (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI1_SHIFT))&FNET_LPC_NVIC_IP_PRI1_MASK)
#define FNET_LPC_NVIC_IP_PRI2_MASK                        0xFFu
#define FNET_LPC_NVIC_IP_PRI2_SHIFT                       0
#define FNET_LPC_NVIC_IP_PRI2(x)                          (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI2_SHIFT))&FNET_LPC_NVIC_IP_PRI2_MASK)
#define FNET_LPC_NVIC_IP_PRI3_MASK                        0xFFu
#define FNET_LPC_NVIC_IP_PRI3_SHIFT                       0
#define FNET_LPC_NVIC_IP_PRI3(x)                          (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI3_SHIFT))&FNET_LPC_NVIC_IP_PRI3_MASK)
#define FNET_LPC_NVIC_IP_PRI4_MASK                        0xFFu
#define FNET_LPC_NVIC_IP_PRI4_SHIFT                       0
#define FNET_LPC_NVIC_IP_PRI4(x)                          (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI4_SHIFT))&FNET_LPC_NVIC_IP_PRI4_MASK)
#define FNET_LPC_NVIC_IP_PRI5_MASK                        0xFFu
#define FNET_LPC_NVIC_IP_PRI5_SHIFT                       0
#define FNET_LPC_NVIC_IP_PRI5(x)                          (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI5_SHIFT))&FNET_LPC_NVIC_IP_PRI5_MASK)
#define FNET_LPC_NVIC_IP_PRI6_MASK                        0xFFu
#define FNET_LPC_NVIC_IP_PRI6_SHIFT                       0
#define FNET_LPC_NVIC_IP_PRI6(x)                          (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI6_SHIFT))&FNET_LPC_NVIC_IP_PRI6_MASK)
#define FNET_LPC_NVIC_IP_PRI7_MASK                        0xFFu
#define FNET_LPC_NVIC_IP_PRI7_SHIFT                       0
#define FNET_LPC_NVIC_IP_PRI7(x)                          (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI7_SHIFT))&FNET_LPC_NVIC_IP_PRI7_MASK)
#define FNET_LPC_NVIC_IP_PRI8_MASK                        0xFFu
#define FNET_LPC_NVIC_IP_PRI8_SHIFT                       0
#define FNET_LPC_NVIC_IP_PRI8(x)                          (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI8_SHIFT))&FNET_LPC_NVIC_IP_PRI8_MASK)
#define FNET_LPC_NVIC_IP_PRI9_MASK                        0xFFu
#define FNET_LPC_NVIC_IP_PRI9_SHIFT                       0
#define FNET_LPC_NVIC_IP_PRI9(x)                          (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI9_SHIFT))&FNET_LPC_NVIC_IP_PRI9_MASK)
#define FNET_LPC_NVIC_IP_PRI10_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI10_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI10(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI10_SHIFT))&FNET_LPC_NVIC_IP_PRI10_MASK)
#define FNET_LPC_NVIC_IP_PRI11_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI11_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI11(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI11_SHIFT))&FNET_LPC_NVIC_IP_PRI11_MASK)
#define FNET_LPC_NVIC_IP_PRI12_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI12_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI12(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI12_SHIFT))&FNET_LPC_NVIC_IP_PRI12_MASK)
#define FNET_LPC_NVIC_IP_PRI13_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI13_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI13(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI13_SHIFT))&FNET_LPC_NVIC_IP_PRI13_MASK)
#define FNET_LPC_NVIC_IP_PRI14_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI14_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI14(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI14_SHIFT))&FNET_LPC_NVIC_IP_PRI14_MASK)
#define FNET_LPC_NVIC_IP_PRI15_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI15_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI15(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI15_SHIFT))&FNET_LPC_NVIC_IP_PRI15_MASK)
#define FNET_LPC_NVIC_IP_PRI16_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI16_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI16(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI16_SHIFT))&FNET_LPC_NVIC_IP_PRI16_MASK)
#define FNET_LPC_NVIC_IP_PRI17_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI17_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI17(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI17_SHIFT))&FNET_LPC_NVIC_IP_PRI17_MASK)
#define FNET_LPC_NVIC_IP_PRI18_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI18_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI18(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI18_SHIFT))&FNET_LPC_NVIC_IP_PRI18_MASK)
#define FNET_LPC_NVIC_IP_PRI19_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI19_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI19(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI19_SHIFT))&FNET_LPC_NVIC_IP_PRI19_MASK)
#define FNET_LPC_NVIC_IP_PRI20_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI20_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI20(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI20_SHIFT))&FNET_LPC_NVIC_IP_PRI20_MASK)
#define FNET_LPC_NVIC_IP_PRI21_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI21_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI21(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI21_SHIFT))&FNET_LPC_NVIC_IP_PRI21_MASK)
#define FNET_LPC_NVIC_IP_PRI22_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI22_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI22(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI22_SHIFT))&FNET_LPC_NVIC_IP_PRI22_MASK)
#define FNET_LPC_NVIC_IP_PRI23_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI23_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI23(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI23_SHIFT))&FNET_LPC_NVIC_IP_PRI23_MASK)
#define FNET_LPC_NVIC_IP_PRI24_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI24_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI24(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI24_SHIFT))&FNET_LPC_NVIC_IP_PRI24_MASK)
#define FNET_LPC_NVIC_IP_PRI25_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI25_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI25(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI25_SHIFT))&FNET_LPC_NVIC_IP_PRI25_MASK)
#define FNET_LPC_NVIC_IP_PRI26_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI26_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI26(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI26_SHIFT))&FNET_LPC_NVIC_IP_PRI26_MASK)
#define FNET_LPC_NVIC_IP_PRI27_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI27_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI27(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI27_SHIFT))&FNET_LPC_NVIC_IP_PRI27_MASK)
#define FNET_LPC_NVIC_IP_PRI28_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI28_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI28(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI28_SHIFT))&FNET_LPC_NVIC_IP_PRI28_MASK)
#define FNET_LPC_NVIC_IP_PRI29_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI29_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI29(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI29_SHIFT))&FNET_LPC_NVIC_IP_PRI29_MASK)
#define FNET_LPC_NVIC_IP_PRI30_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI30_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI30(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI30_SHIFT))&FNET_LPC_NVIC_IP_PRI30_MASK)
#define FNET_LPC_NVIC_IP_PRI31_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI31_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI31(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI31_SHIFT))&FNET_LPC_NVIC_IP_PRI31_MASK)
#define FNET_LPC_NVIC_IP_PRI32_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI32_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI32(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI32_SHIFT))&FNET_LPC_NVIC_IP_PRI32_MASK)
#define FNET_LPC_NVIC_IP_PRI33_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI33_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI33(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI33_SHIFT))&FNET_LPC_NVIC_IP_PRI33_MASK)
#define FNET_LPC_NVIC_IP_PRI34_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI34_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI34(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI34_SHIFT))&FNET_LPC_NVIC_IP_PRI34_MASK)
#define FNET_LPC_NVIC_IP_PRI35_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI35_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI35(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI35_SHIFT))&FNET_LPC_NVIC_IP_PRI35_MASK)
#define FNET_LPC_NVIC_IP_PRI36_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI36_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI36(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI36_SHIFT))&FNET_LPC_NVIC_IP_PRI36_MASK)
#define FNET_LPC_NVIC_IP_PRI37_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI37_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI37(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI37_SHIFT))&FNET_LPC_NVIC_IP_PRI37_MASK)
#define FNET_LPC_NVIC_IP_PRI38_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI38_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI38(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI38_SHIFT))&FNET_LPC_NVIC_IP_PRI38_MASK)
#define FNET_LPC_NVIC_IP_PRI39_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI39_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI39(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI39_SHIFT))&FNET_LPC_NVIC_IP_PRI39_MASK)
#define FNET_LPC_NVIC_IP_PRI40_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI40_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI40(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI40_SHIFT))&FNET_LPC_NVIC_IP_PRI40_MASK)
#define FNET_LPC_NVIC_IP_PRI41_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI41_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI41(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI41_SHIFT))&FNET_LPC_NVIC_IP_PRI41_MASK)
#define FNET_LPC_NVIC_IP_PRI42_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI42_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI42(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI42_SHIFT))&FNET_LPC_NVIC_IP_PRI42_MASK)
#define FNET_LPC_NVIC_IP_PRI43_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI43_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI43(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI43_SHIFT))&FNET_LPC_NVIC_IP_PRI43_MASK)
#define FNET_LPC_NVIC_IP_PRI44_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI44_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI44(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI44_SHIFT))&FNET_LPC_NVIC_IP_PRI44_MASK)
#define FNET_LPC_NVIC_IP_PRI45_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI45_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI45(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI45_SHIFT))&FNET_LPC_NVIC_IP_PRI45_MASK)
#define FNET_LPC_NVIC_IP_PRI46_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI46_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI46(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI46_SHIFT))&FNET_LPC_NVIC_IP_PRI46_MASK)
#define FNET_LPC_NVIC_IP_PRI47_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI47_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI47(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI47_SHIFT))&FNET_LPC_NVIC_IP_PRI47_MASK)
#define FNET_LPC_NVIC_IP_PRI48_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI48_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI48(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI48_SHIFT))&FNET_LPC_NVIC_IP_PRI48_MASK)
#define FNET_LPC_NVIC_IP_PRI49_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI49_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI49(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI49_SHIFT))&FNET_LPC_NVIC_IP_PRI49_MASK)
#define FNET_LPC_NVIC_IP_PRI50_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI50_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI50(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI50_SHIFT))&FNET_LPC_NVIC_IP_PRI50_MASK)
#define FNET_LPC_NVIC_IP_PRI51_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI51_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI51(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI51_SHIFT))&FNET_LPC_NVIC_IP_PRI51_MASK)
#define FNET_LPC_NVIC_IP_PRI52_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI52_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI52(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI52_SHIFT))&FNET_LPC_NVIC_IP_PRI52_MASK)
#define FNET_LPC_NVIC_IP_PRI53_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI53_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI53(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI53_SHIFT))&FNET_LPC_NVIC_IP_PRI53_MASK)
#define FNET_LPC_NVIC_IP_PRI54_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI54_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI54(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI54_SHIFT))&FNET_LPC_NVIC_IP_PRI54_MASK)
#define FNET_LPC_NVIC_IP_PRI55_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI55_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI55(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI55_SHIFT))&FNET_LPC_NVIC_IP_PRI55_MASK)
#define FNET_LPC_NVIC_IP_PRI56_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI56_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI56(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI56_SHIFT))&FNET_LPC_NVIC_IP_PRI56_MASK)
#define FNET_LPC_NVIC_IP_PRI57_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI57_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI57(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI57_SHIFT))&FNET_LPC_NVIC_IP_PRI57_MASK)
#define FNET_LPC_NVIC_IP_PRI58_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI58_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI58(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI58_SHIFT))&FNET_LPC_NVIC_IP_PRI58_MASK)
#define FNET_LPC_NVIC_IP_PRI59_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI59_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI59(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI59_SHIFT))&FNET_LPC_NVIC_IP_PRI59_MASK)
#define FNET_LPC_NVIC_IP_PRI60_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI60_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI60(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI60_SHIFT))&FNET_LPC_NVIC_IP_PRI60_MASK)
#define FNET_LPC_NVIC_IP_PRI61_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI61_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI61(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI61_SHIFT))&FNET_LPC_NVIC_IP_PRI61_MASK)
#define FNET_LPC_NVIC_IP_PRI62_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI62_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI62(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI62_SHIFT))&FNET_LPC_NVIC_IP_PRI62_MASK)
#define FNET_LPC_NVIC_IP_PRI63_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI63_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI63(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI63_SHIFT))&FNET_LPC_NVIC_IP_PRI63_MASK)
#define FNET_LPC_NVIC_IP_PRI64_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI64_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI64(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI64_SHIFT))&FNET_LPC_NVIC_IP_PRI64_MASK)
#define FNET_LPC_NVIC_IP_PRI65_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI65_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI65(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI65_SHIFT))&FNET_LPC_NVIC_IP_PRI65_MASK)
#define FNET_LPC_NVIC_IP_PRI66_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI66_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI66(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI66_SHIFT))&FNET_LPC_NVIC_IP_PRI66_MASK)
#define FNET_LPC_NVIC_IP_PRI67_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI67_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI67(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI67_SHIFT))&FNET_LPC_NVIC_IP_PRI67_MASK)
#define FNET_LPC_NVIC_IP_PRI68_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI68_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI68(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI68_SHIFT))&FNET_LPC_NVIC_IP_PRI68_MASK)
#define FNET_LPC_NVIC_IP_PRI69_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI69_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI69(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI69_SHIFT))&FNET_LPC_NVIC_IP_PRI69_MASK)
#define FNET_LPC_NVIC_IP_PRI70_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI70_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI70(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI70_SHIFT))&FNET_LPC_NVIC_IP_PRI70_MASK)
#define FNET_LPC_NVIC_IP_PRI71_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI71_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI71(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI71_SHIFT))&FNET_LPC_NVIC_IP_PRI71_MASK)
#define FNET_LPC_NVIC_IP_PRI72_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI72_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI72(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI72_SHIFT))&FNET_LPC_NVIC_IP_PRI72_MASK)
#define FNET_LPC_NVIC_IP_PRI73_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI73_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI73(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI73_SHIFT))&FNET_LPC_NVIC_IP_PRI73_MASK)
#define FNET_LPC_NVIC_IP_PRI74_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI74_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI74(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI74_SHIFT))&FNET_LPC_NVIC_IP_PRI74_MASK)
#define FNET_LPC_NVIC_IP_PRI75_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI75_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI75(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI75_SHIFT))&FNET_LPC_NVIC_IP_PRI75_MASK)
#define FNET_LPC_NVIC_IP_PRI76_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI76_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI76(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI76_SHIFT))&FNET_LPC_NVIC_IP_PRI76_MASK)
#define FNET_LPC_NVIC_IP_PRI77_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI77_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI77(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI77_SHIFT))&FNET_LPC_NVIC_IP_PRI77_MASK)
#define FNET_LPC_NVIC_IP_PRI78_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI78_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI78(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI78_SHIFT))&FNET_LPC_NVIC_IP_PRI78_MASK)
#define FNET_LPC_NVIC_IP_PRI79_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI79_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI79(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI79_SHIFT))&FNET_LPC_NVIC_IP_PRI79_MASK)
#define FNET_LPC_NVIC_IP_PRI80_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI80_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI80(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI80_SHIFT))&FNET_LPC_NVIC_IP_PRI80_MASK)
#define FNET_LPC_NVIC_IP_PRI81_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI81_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI81(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI81_SHIFT))&FNET_LPC_NVIC_IP_PRI81_MASK)
#define FNET_LPC_NVIC_IP_PRI82_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI82_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI82(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI82_SHIFT))&FNET_LPC_NVIC_IP_PRI82_MASK)
#define FNET_LPC_NVIC_IP_PRI83_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI83_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI83(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI83_SHIFT))&FNET_LPC_NVIC_IP_PRI83_MASK)
#define FNET_LPC_NVIC_IP_PRI84_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI84_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI84(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI84_SHIFT))&FNET_LPC_NVIC_IP_PRI84_MASK)
#define FNET_LPC_NVIC_IP_PRI85_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI85_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI85(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI85_SHIFT))&FNET_LPC_NVIC_IP_PRI85_MASK)
#define FNET_LPC_NVIC_IP_PRI86_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI86_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI86(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI86_SHIFT))&FNET_LPC_NVIC_IP_PRI86_MASK)
#define FNET_LPC_NVIC_IP_PRI87_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI87_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI87(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI87_SHIFT))&FNET_LPC_NVIC_IP_PRI87_MASK)
#define FNET_LPC_NVIC_IP_PRI88_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI88_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI88(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI88_SHIFT))&FNET_LPC_NVIC_IP_PRI88_MASK)
#define FNET_LPC_NVIC_IP_PRI89_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI89_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI89(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI89_SHIFT))&FNET_LPC_NVIC_IP_PRI89_MASK)
#define FNET_LPC_NVIC_IP_PRI90_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI90_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI90(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI90_SHIFT))&FNET_LPC_NVIC_IP_PRI90_MASK)
#define FNET_LPC_NVIC_IP_PRI91_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI91_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI91(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI91_SHIFT))&FNET_LPC_NVIC_IP_PRI91_MASK)
#define FNET_LPC_NVIC_IP_PRI92_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI92_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI92(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI92_SHIFT))&FNET_LPC_NVIC_IP_PRI92_MASK)
#define FNET_LPC_NVIC_IP_PRI93_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI93_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI93(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI93_SHIFT))&FNET_LPC_NVIC_IP_PRI93_MASK)
#define FNET_LPC_NVIC_IP_PRI94_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI94_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI94(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI94_SHIFT))&FNET_LPC_NVIC_IP_PRI94_MASK)
#define FNET_LPC_NVIC_IP_PRI95_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI95_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI95(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI95_SHIFT))&FNET_LPC_NVIC_IP_PRI95_MASK)
#define FNET_LPC_NVIC_IP_PRI96_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI96_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI96(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI96_SHIFT))&FNET_LPC_NVIC_IP_PRI96_MASK)
#define FNET_LPC_NVIC_IP_PRI97_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI97_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI97(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI97_SHIFT))&FNET_LPC_NVIC_IP_PRI97_MASK)
#define FNET_LPC_NVIC_IP_PRI98_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI98_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI98(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI98_SHIFT))&FNET_LPC_NVIC_IP_PRI98_MASK)
#define FNET_LPC_NVIC_IP_PRI99_MASK                       0xFFu
#define FNET_LPC_NVIC_IP_PRI99_SHIFT                      0
#define FNET_LPC_NVIC_IP_PRI99(x)                         (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI99_SHIFT))&FNET_LPC_NVIC_IP_PRI99_MASK)
#define FNET_LPC_NVIC_IP_PRI100_MASK                      0xFFu
#define FNET_LPC_NVIC_IP_PRI100_SHIFT                     0
#define FNET_LPC_NVIC_IP_PRI100(x)                        (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI100_SHIFT))&FNET_LPC_NVIC_IP_PRI100_MASK)
#define FNET_LPC_NVIC_IP_PRI101_MASK                      0xFFu
#define FNET_LPC_NVIC_IP_PRI101_SHIFT                     0
#define FNET_LPC_NVIC_IP_PRI101(x)                        (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI101_SHIFT))&FNET_LPC_NVIC_IP_PRI101_MASK)
#define FNET_LPC_NVIC_IP_PRI102_MASK                      0xFFu
#define FNET_LPC_NVIC_IP_PRI102_SHIFT                     0
#define FNET_LPC_NVIC_IP_PRI102(x)                        (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI102_SHIFT))&FNET_LPC_NVIC_IP_PRI102_MASK)
#define FNET_LPC_NVIC_IP_PRI103_MASK                      0xFFu
#define FNET_LPC_NVIC_IP_PRI103_SHIFT                     0
#define FNET_LPC_NVIC_IP_PRI103(x)                        (((fnet_uint8_t)(((fnet_uint8_t)(x))<<FNET_LPC_NVIC_IP_PRI103_SHIFT))&FNET_LPC_NVIC_IP_PRI103_MASK)
/* STIR Bit Fields */
#define FNET_LPC_NVIC_STIR_INTID_MASK                     0x1FFu
#define FNET_LPC_NVIC_STIR_INTID_SHIFT                    0
#define FNET_LPC_NVIC_STIR_INTID(x)                       (((fnet_uint32_t)(((fnet_uint32_t)(x))<<FNET_LPC_NVIC_STIR_INTID_SHIFT))&FNET_LPC_NVIC_STIR_INTID_MASK)


/* NVIC - Peripheral instance base addresses */
/* Peripheral NVIC base pointer */
#define FNET_LPC_NVIC_BASE_PTR                            ((FNET_LPC_NVIC_MemMapPtr)0xE000E100u)

/* ----------------------------------------------------------------------------
   -- NVIC - Register accessor macros
   ---------------------------------------------------------------------------- */

/* NVIC - Register instance definitions */
/* NVIC */
#define FNET_LPC_NVICISER0                                FNET_LPC_NVIC_ISER_REG(FNET_LPC_NVIC_BASE_PTR,0)
#define FNET_LPC_NVICISER1                                FNET_LPC_NVIC_ISER_REG(FNET_LPC_NVIC_BASE_PTR,1)
#define FNET_LPC_NVICISER2                                FNET_LPC_NVIC_ISER_REG(FNET_LPC_NVIC_BASE_PTR,2)
#define FNET_LPC_NVICISER3                                FNET_LPC_NVIC_ISER_REG(FNET_LPC_NVIC_BASE_PTR,3)
#define FNET_LPC_NVICICER0                                FNET_LPC_NVIC_ICER_REG(FNET_LPC_NVIC_BASE_PTR,0)
#define FNET_LPC_NVICICER1                                FNET_LPC_NVIC_ICER_REG(FNET_LPC_NVIC_BASE_PTR,1)
#define FNET_LPC_NVICICER2                                FNET_LPC_NVIC_ICER_REG(FNET_LPC_NVIC_BASE_PTR,2)
#define FNET_LPC_NVICICER3                                FNET_LPC_NVIC_ICER_REG(FNET_LPC_NVIC_BASE_PTR,3)
#define FNET_LPC_NVICISPR0                                FNET_LPC_NVIC_ISPR_REG(FNET_LPC_NVIC_BASE_PTR,0)
#define FNET_LPC_NVICISPR1                                FNET_LPC_NVIC_ISPR_REG(FNET_LPC_NVIC_BASE_PTR,1)
#define FNET_LPC_NVICISPR2                                FNET_LPC_NVIC_ISPR_REG(FNET_LPC_NVIC_BASE_PTR,2)
#define FNET_LPC_NVICISPR3                                FNET_LPC_NVIC_ISPR_REG(FNET_LPC_NVIC_BASE_PTR,3)
#define FNET_LPC_NVICICPR0                                FNET_LPC_NVIC_ICPR_REG(FNET_LPC_NVIC_BASE_PTR,0)
#define FNET_LPC_NVICICPR1                                FNET_LPC_NVIC_ICPR_REG(FNET_LPC_NVIC_BASE_PTR,1)
#define FNET_LPC_NVICICPR2                                FNET_LPC_NVIC_ICPR_REG(FNET_LPC_NVIC_BASE_PTR,2)
#define FNET_LPC_NVICICPR3                                FNET_LPC_NVIC_ICPR_REG(FNET_LPC_NVIC_BASE_PTR,3)
#define FNET_LPC_NVICIABR0                                FNET_LPC_NVIC_IABR_REG(FNET_LPC_NVIC_BASE_PTR,0)
#define FNET_LPC_NVICIABR1                                FNET_LPC_NVIC_IABR_REG(FNET_LPC_NVIC_BASE_PTR,1)
#define FNET_LPC_NVICIABR2                                FNET_LPC_NVIC_IABR_REG(FNET_LPC_NVIC_BASE_PTR,2)
#define FNET_LPC_NVICIABR3                                FNET_LPC_NVIC_IABR_REG(FNET_LPC_NVIC_BASE_PTR,3)
#define FNET_LPC_NVICIP0                                  FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,0)
#define FNET_LPC_NVICIP1                                  FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,1)
#define FNET_LPC_NVICIP2                                  FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,2)
#define FNET_LPC_NVICIP3                                  FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,3)
#define FNET_LPC_NVICIP4                                  FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,4)
#define FNET_LPC_NVICIP5                                  FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,5)
#define FNET_LPC_NVICIP6                                  FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,6)
#define FNET_LPC_NVICIP7                                  FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,7)
#define FNET_LPC_NVICIP8                                  FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,8)
#define FNET_LPC_NVICIP9                                  FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,9)
#define FNET_LPC_NVICIP10                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,10)
#define FNET_LPC_NVICIP11                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,11)
#define FNET_LPC_NVICIP12                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,12)
#define FNET_LPC_NVICIP13                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,13)
#define FNET_LPC_NVICIP14                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,14)
#define FNET_LPC_NVICIP15                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,15)
#define FNET_LPC_NVICIP16                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,16)
#define FNET_LPC_NVICIP17                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,17)
#define FNET_LPC_NVICIP18                                 FNET_LPC_NVIC_IP_REG(FNET_LPC3_NVIC_BASE_PTR,18)
#define FNET_LPC_NVICIP19                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,19)
#define FNET_LPC_NVICIP20                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,20)
#define FNET_LPC_NVICIP21                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,21)
#define FNET_LPC_NVICIP22                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,22)
#define FNET_LPC_NVICIP23                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,23)
#define FNET_LPC_NVICIP24                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,24)
#define FNET_LPC_NVICIP25                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,25)
#define FNET_LPC_NVICIP26                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,26)
#define FNET_LPC_NVICIP27                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,27)
#define FNET_LPC_NVICIP28                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,28)
#define FNET_LPC_NVICIP29                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,29)
#define FNET_LPC_NVICIP30                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,30)
#define FNET_LPC_NVICIP31                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,31)
#define FNET_LPC_NVICIP32                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,32)
#define FNET_LPC_NVICIP33                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,33)
#define FNET_LPC_NVICIP34                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,34)
#define FNET_LPC_NVICIP35                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,35)
#define FNET_LPC_NVICIP36                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,36)
#define FNET_LPC_NVICIP37                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,37)
#define FNET_LPC_NVICIP38                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,38)
#define FNET_LPC_NVICIP39                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,39)
#define FNET_LPC_NVICIP40                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,40)
#define FNET_LPC_NVICIP41                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,41)
#define FNET_LPC_NVICIP42                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,42)
#define FNET_LPC_NVICIP43                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,43)
#define FNET_LPC_NVICIP44                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,44)
#define FNET_LPC_NVICIP45                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,45)
#define FNET_LPC_NVICIP46                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,46)
#define FNET_LPC_NVICIP47                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,47)
#define FNET_LPC_NVICIP48                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,48)
#define FNET_LPC_NVICIP49                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,49)
#define FNET_LPC_NVICIP50                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,50)
#define FNET_LPC_NVICIP51                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,51)
#define FNET_LPC_NVICIP52                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,52)
#define FNET_LPC_NVICIP53                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,53)
#define FNET_LPC_NVICIP54                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,54)
#define FNET_LPC_NVICIP55                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,55)
#define FNET_LPC_NVICIP56                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,56)
#define FNET_LPC_NVICIP57                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,57)
#define FNET_LPC_NVICIP58                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,58)
#define FNET_LPC_NVICIP59                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,59)
#define FNET_LPC_NVICIP60                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,60)
#define FNET_LPC_NVICIP61                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,61)
#define FNET_LPC_NVICIP62                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,62)
#define FNET_LPC_NVICIP63                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,63)
#define FNET_LPC_NVICIP64                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,64)
#define FNET_LPC_NVICIP65                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,65)
#define FNET_LPC_NVICIP66                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,66)
#define FNET_LPC_NVICIP67                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,67)
#define FNET_LPC_NVICIP68                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,68)
#define FNET_LPC_NVICIP69                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,69)
#define FNET_LPC_NVICIP70                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,70)
#define FNET_LPC_NVICIP71                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,71)
#define FNET_LPC_NVICIP72                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,72)
#define FNET_LPC_NVICIP73                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,73)
#define FNET_LPC_NVICIP74                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,74)
#define FNET_LPC_NVICIP75                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,75)
#define FNET_LPC_NVICIP76                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,76)
#define FNET_LPC_NVICIP77                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,77)
#define FNET_LPC_NVICIP78                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,78)
#define FNET_LPC_NVICIP79                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,79)
#define FNET_LPC_NVICIP80                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,80)
#define FNET_LPC_NVICIP81                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,81)
#define FNET_LPC_NVICIP82                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,82)
#define FNET_LPC_NVICIP83                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,83)
#define FNET_LPC_NVICIP84                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,84)
#define FNET_LPC_NVICIP85                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,85)
#define FNET_LPC_NVICIP86                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,86)
#define FNET_LPC_NVICIP87                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,87)
#define FNET_LPC_NVICIP88                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,88)
#define FNET_LPC_NVICIP89                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,89)
#define FNET_LPC_NVICIP90                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,90)
#define FNET_LPC_NVICIP91                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,91)
#define FNET_LPC_NVICIP92                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,92)
#define FNET_LPC_NVICIP93                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,93)
#define FNET_LPC_NVICIP94                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,94)
#define FNET_LPC_NVICIP95                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,95)
#define FNET_LPC_NVICIP96                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,96)
#define FNET_LPC_NVICIP97                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,97)
#define FNET_LPC_NVICIP98                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,98)
#define FNET_LPC_NVICIP99                                 FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,99)
#define FNET_LPC_NVICIP100                                FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,100)
#define FNET_LPC_NVICIP101                                FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,101)
#define FNET_LPC_NVICIP102                                FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,102)
#define FNET_LPC_NVICIP103                                FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,103)
#define FNET_LPC_NVICSTIR                                 FNET_LPC_NVIC_STIR_REG(FNET_LPC_NVIC_BASE_PTR,0)

/* NVIC - Register array accessors */
#define FNET_LPC_NVIC_ISER(index)                         FNET_LPC_NVIC_ISER_REG(FNET_LPC_NVIC_BASE_PTR,(index))
#define FNET_LPC_NVIC_ICER(index)                         FNET_LPC_NVIC_ICER_REG(FNET_LPC_NVIC_BASE_PTR,(index))
#define FNET_LPC_NVIC_ISPR(index)                         FNET_LPC_NVIC_ISPR_REG(FNET_LPC_NVIC_BASE_PTR,(index))
#define FNET_LPC_NVIC_ICPR(index)                         FNET_LPC_NVIC_ICPR_REG(FNET_LPC_NVIC_BASE_PTR,(index))
#define FNET_LPC_NVIC_IABR(index)                         FNET_LPC_NVIC_IABR_REG(FNET_LPC_NVIC_BASE_PTR,(index))
#define FNET_LPC_NVIC_IP(index)                           FNET_LPC_NVIC_IP_REG(FNET_LPC_NVIC_BASE_PTR,(index))
#define FNET_LPC_NVIC_STIR(index)                         FNET_LPC_NVIC_STIR_REG(FNET_LPC_NVIC_BASE_PTR,(index))



//TBD

#if defined(__cplusplus)
extern "C" {
#endif

void fnet_lpc_irq_enable(fnet_uint32_t irq_desc);
fnet_uint32_t fnet_lpc_irq_disable(void);

#if defined(__cplusplus)
}
#endif

#endif /* FNET_LPC */

#endif /*_FNET_LPC_H_*/
