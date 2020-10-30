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
*  FNET Azure-IOT Socket IO adapter
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
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "azure_c_shared_utility/socketio.h"
#include "azure_c_shared_utility/singlylinkedlist.h"
#include "azure_c_shared_utility/optimize_size.h"
#include "azure_c_shared_utility/xlogging.h"
#include "azure_c_shared_utility/threadapi.h"

/************************************************************************
*     Definitions
*************************************************************************/
#ifndef FNET_CFG_AZURE_OPEN_BLOCKED
    #define FNET_CFG_AZURE_OPEN_BLOCKED 0
#endif

#define FNET_AZURE_RX_BUFFER_SIZE    128

typedef enum
{
    FNET_AZURE_SOCKET_IO_STATE_CLOSED,
    FNET_AZURE_SOCKET_IO_STATE_HOSTNAME_RESOLVING,
    FNET_AZURE_SOCKET_IO_STATE_CONNECTING,
    FNET_AZURE_SOCKET_IO_STATE_OPEN,
    FNET_AZURE_SOCKET_IO_STATE_CLOSING,
    FNET_AZURE_SOCKET_IO_STATE_ERROR
} FNET_AZURE_SOCKET_IO_STATE;

typedef struct
{
    unsigned char           *bytes;
    size_t                  size;
    ON_SEND_COMPLETE        on_send_complete;
    void                    *callback_context;
    SINGLYLINKEDLIST_HANDLE pending_io_list;
} FNET_AZURE_SOCKET_IO_PENDING;

typedef struct
{
    fnet_socket_t                   socket;
    ON_BYTES_RECEIVED               on_bytes_received;
    ON_IO_ERROR                     on_io_error;
    ON_IO_OPEN_COMPLETE             on_io_open_complete;
    void                            *on_bytes_received_context;
    void                            *on_io_error_context;
    void                            *on_io_open_complete_context;
    char                            *hostname;
    int                             port;
    FNET_AZURE_SOCKET_IO_STATE      io_state;
    SINGLYLINKEDLIST_HANDLE         pending_io_list;
    unsigned char                   rx_buffer[FNET_AZURE_RX_BUFFER_SIZE];
} SOCKET_IO_INSTANCE;

/************************************************************************
*     Function Prototypes
*************************************************************************/
#if FNET_CFG_AZURE_OPEN_BLOCKED
    static fnet_ip4_addr_t _fnet_azure_hostbyname(const char *host_name);
#endif
static void _fnet_azure_dns_handler_resolved(const fnet_dns_resolved_addr_t *addr_list, fnet_size_t addr_list_size, const fnet_char_t *host_name, void *cookie);

static OPTIONHANDLER_HANDLE socketio_retrieveoptions(CONCRETE_IO_HANDLE socket_io);

static void _fnet_azure_socketio_indicate_error(SOCKET_IO_INSTANCE *socket_io_instance);

static const IO_INTERFACE_DESCRIPTION socket_io_interface_description =
{
    .concrete_io_retrieveoptions = socketio_retrieveoptions,
    .concrete_io_create = socketio_create,
    .concrete_io_destroy = socketio_destroy,
    .concrete_io_open = socketio_open,
    .concrete_io_close =  socketio_close,
    .concrete_io_send = socketio_send,
    .concrete_io_dowork = socketio_dowork,
    .concrete_io_setoption = socketio_setoption
};

/*this function will clone an option given by name and value*/
static void *socketio_CloneOption(const char *name, const void *value)
{
    (void)name;
    (void)value;
    return NULL;
}

/*this function destroys an option previously created*/
static void socketio_DestroyOption(const char *name, const void *value)
{
    (void)name;
    (void)value;
}

static OPTIONHANDLER_HANDLE socketio_retrieveoptions(CONCRETE_IO_HANDLE socket_io)
{
    OPTIONHANDLER_HANDLE result;
    (void)socket_io;
    result = OptionHandler_Create(socketio_CloneOption, socketio_DestroyOption, socketio_setoption);
    if (result == NULL)
    {
        /*return as is*/
    }
    else
    {
        /*insert here work to add the options to "result" handle*/
    }
    return result;
}

