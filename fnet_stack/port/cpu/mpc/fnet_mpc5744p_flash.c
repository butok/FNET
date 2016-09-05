/**************************************************************************
*
* This file includes code from Standard Software Driver (SSD) for C55 Flash module v1.1.0.
*
***************************************************************************
*
* This is FREESCALE SOFTWARE LICENSE AGREEMENT copy of ReleaseNotes.rtf from
* Standard Software Driver (SSD) for C55 Flash module v1.1.0.
*___________________________________________________________________________
*
* IMPORTANT. Read the following Freescale Software License Agreement
* ("Agreement") completely. By selecting the "I Accept" button at the end of
* this page, you indicate that you accept the terms of this Agreement. You
* may then download the file.
*
*                    FREESCALE SOFTWARE LICENSE AGREEMENT
*
* This is a legal agreement between you (either as an individual or as anauthorized representative
* of your employer) and Freescale Semiconductor, Inc. ("Freescale"). It concerns your rights to use
* this file and any accompanying written materials (the "Software"). In consideration for Freescale
* allowing you to access the Software, you are agreeing to be bound by the terms of this
* Agreement. If you do not agree to all of the terms of this Agreement, do not download the
* Software. If you change your mind later, stop using the Software and delete all copies of the
* Software in your possession or control. Any copies of the Software that you have already
* distributed, where permitted, and do not destroy will continue to be governed by this Agreement.
* Your prior use will also continue to be governed by this Agreement.
*
* LICENSE GRANT. Freescale grants to you, free of charge, the non-exclusive, non-transferable
* right (1) to use the Software, (2) to reproduce the Software, (3) to prepare derivative works of the
* Software, (4) to distribute the Software and derivative works thereof in source (human-readable)
* form and object (machine-readable) form, and (5) to
* sublicense to others the right to use the distributed Software. If you violate any of the terms or
* restrictions of this Agreement, Freescale may immediately terminate this Agreement, and require
* that you stop using and delete all copies of the Software in your possession or control.
*
* COPYRIGHT. The Software is licensed to you, not sold. Freescale owns the Software, and
* United States copyright laws and international treaty provisions protect the Software. Therefore,
* you must treat the Software like any other copyrighted material (e.g., a book or musical
* recording). You may not use or copy the Software for any other purpose than what is described in
* this Agreement. Except as expressly provided herein, Freescale does not grant to you any express
* or implied rights under any Freescale or third party patents, copyrights, trademarks, or trade
* secrets. Additionally, you must reproduce and apply any copyright or other proprietary rights
* notices included on or embedded in the Software to any copies or derivative works made thereof,
* in whole or in part, if any.
*
* SUPPORT. Freescale is NOT obligated to provide any support, upgrades or new releases of the
* Software. If you wish, you may contact Freescale and report problems and provide suggestions
* regarding the Software. Freescale has no obligation whatsoever to respond in any way to such a
* problem report or suggestion. Freescale may make changes to the Software at any time, without
* any obligation to notify or provide updated versions of the Software to you.
*
* NO WARRANTY. TO THE MAXIMUM EXTENT PERMITTED BY LAW,  FREESCALE
* EXPRESSLY DISCLAIMS ANY WARRANTY FOR THE SOFTWARE. THE SOFTWARE IS
* PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR
* IMPLIED, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR NON-
* INFRINGEMENT. YOU ASSUME THE ENTIRE RISK ARISING OUT OF THE USE OR
* PERFORMANCE OF THE SOFTWARE, OR ANY SYSTEMS YOU DESIGN USING THE
* SOFTWARE (IF ANY). NOTHING IN THIS AGREEMENT MAY BE CONSTRUED AS A
* WARRANTY OR REPRESENTATION BY FREESCALE THAT THE SOFTWARE OR ANY
* DERIVATIVE WORK DEVELOPED WITH OR INCORPORATING THE SOFTWARE WILL
* BE FREE FROM INFRINGEMENT OF THE INTELLECTUAL PROPERTY RIGHTS OF
* THIRD PARTIES.
*
* INDEMNITY. You agree to fully defend and indemnify Freescale from any and all claims,
* liabilities, and costs (including reasonable attorney's fees) related to (1) your use (including your
* sublicensee's use, if permitted) of the Software or (2) your violation of the terms and conditions
* of this Agreement.
*
* LIMITATION OF LIABILITY. IN NO EVENT WILL FREESCALE BE LIABLE, WHETHER
* IN CONTRACT, TORT, OR OTHERWISE, FOR ANY INCIDENTAL, SPECIAL, INDIRECT,
* CONSEQUENTIAL OR PUNITIVE DAMAGES, INCLUDING, BUT NOT LIMITED TO,
* DAMAGES FOR ANY LOSS OF USE, LOSS OF TIME, INCONVENIENCE, COMMERCIAL
* LOSS, OR LOST PROFITS, SAVINGS, OR REVENUES TO THE FULL EXTENT SUCH
* MAY BE DISCLAIMED BY LAW.
*
* COMPLIANCE WITH LAWS; EXPORT RESTRICTIONS. You must use the Software in
* accordance with all applicable U.S. laws, regulations and statutes. You agree that neither you nor
* your licensees (if any) intend to or will, directly or indirectly, export or transmit the Software to
* any country in violation of U.S. export restrictions.
*
* GOVERNMENT USE. Use of the Software and any corresponding documentation, if
* any, is provided with RESTRICTED RIGHTS. Use, duplication or disclosure by the Government
* is subject to restrictions as set forth in subparagraph (c)(1)(ii) of The Rights in Technical Data
* and Computer Software clause at DFARS 252.227-7013 or subparagraphs (c)(l) and (2) of the
* Commercial Computer Software--Restricted Rights at 48 CFR 52.227-19, as applicable.
* Manufacturer is Freescale, Inc., 6501 William Cannon Drive West, Austin, TX, 78735.
*
* HIGH RISK ACTIVITIES. You acknowledge that the Software is not fault tolerant and is not
* designed, manufactured or intended by Freescale for incorporation into products intended for use
* or resale in on-line control equipment in hazardous, dangerous to life or potentially life-
* threatening environments requiring fail-safe performance, such as in the operation of nuclear
* facilities, aircraft navigation or communication systems, air traffic control, direct life support
* machines or weapons systems, in which the failure of products could lead directly to death,
* personal injury or severe physical or environmental damage ("High Risk Activities"). You
* specifically represent and warrant that you will not use the Software or any derivative work of the
* Software for High Risk Activities.
*
* CHOICE OF LAW; VENUE; LIMITATIONS. You agree that the statutes and laws of
* the United States and the State of Texas, USA, without regard to conflicts of laws principles, will
* apply to all matters relating to this Agreement or the Software, and you agree that any litigation
* will be subject to the exclusive jurisdiction of the state or federal courts in Texas, USA. You
* agree that regardless of any statute or law to the contrary, any claim or cause of action arising out
* of or related to this Agreement or the Software must be filed within one (1) year after such claim or cause of action arose
* or be forever barred.
*
* PRODUCT LABELING. You are not authorized to use any Freescale trademarks, brand names,
* or logos.
*
* ENTIRE AGREEMENT. This Agreement constitutes the entire agreement between you and
* Freescale regarding the subject matter of this Agreement, and supersedes all prior
* communications, negotiations, understandings, agreements or representations, either written or
* oral, if any. This Agreement may only be amended in written form, executed by you and
* Freescale.
*
* SEVERABILITY. If any provision of this Agreement is held for any reason to be invalid or
* unenforceable, then the remaining provisions of this Agreement will be unimpaired and, unless a
* modification or replacement of the invalid or unenforceable provision is further held to deprive
* you or Freescale of a material benefit, in which case the Agreement will immediately terminate,
* the invalid or unenforceable provision will be replaced with a provision that is valid and
* enforceable and that comes closest to the intention underlying the invalid or unenforceable
* provision.
*
* NO WAIVER. The waiver by Freescale of any breach of any provision of this Agreement will
* not operate or be construed as a waiver of any other or a subsequent breach of the same or a
* different provision.
*
**********************************************************************/
/*!
*
* @file fnet_mpc5744p_flash.c
*
* @author Pat, thanks to :
*                   - Sources of Standard Software Driver v1.1.0 (SSD) for C55 Flash module from NXP.
*                   - Andrey's FNET_MK_FLASH sources.
*
* @brief MPC5744P Flash driver.
*
* @Requirements :
*              1. SSD: Destination must be aligned on double word (64-bit) boundary.
*              2. SSD: Size and source must be aligned on word (32-bit) boundary.
*              3. SDD: Create data and code sections in linker file to place SSD functions
*                 in RAM in order to erase block in same partition than the Bootloader.
*                   example:
*                          SECTIONS
*                          {
*                            .ROM.SSDdata     CROM(.SSDdata) : {} > flash_memory
*                            .ROM.SSDcode     CROM(.SSDcode) : {} > flash_memory
*                            .SSDdata                        : {} > sram_memory
*                            .SSDcode                        : {} > sram_memory
*                          }
*
***************************************************************************/
#include "fnet.h"

#if FNET_MPC && FNET_CFG_CPU_FLASH

#if (FNET_CFG_CPU_FLASH_PROGRAM_SIZE != 8u) && (FNET_CFG_CPU_FLASH_PROGRAM_SIZE != 16u) && (FNET_CFG_CPU_FLASH_PROGRAM_SIZE != 32u)
    #error "MPC Flash driver required double word (64-bit) alignement and FNET_CFG_CPU_FLASH_PROGRAM_SIZE = 8 * 2^n (n must be an integer)"
#endif

/************************************************************************
*     Definitions.
*************************************************************************/

#if FNET_CFG_COMP_GHS
    #define IN_RAM_CODE   __attribute__((section(".SSDcode")))
    #define IN_RAM_DATA   __attribute__((section(".SSDdata")))
#endif


