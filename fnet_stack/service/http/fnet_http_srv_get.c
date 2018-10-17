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
*  FNET HTTP Server GET method implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_HTTP_SRV && FNET_CFG_FS

#include "fnet_http_srv_prv.h"

/* Prototypes */
static fnet_return_t _fnet_http_srv_get_handle(struct fnet_http_srv_if *http, struct fnet_http_srv_uri *uri);
static fnet_return_t _fnet_http_srv_get_send(struct fnet_http_srv_if *http);
static void _fnet_http_srv_get_close(struct fnet_http_srv_if *http);

/* GET method. */
const struct fnet_http_srv_method fnet_http_srv_method_get =
{
    .token = "GET",
    .handle = _fnet_http_srv_get_handle,
    .receive = FNET_NULL,
    .send = _fnet_http_srv_get_send,
    .close = _fnet_http_srv_get_close
};

/* The HEAD method.
 It is identical to GET except that the server MUST NOT return a message-body in the response.
 The metainformation contained in the HTTP headers in response to a HEAD request SHOULD be identical to the information sent in response to a GET request. */
const struct fnet_http_srv_method fnet_http_srv_method_head =
{
    .token = "HEAD",
    .handle = _fnet_http_srv_get_handle,
    .receive = FNET_NULL,
    .send = FNET_NULL,
    .close = _fnet_http_srv_get_close
};

/************************************************************************
* DESCRIPTION:
************************************************************************/
static fnet_return_t _fnet_http_srv_get_handle(struct fnet_http_srv_if *http, struct fnet_http_srv_uri *uri)
{
    struct fnet_http_srv_session_if *session =  http->session_active;
    fnet_return_t               result = FNET_ERR;

    /* Request is found */
    if(uri)
    {
        session->response.send_file_handler = _fnet_http_srv_find_handler(http, uri);           /* Find file handler.*/

#if FNET_CFG_HTTP_SRV_VERSION_MAJOR
        session->response.send_file_content_type = _fnet_http_srv_find_content_type(http, uri); /* Find file content-type.*/
#endif

        result = session->response.send_file_handler->file_handle(http, uri);              /* Initial handling. */
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Simple-Response. Simple-responce consists only of the
* entity body and is terminated by the server closing connection.
************************************************************************/
static fnet_return_t _fnet_http_srv_get_send(struct fnet_http_srv_if *http)
{
    struct fnet_http_srv_session_if *session =  http->session_active;
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
* DESCRIPTION:
************************************************************************/
static void _fnet_http_srv_get_close(struct fnet_http_srv_if *http)
{
    struct fnet_http_srv_session_if *session =  http->session_active;

    if((session->response.send_file_handler) && (session->response.send_file_handler->file_close))
    {
        session->response.send_file_handler->file_close(http);
    }
}

#endif
