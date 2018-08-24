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
*  TFTP Client implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_TFTP_CLN

#if FNET_CFG_DEBUG_TFTP_CLN && FNET_CFG_DEBUG
    #define FNET_DEBUG_TFTP         FNET_DEBUG
#else
    #define FNET_DEBUG_TFTP(...)    do {}while(0)
#endif

/************************************************************************
*     Definitions
*************************************************************************/
#define FNET_TFTP_MODE                  "octet"

#define FNET_TFTP_MODE_SIZE_MAX         (9u)
#define FNET_TFTP_FILENAME_SIZE_MAX     (FNET_TFTP_DATA_SIZE_MAX-FNET_TFTP_MODE_SIZE_MAX)

#define FNET_TFTP_OPCODE_READ_REQUEST   (1u)
#define FNET_TFTP_OPCODE_WRITE_REQUEST  (2u)
#define FNET_TFTP_OPCODE_DATA           (3u)
#define FNET_TFTP_OPCODE_ACK            (4u)
#define FNET_TFTP_OPCODE_ERROR          (5u)

#define FNET_TFTP_ERR_PARAMS            "ERROR: Wrong input parameters."
#define FNET_TFTP_ERR_SOCKET_CREATION   "ERROR: Socket creation error."
#define FNET_TFTP_ERR_SOCKET_BIND       "ERROR: Socket Error during bind."
#define FNET_TFTP_ERR_SERVICE           "ERROR: Service registration is failed."
#define FNET_TFTP_ERR_IS_INITIALIZED    "ERROR: TFTP is already initialized."

static void _fnet_tftp_cln_poll(void *fnet_tftp_cln_if_p);

/* TFTP packets:*/
FNET_COMP_PACKED_BEGIN
struct fnet_tftp_packet_request
{
    fnet_uint16_t opcode FNET_COMP_PACKED;
    fnet_char_t  filename_mode[FNET_TFTP_DATA_SIZE_MAX] FNET_COMP_PACKED; /* Filename, Mode */
};
FNET_COMP_PACKED_END

FNET_COMP_PACKED_BEGIN
struct fnet_tftp_packet_data
{
    fnet_uint16_t opcode FNET_COMP_PACKED;
    fnet_uint16_t block_number FNET_COMP_PACKED;
    fnet_uint8_t  data[FNET_TFTP_DATA_SIZE_MAX] FNET_COMP_PACKED;
};
FNET_COMP_PACKED_END

FNET_COMP_PACKED_BEGIN
struct fnet_tftp_packet_ack
{
    fnet_uint16_t opcode FNET_COMP_PACKED;
    fnet_uint16_t block_number FNET_COMP_PACKED;
};
FNET_COMP_PACKED_END

FNET_COMP_PACKED_BEGIN
struct fnet_tftp_packet_error
{
    fnet_uint16_t   opcode FNET_COMP_PACKED;
    fnet_uint16_t   error_code FNET_COMP_PACKED;
    fnet_char_t    error_message[FNET_TFTP_DATA_SIZE_MAX] FNET_COMP_PACKED;
};
FNET_COMP_PACKED_END

#if 0 /* Actually it's not needed as a TFT server sends error string message with packet. */
const static fnet_uint8_t *fnet_tftp_error[] =
{
    "Not defined, see error message (if any).",
    "File not found.",
    "Access violation.",
    "Disk full or allocation exceeded.",
    "Illegal TFTP operation.",
    "Unknown transfer ID.",
    "File already exists.",
    "No such user."
};
#endif

/* Default error message. */
static fnet_uint8_t FNET_TFTP_DEFAULT_ERROR[] = "Connection failed";

/************************************************************************
*    TFTP-client interface structure
*************************************************************************/
typedef struct
{
    fnet_socket_t           socket_client;
    fnet_service_desc_t     service_descriptor;
    fnet_tftp_request_t     request_type;
    fnet_tftp_cln_state_t   state;                  /* Current state.*/
    struct fnet_sockaddr    server_addr;            /* TFTP Server address. */
    fnet_tftp_cln_handler_t handler;                /* Callback function. */
    void                    *handler_param;         /* Handler specific parameter. */
    fnet_uint16_t           server_port;            /* TFTP Server port number for data transfer. */
    fnet_uint16_t           block_number_ack;       /* Acknoladged block number. */
    fnet_time_t             last_time_ms;           /* Last receive time, used for timeout detection. */
    fnet_time_t             timeout;                /* Timeout in ms. */
    union
    {
        struct fnet_tftp_packet_request packet_request;
        struct fnet_tftp_packet_data    packet_data;
        struct fnet_tftp_packet_ack     packet_ack;
        struct fnet_tftp_packet_error   packet_error;
    } packet;
    fnet_size_t             packet_size;
    fnet_size_t             tx_data_size;
}
fnet_tftp_if_t;

