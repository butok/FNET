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
* @file fnet_http_post.c
*
* @author Andrey Butok
*
* @brief The FNET HTTP server POST method implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_HTTP && FNET_CFG_HTTP_POST && FNET_CFG_HTTP_VERSION_MAJOR

#include "fnet_http_prv.h"
#include "fnet_http_post.h"


/* Prototypes */
static fnet_return_t fnet_http_post_handle(struct fnet_http_if *http, struct fnet_http_uri *uri);
static fnet_return_t fnet_http_post_receive(struct fnet_http_if *http);
static fnet_return_t fnet_http_post_send(struct fnet_http_if *http);

/* POST method. */
const struct fnet_http_method fnet_http_method_post =
{
    "POST",
    fnet_http_post_handle,
    fnet_http_post_receive,
    fnet_http_post_send,
    0
};

/************************************************************************
* NAME: fnet_http_post_handle
*
* DESCRIPTION:
************************************************************************/
static fnet_return_t fnet_http_post_handle(struct fnet_http_if *http, struct fnet_http_uri *uri)
{
    struct fnet_http_session_if *session =  http->session_active;
    fnet_return_t                result = FNET_ERR;
    const struct fnet_http_post *post_ptr;

    if(http->post_table)
        /* POST table is initialized.*/
    {
        /* Skip first '/' and ' ' */
        while((*uri->path == '/') || (*uri->path == ' '))
        {
            uri->path++;
        }

        session->send_param.data_ptr = 0; /* Clear. */

        /* Find POST function */
        for(post_ptr = http->post_table; post_ptr->name; post_ptr++)
        {
            if (!fnet_strcmp(uri->path, post_ptr->name))
            {
                session->send_param.data_ptr = post_ptr;
                if(post_ptr->post_handle)
                {
                    result = post_ptr->post_handle((fnet_http_session_t)session, uri->query, &session->response.cookie);
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
* NAME: fnet_http_post_receive
*
* DESCRIPTION:
************************************************************************/
static fnet_return_t fnet_http_post_receive(struct fnet_http_if *http)
{
    struct fnet_http_session_if *session =  http->session_active;
    fnet_return_t                result = FNET_ERR;
    const struct fnet_http_post *post_ptr;

    if(session->send_param.data_ptr)
    {
        post_ptr = (const struct fnet_http_post *)session->send_param.data_ptr;

        if(post_ptr->post_receive)
        {
            result = post_ptr->post_receive((fnet_http_session_t)session, session->buffer, session->buffer_actual_size, &session->response.cookie);
        }
        else
        {
            result = FNET_OK;
        }
    }

    return result;
}

/************************************************************************
* NAME: fnet_http_post_send
*
* DESCRIPTION:
************************************************************************/
static fnet_return_t fnet_http_post_send(struct fnet_http_if *http)
{
    struct fnet_http_session_if *session =  http->session_active;
    fnet_return_t               result = FNET_ERR;
    const struct fnet_http_post *post_ptr = (const struct fnet_http_post *) session->send_param.data_ptr;

    if(post_ptr && (post_ptr->post_send)
       && ((session->buffer_actual_size = post_ptr->post_send(session->buffer, sizeof(session->buffer), &session->response.send_eof, &session->response.cookie)) > 0u) )
    {
        result = FNET_OK;
    }

    return result;
}

#endif
