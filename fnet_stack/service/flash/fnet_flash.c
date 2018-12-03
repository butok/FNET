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
*  On-chip Flash Module driver.
*
***************************************************************************/
#include "fnet.h"

#if FNET_CFG_FLASH && FNET_CFG_CPU_FLASH

#if FNET_CFG_FLASH_CACHE

/**************************************************************************/ /*!
 * Flash cache entry.
 ******************************************************************************/
struct fnet_flash_cache_entry
{
    void            *dest_addr;
    fnet_uint8_t    data[FNET_CFG_CPU_FLASH_PROGRAM_SIZE];
};

static  fnet_index_t   entry_next = 0U;

/* Write-cache.*/
static  struct fnet_flash_cache_entry flash_cache[FNET_CFG_FLASH_CACHE_SIZE] = { {0} };

static void _fnet_flash_cache_clear_entry(struct fnet_flash_cache_entry   *entry);
static fnet_return_t _fnet_flash_cache_flush_entry(struct fnet_flash_cache_entry   *entry);
#endif /* FNET_CFG_FLASH_CACHE */

static fnet_return_t _fnet_flash_write_low( fnet_uint8_t *dest, const fnet_uint8_t *src, fnet_size_t n_blocks );

#if FNET_CFG_FLASH_CACHE

/************************************************************************
* DESCRIPTION: Clear cache entry
************************************************************************/
static void _fnet_flash_cache_clear_entry(struct fnet_flash_cache_entry   *entry)
{
    /* Clean up.*/
    fnet_memset(entry->data, 0xFF, FNET_CFG_CPU_FLASH_PROGRAM_SIZE);
    entry->dest_addr = 0;
}

/************************************************************************
* DESCRIPTION: Flush cacche entries to flash
************************************************************************/
static fnet_return_t _fnet_flash_cache_flush_entry(struct fnet_flash_cache_entry   *entry)
{
    fnet_int32_t result;

    if(entry && entry->dest_addr)
    {
        /* Actual write to the flash.*/
        result = _fnet_flash_write_low( entry->dest_addr, entry->data, 1 );
        /* Clean up.*/
        _fnet_flash_cache_clear_entry(entry);

        if (result == FNET_ERR)
        {
            return FNET_ERR;
        }
    }

    return FNET_OK;
}

/************************************************************************
* DESCRIPTION: Write through cache
************************************************************************/
static fnet_return_t _fnet_flash_cache_write( FNET_COMP_PACKED_VAR fnet_uint8_t *dest_addr, const FNET_COMP_PACKED_VAR fnet_uint8_t *data_p, fnet_size_t n)
{
    struct fnet_flash_cache_entry   *entry = FNET_NULL;
    fnet_index_t                    i;
    fnet_return_t                   err;

    /* Find existing entry*/
    for(i = 0u; i < FNET_CFG_FLASH_CACHE_SIZE; i++)
    {
        /*search if aligned dest_addr belongs to an existing entry.*/
        if (flash_cache[i].dest_addr != 0U)
        {
            if ((fnet_uint32_t)flash_cache[i].dest_addr == ((fnet_uint32_t)dest_addr & ~(FNET_CFG_CPU_FLASH_PROGRAM_SIZE - 1U)))
            {
                entry = &flash_cache[i];
                break;
            }
        }
    }

    while (n)
    {
        if(entry == FNET_NULL) /* No existing, so use the current one.*/
        {
            entry = &flash_cache[entry_next];

            entry_next++;
            if(entry_next == FNET_CFG_FLASH_CACHE_SIZE)
            {
                entry_next = 0;
            }

            err = _fnet_flash_cache_flush_entry(entry); /* Flash if something in the cache entry.*/
            if (err)
            {
                return err;
            }

            entry->dest_addr = (void *)((fnet_uint32_t)dest_addr & ~(FNET_CFG_CPU_FLASH_PROGRAM_SIZE - 1U)); /* Init destination address.*/

        }

        for( i = ((fnet_uint32_t)dest_addr & (FNET_CFG_CPU_FLASH_PROGRAM_SIZE - 1U)); (i < FNET_CFG_CPU_FLASH_PROGRAM_SIZE) && n; i++)
        {
            entry->data[(fnet_uint32_t)dest_addr & (FNET_CFG_CPU_FLASH_PROGRAM_SIZE - 1U)] = *data_p++;
            dest_addr++;
            n--;
        }

        entry = FNET_NULL; /*add new entry*/
    }

    return FNET_OK;
}
#endif /* FNET_CFG_FLASH_CACHE */