/* TFTP-client interface */
static fnet_tftp_if_t fnet_tftp_if;

/************************************************************************
* DESCRIPTION: TFTP-client initialization.
************************************************************************/
fnet_return_t fnet_tftp_cln_init( fnet_tftp_cln_params_t *params )
{
    struct fnet_sockaddr    addr_client;
    fnet_char_t             *data_ptr;

    fnet_service_mutex_lock();

    /* Check input parameters. */
    if((params == 0) || (params->server_addr.sa_family == AF_UNSPEC) ||
       (fnet_socket_addr_is_unspecified(&params->server_addr)) ||
       (params->file_name == 0) || (params->handler == 0))
    {
        FNET_DEBUG_TFTP(FNET_TFTP_ERR_PARAMS);
        goto ERROR;
    }

    if(fnet_tftp_if.state != FNET_TFTP_CLN_STATE_DISABLED)
    {
        FNET_DEBUG_TFTP(FNET_TFTP_ERR_IS_INITIALIZED);
        goto ERROR;
    }

    /* Reset interface structure. */
    fnet_memset_zero(&fnet_tftp_if, sizeof(fnet_tftp_if_t));

    fnet_memcpy(&fnet_tftp_if.server_addr, &params->server_addr, sizeof(fnet_tftp_if.server_addr));

    if(fnet_tftp_if.server_addr.sa_port == 0u)
    {
        fnet_tftp_if.server_addr.sa_port = FNET_CFG_TFTP_CLN_PORT; /* Default TFTP server port number.*/
    }

    fnet_tftp_if.handler = params->handler;
    fnet_tftp_if.handler_param = params->handler_param;

    if(params->timeout == 0u )
    {
        fnet_tftp_if.timeout = FNET_CFG_TFTP_CLN_TIMEOUT * 1000u;
    }
    else
    {
        fnet_tftp_if.timeout = (fnet_time_t) params->timeout * 1000u;
    }

    /* Create client socket */
    fnet_tftp_if.socket_client = fnet_socket(params->server_addr.sa_family, SOCK_DGRAM, 0u);
    if(fnet_tftp_if.socket_client == FNET_NULL)
    {
        FNET_DEBUG_TFTP(FNET_TFTP_ERR_SOCKET_CREATION);
        goto ERROR;
    }

    /* Bind socket (optional) */
    fnet_memset_zero(&addr_client, sizeof(addr_client));
    addr_client.sa_family = params->server_addr.sa_family;

    if ( fnet_socket_bind( fnet_tftp_if.socket_client, (struct fnet_sockaddr *)&addr_client, sizeof(addr_client)) == FNET_ERR  )
    {
        FNET_DEBUG_TFTP(FNET_TFTP_ERR_SOCKET_BIND);
        goto ERROR_1;
    }

    fnet_tftp_if.request_type = params->request_type;

    /* Prepare request. */
    switch(fnet_tftp_if.request_type)
    {
        case FNET_TFTP_REQUEST_WRITE:
            fnet_tftp_if.packet.packet_request.opcode = FNET_HTONS(FNET_TFTP_OPCODE_WRITE_REQUEST);
            break;
        case FNET_TFTP_REQUEST_READ:
            fnet_tftp_if.packet.packet_request.opcode = FNET_HTONS(FNET_TFTP_OPCODE_READ_REQUEST);
            break;
        default:
            goto ERROR_1;
    }

    data_ptr = fnet_tftp_if.packet.packet_request.filename_mode;
    fnet_strlcpy( data_ptr, params->file_name, FNET_TFTP_FILENAME_SIZE_MAX);
    data_ptr += fnet_strlen(params->file_name) + 1u;
    fnet_strlcpy((fnet_char_t *)data_ptr, FNET_TFTP_MODE, FNET_TFTP_MODE_SIZE_MAX);

    fnet_tftp_if.packet_size = sizeof(fnet_tftp_if.packet.packet_request.opcode) + fnet_strlen(params->file_name) + 1u + sizeof(FNET_TFTP_MODE);

    /* Register TFTP service. */
    fnet_tftp_if.service_descriptor = fnet_service_register(_fnet_tftp_cln_poll, (void *) &fnet_tftp_if);
    if(fnet_tftp_if.service_descriptor == 0)
    {
        FNET_DEBUG_TFTP(FNET_TFTP_ERR_SERVICE);
        goto ERROR_1;
    }

    fnet_tftp_if.state = FNET_TFTP_CLN_STATE_SEND_REQUEST; /* => Send REQUEST */

    fnet_service_mutex_unlock();
    return FNET_OK;
ERROR_1:
    fnet_socket_close(fnet_tftp_if.socket_client);

ERROR:
    fnet_service_mutex_unlock();
    return FNET_ERR;
}

