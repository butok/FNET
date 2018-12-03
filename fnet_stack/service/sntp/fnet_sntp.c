/**************************************************************************
*
* Copyright 2017 by Andrey Butok. FNET Community.
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
* SNTP Client implementation.
*
***************************************************************************/
#include "fnet.h"

#if FNET_CFG_SNTP

#if FNET_CFG_DEBUG_SNTP && FNET_CFG_DEBUG
    #define FNET_DEBUG_SNTP   FNET_DEBUG
#else
    #define FNET_DEBUG_SNTP(...) do{}while(0)
#endif

/************************************************************************
*     Definitions
*************************************************************************/
#define FNET_SNTP_ERR_PARAMS            "ERROR: Wrong input parameters."
#define FNET_SNTP_ERR_SOCKET_CREATION   "ERROR: Socket creation error."
#define FNET_SNTP_ERR_SOCKET_CONNECT    "ERROR: Socket Error during connect."
#define FNET_SNTP_ERR_SERVICE           "ERROR: Service registration is failed."
#define FNET_SNTP_ERR_IS_INITIALIZED    "ERROR: SNTP is already initialized."

#define FNET_SNTP_SECONDS_IN_MINUTE                         60
#define FNET_SNTP_MINUTES_IN_HOUR                           60
#define FNET_SNTP_HOURS_IN_DAY                              24
#define FNET_SNTP_MONTHS_IN_YEAR                            12
#define FNET_SNTP_DAYS_IN_YEAR                              365
#define FNET_SNTP_DAYS_IN_LEAP_YEAR                         366
#define FNET_SNTP_DAYS_IN_4_YEARS                           (FNET_SNTP_DAYS_IN_YEAR*3 + FNET_SNTP_DAYS_IN_LEAP_YEAR)
#define FNET_SNTP_LEAP_YEAR                                 4
#define FNET_SNTP_FRACTIONS_IN_MILLISECOND                  (4294967296/FNET_TIMER_MS_IN_SEC) /*2^32=socond/1000*/



#define	FNET_SNTP_HEADER_MODE                               0x07	/* Mode mask */
#define	FNET_SNTP_HEADER_MODE_CLIENT                        0x03	/* Client */
#define	FNET_SNTP_HEADER_MODE_SERVER                        0x04	/* Server */
#define	FNET_SNTP_HEADER_VN                                 0x38    /* NTP version mask */
#define	FNET_SNTP_HEADER_VN_4                               0x20    /* NTP version 4 */
#define	FNET_SNTP_HEADER_LI                                 0xC0    /* Leap Indicator mask */
#define	FNET_SNTP_HEADER_LI_NO_WARNING                      0x00    /* No warning */
#define	FNET_SNTP_HEADER_LI_61_SECONDS                      0x40    /* Last minute has 61 seconds */
#define	FNET_SNTP_HEADER_LI_59_SECONDS                      0x80    /* Last minute has 59 seconds */
#define	FNET_SNTP_HEADER_LI_ALARM_CONDITION                 0xC0    /* Alarm condition (clock not synchronized) */
#define	FNET_SNTP_HEADER_STRATUM_KISS_DEATH                 0x00    /* Kiss-o'-death message */
#define	FNET_SNTP_HEADER_STRATUM_PRIMARY_REFERENCE          0x01    /* Primary reference (e.g., synchronized by radio clock) */
#define	FNET_SNTP_HEADER_STRATUM_SECONDARY_REFERENCE_MIN    0x02    /* Minimum secondary reference (synchronized by NTP or SNTP) */
#define	FNET_SNTP_HEADER_STRATUM_SECONDARY_REFERENCE_MAX    0x0F    /* Maximum secondary reference (synchronized by NTP or SNTP)*/

/******************************************************************************
 * SNTP-client states.
 ******************************************************************************/
typedef enum
{
    FNET_SNTP_STATE_DISABLED = 0,    /* The SNTP-client service is not initialized or is released.*/
    FNET_SNTP_STATE_TX,              /* The SNTP-client service sends the request to the SNTP server.*/
    FNET_SNTP_STATE_RX,              /* The SNTP-client service waits a response from the SNTP server.*/
    FNET_SNTP_STATE_RELEASE          /* The SNTP resolving is completed or timeout is occurred.*/
} fnet_sntp_state_t;

