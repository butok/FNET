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
**********************************************************************/
/*!
* @brief FNET Shell Demo implementation (SNTP client).
*
***************************************************************************/

#include "fapp.h"
#include "fapp_prv.h"
#include "fapp_sntp.h"
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
static void fapp_sntp_on_ctrlc(fnet_shell_desc_t desc);

/************************************************************************
* DESCRIPTION: Event handler callback on resolved time over SNTP.
************************************************************************/
static void fapp_sntp_callback_resolved (const fnet_sntp_timestamp_t *timestamp, void *cookie)
{
    fnet_shell_desc_t   desc = (fnet_shell_desc_t) cookie;
    fnet_sntp_utc_t     utc;

    fnet_shell_unblock((fnet_shell_desc_t)cookie); /* Unblock the shell. */

    if(timestamp)
    {
        fnet_sntp_timestamp2utc(timestamp, &utc);
        fnet_shell_println(desc, " UTC: %u-%u-%u %u:%u:%u.%u", utc.year, utc.month, utc.day, utc.hour, utc.minute, utc.second, utc.millisecond);
    }
    else
    {
        fnet_shell_println(desc, FNET_SNTP_RESOLUTION_FAILED);
    }
}

/************************************************************************
* DESCRIPTION: Ctr+C termination handler.
************************************************************************/
static void fapp_sntp_on_ctrlc(fnet_shell_desc_t desc)
{
    /* Terminate SNTP service. */
    fnet_sntp_release();
    fnet_shell_println( desc, FAPP_CANCELLED_STR);
}

/************************************************************************
* DESCRIPTION: Start SNTP client.
************************************************************************/
void fapp_sntp_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    struct fnet_sntp_params     sntp_params;
    fnet_char_t                 ip_str[FNET_IP_ADDR_STR_SIZE];
    fnet_index_t                error_param;

    FNET_COMP_UNUSED_ARG(argc);

    /* Set SNTP client parameters.*/
    fnet_memset_zero(&sntp_params, sizeof(struct fnet_sntp_params));

    /* Set SNTP server address.*/
    if(fnet_inet_ptos(argv[1], &sntp_params.sntp_server_addr) == FNET_ERR)
    {
        error_param = 1u;
        goto ERROR_PARAMETER;
    }

    sntp_params.callback = fapp_sntp_callback_resolved; /* Callback function.*/
    sntp_params.cookie = desc;                          /* Application-specific parameter
                                                        which will be passed to fapp_sntp_callback_resolved().*/

    /* Run SNTP client. */
    if(fnet_sntp_init(&sntp_params) != FNET_ERR)
    {
        fnet_shell_println(desc, FAPP_DELIMITER_STR);
        fnet_shell_println(desc, " SNTP Resolving");
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "SNTP Server",
                           fnet_inet_ntop(sntp_params.sntp_server_addr.sa_family, sntp_params.sntp_server_addr.sa_data, ip_str, sizeof(ip_str)));
        fnet_shell_println(desc, FAPP_TOCANCEL_STR);
        fnet_shell_println(desc, FAPP_DELIMITER_STR);

        fnet_shell_block(desc, fapp_sntp_on_ctrlc); /* Block the shell input.*/
    }
    else
    {
        fnet_shell_println(desc, FAPP_INIT_ERR, "SNTP");
    }
    return;

ERROR_PARAMETER:
    fnet_shell_println(desc, FAPP_PARAM_ERR, argv[error_param]);
    return;
}

#endif /* FAPP_CFG_SNTP_CMD && FNET_CFG_SNTP */
