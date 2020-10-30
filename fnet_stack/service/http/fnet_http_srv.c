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
*  FNET HTTP Server implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_HTTP_SRV

#include "fnet_http_srv_prv.h"

#if FNET_CFG_HTTP_SRV_AUTHENTICATION_BASIC && FNET_CFG_HTTP_SRV_VERSION_MAJOR
    #include "fnet_http_srv_auth_prv.h"
#endif

#if FNET_CFG_FS == 0
    #error "FNET_CFG_FS must be defined for HTTP server"
#endif

/************************************************************************
*     Definitions
************************************************************************/
/* Automated time-outs */
#define FNET_HTTP_SRV_WAIT_TX_MS                    (15000u)  /* ms*/
#define FNET_HTTP_SRV_WAIT_RX_MS                    (20000u)  /* ms*/

#define FNET_HTTP_SRV_BACKLOG_MAX                   (FNET_CFG_HTTP_SRV_SESSION_MAX)

#define FNET_HTTP_SRV_VERSION_HEADER                "HTTP/" /* Protocol version HTTP/x.x*/
#define FNET_HTTP_SRV_ITERATION_NUMBER              (2u)

#define FNET_HTTP_SRV_HEADER_FIELD_CONTENT_TYPE     "Content-Type:"
#define FNET_HTTP_SRV_HEADER_FIELD_CONTENT_LENGTH   "Content-Length:"
#define FNET_HTTP_SRV_HEADER_FIELD_AUTHENTICATE     "WWW-Authenticate:"
#define FNET_HTTP_SRV_HEADER_FIELD_AUTHORIZATION    "Authorization:"

/* Supported method list. */
static const struct fnet_http_srv_method *fnet_http_srv_method_list[] =
{
    &fnet_http_srv_method_get,  /* GET method. */
    &fnet_http_srv_method_head,  /* HEAD method. */
#if FNET_CFG_HTTP_SRV_POST && FNET_CFG_HTTP_SRV_VERSION_MAJOR
    &fnet_http_srv_method_post, /* POST method. */
#endif
    0                /* End of the list */
};

/* File handler list.`*/
static const struct fnet_http_srv_file_handler *fnet_http_srv_file_handler_list[] =
{
#if FNET_CFG_HTTP_SRV_SSI
    &fnet_http_srv_ssi_handler, /* SSI handler */
#endif
#if FNET_CFG_HTTP_SRV_CGI
    &fnet_http_srv_cgi_handler, /* CGI handler */
#endif
    /* Add your file-handler here.*/
    0
};

/* Default file handler.*/
static const struct fnet_http_srv_file_handler fnet_http_srv_default_handler =
{
    "", _fnet_http_srv_default_handle,
    _fnet_http_srv_default_send,
    _fnet_http_srv_default_close
};
/*File extension*/ /* MIME type*/
static const struct fnet_http_srv_content_type fnet_http_srv_content_css = {"css", "text/css"};
static const struct fnet_http_srv_content_type fnet_http_srv_content_jpg = {"jpg", "image/jpeg"};
static const struct fnet_http_srv_content_type fnet_http_srv_content_gif = {"gif", "image/gif"};
static const struct fnet_http_srv_content_type fnet_http_srv_content_js = {"js", "application/javascript"};
static const struct fnet_http_srv_content_type fnet_http_srv_content_gzip = {"gz", "application/x-gzip"};
static const struct fnet_http_srv_content_type fnet_http_srv_content_zip = {"zip", "application/zip"};
static const struct fnet_http_srv_content_type fnet_http_srv_content_json = {"json", "application/json"};

/************************************************************************
*    File content-type list.
*************************************************************************/
static const struct fnet_http_srv_content_type *fnet_http_srv_content_type_list[] =
{
    &fnet_http_srv_content_css,
    &fnet_http_srv_content_jpg,
    &fnet_http_srv_content_gif,
    &fnet_http_srv_content_js,
    &fnet_http_srv_content_gzip,
    &fnet_http_srv_content_zip,
    &fnet_http_srv_content_json,
    /* Add your content-type here. */
    0
};

/* The HTTP interface */
static struct fnet_http_srv_if http_if_list[FNET_CFG_HTTP_SRV];

static void _fnet_http_srv_poll(void *http_if_p);
static fnet_int32_t _fnet_http_srv_recv(struct fnet_http_srv_session_if *session, fnet_uint8_t *buf, fnet_size_t len);
static fnet_int32_t _fnet_http_srv_send(struct fnet_http_srv_session_if *session, fnet_uint8_t *buf, fnet_size_t len);

#if FNET_CFG_HTTP_SRV_VERSION_MAJOR /* HTTP/1.x*/

/************************************************************************
*    HTTP response status list.
*************************************************************************/
struct fnet_http_srv_status fnet_http_srv_status_list[] =
{
    {FNET_HTTP_SRV_STATUS_CODE_OK,                  FNET_HTTP_SRV_REASON_PHRASE_OK},
    {FNET_HTTP_SRV_STATUS_CODE_CREATED,             FNET_HTTP_SRV_REASON_PHRASE_CREATED},
    {FNET_HTTP_SRV_STATUS_CODE_ACCEPTED,            FNET_HTTP_SRV_REASON_PHRASE_ACCEPTED},
    {FNET_HTTP_SRV_STATUS_CODE_NO_CONTENT,          FNET_HTTP_SRV_REASON_PHRASE_NO_CONTENT},
    {FNET_HTTP_SRV_STATUS_CODE_MOVED_PERMANENTLY,   FNET_HTTP_SRV_REASON_PHRASE_MOVED_PERMANENTLY},
    {FNET_HTTP_SRV_STATUS_CODE_MOVED_TEMPORARILY,   FNET_HTTP_SRV_REASON_PHRASE_MOVED_TEMPORARILY},
    {FNET_HTTP_SRV_STATUS_CODE_NOT_MODIFIED,        FNET_HTTP_SRV_REASON_PHRASE_NOT_MODIFIED},
    {FNET_HTTP_SRV_STATUS_CODE_BAD_REQUEST,         FNET_HTTP_SRV_REASON_PHRASE_BAD_REQUEST},
    {FNET_HTTP_SRV_STATUS_CODE_UNAUTHORIZED,        FNET_HTTP_SRV_REASON_PHRASE_UNAUTHORIZED},
    {FNET_HTTP_SRV_STATUS_CODE_FORBIDDEN,           FNET_HTTP_SRV_REASON_PHRASE_FORBIDDEN},
    {FNET_HTTP_SRV_STATUS_CODE_NOT_FOUND,           FNET_HTTP_SRV_REASON_PHRASE_NOT_FOUND},
    {FNET_HTTP_SRV_STATUS_CODE_INTERNAL_SERVER_ERROR, FNET_HTTP_SRV_REASON_PHRASE_INTERNAL_SERVER_ERROR},
    {FNET_HTTP_SRV_STATUS_CODE_NOT_IMPLEMENTED,     FNET_HTTP_SRV_REASON_PHRASE_NOT_IMPLEMENTED},
    {FNET_HTTP_SRV_STATUS_CODE_BAD_GATEWAY,         FNET_HTTP_SRV_REASON_PHRASE_BAD_GATEWAY},
    {FNET_HTTP_SRV_STATUS_CODE_SERVICE_UNAVAILABLE, FNET_HTTP_SRV_REASON_PHRASE_SERVICE_UNAVAILABLE},
    {FNET_HTTP_SRV_STATUS_CODE_NONE,                ""} /* End of the list.*/
};