/*************************************************************************/
/* Macros for Accessing the Registers                                    */
/*************************************************************************/
#define WRITE32(address, value)              (*(fnet_vuint32_t*)(address) = (value))
#define READ32(address)                      ((fnet_uint32_t)(*(fnet_vuint32_t*)(address)))
#define SET32(address, value)                (*(fnet_vuint32_t*)(address) |= (value))
#define CLEAR32(address, value)              (*(fnet_vuint32_t*)(address) &= ~(value))

#define READ64(address)                      ((long long)(*(volatile long long*)(address)))
#define READ8(address)                       ((fnet_uint8_t)(*(fnet_vuint8_t*)(address)))

#define C55_REG_BIT_SET(address, mask)       SET32(address, value)
#define C55_REG_BIT_CLEAR(address, mask)     CLEAR32(address, value)
#define C55_REG_BIT_TEST(address, mask)      (*(fnet_vuint32_t*)(address) & (mask))
#define C55_REG_WRITE(address, value)        WRITE32(address, value)
#define C55_REG_READ(address)                READ32(address)
#define C55_GET_BIT(value, position)         (fnet_uint8_t)(((value) >> (position)) & 0x01)

/*************************************************************************/
/*                  Offsets of C55 Control Registers                     */
/*************************************************************************/
#define C55_MCR             0x0000       /* Module Configuration Register */
#define C55_MCRA            0x0004       /* Alternate Module Configuration Register */
#define C55_MCRE            0x0008       /* Extended Module Configuration Register */
#define C55_LOCK0           0x0010       /* Lock 0 Register */
#define C55_LOCK1           0x0014       /* Lock 1 Register */
#define C55_LOCK2           0x0018       /* Lock 2 Register */
#define C55_LOCK3           0x001C       /* Lock 3 Register */
#define C55_LOCK0A          0x0028       /* Alternate Lock 0 Register */
#define C55_LOCK1A          0x002C       /* Alternate Lock 1 Register */
#define C55_SEL0            0x0038       /* Select 0 Register */
#define C55_SEL1            0x003C       /* Select 1 Register */
#define C55_SEL2            0x0040       /* Select 2 Register */
#define C55_SEL3            0x0044       /* Select 3 Register */
#define C55_ADR             0x0050       /* Address Register */
#define C55_UT0             0x0054       /* User Test 0 register */
#define C55_UM0             0x0058       /* User MISR 0 Register */
#define C55_UM1             0x005C       /* User MISR 1 Register */
#define C55_UM2             0x0060       /* User MISR 2 Register */
#define C55_UM3             0x0064       /* User MISR 3 Register */
#define C55_UM4             0x0068       /* User MISR 4 Register */
#define C55_UM5             0x006C       /* User MISR 5 Register */
#define C55_UM6             0x0070       /* User MISR 6 Register */
#define C55_UM7             0x0074       /* User MISR 7 Register */
#define C55_UM8             0x0078       /* User MISR 8 Register */
#define C55_UM9             0x007C       /* User MISR 9 Register */
#define C55_OPP0            0x0080       /* Over-Program Protection 0 Register */
#define C55_OPP1            0x0084       /* Over-Program Protection 1 Register */
#define C55_OPP2            0x0088       /* Over-Program Protection 2 Register */
#define C55_OPP3            0x008C       /* Over-Program Protection 3 Register */

/*************************************************************************/
/*              C55 Module Control Registers Field Definitions           */
/*************************************************************************/
/* Module Configuration Register */
#define C55_MCR_RVE         0x80000000   /* Read Voltage Error */
#define C55_MCR_RRE         0x40000000   /* Read Reference Error */
#define C55_MCR_AEE         0x20000000   /* Address Encode Error */
#define C55_MCR_EEE         0x10000000   /* ECC after ECC Error */
#define C55_MCR_EER         0x00008000   /* ECC Event Error */
#define C55_MCR_RWE         0x00004000   /* Read While Write Event Error */
#define C55_MCR_SBC         0x00002000   /* Single Bit Correction Error */
#define C55_MCR_PEAS        0x00000800   /* Program/Erase Access Space */
#define C55_MCR_DONE        0x00000400   /* State Machine Status */
#define C55_MCR_PEG         0x00000200   /* Program/Erase Good */
#define C55_MCR_PECIE       0x00000100   /* Program/Erase Complete Interrupt Enable */
#define C55_MCR_FERS        0x00000080   /* Factory Erase */
#define C55_MCR_PGM         0x00000010   /* Program */
#define C55_MCR_PSUS        0x00000008   /* Program Suspend */
#define C55_MCR_ERS         0x00000004   /* Erase */
#define C55_MCR_ESUS        0x00000002   /* Erase Suspend */
#define C55_MCR_EHV         0x00000001   /* Enable High Voltage */

/* User Test 0 Register */
#define C55_UT0_UTE         0x80000000
#define C55_UT0_SBCE        0x40000000
#define C55_UT0_NAIBP       0x00000200
#define C55_UT0_AIBPE       0x00000100
#define C55_UT0_AISUS       0x00000040
#define C55_UT0_MRE         0x00000020
#define C55_UT0_MRV         0x00000010
#define C55_UT0_AIS         0x00000004
#define C55_UT0_AIE         0x00000002
#define C55_UT0_AID         0x00000001

/*************************************************************************/
/*                   Return Codes for SSD functions                      */
/*************************************************************************/

#define C55_OK                      0x00000000
#define C55_ERROR_ALIGNMENT         0x00000001   /* Alignment error */
#define C55_ERROR_ENABLE            0x00000002   /* it's impossible to enable an operation */
#define C55_ERROR_BUSY              0x00000004   /* New program/erase cannot be preformed while previous high voltage operation in progress */
#define C55_ERROR_PGOOD             0x00000008   /* The program operation is unsuccessful */
#define C55_ERROR_EGOOD             0x00000010   /* The erase operation is unsuccessful */
#define C55_ERROR_NOT_BLANK         0x00000020   /* There is non-blank location in the checked flash memory */
#define C55_ERROR_VERIFY            0x00000040   /* There is a mismatch between the source data and content in the checked flash memory */
#define C55_ERROR_BLOCK_INDICATOR   0x00000080   /* Invalid block indicator */
#define C55_ERROR_ALTERNATE         0x00000100   /* The operation is unsupported via alternate interface */
#define C55_ERROR_FACTORY_OP        0x00000200   /* Factory erase/program cannot be performed */
#define C55_ERROR_MISMATCH          0x00000400   /* The MISR generated by the AIC hardware doesnot match the MISR passed by the user */
#define C55_ERROR_NO_BLOCK          0x00000800   /* No blocks have been enabled for Array Integrity check */
#define C55_ERROR_ADDR_SEQ          0x00001000   /* Invalid address sequence */
#define C55_ERROR_MARGIN_LEVEL      0x00002000   /* Invalid margin level */
#define C55_ERROR_ERASE_OPTION      0x00004000   /* Invalid erase option */
#define C55_ERROR_MODE_OP           0x00008000   /* Invalid mode op */

#define C55_DONE                    0x00010000   /* Done status */
#define C55_INPROGRESS              0x00020000   /* InProgress status */

/*************************************************************************/
/*            Predefined values for flags, options variables             */
/*************************************************************************/

/* Checking mode used in FlashCheckStatus */
#define C55_MODE_OP_PROGRAM         0x00
#define C55_MODE_OP_ERASE           0x01
#define C55_MODE_OP_PROGRAM_VERIFY  0x02
#define C55_MODE_OP_BLANK_CHECK     0x03
#define C55_MODE_OP_CHECK_SUM       0x04
#define C55_MODE_OP_USER_TEST_CHECK 0x05

/* Indicators for getting/setting block lock state */
#define C55_BLOCK_LOW               0x00   /* Block lock protection of low address space */
#define C55_BLOCK_MID               0x01   /* Block lock protection of mid address space */
#define C55_BLOCK_HIGH              0x02   /* Block lock protection of high address space */
#define C55_BLOCK_256K_FIRST        0x03   /* Block lock protection of first 32 blocks in 256K address space */
#define C55_BLOCK_256K_SECOND       0x04   /* Block lock protection of next 16 blocks in 256K address space */
#define C55_BLOCK_UTEST             0x05   /* Block lock protection of UTest address space */

/* Declarations for flash suspend operation */
#define C55_SUS_NOTHING             0x10   /* No program/erase operation */
#define C55_PGM_WRITE               0x11   /* A program sequence in interlock write stage. */
#define C55_ERS_WRITE               0x12   /* An erase sequence in interlock write stage. */
#define C55_ERS_SUS_PGM_WRITE       0x13   /* An erase-suspend program sequence in interlock write stage. */
#define C55_PGM_SUS                 0x14   /* The program operation is in suspend state */
#define C55_ERS_SUS                 0x15   /* The erase operation is in suspend state */
#define C55_ERS_SUS_PGM_SUS         0x16   /* The erase-suspended program operation is in suspend state */
#define C55_USER_TEST_SUS           0x17   /* The UTest check operation is in suspend state */

/* Declarations for flash resume operation */
#define C55_RES_NOTHING             0x20   /* No suspended program/erase operation */
#define C55_RES_PGM                 0x21   /* The program operation is resumed */
#define C55_RES_ERS                 0x22   /* The erase operation is resumed */
#define C55_RES_ERS_PGM             0x23   /* The erase-suspended program operation is resumed */
#define C55_RES_USER_TEST           0x24   /* The UTest check operation is resumed */

#define C55_USER_TEST_BREAK_SBC     0x30   /* The UTest check operation is broken by Single bit correction */
#define C55_USER_TEST_BREAK_DBD     0x31   /* The UTest check operation is broken by Double bit detection */

/* Declarations of margin levels */
#define C55_MARGIN_LEVEL_PROGRAM    0x00
#define C55_MARGIN_LEVEL_ERASE      0x01

/* Declarations of address sequences */
#define C55_ADDR_SEQ_PROPRIETARY    0x00
#define C55_ADDR_SEQ_LINEAR         0x01

