/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
* Copyright 2008-2010 by Andrey Butok. Freescale Semiconductor, Inc.
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
**********************************************************************/ /*!
*
* @file fnet_mcf_flash.c
*
* @author Andrey Butok
*
* @brief ColdFire Flash Module driver.
*
***************************************************************************/
#include "fnet.h"


#if FNET_MCF && FNET_CFG_CPU_FLASH

#if (FNET_CFG_CPU_FLASH_PROGRAM_SIZE != 4)
    #error "MCF Flash driver supports only 4 size of program-block"
#endif

#ifndef FNET_MK_FLASH_RAM_SECTION
    #define FNET_MK_FLASH_RAM_SECTION   0
#endif /* if 0 -> copy function to RAM buffer.*/


#if FNET_MK_FLASH_RAM_SECTION
/* == Should be in the RAM ==*/
#if FNET_CFG_COMP_CW
    __declspec(data)
#endif
#if FNET_CFG_COMP_IAR
    #pragma segment="FNET_RAMFUNC"
    #pragma location = "FNET_RAMFUNC"
#endif
/************************************************************************
* NAME: _cfm_command_lunch_inram
*
* DESCRIPTION: Launch the command. It must be in RAM.
************************************************************************/
static void _cfm_command_lunch_inram(fnet_vuint8_t *cfmustat_reg)
{

    /* Clear CBEIF flag by writing a 1 to CBEIF to launch the command.*/
    *cfmustat_reg = FNET_MCF_CFM_CFMUSTAT_CBEIF;

    /* The CBEIF flag is set again indicating that the address, data,
     * and command buffers are ready for a new command write sequence to begin.*/
    while( !(*cfmustat_reg & (FNET_MCF_CFM_CFMUSTAT_CBEIF | FNET_MCF_CFM_CFMUSTAT_CCIF)))
    {};
}
#else
/* Position independent code  */
const static fnet_uint8_t cfm_command_lunch_inram_s[] =
{
    /* Compiled by CW10.6 */
    0x10, 0xBC, 0x00, 0x80, 0x72, 0x00, 0x12, 0x10, 0x20, 0x3C, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0x81, 0x67, 0xF2, 0x4E, 0x75
};

/* Pointer to the fnet_ftfl_command_lunch_inram() that must be in RAM.*/
static void(*cfm_command_lunch_inram_ptr)(fnet_vuint8_t *cfmustat_reg) = FNET_NULL;

/* Buffer to copy the cfm_command_lunch_inram_s.*/
/* Length depends on cfm_command_lunch_inram_s size to be copied to RAM.*/
static fnet_uint8_t cfm_command_lunch_inram_buf[sizeof(cfm_command_lunch_inram_s)];

#endif


/************************************************************************
* NAME: cfm_command
*
* DESCRIPTION: CFM command
************************************************************************/
static void cfm_command( unsigned char command, unsigned long *address, unsigned long data )
{
    fnet_cpu_irq_desc_t irq_desc;

    irq_desc = fnet_cpu_irq_disable();

    /* If the CFMCLKD register is written, the DIVLD bit is set. */
    if((FNET_MCF_CFM_CFMCLKD & FNET_MCF_CFM_CFMCLKD_DIVLD) == 0)
    {
        /* CFM initialization. */

        /* Prior to issuing any command, it is necessary to set
         * the CFMCLKD register to divide the internal bus frequency
         * to be within the 150- to 200-kHz range.
         * NOTE: Setting CFMCLKD to a value such that FCLK < 150 KHz
         * can destroy the flash memory due to overstress.
         * Setting CFMCLKD to a value such that FCLK > 200 KHz can
         * result in incomplete programming or erasure of the
         * flash memory array cells.*/
        if ((FNET_CFG_CPU_CLOCK_HZ / 2) > 12800000) /* For bus frequencies greater than 12.8 MHz */
            FNET_MCF_CFM_CFMCLKD = FNET_MCF_CFM_CFMCLKD_DIV((FNET_CFG_CPU_CLOCK_HZ / 2) / 8 / 200000) | FNET_MCF_CFM_CFMCLKD_PRDIV8;
        else
            FNET_MCF_CFM_CFMCLKD = FNET_MCF_CFM_CFMCLKD_DIV((FNET_CFG_CPU_CLOCK_HZ / 2) / 200000);
    }

    /* Write to one or more addresses in the flash memory.*/
#if !FNET_CFG_MCF_V1    /* Use the backdoor address. */
    address = (unsigned long *)(FNET_CFG_MCF_IPSBAR + 0x04000000 + (unsigned long)address);
#endif
    /* Use the frontdoor address. */
    *address = data;

    /* Write a valid command to the CFMCMD register. */
    FNET_MCF_CFM_CFMCMD = command;

#if FNET_MK_FLASH_RAM_SECTION
    _cfm_command_lunch_inram(&FNET_MCF_CFM_CFMUSTAT);
#else
    /* To be sure that the function is alreday in ram.*/
    if(cfm_command_lunch_inram_ptr == FNET_NULL)
    {
        cfm_command_lunch_inram_ptr = (void(*)(fnet_vuint8_t *cfmustat_reg))fnet_memcpy_func((void *)cfm_command_lunch_inram_buf,
                                      cfm_command_lunch_inram_s,
                                      sizeof(cfm_command_lunch_inram_buf));
    }
    cfm_command_lunch_inram_ptr(&FNET_MCF_CFM_CFMUSTAT);
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
    cfm_command( FNET_MCF_CFM_CFMCMD_PAGE_ERASE, flash_page_addr, 0);
}

/************************************************************************
* NAME: fnet_cpu_flash_write
*
* DESCRIPTION:
************************************************************************/
void fnet_cpu_flash_write(unsigned char *dest, const unsigned char *data)
{
    cfm_command(FNET_MCF_CFM_CFMCMD_WORD_PROGRAM, (unsigned long *)dest, *((unsigned long *)data));
}

#endif /* FNET_MCF && FNET_CFG_CPU_FLASH */
