/**************************************************************************
*
* Copyright 2011-2016 by Andrey Butok. FNET Community.
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

/* Memory regions.
 * They should be aligned to logical borders. */
const struct fapp_mem_region fapp_mem_regions[] =
{
    FAPP_CFG_MEM_REGION_LIST
    {0, 0, 0, 0, 0, 0} /* End */
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
* DESCRIPTION: Returns begin and end address of n-th region (starting from 0)
************************************************************************/
fnet_return_t fapp_mem_region_get_begin_end(fnet_index_t n, fnet_uint8_t *FNET_COMP_PACKED_VAR *begin_p, fnet_uint8_t *FNET_COMP_PACKED_VAR *end_p)
{
    const struct fapp_mem_region    *region = fapp_mem_regions;
    fnet_return_t                   result = FNET_ERR;

    while(region->description)
    {
        if(n == 0)
        {
            /* Region start. */
            *begin_p = (fnet_uint8_t *)region->address;
            /* Region end. */
            *end_p = (fnet_uint8_t *)region->address + region->size;
            result = FNET_OK;
            break;
        }
        region++;
        n--;
    }
    return result;
}

/************************************************************************
* DESCRIPTION: Write to not reserved memory region
************************************************************************/
fnet_return_t fapp_mem_memcpy (fnet_shell_desc_t desc, FNET_COMP_PACKED_VAR void *dest, const FNET_COMP_PACKED_VAR void *src, fnet_size_t n )
{
    fnet_size_t     i;
    const struct    fapp_mem_region *region;

    /* Find memory region.*/
    region = fapp_mem_region_find( (fnet_uint32_t)dest, n);

    if(region && (region->memcpy))
    {
#if FAPP_CFG_CHECK_FLASH_BEFORE_WRITE
        if(region->erase) /* Check if it is FLASH memory*/
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
        return region->memcpy(dest, src, n);
    }

FAIL:
    fnet_shell_println(desc, FAPP_MEM_ERROR_WRITEFAILED, n, dest);
    return FNET_ERR;
}

/************************************************************************
* DESCRIPTION: Flush all regions.
************************************************************************/
fnet_return_t fapp_mem_flush(void)
{
    const struct    fapp_mem_region *region = fapp_mem_regions;
    fnet_return_t   result;

    while(region->description && region->flush)
    {
        result = region->flush();
        if(result == FNET_ERR)
        {
            break;
        }
        region++;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Shows the memory regions.
************************************************************************/
#if FAPP_CFG_MEM_CMD
void fapp_mem_cmd ( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    const struct fapp_mem_region *mem = fapp_mem_regions;

    FNET_COMP_UNUSED_ARG(desc);
    FNET_COMP_UNUSED_ARG(argc);
    FNET_COMP_UNUSED_ARG(argv);

    /* Print memory types. */
    fnet_shell_println(desc, FAPP_MEM_HEADER, "Memory type");
    fnet_shell_println(desc, FAPP_MEM_DELIMITER);
    while(mem->description)
    {
        fnet_shell_println(desc, MEM_STR, mem->description, mem->address, mem->address + mem->size - 1u);
        mem++;
    }

    fnet_shell_println(desc, "");
}
#endif


#if FAPP_CFG_ERASE_CMD
/************************************************************************
* DESCRIPTION: Erase
************************************************************************/
static fnet_return_t fapp_mem_erase( void *addr, fnet_size_t n)
{
    fnet_return_t                   result = FNET_ERR;
    const struct fapp_mem_region    *region;

    /* Find memory region.*/
    region = fapp_mem_region_find( (fnet_uint32_t)addr, n);

    if(region)
    {
        if(region->erase)
        {
            result = region->erase(addr, n);
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Erase all regions, except reserved areas
************************************************************************/
void fapp_mem_erase_all(fnet_shell_desc_t desc)
{
    const struct fapp_mem_region    *region = fapp_mem_regions;

    /* Erase all regions. */
    while(region->description)
    {
        if((region->erase) && (region->size))
        {
            if( fapp_mem_erase((void *)region->address, region->size) == FNET_OK)
            {
                fnet_shell_println(desc, FAPP_MEM_ERASE_ERASED, region->address, region->address + region->size - 1u );
            }
            else
            {
                fnet_shell_println(desc, FAPP_MEM_ERASE_FAILED, region->address, region->address + region->size - 1u );
            }
        }
        region++;
    }
}

/************************************************************************
* DESCRIPTION:  Shell Erase command
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
