/**************************************************************************
*
* Copyright 2017-2018 by Andrey Butok. FNET Community.
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
* FNET Shell Demo implementation (SNTP client).
*
***************************************************************************/

#include "fapp.h"
#include "fapp_prv.h"
#include "fapp_sntp.h"
#include "fapp_time.h"
#include "fnet.h"

#if FAPP_CFG_SNTP_CMD && FNET_CFG_SNTP
/************************************************************************
*     Definitions.
*************************************************************************/
#define FNET_SNTP_RESOLUTION_FAILED  "Time resolution is FAILED"
#define FNET_SNTP_UNKNOWN            "SNTP server is unknown"

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void fapp_sntp_callback_resolved (const fnet_sntp_timestamp_t *timestamp, void *cookie);
static void fapp_sntp_on_ctrlc(fnet_shell_desc_t desc, void *cookie);
static void fapp_sntp_init(const struct fnet_sockaddr *server_addr, fnet_shell_desc_t shell_desc);

static fnet_size_t _fapp_sntp_addr_list_size = 0;
const fnet_dns_resolved_addr_t *_fapp_sntp_addr_list = 0;

/************************************************************************
* DESCRIPTION: Event handler callback on resolved time over SNTP.
************************************************************************/
static void fapp_sntp_callback_resolved (const fnet_sntp_timestamp_t *timestamp, void *cookie)
{
    fnet_shell_desc_t   desc = (fnet_shell_desc_t) cookie;
    fnet_sntp_utc_t     utc;


    fnet_shell_unblock(desc); /* Unblock the shell. */

    if(timestamp)
    {
        fnet_sntp_timestamp2utc(timestamp, &utc);
        fnet_shell_println(desc, " UTC: %u-%u-%u %u:%u:%u.%u", utc.year, utc.month, utc.day, utc.hour, utc.minute, utc.second, utc.millisecond);

#if FNET_CFG_TIME
        {
            /*  Update system time */
            time_t sec;
            struct tm date_time;

            /* date and time default value. */
            date_time.tm_sec = utc.second;          /* seconds after the minute	0-61 */
            date_time.tm_min = utc.minute;          /* minutes after the hour	0-59 */
            date_time.tm_hour = utc.hour;           /* hours since midnight	0-23 */
            date_time.tm_mday = utc.day;            /* day of the month	1-31 */
            date_time.tm_mon = utc.month - 1;       /* months since January	0-11 */
            date_time.tm_year = utc.year - 1900;    /* years since 1900 */
            date_time.tm_wday = -1;                 /* days since Sunday	0-6 */
            date_time.tm_yday = -1;                 /* days since January 1	0-365 */
            date_time.tm_isdst = -1;                /* Daylight Saving Time flag */

            sec = mktime(&date_time);

            fnet_time_set(sec);
        }
#endif
    }
    else
    {

        if(_fapp_sntp_addr_list_size)
        {
            fapp_sntp_init(&_fapp_sntp_addr_list->resolved_addr, desc);
            _fapp_sntp_addr_list_size--;
            _fapp_sntp_addr_list++;
        }
        else
        {
            fnet_shell_println(desc, FNET_SNTP_RESOLUTION_FAILED);
        }
    }
}

/************************************************************************
* DESCRIPTION: Ctr+C termination handler.
************************************************************************/
static void fapp_sntp_on_ctrlc(fnet_shell_desc_t desc, void *cookie)
{
    /* Terminate SNTP service. */
    fnet_sntp_release();
    fnet_shell_println( desc, FAPP_CANCELLED_STR);
}

/************************************************************************
* DESCRIPTION: Event handler callback on resolved IP address.
************************************************************************/
#define FNET_DNS_RESOLUTION_FAILED  "Resolution is FAILED"
#define FNET_DNS_UNKNOWN            "DNS server is unknown"

static void fapp_sntp_dns_callback_resolved (const fnet_dns_resolved_addr_t *addr_list, fnet_size_t addr_list_size, void *cookie)
{
    fnet_char_t         ip_str[FNET_IP_ADDR_STR_SIZE_MAX];
    fnet_shell_desc_t   desc = (fnet_shell_desc_t) cookie;
    fnet_index_t        i;

    fnet_shell_unblock((fnet_shell_desc_t)cookie); /* Unblock the shell. */

    if(addr_list && addr_list_size)
    {

        _fapp_sntp_addr_list_size = addr_list_size;
        _fapp_sntp_addr_list = addr_list;

        for(i = 0u; i < addr_list_size; i++)
        {
            fnet_shell_printf(desc, FAPP_SHELL_INFO_FORMAT_S, "Resolved address",
                              fnet_inet_ntop(addr_list->resolved_addr.sa_family, addr_list->resolved_addr.sa_data, ip_str, sizeof(ip_str)));
            fnet_shell_println(desc, "\t TTL=%u", addr_list->resolved_addr_ttl);

            addr_list++;
        }

        fapp_sntp_init(&_fapp_sntp_addr_list->resolved_addr, desc);
        _fapp_sntp_addr_list_size--;
        _fapp_sntp_addr_list++;
    }
    else
    {
        fnet_shell_println(desc, FNET_DNS_RESOLUTION_FAILED);
    }
}

