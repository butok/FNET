/**************************************************************************
*
* Copyright 2008-2016 by Andrey Butok. FNET Community.
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
*  TFTP Server implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_TFTP_SRV


#if FNET_CFG_DEBUG_TFTP_SRV && FNET_CFG_DEBUG
    #define FNET_DEBUG_TFTP_SRV         FNET_DEBUG
#else
    #define FNET_DEBUG_TFTP_SRV(...)    do {}while(0)
#endif

/************************************************************************
*     Definitions
*************************************************************************/
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
#define FNET_TFTP_ERR_IS_INITIALIZED    "ERROR: TFTP Server is already initialized."

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
    fnet_uint16_t opcode FNET_COMP_PACKED;
    fnet_uint16_t error_code FNET_COMP_PACKED;
    fnet_uint8_t error_message[FNET_TFTP_DATA_SIZE_MAX] FNET_COMP_PACKED;
};
FNET_COMP_PACKED_END

static const fnet_char_t *fnet_tftp_srv_error[] =
{
    "Not defined", /* see error message (if any).*/
    "File not found",
    "Access violation",
    "Disk full or allocation exceeded",
    "Illegal TFTP operation",
    "Unknown transfer ID",
    "File already exists",
    "No such user"
};

#define FNET_TFTP_SRV_ERR_MAX   (sizeof(fnet_tftp_srv_error)/sizeof(fnet_uint8_t*))

/************************************************************************
*    TFTP server interface structure
*************************************************************************/
struct fnet_tftp_srv_if
{
    fnet_tftp_srv_state_t   state;                  /* Current state.*/
    fnet_socket_t           socket_listen;          /* Listening socket.*/
    fnet_socket_t           socket_transaction;     /* Socket servicing transaction.*/
    struct fnet_sockaddr    addr_transaction;

    fnet_service_desc_t     service_descriptor;

    fnet_tftp_srv_request_handler_t     request_handler;
    fnet_tftp_srv_data_handler_t        data_handler;
    fnet_tftp_srv_complete_handler_t    complete_handler;
    fnet_return_t           complete_status;        /* FNET_OK or FNET_ERR */
    void                    *handler_param;         /* Handler specific parameter. */

    fnet_tftp_request_t     request_type;
    void                    (*request_send)(struct fnet_tftp_srv_if *tftp_srv_if);

    fnet_uint16_t           block_number_ack;       /* Acknoladged block number. */
    fnet_time_t             last_time_ms;           /* Last receive time, used for timeout detection. */
    fnet_time_t             timeout_ms;             /* Timeout in ms. */
    fnet_index_t            retransmit_max;
    fnet_index_t            retransmit_cur;
    union
    {
        struct fnet_tftp_packet_request packet_request;
        struct fnet_tftp_packet_data    packet_data;
        struct fnet_tftp_packet_ack     packet_ack;
        struct fnet_tftp_packet_error   packet_error;
    } packet;
    fnet_size_t             packet_size;
    fnet_size_t             tx_data_size;
};

/* The TFTP Server interface */
static struct fnet_tftp_srv_if tftp_srv_if_list[FNET_CFG_TFTP_SRV];

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void _fnet_tftp_srv_poll( void *fnet_tftp_srv_if_p );
static void _fnet_tftp_srv_send_error(struct fnet_tftp_srv_if *tftp_srv_if, fnet_socket_t s, fnet_tftp_error_t error_code, const fnet_char_t *error_message, struct fnet_sockaddr *dest_addr);
static void _fnet_tftp_srv_send_data(struct fnet_tftp_srv_if *tftp_srv_if);
static void _fnet_tftp_srv_send_ack(struct fnet_tftp_srv_if *tftp_srv_if);
static fnet_int32_t _fnet_tftp_srv_data_handler(struct fnet_tftp_srv_if *tftp_srv_if, fnet_size_t data_size);