static void _fnet_http_srv_version_parse(fnet_char_t *in_str, struct fnet_http_srv_version *version);
static fnet_return_t _fnet_http_srv_tx_status_line (struct fnet_http_srv_if *http);
#endif /* FNET_CFG_HTTP_SRV_VERSION_MAJOR */

/************************************************************************
* DESCRIPTION: Http server state machine.
************************************************************************/
static void _fnet_http_srv_poll( void *http_if_p )
{
    struct fnet_sockaddr            foreign_addr;
    fnet_size_t                     len;
    fnet_int32_t                    res;
    struct fnet_http_srv_if             *http = (struct fnet_http_srv_if *)http_if_p;
    fnet_index_t                    iteration;
    fnet_uint8_t                    *ch;
    fnet_index_t                    i;
    struct fnet_http_srv_session_if     *session;

    for(i = 0u; i < FNET_CFG_HTTP_SRV_SESSION_MAX; i++)
    {
        session = &http->session[i];
        http->session_active = session;

        for(iteration = 0u; iteration < FNET_HTTP_SRV_ITERATION_NUMBER; iteration++)
        {
            switch(session->state)
            {

                /*---- LISTENING ------------------------------------------------*/
                case FNET_HTTP_SRV_STATE_LISTENING:
                    len = sizeof(foreign_addr);
                    session->socket_foreign = fnet_socket_accept(http->socket_listen, &foreign_addr, &len);
                    if(session->socket_foreign)
                    {
#if FNET_CFG_DEBUG_HTTP && FNET_CFG_DEBUG
                        {
                            fnet_uint8_t ip_str[FNET_IP_ADDR_STR_SIZE];
                            fnet_inet_ntop(foreign_addr.sa_family, (fnet_uint8_t *)(foreign_addr.sa_data), ip_str, sizeof(ip_str));
                            FNET_DEBUG_HTTP("");
                            FNET_DEBUG_HTTP("HTTP: RX Request From: %s; Port: %d.", ip_str, fnet_ntohs(foreign_addr.sa_port));
                        }
#endif
                        /* Reset response & request parameters.*/
                        fnet_memset_zero(&session->response, sizeof(struct fnet_http_srv_response));
                        fnet_memset_zero(&session->request, sizeof(struct fnet_http_srv_request));

#if FNET_CFG_HTTP_SRV_VERSION_MAJOR /* HTTP/1.x*/
                        session->response.content_length = -1; /* No content length by default.*/
                        /* Default HTTP version response.*/
                        session->response.version.major = FNET_HTTP_SRV_VERSION_MAJOR;
                        session->response.version.minor = FNET_HTTP_SRV_VERSION_MINOR;
                        session->response.tx_data = _fnet_http_srv_tx_status_line;
#endif
                        session->state_time_ms = fnet_timer_get_ms();          /* Reset timeout. */
                        session->buffer_actual_size = 0u;

#if (FNET_CFG_HTTP_SRV_TLS && FNET_CFG_TLS)
                        if(http->tls_desc)
                        {
                            session->tls_sock = fnet_tls_socket(http->tls_desc, session->socket_foreign);
                            if(session->tls_sock == 0)
                            {
                                FNET_DEBUG_HTTP("HTTP: TLS socket() failed");
                                session->state = FNET_HTTP_SRV_STATE_CLOSING;
                                break;
                            }

#if 0 /* Optional */
                            if(fnet_tls_socket_connect(session->tls_sock) == FNET_ERR)
                            {
                                FNET_DEBUG_HTTP("HTTP: TLS connect() failed");
                                session->state = FNET_HTTP_SRV_STATE_CLOSING;
                                break;
                            }
#endif
                        }
#endif
                        session->state = FNET_HTTP_SRV_STATE_RX_REQUEST; /* => WAITING HTTP REQUEST */
                    }
                    break;
                /*---- RX_LINE -----------------------------------------------*/
                case FNET_HTTP_SRV_STATE_RX_REQUEST:
                    do
                    {
                        /* Read character by character.*/
                        ch = &session->buffer[session->buffer_actual_size];

                        if((res = _fnet_http_srv_recv(session, ch, 1u) ) != FNET_ERR)
                        {
                            if(res > 0) /* Received a data.*/
                            {
                                session->state_time_ms = fnet_timer_get_ms();  /* Reset timeout.*/

                                session->buffer_actual_size++;

                                if(*ch == '\r')
                                {
                                    *ch = '\0';
                                }
                                else if(*ch == '\n')
                                    /* Line received.*/
                                {
                                    fnet_uint8_t *req_buf = &session->buffer[0];

                                    *ch = '\0';

                                    if(session->request.method == 0)
                                        /* Parse Request line.*/
                                    {
                                        const struct fnet_http_srv_method **method = &fnet_http_srv_method_list[0];

                                        FNET_DEBUG_HTTP("HTTP: RX Request: %s", req_buf);

                                        /* Determine the method type. */
                                        while(*method)
                                        {
                                            if ( !fnet_strcmp_splitter((fnet_char_t *)req_buf, (*method)->token, ' ') )
                                            {
                                                req_buf += fnet_strlen((*method)->token);
                                                session->request.method = *method;
                                                break;
                                            }
                                            method++;
                                        }

                                        /* Check if the method is supported? */
                                        if((session->request.method) && (session->request.method->handle))
                                        {
                                            /* Parse URI.*/
                                            req_buf = (fnet_uint8_t *)_fnet_http_srv_uri_parse((fnet_char_t *)req_buf, &session->request.uri);

                                            FNET_DEBUG_HTTP("HTTP: URI Path = %s; Query = %s", session->request.uri.path, session->request.uri.query);

#if FNET_CFG_HTTP_SRV_VERSION_MAJOR /* HTTP/1.x*/
                                            /* Parse HTTP/x.x version.*/
                                            _fnet_http_srv_version_parse((fnet_char_t *)++req_buf, &session->response.version);

                                            /* Check the highest supported HTTP version.*/
                                            if((((fnet_uint32_t)session->response.version.major << 8) | (fnet_uint32_t)session->response.version.minor) > ((FNET_HTTP_SRV_VERSION_MAJOR << 8) | FNET_HTTP_SRV_VERSION_MINOR))
                                            {
                                                session->response.version.major = FNET_HTTP_SRV_VERSION_MAJOR;
                                                session->response.version.minor = FNET_HTTP_SRV_VERSION_MINOR;
                                            }

                                            if(session->response.version.major == 0u)
                                                /* HTTP/0.x */
                                            {
                                                /* Client does not support HTTP/1.x*/
                                                session->state = FNET_HTTP_SRV_STATE_CLOSING;  /*=> CLOSING */
                                                FNET_DEBUG_HTTP("HTTP: client does not support HTTP/1.x");
                                            }
                                            else
                                            {
#if FNET_CFG_HTTP_SRV_AUTHENTICATION_BASIC && FNET_CFG_HTTP_SRV_VERSION_MAJOR
                                                /* Check Authentification.*/
                                                _fnet_http_srv_auth_validate_uri(http);
#endif
#endif/*FNET_CFG_HTTP_SRV_VERSION_MAJOR*/

                                                /* Call method initial handler.*/
                                                res = session->request.method->handle(http, &session->request.uri);

#if FNET_CFG_HTTP_SRV_VERSION_MAJOR /* HTTP/1.x*/
                                                if(session->response.no_header == FNET_TRUE)
                                                {
                                                    /*  Skip sending of status code and header.*/
                                                    session->response.tx_data = session->request.method->send;
                                                }
#endif

                                                if((res == FNET_OK))
                                                {
#if FNET_CFG_HTTP_SRV_VERSION_MAJOR /* HTTP/1.x*/
                                                    session->buffer_actual_size = 0u;
                                                    /* => Parse Header line.*/
#else /* HTTP/0.9 */
                                                    session->response.tx_data = session->request.method->send;

                                                    /* Reset buffer pointers.*/
                                                    session->buffer_actual_size = 0;
                                                    session->state = FNET_HTTP_SRV_STATE_TX; /* Send data.*/
#endif
                                                }
                                                /* Method error.*/
                                                else /* Error.*/
                                                {
#if FNET_CFG_HTTP_SRV_VERSION_MAJOR /* HTTP/1.x*/
                                                    /* Send status line.*/
                                                    session->buffer_actual_size = 0u;
                                                    session->state = FNET_HTTP_SRV_STATE_TX; /* Send error.*/
#else /* HTTP/0.9 */
                                                    session->state = FNET_HTTP_SRV_STATE_CLOSING;
                                                    FNET_DEBUG_HTTP("HTTP: method error");
#endif
                                                }
                                            }
                                        }
                                        /* Method is not supported.*/
                                        else /* Error.*/
                                        {
#if FNET_CFG_HTTP_SRV_VERSION_MAJOR /* HTTP/1.x*/
                                            session->response.status.code = FNET_HTTP_SRV_STATUS_CODE_NOT_IMPLEMENTED;
                                            /* Send status line.*/
                                            session->buffer_actual_size = 0u;
                                            session->state = FNET_HTTP_SRV_STATE_TX; /* Send error.*/
#else /* HTTP/0.9 */
                                            session->state = FNET_HTTP_SRV_STATE_CLOSING;
                                            FNET_DEBUG_HTTP("HTTP: Method is not supported.");
#endif
                                        }

                                    }
#if FNET_CFG_HTTP_SRV_VERSION_MAJOR /* HTTP/1.x*/
                                    /* Parse Header line.*/
                                    else
                                    {
                                        if(session->request.skip_line == FNET_FALSE)
                                        {
                                            if(*req_buf == 0u)
                                                /* === Empty line => End of the request header. ===*/
                                            {
#if FNET_CFG_HTTP_SRV_AUTHENTICATION_BASIC && FNET_CFG_HTTP_SRV_VERSION_MAJOR
                                                if(session->response.auth_entry)
                                                {
                                                    /* Send UNAUTHORIZED error.*/
                                                    session->response.status.code = FNET_HTTP_SRV_STATUS_CODE_UNAUTHORIZED;
                                                }
                                                else /* Send Data.*/
#endif
                                                {

                                                    if(session->response.status.code == FNET_HTTP_SRV_STATUS_CODE_NONE) /* If status code is not set.*/
                                                    {
                                                        session->response.status.code = FNET_HTTP_SRV_STATUS_CODE_OK;
                                                    }
                                                }
#if FNET_CFG_HTTP_SRV_POST && FNET_CFG_HTTP_SRV_VERSION_MAJOR
                                                if(session->request.content_length > 0)
                                                    /* RX Entity-Body.*/
                                                {
                                                    session->buffer_actual_size = 0u;
                                                    session->state = FNET_HTTP_SRV_STATE_RX;
                                                }
                                                else
#endif
                                                    /* TX Full-Responce.*/
                                                {
                                                    /* Send status line.*/
                                                    session->buffer_actual_size = 0u;
                                                    session->state = FNET_HTTP_SRV_STATE_TX;
                                                }
                                                break;
                                            }
                                            else
                                                /* === Parse header fields. ===*/
                                            {

                                                FNET_DEBUG_HTTP("HTTP: RX Header: %s", req_buf);

#if FNET_CFG_HTTP_SRV_AUTHENTICATION_BASIC && FNET_CFG_HTTP_SRV_VERSION_MAJOR
                                                /* --- Authorization: ---*/
                                                if ((session->response.auth_entry) && (fnet_strncmp((fnet_char_t *)req_buf, FNET_HTTP_SRV_HEADER_FIELD_AUTHORIZATION, sizeof(FNET_HTTP_SRV_HEADER_FIELD_AUTHORIZATION) - 1u) == 0))
                                                    /* Authetication is required.*/
                                                {
                                                    fnet_char_t *auth_str = (fnet_char_t *)&req_buf[sizeof(FNET_HTTP_SRV_HEADER_FIELD_AUTHORIZATION) - 1u];

                                                    /* Validate credentials.*/
                                                    if(_fnet_http_srv_auth_validate_credentials(http, auth_str) == FNET_OK)
                                                    {
                                                        session->response.auth_entry = 0; /* Authorization is succesful.*/
                                                    }
                                                }
#endif

#if FNET_CFG_HTTP_SRV_POST && FNET_CFG_HTTP_SRV_VERSION_MAJOR
                                                /* --- Content-Length: ---*/
                                                if ((session->request.method->receive) && (fnet_strncmp((fnet_char_t *)req_buf, FNET_HTTP_SRV_HEADER_FIELD_CONTENT_LENGTH, sizeof(FNET_HTTP_SRV_HEADER_FIELD_CONTENT_LENGTH) - 1u) == 0))
                                                {
                                                    fnet_char_t *p;
                                                    fnet_char_t *length_str = (fnet_char_t *)&req_buf[sizeof(FNET_HTTP_SRV_HEADER_FIELD_CONTENT_LENGTH) - 1u];

                                                    session->request.content_length = (fnet_int32_t)fnet_strtoul(length_str, &p, 10u);
                                                }
#endif
                                            }
                                        }
                                        /* Line is skiped.*/
                                        else
                                        {
                                            session->request.skip_line = FNET_FALSE; /* Reset the Skip flag.*/
                                        }

                                        session->buffer_actual_size = 0u; /* => Parse Next Header line.*/
                                    }
#endif/* FNET_CFG_HTTP_SRV_VERSION_MAJOR */
                                }
                                /* Not whole line received yet.*/
                                else if (session->buffer_actual_size == FNET_HTTP_SRV_BUF_SIZE)
                                    /* Buffer is full.*/
                                {
#if FNET_CFG_HTTP_SRV_VERSION_MAJOR /* HTTP/1.x*/
                                    if(session->request.method != 0)
                                        /* For header, skip the line.*/
                                    {
                                        /* Skip line.*/
                                        session->request.skip_line = FNET_TRUE;
                                        session->buffer_actual_size = 0u;
                                    }
                                    else /* Error.*/
                                    {
                                        /* Default code = FNET_HTTP_SRV_STATUS_CODE_INTERNAL_SERVER_ERROR. */
                                        session->buffer_actual_size = 0u;
                                        session->state = FNET_HTTP_SRV_STATE_TX; /* Send error.*/
                                    }

#else /* HTTP/0.9 */
                                    session->state = FNET_HTTP_SRV_STATE_CLOSING;
                                    fnet_println("HTTP: Buffer is full.");
#endif
                                }
                                else
                                {}
                            }
                            /* No data.*/
                            else if((fnet_timer_get_ms() - session->state_time_ms) /* Time out? */
                                    > FNET_HTTP_SRV_WAIT_RX_MS)
                            {
                                session->state = FNET_HTTP_SRV_STATE_CLOSING; /*=> CLOSING */
                                FNET_DEBUG_HTTP("HTTP: Timeout.");
                            }
                            /* else => WAITING REQUEST. */
                            else
                            {}
                        }
                        /* _fnet_http_srv_recv() error.*/
                        else
                        {
                            session->state = FNET_HTTP_SRV_STATE_CLOSING; /*=> CLOSING */
                            FNET_DEBUG_HTTP("HTTP: RX error.");
                        }
                    }
                    while ((res > 0) && (session->state == FNET_HTTP_SRV_STATE_RX_REQUEST)); /* Till receiving the request header.*/
                    break;
#if FNET_CFG_HTTP_SRV_POST && FNET_CFG_HTTP_SRV_VERSION_MAJOR
                /*---- RX --------------------------------------------------*/
                case FNET_HTTP_SRV_STATE_RX: /* Receive data (Entity-Body). */
                    if((res = _fnet_http_srv_recv(session, &session->buffer[session->buffer_actual_size], (FNET_HTTP_SRV_BUF_SIZE - session->buffer_actual_size)) ) != FNET_ERR)
                    {
                        session->buffer_actual_size += (fnet_size_t)res;
                        session->request.content_length -= res;

                        if(res > 0)
                            /* Some Data.*/
                        {
                            session->state_time_ms = fnet_timer_get_ms();  /* Reset timeout.*/
                            res = session->request.method->receive(http);
                            if(res == FNET_ERR)
                            {
                                if(session->response.status.code == FNET_HTTP_SRV_STATUS_CODE_NONE )
                                {
                                    session->response.status.code = FNET_HTTP_SRV_STATUS_CODE_INTERNAL_SERVER_ERROR;
                                }

                                session->request.content_length = 0;
                            }

                            if(session->request.content_length <= 0) /* The last data.*/
                            {
                                session->state = FNET_HTTP_SRV_STATE_TX; /* Send data.*/
                            }

                            session->buffer_actual_size = 0u;
                        }
                        else
                            /* No Data.*/
                        {
                            if((fnet_timer_get_ms() - session->state_time_ms) > FNET_HTTP_SRV_WAIT_RX_MS)
                                /* Time out.*/
                            {
                                FNET_DEBUG_HTTP("HTTP: Time out.");
                                session->state = FNET_HTTP_SRV_STATE_CLOSING; /*=> CLOSING */
                            }
                        }
                    }
                    else
                        /* Socket error.*/
                    {
                        FNET_DEBUG_HTTP("HTTP: Socket error.");
                        session->state = FNET_HTTP_SRV_STATE_CLOSING; /*=> CLOSING */
                    }

                    break;
#endif /* FNET_CFG_HTTP_SRV_POST.*/
                /*---- TX --------------------------------------------------*/
                case FNET_HTTP_SRV_STATE_TX: /* Send data. */
                    if((fnet_timer_get_ms() - session->state_time_ms) < FNET_HTTP_SRV_WAIT_TX_MS) /* Check timeout */
                    {
                        fnet_size_t send_size;

                        if(session->buffer_actual_size == session->response.buffer_sent)
                        {
                            /* Reset counters.*/
                            session->buffer_actual_size = 0u;
                            session->response.buffer_sent = 0u;

                            if((session->response.send_eof) || (session->response.tx_data == FNET_NULL) || (session->response.tx_data(http) == FNET_ERR)) /* get data for sending */
                            {
                                FNET_DEBUG_HTTP("HTTP: TX EOF.");
                                session->state = FNET_HTTP_SRV_STATE_CLOSING; /*=> CLOSING */
                                break;
                            }
                        }

                        send_size = (session->buffer_actual_size - session->response.buffer_sent);

                        if((res = _fnet_http_srv_send(session, session->buffer + session->response.buffer_sent, send_size)) != FNET_ERR)
                        {
                            if(res)
                            {
                                FNET_DEBUG_HTTP("HTTP: TX %d bytes.", res);

                                session->state_time_ms = fnet_timer_get_ms();              /* reset timeout */
                                session->response.buffer_sent += (fnet_size_t)res;
                            }
                            break; /* => SENDING */
                        }
                    }
                    FNET_DEBUG_HTTP("HTTP:Time out.");
                    session->state = FNET_HTTP_SRV_STATE_CLOSING; /*=> CLOSING */
                    break;
                /*---- CLOSING --------------------------------------------------*/
                case FNET_HTTP_SRV_STATE_CLOSING:
                    if((session->request.method) && (session->request.method->close))
                    {
                        session->request.method->close(http);
                    }

#if (FNET_CFG_HTTP_SRV_TLS && FNET_CFG_TLS)
                    if(session->tls_sock)
                    {
                        /* Close TLS session */
                        fnet_tls_socket_close(session->tls_sock);
                        session->tls_sock = 0;
                    }
#endif
                    fnet_socket_close(session->socket_foreign);
                    session->socket_foreign = FNET_NULL;

                    FNET_DEBUG_HTTP("HTTP: Session close");

                    session->state = FNET_HTTP_SRV_STATE_LISTENING; /*=> LISTENING */
                    break;
                default:
                    break;
            }
        }
    } /*for(sessions)*/
}