/* Declarations of break options */
#define C55_BREAK_NONE              0x00   /* No break at all */
#define C55_BREAK_ON_DBD            0x01   /* Break on Double bit detection */
#define C55_BREAK_ON_DBD_SBC        0x02   /* Break on bot Double bit detection and Single bit correction */

/* Declarations of erase options */
#define C55_ERASE_MAIN              0x00   /* Perform normal erase on main array */
#define C55_ERASE_MAIN_FERS         0x01   /* Perform factory erase on main array */
#define C55_ERASE_UTEST             0x02   /* Perform erase on UTest array */
#define C55_ERASE_UTEST_FERS        0x03   /* Perform factory erase on UTest array */

/*************************************************************************/
/*                   Other Macros for SSD functions                      */
/*************************************************************************/

#define C55_WORD_SIZE               4      /* size of a word in byte */
#define C55_DWORD_SIZE              8      /* size of a double word in byte */

#define C55_USER_TEST_ENABLE_PASSWORD      0xF9F99999
#define C55_FACTORY_ERASE_DIARY_LOCATION   0x0020


/* Set/Clear C55-MCR bits without affecting MCR-EER, MCR-RWE, and MCR-SBC */
#define C55_MCR_BIT_SET(MCRAddress, mask)      \
    C55_REG_WRITE(MCRAddress, ((mask | C55_REG_READ(MCRAddress)) & (~(C55_MCR_EER | C55_MCR_RWE | C55_MCR_SBC))))

#define C55_MCR_BIT_CLEAR(MCRAddress, mask)    \
    C55_REG_WRITE(MCRAddress, (((~mask) & C55_REG_READ(MCRAddress)) & (~(C55_MCR_EER | C55_MCR_RWE | C55_MCR_SBC))))

#define C55_MCR_EER_CLEAR(MCRAddress)    \
    C55_REG_WRITE(MCRAddress, (C55_REG_READ(MCRAddress) | C55_MCR_RWE | C55_MCR_SBC))

#define C55_MCR_RWE_CLEAR(MCRAddress)    \
    C55_REG_WRITE(MCRAddress, (C55_REG_READ(MCRAddress) | C55_MCR_EER | C55_MCR_SBC))

#define C55_MCR_SBC_CLEAR(MCRAddress)    \
    C55_REG_WRITE(MCRAddress, (C55_REG_READ(MCRAddress) | C55_MCR_EER | C55_MCR_RWE))

#define C55_MCR_EER_RWE_SBC_CLEAR(MCRAddress)    \
    C55_REG_WRITE(MCRAddress, (C55_REG_READ(MCRAddress) | C55_MCR_EER | C55_MCR_RWE | C55_MCR_SBC))

#define C55_MAIN_ARRAY_SIZE(pSSDConfig)    \
    ((((pSSDConfig->lowBlockInfo.n16KBlockNum + pSSDConfig->midBlockInfo.n16KBlockNum + \
        pSSDConfig->highBlockInfo.n16KBlockNum) << 4 /*16*/) + \
      ((pSSDConfig->lowBlockInfo.n32KBlockNum + pSSDConfig->midBlockInfo.n32KBlockNum + \
        pSSDConfig->highBlockInfo.n32KBlockNum) << 5 /*32*/) + \
      ((pSSDConfig->lowBlockInfo.n64KBlockNum + pSSDConfig->midBlockInfo.n64KBlockNum + \
        pSSDConfig->highBlockInfo.n64KBlockNum) << 6 /*64*/) + \
      (pSSDConfig->n256KBlockNum << 8)) << 10 /*1024*/)

#define C55_LOW_BLOCK_NUM(pSSDConfig)     \
    (pSSDConfig->lowBlockInfo.n16KBlockNum + pSSDConfig->lowBlockInfo.n32KBlockNum +  \
     pSSDConfig->lowBlockInfo.n64KBlockNum)

#define C55_MID_BLOCK_NUM(pSSDConfig)     \
    (pSSDConfig->midBlockInfo.n16KBlockNum + pSSDConfig->midBlockInfo.n32KBlockNum +  \
     pSSDConfig->midBlockInfo.n64KBlockNum)

#define C55_HIGH_BLOCK_NUM(pSSDConfig)     \
    (pSSDConfig->highBlockInfo.n16KBlockNum + pSSDConfig->highBlockInfo.n32KBlockNum +  \
     pSSDConfig->highBlockInfo.n64KBlockNum)

/************************************************************************
*    flash dependent configuration
************************************************************************/
#define C55_REG_BASE                    0xFFFE0000
#define MAIN_ARRAY_BASE                 0x00800000
#define UTEST_ARRAY_BASE                0x00400000
#define UTEST_ARRAY_SIZE                0x00004000

#define C55_PROGRAMMABLE_SIZE           0x80


/* Flash controller definition */
#define FLASH_FMC                       0xFC030000
/* Offsets of Platform Flash Configuration Register 1 & 3 */
#define FLASH_PFCR1                     0             /* Offset address of PFCR1 */
#define FLASH_PFCR3                     8             /* Offset address of PFCR3 */
#define FLASH_FMC_BFEN                  0x00000001    /* Enable PBFEN bit */

/* block number for each address space */
#define NUM_LOW_BLOCK                   4
#define NUM_MID_BLOCK                   2
#define NUM_HIGH_BLOCK                  6
#define NUM_256K_BLOCK_FIRST            8
#define NUM_256K_BLOCK_SECOND           0
#define NUM_BLOCK_ALL                   (NUM_LOW_BLOCK + NUM_MID_BLOCK + NUM_HIGH_BLOCK + NUM_256K_BLOCK_FIRST + NUM_256K_BLOCK_SECOND)

#define OPERATION_PASS                  0x00000000
#define OPERATION_FAIL                  0xFFFFFFFF

/**************************************************************************/ /*!
 * @brief Possible flash error codes
 ******************************************************************************/
typedef enum
{
    FNET_FLASH_ERR_NOTERASED      = (-2),  /**< @brief memory not erased.
                                           */
    FNET_FLASH_ERR_WRITEFAILED    = (-3),  /**< @brief 
                                           */


    FNET_FLASH_ERR_DST_UNALIGNED  = (-5),  /**< @brief 
                                           */
    FNET_FLASH_ERR_VERIFY         = (-6),  /**< @brief 
                                           */
    FNET_FLASH_ERR_NOTBLANK       = (-7),  /**< @brief 
                                           */
    FNET_FLASH_ERR_LOCK_OP        = (-8),  /**< @brief error on 
                                           */
    FNET_FLASH_ERR_INIT           = (-9),  /**< @brief error on 
                                           */
    FNET_FLASH_ERR_ERASE          = (-10),  /**< @brief error on 
                                           */
} fnet_flash_error_t;

/********************************************************************************************************************************************/
/* TYPEDEFS                                                                                                                                 */
/********************************************************************************************************************************************/
#ifndef fnet_boolean_t
    typedef fnet_uint8_t fnet_boolean_t;
#endif

/* Block select structure for 256K address space */
typedef struct _c55_n256k_block_sel
{
    fnet_uint32_t     first256KBlockSelect;
    fnet_uint32_t     second256KBlockSelect;
} N256K_BLOCK_SEL, *PN256K_BLOCK_SEL;

/* MISR structure */
typedef struct _c55_misr
{
    fnet_uint32_t     W0;
    fnet_uint32_t     W1;
    fnet_uint32_t     W2;
    fnet_uint32_t     W3;
    fnet_uint32_t     W4;
    fnet_uint32_t     W5;
    fnet_uint32_t     W6;
    fnet_uint32_t     W7;
    fnet_uint32_t     W8;
    fnet_uint32_t     W9;
} MISR, *PMISR;

/* Structure data for the context values */
typedef struct _c55_context_data
{
    fnet_uint32_t     dest;
    fnet_uint32_t     size;
    fnet_uint32_t     source;
    fnet_uint32_t     *pFailedAddress;
    fnet_uint32_t     *pFailedData;
    fnet_uint32_t     *pFailedSource;
    fnet_uint32_t     *pSum;
    PMISR             pMisr;
    void              *pReqCompletionFn;
} CONTEXT_DATA, *PCONTEXT_DATA;

/*************************************************************************/
/*                   SSD Configuration Structure                         */
/*************************************************************************/
/* Block information for an address space */
typedef struct _c55_block_info
{
    fnet_uint32_t     n16KBlockNum;        /* Number of 16K blocks */
    fnet_uint32_t     n32KBlockNum;        /* Number of 32K blocks */
    fnet_uint32_t     n64KBlockNum;        /* Number of 64K blocks */
} BLOCK_INFO, *PBLOCK_INFO;

/* SSD Configuration Structure */
typedef struct _c55_ssd_config
{
    fnet_uint32_t     c55RegBase;          /* C55 control register base */
    fnet_uint32_t     mainArrayBase;       /* base of main array */
    BLOCK_INFO        lowBlockInfo;        /* blocks info of low address space */
    BLOCK_INFO        midBlockInfo;        /* blocks info of mid address space */
    BLOCK_INFO        highBlockInfo;       /* blocks info of high address space */
    fnet_uint32_t     n256KBlockNum;       /* number of blocks in 256K address space */
    fnet_uint32_t     uTestArrayBase;      /* base of UTEST array */
    fnet_boolean_t    mainInterfaceFlag;   /* interface flag indicate main or alternate interface */
    fnet_uint32_t     programmableSize;    /* programmable size */
    fnet_boolean_t    BDMEnable;           /* debug mode selection */
} SSD_CONFIG, *PSSD_CONFIG;

typedef struct _c55_all_block_sel
{
    fnet_uint32_t     lowBlockSelect;
    fnet_uint32_t     midBlockSelect;
    fnet_uint32_t     highBlockSelect;
    N256K_BLOCK_SEL   n256KBlockSelect;
} ALL_BLOCK_SEL, *PALL_BLOCK_SEL;

