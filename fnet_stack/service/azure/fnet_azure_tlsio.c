/**************************************************************************
*
* Copyright 2020 by Andrej Butok. FNET Community.
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
*  FNET Azure-IOT TLS IO adapter
*
***************************************************************************/
#include "fnet.h"

#if FNET_CFG_AZURE

#if FNET_CFG_DEBUG_AZURE && FNET_CFG_DEBUG
    #define FNET_DEBUG_AZURE   FNET_DEBUG
#else
    #define FNET_DEBUG_AZURE(...) do{}while(0)
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "azure_c_shared_utility/optimize_size.h"
#include "azure_c_shared_utility/tlsio.h"
#include "azure_c_shared_utility/singlylinkedlist.h"
#include "azure_c_shared_utility/socketio.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/shared_util_options.h"

typedef enum
{
    FNET_AZURE_TLSIO_STATE_NOT_OPEN,
    FNET_AZURE_TLSIO_STATE_HOSTNAME_RESOLVING,
    FNET_AZURE_TLSIO_STATE_CONNECTING,
    FNET_AZURE_TLSIO_STATE_OPEN,
    FNET_AZURE_TLSIO_STATE_ERROR
} TLSIO_STATE_ENUM;

typedef struct
{
    unsigned char           *bytes;
    size_t                  size;
    ON_SEND_COMPLETE        on_send_complete;
    void                    *callback_context;
    SINGLYLINKEDLIST_HANDLE pending_io_list;
} FNET_AZURE_TLSIO_PENDING;

typedef struct
{
    TLSIO_STATE_ENUM        tlsio_state;
    ON_BYTES_RECEIVED       on_bytes_received;
    void                    *on_bytes_received_context;
    ON_IO_OPEN_COMPLETE     on_io_open_complete;
    void                    *on_io_open_complete_context;
    ON_IO_ERROR             on_io_error;
    void                    *on_io_error_context;
    char                    *trusted_certificates;
    char                    *hostname;
    int                     port;
    fnet_tls_desc_t         tls_desc;
    fnet_tls_socket_t       tls_sock;   /* TLS socket descriptor. */
    fnet_socket_t           sock;
    SINGLYLINKEDLIST_HANDLE pending_io_list;
} FNET_AZURE_TLS_IO_INSTANCE;

static CONCRETE_IO_HANDLE fnet_azure_tlsio_create(void *io_create_parameters);
static void fnet_azure_tlsio_destroy(CONCRETE_IO_HANDLE tlsio);
static int fnet_azure_tlsio_open(CONCRETE_IO_HANDLE tls_io, ON_IO_OPEN_COMPLETE on_io_open_complete, void *on_io_open_complete_context, ON_BYTES_RECEIVED on_bytes_received, void *on_bytes_received_context, ON_IO_ERROR on_io_error, void *on_io_error_context);
static void _fnet_azure_dns_handler_resolved(const fnet_dns_resolved_addr_t *addr_list, fnet_size_t addr_list_size, const fnet_char_t *host_name, void *cookie);
static void fnet_azure_tlsio_mbedtls_dowork(CONCRETE_IO_HANDLE tls_io);
static int fnet_azure_tlsio_setoption(CONCRETE_IO_HANDLE tls_io, const char *optionName, const void *value);
static OPTIONHANDLER_HANDLE fnet_azure_tlsio_retrieveoptions(CONCRETE_IO_HANDLE handle);
static void fnet_azure_tlsio_destroy_option(const char *name, const void *value);
static int fnet_azure_tlsio_close(CONCRETE_IO_HANDLE tls_io, ON_IO_CLOSE_COMPLETE on_io_close_complete, void *callback_context);
static int fnet_azure_tlsio_send(CONCRETE_IO_HANDLE tls_io, const void *buffer, size_t size, ON_SEND_COMPLETE on_send_complete, void *callback_context);
static int _fnet_azure_tlsio_add_pending_io(FNET_AZURE_TLS_IO_INSTANCE *tlsio_instance, const unsigned char *buffer, size_t size, ON_SEND_COMPLETE on_send_complete, void *callback_context);
static void _fnet_azure_tlsio_on_error(FNET_AZURE_TLS_IO_INSTANCE *tls_io_instance);
static void _fnet_azure_tlsio_on_open_complete(FNET_AZURE_TLS_IO_INSTANCE *tls_io_instance, IO_OPEN_RESULT open_result);