/************************************************************************
* DESCRIPTION: Initialization of the HTTP server.
*************************************************************************/
fnet_http_srv_desc_t fnet_http_srv_init( fnet_http_srv_params_t *params )
{
    struct fnet_sockaddr        local_addr;
    struct fnet_http_srv_uri        uri;
    fnet_index_t                i;
    struct fnet_http_srv_if         *http_if = 0;
    const struct fnet_linger    linger_option =
    {
        .l_onoff = FNET_TRUE,
        .l_linger = 4 /*sec*/
    };

    fnet_service_mutex_lock();

    if((params == 0) || (params->root_path == 0) || (params->index_path == 0))
    {
        FNET_DEBUG_HTTP("HTTP: Wrong init parameters.");
        goto ERROR_1;
    }

    /* Try to find free HTTP server. */
    for(i = 0u; i < FNET_CFG_HTTP_SRV; i++)
    {
        if(http_if_list[i].is_enabled == FNET_FALSE)
        {
            http_if = &http_if_list[i];
            break;
        }
    }

    /* Is HTTP server already initialized. */
    if(http_if == 0)
    {
        FNET_DEBUG_HTTP("HTTP: No free HTTP Server.");
        goto ERROR_1;
    }

    /* Clear all parameters.*/
    fnet_memset(http_if, 0, sizeof(*http_if));

#if FNET_CFG_HTTP_SRV_SSI
    http_if->ssi.ssi_table = params->ssi_table;
#endif

#if FNET_CFG_HTTP_SRV_CGI
    http_if->cgi_table = params->cgi_table;
#endif

#if FNET_CFG_HTTP_SRV_AUTHENTICATION_BASIC && FNET_CFG_HTTP_SRV_VERSION_MAJOR
    http_if->auth_table = params->auth_table;
#endif

#if FNET_CFG_HTTP_SRV_POST && FNET_CFG_HTTP_SRV_VERSION_MAJOR
    http_if->post_table = params->post_table;
#endif

    fnet_memcpy(&local_addr, &params->address, sizeof(local_addr));

    if(local_addr.sa_port == 0u)
    {
        /* Aply the default port.*/
#if (FNET_CFG_HTTP_SRV_TLS && FNET_CFG_TLS)
        if(params->tls_params)
        {
            local_addr.sa_port = FNET_CFG_HTTP_SRV_TLS_PORT;
        }
        else
#endif
        {
            local_addr.sa_port = FNET_CFG_HTTP_SRV_PORT;
        }
    }

    if(local_addr.sa_family == AF_UNSPEC)
    {
        local_addr.sa_family = AF_SUPPORTED; /* Assign supported families.*/
    }

    /* Create listen socket */
    http_if->socket_listen = fnet_socket(local_addr.sa_family, SOCK_STREAM, 0u);
    if(http_if->socket_listen == FNET_NULL)
    {
        FNET_DEBUG_HTTP("HTTP: Socket creation error.");
        goto ERROR_1;
    }

    /* Bind.*/
    if(fnet_socket_bind(http_if->socket_listen, &local_addr, sizeof(local_addr)) == FNET_ERR)
    {
        FNET_DEBUG_HTTP("HTTP: Socket bind error.");
        goto ERROR_2;
    }

    /* Set socket options.*/
    fnet_socket_setopt (http_if->socket_listen, SOL_SOCKET, SO_LINGER, &linger_option, sizeof(linger_option));

    /* Listen.*/
    if(fnet_socket_listen(http_if->socket_listen, FNET_HTTP_SRV_BACKLOG_MAX) == FNET_ERR)
    {
        FNET_DEBUG_HTTP("HTTP: Socket listen error.");
        goto ERROR_2;
    }

    /* Open root dir. */
    http_if->root_dir = fnet_fs_opendir(params->root_path);
    if(http_if->root_dir == 0)
    {
        FNET_DEBUG_HTTP("HTTP: Root directory is failed.");
        goto ERROR_2;
    }

    /* Open index file. */
    http_if->index_file = fnet_fs_fopen_re(params->index_path, "r", http_if->root_dir);
    if(http_if->index_file == 0)
    {
        FNET_DEBUG_HTTP("HTTP: Root directory is failed.");
        goto ERROR_3;
    }

    _fnet_http_srv_uri_parse(params->index_path, &uri);
    http_if->index_file_handler = _fnet_http_srv_find_handler(http_if, &uri); /* Find Handler for the index file. */

#if FNET_CFG_HTTP_SRV_VERSION_MAJOR
    http_if->index_file_content_type = _fnet_http_srv_find_content_type(http_if, &uri); /* Find Content-Type for the index file. */
#endif

    /* Init session parameters.*/
    for(i = 0u; i < FNET_CFG_HTTP_SRV_SESSION_MAX; i++)
    {
        struct fnet_http_srv_session_if   *session = &http_if->session[i];
        session->state = FNET_HTTP_SRV_STATE_LISTENING;
    }

    http_if->service_descriptor = fnet_service_register(_fnet_http_srv_poll, (void *) http_if);
    if(http_if->service_descriptor == 0)
    {
        FNET_DEBUG_HTTP("HTTP: Service registration error.");
        goto ERROR_4;
    }

#if (FNET_CFG_HTTP_SRV_TLS && FNET_CFG_TLS)
    if(params->tls_params)
    {
        http_if->tls_desc = fnet_tls_init(FNET_TLS_ROLE_SERVER);
        if(http_if->tls_desc == 0)
        {
            FNET_DEBUG_HTTP("HTTP: TLS initialization error.");
            goto ERROR_5;
        }
        else
        {
            /* Set own certificate chain and private key */
            if(fnet_tls_set_own_certificate(http_if->tls_desc, params->tls_params->certificate_buffer, params->tls_params->certificate_buffer_size, params->tls_params->private_key_buffer, params->tls_params->private_key_buffer_size) == FNET_ERR)
            {
                FNET_DEBUG_HTTP("HTTP: TLS certificate error.");
                goto ERROR_6;
            }
        }
    }
#endif

    http_if->is_enabled = FNET_TRUE;

    fnet_service_mutex_unlock();

    return (fnet_http_srv_desc_t)http_if;

#if (FNET_CFG_HTTP_SRV_TLS && FNET_CFG_TLS)
ERROR_6:
    fnet_tls_release(http_if->tls_desc);
ERROR_5:
    fnet_service_unregister(http_if->service_descriptor);
#endif
ERROR_4:
    fnet_fs_fclose(http_if->index_file);
ERROR_3:
    fnet_fs_closedir(http_if->root_dir);
ERROR_2:
    fnet_socket_close(http_if->socket_listen);
ERROR_1:
    fnet_service_mutex_unlock();

    return 0;
}

