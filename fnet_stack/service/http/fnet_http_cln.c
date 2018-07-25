/**************************************************************************
*
* Copyright 2018 by Andrey Butok. FNET Community.
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
*  FNET HTTP client implementation.
*
***************************************************************************/
#include "fnet.h"

#if FNET_CFG_HTTP_CLN

#if FNET_CFG_DEBUG_HTTP_CLN && FNET_CFG_DEBUG
    #define FNET_DEBUG_HTTP_CLN   FNET_DEBUG
#else
    #define FNET_DEBUG_HTTP_CLN(...) do{}while(0)
#endif

/************************************************************************
*     Definitions
*************************************************************************/

/* Error strings.*/
#define FNET_HTTP_CLN_ERR_PARAMS             "[HTTP cln] Wrong input parameters"
#define FNET_HTTP_CLN_ERR_SOCKET_CREATION    "[HTTP cln] Socket creation error"
#define FNET_HTTP_CLN_ERR_SOCKET_CONNECT     "[HTTP cln] Socket connect error"
#define FNET_HTTP_CLN_ERR_SOCKET_OPTION      "[HTTP cln] Socket option error"
#define FNET_HTTP_CLN_ERR_SERVICE            "[HTTP cln] Service registration error."
#define FNET_HTTP_CLN_ERR_IS_INITIALIZED     "[HTTP cln] Is already initialized."

#define FNET_HTTP_CLN_VERSION_HEADER    "HTTP/1.0" /* Protocol version HTTP/x.x*/
#define FNET_HTTP_CLN_METHOD_NAME       "GET" /*GET, POST, PUT, PATCH, DELETE, HEAD, OPTIONS*/

typedef enum
{
    FNET_HTTP_CLN_STATE_DISABLED = 0,  /* HTTP client is not initialized.*/
    FNET_HTTP_CLN_STATE_CONNECTING,    /* HTTP client is connecting (TCP).*/
    FNET_HTTP_CLN_STATE_TX,            /* HTTP client is transmitting.*/
    FNET_HTTP_CLN_STATE_CLOSE,         /* HTTP client is closing session.*/
} fnet_http_cln_state_t;

/************************************************************************
*    HTTP client interface structure.
*************************************************************************/
typedef struct fnet_http_cln_if
{
    fnet_bool_t                             is_enabled;
    fnet_service_desc_t                     service_descriptor;
    fnet_http_cln_state_t                   state;
    struct fnet_sockaddr                    address_foreign;
    fnet_socket_t                           socket_foreign;     /* Foreign socket.*/
    fnet_uint8_t                            buffer[FNET_CFG_HTTP_CLN_BUFFER_SIZE]; /* Internal buffer */
    fnet_index_t                            buffer_offset;
    fnet_size_t                             message_size;

    //TBD
    void                                    *cookie;
    fnet_time_t                             time_begin;
} fnet_http_cln_if_t;

/* The HTTP client interface list*/
static  fnet_http_cln_if_t fnet_http_cln_if_list[FNET_CFG_HTTP_CLN];

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void _fnet_http_cln_poll(void *fnet_http_cln_if_p);

/************************************************************************
* DESCRIPTION: Initialize the HTTP client service.
************************************************************************/
fnet_http_cln_desc_t fnet_http_cln_init( struct fnet_http_cln_params *params )
{
    fnet_index_t            i;
    fnet_http_cln_if_t      *http_cln_if = FNET_NULL;
    fnet_char_t             *uri;

    fnet_service_mutex_lock();

    if((params == 0)  ) //TBD
    {
        FNET_DEBUG_HTTP_CLN(FNET_HTTP_CLN_ERR_PARAMS);
        goto ERROR_1;
    }

    /* Try to find free HTTP client. */
    for(i = 0u; i < FNET_CFG_HTTP_CLN; i++)
    {
        if(fnet_http_cln_if_list[i].is_enabled == FNET_FALSE)
        {
            http_cln_if = &fnet_http_cln_if_list[i];
            break;
        }
    }

    /* Is HTTP client is already initialized. */
    if(http_cln_if == 0)
    {
        FNET_DEBUG_HTTP_CLN(FNET_HTTP_CLN_ERR_IS_INITIALIZED);
        goto ERROR_1;
    }

    /* Clear all parameters.*/
    fnet_memset_zero(http_cln_if, sizeof(*http_cln_if));

    /* Set parameters */
    fnet_memcpy(&http_cln_if->address_foreign, &params->address, sizeof(http_cln_if->address_foreign));
    if(http_cln_if->address_foreign.sa_port == 0u)
    {
        http_cln_if->address_foreign.sa_port = FNET_CFG_HTTP_CLN_PORT;
    }

    http_cln_if->cookie = params->cookie;

    //TBD

    /* Create socket */
    http_cln_if->socket_foreign = fnet_socket(http_cln_if->address_foreign.sa_family,  SOCK_STREAM, 0u);
    if(http_cln_if->socket_foreign == FNET_NULL)
    {
        FNET_DEBUG_HTTP_CLN(FNET_HTTP_CLN_ERR_SOCKET_CREATION);
        goto ERROR_1;
    }

    /* Connect */
    if(fnet_socket_connect(http_cln_if->socket_foreign, &http_cln_if->address_foreign, sizeof(http_cln_if->address_foreign)) == FNET_ERR)
    {
        FNET_DEBUG_HTTP_CLN(FNET_HTTP_CLN_ERR_SOCKET_CONNECT);
        goto ERROR_2;
    }

    http_cln_if->service_descriptor = fnet_service_register(_fnet_http_cln_poll, (void *)http_cln_if);
    if(http_cln_if->service_descriptor == 0)
    {
        FNET_DEBUG_HTTP_CLN(FNET_HTTP_CLN_ERR_SERVICE);
        goto ERROR_2;
    }

    http_cln_if->state = FNET_HTTP_CLN_STATE_CONNECTING;

    /* Build request message (without message body) */
    if(params->uri)
    {
        uri = params->uri; //TBD check for /
    }
    else
    {
        uri = "/";
    }
    /* GET /index.html HTTP/1.1 */
    //DM message_size = fnet_strlen(FNET_HTTP_CLN_METHOD_NAME)+1+fnet_strlen(uri)+ fnet_strlen(FNET_HTTP_CLN_VERSION_HEADER);
    fnet_strlcpy ((fnet_char_t *)http_cln_if->buffer, FNET_HTTP_CLN_METHOD_NAME, sizeof(http_cln_if->buffer));
    fnet_strlcat ((fnet_char_t *)http_cln_if->buffer, " ", sizeof(http_cln_if->buffer));
    fnet_strlcat ((fnet_char_t *)http_cln_if->buffer, uri, sizeof(http_cln_if->buffer));
    fnet_strlcat ((fnet_char_t *)http_cln_if->buffer, " ", sizeof(http_cln_if->buffer));
    fnet_strlcat ((fnet_char_t *)http_cln_if->buffer, FNET_HTTP_CLN_VERSION_HEADER, sizeof(http_cln_if->buffer));
    fnet_strlcat ((fnet_char_t *)http_cln_if->buffer, "\r\n\r\n", sizeof(http_cln_if->buffer));
    http_cln_if->message_size = fnet_strlen((fnet_char_t *)http_cln_if->buffer);

    http_cln_if->is_enabled = FNET_TRUE;

    fnet_service_mutex_unlock();

    return (fnet_http_cln_desc_t)http_cln_if;

ERROR_2:
    fnet_socket_close(http_cln_if->socket_foreign);
ERROR_1:
    fnet_service_mutex_unlock();
    return FNET_NULL;
}