/************************************************************************
* DESCRIPTION: TFTP-client state machine.
************************************************************************/
static void _fnet_tftp_cln_poll( void *fnet_tftp_cln_if_p )
{
    struct fnet_sockaddr    addr;
    fnet_size_t             addr_len;
    fnet_ssize_t            sent_size;
    fnet_int32_t            received;
    fnet_tftp_if_t          *tftp_if = (fnet_tftp_if_t *)fnet_tftp_cln_if_p;

    switch(tftp_if->state)
    {
        /*---- SEND_REQUEST --------------------------------------------*/
        case FNET_TFTP_CLN_STATE_SEND_REQUEST:
            fnet_memcpy(&addr, &fnet_tftp_if.server_addr, sizeof(addr));

            /* ---- Send request ---- */
            sent_size = fnet_socket_sendto(tftp_if->socket_client, &tftp_if->packet.packet_request, tftp_if->packet_size, 0u, &addr, sizeof(addr));

            if (sent_size != (fnet_int32_t)tftp_if->packet_size)
            {
                goto ERROR;
            }
            else
            {
                tftp_if->last_time_ms = fnet_timer_get_ms();
                tftp_if->state = FNET_TFTP_CLN_STATE_HANDLE_REQUEST;
            }
            break;
        /*---- HANDLE_REQUEST -----------------------------------------------*/
        case  FNET_TFTP_CLN_STATE_HANDLE_REQUEST:
            /* Receive data */
            addr_len = sizeof(addr);

            received = fnet_socket_recvfrom  (tftp_if->socket_client, &tftp_if->packet.packet_data, sizeof(struct fnet_tftp_packet_data), 0u,
                                              &addr, &addr_len );

            if(received >= 4)
            {
                /* Is it for us. */
                if(!fnet_socket_addr_are_equal(&addr, &tftp_if->server_addr))
                {
                    FNET_DEBUG_TFTP( "\nWARNING: Block not from our server!" );
                }
                /* Error. */
                else if ( tftp_if->packet.packet_data.opcode == FNET_HTONS(FNET_TFTP_OPCODE_ERROR) )
                {
                    tftp_if->handler(tftp_if->request_type, (fnet_uint8_t *)&tftp_if->packet.packet_error.error_message[0], (unsigned long)fnet_htons(tftp_if->packet.packet_error.error_code), FNET_ERR, tftp_if->handler_param);
                    tftp_if->state = FNET_TFTP_CLN_STATE_RELEASE;
                }
                /* Received Data. */
                else if( (tftp_if->request_type == FNET_TFTP_REQUEST_READ) && (tftp_if->packet.packet_data.opcode == FNET_HTONS(FNET_TFTP_OPCODE_DATA)) )
                {
                    if(tftp_if->server_port == 0u)
                    {
                        tftp_if->server_port = addr.sa_port; /* Save port of the first session only. */
                    }

                    /* Is it our session. */
                    if(tftp_if->server_port == addr.sa_port)
                    {
                        /* Send ACK */
                        tftp_if->packet.packet_ack.opcode = FNET_HTONS(FNET_TFTP_OPCODE_ACK);
                        fnet_socket_sendto(tftp_if->socket_client, &tftp_if->packet.packet_ack, sizeof(struct fnet_tftp_packet_ack), 0u,
                                           (struct fnet_sockaddr *)&addr, sizeof(addr) );

                        /* Reset timeout. */
                        tftp_if->last_time_ms = fnet_timer_get_ms();

                        /* Message the application. */
                        if((tftp_if->block_number_ack + 1u) == fnet_htons(tftp_if->packet.packet_data.block_number))
                        {
                            tftp_if->block_number_ack ++;

                            /* Call Rx handler. */
                            if(tftp_if->handler(tftp_if->request_type, (fnet_uint8_t *)&tftp_if->packet.packet_data.data[0], (unsigned long)(received - 4), FNET_OK, tftp_if->handler_param) == FNET_ERR)
                            {
                                tftp_if->state = FNET_TFTP_CLN_STATE_RELEASE;
                                break;
                            }

                            /* Check return result.*/
                            if(received < (fnet_int32_t)sizeof(struct fnet_tftp_packet_data)) /* EOF */
                            {
                                fnet_tftp_if.state = FNET_TFTP_CLN_STATE_RELEASE;   /* => RELEASE */
                            }
                        }
                    }
                }
                /* Received ACK. */
                else if ((tftp_if->request_type == FNET_TFTP_REQUEST_WRITE) && (tftp_if->packet.packet_data.opcode == FNET_HTONS(FNET_TFTP_OPCODE_ACK)))
                {
                    if(tftp_if->block_number_ack == 0u)
                    {
                        tftp_if->server_port = addr.sa_port; /* Save port number of the first ACK only. */
                    }

                    /* Is it our session. */
                    if(tftp_if->server_port == addr.sa_port)
                    {
                        if(tftp_if->block_number_ack == fnet_ntohs(tftp_if->packet.packet_data.block_number)) /* Correct ACK. */
                        {
                            /* Last ACK. */
                            if((tftp_if->block_number_ack) && ((tftp_if->tx_data_size) < sizeof(tftp_if->packet.packet_data.data)))
                            {
                                tftp_if->state = FNET_TFTP_CLN_STATE_RELEASE;
                                break;
                            }
                            else /* More data to send. */
                            {
                                tftp_if->block_number_ack++;

                                if((sent_size = tftp_if->handler(tftp_if->request_type, (fnet_uint8_t *)&tftp_if->packet.packet_data.data[0],
                                                                 (fnet_uint16_t)(sizeof(tftp_if->packet.packet_data.data)),
                                                                 FNET_OK, tftp_if->handler_param)) == FNET_ERR)
                                {
                                    tftp_if->state = FNET_TFTP_CLN_STATE_RELEASE;
                                    break;
                                }

                                tftp_if->tx_data_size = (fnet_size_t)sent_size;
                            }
                        }
                        /* else: Resend last packet. */

                        /* Send data. */
                        tftp_if->packet.packet_data.opcode = FNET_HTONS(FNET_TFTP_OPCODE_DATA);
                        tftp_if->packet.packet_data.block_number = fnet_htons(tftp_if->block_number_ack);
                        fnet_socket_sendto(tftp_if->socket_client, &tftp_if->packet.packet_data, (4u + (tftp_if->tx_data_size)), 0u,
                                           &addr, sizeof(addr) );
                        /* Reset timeout. */
                        tftp_if->last_time_ms = fnet_timer_get_ms();
                    }

                }
                /* Wrong opration. */
                else
                {
                    goto ERROR;
                }
            }
            else
            {
                /* Check error. Check timeout */
                if((received == FNET_ERR) ||
                   ((fnet_timer_get_ms() - tftp_if->last_time_ms) > fnet_tftp_if.timeout))
                {
                    goto ERROR;
                }

            }
            break;
        /*---- RELEASE -------------------------------------------------*/
        case FNET_TFTP_CLN_STATE_RELEASE:
        default:
            fnet_tftp_cln_release();
            break;
    }

    return;

ERROR:
    tftp_if->state = FNET_TFTP_CLN_STATE_RELEASE; /* => RELEASE */
    tftp_if->handler(tftp_if->request_type, (fnet_uint8_t *)FNET_TFTP_DEFAULT_ERROR, 0u, FNET_ERR, tftp_if->handler_param);
}

/************************************************************************
* DESCRIPTION: TFTP-client release.
************************************************************************/
void fnet_tftp_cln_release(void)
{
    fnet_service_mutex_lock();
    if(fnet_tftp_if.state != FNET_TFTP_CLN_STATE_DISABLED)
    {
        /* Close socket. */
        fnet_socket_close(fnet_tftp_if.socket_client);

        /* Unregister the tftp service. */
        fnet_service_unregister( fnet_tftp_if.service_descriptor );

        fnet_tftp_if.state = FNET_TFTP_CLN_STATE_DISABLED;
    }
    fnet_service_mutex_unlock();
}

/************************************************************************
* DESCRIPTION: This function returns a current state of the TFTP client.
************************************************************************/
fnet_tftp_cln_state_t fnet_tftp_cln_state(void)
{
    return fnet_tftp_if.state;
}

#endif