/************************************************************************
*    NTP Packet Header [RFC4330]
*************************************************************************
                           1                   2                   3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9  0  1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |LI | VN  |Mode |    Stratum    |     Poll      |   Precision    |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                          Root  Delay                           |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                       Root  Dispersion                         |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                     Reference Identifier                       |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                                |
      |                    Reference Timestamp (64)                    |
      |                                                                |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                                |
      |                    Originate Timestamp (64)                    |
      |                                                                |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                                |
      |                     Receive Timestamp (64)                     |
      |                                                                |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                                |
      |                     Transmit Timestamp (64)                    |
      |                                                                |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                 Key Identifier (optional) (32)                 |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                                |
      |                                                                |
      |                 Message Digest (optional) (128)                |
      |                                                                |
      |                                                                |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
FNET_COMP_PACKED_BEGIN
typedef struct
{
    fnet_uint8_t    li_vn_mode FNET_COMP_PACKED;                /*[0-1] Leap Indicator (LI); [2-4] Version Number (VN); [5-7] Mode. */
    fnet_uint8_t    stratum FNET_COMP_PACKED;                   /* Stratum.*/
    fnet_uint8_t    poll FNET_COMP_PACKED;                      /* Poll Interval.*/
    fnet_uint8_t    precision FNET_COMP_PACKED;                 /* Precision.*/
    fnet_uint32_t   root_delay FNET_COMP_PACKED;                /* Root Delay.*/
    fnet_uint32_t   root_dispersion FNET_COMP_PACKED;           /* Root Dispersion.*/
    fnet_uint32_t   reference_identifier FNET_COMP_PACKED;      /* Reference Identifier.*/
    fnet_uint32_t   reference_timestamp[2] FNET_COMP_PACKED;    /* Reference Timestamp.*/
    fnet_uint32_t   originate_timestamp[2] FNET_COMP_PACKED;    /* Originate Timestamp.*/
    fnet_uint32_t   receive_timestamp[2] FNET_COMP_PACKED;      /* Receive Timestamp.*/
    fnet_uint32_t   transmit_timestamp[2] FNET_COMP_PACKED;     /* Transmit Timestamp.*/
} fnet_sntp_header_t;
FNET_COMP_PACKED_END

/************************************************************************
*    SNTP-client interface structure.
*************************************************************************/
typedef struct
{
    fnet_socket_t                   socket_cln;
    fnet_service_desc_t             service_descriptor;
    fnet_sntp_state_t               state;                          /* Current state. */
    fnet_sntp_callback_resolved_t   callback;                       /* Callback function. */
    void                            *callback_cookie;               /* Callback-handler specific parameter. */
    fnet_time_t                     last_time_ms;                   /* Last receive time, used for timeout detection. */
    fnet_index_t                    iteration;                      /* Current iteration number.*/
    fnet_sntp_header_t              message;                        /* TX/RX message buffer */
    fnet_sntp_timestamp_t           timestamp;                      /* Timestamp */
}
fnet_sntp_if_t;

/* SNTP-client interface */
static fnet_sntp_if_t fnet_sntp_if;

static const fnet_uint8_t fnet_sntp_year_days[FNET_SNTP_MONTHS_IN_YEAR] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const fnet_uint8_t fnet_sntp_leap_year_days[FNET_SNTP_MONTHS_IN_YEAR] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static void _fnet_sntp_poll(void *fnet_sntp_if_p);

