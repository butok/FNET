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
* @file fapp_fs.h
*
* @author Andrey Butok
*
* @brief FNET Shell Demo (File System Explorer).
*
***************************************************************************/

#ifndef _FAPP_FS_H_
#define _FAPP_FS_H_

#include "fapp.h"

/************************************************************************
*     File System Image
*************************************************************************/
#if FNET_CFG_FS && FNET_CFG_FS_ROM
    extern const struct fnet_fs_rom_image fnet_fs_image;
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#if FAPP_CFG_EXP_CMD || FAPP_CFG_HTTP_CMD

#define FAPP_FS_MOUNT_NAME       "rom"

void fapp_fs_mount(void);
void fapp_fs_unmount(void);
#endif

#if FAPP_CFG_EXP_CMD
extern const struct fnet_shell fapp_fs_shell;
#endif

#if defined(__cplusplus)
}
#endif

#endif