/************************************************************************
* DESCRIPTION: Flush write cache.
************************************************************************/
fnet_return_t fnet_flash_flush (void)
{
#if FNET_CFG_FLASH_CACHE
    fnet_index_t i;
    fnet_return_t err;

    fnet_service_mutex_lock();
    for(i = 0u; i < FNET_CFG_FLASH_CACHE_SIZE; i++)
    {
        err = _fnet_flash_cache_flush_entry(&flash_cache[i]);
        if (err == FNET_ERR)
        {
            return FNET_ERR;
        }
    }
    fnet_service_mutex_unlock();
#endif

    return FNET_OK;
}

/************************************************************************
* DESCRIPTION: Flash erase
************************************************************************/
fnet_return_t fnet_flash_erase( void *flash_addr, fnet_size_t bytes)
{
    fnet_return_t result;

    fnet_service_mutex_lock();
    result = fnet_cpu_flash_erase((fnet_uint8_t *)flash_addr, bytes);
    fnet_service_mutex_unlock();

    return result;
}

/************************************************************************
* DESCRIPTION: Write n blocks
************************************************************************/
static fnet_return_t _fnet_flash_write_low( fnet_uint8_t *dest, const fnet_uint8_t *src, fnet_size_t n_blocks  )
{
    fnet_return_t err = FNET_OK;

    while (n_blocks && !err)
    {
        err = fnet_cpu_flash_write(dest, src);
        dest += FNET_CFG_CPU_FLASH_PROGRAM_SIZE;
        src += FNET_CFG_CPU_FLASH_PROGRAM_SIZE;
        n_blocks--;
    }

    return err;
}

/************************************************************************
* DESCRIPTION: Writes to the Flash memory.
************************************************************************/
fnet_return_t fnet_flash_memcpy( FNET_COMP_PACKED_VAR void *flash_addr, FNET_COMP_PACKED_VAR const void *src, fnet_size_t n )
{
#if FNET_CFG_FLASH_CACHE
    fnet_return_t result;

    fnet_service_mutex_lock();
    result = _fnet_flash_cache_write( flash_addr, src, n);
    fnet_service_mutex_unlock();

    return result;
#else /* Without cache */
    fnet_size_t     i;
    fnet_uint8_t    data[FNET_CFG_CPU_FLASH_PROGRAM_SIZE];
    fnet_size_t     bytes;
    fnet_size_t     blocks;
    fnet_uint32_t   count;
    fnet_return_t   err;

    fnet_service_mutex_lock();

    if(n)
    {
        count = (fnet_uint32_t)flash_addr & (FNET_CFG_CPU_FLASH_PROGRAM_SIZE - 1U);

        /* Align dest. */
        if(count)
        {
            flash_addr = ((fnet_uint8_t *)flash_addr - count);
            bytes = FNET_CFG_CPU_FLASH_PROGRAM_SIZE - count;

            if(bytes > n)
            {
                bytes = n;
            }

            fnet_memset(data, 0xFFu, FNET_CFG_CPU_FLASH_PROGRAM_SIZE);
            for(i = count; i < count + bytes; i++)
            {
                data[i] = *((const fnet_uint8_t *)src);
                src = ((const fnet_uint8_t *)src + 1U);
            }

            err = _fnet_flash_write_low( (fnet_uint8_t *)flash_addr, data, 1U );
            if(err != FNET_OK)
            {
                goto ERROR;
            }

            flash_addr =  ((fnet_uint8_t *)flash_addr + FNET_CFG_CPU_FLASH_PROGRAM_SIZE);

            n -= bytes;
        }

        if(n)
        {
            bytes = n & (FNET_CFG_CPU_FLASH_PROGRAM_SIZE - 1U);
            blocks = (n - bytes) / FNET_CFG_CPU_FLASH_PROGRAM_SIZE;

            err = _fnet_flash_write_low((fnet_uint8_t *)flash_addr, (const fnet_uint8_t *)src, blocks );
            if(err != FNET_OK)
            {
                goto ERROR;
            }

            flash_addr =  ((fnet_uint8_t *)flash_addr + (FNET_CFG_CPU_FLASH_PROGRAM_SIZE * blocks));
            src = ((const fnet_uint8_t *)src + (FNET_CFG_CPU_FLASH_PROGRAM_SIZE * blocks));

            if(bytes)
            {
                fnet_memset(data, 0xFFu, FNET_CFG_CPU_FLASH_PROGRAM_SIZE);

                for(i = 0U; i < bytes; i++)
                {
                    data[i] = *((const fnet_uint8_t *)src);
                    src = ((const fnet_uint8_t *)src + 1U);
                }

                err = _fnet_flash_write_low((fnet_uint8_t *)flash_addr, data, 1U );
                if(err != FNET_OK)
                {
                    goto ERROR;
                }
            }
        }
    }

    fnet_service_mutex_unlock();

    return FNET_OK;

ERROR:
    fnet_service_mutex_unlock();
    return FNET_ERR;
#endif
}

#endif /* FNET_CFG_FLASH && FNET_CFG_CPU_FLASH */