/************************************************************************
* DESCRIPTION: Initializes SNTP client service and starts the time
*              resolving.
************************************************************************/
fnet_return_t fnet_sntp_init( fnet_sntp_params_t *params )
{
    const fnet_uint32_t     bufsize_option = sizeof(fnet_sntp_header_t);
    struct fnet_sockaddr    remote_addr;

    fnet_service_mutex_lock();

    /* Check input parameters. */
    if((params == 0)
       || (params->sntp_server_addr.sa_family == AF_UNSPEC)
       || (fnet_socket_addr_is_unspecified(&params->sntp_server_addr))
       || (params->callback == 0))
    {
        FNET_DEBUG_SNTP(FNET_SNTP_ERR_PARAMS);
        goto ERROR;
    }

    /* Check if SNTP service is free.*/
    if(fnet_sntp_if.state != FNET_SNTP_STATE_DISABLED)
    {
        FNET_DEBUG_SNTP(FNET_SNTP_ERR_IS_INITIALIZED);
        goto ERROR;
    }

    /* Save input parmeters.*/
    fnet_memset_zero(&fnet_sntp_if, sizeof(fnet_sntp_if));
    fnet_sntp_if.callback = params->callback;
    fnet_sntp_if.callback_cookie = params->cookie;

    /* Create socket */
    if((fnet_sntp_if.socket_cln = fnet_socket(params->sntp_server_addr.sa_family, SOCK_DGRAM, 0u)) == FNET_NULL)
    {
        FNET_DEBUG_SNTP(FNET_SNTP_ERR_SOCKET_CREATION);
        goto ERROR;
    }

    /* Set socket options */
    fnet_socket_setopt(fnet_sntp_if.socket_cln, SOL_SOCKET, SO_RCVBUF, &bufsize_option, sizeof(bufsize_option));
    fnet_socket_setopt(fnet_sntp_if.socket_cln, SOL_SOCKET, SO_SNDBUF, &bufsize_option, sizeof(bufsize_option));

    /* Bind/connect to the server.*/
    FNET_DEBUG_SNTP("Connecting to SNTP Server.");
    fnet_memcpy(&remote_addr, &params->sntp_server_addr, sizeof(remote_addr));
    if(remote_addr.sa_port == 0U)
    {
        remote_addr.sa_port = FNET_CFG_SNTP_PORT;
    }

    if(fnet_socket_connect(fnet_sntp_if.socket_cln, &remote_addr, sizeof(remote_addr)) == FNET_ERR)
    {
        FNET_DEBUG_SNTP(FNET_SNTP_ERR_SOCKET_CONNECT);
        goto ERROR_1;
    }

    /* Register SNTP service. */
    fnet_sntp_if.service_descriptor = fnet_service_register(_fnet_sntp_poll, (void *) &fnet_sntp_if);
    if(fnet_sntp_if.service_descriptor == 0)
    {
        FNET_DEBUG_SNTP(FNET_SNTP_ERR_SERVICE);
        goto ERROR_1;
    }

    /* Build Unicast Request.*/
    fnet_memset_zero(&fnet_sntp_if.message, sizeof(fnet_sntp_if.message)); /* Clear message buffer.*/

    /* Set header fields  */
    fnet_sntp_if.message.li_vn_mode = FNET_SNTP_HEADER_LI_NO_WARNING | FNET_SNTP_HEADER_VN_4 | FNET_SNTP_HEADER_MODE_CLIENT;

    /* The rest of fields are not significant in SNTP client messages and may set to zeros*/

    fnet_sntp_if.state = FNET_SNTP_STATE_TX; /* => Send request. */

    fnet_service_mutex_unlock();

    return FNET_OK;
ERROR_1:
    fnet_socket_close(fnet_sntp_if.socket_cln);

ERROR:
    fnet_service_mutex_unlock();
    return FNET_ERR;
}

/************************************************************************
* DESCRIPTION: SNTP-client state machine.
************************************************************************/
static void _fnet_sntp_poll( void *fnet_sntp_if_p )
{
    fnet_ssize_t            sent_size;
    fnet_ssize_t            received;
    fnet_sntp_if_t           *sntp_if = (fnet_sntp_if_t *)fnet_sntp_if_p;

    switch(sntp_if->state)
    {
        /*---- TX --------------------------------------------*/
        case FNET_SNTP_STATE_TX:

            FNET_DEBUG_SNTP("Sending query...");
            sent_size = fnet_socket_send(sntp_if->socket_cln, &sntp_if->message, sizeof(sntp_if->message), 0U);

            if (sent_size != sizeof(sntp_if->message))
            {
                sntp_if->state = FNET_SNTP_STATE_RELEASE; /* ERROR */
            }
            else
            {
                sntp_if->last_time_ms = fnet_timer_get_ms();
                sntp_if->state = FNET_SNTP_STATE_RX;
            }
            break;
        /*---- RX -----------------------------------------------*/
        case  FNET_SNTP_STATE_RX:
            /* Receive data */

            received = fnet_socket_recv(sntp_if->socket_cln, &sntp_if->message, sizeof(sntp_if->message), 0U);

            /* Server reply check */
            if((received == sizeof(sntp_if->message))                                                           /* Check message size */
               && ((sntp_if->message.li_vn_mode & FNET_SNTP_HEADER_MODE) == FNET_SNTP_HEADER_MODE_SERVER)      /* Server reply */
               && ((sntp_if->message.li_vn_mode & FNET_SNTP_HEADER_LI) != FNET_SNTP_HEADER_LI_ALARM_CONDITION) /* Clock is synchronized */
               && (sntp_if->message.stratum >= FNET_SNTP_HEADER_STRATUM_PRIMARY_REFERENCE)                     /* Stratum min/max check */
               && (sntp_if->message.stratum <= FNET_SNTP_HEADER_STRATUM_SECONDARY_REFERENCE_MAX)
               && (sntp_if->message.transmit_timestamp[0] != 0) && (sntp_if->message.transmit_timestamp[1] != 0) /* Nonzero timestamp */
              )
            {
                /* Save timestamp */
                sntp_if->timestamp.seconds = fnet_ntohl(sntp_if->message.transmit_timestamp[0]);
                sntp_if->timestamp.seconds_fraction = fnet_ntohl(sntp_if->message.transmit_timestamp[1]);

                sntp_if->state = FNET_SNTP_STATE_RELEASE;
            }
            else /* No data. Check timeout */
            {
                if((fnet_timer_get_ms() - sntp_if->last_time_ms) > (FNET_CFG_SNTP_RETRANSMISSION_TIMEOUT * 1000U))
                {
                    sntp_if->iteration++;

                    if(sntp_if->iteration > FNET_CFG_SNTP_RETRANSMISSION_MAX)
                    {
                        sntp_if->state = FNET_SNTP_STATE_RELEASE; /* Timeout */
                    }
                    else
                    {
                        sntp_if->state = FNET_SNTP_STATE_TX;
                    }
                }
                else
                {}
            }
            break;
        /*---- RELEASE -------------------------------------------------*/
        case FNET_SNTP_STATE_RELEASE:
        {
            fnet_sntp_timestamp_t   *timestamp = FNET_NULL;

            fnet_sntp_release();

            /* Check resolved timestamp */
            if(sntp_if->timestamp.seconds || sntp_if->timestamp.seconds_fraction)
            {
                timestamp = &sntp_if->timestamp;
            }

            sntp_if->callback(timestamp, sntp_if->callback_cookie); /* User Callback.*/
        }
        break;
        default:
            break;
    }

}

