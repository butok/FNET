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
* @file fnet_mk_flash.c
*
* @author Andrey Butok
*
* @brief Kinetis Flash Memory Module (FTFL) driver.
*
***************************************************************************/
#include "fnet.h"

#if FNET_MK && FNET_CFG_CPU_FLASH


#if (FNET_CFG_CPU_FLASH_PROGRAM_SIZE != 4u) && (FNET_CFG_CPU_FLASH_PROGRAM_SIZE != 8u)
    #error "MK Flash driver supports only 4 and 8 size of program-block"
#endif

#ifndef FNET_MK_FLASH_RAM_SECTION
    #define FNET_MK_FLASH_RAM_SECTION   0
#endif /* if 0 -> copy function to RAM buffer.*/


#if FNET_MK_FLASH_RAM_SECTION
    static
    #if FNET_CFG_COMP_UV
        __attribute__((section("FNET_RAM"))) __attribute__((used))
    #endif
    #if FNET_CFG_COMP_GNUC
        __attribute__((section(".FNET_RAM")))
    #endif
    void fnet_ftfl_command_lunch_inram(fnet_vuint8_t *fstat_reg);
#endif


static void fnet_ftfl_command(fnet_uint8_t command, fnet_uint32_t *address, const fnet_uint8_t *data);

/************************************************************************
* NAME: fnet_ftfl_command_lunch_inram
*
* DESCRIPTION: Launch the command. It must be in RAM.
************************************************************************/

#if FNET_MK_FLASH_RAM_SECTION
/* == Must be in the RAM ==*/
#if FNET_CFG_COMP_CW
    #pragma define_section FNET_RAM ".FNET_RAM" ".FNET_RAM" ".FNET_RAM" far_abs RX
    __declspec(FNET_RAM)
#endif
#if FNET_CFG_COMP_IAR
    __ramfunc
#endif
static
#if FNET_CFG_COMP_UV
    __attribute__((section("FNET_RAM"))) __attribute__((used))
#endif
#if FNET_CFG_COMP_GNUC
    __attribute__((section(".FNET_RAM")))
#endif
void fnet_ftfl_command_lunch_inram(fnet_vuint8_t *fstat_reg)
{
    *fstat_reg = FNET_MK_FTFL_FSTAT_CCIF_MASK;
    while ((*fstat_reg & FNET_MK_FTFL_FSTAT_CCIF_MASK) == 0u)
    {}
}

#else

/* Position independent code. fnet_ftfl_command_lunch_inram()  */
const static fnet_uint16_t fnet_ftfl_command_lunch_inram_s[] =
{
    /* Compiled by IAR 7.5 */
    0x2180,         /* MOVS      R1, #128       */
    0x7001,         /* STRB      R1, [R0]       */
    /*@loop*/
    0x7801,         /* LDRB      R1, [R0]       */
    0x0609,         /* LSLS      R1, R1, #24    */
    0xd5fc,         /* BPL.N     @loop          */
    0x4770          /* BX        LR             */
};

/* Pointer to the fnet_ftfl_command_lunch_inram() that must be in RAM.*/
static void(*fnet_ftfl_command_lunch_inram_ptr)(fnet_vuint8_t *fstat_reg) = FNET_NULL;

/* Buffer to copy the fnet_ftfl_command_lunch_inram_s.*/
/* Length depends on fnet_ftfl_command_lunch_inram_s size to be copied to RAM.*/
static fnet_uint8_t fnet_ftfl_command_lunch_inram_buf[sizeof(fnet_ftfl_command_lunch_inram_s)];
#endif