/************************************************************************
* TLS IO Description
************************************************************************/
static const IO_INTERFACE_DESCRIPTION _fnet_azure_tlsio_interface_description =
{
    .concrete_io_retrieveoptions = fnet_azure_tlsio_retrieveoptions,
    .concrete_io_create = fnet_azure_tlsio_create,
    .concrete_io_destroy = fnet_azure_tlsio_destroy,
    .concrete_io_open = fnet_azure_tlsio_open,
    .concrete_io_close = fnet_azure_tlsio_close,
    .concrete_io_send = fnet_azure_tlsio_send,
    .concrete_io_dowork = fnet_azure_tlsio_mbedtls_dowork,
    .concrete_io_setoption = fnet_azure_tlsio_setoption
};

/************************************************************************
* On error
************************************************************************/
static void _fnet_azure_tlsio_on_error(FNET_AZURE_TLS_IO_INSTANCE *tls_io_instance)
{
    tls_io_instance->tlsio_state = FNET_AZURE_TLSIO_STATE_ERROR;

    if (tls_io_instance->on_io_error != NULL)
    {
        tls_io_instance->on_io_error(tls_io_instance->on_io_error_context);
    }
}

/************************************************************************
* On open complete
************************************************************************/
static void _fnet_azure_tlsio_on_open_complete(FNET_AZURE_TLS_IO_INSTANCE *tls_io_instance, IO_OPEN_RESULT open_result)
{
    if (tls_io_instance->on_io_open_complete != NULL)
    {
        tls_io_instance->on_io_open_complete(tls_io_instance->on_io_open_complete_context, open_result);
    }
}

/************************************************************************
* concrete_io_create
************************************************************************/
static CONCRETE_IO_HANDLE fnet_azure_tlsio_create(void *io_create_parameters)
{
    TLSIO_CONFIG                *tls_io_config = io_create_parameters;
    FNET_AZURE_TLS_IO_INSTANCE  *tlsio_instance = NULL;

    if ((tls_io_config == NULL) || (tls_io_config->hostname == NULL))
    {
        LogError("[TLSIO] NULL tls_io_config");
        goto ERROR;
    }
    else
    {
        tlsio_instance = malloc(sizeof(FNET_AZURE_TLS_IO_INSTANCE));
        if (tlsio_instance != NULL)
        {
            fnet_memset_zero(tlsio_instance, sizeof(*tlsio_instance));

            tlsio_instance->hostname = (char *)malloc(strlen(tls_io_config->hostname) + 1);
            if (tlsio_instance->hostname == NULL)
            {
                goto ERROR;
            }
            else
            {
                strcpy(tlsio_instance->hostname, tls_io_config->hostname);
                tlsio_instance->port = tls_io_config->port;

                tlsio_instance->on_bytes_received = NULL;
                tlsio_instance->on_bytes_received_context = NULL;

                tlsio_instance->on_io_open_complete = NULL;
                tlsio_instance->on_io_open_complete_context = NULL;

                tlsio_instance->on_io_error = NULL;
                tlsio_instance->on_io_error_context = NULL;

                tlsio_instance->trusted_certificates = NULL;

                tlsio_instance->pending_io_list = singlylinkedlist_create();
                if (tlsio_instance->pending_io_list == NULL)
                {
                    LogError("Failure: singlylinkedlist_create unable to create pending list.");
                    goto ERROR;
                }

                /* TLS initialize */
                tlsio_instance->tls_desc = fnet_tls_init(FNET_TLS_ROLE_CLIENT);
                if(tlsio_instance->tls_desc == FNET_NULL)
                {
                    LogError("[TLSIO] TLS initialization error");
                    goto ERROR;
                }

                tlsio_instance->tlsio_state = FNET_AZURE_TLSIO_STATE_NOT_OPEN;
            }
        }
    }

    return tlsio_instance;

ERROR:
    if(tlsio_instance)
    {
        if(tlsio_instance->hostname)
        {
            free(tlsio_instance->hostname);
        }

        if(tlsio_instance->pending_io_list)
        {
            singlylinkedlist_destroy(tlsio_instance->pending_io_list);
        }

        free(tlsio_instance);
    }

    return NULL;
}

