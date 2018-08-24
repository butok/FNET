/**************************************************************************
*
* Copyright 2008-2017 by Andrey Butok. FNET Community.
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
* ROM FS Implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_FS && FNET_CFG_FS_ROM

#include "fnet_fs_rom.h"
#include "fnet_fs_prv.h"

/* Supported fopen mode = read-only */
#define FNET_FS_ROM_OPENMODE     (FNET_FS_MODE_READ|FNET_FS_MODE_OPEN_EXISTING)

static fnet_return_t _fnet_fs_rom_opendir( struct fnet_fs_desc *dir, const fnet_char_t *name);
static fnet_return_t _fnet_fs_rom_readdir(struct fnet_fs_desc *dir, fnet_fs_dirent_t *dirent);
static fnet_return_t _fnet_fs_rom_fopen( struct fnet_fs_desc *file, const fnet_char_t *name, fnet_uint8_t mode, struct fnet_fs_desc *re_dir);
static fnet_size_t _fnet_fs_rom_fread (struct fnet_fs_desc *file, void *buf_p, fnet_size_t bytes);
static fnet_return_t _fnet_fs_rom_mount( const void *arg );
static fnet_return_t _fnet_fs_rom_fseek (struct fnet_fs_desc *file, fnet_int32_t offset, fnet_fs_seek_origin_t origin) ;
static fnet_return_t _fnet_fs_rom_finfo (struct fnet_fs_desc *file, fnet_fs_dirent_t *dirent);
static const fnet_fs_rom_node_t *_fnet_fs_rom_find(const fnet_fs_rom_node_t *file_table, const fnet_char_t *name);
static void _fnet_fs_rom_fill_dirent(fnet_fs_rom_node_t *node, fnet_fs_dirent_t *dirent);

/* FS  directory operations */
static const struct fnet_fs_dir_operations fnet_fs_rom_dir_operations =
{
    _fnet_fs_rom_opendir,
    _fnet_fs_rom_readdir
};

/* FS  file operations */
static const struct fnet_fs_file_operations fnet_fs_rom_file_operations =
{
    _fnet_fs_rom_fopen,
    _fnet_fs_rom_fread,
    _fnet_fs_rom_fseek,
    _fnet_fs_rom_finfo
};

/* FS operations */
static const struct fnet_fs_operations fnet_fs_rom_operations =
{
    _fnet_fs_rom_mount,
    0
};

/* FS interface structure */
static struct fnet_fs fnet_fs_rom =
{
    FNET_FS_ROM_NAME,
    &fnet_fs_rom_operations,
    &fnet_fs_rom_file_operations,
    &fnet_fs_rom_dir_operations,
    0,
    0
};

static fnet_bool_t fnet_fs_rom_registered = FNET_FALSE;  /* Flag that ROM FS is registered or not.*/

