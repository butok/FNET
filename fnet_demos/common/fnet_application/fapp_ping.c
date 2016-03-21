/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
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
*
* @file fapp_ping.c
*
* @author Andrey Butok
*
* @brief FNET Shell Demo implementation (Ping).
*
***************************************************************************/

#include "fapp.h"
#include "fapp_prv.h"


#if FNET_CFG_PING && (FAPP_CFG_PING_CMD || FAPP_CFG_PING6_CMD)

#include "fapp_ping.h"

/************************************************************************
*     Definitions.
*************************************************************************/
#define FAPP_PING_BUFFER_SIZE       (100)
#define FAPP_PING_IDENTIFIER        (1)

#define FAPP_PING_STR_REPLY         "Reply from %s"
#define FAPP_PING_STR_TIMEOUT       "Request timed out."


/************************************************************************
*     Function Prototypes
*************************************************************************/
static void fapp_ping_handler(fnet_error_t result, fnet_size_t packet_count, struct sockaddr *target_addr, fnet_uint32_t cookie);
static void fapp_ping_on_ctrlc(fnet_shell_desc_t desc);

/************************************************************************
* NAME: fapp_ping_handler
*
* DESCRIPTION:
************************************************************************/
static void fapp_ping_handler (fnet_error_t result, fnet_size_t packet_count, struct sockaddr *target_addr, fnet_uint32_t cookie)
{
    fnet_char_t                ip_str[FNET_IP_ADDR_STR_SIZE];
    fnet_shell_desc_t   desc = (fnet_shell_desc_t)cookie;

    if(result == FNET_ERR_OK)
    {
        fnet_shell_println(desc, FAPP_PING_STR_REPLY, fnet_inet_ntop(target_addr->sa_family, target_addr->sa_data, ip_str, sizeof(ip_str)));
    }
    else if(result == FNET_ERR_TIMEDOUT)
    {
        fnet_shell_println(desc, FAPP_PING_STR_TIMEOUT);
    }
    else
    {
        fnet_shell_println(desc, "Error = %d", result);
    }

    if(packet_count == 0U)
    {
        fnet_shell_unblock(desc); /* Unblock the shell. */
    }
}

/************************************************************************
* NAME: fapp_ping_on_ctrlc
*
* DESCRIPTION:
************************************************************************/
static void fapp_ping_on_ctrlc(fnet_shell_desc_t desc)
{
    /* Release PING. */
    fnet_ping_release();
    fnet_shell_println( desc, FAPP_CANCELLED_STR);
}

/************************************************************************
* NAME: fapp_ping_cmd
*
* DESCRIPTION: Ping command.
************************************************************************/
void fapp_ping_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    struct fnet_ping_params ping_params;
    fnet_index_t            i;
    fnet_char_t             *p;
    fnet_uint32_t           value;
    fnet_char_t             ip_str[FNET_IP_ADDR_STR_SIZE];


    fnet_memset_zero(&ping_params, sizeof(ping_params));

    ping_params.cookie = (fnet_uint32_t)desc;
    ping_params.handler = fapp_ping_handler;
    ping_params.packet_size = FAPP_PING_DEFAULT_SIZE;
    ping_params.timeout = FAPP_PING_DEFAULT_TIMEOUT;
    ping_params.pattern = FAPP_PING_DEFAULT_PATTERN;
    ping_params.ttl = FAPP_PING_DEFAULT_HOP_LIMIT;
    ping_params.packet_count = FAPP_PING_DEFAULT_NUMBER;

    /* Last parameter must be ip address.*/
    i = (argc - 1u);
    if(fnet_inet_ptos(argv[i], &ping_params.target_addr) == FNET_ERR)
    {
        goto ERROR_PARAMETER;
    }
    else
    {
        /* TBD Optimise parameters parsing.*/
        if(argc > 2u) /* There are additional parameters */
        {
            /* [-c <count>][-i <seconds>]\n\t[-p <pattern>][-s <size>][-h <hoplimit/ttl>] */
            for(i = 1u; i < (fnet_index_t)(argc - 1u); i++)
            {
                if (!fnet_strcmp(argv[i], "-c"))
                {
                    i++;
                    value = fnet_strtoul(argv[i], &p, 10u);
                    if ((value == 0U) && (p == argv[i]))
                    {
                        goto ERROR_PARAMETER;
                    }
                    else
                    {
                        ping_params.packet_count = value;
                    }
                }
                else if (!fnet_strcmp(argv[i], "-i"))
                {
                    i++;
                    value = fnet_strtoul(argv[i], &p, 10u);
                    if ((value == 0U) && (p == argv[i]))
                    {
                        goto ERROR_PARAMETER;
                    }
                    else
                    {
                        ping_params.timeout = value * 1000U;
                    }
                }
                else if (!fnet_strcmp(argv[i], "-p"))
                {
                    i++;
                    value = fnet_strtoul(argv[i], &p, 10u);
                    if ((value == 0U) && (p == argv[i]))
                    {
                        goto ERROR_PARAMETER;
                    }
                    else
                    {
                        ping_params.pattern = (fnet_uint8_t)value;
                    }
                }
                else if (!fnet_strcmp(argv[i], "-s"))
                {
                    i++;
                    value = fnet_strtoul(argv[i], &p, 10u);
                    if ((value == 0U) && (p == argv[i]))
                    {
                        goto ERROR_PARAMETER;
                    }
                    else
                    {
                        ping_params.packet_size = (fnet_size_t)value;
                    }
                }
                else if (!fnet_strcmp(argv[i], "-h"))
                {
                    i++;
                    value = fnet_strtoul(argv[i], &p, 10u);
                    if ((value == 0U) && (p == argv[i]))
                    {
                        goto ERROR_PARAMETER;
                    }
                    else
                    {
                        ping_params.ttl = (fnet_uint8_t)value;
                    }
                }
                else if (!fnet_strcmp(argv[i], "-n"))
                {
                    /* Just ignore  the -n parameter.*/
                }
                else if (!fnet_strcmp(argv[i], "-I"))
                {
                    i++;
                    /* Just ignore  the -I parameter and its value.*/
                }
                else /* Wrong parameter.*/
                {
                    goto ERROR_PARAMETER;
                }
            }
        }

        if(fnet_ping_request(&ping_params) == FNET_OK)
        {
            fnet_shell_println(desc, FAPP_DELIMITER_STR);
            fnet_shell_println(desc, " PING" );
            fnet_shell_println(desc, FAPP_DELIMITER_STR);
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Remote IP addr", fnet_inet_ntop(ping_params.target_addr.sa_family, ping_params.target_addr.sa_data, ip_str, sizeof(ip_str)));
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Message Size", ping_params.packet_size > FNET_CFG_PING_PACKET_MAX ? FNET_CFG_PING_PACKET_MAX : ping_params.packet_size);
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Num. of messages", ping_params.packet_count);
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Pattern", ping_params.pattern);
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Hoplimit (TTL)", ping_params.ttl);
            fnet_shell_println(desc, FAPP_TOCANCEL_STR);
            fnet_shell_println(desc, FAPP_DELIMITER_STR);

            fnet_shell_block(desc, fapp_ping_on_ctrlc); /* Block shell. */
        }
        else
        {
            fnet_shell_println(desc, FAPP_INIT_ERR, "PING");
        }
    }

    return;

ERROR_PARAMETER:
    fnet_shell_println(desc, FAPP_PARAM_ERR, argv[i]);
    return;
}

#endif /* FAPP_CFG_PING_CMD || FAPP_CFG_PING6_CMD */






