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
*  FNET Shell Demo API.
*
***************************************************************************/

#ifndef _FAPP_MEM_H_

#define _FAPP_MEM_H_

#include "fapp_config.h"
#include "fapp.h"

/************************************************************************
*     Definitions.
*************************************************************************/
/* Memory region.*/
struct fapp_mem_region
{
    fnet_char_t     *description;       /* Memory region description. Displayed to user.*/
    fnet_uint32_t   address;            /* Memory region start address.*/
    fnet_size_t     size;               /* Memory region size in bytes.*/
    fnet_return_t   (*memcpy)( FNET_COMP_PACKED_VAR void *dest, const FNET_COMP_PACKED_VAR void *src, fnet_size_t n ); /* Write */
    fnet_return_t   (*erase)( void *erase_addr, fnet_size_t n ); /* Erase */
    fnet_return_t   (*flush)( void );   /* Flush write cache. It is optional. */
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

fnet_return_t fapp_mem_memcpy (fnet_shell_desc_t desc, FNET_COMP_PACKED_VAR void *dest, const FNET_COMP_PACKED_VAR void *src, fnet_size_t n );
fnet_return_t fapp_mem_flush(void);
void fapp_mem_erase_all(fnet_shell_desc_t desc);
fnet_return_t fapp_mem_region_get_begin_end(fnet_index_t n, fnet_uint8_t *FNET_COMP_PACKED_VAR *begin_p, fnet_uint8_t *FNET_COMP_PACKED_VAR *end_p);

#if defined(__cplusplus)
}
#endif

#endif
