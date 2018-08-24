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
*  Root FS Implementation.
*
***************************************************************************/
#include "fnet.h"

#if FNET_CFG_FS

#include "fnet_fs_prv.h"
#include "fnet_fs_root.h"


#define FNET_FS_ROOTDIR_ID          ((fnet_uint32_t)(-1))   /* Root dir ID */

/* Root FS */

static fnet_return_t _fnet_fs_root_opendir( struct fnet_fs_desc *dir, const fnet_char_t *name);
static fnet_return_t _fnet_fs_root_readdir(struct fnet_fs_desc *dir, fnet_fs_dirent_t *dirent);

static const struct fnet_fs_dir_operations fnet_fs_root_dir_operations =
{
    _fnet_fs_root_opendir,
    _fnet_fs_root_readdir
};

static struct fnet_fs fnet_fs_root =
{
    FNET_FS_ROOT_NAME,
    0,
    0,
    &fnet_fs_root_dir_operations,   /* fs directory operations.*/
    0,
    0
};


static fnet_bool_t fnet_fs_root_registered;  /* Flag that ROM FS is registered or not.*/

/************************************************************************
* DESCRIPTION:
*************************************************************************/
void _fnet_fs_root_register( void )
{
    if(fnet_fs_root_registered == FNET_FALSE)
    {
        _fnet_fs_register(&fnet_fs_root);
        fnet_fs_root_registered = FNET_TRUE;
    }
}

/************************************************************************
* DESCRIPTION:
*************************************************************************/
void _fnet_fs_root_unregister( void )
{
    if(fnet_fs_root_registered == FNET_TRUE)
    {
        _fnet_fs_unregister(&fnet_fs_root);
        fnet_fs_root_registered = FNET_FALSE;
    }
}

/************************************************************************
* DESCRIPTION: Open DIR stream for the ROOT FS.
*************************************************************************/
static fnet_return_t _fnet_fs_root_opendir( struct fnet_fs_desc *dir, const fnet_char_t *name)
{
    fnet_return_t result = FNET_ERR;

    FNET_COMP_UNUSED_ARG(name);

    if(dir)
    {
        dir->id = FNET_FS_ROOTDIR_ID;
        result = FNET_OK;
    }
    else
    {
        result = FNET_ERR;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Read DIR stream for the ROOT FS.
*************************************************************************/
static fnet_return_t _fnet_fs_root_readdir(struct fnet_fs_desc *dir, fnet_fs_dirent_t *dirent)
{
    fnet_return_t               result = FNET_ERR;
    fnet_index_t                i;
    struct fnet_fs_mount_point  *tmp;

    if(dir && (dir->id == FNET_FS_ROOTDIR_ID) && (dir->pos != (fnet_uint32_t)FNET_FS_EOF) && dirent)
    {
        for(i = dir->pos; i < FNET_CFG_FS_MOUNT_MAX; i++)
        {
            tmp = &fnet_fs_mount_list[i];
            if(tmp->fs) /* Found next mount - dir */
            {
                dir->pos = i + 1u; /* incriment next dir index */
                if(fnet_strcmp(tmp->fs->name, FNET_FS_ROOT_NAME ) )/* It's not ROOT FS mount. */
                {
                    /*fill ident */
                    dirent->d_ino = (fnet_uint32_t) tmp; /* File serial number. */
                    dirent->d_type = DT_DIR;
                    dirent->d_name = tmp->name;
                    dirent->d_size = 0u;
                    result = FNET_OK;
                    break;
                }
            }
        }
        if (result == FNET_ERR)
        {
            dir->pos = (fnet_uint32_t)FNET_FS_EOF; /* End of the directory is encountered */
        }
    }

    return result;
}

#endif /* FNET_CFG_FS */