/************************************************************************
*  concrete_io_destroy
************************************************************************/
static void fnet_azure_tlsio_destroy(CONCRETE_IO_HANDLE tlsio)
{
    if (tlsio != NULL)
    {
        FNET_AZURE_TLS_IO_INSTANCE *tlsio_instance = (FNET_AZURE_TLS_IO_INSTANCE *)tlsio;

        /* Close TLS session */
        fnet_tls_socket_close(tlsio_instance->tls_sock);
        fnet_socket_close(tlsio_instance->sock);

        fnet_tls_release(tlsio_instance->tls_desc);

        if (tlsio_instance->trusted_certificates != NULL)
        {
            free(tlsio_instance->trusted_certificates);
        }

        /* Clear all pending IOs */
        LIST_ITEM_HANDLE first_pending_io = singlylinkedlist_get_head_item(tlsio_instance->pending_io_list);
        while (first_pending_io != NULL)
        {
            FNET_AZURE_TLSIO_PENDING *pending_tlsio = (FNET_AZURE_TLSIO_PENDING *)singlylinkedlist_item_get_value(first_pending_io);
            if (pending_tlsio != NULL)
            {
                free(pending_tlsio->bytes);
                free(pending_tlsio);
            }

            (void)singlylinkedlist_remove(tlsio_instance->pending_io_list, first_pending_io);
            first_pending_io = singlylinkedlist_get_head_item(tlsio_instance->pending_io_list);
        }

        singlylinkedlist_destroy(tlsio_instance->pending_io_list);

        free(tlsio_instance->hostname);
        free(tlsio_instance);
    }
}

/************************************************************************
*  concrete_io_open
************************************************************************/
static int fnet_azure_tlsio_open(CONCRETE_IO_HANDLE tls_io, ON_IO_OPEN_COMPLETE on_io_open_complete, void *on_io_open_complete_context, ON_BYTES_RECEIVED on_bytes_received, void *on_bytes_received_context, ON_IO_ERROR on_io_error, void *on_io_error_context)
{
    int result = 0;

    if (tls_io == NULL)
    {
        LogError("[TLSIO] NULL tls_io");
        result = __FAILURE__;
    }
    else
    {
        FNET_AZURE_TLS_IO_INSTANCE *tls_io_instance = (FNET_AZURE_TLS_IO_INSTANCE *)tls_io;

        if (tls_io_instance->tlsio_state != FNET_AZURE_TLSIO_STATE_NOT_OPEN)
        {
            LogError("[TLSIO] IO should not be open: %d\n", tls_io_instance->tlsio_state);
            result = __FAILURE__;
        }
        else
        {
            tls_io_instance->on_bytes_received = on_bytes_received;
            tls_io_instance->on_bytes_received_context = on_bytes_received_context;

            tls_io_instance->on_io_open_complete = on_io_open_complete;
            tls_io_instance->on_io_open_complete_context = on_io_open_complete_context;

            tls_io_instance->on_io_error = on_io_error;
            tls_io_instance->on_io_error_context = on_io_error_context;

            /* TCP Socket */
            tls_io_instance->sock = fnet_socket(AF_SUPPORTED /*AF_INET*/, SOCK_STREAM, 0u);
            if (tls_io_instance->sock == NULL)
            {
                FNET_DEBUG_AZURE("[AZURE] socket() failed");
                result = __FAILURE__;
            }
            else
                /* Resolve host name */
            {
                struct fnet_dns_params  dns_params;

                /* Set DNS client/resolver parameters.*/
                fnet_memset_zero(&dns_params, sizeof(struct fnet_dns_params));
                dns_params.addr_family = AF_INET;
                dns_params.host_name = tls_io_instance->hostname;       /* Host name to resolve.*/
                dns_params.callback = _fnet_azure_dns_handler_resolved; /* Callback function.*/
                dns_params.cookie = tls_io_instance;                    /* Application-specific parameter */

                /* Run DNS client/resolver. */
                if (fnet_dns_init(&dns_params) == FNET_NULL)
                {
                    LogError("[AZURE] DNS resolver init failed");

                    fnet_socket_close(tls_io_instance->sock);
                    tls_io_instance->sock = NULL;

                    result = __FAILURE__;
                }
                else
                {
                    FNET_DEBUG_AZURE("[AZURE] Resolving \"%s\"", tls_io_instance->hostname);
                    tls_io_instance->tlsio_state = FNET_AZURE_TLSIO_STATE_HOSTNAME_RESOLVING;
                    result = 0;
                }
            }
        }
    }

    return result;
}

