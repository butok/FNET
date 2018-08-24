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
*  FNET HTTP Server SSI implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_HTTP_SRV && FNET_CFG_HTTP_SRV_SSI

#include "fnet_http_srv_prv.h"
#include "fnet_http_srv_ssi_prv.h"

/************************************************************************
*     Definitions
************************************************************************/
static fnet_size_t _fnet_http_srv_ssi_send (struct fnet_http_srv_if *http);

#if FNET_CFG_HTTP_SRV_VERSION_MAJOR /* HTTP/1.x*/
    static fnet_return_t _fnet_http_srv_ssi_handle (struct fnet_http_srv_if *http, struct fnet_http_srv_uri *uri);
#endif

static const fnet_uint8_t fnet_http_srv_ssi_head[] = {'<', '!', '-', '-', '#'};
static const fnet_uint8_t fnet_http_srv_ssi_tail[] = {'-', '-', '>'};

const struct fnet_http_srv_file_handler fnet_http_srv_ssi_handler =
{
    .file_extension = FNET_HTTP_SRV_SSI_EXTENSION,
#if FNET_CFG_HTTP_SRV_VERSION_MAJOR /* HTTP/1.x*/
    .file_handle = _fnet_http_srv_ssi_handle,
#else
    .file_handle = _fnet_http_srv_default_handle,
#endif
    .file_send = _fnet_http_srv_ssi_send,
    .file_close = _fnet_http_srv_default_close
};

#if FNET_CFG_HTTP_SRV_VERSION_MAJOR /* HTTP/1.x*/
/************************************************************************
* DESCRIPTION:
************************************************************************/
static fnet_return_t _fnet_http_srv_ssi_handle (struct fnet_http_srv_if *http, struct fnet_http_srv_uri *uri)
{
    fnet_return_t     result = _fnet_http_srv_default_handle (http, uri);
    http->session_active->response.content_length = -1; /* No content length.*/
    return result;
}
#endif

/************************************************************************
* DESCRIPTION:
************************************************************************/
static fnet_size_t _fnet_http_srv_ssi_send (struct fnet_http_srv_if *http)
{
    fnet_size_t                 result = 0u;
    fnet_size_t                 read_result = 0u;
    fnet_index_t                ssi_head_index = 0u;
    fnet_index_t                ssi_tail_index = 0u;
    struct fnet_http_srv_session_if *session =  http->session_active;
    fnet_uint8_t                *buffer = session->buffer;
    fnet_bool_t                 next = FNET_FALSE;

    while ((result < sizeof(session->buffer)) && (next == FNET_FALSE))
    {
        if((http->ssi.state != FNET_HTTP_SRV_SSI_INCLUDING) /* Read from file if not in including. */
           && ((read_result = fnet_fs_fread(buffer, 1u, session->file_desc)) == 0u) )
        {
            break; /*EOF*/
        }

        switch (http->ssi.state)
        {
            case FNET_HTTP_SRV_SSI_WAIT_HEAD:
                if(*buffer == fnet_http_srv_ssi_head[ssi_head_index])
                {
                    ssi_head_index++;
                    if(ssi_head_index == sizeof(fnet_http_srv_ssi_head))
                    {
                        /* Head is found */

                        if(result >= sizeof(fnet_http_srv_ssi_head))
                        {
                            /* Found in the middle */
                            fnet_fs_fseek (session->file_desc, -((fnet_int32_t)sizeof(fnet_http_srv_ssi_head)), FNET_FS_SEEK_CUR);
                            next = FNET_TRUE; /* break */
                            result -= sizeof(fnet_http_srv_ssi_head); /* Correct result */
                        }
                        else
                        {
                            http->ssi.state = FNET_HTTP_SRV_SSI_WAIT_TAIL;
                        }
                    }
                }
                else
                {
                    ssi_head_index = 0u;
                }
                break;

            case FNET_HTTP_SRV_SSI_WAIT_TAIL:
                if(*buffer == fnet_http_srv_ssi_tail[ssi_tail_index])
                {
                    ssi_tail_index++;
                    if(ssi_tail_index == sizeof(fnet_http_srv_ssi_tail))
                    {
                        /* Tail is found */
                        const fnet_http_srv_ssi_t  *ssi_ptr = 0;
                        fnet_char_t                *ssi_name = (fnet_char_t *)&session->buffer[sizeof(fnet_http_srv_ssi_head)];
                        fnet_char_t                *ssi_param;

                        http->ssi.send = 0;

                        session->buffer[buffer + 1 - session->buffer - sizeof(fnet_http_srv_ssi_tail)] = '\0'; /* Mark end of the SSI. */

                        /* Find SSI parameters. */
                        if((ssi_param = fnet_strchr( (fnet_char_t *)session->buffer, ' ' )) != 0)
                        {
                            *ssi_param = '\0';  /* Mark end of the SSI name. */
                            ssi_param ++;       /* Point to the begining of params. */
                        }

                        if(http->ssi.ssi_table)
                            /* SSI table is initialized.*/
                        {
                            /* Find SSI handler */
                            for(ssi_ptr = http->ssi.ssi_table; (ssi_ptr->name) && (ssi_ptr->send); ssi_ptr++)
                            {
                                if (!fnet_strcmp( ssi_name,
                                                  ssi_ptr->name))
                                {
                                    http->ssi.send = ssi_ptr->send;
                                    break;
                                }
                            }
                        }

                        read_result = 0u; /* Eliminate the include. */
                        if(http->ssi.send)
                        {
                            /* SSI Handler is found. */
                            if((ssi_ptr->handle == 0) || (ssi_ptr->handle(ssi_param, &session->response.cookie) == FNET_OK))
                            {
                                buffer = session->buffer; /* Reset */
                                result = 0u;

                                http->ssi.state = FNET_HTTP_SRV_SSI_INCLUDING;
                            }
                            else
                            {
                                http->ssi.state = FNET_HTTP_SRV_SSI_WAIT_HEAD;
                            }
                        }
                        else
                        {
                            http->ssi.state = FNET_HTTP_SRV_SSI_WAIT_HEAD;
                        }
                    }
                }
                else
                {
                    ssi_tail_index = 0u;
                }
                break;
            case FNET_HTTP_SRV_SSI_INCLUDING:
            {
                fnet_bool_t eof;
                read_result = http->ssi.send(session->buffer, sizeof(session->buffer), &eof, &session->response.cookie);
                if((read_result == 0u) || (eof == FNET_TRUE))
                {
                    http->ssi.state = FNET_HTTP_SRV_SSI_WAIT_HEAD;
                }

                next = FNET_TRUE; /* break */
            }
            break;
            default:
                break;
        }
        buffer += read_result;
        result += read_result;
    }

    if(read_result && (next == FNET_FALSE) && ssi_head_index && (http->ssi.state == FNET_HTTP_SRV_SSI_WAIT_HEAD) )
    {
        /* Potential SSI is splitted => parse in the next itteration */
        result -= ssi_head_index; /* adjust result */
        fnet_fs_fseek(session->file_desc, -(fnet_int32_t)ssi_head_index, FNET_FS_SEEK_CUR);
    }

    return result;
}

#endif /* FNET_CFG_HTTP_SRV && FNET_CFG_HTTP_SRV_SSI */