/************************************************************************
* DESCRIPTION: Release the HTTP client service.
************************************************************************/
void fnet_http_cln_release(fnet_http_cln_desc_t desc)
{
    fnet_http_cln_if_t     *http_cln_if = (fnet_http_cln_if_t *)desc;

    if(http_cln_if && http_cln_if->is_enabled)
    {
        fnet_service_mutex_lock();

        http_cln_if->is_enabled = FNET_FALSE;

        fnet_service_unregister(http_cln_if->service_descriptor); /* Delete service. */

        fnet_socket_close(http_cln_if->socket_foreign);

        http_cln_if->state = FNET_HTTP_CLN_STATE_DISABLED;

        /* Inform a user application about the session end */
        //TBD

        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: HTTP client state machine.
************************************************************************/
static void _fnet_http_cln_poll( void *fnet_http_cln_if_p )
{
    fnet_http_cln_if_t  *http_cln_if = (fnet_http_cln_if_t *)fnet_http_cln_if_p;
    fnet_ssize_t        send_result;

    if(http_cln_if)
    {
        switch(http_cln_if->state)
        {
            case FNET_HTTP_CLN_STATE_CONNECTING:            /* HTTP client is connecting.*/
            {
                fnet_socket_poll_t socket_poll;
                socket_poll.s = http_cln_if->socket_foreign;
                socket_poll.events = (fnet_socket_event_t)(FNET_SOCKET_EVENT_OUT | FNET_SOCKET_EVENT_ERR); /* Connection attempt successful or failed */

                if(fnet_socket_poll(&socket_poll, 1))
                {
                    if(socket_poll.events_occurred & FNET_SOCKET_EVENT_OUT) /* Connection successful */
                    {



                        http_cln_if->state = FNET_HTTP_CLN_STATE_TX;
                    }
                    else if(socket_poll.events_occurred & FNET_SOCKET_EVENT_ERR) /* Connection failed */
                    {
                        fnet_http_cln_release(http_cln_if);
                    }
                }
            }
            break;
            case FNET_HTTP_CLN_STATE_TX:   /* HTTP client is sending.*/
            {
                fnet_bool_t     is_session_end = FNET_FALSE;

                send_result = fnet_socket_send(http_cln_if->socket_foreign, &http_cln_if->buffer[http_cln_if->buffer_offset], (http_cln_if->message_size - http_cln_if->buffer_offset), 0);

                if (send_result == FNET_ERR)
                {
                    is_session_end = FNET_TRUE;
                }
                else if(send_result)
                {
                    http_cln_if->buffer_offset += send_result;
                }
                else /* 0 */
                {}

                if(is_session_end == FNET_TRUE)
                {
                    fnet_http_cln_release(http_cln_if);
                }
            }
            break;
            default:
                break;
        }
    }
}

/************************************************************************
* DESCRIPTION: Detects if the HTTP client service is enabled or disabled.
************************************************************************/
fnet_bool_t fnet_http_cln_is_enabled(fnet_http_cln_desc_t desc)
{
    struct fnet_http_cln_if    *http_cln_if = (struct fnet_http_cln_if *)desc;
    fnet_bool_t                 result;

    if(http_cln_if)
    {
        result = http_cln_if->is_enabled;
    }
    else
    {
        result = FNET_FALSE;
    }

    return result;
}


#endif /* FNET_CFG_HTTP_CLN */
