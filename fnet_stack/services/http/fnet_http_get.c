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
* @file fnet_http_get.c
*
* @author Andrey Butok
*
* @brief FNET HTTP Server GET method implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_HTTP && FNET_CFG_FS

#include "fnet_http_prv.h"

/* Prototypes */
static fnet_return_t fnet_http_get_handle(struct fnet_http_if *http, struct fnet_http_uri *uri);
static fnet_return_t fnet_http_get_send(struct fnet_http_if *http);
static void fnet_http_get_close(struct fnet_http_if *http);

/* GET method. */
const struct fnet_http_method fnet_http_method_get =
{
    "GET",
    fnet_http_get_handle,
    0,
    fnet_http_get_send,
    fnet_http_get_close
};

/************************************************************************
* NAME: fnet_http_get_handle
*
* DESCRIPTION:
************************************************************************/
static fnet_return_t fnet_http_get_handle(struct fnet_http_if *http, struct fnet_http_uri *uri)
{
    struct fnet_http_session_if *session =  http->session_active;
    fnet_return_t               result = FNET_ERR;

    /* Request is found */
    if(uri)
    {
        session->response.send_file_handler = fnet_http_find_handler(http, uri);           /* Find file handler.*/

#if FNET_CFG_HTTP_VERSION_MAJOR
        session->response.send_file_content_type = fnet_http_find_content_type(http, uri); /* Find file content-type.*/
#endif

        result = session->response.send_file_handler->file_handle(http, uri);              /* Initial handling. */
    }

    return result;
}

/************************************************************************
* NAME: fnet_http_get_send
*
* DESCRIPTION: Simple-Response. Simple-responce consists only of the
* entity body and is terminated by the server closing connection.
************************************************************************/
static fnet_return_t fnet_http_get_send(struct fnet_http_if *http)
{
    struct fnet_http_session_if *session =  http->session_active;
    fnet_return_t               result;

    if((session->buffer_actual_size = session->response.send_file_handler->file_send(http)) > 0u)
    {
        result = FNET_OK;
    }
    else
    {
        result = FNET_ERR;
    }

    return result;
}

/************************************************************************
* NAME: fnet_http_get_close
*
* DESCRIPTION:
************************************************************************/
static void fnet_http_get_close(struct fnet_http_if *http)
{
    struct fnet_http_session_if *session =  http->session_active;

    if((session->response.send_file_handler) && (session->response.send_file_handler->file_close))
    {
        session->response.send_file_handler->file_close(http);
    }
}

#endif