/*************************************************************************/
/*                      SSD Function Pointer Types                       */
/*************************************************************************/
typedef fnet_uint32_t (*PFLASHINIT)(
    PSSD_CONFIG       pSSDConfig
);
typedef fnet_uint32_t (*PFLASHERASE)(
    PSSD_CONFIG       pSSDConfig,
    fnet_uint8_t      eraseOption,
    fnet_uint32_t     lowBlockSelect,
    fnet_uint32_t     midBlockSelect,
    fnet_uint32_t     highBlockSelect,
    N256K_BLOCK_SEL   n256KBlockSelect
);
typedef fnet_uint32_t (*PFLASHPROGRAM)(
    PSSD_CONFIG       pSSDConfig,
    fnet_boolean_t    factoryPgmFlag,
    fnet_uint32_t     dest,
    fnet_uint32_t     size,
    fnet_uint32_t     source,
    PCONTEXT_DATA     pCtxData
);
typedef fnet_uint32_t (*PFLASHCHECKSTATUS)(
    PSSD_CONFIG       pSSDConfig,
    fnet_uint8_t      modeOp,
    fnet_uint32_t     *opResult,
    PCONTEXT_DATA     pCtxData
);
typedef fnet_uint32_t (*PGETLOCK)(
    PSSD_CONFIG       pSSDConfig,
    fnet_uint8_t      blkLockIndicator,
    fnet_uint32_t     *blkLockState
);
typedef fnet_uint32_t (*PSETLOCK)(
    PSSD_CONFIG       pSSDConfig,
    fnet_uint8_t      blkLockIndicator,
    fnet_uint32_t     blkLockState
);
typedef fnet_uint32_t (*PPROGRAMVERIFY)(
    PSSD_CONFIG       pSSDConfig,
    fnet_uint32_t     dest,
    fnet_uint32_t     size,
    fnet_uint32_t     source,
    fnet_uint32_t     *pFailedAddress,
    fnet_uint32_t     *pFailedData,
    fnet_uint32_t     *pFailedSource,
    PCONTEXT_DATA     pCtxData
);
typedef fnet_uint32_t (*PBLANKCHECK)(
    PSSD_CONFIG       pSSDConfig,
    fnet_uint32_t     dest,
    fnet_uint32_t     size,
    fnet_uint32_t     *pFailedAddress,
    fnet_uint32_t     *pFailedData,
    PCONTEXT_DATA     pCtxData
);

/*************************************************************************/
/*               Application Function Pointer Types                      */
/*************************************************************************/
typedef fnet_uint32_t (*PAPPFLASHERASE)(
    PSSD_CONFIG       pSSDConfig,
    fnet_uint8_t      eraseOption,
    fnet_uint32_t     lowBlockSelect,
    fnet_uint32_t     midBlockSelect,
    fnet_uint32_t     highBlockSelect,
    N256K_BLOCK_SEL   n256KBlockSelect
);
typedef fnet_uint32_t (*PAPPFLASHPROG)(
    PSSD_CONFIG       pSSDConfig,
    fnet_uint8_t      pgmOption,
    fnet_uint32_t     dest,
    fnet_uint32_t     size,
    fnet_uint32_t     source,
    PCONTEXT_DATA     pCtxData
);
typedef  fnet_uint32_t(*PAPPFLASHPROGVERIFY)(
    PSSD_CONFIG       pSSDConfig,
    fnet_uint32_t     dest,
    fnet_uint32_t     size,
    fnet_uint32_t     source,
    fnet_uint32_t     *pFailedAddress,
    fnet_uint32_t     *pFailedData,
    fnet_uint32_t     *pFailedSource,
    PCONTEXT_DATA     pCtxData
);
typedef  fnet_uint32_t(*PAPPBLANKCHECK)(
    PSSD_CONFIG       pSSDConfig,
    fnet_uint32_t     dest,
    fnet_uint32_t     size,
    fnet_uint32_t     *pFailedAddress,
    fnet_uint32_t     *pFailedData,
    PCONTEXT_DATA     pCtxData
);

/********************************************************************************************************************************************/
/* PRIVATE VARIABLES                                                                                                                        */
/********************************************************************************************************************************************/
/*const unsigned long FlashInit_C[] = */
static IN_RAM_DATA fnet_uint32_t FlashInit_C[] =
{
    0x182106C0, 0x00801AC1, 0x09185401, 0x0044480F, 0x30030034,
    0x2A00E655, 0xC0E373BE, 0xE0001BBD, 0xD1E0C00E, 0x44D0D00E,
    0xC0E3CE03, 0xC2EE6600, 0xE62AC0D3, 0x77D70F3D, 0x77CCF73D,
    0x77CBDF3D, 0x77D8C73D, 0x77D9AF3D, 0x52DD0000, 0x77DA973D,
    0x77DB7F3D, 0x77DC673D, 0x77C04F3D, 0x1AD6D203, 0x77DE36BD,
    0x56DD0000, 0x56E30008, 0x5583000C, 0x55630010, 0xD583D693,
    0xD7A3D8B3, 0xD9C3DA03, 0xDBE3E81F, 0x77C00F3D, 0x77DCF77D,
    0xD203D3C3, 0x77C0E73D, 0xD40377DC, 0xCF3DD5C3, 0x77C0B77D,
    0xD60377DC, 0xA73DD7C3, 0x77C08F3D, 0xD80377DC, 0x777DD9C3,
    0x77C0673D, 0x77DE4EBD, 0xDA03DBE3, 0x3003003C, 0x2A00E607,
    0x01F31800, 0xD0000000, 0x1800D000, 0x01F35001, 0x00441AC1,
    0x08180090, 0x18218040, 0x00044D50, 0x43353546, 0x49313130
};
/* Total Data Size = 240 bytes */

/*const unsigned long FlashErase_C[] = */
static IN_RAM_DATA fnet_uint32_t FlashErase_C[] =
{
    0x182106D0, 0x00801B01, 0x0910DD01, 0x480FC0E3, 0x7480063F,
    0x67E0C0CE, 0xE603CCD3, 0xE802C1D3, 0x67BCE206, 0x67DCE607,
    0x1B80C803, 0xE604484F, 0x78000198, 0xCE036600, 0xE611C00E,
    0x73837F7F, 0x73637FFB, 0x46C0D00E, 0xC00E46B0, 0xD00EC00E,
    0x1800D203, 0x740004DF, 0xD00E749C, 0xDFBF2A0C, 0xE605C0B3,
    0xC30B44C0, 0xD30BC00E, 0x65D07400, 0x04DFD00E, 0xC00E67D0,
    0xE204482F, 0x78000114, 0x7480063F, 0x67E0E264, 0xC2932C00,
    0x5163000C, 0xC4B3C5A3, 0xC6837F39, 0x5A1404B9, 0x5163001C,
    0x1999B020, 0xC8B3C993, 0x048ACA83, 0x7F5A5A14, 0x5163002C,
    0x1B5AB020, 0x049B7C1A, 0xD4307C19, 0x643004B8, 0x7F3B2838,
    0x188BA820, 0x1B38B020, 0x7C0CCC30, 0x7F403038, 0x7D8C3838,
    0xE5105163, 0x002C480A, 0x53280000, 0x2C085748, 0x00041B4B,
    0xB02040A8, 0x46895728, 0x0000E80E, 0x2C085328, 0x00045708,
    0x0000CBA3, 0x25FA1B5A, 0xB02040A8, 0x46895728, 0x00045328,
    0x00007D9A, 0xDB785308, 0x0004440A, 0x449A7F5A, 0xC379E633,
    0xC0A36D0B, 0x44B0DE0A, 0xC0B3559B, 0x003C5008, 0x0000C0B3,
    0x541B0040, 0x50080004, 0xC0B3541B, 0x00447480, 0x063F67F0,
    0xE60BC00E, 0x65807400, 0x04DFD00E, 0xC00E6780, 0xE203636F,
    0xE8142C00, 0xD00DC00E, 0x65F07400, 0x04DFD00E, 0xCE036600,
    0xE626C00E, 0x66E0E603, 0x624FE805, 0xC00E66F0, 0xE61E623F,
    0xC00E7363, 0x7FFE7343, 0x7F7F73A3, 0x7FFB46B0, 0xD00EC00E,
    0x46A0D00E, 0xC00E46D0, 0xD00ECE03, 0x6600E60B, 0xC00E1800,
    0xD2037400, 0x04DFD00E, 0xC0E3C30E, 0x45C0D30E, 0x3003003C,
    0x2A00E607, 0x01F31800, 0xD0000000, 0x1800D000, 0x01F3CD01,
    0x1B010810, 0x00901821, 0x80300004, 0x4D504335, 0x35464531,
    0x313000
};
/* Total Data Size = 504 bytes */


