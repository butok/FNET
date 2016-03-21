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
* @file fapp_http.h
*
* @author Andrey Butok
*
* @brief FNET Shell Demo (HTTP Server).
*
***************************************************************************/


/************************************************************************
*     Private Definitions.
*************************************************************************/
#ifndef _FAPP_HTTP_H_

#define _FAPP_HTTP_H_

#if FAPP_CFG_HTTP_CMD

#define FAPP_HTTP_MOUNT_NAME   FAPP_FS_MOUNT_NAME
#define FAPP_HTTP_INDEX_FILE   "index.html"

#if defined(__cplusplus)
extern "C" {
#endif

void fapp_http_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
void fapp_http_info(fnet_shell_desc_t desc);
void fapp_http_release(void);

#if defined(__cplusplus)
}
#endif

#endif

#endif
