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
*  File System API Implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_FS

#include "fnet_fs_prv.h"
#include "fnet_fs_root.h"

/* Mount point list */
struct fnet_fs_mount_point fnet_fs_mount_list[FNET_CFG_FS_MOUNT_MAX];

/* File&dir descriptor list */
static struct fnet_fs_desc fnet_fs_desc_list[FNET_CFG_FS_DESC_MAX];

/* The list of registered FSs. */
static struct fnet_fs *fnet_fs_list;

static struct fnet_fs_mount_point *_fnet_fs_find_mount( const fnet_char_t **name );

/************************************************************************
* DESCRIPTION: This function initializes FNET FS interface.
*************************************************************************/
fnet_return_t fnet_fs_init( void )
{
    fnet_return_t result = FNET_ERR;

    if(fnet_fs_list == 0) /* If no init before. */
    {
        _fnet_fs_root_register();
        result = fnet_fs_mount( FNET_FS_ROOT_NAME, "", 0 );
    }

    return result;
}

/************************************************************************
* DESCRIPTION: This function releases FNET FS interface.
*************************************************************************/
void fnet_fs_release( void )
{
    fnet_fs_unmount("");
    _fnet_fs_root_unregister();

    /* Clear the rest. */
    fnet_memset_zero( fnet_fs_mount_list, sizeof(struct fnet_fs_mount_point)*FNET_CFG_FS_MOUNT_MAX);
    fnet_memset_zero( fnet_fs_desc_list, sizeof(struct fnet_fs_desc)*FNET_CFG_FS_DESC_MAX);
    fnet_fs_list = 0;
}

/************************************************************************
* DESCRIPTION: This function registers a FS.
*************************************************************************/
void _fnet_fs_register( struct fnet_fs *fs )
{
    if(fs)
    {
        fs->_next = fnet_fs_list;

        if(fs->_next != 0)
        {
            fs->_next->_prev = fs;
        }

        fs->_prev = 0;
        fnet_fs_list = fs;
    }
}