/*const unsigned long FlashProgram_C[] = */
static IN_RAM_DATA fnet_uint32_t FlashProgram_C[] =
{
    0x182106C0, 0x00801A81, 0x09105401, 0x0044014E, 0x015D016C,
    0x017B030A, 0x30030034, 0x480FCE93, 0x2A007739, 0x013FE20A,
    0x7480063F, 0x1800C881, 0x2A00E604, 0x637F7800, 0x02047480,
    0x063F7160, 0x00084A08, 0x67807C0B, 0xC09E7F05, 0x03960580,
    0x7C002851, 0xE2067CE0, 0x33781800, 0xC803E604, 0x481F7800,
    0x01D83003, 0x00342A00, 0xE603C083, 0xE803C083, 0x20385178,
    0x00001960, 0xC810E605, 0x1960C809, 0x2A00E205, 0x7560077D,
    0x2A40E204, 0x484F7800, 0x01A82A06, 0xE2064800, 0x54080004,
    0x7800019A, 0xCE036600, 0xE61DC008, 0x1800CA03, 0xE619C008,
    0x72E37F7F, 0x72C37FBF, 0x71837FEF, 0x7C00B838, 0xD008C008,
    0x7C00B038, 0xD008C008, 0x7C006038, 0xD008C008, 0x1800D203,
    0x740004DF, 0xD0087496, 0xDFBF1816, 0xA800E609, 0x51830000,
    0x500C000C, 0x7C00B378, 0x540C000C, 0xC00865B0, 0x740004DF,
    0xD008C008, 0x67B0E204, 0x482F7800, 0x00E87480, 0x063F6780,
    0xE606C008, 0x65907400, 0x04DFD008, 0x7480063F, 0x67F0E60B,
    0xC0086580, 0x740004DF, 0xD008C008, 0x6780E203, 0x636FE85C,
    0x74D7F0BF, 0x772CF0BF, 0x0D967480, 0x063F7EEC, 0xB85E6780,
    0x7EAC389E, 0x63307D6B, 0x289E7D99, 0x009E4809, 0x7C19B840,
    0xE01C529B, 0x000077C0, 0x063F243C, 0x6780569D, 0x0000203B,
    0x203DE608, 0x1BA0C81F, 0xE2051FBD, 0x03E01F7B, 0x03E07C1D,
    0x63967C00, 0x61D67C00, 0xE851E603, 0x2009E8E3, 0xC00877D9,
    0x063F65F0, 0x740004DF, 0xD008D0DA, 0xD1CAD2BA, 0xD99ACE03,
    0x6600E60B, 0xC00866E0, 0xE603624F, 0xE819C008, 0x66F0E603,
    0x623FE814, 0x00CE678E, 0xE633196B, 0x80201AB5, 0x8020557A,
    0x00001960, 0xC90456BA, 0x0008E628, 0xC0EA1BDE, 0x810CD0EA,
    0xE823C008, 0x73837FFE, 0x73637F7F, 0x73A37FBF, 0x73C37FEF,
    0x46C0D008, 0xC00846B0, 0xD008C008, 0x46D0D008, 0xC00846E0,
    0xD008CE03, 0x6600E60C, 0xC0081800, 0xD2037400, 0x04DFD008,
    0xC0E3C30E, 0x7C00B378, 0xD30E3003, 0x003C2A00, 0xE60701F3,
    0x1800D000, 0x00001800, 0xD00001F3, 0x50010044, 0x1A810810,
    0x00901821, 0x80400004, 0x4D504335, 0x35465031, 0x313000
};
/* Total Data Size = 620 bytes */

/*const unsigned long GetLock_C[] = */
static IN_RAM_DATA fnet_uint32_t GetLock_C[] =
{
    0x182106D0, 0x00801B41, 0x0918DD01, 0x480F3003, 0x00342A00,
    0xE6237480, 0x063F6790, 0xE60BC063, 0x500600B0, 0x53C600B4,
    0x538600B8, 0x53A600BC, 0xE8187480, 0x063F6780, 0xE60BC063,
    0x500600A0, 0x53C600A4, 0x538600A8, 0x53A600AC, 0xE80AC063,
    0xC406C5E6, 0xC6C6C7D6, 0xE804C063, 0xCA06CBE6, 0x748606BF,
    0x2A36E509, 0xE6312A06, 0xE60B2A16, 0xE6162A26, 0xE61FE857,
    0x2A46E63C, 0x2A56E64F, 0xE852C2D3, 0x2C0BC3C3, 0x490AC4E3,
    0x04CD04DE, 0x1BDEB020, 0x40EB777E, 0x801FE847, 0xC5D32C0E,
    0xC6C3480A, 0xC7B304CD, 0x04BD1BBD, 0xB02040DE, 0xE83CC8D3,
    0x01E0C9C3, 0x2C0ECAB3, 0x480A04CD, 0x04BD1BBD, 0xB02040DE,
    0xE8303003, 0x00342A00, 0xE608CBE3, 0x01C0480A, 0x23FEE405,
    0x2C0EE825, 0x637FE826, 0xCBE32C0D, 0x1BDEB020, 0x7FBEF430,
    0xE81C3003, 0x00342A00, 0xE60ECBE3, 0x01D0480A, 0x23FEE10B,
    0xCBE32C0D, 0x25FE1BDE, 0xB0207FBE, 0xF430E80B, 0x637FE80C,
    0x480EE807, 0x49FA73D0, 0xE000E803, 0x638FE804, 0x46E040A0,
    0xD0053003, 0x003C2A00, 0xE60701F3, 0x1800D000, 0x00001800,
    0xD00001F3, 0xCD011B41, 0x08180090, 0x18218030, 0x00044D50,
    0x43353547, 0x4C313130
};
/* Total Data Size = 348 bytes */

/*const unsigned long SetLock_C[] = */
static IN_RAM_DATA fnet_uint32_t SetLock_C[] =
{
    0x182106D0, 0x00801B41, 0x0918DD01, 0x490E480F, 0x30C30034,
    0x49402A06, 0x7D66F09E, 0x7C06009E, 0x4A8E4AC6, 0x7FDE589E,
    0x7C06009E, 0x7486063F, 0x2A36E509, 0xE6312A06, 0xE60B2A16,
    0xE6162A26, 0xE61FE856, 0x2A46E63C, 0x2A56E64E, 0xE851C2D3,
    0x2C0BC3C3, 0x490AC403, 0x04CD04D0, 0x1800B020, 0x400B7760,
    0x801FE846, 0xC5D32C00, 0xC6C3480A, 0xC7B304CD, 0x04BD1BBD,
    0xB02040D0, 0xE83BC8D3, 0x010EC9C3, 0x2C00CAB3, 0x480A04CD,
    0x04BD1BBD, 0xB02040D0, 0xE82F3003, 0x00342A00, 0xE608CB03,
    0x498E480A, 0x23F0E405, 0x2C00E824, 0x637FE834, 0xCB032C0C,
    0x1800B020, 0x7F800430, 0xE81B3003, 0x00342A00, 0xE60DCB03,
    0x49CE480A, 0x23F0E10A, 0xCBD32C00, 0x25FD1BBD, 0xB02040D0,
    0xE80B637F, 0xE81B4800, 0xE80749FA, 0x7010E000, 0xE803638F,
    0xE813C0D3, 0x7CBCD030, 0x460C62DB, 0x7D7EE86E, 0x7D600078,
    0xD00EC00E, 0x44C0D00E, 0xC00E46C0, 0x0D0C7FFF, 0xD89E3003,
    0x003C2A00, 0xE60701F3, 0x1800D000, 0x00001800, 0xD00001F3,
    0xCD011B41, 0x08180090, 0x18218030, 0x00044D50, 0x43353553,
    0x4C313130
};
/* Total Data Size = 324 bytes */

/*const unsigned long FlashCheckStatus_C[] = */
static IN_RAM_DATA fnet_uint32_t FlashCheckStatus_C[] =
{
    0x182106D0, 0x00801806, 0xACFF1B21, 0x0914DD01, 0x013F015E,
    0x62FC016D, 0x4800D005, 0xE602C8B6, 0x7486063F, 0x2256E518,
    0xE9016C26, 0x00871CC6, 0x000E0476, 0x00B60006, 0x78000024,
    0x78000100, 0x78000176, 0x780001A6, 0x780001CE, 0x780001F4,
    0x780002EA, 0xE804630C, 0x78000340, 0x30030034, 0x2A00E603,
    0xC033E803, 0xC0332033, 0xC0A367BA, 0x7A120328, 0x675AE660,
    0x67CAE608, 0x775AF7FF, 0x775C083D, 0x213C7800, 0x0312676A,
    0xE63467FA, 0xE62BC003, 0x73437FFE, 0x46A0D003, 0xC10D2A00,
    0xE612C05D, 0x00BBC16D, 0x01F3C27D, 0x02D0309D, 0x002762EC,
    0x0007D03E, 0x2A037A12, 0x02DE62FC, 0x780002D8, 0xC00373A3,
    0x7F7F7363, 0x7FBF73C3, 0x7FEF46D0, 0xD003C003, 0x46B0D003,
    0xC00346E0, 0xD0037800, 0x02B6775A, 0xF7FF775C, 0x083D210C,
    0x780002A8, 0x67FAE619, 0x48807363, 0x7FFED005, 0x73437F7F,
    0x73A37FBF, 0x73C37FEF, 0xC00346B0, 0xD003C003, 0x46A0D003,
    0xC00346D0, 0xD003C003, 0x46E0D003, 0x78000274, 0x775AF7FF,
    0x775C083D, 0x210C7800, 0x026662EC, 0x78000260, 0x30030034,
    0x2A00E603, 0xC033E803, 0xC0332033, 0xC0A367DA, 0x7A120248,
    0x675AE62D, 0x67EAE604, 0x495C7800, 0x023A676A, 0xE60C67FA,
    0xE607C003, 0x73C37FFE, 0x46E0D003, 0xE80F492C, 0x78000220,
    0x490E67FA, 0xD0E5E607, 0xC00373C3, 0x7FFE46E0, 0xD003E802,
    0x62FC2B2C, 0x7A120204, 0xC00373C3, 0x7F7F73A3, 0x7FFB46E0,
    0xD003C003, 0x46D0D003, 0x780001EC, 0x62EC7800, 0x01E6C10D,
    0x2A007A12, 0x01DEC04D, 0x00BBC15D, 0x02D2C26D, 0x62ECC37D,
    0x511D0010, 0x513D0014, 0x0007D03E, 0x2A03E205, 0xC10D2A00,
    0x7A0201B8, 0x62FC7800, 0x01B2C10D, 0x2A007A12, 0x01AAC04D,
    0x00BBC15D, 0x02D0C36D, 0x62ECC47D, 0x0007D03E, 0x2A03E205,
    0xC10D2A00, 0x7A02018C, 0x62FC7800, 0x0186C10D, 0x2A007A12,
    0x017EC04D, 0x00BBC15D, 0x01D72C06, 0x62EC0007, 0xD03E2A03,
    0xE205C10D, 0x2A007A02, 0x016262FC, 0x7800015C, 0x30030034,
    0x2A00E204, 0x637C7800, 0x014EC033, 0x51630054, 0x18638054,
    0x1960C802, 0x7A1200DE, 0x1960C801, 0x7A1200D4, 0x1960C840,
    0xE603497C, 0xE865C0AF, 0xC79D501A, 0x0058C0A9, 0x0DA0E240,
    0xC0AFC79D, 0x501A005C, 0xC1A90DA0, 0xE239C0AF, 0xC79D501A,
    0x0060C2A9, 0x0DA0E232, 0xC0AFC79D, 0x501A0064, 0xC3A90DA0,
    0xE22BC0AF, 0xC79D501A, 0x0068C4A9, 0x0DA0E224, 0xC0AFC79D,
    0x501A006C, 0xC5A90DA0, 0xE21DC0AF, 0xC79D501A, 0x0070C6A9,
    0x0DA0E216, 0xC0AFC79D, 0x501A0074, 0xC7A90DA0, 0xE20FC0AF,
    0xC79D501A, 0x0078C8A9, 0x0DA0E208, 0xC0AFC79D, 0x501A007C,
    0xC9A90DA0, 0xE6046350, 0xD005E803, 0x4800D005, 0x1960C902,
    0xE60BC03F, 0x4B0AC003, 0x67207C1C, 0xD09E4B13, 0x7F83009E,
    0xE80DC003, 0x1800CCDB, 0xD003C003, 0x61E0D003, 0xC0036000,
    0xD003E802, 0x62ECC03F, 0x534300F0, 0x186380F0, 0x67BAE629,
    0x679AE626, 0x67AAE61B, 0xC00361F0, 0xD003C003, 0x61B0D003,
    0xC00367D0, 0xE2FEC10D, 0x2A00E619, 0xC05D00BB, 0xC16D01F3,
    0xC27D02D0, 0xC34D62EC, 0x0007D03E, 0x2A03E60D, 0x62FCE80B,
    0x4880D005, 0xC00361F0, 0xD003C003, 0x61B0D003, 0xE80262EC,
    0x301F003C, 0x2A00E607, 0x01C31800, 0xD0000000, 0x1800D000,
    0x01C3CD01, 0x1B210814, 0x00901821, 0x80300004, 0x4D504335,
    0x35435331, 0x313000
};
/* Total Data Size = 968 bytes */