static void _fnet_azure_socketio_indicate_error(SOCKET_IO_INSTANCE *socket_io_instance)
{
    if (socket_io_instance->on_io_error != NULL)
    {
        socket_io_instance->on_io_error(socket_io_instance->on_io_error_context);
    }
}

static int add_pending_io(SOCKET_IO_INSTANCE *socket_io_instance, const unsigned char *buffer, size_t size, ON_SEND_COMPLETE on_send_complete, void *callback_context)
{
    int                             result;
    FNET_AZURE_SOCKET_IO_PENDING    *pending_socket_io = (FNET_AZURE_SOCKET_IO_PENDING *)malloc(sizeof(FNET_AZURE_SOCKET_IO_PENDING));
    if (pending_socket_io == NULL)
    {
        result = __FAILURE__;
    }
    else
    {
        pending_socket_io->bytes = (unsigned char *)malloc(size);
        if (pending_socket_io->bytes == NULL)
        {
            LogError("Allocation Failure: Unable to allocate pending list.");
            free(pending_socket_io);
            result = __FAILURE__;
        }
        else
        {
            pending_socket_io->size = size;
            pending_socket_io->on_send_complete = on_send_complete;
            pending_socket_io->callback_context = callback_context;
            pending_socket_io->pending_io_list = socket_io_instance->pending_io_list;
            (void)memcpy(pending_socket_io->bytes, buffer, size);

            if (singlylinkedlist_add(socket_io_instance->pending_io_list, pending_socket_io) == NULL)
            {
                free(pending_socket_io->bytes);
                free(pending_socket_io);
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

CONCRETE_IO_HANDLE socketio_create(void *io_create_parameters)
{
    SOCKETIO_CONFIG *socket_io_config = io_create_parameters;
    SOCKET_IO_INSTANCE *result;

    if (socket_io_config == NULL)
    {
        LogError("Invalid argument: socket_io_config is NULL");
        result = NULL;
    }
    else
    {
        result = malloc(sizeof(SOCKET_IO_INSTANCE));
        if (result != NULL)
        {
            result->pending_io_list = singlylinkedlist_create();
            if (result->pending_io_list == NULL)
            {
                LogError("Failure: singlylinkedlist_create unable to create pending list.");
                free(result);
                result = NULL;
            }
            else
            {
                result->hostname = (char *)malloc(strlen(socket_io_config->hostname) + 1);
                if (result->hostname == NULL)
                {
                    singlylinkedlist_destroy(result->pending_io_list);
                    free(result);
                    result = NULL;
                }
                else
                {
                    strcpy(result->hostname, socket_io_config->hostname);
                    result->port = socket_io_config->port;
                    result->on_bytes_received = NULL;
                    result->on_io_error = NULL;
                    result->on_bytes_received_context = NULL;
                    result->on_io_error_context = NULL;
                    result->io_state = FNET_AZURE_SOCKET_IO_STATE_CLOSED;
                    result->socket = NULL;
                }
            }
        }
    }

    return result;
}

void socketio_destroy(CONCRETE_IO_HANDLE socket_io)
{
    if (socket_io != NULL)
    {
        SOCKET_IO_INSTANCE *socket_io_instance = (SOCKET_IO_INSTANCE *)socket_io;

        if (socket_io_instance->socket)
        {
            fnet_socket_close(socket_io_instance->socket);
            socket_io_instance->socket = 0;
        }

        /* clear all pending IOs */
        LIST_ITEM_HANDLE first_pending_io = singlylinkedlist_get_head_item(socket_io_instance->pending_io_list);
        while (first_pending_io != NULL)
        {
            FNET_AZURE_SOCKET_IO_PENDING *pending_socket_io = (FNET_AZURE_SOCKET_IO_PENDING *)singlylinkedlist_item_get_value(first_pending_io);
            if (pending_socket_io != NULL)
            {
                free(pending_socket_io->bytes);
                free(pending_socket_io);
            }

            (void)singlylinkedlist_remove(socket_io_instance->pending_io_list, first_pending_io);
            first_pending_io = singlylinkedlist_get_head_item(socket_io_instance->pending_io_list);
        }

        singlylinkedlist_destroy(socket_io_instance->pending_io_list);
        free(socket_io_instance->hostname);
        free(socket_io);
    }
}

#if FNET_CFG_AZURE_OPEN_BLOCKED  /* Blocked version of open */
int socketio_open(CONCRETE_IO_HANDLE socket_io, ON_IO_OPEN_COMPLETE on_io_open_complete, void *on_io_open_complete_context, ON_BYTES_RECEIVED on_bytes_received, void *on_bytes_received_context, ON_IO_ERROR on_io_error, void *on_io_error_context)
{
    int result = 0;

    SOCKET_IO_INSTANCE *socket_io_instance = (SOCKET_IO_INSTANCE *)socket_io;
    if (socket_io == NULL)
    {
        result = __FAILURE__;
    }
    else
    {
        /* TCP Socket */
        socket_io_instance->socket = fnet_socket(AF_SUPPORTED /*AF_INET*/, SOCK_STREAM, 0u);
        if (socket_io_instance->socket == FNET_NULL)
        {
            FNET_DEBUG_AZURE("[AZURE] socket() failed");
            result = __FAILURE__;
        }
        else
        {
            fnet_ip4_addr_t hostaddr;

            /* Resolve host name */
            FNET_DEBUG_AZURE("[AZURE] Resolving \"%s\"", socket_io_instance->hostname);
            hostaddr = _fnet_azure_hostbyname(socket_io_instance->hostname);
            if(hostaddr)
            {
                struct fnet_sockaddr_in    address;

                fnet_memset_zero(&address, sizeof(address));

                address.sin_port = fnet_htons(socket_io_instance->port);
                address.sin_addr.s_addr = hostaddr;
                address.sin_family =  AF_INET;

                /* Connecting */
                FNET_DEBUG_AZURE("[AZURE] Connecting...");
                if(fnet_socket_connect(socket_io_instance->socket, (struct fnet_sockaddr *)&address, sizeof(address)) == FNET_ERR)
                {
                    FNET_DEBUG_AZURE("[AZURE] connect() failed");
                    result = __FAILURE__;
                }
                else
                {
                    /* Wait for connection complete or fail */
                    fnet_socket_poll_t socket_poll;
                    socket_poll.s = socket_io_instance->socket;
                    socket_poll.events = (fnet_socket_event_t)(FNET_SOCKET_EVENT_OUT | FNET_SOCKET_EVENT_ERR); /* Connection attempt successful or failed */

                    while(fnet_socket_poll(&socket_poll, 1) == 0)
                    {
                        fnet_poll();
                    }

                    if(socket_poll.events_occurred & FNET_SOCKET_EVENT_OUT) /* Connection successful */
                    {
                        result = 0;
                    }
                    else if(socket_poll.events_occurred & FNET_SOCKET_EVENT_ERR) /* Connection failed */
                    {
                        FNET_DEBUG_AZURE("[AZURE] Connection failed");
                        result = __FAILURE__;
                    }
                }
            }
            else
            {
                FNET_DEBUG_AZURE("[AZURE] Host name resolving failed");
                result = __FAILURE__;
            }
        }
    }

    if(result == 0)
    {
        socket_io_instance->on_bytes_received = on_bytes_received;
        socket_io_instance->on_bytes_received_context = on_bytes_received_context;

        socket_io_instance->on_io_error = on_io_error;
        socket_io_instance->on_io_error_context = on_io_error_context;

        socket_io_instance->io_state = FNET_AZURE_SOCKET_IO_STATE_OPEN;
        FNET_DEBUG_AZURE("[AZURE] Connected");
    }
    else
    {
        if(socket_io_instance->socket)
        {
            fnet_socket_close(socket_io_instance->socket);
            socket_io_instance->socket = NULL;
        }
    }


    if (on_io_open_complete != NULL)
    {
        on_io_open_complete(on_io_open_complete_context, result == 0 ? IO_OPEN_OK : IO_OPEN_ERROR);
    }

    return result;
}
#else  /* Non blocking version */
int socketio_open(CONCRETE_IO_HANDLE socket_io, ON_IO_OPEN_COMPLETE on_io_open_complete, void *on_io_open_complete_context, ON_BYTES_RECEIVED on_bytes_received, void *on_bytes_received_context, ON_IO_ERROR on_io_error, void *on_io_error_context)
{
    int result = 0;

    SOCKET_IO_INSTANCE *socket_io_instance = (SOCKET_IO_INSTANCE *)socket_io;
    if (socket_io == NULL)
    {
        result = __FAILURE__;
    }
    else
    {
        /* TCP Socket */
        socket_io_instance->socket = fnet_socket(AF_SUPPORTED /*AF_INET*/, SOCK_STREAM, 0u);
        if (socket_io_instance->socket == NULL)
        {
            FNET_DEBUG_AZURE("[AZURE] socket() failed");
            result = __FAILURE__;
        }
        else
        {
            /* Resolve host name */
            {
                struct fnet_dns_params  dns_params;

                /* Set DNS client/resolver parameters.*/
                fnet_memset_zero(&dns_params, sizeof(struct fnet_dns_params));
                dns_params.addr_family = AF_INET; //TBD AF_INET6
                dns_params.host_name = socket_io_instance->hostname;    /* Host name to resolve.*/
                dns_params.callback = _fnet_azure_dns_handler_resolved; /* Callback function.*/
                dns_params.cookie = socket_io_instance;                 /* Application-specific parameter */

                /* Run DNS client/resolver. */
                if (fnet_dns_init(&dns_params) == FNET_NULL)
                {
                    FNET_DEBUG_AZURE("[AZURE] DNS resolver init failed");
                    result = __FAILURE__;
                }
                else
                {
                    FNET_DEBUG_AZURE("[AZURE] Resolving \"%s\"", socket_io_instance->hostname);
                    socket_io_instance->io_state = FNET_AZURE_SOCKET_IO_STATE_HOSTNAME_RESOLVING;
                }
            }
        }
    }

    if(result == 0)
    {
        socket_io_instance->on_bytes_received = on_bytes_received;
        socket_io_instance->on_bytes_received_context = on_bytes_received_context;

        socket_io_instance->on_io_error = on_io_error;
        socket_io_instance->on_io_error_context = on_io_error_context;

        socket_io_instance->on_io_open_complete = on_io_open_complete;
        socket_io_instance->on_io_open_complete_context = on_io_open_complete_context;
    }
    else
    {
        if(socket_io_instance->socket)
        {
            fnet_socket_close(socket_io_instance->socket);
            socket_io_instance->socket = NULL;
        }
        on_io_open_complete(on_io_open_complete_context, IO_OPEN_ERROR);
    }

    return result;
}
#endif /* FNET_CFG_AZURE_OPEN_BLOCKED */

int socketio_close(CONCRETE_IO_HANDLE socket_io, ON_IO_CLOSE_COMPLETE on_io_close_complete, void *callback_context)
{
    int result = 0;

    if (socket_io == NULL)
    {
        result = __FAILURE__;
    }
    else
    {
        SOCKET_IO_INSTANCE *socket_io_instance = (SOCKET_IO_INSTANCE *)socket_io;

        if ((socket_io_instance->io_state == FNET_AZURE_SOCKET_IO_STATE_CLOSED) ||
            (socket_io_instance->io_state == FNET_AZURE_SOCKET_IO_STATE_CLOSING))
        {
            result = __FAILURE__;
        }
        else
        {
            FNET_DEBUG_AZURE("[AZURE] close()");
            fnet_socket_close(socket_io_instance->socket);
            socket_io_instance->socket = NULL;
            socket_io_instance->io_state = FNET_AZURE_SOCKET_IO_STATE_CLOSED;

            if (on_io_close_complete != NULL)
            {
                on_io_close_complete(callback_context);
            }

            result = 0;
        }
    }

    return result;
}

int socketio_send(CONCRETE_IO_HANDLE socket_io, const void *buffer, size_t size, ON_SEND_COMPLETE on_send_complete, void *callback_context)
{
    int result;

    if ((socket_io == NULL) ||
        (buffer == NULL) ||
        (size == 0))
    {
        /* Invalid arguments */
        LogError("Invalid argument: send given invalid parameter");
        result = __FAILURE__;
    }
    else
    {
        SOCKET_IO_INSTANCE *socket_io_instance = (SOCKET_IO_INSTANCE *)socket_io;

        if (socket_io_instance->io_state == FNET_AZURE_SOCKET_IO_STATE_CLOSED)
        {
            LogError("Failure: socket state is not opened.");
            result = __FAILURE__;
        }
        else
        {
            LIST_ITEM_HANDLE first_pending_io = singlylinkedlist_get_head_item(socket_io_instance->pending_io_list);
            if (first_pending_io != NULL)
            {
                if (add_pending_io(socket_io_instance, buffer, size, on_send_complete, callback_context) != 0)
                {
                    LogError("Failure: add_pending_io failed.");
                    result = __FAILURE__;
                }
                else
                {
                    result = 0;
                }
            }
            else
            {
                /* queue data */
                if (add_pending_io(socket_io_instance, (unsigned char *)buffer, size, on_send_complete, callback_context) != 0)
                {
                    result = __FAILURE__;
                }
                else
                {
                    result = 0;
                }
            }
        }
    }

    return result;
}

void socketio_dowork(CONCRETE_IO_HANDLE socket_io)
{
    if (socket_io != NULL)
    {
        SOCKET_IO_INSTANCE *socket_io_instance = (SOCKET_IO_INSTANCE *)socket_io;

        switch(socket_io_instance->io_state)
        {
            case FNET_AZURE_SOCKET_IO_STATE_HOSTNAME_RESOLVING:
                break;
            case FNET_AZURE_SOCKET_IO_STATE_CONNECTING:
            {
                /* Wait for connection complete or fail */
                fnet_socket_poll_t socket_poll;
                socket_poll.s = socket_io_instance->socket;
                socket_poll.events = (fnet_socket_event_t)(FNET_SOCKET_EVENT_OUT | FNET_SOCKET_EVENT_ERR); /* Connection attempt successful or failed */

                if(fnet_socket_poll(&socket_poll, 1) != 0)
                {
                    if(socket_poll.events_occurred & FNET_SOCKET_EVENT_OUT) /* Connection successful */
                    {
                        socket_io_instance->io_state = FNET_AZURE_SOCKET_IO_STATE_OPEN;
                        FNET_DEBUG_AZURE("[AZURE] Connected");
                        socket_io_instance->on_io_open_complete(socket_io_instance->on_io_open_complete_context, IO_OPEN_OK);
                    }
                    else if(socket_poll.events_occurred & FNET_SOCKET_EVENT_ERR) /* Connection failed */
                    {
                        FNET_DEBUG_AZURE("[AZURE] Connection failed");
                        if(socket_io_instance->socket)
                        {
                            fnet_socket_close(socket_io_instance->socket);
                            socket_io_instance->socket = NULL;
                            socket_io_instance->io_state = FNET_AZURE_SOCKET_IO_STATE_CLOSED;
                        }
                        socket_io_instance->on_io_open_complete(socket_io_instance->on_io_open_complete_context, IO_OPEN_ERROR);
                    }
                }
            }
            break;
            case FNET_AZURE_SOCKET_IO_STATE_OPEN:
            {
                /* TX */
                LIST_ITEM_HANDLE first_pending_io = singlylinkedlist_get_head_item(socket_io_instance->pending_io_list);
                while (first_pending_io != NULL)
                {
                    FNET_AZURE_SOCKET_IO_PENDING *pending_socket_io = (FNET_AZURE_SOCKET_IO_PENDING *)singlylinkedlist_item_get_value(first_pending_io);
                    if (pending_socket_io == NULL)
                    {
                        socket_io_instance->io_state = FNET_AZURE_SOCKET_IO_STATE_ERROR;
                        _fnet_azure_socketio_indicate_error(socket_io_instance);
                        LogError("Failure: retrieving socket from list");
                        break;
                    }

                    fnet_ssize_t send_result = fnet_socket_send(socket_io_instance->socket, (const char *)pending_socket_io->bytes, pending_socket_io->size, 0);
                    if (send_result != pending_socket_io->size)
                    {
                        if (send_result < 0)
                        {
                            free(pending_socket_io->bytes);
                            free(pending_socket_io);
                            (void)singlylinkedlist_remove(socket_io_instance->pending_io_list, first_pending_io);

                            LogError("Failure: sending Socket information.");
                            socket_io_instance->io_state = FNET_AZURE_SOCKET_IO_STATE_ERROR;
                            _fnet_azure_socketio_indicate_error(socket_io_instance);
                            break;
                        }
                        else
                        {
                            /* send something, wait for the rest */
                            (void)memmove(pending_socket_io->bytes, pending_socket_io->bytes + send_result, pending_socket_io->size - send_result);
                            pending_socket_io->size -= send_result;
                        }
                    }
                    else
                    {
                        if (pending_socket_io->on_send_complete != NULL)
                        {
                            pending_socket_io->on_send_complete(pending_socket_io->callback_context, IO_SEND_OK);
                        }

                        free(pending_socket_io->bytes);
                        free(pending_socket_io);
                        if (singlylinkedlist_remove(socket_io_instance->pending_io_list, first_pending_io) != 0)
                        {
                            socket_io_instance->io_state = FNET_AZURE_SOCKET_IO_STATE_ERROR;
                            _fnet_azure_socketio_indicate_error(socket_io_instance);
                        }
                    }

                    first_pending_io = singlylinkedlist_get_head_item(socket_io_instance->pending_io_list);
                }
            }

            {
                /* RX */
                fnet_ssize_t received = 0;
                do
                {
                    received = fnet_socket_recv(socket_io_instance->socket, socket_io_instance->rx_buffer, FNET_AZURE_RX_BUFFER_SIZE, 0);
                    if (received > 0)
                    {
                        if (socket_io_instance->on_bytes_received != NULL)
                        {
                            /* Explicitly ignoring here the result of the callback */
                            (void)socket_io_instance->on_bytes_received(socket_io_instance->on_bytes_received_context, socket_io_instance->rx_buffer, received);
                        }
                    }
                    else if (received < 0)
                    {
                        // Do not log error here due to this is probably the socket being closed on the other end
                        _fnet_azure_socketio_indicate_error(socket_io_instance);
                    }

                }
                while ((received > 0) && (socket_io_instance->io_state == FNET_AZURE_SOCKET_IO_STATE_OPEN));
            }
            break;
            default:
                break;
        }

        fnet_poll();
    }
}

int socketio_setoption(CONCRETE_IO_HANDLE socket_io, const char *optionName, const void *value)
{
    /* Not implementing any options */
    return __FAILURE__;
}

const IO_INTERFACE_DESCRIPTION *socketio_get_interface_description(void)
{
    return &socket_io_interface_description;
}


/************************************************************************
* DESCRIPTION: DNS resolver callback
************************************************************************/
#if FNET_CFG_AZURE_OPEN_BLOCKED /* Blocked version of open */
fnet_bool_t _fnet_azure_dns_is_resolved = FNET_FALSE;
static void _fnet_azure_dns_handler_resolved(const fnet_dns_resolved_addr_t *addr_list, fnet_size_t addr_list_size, const fnet_char_t *host_name, void *cookie)
{
#if FNET_CFG_DEBUG_AZURE && FNET_CFG_DEBUG
    fnet_char_t             ip_str[FNET_IP_ADDR_STR_SIZE_MAX];
#endif
    fnet_ip4_addr_t *result = (fnet_ip4_addr_t *)cookie;

    if (addr_list && addr_list_size)
    {
        FNET_DEBUG_AZURE("DNS: Resolved address: %s\n", fnet_inet_ntop(addr_list->resolved_addr.sa_family,
                         addr_list->resolved_addr.sa_data, ip_str, sizeof(ip_str)) );

        /* Copy resolved address */
        if (addr_list->resolved_addr.sa_family == AF_INET)
        {
            memcpy(result, addr_list->resolved_addr.sa_data, 4);
        }
    }
    /* else.  DNS resolving failed */

    _fnet_azure_dns_is_resolved = FNET_TRUE;
}
#else
static void _fnet_azure_dns_handler_resolved(const fnet_dns_resolved_addr_t *addr_list, fnet_size_t addr_list_size, const fnet_char_t *host_name, void *cookie)
{
#if FNET_CFG_DEBUG_AZURE && FNET_CFG_DEBUG
    fnet_char_t             ip_str[FNET_IP_ADDR_STR_SIZE_MAX];
#endif
    fnet_ip4_addr_t     ip_addr = 0;
    SOCKET_IO_INSTANCE  *socket_io_instance = (SOCKET_IO_INSTANCE *)cookie;
    fnet_return_t       result = FNET_ERR;

    if (addr_list && addr_list_size)
    {
        struct fnet_sockaddr_in    address;

        FNET_DEBUG_AZURE("[AZURE] DNS: Resolved address: %s\n", fnet_inet_ntop(addr_list->resolved_addr.sa_family, addr_list->resolved_addr.sa_data, ip_str, sizeof(ip_str)) );

        /* Copy resolved address */
        if (addr_list->resolved_addr.sa_family == AF_INET)
        {
            memcpy(&ip_addr, addr_list->resolved_addr.sa_data, 4);
        }

        /* Connecting */
        fnet_memset_zero(&address, sizeof(address));

        address.sin_port = fnet_htons(socket_io_instance->port);
        address.sin_addr.s_addr = ip_addr;
        address.sin_family =  AF_INET;

        /* Connecting */
        FNET_DEBUG_AZURE("[AZURE] Connecting...");
        result =  fnet_socket_connect(socket_io_instance->socket, (struct fnet_sockaddr *)&address, sizeof(address));
        if( result == FNET_OK)
        {
            socket_io_instance->io_state = FNET_AZURE_SOCKET_IO_STATE_CONNECTING;
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
        if(socket_io_instance->socket)
        {
            fnet_socket_close(socket_io_instance->socket);
            socket_io_instance->socket = NULL;
            socket_io_instance->io_state = FNET_AZURE_SOCKET_IO_STATE_CLOSED;
        }
        socket_io_instance->on_io_open_complete(socket_io_instance->on_io_open_complete_context, IO_OPEN_ERROR);
    }
}

#endif /* FNET_CFG_AZURE_OPEN_BLOCKED */


/************************************************************************
* Resolve a host name using Domain Name Service.
* host_name - The host name to resolve.
* Returns the IPv4 address of the specified host, otherwise zero on error.
************************************************************************/
#if FNET_CFG_AZURE_OPEN_BLOCKED
static fnet_ip4_addr_t _fnet_azure_hostbyname(const char *host_name)
{
    fnet_ip4_addr_t         result = 0;
    struct fnet_dns_params  dns_params;
    fnet_netif_desc_t       netif = fnet_netif_get_default();

    /* Set DNS client/resolver parameters.*/
    fnet_memset_zero(&dns_params, sizeof(struct fnet_dns_params));
    dns_params.addr_family = AF_INET;
    dns_params.host_name = host_name;                   /* Host name to resolve.*/
    dns_params.callback = _fnet_azure_dns_handler_resolved; /* Callback function.*/
    dns_params.cookie = &result;                         /* Application-specific parameter */

    _fnet_azure_dns_is_resolved = FNET_FALSE;
    /* Run DNS client/resolver. */
    if (fnet_dns_init(&dns_params) != FNET_NULL)
    {
        while (_fnet_azure_dns_is_resolved == FNET_FALSE) /* Wait for resolving */
        {
            fnet_poll();
            ThreadAPI_Sleep(1);
        }
    }

    return result;
}
#endif

#endif /* FNET_CFG_AZURE */
