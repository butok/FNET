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
* @file fnet_http_cgi.c
*
* @author Andrey Butok
*
* @brief FNET HTTP server  CGI implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_HTTP && FNET_CFG_HTTP_CGI

#include "fnet_http_prv.h"

static fnet_return_t fnet_http_cgi_handle (struct fnet_http_if *http, struct fnet_http_uri *uri);
static fnet_size_t fnet_http_cgi_send (struct fnet_http_if *http);

/************************************************************************
*     Definitions
************************************************************************/

const struct fnet_http_file_handler fnet_http_cgi_handler =
{
    FNET_HTTP_CGI_EXTENSION,
    fnet_http_cgi_handle,
    fnet_http_cgi_send,
    0
};

/************************************************************************
* NAME: fnet_http_cgi_handle
*
* DESCRIPTION:
************************************************************************/
static fnet_return_t fnet_http_cgi_handle (struct fnet_http_if *http, struct fnet_http_uri *uri)
{
    fnet_return_t               result = FNET_ERR;
    const struct fnet_http_cgi  *cgi_ptr;
    struct fnet_http_session_if *session =  http->session_active;

    if(http->cgi_table)
        /* CGI table is initialized.*/
    {
        /* Skip first '/' and ' ' */
        while((*uri->path == '/') || (*uri->path == ' '))
        {
            uri->path++;
        }

        session->send_param.data_ptr = 0; /* Clear. */

        /* Find CGI function */
        for(cgi_ptr = http->cgi_table; cgi_ptr->name; cgi_ptr++)
        {
            if (!fnet_strncmp( uri->path,
                               cgi_ptr->name,
                               fnet_strlen(cgi_ptr->name)))
            {
                session->send_param.data_ptr = cgi_ptr;
                if(cgi_ptr->handle)
                {
                    result = cgi_ptr->handle((fnet_http_session_t)session, uri->query, &session->response.cookie);
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
* NAME: fnet_http_cgi_send
*
* DESCRIPTION:
************************************************************************/
static fnet_size_t fnet_http_cgi_send (struct fnet_http_if *http)
{
    const struct fnet_http_cgi  *cgi_ptr;
    fnet_size_t                 result = 0u;
    struct fnet_http_session_if *session =  http->session_active;

    if(session->send_param.data_ptr)
    {
        cgi_ptr = (const struct fnet_http_cgi *) session->send_param.data_ptr;

        if(cgi_ptr->send)
        {
            result = cgi_ptr->send(session->buffer, sizeof(session->buffer), &session->response.send_eof, &session->response.cookie);
        }
    }

    return result;
}



#endif /* FNET_CFG_HTTP && FNET_CFG_HTTP_CGI */