/************************************************************************
* DESCRIPTION: This function aborts the time resolving and releases
* the SNTP-client service.
************************************************************************/
void fnet_sntp_release( void )
{
    if(fnet_sntp_if.state != FNET_SNTP_STATE_DISABLED)
    {
        fnet_service_mutex_lock();
        /* Unregister the tftp service. */
        fnet_service_unregister( fnet_sntp_if.service_descriptor );

        /* Close socket. */
        fnet_socket_close(fnet_sntp_if.socket_cln);

        fnet_sntp_if.state = FNET_SNTP_STATE_DISABLED;
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: Convert SNTP timestamp to Coordinated Universal Time (UTC).
************************************************************************/
void fnet_sntp_timestamp2utc(const fnet_sntp_timestamp_t *timestamp, fnet_sntp_utc_t *utc)
{
    if(timestamp && utc)
    {
        fnet_uint32_t       days;
        const fnet_uint8_t  *year_days;
        fnet_index_t        months;
        fnet_uint64_t        seconds;

        utc->millisecond = timestamp->seconds_fraction / FNET_SNTP_FRACTIONS_IN_MILLISECOND;

        /* RFC4330: a convenient way to extend the useful life of NTP timestamps is the following
        convention: If bit 0 is set, the UTC time is in the range 1968-
        2036, and UTC time is reckoned from 0h 0m 0s UTC on 1 January
        1900.  If bit 0 is not set, the time is in the range 2036-2104 and
        UTC time is reckoned from 6h 28m 16s UTC on 7 February 2036.*/
        seconds = (fnet_uint64_t)timestamp->seconds  + ((timestamp->seconds & 0x80000000U) ? 0x0U : 0xffffffffU);

        utc->second = seconds % FNET_SNTP_SECONDS_IN_MINUTE;

        utc->minute = (seconds / FNET_SNTP_SECONDS_IN_MINUTE) % FNET_SNTP_MINUTES_IN_HOUR;

        utc->hour = (seconds / (FNET_SNTP_SECONDS_IN_MINUTE * FNET_SNTP_MINUTES_IN_HOUR)) % FNET_SNTP_HOURS_IN_DAY;

        days = seconds / (FNET_SNTP_SECONDS_IN_MINUTE * FNET_SNTP_MINUTES_IN_HOUR * FNET_SNTP_HOURS_IN_DAY);

        utc->year = (days / FNET_SNTP_DAYS_IN_4_YEARS) * 4 + ((days % FNET_SNTP_DAYS_IN_4_YEARS) / FNET_SNTP_DAYS_IN_YEAR);

        days = days - (((utc->year / 4) * FNET_SNTP_DAYS_IN_4_YEARS) + ((utc->year % 4) * FNET_SNTP_DAYS_IN_YEAR)); /* Days in last year. */

        if((utc->year % 4) == 0) /* Leap year */
        {
            year_days = fnet_sntp_leap_year_days;
        }
        else
        {
            year_days = fnet_sntp_year_days;
        }

        for(months = 0; months < FNET_SNTP_MONTHS_IN_YEAR; months++)
        {
            if(days >= year_days[months])
            {
                days -= year_days[months];
            }
            else
            {
                break;
            }
        }

        utc->month = months + 1;
        utc->day = days + 1;
        utc->year += 1900; /* UTC time is reckoned from 0h 0m 0s UTC on 1 January 1900 */
    }
}

#endif /* FNET_CFG_SNTP*/