/************************************************************************
* DESCRIPTION: TFTP server initialization.
************************************************************************/
fnet_tftp_srv_desc_t fnet_tftp_srv_init( fnet_tftp_srv_params_t *params )
{
    struct fnet_sockaddr    local_addr;
    fnet_index_t            i;
    struct fnet_tftp_srv_if *tftp_srv_if = 0;

    fnet_service_mutex_lock();

    /* Check input paramters. */
    if((params == 0) || (params->data_handler == 0) || (params->request_handler == 0))
    {
        FNET_DEBUG_TFTP_SRV("TFTP_SRV: Wrong init parameters.");
        goto ERROR_1;
    }

    /* Try to find free TFTP server descriptor. */
    for(i = 0u; i < FNET_CFG_TFTP_SRV; i++)
    {
        if(tftp_srv_if_list[i].state == FNET_TFTP_SRV_STATE_DISABLED)
        {
            tftp_srv_if = &tftp_srv_if_list[i];
        }
    }

    if(tftp_srv_if == 0)
    {
        /* No free TFTP server descriptor. */
        FNET_DEBUG_TFTP_SRV("TFTP_SRV: No free TFTP Server.");
        goto ERROR_1;
    }

    /* Reset interface structure. */
    fnet_memset_zero(tftp_srv_if, sizeof(struct fnet_tftp_srv_if));

    tftp_srv_if->request_handler    = params->request_handler;
    tftp_srv_if->data_handler       = params->data_handler;
    tftp_srv_if->complete_handler   = params->complete_handler;
    tftp_srv_if->handler_param      = params->handler_param;

    if(params->timeout == 0u )
    {
        tftp_srv_if->timeout_ms = FNET_CFG_TFTP_SRV_TIMEOUT * 1000U;
    }
    else
    {
        tftp_srv_if->timeout_ms = params->timeout * 1000U;
    }

    if(params->retransmit_max == 0U)
    {
        tftp_srv_if->retransmit_max = FNET_CFG_TFTP_SRV_RETRANSMIT_MAX;
    }
    else
    {
        tftp_srv_if->retransmit_max = params->retransmit_max;
    }

    tftp_srv_if->socket_transaction = FNET_NULL;

    fnet_memcpy(&local_addr, &params->address, sizeof(local_addr));

    if(local_addr.sa_port == 0u)
    {
        local_addr.sa_port = FNET_CFG_TFTP_SRV_PORT; /* Aply the default port. */
    }

    if(local_addr.sa_family == AF_UNSPEC)
    {
        local_addr.sa_family = AF_SUPPORTED; /* Assign supported families.*/
    }

    /* Create listen socket */
    tftp_srv_if->socket_listen = fnet_socket(local_addr.sa_family, SOCK_DGRAM, 0u);
    if(tftp_srv_if->socket_listen == FNET_NULL)
    {
        FNET_DEBUG_TFTP_SRV("TFTP_SRV: Socket creation error.");
        goto ERROR_1;
    }

    if(fnet_socket_bind(tftp_srv_if->socket_listen, &local_addr, sizeof(local_addr)) == FNET_ERR)
    {
        FNET_DEBUG_TFTP_SRV("TFTP_SRV: Socket bind error.");
        goto ERROR_2;
    }

    /* Register service. */
    tftp_srv_if->service_descriptor = fnet_service_register(_fnet_tftp_srv_poll, (void *) tftp_srv_if);

    if(tftp_srv_if->service_descriptor == 0)
    {
        FNET_DEBUG_TFTP_SRV("TFTP_SRV: Service registration error.");
        goto ERROR_2;
    }

    tftp_srv_if->state = FNET_TFTP_SRV_STATE_WAITING_REQUEST; /* => Send WAITING_REQUEST */

    fnet_service_mutex_unlock();
    return (fnet_tftp_srv_desc_t)tftp_srv_if;
ERROR_2:
    fnet_socket_close(tftp_srv_if->socket_listen);
ERROR_1:
    fnet_service_mutex_unlock();
    return 0;
}

/************************************************************************
* DESCRIPTION: Send TFTP error packet.
************************************************************************/
static void _fnet_tftp_srv_send_error(struct fnet_tftp_srv_if *tftp_srv_if, fnet_socket_t s, fnet_tftp_error_t error_code, const fnet_char_t *error_message, struct fnet_sockaddr *dest_addr)
{
    /*        2 bytes   2 bytes        string    1 byte
             ------------------------------------------
    ERROR   |   05   |  ErrorCode   |  ErrMsg  |  0    |
             ------------------------------------------
    */

    tftp_srv_if->packet.packet_error.opcode = FNET_HTONS(FNET_TFTP_OPCODE_ERROR);
    tftp_srv_if->packet.packet_error.error_code = fnet_htons((fnet_uint16_t)error_code);

    if((error_message == 0) && (error_code < FNET_TFTP_SRV_ERR_MAX))
    {
        error_message = fnet_tftp_srv_error[error_code]; /* Stanndard error message acording to RFC783. */
    }

    if(error_message)
    {
        fnet_strlcpy( (fnet_char_t *)tftp_srv_if->packet.packet_error.error_message, error_message, FNET_TFTP_DATA_SIZE_MAX - 1u );
    }
    else
    {
        tftp_srv_if->packet.packet_error.error_message[0] = 0u;
    }

    fnet_socket_sendto(s, &tftp_srv_if->packet.packet_error, fnet_strlen(error_message) + (2u + 2u + 1u), 0u,
                       dest_addr, sizeof(*dest_addr));
}

