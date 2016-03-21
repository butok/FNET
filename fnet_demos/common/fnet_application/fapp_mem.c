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
**********************************************************************/
/*!
*
* @file fapp_mem.c
*
* @author Andrey Butok
*
* @brief FNET Shell Demo implementation.
*
***************************************************************************/

#include "fapp.h"
#include "fapp_prv.h"
#include "fapp_mem.h"

#if FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD || FAPP_CFG_ERASE_CMD || FAPP_CFG_MEM_CMD

/************************************************************************
*     Definitions.
*************************************************************************/


/* Flash regions should be aligned to logical border. */
struct fapp_mem_region_reserved
{
    fnet_char_t    *description;
    fnet_uint32_t   address;
    fnet_size_t     size;
};



const struct fapp_mem_region fapp_mem_regions[] =
{
#if FNET_CFG_FLASH
    {"FLASH", FAPP_FLASH_ADDRESS, FAPP_FLASH_SIZE, fnet_flash_memcpy, fnet_flash_erase, FNET_CFG_CPU_FLASH_PAGE_SIZE},
#endif
    {"SRAM", FAPP_SRAM_ADDRESS, FAPP_SRAM_SIZE, fnet_memcpy, 0, 0},
    {0, 0, 0, 0, 0, 0} /* End */
};


static const struct fapp_mem_region_reserved fapp_mem_regions_reserved[] =
{
    {"FNET ROM", FAPP_APPLICATION_ADDRESS, FAPP_APPLICATION_SIZE},
    {"FNET Params", FAPP_FLASH_PARAMS_ADDRESS, FAPP_FLASH_PARAMS_SIZE},
    {0, 0, 0} /* End */
};


#define MEM_STR             "  %-14s   %#08X   %#08X"
#define FAPP_MEM_HEADER     "\n   %11s       Start         End"
#define FAPP_MEM_DELIMITER  "  ----------------------------------------"

#define FAPP_MEM_ERASE_ALL      "all"
#define FAPP_MEM_ERASE_ERASED   " 0x%08X to 0x%08X erased"
#define FAPP_MEM_ERASE_SKIPPED  " 0x%08X to 0x%08X skipped"
#define FAPP_MEM_ERASE_FAILED   " 0x%08X t7yo 0x%08X failed"


#define FAPP_MEM_ERROR_WRITEFAILED  "\n ERR: Writing %d bytes to 0x%08X failed!"
#define FAPP_MEM_ERROR_NOTERASED    "\n ERR: Memory is not erased!"

static const struct fapp_mem_region *fapp_mem_region_find( fnet_uint32_t start, fnet_size_t n);

#if FAPP_CFG_ERASE_CMD
    static fnet_return_t fapp_mem_erase( void *addr, fnet_size_t n);
#endif

/************************************************************************
* NAME: fapp_mem_region_is_protected
*
* DESCRIPTION:
************************************************************************/
fnet_bool_t fapp_mem_region_is_protected( fnet_uint32_t start, fnet_size_t n)
{
    fnet_bool_t                             result = FNET_FALSE;
    const struct fapp_mem_region_reserved   *region_reserved = fapp_mem_regions_reserved;

    while(region_reserved->description)
    {
        if((region_reserved->address < (start + n)) && ( start <= (region_reserved->address + region_reserved->size - 1u)))
        {
            result = FNET_TRUE;
            break;
        }
        region_reserved++;
    }

    return result;
}

/************************************************************************
* NAME: fapp_mem_region_find
*
* DESCRIPTION:
************************************************************************/
static const struct fapp_mem_region *fapp_mem_region_find( fnet_uint32_t start, fnet_size_t n)
{
    const struct fapp_mem_region *result = 0;
    const struct fapp_mem_region *region = fapp_mem_regions;

    while(region->description)
    {
        if((region->address <= start) && ( (start + n) <= region->address + region->size))
        {
            result = region;
            break;
        }
        region++;
    }

    return result;
}

/************************************************************************
* NAME: fapp_mem_memcpy
*
* DESCRIPTION:
************************************************************************/
fnet_return_t fapp_mem_memcpy (fnet_shell_desc_t desc, void *dest, const void *src, fnet_size_t n )
{
    fnet_size_t     i;
    const struct    fapp_mem_region *region;

    if(fapp_mem_region_is_protected( (fnet_uint32_t)dest, n) == FNET_FALSE)
    {
        /* Find memory region.*/
        region = fapp_mem_region_find( (fnet_uint32_t)dest, n);

        if(region && (region->memcpy))
        {
#if FAPP_CFG_CHECK_FLASH_BEFORE_WRITE
            if(region->erase)
            {
                /* Check if memory is erased.*/
                for(i = 0u; i < n; i++)
                {
                    if(((fnet_uint8_t *)dest)[i] != 0xFFu)
                    {
                        fnet_shell_println(desc, FAPP_MEM_ERROR_NOTERASED);
                        goto FAIL;
                    }
                }
            }
#endif

            /* Write. */
            region->memcpy(dest, src, n);

            /* Verify result. */
#if 0 /* Old code.*/
            for(i = 0; i < n; i++)
            {
                if(((volatile fnet_uint8_t *)dest)[i] != ((fnet_uint8_t *)src)[i])
                {
                    goto FAIL;
                }
            }
#else
            /* Workaround of the Flash cache issue discovered on K60.*/
            /* Delay.  fnet_timer_delay(1); is too big - 100ms.*/
            for(i = 0u; i < 100000000u; i++)
            {
                if(fnet_memcmp(dest, src, n) == 0)
                {
                    break;
                }
            }
            if(i == 100000000u)
            {
                goto FAIL;
            }
#endif

            return FNET_OK;
        }
    }

FAIL:
    fnet_shell_println(desc, FAPP_MEM_ERROR_WRITEFAILED, n, dest);
    return FNET_ERR;
}