/*const unsigned long ProgramVerify_C[] = */
static IN_RAM_DATA fnet_uint32_t ProgramVerify_C[] =
{
    0x182106C0, 0x00801AE1, 0x091C5401, 0x0044025F, 0x014E016D,
    0x017C030B, 0x7CC02378, 0x031A7C00, 0xBB780329, 0x480F1800,
    0xC803E603, 0x481FE82D, 0x2A05E205, 0x4800540A, 0x0004E827,
    0x74A0F0BF, 0x71600050, 0x1880A850, 0x48087C0B, 0x005E0D08,
    0xE013519E, 0x0000517D, 0x00007C0C, 0x5840E608, 0xD0EC4C0F,
    0xC00ED00B, 0xC00DD00A, 0xE80E203E, 0x203D2008, 0xE8ED6C20,
    0xD0E97C00, 0xB850D2D9, 0xD109D3C9, 0xD4B9D5A9, 0x3003003C,
    0x2A00E607, 0x01F31800, 0xD0000000, 0x1800D000, 0x01F35001,
    0x00441AE1, 0x081C0090, 0x18218040, 0x00044D50, 0x43353550,
    0x56313130
};
/* Total Data Size = 184 bytes */

/*const unsigned long BlankCheck_C[] =*/
static IN_RAM_DATA fnet_uint32_t BlankCheck_C[] =
{
    0x182106D0, 0x00801B01, 0x0910DD01, 0x016D017C, 0x030B480F,
    0x014E7C80, 0x2B780158, 0x1800C803, 0xE603481F, 0xE8232A05,
    0xE2054800, 0x54080004, 0xE81D74A0, 0xF0BF4DA9, 0x1880A85A,
    0x480A7C19, 0x005E0D0A, 0xE00DC09E, 0x1819ACFF, 0xE606D0ED,
    0x4A0FC00E, 0xD00CE80A, 0x203E200A, 0xE8F36C20, 0xD0EB0780,
    0xD3DBD10B, 0xD4CB3003, 0x003C2A00, 0xE60701F3, 0x1800D000,
    0x00001800, 0xD00001F3, 0xCD011B01, 0x08100090, 0x18218030,
    0x00044D50, 0x43353542, 0x43313130
};
/* Total Data Size = 152 bytes */


/* Block start addresses */
const fnet_uint32_t BLOCK_START_ADDRS[NUM_BLOCK_ALL] =
{
    /* LOW */
    0x00800000, 0x00804000, 0x00F98000, 0x00F9C000,
    /* MID */
    0x00808000, 0x00810000,
    /* HIGH */
    0x00FA0000, 0x00FB0000, 0x00FC0000, 0x00FD0000, 0x00FE0000, 0x00FF0000,
    /* 256K */
    0x01000000, 0x01040000, 0x01080000, 0x010C0000,
    0x01100000, 0x01140000, 0x01180000, 0x011C0000
};

/* Block end addresses */
const fnet_uint32_t BLOCK_END_ADDRS[NUM_BLOCK_ALL] =
{
    /* LOW */
    0x00803FFF, 0x00807FFF, 0x00F9BFFF, 0x00F9FFFF,
    /* MID */
    0x0080FFFF, 0x00817FFF,
    /* HIGH */
    0x00FAFFFF, 0x00FBFFFF, 0x00FCFFFF, 0x00FDFFFF, 0x00FEFFFF, 0x00FFFFFF,
    /* 256K */
    0x0103FFFF, 0x0107FFFF, 0x010BFFFF, 0x010FFFFF,
    0x0113FFFF, 0x0117FFFF, 0x011BFFFF, 0x011FFFFF,
};

static IN_RAM_DATA SSD_CONFIG    *pSSDConfig;
static fnet_uint8_t              previousBlockId = 255;
static ALL_BLOCK_SEL             blockSel;
static fnet_uint32_t             pflash_pfcr1;

static PFLASHINIT                pFlashInit        = (PFLASHINIT) FlashInit_C;
static PFLASHERASE               pFlashErase       = (PFLASHERASE) FlashErase_C;
static PFLASHPROGRAM             pFlashProgram     = (PFLASHPROGRAM) FlashProgram_C;
static PFLASHCHECKSTATUS         pFlashCheckStatus = (PFLASHCHECKSTATUS) FlashCheckStatus_C;
static PGETLOCK                  pGetLock          = (PGETLOCK) GetLock_C;
static PSETLOCK                  pSetLock          = (PSETLOCK) SetLock_C;
static PPROGRAMVERIFY            pProgramVerify    = (PPROGRAMVERIFY) ProgramVerify_C;
static PBLANKCHECK               pBlankCheck       = (PBLANKCHECK) BlankCheck_C;

static IN_RAM_DATA CONTEXT_DATA  bcCtxData;
static IN_RAM_DATA CONTEXT_DATA  pgmCtxData;
static IN_RAM_DATA CONTEXT_DATA  pvCtxData;
/* no context for erase and user test operation */
static IN_RAM_DATA CONTEXT_DATA  dummyCtxData;


static IN_RAM_DATA fnet_uint32_t failedAddress; /* save the failed address in flash */
static IN_RAM_DATA fnet_uint32_t failedData;    /* save the failed data in flash */
static IN_RAM_DATA fnet_uint32_t failedSource;  /* save the failed source for verification */

/********************************************************************************************************************************************/
/* PRIVATE PROTOTYPE                                                                                                                        */
/********************************************************************************************************************************************/
static void DisableFlashControllerCache(
    fnet_uint32_t     flashConfigReg,
    fnet_uint32_t     disableVal,
    fnet_uint32_t     *origin_pflash_pfcr
);
static void RestoreFlashControllerCache(
    fnet_uint32_t     flashConfigReg,
    fnet_uint32_t     pflash_pfcr
);
static IN_RAM_CODE fnet_uint32_t WaitOperationFinish(
    PSSD_CONFIG       pSSDConfig,
    fnet_uint8_t      modeOp,
    PCONTEXT_DATA     pCtxData
);
static IN_RAM_CODE fnet_uint32_t App_FlashErase(
    PSSD_CONFIG       pSSDConfig,
    fnet_uint8_t      eraseOption,
    fnet_uint32_t     lowBlockSelect,
    fnet_uint32_t     midBlockSelect,
    fnet_uint32_t     highBlockSelect,
    N256K_BLOCK_SEL   n256KBlockSelect
);
static IN_RAM_CODE fnet_uint32_t App_FlashProgram(
    PSSD_CONFIG    pSSDConfig,
    fnet_uint8_t   pgmOption,
    fnet_uint32_t  dest,
    fnet_uint32_t  size,
    fnet_uint32_t  source,
    PCONTEXT_DATA  pCtxData
);
static IN_RAM_CODE fnet_uint32_t App_ProgramVerify(PSSD_CONFIG pSSDConfig,
        fnet_uint32_t  dest,
        fnet_uint32_t  size,
        fnet_uint32_t  source,
        fnet_uint32_t  *pFailedAddress,
        fnet_uint32_t  *pFailedData,
        fnet_uint32_t  *pFailedSource,
        PCONTEXT_DATA  pCtxData
                                                  );
static IN_RAM_CODE fnet_uint32_t App_BlankCheck(PSSD_CONFIG pSSDConfig,
        fnet_uint32_t  dest,
        fnet_uint32_t  size,
        fnet_uint32_t  *pFailedAddress,
        fnet_uint32_t  *pFailedData,
        PCONTEXT_DATA  pCtxData
                                               );

static fnet_uint8_t fnet_cpu_flash_block_find(
    fnet_uint8_t  *flash_addr
);
static void fnet_cpu_flash_setBlockSel(
    PALL_BLOCK_SEL pBlockSel,
    fnet_uint8_t   blockID
);
static fnet_int32_t fnet_cpu_flash_init(
    void
);
static fnet_int32_t fnet_cpu_flash_unlock(
    PSSD_CONFIG pSSDConfig
);
static  SSD_CONFIG* fnet_cpu_flash_init_SSD(
    void
);