/************************************************************************
*  DNS resolver callback
************************************************************************/
static void _fnet_azure_dns_handler_resolved(const fnet_dns_resolved_addr_t *addr_list, fnet_size_t addr_list_size, const fnet_char_t *host_name, void *cookie)
{
#if FNET_CFG_DEBUG_AZURE && FNET_CFG_DEBUG
    fnet_char_t             ip_str[FNET_IP_ADDR_STR_SIZE_MAX];
#endif
    FNET_AZURE_TLS_IO_INSTANCE *tls_io_instance = (FNET_AZURE_TLS_IO_INSTANCE *)cookie;
    fnet_return_t               result = FNET_ERR;

    if (addr_list && addr_list_size)
    {
        struct fnet_sockaddr_in    address;

        /* Take first resolved address. */
        FNET_DEBUG_AZURE("[AZURE] DNS: Resolved address: %s\n", fnet_inet_ntop(addr_list->resolved_addr.sa_family, addr_list->resolved_addr.sa_data, ip_str, sizeof(ip_str)) );

        /* Copy resolved address */
        memcpy(&address, &addr_list->resolved_addr, sizeof(address));
        address.sin_port = fnet_htons(tls_io_instance->port);

        /* Connecting */
        FNET_DEBUG_AZURE("[AZURE] Connecting...");
        result =  fnet_socket_connect(tls_io_instance->sock, (struct fnet_sockaddr *)&address, sizeof(address));
        if( result == FNET_OK)
        {
            tls_io_instance->tlsio_state = FNET_AZURE_TLSIO_STATE_CONNECTING;
        }
        else
        {
            FNET_DEBUG_AZURE("[AZURE] Connect failed");
        }
    }
    else
    {
        FNET_DEBUG_AZURE("[AZURE] DNS Resolving failed");
    }

    if(result == FNET_ERR)
    {
        if(tls_io_instance->sock)
        {
            fnet_socket_close(tls_io_instance->sock);
            tls_io_instance->sock = NULL;
            tls_io_instance->tlsio_state = FNET_AZURE_TLSIO_STATE_NOT_OPEN;
        }

        _fnet_azure_tlsio_on_open_complete(tls_io_instance, IO_OPEN_ERROR);
    }
}

/************************************************************************
*  concrete_io_close
************************************************************************/
static int fnet_azure_tlsio_close(CONCRETE_IO_HANDLE tls_io, ON_IO_CLOSE_COMPLETE on_io_close_complete, void *callback_context)
{
    int result = 0;

    if (tls_io == NULL)
    {
        result = __FAILURE__;
    }
    else
    {
        FNET_AZURE_TLS_IO_INSTANCE *tls_io_instance = (FNET_AZURE_TLS_IO_INSTANCE *)tls_io;

        if (tls_io_instance->tlsio_state == FNET_AZURE_TLSIO_STATE_NOT_OPEN)
        {
            result = __FAILURE__;
        }
        else
        {
            fnet_tls_socket_close(tls_io_instance->tls_sock);
            tls_io_instance->tls_sock = FNET_NULL;
            fnet_socket_close(tls_io_instance->sock);
            tls_io_instance->sock = FNET_NULL;

            tls_io_instance->tlsio_state = FNET_AZURE_TLSIO_STATE_NOT_OPEN;

            if (on_io_close_complete != NULL)
            {
                on_io_close_complete(callback_context);
            }

            result = 0;
        }
    }

    return result;
}