/************************************************************************
* DESCRIPTION: This function unregisters a FS.
*************************************************************************/
void _fnet_fs_unregister( struct fnet_fs *fs )
{
    if(fs)
    {
        fnet_service_mutex_lock();
        if(fs->_prev == 0)
        {
            fnet_fs_list = fs->_next;
        }
        else
        {
            fs->_prev->_next = fs->_next;
        }

        if(fs->_next != 0)
        {
            fs->_next->_prev = fs->_prev;
        }
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: Returns a FS given its name.
*************************************************************************/
struct fnet_fs *_fnet_fs_find_name( fnet_char_t *name )
{
    struct fnet_fs *fs;
    struct fnet_fs *result = 0;

    if(name)
    {
        for (fs = fnet_fs_list; fs != 0; fs = fs->_next)
        {
            if(fnet_strcmp(name, fs->name) == 0)
            {
                result = fs;
                break;
            }
        }
    }
    return result;
}

/************************************************************************
* DESCRIPTION: Mounts a FS.
*************************************************************************/
fnet_return_t fnet_fs_mount( fnet_char_t *fs_name, const fnet_char_t *mount_name, const void *arg )
{
    fnet_return_t   result = FNET_ERR;
    struct          fnet_fs_mount_point *mount_point = 0;
    struct          fnet_fs *fs;
    fnet_index_t    i;

    if(fs_name && mount_name)
    {
        fnet_service_mutex_lock();

        fs = _fnet_fs_find_name(fs_name);
        if(fs)
        {
            for(i = 0U; i < FNET_CFG_FS_MOUNT_MAX; i++)
            {
                if(fnet_fs_mount_list[i].fs == 0)
                {
                    mount_point = &fnet_fs_mount_list[i]; /* found free mount point */
                    break;
                }
            }

            if(mount_point)
            {
                if((fs->operations) && (fs->operations->mount))
                {
                    result = fs->operations->mount(arg);
                }
                else
                {
                    result = FNET_OK;
                }

                if(result == FNET_OK) /* Mount is successful */
                {
                    mount_point->arg = arg; /* Fill mount info structure. */
                    mount_point->fs = fs;
                    fnet_strlcpy( mount_point->name, mount_name, FNET_CFG_FS_MOUNT_NAME_MAX + 1U );
                }
            }
        }

        fnet_service_mutex_unlock();
    }

    return result;
}

/****************************************************************/
fnet_int32_t _fnet_fs_path_cmp( const fnet_char_t **path, const fnet_char_t *name)
{
    fnet_int32_t        result;

    /* No checks for 0 */
    const fnet_char_t   *s1p = *path;
    const fnet_char_t   *s2p = name;

    while(*s1p == ' ')
    {
        s1p++;	            /* Strip leading spaces */
    }
    while(*s1p == FNET_FS_SPLITTER)
    {
        s1p++;	/* Strip heading slash */
    }

    while((*s2p != '\0') && (*s1p == *s2p) )
    {
        ++s1p;
        ++s2p;

        if (*s1p == FNET_FS_SPLITTER)
        {
            break; /* next path element */
        }
    }

    if(*s1p == FNET_FS_SPLITTER)
    {
        result = 0;
    }
    else
    {
        result = (fnet_int32_t)(*s1p) - (fnet_int32_t)(*s2p);
    }

    if(result == 0) /* Save end of compare */
    {
        *path = s1p;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Find mount point given its name and remove mount name from
*               the path.
*************************************************************************/
static struct fnet_fs_mount_point *_fnet_fs_find_mount( const fnet_char_t **name )
{
    struct fnet_fs_mount_point  *result = 0;
    struct fnet_fs_mount_point  *tmp;
    fnet_index_t                i;

    if(name && (*name))
    {
        for(i = 0U; i < FNET_CFG_FS_MOUNT_MAX; i++)
        {
            tmp = &fnet_fs_mount_list[i];
            if((tmp->fs) && (_fnet_fs_path_cmp(name, tmp->name) == 0))
            {
                result = tmp;
                break;
            }
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Unmounts a FS.
*************************************************************************/
fnet_return_t fnet_fs_unmount( const fnet_char_t *mount_name )
{
    fnet_return_t               result = FNET_ERR;
    struct fnet_fs_mount_point  *mount_point;
    struct fnet_fs              *fs;

    if(mount_name)
    {
        fnet_service_mutex_lock();
        mount_point = _fnet_fs_find_mount(&mount_name);
        if(mount_point)
        {
            fs = mount_point->fs;

            if(fs && (fs->operations) && (fs->operations->unmount) )
            {
                fs->operations->unmount(mount_point->arg);
            }

            fnet_memset_zero( mount_point, sizeof(struct fnet_fs_mount_point) );
            result = FNET_OK;
        }
        fnet_service_mutex_unlock();
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Open DIR stream.
*************************************************************************/
fnet_fs_dir_t fnet_fs_opendir( const fnet_char_t *dirname)
{
    fnet_fs_dir_t               result = 0;
    fnet_index_t                i;
    struct fnet_fs_desc         *dir = 0;
    struct fnet_fs_mount_point  *mount_point;

    if(dirname)
    {
        fnet_service_mutex_lock();
        for(i = 0U; i < FNET_CFG_FS_DESC_MAX; i++) /* Free descriptor? */
        {
            if(fnet_fs_desc_list[i].id == 0u)
            {
                dir = &fnet_fs_desc_list[i]; /* found free DIR descriptor */
                break;
            }
        }

        if(dir) /* Found free descriptor. */
        {
            mount_point = _fnet_fs_find_mount(&dirname);
            if(mount_point && (mount_point->fs) && (mount_point->fs->dir_operations)
               && (mount_point->fs->dir_operations->opendir))
            {
                dir->mount = mount_point;
                if(mount_point->fs->dir_operations->opendir(dir, dirname) == FNET_OK)
                {
                    result = dir;
                }
                else
                {
                    fnet_memset_zero( dir, sizeof(struct fnet_fs_desc) ); /* clear dir structure */
                }
            }
        }
        fnet_service_mutex_unlock();
    }
    return result;
}

/************************************************************************
* DESCRIPTION: Close DIR stream.
*************************************************************************/
fnet_return_t fnet_fs_closedir( fnet_fs_dir_t dir)
{
    fnet_return_t       result = FNET_ERR;
    struct fnet_fs_desc *dirp = (struct fnet_fs_desc *)dir;

    if(dirp)
    {
        fnet_service_mutex_lock();
        fnet_memset_zero( dirp, sizeof(struct fnet_fs_desc) ); /* clear dir structure */
        fnet_service_mutex_unlock();

        result = FNET_OK;
    }
    return result;
}

/************************************************************************
* DESCRIPTION: Returns a pointer to a structure representing the directory
* entry.
*************************************************************************/
fnet_return_t fnet_fs_readdir(fnet_fs_dir_t dir, fnet_fs_dirent_t *dirent)
{
    fnet_return_t       result = FNET_ERR;
    struct fnet_fs_desc *dirp = (struct fnet_fs_desc *) dir;

    if(dirp)
    {
        if((dirp->mount) && (dirp->mount->fs)
           && (dirp->mount->fs->dir_operations)
           && (dirp->mount->fs->dir_operations->readdir))
        {
            fnet_service_mutex_lock();
            result = dirp->mount->fs->dir_operations->readdir(dirp, dirent);
            fnet_service_mutex_unlock();
        }
    }
    return result;
}

/************************************************************************
* DESCRIPTION: Resets the position of the directory stream.
*************************************************************************/
void fnet_fs_rewinddir( fnet_fs_dir_t dir )
{
    struct fnet_fs_desc *dirp = (struct fnet_fs_desc *) dir;

    if(dirp)
    {
        fnet_service_mutex_lock();
        /* Reset current index. */
        dirp->pos = 0U;
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: Opens the specified file.
*************************************************************************/
fnet_fs_file_t fnet_fs_fopen(const fnet_char_t *filename, const fnet_char_t *mode)
{
    fnet_fs_file_t result;

    result = fnet_fs_fopen_re(filename, mode, 0 );

    return result;
}

/************************************************************************
* DESCRIPTION: Opens the specified file relative to.
*************************************************************************/
fnet_fs_file_t fnet_fs_fopen_re(const fnet_char_t *filename, const fnet_char_t *mode, fnet_fs_dir_t dir )
{
    fnet_uint8_t                mode_in = 0U;
    fnet_fs_file_t              result = 0;
    fnet_index_t                i;
    struct fnet_fs_desc         *file = 0;
    struct fnet_fs_mount_point  *mount_point;
    struct fnet_fs_desc         *cur_dir = (struct fnet_fs_desc *) dir;

    if(filename && mode)
    {
        fnet_service_mutex_lock();
        /* Parse the file mode. */
        switch(*mode)
        {
            case 'r':
                mode_in = FNET_FS_MODE_READ | FNET_FS_MODE_OPEN_EXISTING;
                break;
            case 'w':
                mode_in = FNET_FS_MODE_WRITE | FNET_FS_MODE_OPEN_ALWAYS | FNET_FS_MODE_OPEN_TRUNC;
                break;
            case 'a':
                mode_in = FNET_FS_MODE_WRITE | FNET_FS_MODE_OPEN_ALWAYS | FNET_FS_MODE_END;
                break;
            default:
                break;
        }

        if(mode_in)         /* Is mode correct? */
        {
            mode++;

            if(*mode == 'b')    /* Text and binary is the same for us. */
            {
                mode++;
            }

            if(*mode == '+')
            {
                mode_in |= FNET_FS_MODE_READ | FNET_FS_MODE_WRITE;
            }

            for(i = 0u; i < FNET_CFG_FS_DESC_MAX; i++) /* Free descriptor? */
            {
                if(fnet_fs_desc_list[i].id == 0u)
                {
                    file = &fnet_fs_desc_list[i]; /* found free DIR descriptor */
                    break;
                }
            }

            if(file) /* Found free descriptor. */
            {
                if(cur_dir)
                {
                    mount_point = cur_dir->mount;
                }
                else
                {
                    mount_point = _fnet_fs_find_mount(&filename);
                }

                if(mount_point && (mount_point->fs)
                   && (mount_point->fs->file_operations)
                   && (mount_point->fs->file_operations->fopen))
                {
                    file->mount = mount_point;
                    if(mount_point->fs->file_operations->fopen(file, filename, mode_in, cur_dir) == FNET_OK)
                    {
                        result = file;
                    }
                    else
                    {
                        fnet_memset_zero( file, sizeof(struct fnet_fs_desc) ); /* clear file structure */
                    }
                }
            }
        }

        fnet_service_mutex_unlock();
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Close DIR stream.
*************************************************************************/
fnet_return_t fnet_fs_fclose( fnet_fs_file_t file)
{
    fnet_return_t       result = FNET_ERR;
    struct fnet_fs_desc *filep = (struct fnet_fs_desc *)file;

    if(filep)
    {
        fnet_service_mutex_lock();
        fnet_memset_zero( filep, sizeof(struct fnet_fs_desc) ); /* clear file structure */
        fnet_service_mutex_unlock();

        result = FNET_OK;
    }
    return result;
}

/************************************************************************
* DESCRIPTION: Resets the position of the file stream.
*************************************************************************/
void fnet_fs_rewind(fnet_fs_file_t file)
{
    struct fnet_fs_desc *filep = (struct fnet_fs_desc *) file;

    if(filep)
    {
        fnet_service_mutex_lock();
        /* Reset current pos. */
        filep->pos = 0u;
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION:
*************************************************************************/
fnet_size_t fnet_fs_fread(void *buf, fnet_size_t size, fnet_fs_file_t file)
{
    fnet_size_t         result = 0u;
    struct fnet_fs_desc *filep = (struct fnet_fs_desc *) file;
    fnet_size_t         bytes = size;

    if(filep && bytes && buf)
    {
        if((filep->mount) && (filep->mount->fs)
           && (filep->mount->fs->file_operations)
           && (filep->mount->fs->file_operations->fread))
        {
            fnet_service_mutex_lock();
            result = filep->mount->fs->file_operations->fread(filep, buf, bytes);
            fnet_service_mutex_unlock();
        }
    }
    return result;
}

/************************************************************************
* DESCRIPTION: A nonzero value signifies that the end of the file has been
*               reached; a value of zero signifies that it has not.
* In virtually all cases, there's no need to use feof at all.
*(feof, or more likely ferror, may be useful after a stdio call has
* returned EOF or NULL, to distinguish between an end-of-file condition
* and a read error.
*************************************************************************/
fnet_bool_t fnet_fs_feof(fnet_fs_file_t file)
{
    fnet_bool_t         result = FNET_FALSE;
    struct fnet_fs_desc *filep = (struct fnet_fs_desc *) file;

    if(filep && (filep->pos == (fnet_uint32_t)FNET_FS_EOF))
    {
        result = FNET_TRUE;
    }
    return result;
}

/************************************************************************
* DESCRIPTION: If successful, fgetc returns the next byte or character from the stream
*  If unsuccessful, fgetc returns EOF.
*************************************************************************/
fnet_int32_t fnet_fs_fgetc(fnet_fs_file_t file)
{
    fnet_int32_t        result = FNET_FS_EOF;
    struct fnet_fs_desc *filep = (struct fnet_fs_desc *) file;
    fnet_uint8_t        buf;

    if(filep)
    {
        if((filep->mount) && (filep->mount->fs)
           && (filep->mount->fs->file_operations)
           && (filep->mount->fs->file_operations->fread))
        {
            fnet_service_mutex_lock();
            if(filep->mount->fs->file_operations->fread(filep, &buf, 1u) != 0u)
            {
                result = (fnet_int32_t)buf;
            }
            fnet_service_mutex_unlock();
        }
    }
    return result;
}

/************************************************************************
* DESCRIPTION: Change the file position indicator for the specified file.
*************************************************************************/
fnet_return_t fnet_fs_fseek (fnet_fs_file_t file, fnet_int32_t offset, fnet_fs_seek_origin_t origin)
{
    fnet_return_t       result = FNET_ERR;
    struct fnet_fs_desc *filep = (struct fnet_fs_desc *) file;

    if(filep)
    {
        if((filep->mount) && (filep->mount->fs)
           && (filep->mount->fs->file_operations)
           && (filep->mount->fs->file_operations->fseek))
        {
            fnet_service_mutex_lock();
            result = filep->mount->fs->file_operations->fseek(filep, offset, origin);
            fnet_service_mutex_unlock();
        }
    }
    return result;
}

/************************************************************************
* DESCRIPTION: Returns the current offset in a stream in relation to the first byte.
*************************************************************************/
fnet_int32_t fnet_fs_ftell (fnet_fs_file_t file)
{
    fnet_int32_t            result = FNET_ERR;
    struct fnet_fs_desc     *filep = (struct fnet_fs_desc *) file;

    if(filep)
    {
        fnet_service_mutex_lock();
        result = (fnet_int32_t) filep->pos;
        fnet_service_mutex_unlock();
    }
    return result;
}

/************************************************************************
* DESCRIPTION: Reads file info data.
*************************************************************************/
fnet_return_t fnet_fs_finfo (fnet_fs_file_t file, fnet_fs_dirent_t *dirent)
{
    fnet_return_t       result = FNET_ERR;
    struct fnet_fs_desc *filep = (struct fnet_fs_desc *) file;

    if(filep)
    {
        if((filep->mount) && (filep->mount->fs)
           && (filep->mount->fs->file_operations)
           && (filep->mount->fs->file_operations->finfo))
        {
            fnet_service_mutex_lock();
            result = filep->mount->fs->file_operations->finfo(filep, dirent);
            fnet_service_mutex_unlock();
        }
    }
    return result;
}

#endif /* FNET_CFG_FS */