static PAPPFLASHERASE            pAppFlashErase    = (PAPPFLASHERASE) App_FlashErase;
static PAPPFLASHPROG             pAppFlashProgram  = (PAPPFLASHPROG) App_FlashProgram;
static PAPPFLASHPROGVERIFY       pAppProgramVerify = (PAPPFLASHPROGVERIFY) App_ProgramVerify;
static PAPPBLANKCHECK            pAppBlankCheck    = (PAPPBLANKCHECK) App_BlankCheck;


/************************************************************************
* NAME: fnet_cpu_flash_block_find
*
* DESCRIPTION:
************************************************************************/
static fnet_uint8_t fnet_cpu_flash_block_find(
    fnet_uint8_t  *flash_addr
)
{
    fnet_uint8_t blockID;
    fnet_uint8_t i;

    for (i = 0; i < NUM_BLOCK_ALL; i++)
    {
       if((BLOCK_START_ADDRS[i] >= (fnet_uint32_t)flash_addr) )
       {
            blockID = i;
            break;
        }
    }

    return blockID;
}

/************************************************************************
* NAME: fnet_cpu_flash_unlock
*
* DESCRIPTION:
************************************************************************/
static void fnet_cpu_flash_setBlockSel(
    PALL_BLOCK_SEL pBlockSel,
    fnet_uint8_t   blockID
)
{
    register fnet_uint32_t returnCode;           /* Return code from each SSD function. */
    fnet_uint32_t temp;

    pBlockSel->lowBlockSelect = 0x0;
    pBlockSel->midBlockSelect = 0;
    pBlockSel->highBlockSelect = 0;
    pBlockSel->n256KBlockSelect.first256KBlockSelect = 0;
    pBlockSel->n256KBlockSelect.second256KBlockSelect = 0;

    /**************************************************************************/
    /* Unlock blocks according to address space                               */
    /**************************************************************************/
    if (blockID < NUM_LOW_BLOCK)
    {
        pBlockSel->lowBlockSelect = (1 << blockID);
    }
    else if (blockID < (NUM_LOW_BLOCK + NUM_MID_BLOCK))
    {
    }
    else if (blockID < (NUM_LOW_BLOCK + NUM_MID_BLOCK + NUM_HIGH_BLOCK))
    {
        temp = blockID - (NUM_LOW_BLOCK + NUM_MID_BLOCK);
        pBlockSel->highBlockSelect = (1 << temp);
    }
    else if (blockID < (NUM_LOW_BLOCK + NUM_MID_BLOCK + NUM_HIGH_BLOCK + NUM_256K_BLOCK_FIRST))
    {

        temp = blockID - (NUM_LOW_BLOCK + NUM_MID_BLOCK + NUM_HIGH_BLOCK);
        pBlockSel->n256KBlockSelect.first256KBlockSelect = (1 << temp);
    }
    else
    {
        temp = blockID - (NUM_LOW_BLOCK + NUM_MID_BLOCK + NUM_HIGH_BLOCK + NUM_256K_BLOCK_SECOND);
        pBlockSel->n256KBlockSelect.second256KBlockSelect = (1 << temp);
    }
}

/************************************************************************
* NAME: fnet_cpu_flash_unlock
*
* DESCRIPTION:
************************************************************************/
static fnet_int32_t fnet_cpu_flash_unlock(
    PSSD_CONFIG pSSDConfig
)
{
    register fnet_uint32_t  returnCode;           /* Return code from each SSD function. */
    fnet_uint32_t           blkLockState;         /* block lock status to be retrieved */

    /**************************************************************************/
    /* Unlock all except FNET Bootloader region                               */
    /**************************************************************************/
    returnCode = pSetLock(pSSDConfig, C55_BLOCK_LOW, 0x00000000L);
    if (C55_OK != returnCode)
    {
         return FNET_FLASH_ERR_LOCK_OP;
    }

    returnCode = pSetLock(pSSDConfig, C55_BLOCK_MID, 0x00000000L);
    if (C55_OK != returnCode)
    {
         return FNET_FLASH_ERR_LOCK_OP;
    }

    returnCode = pSetLock(pSSDConfig, C55_BLOCK_HIGH, 0x00000003L);
    if (C55_OK != returnCode)
    {
         return FNET_FLASH_ERR_LOCK_OP;
    }

    returnCode = pSetLock(pSSDConfig, C55_BLOCK_256K_FIRST, 0x00000000L);
    if (C55_OK != returnCode)
    {
         return FNET_FLASH_ERR_LOCK_OP;
    }


    /**************************************************************************/
    /* Lock to Protect UTest Address Space                                    */
    /**************************************************************************/
    returnCode = pGetLock(pSSDConfig, C55_BLOCK_UTEST, &blkLockState);
    if (C55_OK != returnCode)
    {
         return FNET_FLASH_ERR_LOCK_OP;
    }

    if (!(blkLockState & 0x00000001))
    {
         returnCode = pSetLock(pSSDConfig, C55_BLOCK_UTEST, 0x1);
         if (C55_OK != returnCode)
         {
             return FNET_FLASH_ERR_LOCK_OP;
         }
    }

    return FNET_OK;
}


/************************************************************************
* NAME: fnet_cpu_flash_init_SSD
*
* DESCRIPTION:
************************************************************************/
static SSD_CONFIG* fnet_cpu_flash_init_SSD(
    void
)
{
    static SSD_CONFIG ssdConfig =
    {
        C55_REG_BASE,                /* C55 control register base */
        MAIN_ARRAY_BASE,             /* base of main array */
        { 0, 0, 0 },                 /* blocks info of low address space */
        { 0, 0, 0 },                 /* blocks info of mid address space */
        { 0, 0, 0 },                 /* blocks info of high address space */
        0,                           /* number of blocks in 256K address space */
        UTEST_ARRAY_BASE,            /* base of UTEST array */
        (fnet_boolean_t)FNET_TRUE,   /* interface flag indicate main or alternate interface */
        C55_PROGRAMMABLE_SIZE,       /* programmable size */
        (fnet_boolean_t)FNET_FALSE   /* debug mode selection */
    };

    /* set up function pointers in context data */
    pgmCtxData.pReqCompletionFn = pFlashProgram;
    bcCtxData.pReqCompletionFn = pBlankCheck;
    pvCtxData.pReqCompletionFn = pProgramVerify;
    /*csCtxData.pReqCompletionFn = pCheckSum;*/
    /*exPgmCtxData.pReqCompletionFn = pFlashExpressProgram;*/

    /*set SSD config pointer*/
    return &ssdConfig;
}


/************************************************************************
* NAME: fnet_cpu_flash_init
*
* DESCRIPTION:
************************************************************************/
static fnet_int32_t fnet_cpu_flash_init(
    void
)
{
    register fnet_uint32_t  returnCode;           /* Return code from each SSD function. */

    /*Init pointer to the SSD struct*/
    pSSDConfig = fnet_cpu_flash_init_SSD();

    /*Data cache must be also disabled, set FNET_CFG_CPU_CACHE for proper operation of the Flash Driver (SSD)*/
    DisableFlashControllerCache(FLASH_PFCR1, FLASH_FMC_BFEN, &pflash_pfcr1);

    /* Flash Init */
    returnCode = pFlashInit(pSSDConfig);
    if (C55_OK != returnCode)
    {
         return FNET_FLASH_ERR_INIT;
    }

    /* Flash unlock blocks, except reserved*/
    returnCode = fnet_cpu_flash_unlock(pSSDConfig);
    if (returnCode)
    {
         return FNET_FLASH_ERR_LOCK_OP;
    }

    return FNET_OK;
}


/************************************************************************
* NAME: fnet_cpu_flash_erase
*
* DESCRIPTION:
************************************************************************/
fnet_return_t fnet_cpu_flash_erase(
    void *flash_addr,
    fnet_size_t  n
)
{
    register fnet_uint32_t  result;
    fnet_uint8_t            blockID;
    fnet_cpu_irq_desc_t     irq_desc;
    fnet_int32_t            err = FNET_OK;
    fnet_size_t             bytes;
    fnet_uint8_t            *addr = flash_addr;

    irq_desc = fnet_cpu_irq_disable();

    /*Initialize only if pointer is NULL*/
    if (pSSDConfig == NULL)
    {
        err = fnet_cpu_flash_init();
        if (err)
        {
            goto FAIL;
        }
    }

    /*Erase block(s) loop*/
    while (n)
    {
        /*find block index according to the current address*/
        blockID = fnet_cpu_flash_block_find((fnet_uint8_t *)addr);

        /*set the block selection according to the current block*/
        fnet_cpu_flash_setBlockSel(&blockSel, blockID);

        /*get block size*/
        bytes = (BLOCK_END_ADDRS[blockID] - BLOCK_START_ADDRS[blockID]) + 1U;

        /* erase block individually */
        result = pAppFlashErase(pSSDConfig,
                                C55_ERASE_MAIN,
                                blockSel.lowBlockSelect,
                                blockSel.midBlockSelect,
                                blockSel.highBlockSelect,
                                blockSel.n256KBlockSelect);

        if (result == OPERATION_FAIL)
        {
            err = FNET_FLASH_ERR_ERASE;
            goto FAIL;
        }

        /* blank check */
        result = pAppBlankCheck(pSSDConfig,
                                (fnet_uint32_t)addr,
                                bytes,
                                &failedAddress,
                                &failedData,
                                &bcCtxData);

        if (result == OPERATION_FAIL)
        {
            err = FNET_FLASH_ERR_NOTBLANK;
            goto FAIL;
        }

        if(bytes > n)
        {
            bytes = n;
        }
        n -= bytes;

        /*incr. address for next erase loop*/
        flash_addr = (fnet_uint8_t*)flash_addr + n;
    }

    //RestoreFlashControllerCache(FLASH_PFCR1 ,pflash_pfcr1);
FAIL:
    fnet_cpu_irq_enable(irq_desc);

	if(err != FNET_OK)
	{
		err = FNET_ERR
	}
	
    return (fnet_return_t)err;
}