/************************************************************************
* DESCRIPTION: HTTP server release.
************************************************************************/
void fnet_http_srv_release(fnet_http_srv_desc_t desc)
{
    struct fnet_http_srv_if     *http_if = (struct fnet_http_srv_if *) desc;
    fnet_index_t            i;

    fnet_service_mutex_lock();

    if(http_if && (http_if->is_enabled == FNET_TRUE))
    {
        for(i = 0u; i < FNET_CFG_HTTP_SRV_SESSION_MAX; i++)
        {
            struct fnet_http_srv_session_if   *session = &http_if->session[i];

#if (FNET_CFG_HTTP_SRV_TLS && FNET_CFG_TLS)
            if(session->tls_sock)
            {
                /* Close TLS session */
                fnet_tls_socket_close(session->tls_sock);
            }
#endif

            fnet_socket_close(session->socket_foreign);
            fnet_fs_fclose(session->file_desc);
        }

        fnet_fs_fclose(http_if->index_file);
        fnet_fs_closedir(http_if->root_dir);
        fnet_socket_close(http_if->socket_listen);
        fnet_service_unregister(http_if->service_descriptor); /* Delete service.*/

#if (FNET_CFG_HTTP_SRV_TLS && FNET_CFG_TLS)
        if(http_if->tls_desc)
        {
            fnet_tls_release(http_if->tls_desc);                   /* Release TLS. */
        }
#endif

        /* Clear all parameters.*/
        fnet_memset(http_if, 0, sizeof(*http_if));
    }
    fnet_service_mutex_unlock();
}

