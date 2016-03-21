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
* @file fnet_http_ssi_prv.h
*
* @author Andrey Butok
*
* @brief Private. FNET HTTP Server SSI API.
*
***************************************************************************/

#ifndef _FNET_HTTP_SSI_PRV_H_

#define _FNET_HTTP_SSI_PRV_H_

#include "fnet.h"

#if FNET_CFG_HTTP && FNET_CFG_HTTP_SSI

/* SSI statemachine state. */
typedef enum
{
    FNET_HTTP_SSI_WAIT_HEAD = 0,
    FNET_HTTP_SSI_WAIT_TAIL,
    FNET_HTTP_SSI_INCLUDING
}
fnet_http_ssi_state_t;

/* SSI private control structure. */
struct fnet_http_ssi_if
{
    const struct fnet_http_ssi *ssi_table; /* Pointer to the SSI table.*/
    fnet_http_ssi_send_t send;    /* Pointer to the respond callback.*/
    fnet_http_ssi_state_t state;        /* State. */
};

extern const struct fnet_http_file_handler fnet_http_ssi_handler; /* SSI file handler. */


#endif


#endif