/************************************************************************
* NAME: fnet_ftfl_command
*
* DESCRIPTION: FTFL command
************************************************************************/
static void fnet_ftfl_command( fnet_uint8_t command, fnet_uint32_t *address, const fnet_uint8_t *data )
{
    fnet_cpu_irq_desc_t irq_desc;

#if FNET_CFG_CPU_MK60N512 || FNET_CFG_CPU_MK60DN512 /* This problem exists in first-released-version product (mask set: 0M33Z). */

#if 0  /* For restoring.*/
    fnet_uint32_t fmc_pfb0cr_reg = FNET_MK_FMC_PFB0CR;
    fnet_uint32_t fmc_pfb1cr_reg = FNET_MK_FMC_PFB1CR;
#endif

    /* Workaround:  Allow pflash_only or pflash_only with pflash_swap
     * configurations but disable speculation when in these configurations
     * (via programming PFB0CR and PFB1CR's [2:1] bits to 2'h0."*/
    FNET_MK_FMC_PFB0CR &= 0xFFFFFFE6; /* Single entry buffer disable.*/
    FNET_MK_FMC_PFB1CR &= 0xFFFFFFE6; /* Single entry buffer disable. */
    FNET_MK_FMC_PFB0CR &= 0xFFFFFFF9; /* Data Cache disable. */
    FNET_MK_FMC_PFB1CR &= 0xFFFFFFF9; /* Data Cache disable. */

    /* Just more info about the issue (Lozano Alejandro):
     * I think what we are looking is silicon bug of P2 1.0 with mask set "0M33Z".
     * The Platform Bank 0 Control Register (FMC_PFB0CR) and Platform Bank 1 Control Register
     * (FMC_PFB1CR) provide cache control to the FlexMemory space.  PFB0CR[4:3,0] controls
     * bank0/Code Flash cache functionality, and PFB1CR[4:3,0] controls bank1/Data Flash cache
     * functionality. The FMC comes out of reset with full cache functionality enabled for both flash banks.
     * Due to a logic error, cache functionality cannot be permitted in pflash_only or pflash_only
     * with pflash_swap configurations with total pflash size equal to 384KB or 512KB.
     * Pflash_only sized with total pflash size equal to 128KB, 192KB or 256KB have correct
     * cache functionality.
     * The logic error allows cache aliasing to occur when performing accesses 256KB apart
     * in pflash_only or pflash_only with pflash_swap configurations sized at 384KB or 512KB.
     * E.G. An access to 0x0_eb00 (bank0 access) subsequent to an access to 0x5_eb00 (bank1 access)
     * will erroneously hit and return the pflash cache's 0x5_eb00 read data.
     *
     * The Platform Bank 0 Control Register (FMC_PFB0CR) and Platform Bank 1 Control Register (FMC_PFB1CR) provide speculation control to the FlexMemory space. FMC_PFB0CR[2:1] controls bank0/Code Flash speculation functionality, and FMC_PFB1CR[2:1] controls bank1/Data Flash speculation functionality. The FMC comes out of reset with full speculation functionality enabled for both flash banks.
     *
     * Due to a logic error, speculation functionality cannot be permitted in pflash_only or pflash_only with pflash_swap configurations.  (It is available in the FlexMem configuration.)
     * The logic error allows speculation aliasing to occur in pflash_only or pflash_only with pflash_swap configurations.
     *
     * In pfonly total size 512K, aliasing occurs between accesses 256KB apart.
     * In pfonly total size 384KB, (pfswap illegal), aliasing occurs between
     * accesses in the first 128KB and the last 128KB.
     * In pfonly total size 256KB, aliasing occurs between accesses 128KB apart.
     * In pfonly total size 192KB, (pfswap illegal), aliasing occurs between
     * accesses in the first 64KB and the last 64KB.
     * In pfonly total size 128KB, aliasing occurs between accesses 64KB part.
     */
#if 0 /* Old Version.*/
    /* This problem exists in first released version product (mask set: 0M33Z). It is proved.
     * It is solved in next released version product (mask set: 0N30D) (do not have this mask set => not able to check it).
     */
    FNET_MK_FMC_PFB0CR &= 0xFFFFFFFE; /* Single entry buffer disable.*/
    FNET_MK_FMC_PFB1CR &= 0xFFFFFFFE; /* Single entry buffer disable. */
    FNET_MK_FMC_PFB0CR &= 0xFFFFFFEF; /* Data Cache disable. */
    FNET_MK_FMC_PFB1CR &= 0xFFFFFFEF; /* Data Cache disable. */
#endif
#endif


    irq_desc = fnet_cpu_irq_disable();

    /* The CCIF flag must read 1 to verify that any previous command has
    * completed. If CCIF is zero, the previous command execution is still active, a new
    * command write sequence cannot be started, and all writes to the FCCOB registers are
    * ignored.
    */
    while ((FNET_MK_FTFL_FSTAT & FNET_MK_FTFL_FSTAT_CCIF_MASK) == 0u)
    {}

    /* Ensure that the ACCERR and FPVIOL bits in the FSTAT register are cleared prior to
    *  starting the command write sequence.
    */
    FNET_MK_FTFL_FSTAT = (FNET_MK_FTFL_FSTAT_ACCERR_MASK | FNET_MK_FTFL_FSTAT_FPVIOL_MASK);

    /* The FCCOB register group uses a big endian addressing convention. */

    /* Write a valid command to the FCCOB0 register. */
    FNET_MK_FTFL_FCCOB0 = command;

    /* Flash address.*/
    FNET_MK_FTFL_FCCOB1 = (fnet_uint8_t)(((fnet_uint32_t)address) >> 16);   /* Flash address [23:16] */
    FNET_MK_FTFL_FCCOB2 = (fnet_uint8_t)(((fnet_uint32_t)address) >> 8);    /* Flash address [15:8] */
    FNET_MK_FTFL_FCCOB3 = (fnet_uint8_t)((fnet_uint32_t)address);           /* Flash address [7:0] */
    /* Data.*/

    FNET_MK_FTFL_FCCOB4 = (fnet_uint8_t)(*(data + 3));  /* Data Byte 0.*/
    FNET_MK_FTFL_FCCOB5 = (fnet_uint8_t)(*(data + 2));  /* Data Byte 1.*/
    FNET_MK_FTFL_FCCOB6 = (fnet_uint8_t)(*(data + 1));  /* Data Byte 2.*/
    FNET_MK_FTFL_FCCOB7 = (fnet_uint8_t)(*data);        /* Data Byte 3.*/
#if FNET_CFG_CPU_FLASH_PROGRAM_SIZE == 8u /* K70 */
    if(command == FNET_MK_FNET_FTFL_FCCOB0_CMD_PROGRAM_PHRASE)
    {
        FNET_MK_FTFL_FCCOB8 = (fnet_uint8_t)(*(data + 7));
        FNET_MK_FTFL_FCCOB9 = (fnet_uint8_t)(*(data + 6));
        FNET_MK_FTFL_FCCOBA = (fnet_uint8_t)(*(data + 5));
        FNET_MK_FTFL_FCCOBB = (fnet_uint8_t)(*(data + 4));
    }
#endif

#if FNET_MK_FLASH_RAM_SECTION
    fnet_ftfl_command_lunch_inram(&FNET_MK_FTFL_FSTAT);
#else
    /* To be sure that the function is alreday in ram.*/
    if(fnet_ftfl_command_lunch_inram_ptr == FNET_NULL)
    {
        fnet_ftfl_command_lunch_inram_ptr = (void(*)(fnet_vuint8_t *fstat_reg))fnet_memcpy_func((void *)fnet_ftfl_command_lunch_inram_buf,
                                            fnet_ftfl_command_lunch_inram_s,
                                            sizeof(fnet_ftfl_command_lunch_inram_buf));
    }
    fnet_ftfl_command_lunch_inram_ptr(&FNET_MK_FTFL_FSTAT);
#endif

#if 0 /* FNET_CFG_CPU_MK60N512*/ /* Restore FMC registers.*/
    FNET_MK_FMC_PFB0CR = fmc_pfb0cr_reg;
    FNET_MK_FMC_PFB1CR = fmc_pfb1cr_reg;
#endif

    fnet_cpu_irq_enable(irq_desc);
}

/************************************************************************
* NAME: fnet_cpu_flash_erase
*
* DESCRIPTION:
************************************************************************/
void fnet_cpu_flash_erase( void *flash_page_addr)
{
    fnet_ftfl_command(FNET_MK_FNET_FTFL_FCCOB0_CMD_ERASE_SECTOR, (fnet_uint32_t *)flash_page_addr, 0);
}

/************************************************************************
* NAME: fnet_cpu_flash_write
*
* DESCRIPTION:
************************************************************************/
void fnet_cpu_flash_write( fnet_uint8_t *dest, const fnet_uint8_t *data)
{
#if FNET_CFG_CPU_FLASH_PROGRAM_SIZE == 4u /* K60 */
    fnet_ftfl_command(FNET_MK_FNET_FTFL_FCCOB0_CMD_PROGRAM_LONGWORD, (fnet_uint32_t *)dest, data);
#else /* FNET_CFG_CPU_FLASH_PROGRAM_SIZE == 8 K70 */
    fnet_ftfl_command(FNET_MK_FNET_FTFL_FCCOB0_CMD_PROGRAM_PHRASE, (fnet_uint32_t *)dest, data);
#endif
}


#endif /* FNET_MK && FNET_CFG_CPU_FLASH */