/************************************************************************
* DESCRIPTION: This function returns FNET_TRUE if the HTTP server
*              is enabled/initialised.
************************************************************************/
fnet_bool_t fnet_http_srv_is_enabled(fnet_http_srv_desc_t desc)
{
    struct fnet_http_srv_if     *http_if = (struct fnet_http_srv_if *) desc;
    fnet_bool_t             result;

    if(http_if)
    {
        result = http_if->is_enabled;
    }
    else
    {
        result = FNET_FALSE;
    }

    return result;
}


#if FNET_CFG_HTTP_SRV_VERSION_MAJOR /* HTTP/1.x*/

/************************************************************************
* DESCRIPTION:
************************************************************************/
static fnet_return_t _fnet_http_srv_tx_status_line (struct fnet_http_srv_if *http)
{
    struct fnet_http_srv_status     *status ;
    fnet_size_t                 result = 0u;
    fnet_size_t                 result_state;
    struct fnet_http_srv_session_if *session =  http->session_active;

    session->response.send_eof = FNET_FALSE; /* Data to be sent.*/

    do
    {
        result_state = 0u;

        switch(session->response.status_line_state)
        {
            case 0:
                if(session->response.status.code == 0)
                {
                    /* If the code was not found, produce a 501 internal server error */
                    session->response.status.code = FNET_HTTP_SRV_STATUS_CODE_INTERNAL_SERVER_ERROR;
                    session->response.status.phrase = 0;
                }

                if(session->response.status.phrase == 0) /* If no phrase is defined.*/
                {
                    for(status = fnet_http_srv_status_list; status->code > 0; status++) /* Find phrase.*/
                    {
                        if (status->code == session->response.status.code)
                        {
                            break;
                        }
                    }
                    session->response.status.phrase = status->phrase; /* If no phrase is fond it will pont to empty string.*/
                }

                /* Print status line.*/
                result_state = (fnet_size_t)fnet_snprintf((fnet_char_t *)session->buffer, FNET_HTTP_SRV_BUF_SIZE, "%s%d.%d %d %s%s", FNET_HTTP_SRV_VERSION_HEADER, session->response.version.major, session->response.version.minor,
                               session->response.status.code, session->response.status.phrase,
                               "\r\n");
                break;
            /* Add HTTP header fields where applicable. */
            case 1:
#if FNET_CFG_HTTP_SRV_AUTHENTICATION_BASIC && FNET_CFG_HTTP_SRV_VERSION_MAJOR
                /* Authentificate.*/
                if(session->response.status.code == FNET_HTTP_SRV_STATUS_CODE_UNAUTHORIZED)
                {
                    result_state = (fnet_size_t)fnet_snprintf((fnet_char_t *)&session->buffer[result], (FNET_HTTP_SRV_BUF_SIZE - result), "%s ", FNET_HTTP_SRV_HEADER_FIELD_AUTHENTICATE);
                    result_state += _fnet_http_srv_auth_generate_challenge(http, &session->buffer[result + result_state], FNET_HTTP_SRV_BUF_SIZE - (result + result_state));
                    session->response.content_length = -1; /* No content length .*/
                }
#endif
                break;
            case 2:
                /* Content-Length */
                if(session->response.content_length >= 0)
                {
                    result_state = (fnet_size_t)fnet_snprintf((fnet_char_t *)&session->buffer[result], (FNET_HTTP_SRV_BUF_SIZE - result), "%s %d%s", FNET_HTTP_SRV_HEADER_FIELD_CONTENT_LENGTH, session->response.content_length, "\r\n");
                }
                break;
            case 3:
                /* Add MIME Content Type field, based on file extension.*/
                if(session->response.send_file_content_type)
                {
                    result_state = (fnet_size_t)fnet_snprintf((fnet_char_t *)&session->buffer[result],
                                   (FNET_HTTP_SRV_BUF_SIZE - result), "%s %s%s",
                                   FNET_HTTP_SRV_HEADER_FIELD_CONTENT_TYPE, session->response.send_file_content_type->content_type, "\r\n");
                }
                break;
            case 4:
                /*Final CRLF.*/
                result_state = (fnet_size_t)fnet_snprintf((fnet_char_t *)&session->buffer[result], (FNET_HTTP_SRV_BUF_SIZE - result), "%s", "\r\n");
#if 0 /* On any error code.*/
                if(session->response.status.code != FNET_HTTP_SRV_STATUS_CODE_OK)
                {
                    session->response.send_eof = FNET_TRUE; /* Only sataus (without data).*/
                }
#else
                if(session->response.status.code == FNET_HTTP_SRV_STATUS_CODE_UNAUTHORIZED) /* UNAUTHORIZED */
                {
                    session->response.send_eof = FNET_TRUE; /* Only sataus (without data).*/
                }
#endif

                if(session->request.method && session->request.method->send)
                {
                    session->response.tx_data = session->request.method->send;
                }
                else
                {
                    session->response.send_eof = FNET_TRUE; /* Only sataus (without data).*/
                }
                break;
            default:
                break;
        }

        if((result + result_state) == (FNET_HTTP_SRV_BUF_SIZE - 1u)) /* Buffer overload.*/
        {
            if(result == 0u)
            {
                fnet_sprintf((fnet_char_t *)&session->buffer[FNET_HTTP_SRV_BUF_SIZE - 2u], "%s", "\r\n"); /* Finish line.*/
                session->response.status_line_state++;
            }
            /* else. Do not send last state data.*/
            break; /* Send data.*/
        }
        else
        {
            result += result_state;
            session->response.status_line_state++;
        }
    }
    while (session->response.status_line_state <= 4u);

    session->buffer_actual_size =  result;
    FNET_DEBUG_HTTP("HTTP: TX Status: %s", session->buffer);

    return FNET_OK;
}