/************************************************************************
* NAME: fnet_cpu_flash_write
*
* DESCRIPTION:
************************************************************************/
fnet_return_t fnet_cpu_flash_write(
    fnet_uint8_t         *dest,
    const fnet_uint8_t   *data
)
{
    register fnet_uint32_t  result;
    fnet_uint8_t            blockID;
    fnet_cpu_irq_desc_t     irq_desc;
    fnet_int32_t            err = FNET_OK;

    irq_desc = fnet_cpu_irq_disable();

    /*Initialize only if pointer is NULL*/
    if (pSSDConfig == NULL)
    {
        err = fnet_cpu_flash_init();
        if (err)
        {
            goto FAIL;
        }
    }

    /* address double-word (64-bit) aligned? */
    if (((fnet_uint32_t)dest & 0x00000007UL) != 0U)
    {
        err = FNET_FLASH_ERR_DST_UNALIGNED;
        goto FAIL;
    }

    /* Blank check */
    result = pAppBlankCheck(pSSDConfig,
                            (fnet_uint32_t)dest,
                            FNET_CFG_CPU_FLASH_PROGRAM_SIZE,
                            &failedAddress,
                            &failedData,
                            &bcCtxData);

    if (result == OPERATION_FAIL)
    {
        err = FNET_FLASH_ERR_NOTBLANK;
        goto FAIL;
    }

    /* Program */
    result = pAppFlashProgram(pSSDConfig,
                              (fnet_boolean_t)FNET_FALSE,
                              (fnet_uint32_t)dest,
                              FNET_CFG_CPU_FLASH_PROGRAM_SIZE,
                              (fnet_uint32_t)data,
                              &pgmCtxData);

    if (result == OPERATION_FAIL)
    {
        err = FNET_FLASH_ERR_WRITEFAILED;
        goto FAIL;
    }

    /* Program verify */
    result = pAppProgramVerify(pSSDConfig,
                               (fnet_uint32_t)dest,
                               FNET_CFG_CPU_FLASH_PROGRAM_SIZE,
                               (fnet_uint32_t)data,
                               &failedAddress,
                               &failedData,
                               &failedSource,
                               &pvCtxData);

    if (result == OPERATION_FAIL)
    {
        err = FNET_FLASH_ERR_VERIFY;
        goto FAIL;
    }

    //RestoreFlashControllerCache(FLASH_PFCR1 ,pflash_pfcr1);
FAIL:
    fnet_cpu_irq_enable(irq_desc);

	if(err != FNET_OK)
	{
		err = FNET_ERR
	}
	
    return (fnet_return_t)err;
}

/***************************************************************************!

@name:  WaitOperationFinish

@brief: This function is to complete the whole operation.

@param: PSSD_CONFIG, fnet_uint8_t, PCONTEXT_DATA

@return: fnet_uint32_t

 ****************************************************************************/
static IN_RAM_CODE fnet_uint32_t WaitOperationFinish(
    PSSD_CONFIG    pSSDConfig,
    fnet_uint8_t   modeOp,
    PCONTEXT_DATA  pCtxData
)
{
    fnet_vuint32_t returnCode = C55_INPROGRESS;
    fnet_uint32_t opResult;
    fnet_uint32_t result;

    while (returnCode == C55_INPROGRESS)
    {

        returnCode = pFlashCheckStatus(pSSDConfig, modeOp, &opResult, pCtxData);

        /* this driver is designed with asynchronous model, thus while the FlashCheckStatus function is still INPROGRESS,
        user can do any other task in user's application. If user does not do any task within this time interval, it is most
        likely synchronous model*/

        /*User_App();*/
    }

    if ((returnCode == C55_DONE) && (opResult == C55_OK))
    {
        result = C55_OK;
    }
    else
    {
        result = OPERATION_FAIL;
    }

    return (result);
}
/***************************************************************************!

@name:  App_FlashErase

@brief: This function erase the corresponding flash space and wait until operation
        completed.

@param: PSSD_CONFIG, fnet_uint8_t, fnet_uint32_t, fnet_uint32_t, fnet_uint32_t, N256K_BLOCK_SEL

@return: fnet_uint32_t

 ****************************************************************************/
static IN_RAM_CODE fnet_uint32_t App_FlashErase( PSSD_CONFIG pSSDConfig,
    fnet_uint8_t      eraseOption,
    fnet_uint32_t     lowBlockSelect,
    fnet_uint32_t     midBlockSelect,
    fnet_uint32_t     highBlockSelect,
    N256K_BLOCK_SEL   n256KBlockSelect
)
{
    fnet_uint32_t returnCode;
    fnet_uint32_t result;

    returnCode = pFlashErase(pSSDConfig,
                             eraseOption,
                             lowBlockSelect,
                             midBlockSelect,
                             highBlockSelect,
                             n256KBlockSelect);

    if(C55_OK == returnCode)
    {
        /* Wait until the operation finishes */
        result = WaitOperationFinish(pSSDConfig, C55_MODE_OP_ERASE, &dummyCtxData);
    }
    else
    {
        result = OPERATION_FAIL;
    }

    return (result);
}
/***************************************************************************!

@name:  App_FlashProgram

@brief: This function prgramm the corresponding flash space and wait until operation
        completed.

@param: PSSD_CONFIG, BOOL, fnet_uint32_t, fnet_uint32_t, fnet_uint32_t, PCONTEXT_DATA

@return: fnet_uint32_t

 ****************************************************************************/
static IN_RAM_CODE fnet_uint32_t App_FlashProgram(
    PSSD_CONFIG    pSSDConfig,
    fnet_uint8_t   pgmOption,
    fnet_uint32_t  dest,
    fnet_uint32_t  size,
    fnet_uint32_t  source,
    PCONTEXT_DATA  pCtxData
)
{
    fnet_uint32_t returnCode;
    fnet_uint32_t result;

    returnCode = pFlashProgram(pSSDConfig, pgmOption, dest, size, source, pCtxData);

    if(C55_OK == returnCode)
    {
        /* Wait until the operation finishes */
        result = WaitOperationFinish(pSSDConfig, C55_MODE_OP_PROGRAM, pCtxData);
    }
    else
    {
        result = OPERATION_FAIL;
    }

    return (result);
}

/***************************************************************************!

@name:  App_ProgramVerify

@brief: This function verify the corresponding flash space and wait until operation
        completed.

@param: PSSD_CONFIG, UINT32, UINT32, UINT32, UINT32*, UINT32*, UINT32*, PCONTEXT_DATA

@return: UINT32

 ****************************************************************************/
fnet_uint32_t IN_RAM_CODE App_ProgramVerify(
    PSSD_CONFIG    pSSDConfig,
    fnet_uint32_t  dest,
    fnet_uint32_t  size,
    fnet_uint32_t  source,
    fnet_uint32_t  *pFailedAddress,
    fnet_uint32_t  *pFailedData,
    fnet_uint32_t  *pFailedSource,
    PCONTEXT_DATA  pCtxData
)
{
    fnet_uint32_t returnCode;
    fnet_uint32_t result;

    returnCode = pProgramVerify(pSSDConfig,
                                dest,
                                size,
                                source,
                                pFailedAddress,
                                pFailedData,
                                pFailedSource,
                                pCtxData
                               );

    if(C55_OK == returnCode)
    {
        /* Wait until the operation finishes */
        result = WaitOperationFinish(pSSDConfig, C55_MODE_OP_PROGRAM_VERIFY, pCtxData);
    }
    else
    {
        result = OPERATION_FAIL;
    }

    return (result);
}

/***************************************************************************!

@name:  App_BlankCheck

@brief: This function check whether the the corresponding flash space is blank
        and wait until operation completed.

@param: PSSD_CONFIG, UINT32, UINT32, UINT32*, UINT32*, PCONTEXT_DATA

@return: UINT32

 ****************************************************************************/
static IN_RAM_CODE fnet_uint32_t App_BlankCheck(
    PSSD_CONFIG    pSSDConfig,
    fnet_uint32_t  dest,
    fnet_uint32_t  size,
    fnet_uint32_t  *pFailedAddress,
    fnet_uint32_t  *pFailedData,
    PCONTEXT_DATA  pCtxData
)
{
    fnet_uint32_t returnCode;
    fnet_uint32_t result;

    returnCode = pBlankCheck(pSSDConfig, dest, size, pFailedAddress, pFailedData, pCtxData);

    if(C55_OK == returnCode)
    {
        /* Wait until the operation finishes */
        result = WaitOperationFinish(pSSDConfig, C55_MODE_OP_BLANK_CHECK, pCtxData);
    }
    else
    {
        result = OPERATION_FAIL;
    }

    return (result);
}

/************************************************************************
* NAME: DisableFlashControllerCache
*
* DESCRIPTION:
************************************************************************/
static void DisableFlashControllerCache(
    fnet_uint32_t flashConfigReg,
    fnet_uint32_t disableVal,
    fnet_uint32_t *origin_pflash_pfcr
)
{
    /* Read the values of the register of flash configuration */
    *origin_pflash_pfcr = READ32(FLASH_FMC + flashConfigReg);

    /* Disable Caches */
    CLEAR32(FLASH_FMC + flashConfigReg, disableVal);
}

/************************************************************************
* NAME: RestoreFlashControllerCache
*
* DESCRIPTION: Restore configuration register of FCM
************************************************************************/
static void RestoreFlashControllerCache(
    fnet_uint32_t flashConfigReg,
    fnet_uint32_t pflash_pfcr
)
{
    WRITE32(FLASH_FMC + flashConfigReg, pflash_pfcr);
}

#endif /* FNET_MPC && FNET_CFG_CPU_FLASH */