static fnet_dns_desc_t fapp_sntp_dns_desc; /* DNS service descriptor. */
/************************************************************************
* DESCRIPTION: Ctr+C termination handler.
************************************************************************/
static void fapp_sntp_dns_on_ctrlc(fnet_shell_desc_t desc, void *cookie)
{
    /* Terminate DNS service. */
    fnet_dns_release(fapp_sntp_dns_desc);
    fnet_shell_println( desc, FAPP_CANCELLED_STR);
}

/************************************************************************
* DESCRIPTION: Start SNTP client.
************************************************************************/
void fapp_sntp_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    struct fnet_sockaddr        sntp_server_addr;
    fnet_index_t                error_param;
    const fnet_char_t           *host_name = FNET_NULL;


    FNET_COMP_UNUSED_ARG(argc);

    /* Set SNTP client parameters.*/
    fnet_memset_zero(&sntp_server_addr, sizeof(sntp_server_addr));

    /* Set SNTP server address.*/
    if(argc == 1) /* no server name - use default NTP server name */
    {
        host_name = FAPP_CFG_SNTP_CMD_SERVER_NAME; /* "pool.ntp.org", "89.221.210.188", "37.187.104.44", "77.78.107.252" */
    }
    else
    {
        host_name = argv[1];
    }

    _fapp_sntp_addr_list_size = 0;
    _fapp_sntp_addr_list = 0;

    if(fnet_inet_ptos(host_name, &sntp_server_addr) == FNET_ERR) /* Try if host_name is IP address */
    {
        /* Resolve server name */
        struct fnet_dns_params      dns_params;
        fnet_char_t                 ip_str[FNET_IP_ADDR_STR_SIZE_MAX];
        fnet_dns_desc_t             dns_desc;

        /* Set DNS client/resolver parameters.*/
        fnet_memset_zero(&dns_params, sizeof(struct fnet_dns_params));

        dns_params.addr_family = AF_INET;
        dns_params.host_name = host_name;                        /* Host name to resolve - last parameter.*/
        dns_params.callback = fapp_sntp_dns_callback_resolved;   /* Callback function.*/
        dns_params.cookie = desc;                               /* Application-specific parameter which will be passed to fapp_dns_callback_resolved().*/

        /* Run DNS client/resolver. */
        dns_desc = fnet_dns_init(&dns_params);
        if(dns_desc)
        {
            fnet_shell_println(desc, FAPP_DELIMITER_STR);
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Resolving", dns_params.host_name);
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "DNS Server",
                               fnet_inet_ntop(dns_params.dns_server_addr.sa_family, dns_params.dns_server_addr.sa_data, ip_str, sizeof(ip_str)));
            fnet_shell_println(desc, FAPP_TOCANCEL_STR);
            fnet_shell_println(desc, FAPP_DELIMITER_STR);

            fapp_sntp_dns_desc = dns_desc;

            fnet_shell_block(desc, fapp_sntp_dns_on_ctrlc, FNET_NULL); /* Block the shell input.*/
        }
        else
        {
            fnet_shell_println(desc, FAPP_INIT_ERR, "DNS");

            error_param = 1u;
            goto ERROR_PARAMETER;
        }
    }
    else
    {
        fapp_sntp_init(&sntp_server_addr, desc);
    }

    return;

ERROR_PARAMETER:
    fnet_shell_println(desc, FAPP_PARAM_ERR, argv[error_param]);
    return;
}

/************************************************************************
* DESCRIPTION: Start SNTP client.
************************************************************************/
static void fapp_sntp_init(const struct fnet_sockaddr *server_addr, fnet_shell_desc_t shell_desc)
{
    fnet_sntp_params_t   sntp_params;
    fnet_char_t          ip_str[FNET_IP_ADDR_STR_SIZE];

    /* Set SNTP client parameters.*/
    fnet_memset_zero(&sntp_params, sizeof(fnet_sntp_params_t));

    fnet_memcpy(&sntp_params.sntp_server_addr, server_addr, sizeof(sntp_params.sntp_server_addr));
    sntp_params.callback = fapp_sntp_callback_resolved; /* Callback function.*/
    sntp_params.cookie = shell_desc;                          /* Application-specific parameter
                                                        which will be passed to fapp_sntp_callback_resolved().*/

    /* Run SNTP client. */
    if(fnet_sntp_init(&sntp_params) != FNET_ERR)
    {
        fnet_shell_println(shell_desc, FAPP_DELIMITER_STR);
        fnet_shell_println(shell_desc, " SNTP Requesting");
        fnet_shell_println(shell_desc, FAPP_SHELL_INFO_FORMAT_S, "SNTP Server",
                           fnet_inet_ntop(sntp_params.sntp_server_addr.sa_family, sntp_params.sntp_server_addr.sa_data, ip_str, sizeof(ip_str)));
        fnet_shell_println(shell_desc, FAPP_TOCANCEL_STR);
        fnet_shell_println(shell_desc, FAPP_DELIMITER_STR);

        fnet_shell_block(shell_desc, fapp_sntp_on_ctrlc, FNET_NULL); /* Block the shell input.*/
    }
    else
    {
        fnet_shell_println(shell_desc, FAPP_INIT_ERR, "SNTP");
    }

}


#endif /* FAPP_CFG_SNTP_CMD && FNET_CFG_SNTP */
