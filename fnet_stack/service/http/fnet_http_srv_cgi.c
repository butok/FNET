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
*  FNET HTTP server  CGI implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_HTTP_SRV && FNET_CFG_HTTP_SRV_CGI

#include "fnet_http_srv_prv.h"

static fnet_return_t _fnet_http_srv_cgi_handle (struct fnet_http_srv_if *http, struct fnet_http_srv_uri *uri);
static fnet_size_t _fnet_http_srv_cgi_send (struct fnet_http_srv_if *http);

/************************************************************************
*     Definitions
************************************************************************/
const struct fnet_http_srv_file_handler fnet_http_srv_cgi_handler =
{
    .file_extension = FNET_HTTP_SRV_CGI_EXTENSION,
    .file_handle = _fnet_http_srv_cgi_handle,
    .file_send = _fnet_http_srv_cgi_send,
    .file_close = 0
};

/************************************************************************
* DESCRIPTION:
************************************************************************/
static fnet_return_t _fnet_http_srv_cgi_handle (struct fnet_http_srv_if *http, struct fnet_http_srv_uri *uri)
{
    fnet_return_t               result = FNET_ERR;
    const fnet_http_srv_cgi_t  *cgi_ptr;
    struct fnet_http_srv_session_if *session =  http->session_active;

    if(http->cgi_table)
        /* CGI table is initialized.*/
    {
        /* Skip first '/' and ' ' */
        while((*uri->path == '/') || (*uri->path == ' '))
        {
            uri->path++;
        }

        session->data_ptr = 0; /* Clear. */

        /* Find CGI function */
        for(cgi_ptr = http->cgi_table; cgi_ptr->name; cgi_ptr++)
        {
            if (!fnet_strncmp( uri->path,
                               cgi_ptr->name,
                               fnet_strlen(cgi_ptr->name)))
            {
                session->data_ptr = cgi_ptr;
                if(cgi_ptr->handle)
                {
                    result = cgi_ptr->handle((fnet_http_srv_session_t)session, uri->query, &session->response.cookie);
                }
                else
                {
                    result = FNET_OK;
                }

                break;
            }
        }
    }
    return result;
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
static fnet_size_t _fnet_http_srv_cgi_send (struct fnet_http_srv_if *http)
{
    const fnet_http_srv_cgi_t  *cgi_ptr;
    fnet_size_t                 result = 0u;
    struct fnet_http_srv_session_if *session =  http->session_active;

    if(session->data_ptr)
    {
        cgi_ptr = (const fnet_http_srv_cgi_t *) session->data_ptr;

        if(cgi_ptr->send)
        {
            result = cgi_ptr->send(session->buffer, sizeof(session->buffer), &session->response.send_eof, &session->response.cookie);
        }
    }

    return result;
}

#endif /* FNET_CFG_HTTP_SRV && FNET_CFG_HTTP_SRV_CGI */