/************************************************************************
* NAME: fapp_mem_cmd
*
* DESCRIPTION: Shows the memory-map information for MCU.
* Also shows the memory regions are reserved for FNET (protected).
************************************************************************/
#if FAPP_CFG_MEM_CMD
void fapp_mem_cmd ( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    const struct fapp_mem_region *mem = fapp_mem_regions;
    const struct fapp_mem_region_reserved *region_reserved = fapp_mem_regions_reserved;

    FNET_COMP_UNUSED_ARG(desc);
    FNET_COMP_UNUSED_ARG(argc);
    FNET_COMP_UNUSED_ARG(argv);

    /* Print memory types. */
    fnet_shell_println(desc, FAPP_MEM_HEADER, "Memory type");
    fnet_shell_println(desc, FAPP_MEM_DELIMITER);
    while(mem->description)
    {
        fnet_shell_println(desc, MEM_STR, mem->description, mem->address,
                           mem->address + mem->size - 1u);
        mem++;
    }

    /* Print protected/reserved memory regions.*/
    fnet_shell_println(desc, FAPP_MEM_HEADER, "Reserved");
    fnet_shell_println(desc, FAPP_MEM_DELIMITER);
    while(region_reserved->description)
    {
        fnet_shell_println(desc, MEM_STR, region_reserved->description, region_reserved->address,
                           region_reserved->address + region_reserved->size - 1u);
        region_reserved++;
    }
    fnet_shell_println(desc, "");

}
#endif


#if FAPP_CFG_ERASE_CMD
/************************************************************************
* NAME: fapp_mem_erase
*
* DESCRIPTION:
************************************************************************/
static fnet_return_t fapp_mem_erase( void *addr, fnet_size_t n)
{
    fnet_return_t                   result = FNET_ERR;
    const struct fapp_mem_region    *region;

    if(fapp_mem_region_is_protected( (fnet_uint32_t)addr, n) == FNET_FALSE)
    {
        /* Find memory region.*/
        region = fapp_mem_region_find( (fnet_uint32_t)addr, n);

        if(region)
        {
            if(region->erase)
            {
                region->erase(addr, n);
                result = FNET_OK;
            }
        }
    }
    return result;
}

/************************************************************************
* NAME: fapp_mem_erase_all
*
* DESCRIPTION:
************************************************************************/
void fapp_mem_erase_all(fnet_shell_desc_t desc)
{
    fnet_uint32_t                   addr;
    const struct fapp_mem_region    *region = fapp_mem_regions;

    fnet_uint32_t log_start_addr;
    fnet_size_t   log_erase_size;
    fnet_size_t   log_skip_size;

    /* Check all regions if it has erase function. */
    while(region->description)
    {
        if((region->erase) && (region->erase_size))
        {
            addr = region->address;

            log_start_addr = addr;
            log_erase_size = 0u;
            log_skip_size = 0u;

            while(addr < (region->address + region->size))
            {
                if(fapp_mem_erase((void *)addr, region->erase_size) == FNET_OK)
                {
                    if(log_skip_size)
                    {
                        fnet_shell_println(desc, FAPP_MEM_ERASE_SKIPPED, log_start_addr, log_start_addr + log_skip_size - 1u);
                        log_skip_size = 0u;
                        log_start_addr = addr;
                    }

                    log_erase_size += region->erase_size;
                }
                else
                {
                    if(log_erase_size)
                    {
                        fnet_shell_println(desc, FAPP_MEM_ERASE_ERASED, log_start_addr, log_start_addr + log_erase_size - 1u);
                        log_erase_size = 0u;
                        log_start_addr = addr;
                    }

                    log_skip_size += region->erase_size;
                }
                addr += region->erase_size;
            }

            if(log_erase_size)
            {
                fnet_shell_println(desc, FAPP_MEM_ERASE_ERASED, log_start_addr, log_start_addr + log_erase_size - 1u);
            }
            else
            {
                fnet_shell_println(desc, FAPP_MEM_ERASE_SKIPPED, log_start_addr, log_start_addr + log_skip_size - 1u);
            }
        }
        region++;
    }
}

/************************************************************************
* NAME: fapp_mem_erase_cmd
*
* DESCRIPTION:
************************************************************************/
void fapp_mem_erase_cmd ( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_uint32_t   address;
    fnet_size_t     size;
    fnet_char_t     *p;
    fnet_return_t   result;

    FNET_COMP_UNUSED_ARG(desc);

    fnet_shell_println(desc, "Erasing...");

    if (argc == 3u)
    {
        address = fnet_strtoul(argv[1], &p, 16u);
        if ((address == 0u) && (p == argv[1]))
        {
            fnet_shell_println(desc, FAPP_PARAM_ERR, argv[1] );
            return;
        }

        size = fnet_strtoul(argv[2], &p, 10u);
        if ((size == 0u) && (p == argv[2]))
        {
            fnet_shell_println(desc, FAPP_PARAM_ERR, argv[2] );
            return;
        }

        result = fapp_mem_erase( (void *)address, size);
        if( result == FNET_OK)
        {
            fnet_shell_println(desc, FAPP_MEM_ERASE_ERASED, address, address + size - 1u );
        }
        else
        {
            fnet_shell_println(desc, FAPP_MEM_ERASE_FAILED, address, address + size - 1u );
        }
    }
    else if ((argc == 2u) && (fnet_strcasecmp(FAPP_MEM_ERASE_ALL, argv[1]) == 0)) /* Erase all */
    {
        fapp_mem_erase_all(desc);
    }
    else
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, argv[1]);
    }
}
#endif

#endif /* FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD || FAPP_CFG_ERASE_CMD || FAPP_CFG_MEM_CMD */