/************************************************************************
* DESCRIPTION: Send TFTP data packet.
************************************************************************/
static void _fnet_tftp_srv_send_data(struct fnet_tftp_srv_if *tftp_srv_if)
{
    /* Send data. */
    tftp_srv_if->packet.packet_data.opcode = FNET_HTONS(FNET_TFTP_OPCODE_DATA);
    tftp_srv_if->packet.packet_data.block_number = fnet_htons(tftp_srv_if->block_number_ack);
    fnet_socket_sendto(tftp_srv_if->socket_transaction, &tftp_srv_if->packet.packet_data, (4u + tftp_srv_if->tx_data_size), 0u,
                       &tftp_srv_if->addr_transaction, sizeof(tftp_srv_if->addr_transaction) );
    /* Reset timeout. */
    tftp_srv_if->last_time_ms = fnet_timer_get_ms();
}

/************************************************************************
* DESCRIPTION: Send TFTP acknowledge packet.
************************************************************************/
static void _fnet_tftp_srv_send_ack(struct fnet_tftp_srv_if *tftp_srv_if)
{
    /* Send acknowledge. */
    tftp_srv_if->packet.packet_ack.opcode = FNET_HTONS(FNET_TFTP_OPCODE_ACK);
    tftp_srv_if->packet.packet_ack.block_number = fnet_htons(tftp_srv_if->block_number_ack);
    fnet_socket_sendto(tftp_srv_if->socket_transaction, &tftp_srv_if->packet.packet_ack, sizeof(struct fnet_tftp_packet_ack), 0u,
                       &tftp_srv_if->addr_transaction, sizeof(tftp_srv_if->addr_transaction) );
    /* Reset timeout. */
    tftp_srv_if->last_time_ms = fnet_timer_get_ms();
}