/************************************************************************
* DESCRIPTION: This function registers the ROM FS.
*************************************************************************/
void fnet_fs_rom_register( void )
{
    if(fnet_fs_rom_registered == FNET_FALSE)
    {
        fnet_service_mutex_lock();
        _fnet_fs_register(&fnet_fs_rom);
        fnet_fs_rom_registered = FNET_TRUE;
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: This function unregisters the ROM FS.
*************************************************************************/
void fnet_fs_rom_unregister( void )
{
    if(fnet_fs_rom_registered == FNET_TRUE)
    {
        fnet_service_mutex_lock();
        _fnet_fs_unregister(&fnet_fs_rom);
        fnet_fs_rom_registered = FNET_FALSE;
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION:
*************************************************************************/
static fnet_return_t _fnet_fs_rom_mount( const void *arg )
{
    fnet_return_t                   result = FNET_ERR;
    const fnet_fs_rom_image_t       *image;

    if(arg)
    {
        /* Check if the image is ROM FS image and version number*/
        image = ((const fnet_fs_rom_image_t * )arg);
        if( (fnet_strcmp( FNET_FS_ROM_NAME, image->name ) == 0)
            && (image->version == FNET_FS_ROM_VERSION))
        {
            result = FNET_OK;
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION:
*************************************************************************/
static const fnet_fs_rom_node_t *_fnet_fs_rom_find(const fnet_fs_rom_node_t *file_table, const fnet_char_t *name)
{
    const fnet_fs_rom_node_t *result = 0;
    const fnet_fs_rom_node_t *parent = 0;
    const fnet_fs_rom_node_t *current;

    if (file_table && name)
    {
        current = file_table; /* root is always first */

        if (current->name)
        {
            while(*name == ' ')
            {
                name++;	        /* Strip leading spaces */
            }
            if (*name == FNET_FS_SPLITTER)
            {
                name++;	/* Strip heading slash */
            }

            if (*name == '\0') /* Find root */
            {
                result = current;
            }
            else
            {
                parent = current;
                current++;

                while (current->name)
                {
                    if ((current->parent_node == parent)
                        && (_fnet_fs_path_cmp( &name, current->name) == 0))
                    {
                        parent = current;
                    }

                    if (*name == '\0')
                    {
                        result = current;
                        break;
                    }

                    current++;
                }
            }
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Open DIR stream for the ROM FS.
*************************************************************************/
static fnet_return_t _fnet_fs_rom_opendir( struct fnet_fs_desc *dir, const fnet_char_t *name)
{
    fnet_return_t              result = FNET_ERR;
    const fnet_fs_rom_node_t   *file_table;
    const fnet_fs_rom_node_t   *node;

    if(dir && name)
    {
        /* Find dir */
        file_table = ((const fnet_fs_rom_image_t * )dir->mount->arg)->nodes;

        node = _fnet_fs_rom_find(file_table, name);

        if(node && (node->data == 0) /* Is dir (not file)? */)
        {
            dir->id = (fnet_uint32_t) node; /* save pointer to found dir */
            dir->pos = 0u;
            result = FNET_OK;
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION:
*************************************************************************/
static void _fnet_fs_rom_fill_dirent(fnet_fs_rom_node_t *node, fnet_fs_dirent_t *dirent)
{
    dirent->d_ino = (fnet_uint32_t) node; /* File serial number. */
    dirent->d_type = (node->data == 0) ? DT_DIR : DT_REG;
    dirent->d_name = node->name;
    dirent->d_size = node->data_size;
}

/************************************************************************
* DESCRIPTION: Read DIR stream for the ROM FS.
*************************************************************************/
static fnet_return_t _fnet_fs_rom_readdir(struct fnet_fs_desc *dir, fnet_fs_dirent_t *dirent)
{
    fnet_return_t      result = FNET_ERR;
    fnet_fs_rom_node_t *current;
    fnet_fs_rom_node_t *parent;

    if(dir && (dir->id) && (dir->pos != (fnet_uint32_t)FNET_FS_EOF) && dirent)
    {
        if(dir->pos == 0u)
        {
            current = (fnet_fs_rom_node_t *)(dir->id) + 1;
        }
        else
        {
            current = (fnet_fs_rom_node_t *)dir->pos;
        }

        parent = (fnet_fs_rom_node_t *)(dir->id);

        while (current->name)
        {
            if(current->parent_node == parent) /* Next node is found */
            {

                dir->pos = (fnet_uint32_t) (current + 1); /* Save position */
                _fnet_fs_rom_fill_dirent(current, dirent);
                result = FNET_OK;
                break;
            }

            current++;
        }

        if (result == FNET_ERR)
        {
            dir->pos = (fnet_uint32_t) FNET_FS_EOF; /* end of the directory is encountered */
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Open FILE stream for the ROM FS.
*************************************************************************/
static fnet_return_t _fnet_fs_rom_fopen( struct fnet_fs_desc *file, const fnet_char_t *name, fnet_uint8_t mode, struct fnet_fs_desc *re_dir )
{
    fnet_return_t                   result = FNET_ERR;
    const fnet_fs_rom_node_t        *file_table;
    const fnet_fs_rom_node_t        *node;

    if(file && name && (mode == FNET_FS_ROM_OPENMODE))
    {
        /* Find dir */
        if(re_dir && (re_dir->id))
        {
            file_table = (fnet_fs_rom_node_t *) re_dir->id;
        }
        else
        {
            file_table = ((const fnet_fs_rom_image_t * )file->mount->arg)->nodes;
        }

        node = _fnet_fs_rom_find(file_table, name);

        if(node && (node->data) /* Is file (not dir)? */)
        {
            file->id = (fnet_uint32_t) node; /* save pointer to found dir */
            file->pos = 0u;
            result = FNET_OK;
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION:
*************************************************************************/
static fnet_size_t _fnet_fs_rom_fread (struct fnet_fs_desc *file, void *buf_p, fnet_size_t bytes)
{
    fnet_uint8_t            *buf = (fnet_uint8_t *)buf_p;
    fnet_size_t             result = 0u;
    fnet_fs_rom_node_t      *current;
    fnet_size_t             size;
    fnet_uint32_t           pos;

    if(file && (file->id) && (file->pos != (fnet_uint32_t)FNET_FS_EOF) && buf)
    {
        current = (fnet_fs_rom_node_t *)(file->id);
        if(current && (current->data_size) && (current->data))
        {
            size = current->data_size;
            pos = file->pos;

            if((pos + bytes) > size)
            {
                bytes = size - pos;
                file->pos = (fnet_uint32_t)FNET_FS_EOF;
            }
            else
            {
                file->pos += bytes;
            }

            fnet_memcpy( buf, &current->data[pos], bytes );
            result = bytes;
        }

    }

    return result;
}

/************************************************************************
* DESCRIPTION:
*************************************************************************/
static fnet_return_t _fnet_fs_rom_fseek (struct fnet_fs_desc *file, fnet_int32_t offset, fnet_fs_seek_origin_t origin)
{
    fnet_return_t           result = FNET_ERR;
    fnet_fs_rom_node_t      *current;
    fnet_size_t             size;
    fnet_uint32_t           pos;
    fnet_int32_t            new_pos;

    if(file && (file->id))
    {
        current = (fnet_fs_rom_node_t *)(file->id);
        if(current && (current->data_size))
        {
            size = current->data_size;
            pos = file->pos;

            switch( origin)
            {
                case FNET_FS_SEEK_SET:
                    new_pos = offset;
                    break;
                case FNET_FS_SEEK_CUR:
                    new_pos = (fnet_int32_t)pos + offset;
                    break;
                case FNET_FS_SEEK_END:
                    new_pos = ((fnet_int32_t)size - 1) - offset;
                    break;
                default:
                    new_pos = -1;
                    break;
            }

            if((new_pos > 0) && (new_pos < (fnet_int32_t)size))
            {
                file->pos = (fnet_uint32_t)new_pos;
                result = FNET_OK;
            }
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION:
*************************************************************************/
static fnet_return_t _fnet_fs_rom_finfo (struct fnet_fs_desc *file, fnet_fs_dirent_t *dirent)
{
    fnet_return_t result = FNET_ERR;
    if(file && (file->id) && dirent)
    {
        _fnet_fs_rom_fill_dirent((fnet_fs_rom_node_t *)(file->id), dirent);
        result = FNET_OK;
    }

    return result;
}

#endif