#endif /* FNET_CFG_HTTP_SRV_VERSION_MAJOR */

/************************************************************************
* DESCRIPTION:
************************************************************************/
const struct fnet_http_srv_file_handler *_fnet_http_srv_find_handler (struct fnet_http_srv_if *http, struct fnet_http_srv_uri *uri)
{
    const struct fnet_http_srv_file_handler *result = &fnet_http_srv_default_handler;
    const struct fnet_http_srv_file_handler **handler = &fnet_http_srv_file_handler_list[0];

    if(uri)
    {
        if (!fnet_strcmp(uri->path, "/")) /* Default index file. */
        {
            result = http->index_file_handler;
        }
        else
        {
            while(*handler)
            {
                if ( !fnet_strcmp( uri->extension, (*handler)->file_extension) )
                {
                    result = *handler;
                    break;
                }
                handler++;
            }
        }

    }

    return result;
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
#if FNET_CFG_HTTP_SRV_VERSION_MAJOR
const struct fnet_http_srv_content_type *_fnet_http_srv_find_content_type (struct fnet_http_srv_if *http, struct fnet_http_srv_uri *uri)
{
    const struct fnet_http_srv_content_type **content_type = &fnet_http_srv_content_type_list[0];
    const struct fnet_http_srv_content_type *result = FNET_NULL;

    if(uri)
    {
        if (!fnet_strcmp(uri->path, "/")) /* Default index file. */
        {
            result = http->index_file_content_type;
        }
        else
        {
            while(*content_type)
            {
                if ( !fnet_strcmp(uri->extension, (*content_type)->file_extension))
                {
                    result = *content_type;
                    break;
                }
                content_type++;
            }
        }
    }

    return result;
}
#endif /* FNET_CFG_HTTP_SRV_VERSION_MAJOR */

/************************************************************************
* DESCRIPTION:
************************************************************************/
fnet_return_t _fnet_http_srv_default_handle (struct fnet_http_srv_if *http, struct fnet_http_srv_uri *uri)
{
    fnet_return_t               result;
    struct fnet_http_srv_session_if *session =  http->session_active;

    if (!fnet_strcmp(uri->path, "/")) /* Default index file */
    {
        fnet_fs_rewind(http->index_file);
        session->file_desc = http->index_file;
    }
    else
    {
        session->file_desc = fnet_fs_fopen_re(uri->path, "r", http->root_dir);
    }

    if (session->file_desc)
    {
#if FNET_CFG_HTTP_SRV_VERSION_MAJOR /* HTTP/1.x*/
        {
            fnet_fs_dirent_t dirent;

            fnet_fs_finfo(session->file_desc, &dirent);
            session->response.content_length = (fnet_int32_t)dirent.d_size;
        }
#endif
        result = FNET_OK;
    }
    else
    {
#if FNET_CFG_HTTP_SRV_VERSION_MAJOR /* HTTP/1.x*/
        session->response.status.code = FNET_HTTP_SRV_STATUS_CODE_NOT_FOUND;
#endif
        result = FNET_ERR;
    }

    return result;
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
fnet_size_t _fnet_http_srv_default_send (struct fnet_http_srv_if *http)
{
    struct fnet_http_srv_session_if *session =  http->session_active;

    return fnet_fs_fread(session->buffer, sizeof(session->buffer), session->file_desc);
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
void _fnet_http_srv_default_close (struct fnet_http_srv_if *http)
{
    struct fnet_http_srv_session_if *session =  http->session_active;

    if(session->file_desc != http->index_file)
    {
        fnet_fs_fclose(session->file_desc); /* Close file */
    }
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
void fnet_http_srv_query_unencode(fnet_uint8_t *dest, fnet_uint8_t *src)
{
    if(dest && src)
    {
        while(*src != 0u)
        {
            if(*src == '+')
            {
                *dest = ' ';
            }
            else if(*src == '%')
            {
                fnet_index_t i;
                fnet_uint8_t val = 0u;

                for(i = 0u; (i < 2u) && (*src != 0u) ; i++)
                {
                    src++;
                    if((*src >= '0') && (*src <= '9'))
                    {
                        val = (fnet_uint8_t)((val << 4) + (*src - '0'));
                        continue;
                    }
                    else if(((*src >= 'a') && (*src <= 'f')) || ((*src >= 'A') && (*src <= 'F')))
                    {
                        val = (fnet_uint8_t)((val << 4) + (*src + 10u - (((*src >= 'a') && (*src <= 'f')) ? 'a' : 'A')));
                        continue;
                    }
                    else
                    {}
                    break;
                }
                if(i == 2u)
                {
                    *dest = val;
                }
                else
                {
                    *dest = '?';
                }
            }
            else
            {
                *dest = *src;
            }

            src++;
            dest++;
        }

        *dest = '\0';
    }
}


/************************************************************************
* DESCRIPTION:
*   Returns pointer to the end of URI str.
************************************************************************/
fnet_char_t *_fnet_http_srv_uri_parse(fnet_char_t *in_str, struct fnet_http_srv_uri *uri)
{
    fnet_char_t *cur = in_str;

    /* rel_uri       = [ path ] [ "?" query ]*/
    if(cur && uri)
    {
        /*Parse the Request-URI. */
        /*Extract file name. */

        /* Ignore any initial spaces */
        while (*cur == ' ')
        {
            cur++;
        }

        uri->path = cur;
        uri->query = 0;
        uri->extension = 0;

        /* Find end of the file name. */
        while( *cur != '\0')
        {
            if (*cur == ' ')	/* Path end is found */
            {
                *cur = '\0';                    /* Mark path end */
            }
            else if(*cur == '?')                /* Query is found */
            {
                fnet_char_t  *end_query;

                *cur = '\0';                /* Mark path end */

                uri->query = cur + 1;       /* Point to the next symbol after '?' */

                /* Find end of query.*/
                end_query = fnet_strchr( uri->query, ' ');
                if(end_query)
                {
                    *end_query = '\0';      /* Mark query end */
                    cur = end_query;
                }
                /*else wrong*/
            }
            else
            {
                cur ++;
            }
        }

        uri->extension = fnet_strrchr(uri->path, '.'); /* Find file extension. */

        if( uri->query == 0)        /* No query.*/
        {
            uri->query = cur;       /* Point to the blank string. */
        }

        if( uri->extension == 0)    /* No extension.*/
        {
            uri->extension = cur;   /* Point to the blank string. */
        }
        else
        {
            uri->extension ++;      /* Skip the point character. */
        }
    }

    return cur;
}

#if FNET_CFG_HTTP_SRV_VERSION_MAJOR /* HTTP/1.x*/
/************************************************************************
* DESCRIPTION:
************************************************************************/
static void _fnet_http_srv_version_parse(fnet_char_t *in_str, struct fnet_http_srv_version *version)
{
    fnet_char_t  *cur = in_str;
    fnet_char_t  *ptr;
    fnet_char_t  *point_ptr;

    /* rel_uri       = [ path ] [ "?" query ]*/
    if(cur && version)
    {
        /* Ignore any initial spaces */
        while (*cur == ' ')
        {
            cur++;
        }

        /* Find "HTTP/"*/
        if((cur = fnet_strstr( cur, FNET_HTTP_SRV_VERSION_HEADER )) != 0)
        {
            cur += sizeof(FNET_HTTP_SRV_VERSION_HEADER) - 1u;

            /* Find '.'*/
            if((point_ptr = fnet_strchr(cur, '.')) != 0)
            {
                *point_ptr = 0u; /* Split numbers.*/

                /*Major number*/
                version->major = (fnet_uint8_t)fnet_strtoul (cur, &ptr, 10u);
                if (!((version->major == 0u) && (ptr == cur)))
                {
                    cur = point_ptr + 1;

                    /* Minor number.*/
                    version->minor = (fnet_uint8_t)fnet_strtoul (cur, &ptr, 10u);
                    if (!((version->minor == 0u) && (ptr == cur)))
                    {
                        goto EXIT;    /* Exit.*/
                    }
                }
            }
        }

        /* No version info found.*/
        version->major = 0u;
        version->minor = 9u;
    }

EXIT:
    return;
}

/************************************************************************
* DESCRIPTION: Set response status code.
************************************************************************/
void fnet_http_srv_set_response_status_code (fnet_http_srv_session_t session, fnet_http_srv_status_code_t status_code)
{
    if(session)
    {
        fnet_service_mutex_lock();
        ((struct fnet_http_srv_session_if *)session)->response.status.code = status_code;
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: Set response content length.
************************************************************************/
void fnet_http_srv_set_response_content_length (fnet_http_srv_session_t session, fnet_size_t content_length)
{
    if(session)
    {
        fnet_service_mutex_lock();
        ((struct fnet_http_srv_session_if *)session)->response.content_length = (fnet_int32_t)content_length;
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: Disable response Status-Line and Header
************************************************************************/
void fnet_http_srv_set_response_no_header (fnet_http_srv_session_t session)
{
    if(session)
    {
        fnet_service_mutex_lock();
        ((struct fnet_http_srv_session_if *)session)->response.no_header = FNET_TRUE;
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: Receives the data from a HTTP session socket.
************************************************************************/
static fnet_int32_t _fnet_http_srv_recv(struct fnet_http_srv_session_if *session, fnet_uint8_t *buf, fnet_size_t len)
{
    fnet_ssize_t res;

#if (FNET_CFG_HTTP_SRV_TLS && FNET_CFG_TLS)
    if(session->tls_sock)
    {
        res =  fnet_tls_socket_recv(session->tls_sock, buf, len);
    }
    else
#endif
    {
        res = fnet_socket_recv(session->socket_foreign, buf, len, 0u);
    }
    return res;
}

/************************************************************************
* DESCRIPTION: Sends data on a HTTP session socket.
************************************************************************/
static fnet_int32_t _fnet_http_srv_send(struct fnet_http_srv_session_if *session, fnet_uint8_t *buf, fnet_size_t len)
{
    fnet_ssize_t res;

#if (FNET_CFG_HTTP_SRV_TLS && FNET_CFG_TLS)
    if(session->tls_sock)
    {
        res =  fnet_tls_socket_send(session->tls_sock, buf, len);
    }
    else
#endif
    {
        res = fnet_socket_send(session->socket_foreign, buf, len, 0u);
    }
    return res;
}

#endif /* FNET_CFG_HTTP_SRV_VERSION_MAJOR */

#endif /* FNET_CFG_HTTP_SRV */
