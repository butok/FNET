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
* @file fapp_mem.h
*
* @author Andrey Butok
*
* @brief FNET Shell Demo API.
*
***************************************************************************/

#ifndef _FAPP_MEM_H_

#define _FAPP_MEM_H_

#include "fapp_config.h"
#include "fapp.h"

/************************************************************************
*     Definitions.
*************************************************************************/
#define FAPP_APPLICATION_ADDRESS    FAPP_CFG_APPLICATION_ADDRESS /* Actually is the start address of the flash.*/
#define FAPP_APPLICATION_SIZE       FAPP_CFG_APPLICATION_SIZE

#define FAPP_SRAM_ADDRESS           FNET_CFG_CPU_SRAM_ADDRESS
#define FAPP_SRAM_SIZE              FNET_CFG_CPU_SRAM_SIZE

#define FAPP_FLASH_ADDRESS          FNET_CFG_CPU_FLASH_ADDRESS
#define FAPP_FLASH_SIZE             FNET_CFG_CPU_FLASH_SIZE

#define FAPP_FLASH_PARAMS_SIZE      FNET_CFG_CPU_FLASH_PAGE_SIZE
#define FAPP_FLASH_PARAMS_ADDRESS   (FAPP_FLASH_ADDRESS + FAPP_FLASH_SIZE - FAPP_FLASH_PARAMS_SIZE) /* Last sectopr of the flash.*/

struct fapp_mem_region
{
    fnet_char_t     *description;
    fnet_uint32_t   address;
    fnet_size_t     size;
    void            (*memcpy)( FNET_COMP_PACKED_VAR void *dest, const FNET_COMP_PACKED_VAR void *src, fnet_size_t n );
    void            (*erase)( void *erase_addr, fnet_size_t n );
    fnet_size_t     erase_size; /* Logical page size, that can be erased separately. */
};


extern const struct fapp_mem_region fapp_mem_regions[];

#if defined(__cplusplus)
extern "C" {
#endif

#if FAPP_CFG_MEM_CMD
void fapp_mem_cmd ( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
#endif

#if FAPP_CFG_ERASE_CMD
void fapp_mem_erase_cmd ( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
#endif

fnet_return_t fapp_mem_memcpy (fnet_shell_desc_t desc, void *dest, const void *src, fnet_size_t n );
fnet_bool_t fapp_mem_region_is_protected( fnet_uint32_t start, fnet_size_t n);
void fapp_mem_erase_all(fnet_shell_desc_t desc);

#if defined(__cplusplus)
}
#endif

#endif