/************************************************************************
*  Add pending IO send.
************************************************************************/
static int _fnet_azure_tlsio_add_pending_io(FNET_AZURE_TLS_IO_INSTANCE *tlsio_instance, const unsigned char *buffer, size_t size, ON_SEND_COMPLETE on_send_complete, void *callback_context)
{
    int                         result;
    FNET_AZURE_TLSIO_PENDING    *pending_tlsio = (FNET_AZURE_TLSIO_PENDING *)malloc(sizeof(FNET_AZURE_TLSIO_PENDING));

    if (pending_tlsio == NULL)
    {
        result = __FAILURE__;
    }
    else
    {
        pending_tlsio->bytes = (unsigned char *)malloc(size);
        if (pending_tlsio->bytes == NULL)
        {
            LogError("Allocation Failure: Unable to allocate pending list.");
            free(pending_tlsio);
            result = __FAILURE__;
        }
        else
        {
            pending_tlsio->size = size;
            pending_tlsio->on_send_complete = on_send_complete;
            pending_tlsio->callback_context = callback_context;
            pending_tlsio->pending_io_list = tlsio_instance->pending_io_list;
            (void)memcpy(pending_tlsio->bytes, buffer, size);

            if (singlylinkedlist_add(tlsio_instance->pending_io_list, pending_tlsio) == NULL)
            {
                free(pending_tlsio->bytes);
                free(pending_tlsio);
                result = __FAILURE__;
            }
            else
            {
                result = 0;
            }
        }
    }

    return result;
}

/************************************************************************
*  concrete_io_send
************************************************************************/
static int fnet_azure_tlsio_send(CONCRETE_IO_HANDLE tls_io, const void *buffer, size_t size, ON_SEND_COMPLETE on_send_complete, void *callback_context)
{
    int result;

    if ((tls_io == NULL) || (buffer == NULL) || (size == 0))
    {
        result = __FAILURE__;
    }
    else
    {
        FNET_AZURE_TLS_IO_INSTANCE *tlsio_instance = (FNET_AZURE_TLS_IO_INSTANCE *)tls_io;

        if (tlsio_instance->tlsio_state != FNET_AZURE_TLSIO_STATE_OPEN)
        {
            result = __FAILURE__;
        }
        else
        {
            /* Queue data */
            if (_fnet_azure_tlsio_add_pending_io(tlsio_instance, buffer, size, on_send_complete, callback_context) != 0)
            {
                result = __FAILURE__;
            }
            else
            {
                result = 0;
            }
        }
    }

    return result;
}

