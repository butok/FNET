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
*  The FNET HTTP server POST method implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_HTTP_SRV && FNET_CFG_HTTP_SRV_POST && FNET_CFG_HTTP_SRV_VERSION_MAJOR

#include "fnet_http_srv_prv.h"
#include "fnet_http_srv_post.h"

/* Prototypes */
static fnet_return_t _fnet_http_srv_post_handle(struct fnet_http_srv_if *http, struct fnet_http_srv_uri *uri);
static fnet_return_t _fnet_http_srv_post_receive(struct fnet_http_srv_if *http);
static fnet_return_t _fnet_http_srv_post_send(struct fnet_http_srv_if *http);

/* POST method. */
const struct fnet_http_srv_method fnet_http_srv_method_post =
{
    .token = "POST",
    .handle = _fnet_http_srv_post_handle,
    .receive = _fnet_http_srv_post_receive,
    .send = _fnet_http_srv_post_send,
    .close = FNET_NULL
};

/************************************************************************
* DESCRIPTION:
************************************************************************/
static fnet_return_t _fnet_http_srv_post_handle(struct fnet_http_srv_if *http, struct fnet_http_srv_uri *uri)
{
    struct fnet_http_srv_session_if *session =  http->session_active;
    fnet_return_t                   result = FNET_ERR;
    const fnet_http_srv_post_t      *post_ptr;

    if(http->post_table)
        /* POST table is initialized.*/
    {
        /* Skip first '/' and ' ' */
        while((*uri->path == '/') || (*uri->path == ' '))
        {
            uri->path++;
        }

        session->data_ptr = 0; /* Clear. */

        /* Find POST function */
        for(post_ptr = http->post_table; post_ptr->name; post_ptr++)
        {
            if (!fnet_strcmp(uri->path, post_ptr->name))
            {
                session->data_ptr = post_ptr;
                if(post_ptr->post_handle)
                {
                    result = post_ptr->post_handle((fnet_http_srv_session_t)session, uri->query, &session->response.cookie);
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
static fnet_return_t _fnet_http_srv_post_receive(struct fnet_http_srv_if *http)
{
    struct fnet_http_srv_session_if *session =  http->session_active;
    fnet_return_t                result = FNET_ERR;
    const fnet_http_srv_post_t   *post_ptr;

    if(session->data_ptr)
    {
        post_ptr = (const fnet_http_srv_post_t *)session->data_ptr;

        if(post_ptr->post_receive)
        {
            result = post_ptr->post_receive((fnet_http_srv_session_t)session, session->buffer, session->buffer_actual_size, &session->response.cookie);
        }
        else
        {
            result = FNET_OK;
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
static fnet_return_t _fnet_http_srv_post_send(struct fnet_http_srv_if *http)
{
    struct fnet_http_srv_session_if *session =  http->session_active;
    fnet_return_t                   result = FNET_ERR;
    const fnet_http_srv_post_t      *post_ptr = (const fnet_http_srv_post_t *) session->data_ptr;

    if(post_ptr && (post_ptr->post_send)
       && ((session->buffer_actual_size = post_ptr->post_send(session->buffer, sizeof(session->buffer), &session->response.send_eof, &session->response.cookie)) > 0u) )
    {
        result = FNET_OK;
    }

    return result;
}

#endif