/************************************************************************
* DESCRIPTION: Call TFTP data handler.
************************************************************************/
static fnet_int32_t _fnet_tftp_srv_data_handler(struct fnet_tftp_srv_if *tftp_srv_if, fnet_size_t data_size)
{
    fnet_tftp_error_t   error_code = FNET_TFTP_ERROR_NOT_DEFINED;
    fnet_char_t         *error_message = 0;
    fnet_int32_t        result;

    if((result = tftp_srv_if->data_handler( tftp_srv_if->request_type,
                                            (fnet_uint8_t *)&tftp_srv_if->packet.packet_data.data[0],
                                            data_size,
                                            &error_code,
                                            &error_message,
                                            tftp_srv_if->handler_param)) == FNET_ERR)
    {
        /* Send error. */
        _fnet_tftp_srv_send_error(tftp_srv_if, tftp_srv_if->socket_transaction, error_code, error_message, &tftp_srv_if->addr_transaction);
        tftp_srv_if->state = FNET_TFTP_SRV_STATE_CLOSE;   /* => CLOSE */
    }
    else
    {
        tftp_srv_if->block_number_ack++;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: TFTP server state machine.
************************************************************************/
static void _fnet_tftp_srv_poll( void *fnet_tftp_srv_if_p )
{
    struct fnet_sockaddr    addr;
    fnet_size_t             addr_len;
    fnet_ssize_t            received;
    fnet_ssize_t            sent;
    struct fnet_tftp_srv_if *tftp_srv_if = (struct fnet_tftp_srv_if *)fnet_tftp_srv_if_p;
    fnet_tftp_error_t       error_code;
    fnet_char_t             *error_message;
    fnet_char_t             *filename;        /* null-terminated.*/
    fnet_char_t             *mode;            /* null-terminated.*/
    fnet_index_t            i;
    fnet_return_t           result;

    switch(tftp_srv_if->state)
    {
        /*---- WAITING_REQUEST --------------------------------------------*/
        case FNET_TFTP_SRV_STATE_WAITING_REQUEST:
            addr_len = sizeof(addr);

            received = fnet_socket_recvfrom(tftp_srv_if->socket_listen, &tftp_srv_if->packet.packet_request,
                                            sizeof(struct fnet_tftp_packet_request), 0u,
                                            &addr, &addr_len );
            if(received >= 4)
            {
                /* Extract opcode, filename and mode. */

                /*         2 bytes   string      1 byte   string     1 byte
                *         ----------------------------------------------------
                * RRQ/   | 01/02   |  Filename  |   0    |   Mode   |    0   |
                * WRQ     ----------------------------------------------------
                */
                result = FNET_OK;

                /* Set default error message. */
                error_code = FNET_TFTP_ERROR_NOT_DEFINED;
                error_message = 0;

                switch(tftp_srv_if->packet.packet_request.opcode)
                {
                    case FNET_HTONS(FNET_TFTP_OPCODE_READ_REQUEST):
                        FNET_DEBUG_TFTP_SRV("TFTP_SRV: Get Read request.");
                        tftp_srv_if->request_type = FNET_TFTP_REQUEST_READ;
                        break;
                    case FNET_HTONS(FNET_TFTP_OPCODE_WRITE_REQUEST):
                        FNET_DEBUG_TFTP_SRV("TFTP_SRV: Get Write request.");
                        tftp_srv_if->request_type = FNET_TFTP_REQUEST_WRITE;
                        break;
                    default:
                        FNET_DEBUG_TFTP_SRV("TFTP_SRV: Get wrong request (%d).", fnet_ntohs(tftp_srv_if->packet.packet_request.opcode));
                        result = FNET_ERR;
                        return;
                }

                if(result == FNET_OK)
                {
                    received -= 2;
                    /* Get filename. */
                    filename = tftp_srv_if->packet.packet_request.filename_mode;
                    for(i = 0u; i < (fnet_index_t)received; i++)
                    {
                        if(filename[i] == 0u)
                        {
                            break; /* Found end of file name. */
                        }
                    }

                    i++;
                    /* Get mode.*/
                    mode = &filename[i];

                    for(; i < (fnet_index_t)received; i++)
                    {
                        if(filename[i] == 0u)
                        {
                            break; /* Found end of mode. */
                        }
                    }

                    if( i < (fnet_index_t)received)
                    {
                        result = tftp_srv_if->request_handler(tftp_srv_if->request_type,
                                                              &addr,
                                                              filename,        /* null-terminated.*/
                                                              mode,            /* null-terminated.*/
                                                              &error_code,
                                                              &error_message,
                                                              tftp_srv_if->handler_param);
                    }
                    else
                    {
                        result = FNET_ERR;
                    }
                }

                if(result == FNET_OK)
                {
                    tftp_srv_if->complete_status = FNET_ERR; /* Set default value.*/

                    /* Create a socket for the new transaction. */
                    tftp_srv_if->socket_transaction = fnet_socket(addr.sa_family, SOCK_DGRAM, 0u);
                    if(tftp_srv_if->socket_transaction == FNET_NULL)
                    {
                        FNET_DEBUG_TFTP_SRV("TFTP_SRV: Socket creation error.");
                        _fnet_tftp_srv_send_error(tftp_srv_if, tftp_srv_if->socket_listen, FNET_TFTP_ERROR_NOT_DEFINED, 0, &addr);
                        tftp_srv_if->state = FNET_TFTP_SRV_STATE_CLOSE;   /* => CLOSE */
                    }
                    else
                    {
                        /* Save the client address.*/
                        tftp_srv_if->addr_transaction = addr;

                        /* Bind new socket. */
                        addr.sa_port = FNET_HTONS(0u);
                        fnet_memset_zero(addr.sa_data, sizeof(addr.sa_data));

                        if(fnet_socket_bind(tftp_srv_if->socket_transaction, &addr, sizeof(addr)) == FNET_ERR)
                        {
                            FNET_DEBUG_TFTP_SRV("TFTP_SRV: Socket bind error.");
                            _fnet_tftp_srv_send_error(tftp_srv_if, tftp_srv_if->socket_listen, FNET_TFTP_ERROR_NOT_DEFINED, 0, &addr);
                            tftp_srv_if->state = FNET_TFTP_SRV_STATE_CLOSE;   /* => CLOSE */
                        }
                        else
                        {
                            tftp_srv_if->block_number_ack = 0u;

                            /* REQUEST_WRITE. */
                            if(tftp_srv_if->request_type == FNET_TFTP_REQUEST_WRITE)
                            {
                                tftp_srv_if->request_send = _fnet_tftp_srv_send_ack;
                            }
                            /* REQUEST_READ. */
                            else
                            {
                                /* Data handler.*/
                                if((sent = _fnet_tftp_srv_data_handler(tftp_srv_if, (sizeof(tftp_srv_if->packet.packet_data.data)))) == FNET_ERR)
                                {
                                    break;
                                }
                                tftp_srv_if->tx_data_size = (fnet_size_t)sent;

                                tftp_srv_if->request_send = _fnet_tftp_srv_send_data;
                            }

                            /* Send. */
                            tftp_srv_if->request_send(tftp_srv_if);
                            tftp_srv_if->state = FNET_TFTP_SRV_STATE_HANDLE_REQUEST; /* => HANDLE_REQUEST */
                            tftp_srv_if->retransmit_cur = 0u;
                        }

                    }
                }
                else
                {
                    _fnet_tftp_srv_send_error(tftp_srv_if, tftp_srv_if->socket_listen, error_code, error_message, &addr);
                }
            }
            break;
        /*---- HANDLE_REQUEST -----------------------------------------------*/
        case  FNET_TFTP_SRV_STATE_HANDLE_REQUEST:
            addr_len = sizeof(addr);

            received = fnet_socket_recvfrom(tftp_srv_if->socket_transaction, &tftp_srv_if->packet.packet_data, sizeof(struct fnet_tftp_packet_data), 0u,
                                            &addr, &addr_len );

            if(received >= 4)
            {
                FNET_DEBUG_TFTP_SRV("TFTP_SRV:HANDLE_REQUEST");
                /* Error. */
                if ( (received == FNET_ERR) || (tftp_srv_if->packet.packet_data.opcode == FNET_HTONS(FNET_TFTP_OPCODE_ERROR)) )
                {
                    tftp_srv_if->state = FNET_TFTP_SRV_STATE_CLOSE;
                }
                /* Check TID. */
                else if ( (tftp_srv_if->addr_transaction.sa_port != addr.sa_port) ||
                          (!fnet_socket_addr_are_equal(&tftp_srv_if->addr_transaction, &addr)) )
                {
                    FNET_DEBUG_TFTP_SRV( "\nWARNING: Block not from our server!" );
                    _fnet_tftp_srv_send_error(tftp_srv_if, tftp_srv_if->socket_transaction, FNET_TFTP_ERROR_UNKNOWN_TID, 0, &addr);
                    tftp_srv_if->state = FNET_TFTP_SRV_STATE_CLOSE;
                }
                /* Received ACK. */
                else if ((tftp_srv_if->request_type == FNET_TFTP_REQUEST_READ) && (tftp_srv_if->packet.packet_data.opcode == FNET_HTONS(FNET_TFTP_OPCODE_ACK)))
                {
                    if(tftp_srv_if->block_number_ack == fnet_ntohs(tftp_srv_if->packet.packet_data.block_number)) /* Correct ACK. */
                    {
                        /* If last ACK. */
                        if(tftp_srv_if->tx_data_size < sizeof(tftp_srv_if->packet.packet_data.data))
                        {
                            tftp_srv_if->complete_status = FNET_OK;
                            tftp_srv_if->state = FNET_TFTP_SRV_STATE_CLOSE;
                            break;
                        }
                        /* More data to send. */
                        else
                        {
                            /* Data handler.*/
                            if((sent = _fnet_tftp_srv_data_handler(tftp_srv_if, (sizeof(tftp_srv_if->packet.packet_data.data)))) == FNET_ERR)
                            {
                                break;
                            }
                            tftp_srv_if->tx_data_size = (fnet_size_t)sent;

                            tftp_srv_if->retransmit_cur = 0u;
                        }
                    }
                    /* else: Resend last packet. */

                    /* Send. */
                    tftp_srv_if->request_send(tftp_srv_if);
                }
                /* Received Data. */
                else if ((tftp_srv_if->request_type == FNET_TFTP_REQUEST_WRITE) && (tftp_srv_if->packet.packet_data.opcode == FNET_HTONS(FNET_TFTP_OPCODE_DATA)) )
                {
                    if((tftp_srv_if->block_number_ack + 1u) == fnet_ntohs(tftp_srv_if->packet.packet_data.block_number))
                    {
                        /* Data handler.*/
                        if(_fnet_tftp_srv_data_handler(tftp_srv_if, ((fnet_size_t)received - 4u)) == FNET_ERR)
                        {
                            break;
                        }

                        /* Check return result.*/
                        if(received < (fnet_int32_t)sizeof(struct fnet_tftp_packet_data)) /* EOF */
                        {
                            tftp_srv_if->complete_status = FNET_OK;
                            tftp_srv_if->state = FNET_TFTP_SRV_STATE_CLOSE;   /* => CLOSE */
                        }

                        tftp_srv_if->retransmit_cur = 0u;
                    }
                    /* Send ACK. */
                    tftp_srv_if->request_send(tftp_srv_if);
                }
                else /* Wrong opration. */
                {
                    _fnet_tftp_srv_send_error(tftp_srv_if, tftp_srv_if->socket_transaction, FNET_TFTP_ERROR_ILLEGAL_OPERATION, 0, &addr);
                    tftp_srv_if->state = FNET_TFTP_SRV_STATE_CLOSE;
                }
            }
            /* Error. */
            if ( received == FNET_ERR)
            {
                tftp_srv_if->state = FNET_TFTP_SRV_STATE_CLOSE;
            }
            /* Check timeout */
            else if((fnet_timer_get_ms() - tftp_srv_if->last_time_ms) > (tftp_srv_if->timeout_ms))
            {
                /* Retransmit */
                if(tftp_srv_if->retransmit_cur < tftp_srv_if->retransmit_max)
                {
                    tftp_srv_if->retransmit_cur++;
                    tftp_srv_if->request_send(tftp_srv_if);
                }
                else
                {
                    tftp_srv_if->state = FNET_TFTP_SRV_STATE_CLOSE;
                }
            }
            else
            {}
            break;
        /*---- CLOSING --------------------------------------------*/
        case FNET_TFTP_SRV_STATE_CLOSE:
        default:
            FNET_DEBUG_TFTP_SRV("TFTP_SRV: STATE_CLOSING");

            if(tftp_srv_if->socket_transaction != FNET_NULL)
            {
                fnet_socket_close(tftp_srv_if->socket_transaction);
                tftp_srv_if->socket_transaction = FNET_NULL;
            }

            /* Call complete handler. */
            if(tftp_srv_if->complete_handler)
            {
                tftp_srv_if->complete_handler(tftp_srv_if->request_type, tftp_srv_if->complete_status, tftp_srv_if->handler_param);
            }

            tftp_srv_if->state = FNET_TFTP_SRV_STATE_WAITING_REQUEST; /*=> WAITING_REQUEST */
            break;
    }
}

/************************************************************************
* DESCRIPTION: TFTP server release.
************************************************************************/
void fnet_tftp_srv_release(fnet_tftp_srv_desc_t desc)
{
    struct fnet_tftp_srv_if *tftp_srv_if = (struct fnet_tftp_srv_if *)desc;

    fnet_service_mutex_lock();
    if(tftp_srv_if && (tftp_srv_if->state != FNET_TFTP_SRV_STATE_DISABLED))
    {
        if(tftp_srv_if->state == FNET_TFTP_SRV_STATE_HANDLE_REQUEST)
        {
            _fnet_tftp_srv_send_error(tftp_srv_if, tftp_srv_if->socket_transaction, FNET_TFTP_ERROR_NOT_DEFINED, 0, &tftp_srv_if->addr_transaction);
        }
        fnet_socket_close(tftp_srv_if->socket_listen);
        fnet_socket_close(tftp_srv_if->socket_transaction);

        fnet_service_unregister(tftp_srv_if->service_descriptor); /* Delete service.*/
        tftp_srv_if->state = FNET_TFTP_SRV_STATE_DISABLED;
    }
    fnet_service_mutex_unlock();
}

/************************************************************************
* DESCRIPTION: This function returns FNET_TRUE if the TFTP server
*              is enabled/initialised.
************************************************************************/
fnet_bool_t fnet_tftp_srv_is_enabled(fnet_tftp_srv_desc_t desc)
{
    struct fnet_tftp_srv_if *tftp_srv_if = (struct fnet_tftp_srv_if *) desc;
    fnet_bool_t             result;

    if(tftp_srv_if)
    {
        result = (tftp_srv_if->state != FNET_TFTP_SRV_STATE_DISABLED) ? FNET_TRUE : FNET_FALSE;
    }
    else
    {
        result = FNET_FALSE;
    }

    return result;
}

#endif