/************************************************************************
*  TLS adapter poll
************************************************************************/
static void fnet_azure_tlsio_mbedtls_dowork(CONCRETE_IO_HANDLE tls_io)
{
    if (tls_io != NULL)
    {
        FNET_AZURE_TLS_IO_INSTANCE *tls_io_instance = (FNET_AZURE_TLS_IO_INSTANCE *)tls_io;

        switch(tls_io_instance->tlsio_state)
        {
            case FNET_AZURE_TLSIO_STATE_HOSTNAME_RESOLVING:
                break;
            case FNET_AZURE_TLSIO_STATE_CONNECTING:
            {
                /* Wait for connection complete or fail */
                fnet_socket_poll_t  socket_poll;

                socket_poll.s = tls_io_instance->sock;
                socket_poll.events = (fnet_socket_event_t)(FNET_SOCKET_EVENT_OUT | FNET_SOCKET_EVENT_ERR); /* Connection attempt successful or failed */

                if(fnet_socket_poll(&socket_poll, 1) != 0)
                {
                    if(socket_poll.events_occurred & FNET_SOCKET_EVENT_OUT) /* Connection successful */
                    {
                        fnet_return_t result;

                        FNET_DEBUG_AZURE("[AZURE] Connected");

                        tls_io_instance->tls_sock = fnet_tls_socket(tls_io_instance->tls_desc, tls_io_instance->sock);
                        if(tls_io_instance->tls_sock == 0)
                        {
                            FNET_DEBUG_AZURE("[AZURE] TLS socket() failed");
                            if(tls_io_instance->sock)
                            {
                                fnet_socket_close(tls_io_instance->sock);
                                tls_io_instance->sock = NULL;
                                tls_io_instance->tlsio_state = FNET_AZURE_TLSIO_STATE_NOT_OPEN;
                            }

                            _fnet_azure_tlsio_on_open_complete(tls_io_instance, IO_OPEN_ERROR);
                        }
                        else
                        {
                            fnet_tls_socket_set_hostname(tls_io_instance->tls_sock, tls_io_instance->hostname);

                            /* TLS handshake */
                            result = fnet_tls_socket_connect(tls_io_instance->tls_sock);

                            if (result == FNET_OK)
                            {
                                FNET_DEBUG_AZURE("[AZURE] TLS handshake completed");
                                tls_io_instance->tlsio_state = FNET_AZURE_TLSIO_STATE_OPEN;
                                _fnet_azure_tlsio_on_open_complete(tls_io_instance, IO_OPEN_OK);
                            }
                            else
                            {
                                FNET_DEBUG_AZURE("[AZURE] TLS handshake failed");
                                if(tls_io_instance->sock)
                                {
                                    fnet_socket_close(tls_io_instance->sock);
                                    tls_io_instance->sock = NULL;
                                    tls_io_instance->tlsio_state = FNET_AZURE_TLSIO_STATE_NOT_OPEN;
                                }

                                _fnet_azure_tlsio_on_open_complete(tls_io_instance, IO_OPEN_ERROR);
                            }
                        }
                    }
                    else if(socket_poll.events_occurred & FNET_SOCKET_EVENT_ERR) /* Connection failed */
                    {
                        FNET_DEBUG_AZURE("[AZURE] Connection failed");

                        if(tls_io_instance->sock)
                        {
                            fnet_socket_close(tls_io_instance->sock);
                            tls_io_instance->sock = NULL;
                            tls_io_instance->tlsio_state = FNET_AZURE_TLSIO_STATE_NOT_OPEN;
                        }

                        _fnet_azure_tlsio_on_open_complete(tls_io_instance, IO_OPEN_ERROR);
                    }
                }
            }
            break;
            case FNET_AZURE_TLSIO_STATE_OPEN:
            {
                /* TX */
                LIST_ITEM_HANDLE pending_item = singlylinkedlist_get_head_item(tls_io_instance->pending_io_list);
                if (pending_item != NULL)
                {
                    FNET_AZURE_TLSIO_PENDING   *pending_tlsio = (FNET_AZURE_TLSIO_PENDING *)singlylinkedlist_item_get_value(pending_item);
                    fnet_ssize_t                send_result;
                    if (pending_tlsio == NULL)
                    {
                        LogError("Failure: retrieving socket from list");
                        _fnet_azure_tlsio_on_error(tls_io_instance);
                        break;
                    }

                    send_result = fnet_tls_socket_send(tls_io_instance->tls_sock, pending_tlsio->bytes, pending_tlsio->size);
                    if (send_result != pending_tlsio->size)
                    {
                        if (send_result < 0) /* Error */
                        {
                            free(pending_tlsio->bytes);
                            free(pending_tlsio);
                            (void)singlylinkedlist_remove(tls_io_instance->pending_io_list, pending_item);

                            LogError("Failure: sending Socket information.");
                            _fnet_azure_tlsio_on_error(tls_io_instance);
                            break;
                        }
                        else
                        {
                            /* Send something, wait for the rest */
                            (void)memmove(pending_tlsio->bytes, pending_tlsio->bytes + send_result, pending_tlsio->size - send_result);
                            pending_tlsio->size -= send_result;
                        }
                    }
                    else /* Completed */
                    {
                        if (pending_tlsio->on_send_complete != NULL)
                        {
                            pending_tlsio->on_send_complete(pending_tlsio->callback_context, IO_SEND_OK);
                        }

                        free(pending_tlsio->bytes);
                        free(pending_tlsio);
                        if (singlylinkedlist_remove(tls_io_instance->pending_io_list, pending_item) != 0)
                        {
                            _fnet_azure_tlsio_on_error(tls_io_instance);
                            break;
                        }
                    }
                }
            }

                if(tls_io_instance->tlsio_state == FNET_AZURE_TLSIO_STATE_OPEN) /* If still open after TX */
            {
                /* Rx */
                fnet_ssize_t    rcv_bytes;
                unsigned char   buffer[64];

                rcv_bytes =  fnet_tls_socket_recv(tls_io_instance->tls_sock, buffer, sizeof(buffer));
                if (rcv_bytes > 0)
                {
                    if (tls_io_instance->on_bytes_received != NULL)
                    {
                        tls_io_instance->on_bytes_received(tls_io_instance->on_bytes_received_context, buffer, rcv_bytes);
                    }
                }
                else  if(rcv_bytes < 0) /* Error */
                {
                    _fnet_azure_tlsio_on_error(tls_io_instance);
                }
            }
            break;
            default:
                break;
        }
        fnet_poll();
    }
}

/************************************************************************
* Get TLS IO descriptor.
************************************************************************/
const IO_INTERFACE_DESCRIPTION *fnet_azure_tlsio_get_interface_description(void)
{
    return &_fnet_azure_tlsio_interface_description;
}

/************************************************************************
* Clone an option given by name and value.
************************************************************************/
static void *tlsio_mbedtls_clone_option(const char *name, const void *value)
{
    void *result = NULL;

    if (name && value)
    {
        if (strcmp(name, OPTION_TRUSTED_CERT) == 0)
        {
            if (mallocAndStrcpy_s((char **)&result, value) != 0)
            {
                LogError("unable to mallocAndStrcpy_s TrustedCerts value");
            }
        }
        else
        {
            LogError("Not handled option : %s", name);
        }
    }
    return result;
}

/************************************************************************
* Destroy an option previously created
************************************************************************/
static void fnet_azure_tlsio_destroy_option(const char *name, const void *value)
{
    if ( name && value )
    {
        if (strcmp(name, OPTION_TRUSTED_CERT) == 0)
        {
            free((void *)value);
        }
        else
        {
            LogError("Not handled option : %s", name);
        }
    }
}

/************************************************************************
*  concrete_io_retrieveoptions
************************************************************************/
static OPTIONHANDLER_HANDLE fnet_azure_tlsio_retrieveoptions(CONCRETE_IO_HANDLE handle)
{
    OPTIONHANDLER_HANDLE result = NULL;

    if (handle)
    {
        result = OptionHandler_Create(tlsio_mbedtls_clone_option, fnet_azure_tlsio_destroy_option, fnet_azure_tlsio_setoption);
        if (result == NULL)
        {
            LogError("OptionHandler_Create failed");
        }
        else
        {
            FNET_AZURE_TLS_IO_INSTANCE *tls_io_instance = (FNET_AZURE_TLS_IO_INSTANCE *)handle;

            /* Certificates */
            if ((tls_io_instance->trusted_certificates != NULL) &&
                OptionHandler_AddOption(result, OPTION_TRUSTED_CERT, tls_io_instance->trusted_certificates) != OPTIONHANDLER_OK)
            {
                LogError("unable to save TrustedCerts option");
                OptionHandler_Destroy(result);
                result = NULL;
            }
        }
    }
    return result;
}

/************************************************************************
*  concrete_io_setoption
************************************************************************/
static int fnet_azure_tlsio_setoption(CONCRETE_IO_HANDLE tls_io, const char *optionName, const void *value)
{
    int result = __FAILURE__;

    if(tls_io && optionName)
    {
        FNET_AZURE_TLS_IO_INSTANCE *tls_io_instance = (FNET_AZURE_TLS_IO_INSTANCE *)tls_io;

        if (strcmp(OPTION_TRUSTED_CERT, optionName) == 0)
        {
            if (tls_io_instance->trusted_certificates != NULL)
            {
                /* Free the memory if it has been previously allocated */
                free(tls_io_instance->trusted_certificates);
                tls_io_instance->trusted_certificates = NULL;
            }

            if (mallocAndStrcpy_s(&tls_io_instance->trusted_certificates, (const char *)value) != 0)
            {
                LogError(" mallocAndStrcpy_s failed");
            }
            else
            {
                fnet_return_t res =  fnet_tls_set_ca_certificate(tls_io_instance->tls_desc, value, (strlen(value) + 1));
                if (res == FNET_ERR)
                {
                    LogInfo("Malformed certificate");
                }
                else
                {
                    result = 0; /* OK */
                }
            }
        }
    }

    return result;
}

#endif /* FNET_CFG_AZURE */
