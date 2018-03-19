/**************************************************************************
*
* Copyright 2016-2017 by Andrey Butok. FNET Community.
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
*  Network-interface initialization.
*
***************************************************************************/

#include "fapp.h"

#if FNET_CFG_LINK
    #include "fapp_link.h"
#endif

#include "fapp_shell.h"
#include "fapp_dhcp.h"
#include "fapp_autoip.h"
#include "fapp_llmnr.h"
#include "fapp_mdns.h"
#include "fapp_http.h"
#include "fapp_telnet.h"
#include "fapp_tftp.h"
#include "fapp_netif.h"
#include "fapp_bench.h"

/************************************************************************
*     Definitions.
*************************************************************************/
struct fapp_netif_init_param
{
    fnet_netif_desc_t   netif_desc;
    fnet_mac_addr_t     netif_mac_addr;
    fnet_ip4_addr_t     netif_ip4_addr;
    fnet_ip4_addr_t     netif_ip4_subnet_mask;
    fnet_ip4_addr_t     netif_ip4_gateway;
#if FNET_CFG_DNS
    fnet_ip4_addr_t     netif_ip4_dns;
#endif
};

/* Default address parameters.*/
struct fapp_netif_init_param fapp_netif_init_param_list[] =
{
#if FNET_CFG_CPU_ETH0
    {
        .netif_desc = FNET_CPU_ETH0_IF,
        .netif_mac_addr = FAPP_CFG_CPU_ETH0_MAC_ADDR,
        .netif_ip4_addr = FAPP_CFG_ETH0_IP4_ADDR,
        .netif_ip4_subnet_mask = FAPP_CFG_ETH0_IP4_MASK,
        .netif_ip4_gateway = FAPP_CFG_ETH0_IP4_GW,
#if FNET_CFG_DNS
        .netif_ip4_dns = FAPP_CFG_ETH0_IP4_DNS,
#endif
    },
#endif
#if FNET_CFG_CPU_ETH1
    {
        .netif_desc = FNET_CPU_ETH1_IF,
        .netif_mac_addr = FAPP_CFG_CPU_ETH1_MAC_ADDR,
        .netif_ip4_addr = FAPP_CFG_ETH1_IP4_ADDR,
        .netif_ip4_subnet_mask = FAPP_CFG_ETH1_IP4_MASK,
        .netif_ip4_gateway = FAPP_CFG_ETH1_IP4_GW,
#if FNET_CFG_DNS
        .netif_ip4_dns = FAPP_CFG_ETH1_IP4_DNS,
#endif
    },
#endif
#if FNET_CFG_CPU_WIFI
    {
        .netif_desc = FNET_CPU_WIFI_IF,
        .netif_ip4_addr = FAPP_CFG_WIFI_IP4_ADDR,
        .netif_ip4_subnet_mask = FAPP_CFG_WIFI_IP4_MASK,
        .netif_ip4_gateway = FAPP_CFG_WIFI_IP4_GW,
#if FNET_CFG_DNS
        .netif_ip4_dns = FAPP_CFG_WIFI_IP4_DNS,
#endif
    },
#endif
    /* Here put your new network interfaces.*/
    {.netif_desc = FNET_NULL} /* END */
};

/* String equivalent to fnet_netif_ip_addr_type_t */
static const fnet_char_t *const fapp_netif_ip_addr_type_str[] = {"manual",              /* FNET_NETIF_IP_ADDR_TYPE_MANUAL */
                                                                 "autoconfigurable",    /* FNET_NETIF_IP_ADDR_TYPE_AUTOCONFIGURABLE */
                                                                 "dhcp"                 /* FNET_NETIF_IP_ADDR_TYPE_DHCP */
                                                                };

/* Connection state string */
const fnet_char_t *const fapp_netif_connection_state_str[] = {"disconnected", /* false */
                                                              "connected"    /* true */
                                                             };

static const fnet_char_t *const fapp_netif_type_str[] = {   "",            /* FNET_NETIF_TYPE_OTHER */
                                                            "Ethernet",    /* FNET_NETIF_TYPE_ETHERNET */
                                                            "Wi-Fi",       /* FNET_NETIF_TYPE_WIFI */
                                                            "Loopback"     /* FNET_NETIF_TYPE_LOOPBACK */
                                                        };

static const fnet_char_t *const fapp_wifi_op_mode[] = { "",                 /* FNET_WIFI_OP_MODE_NONE */
                                                        "station",          /* FNET_WIFI_OP_MODE_STATION */
                                                        "access point"      /* FNET_WIFI_OP_MODE_ACCESS_POINT */
                                                      };

/************************************************************************
* DESCRIPTION: Initialization of all supported interfaces.
*************************************************************************/
fnet_return_t fapp_netif_init(fnet_shell_desc_t desc)
{
    fnet_return_t       result = FNET_OK;
    fnet_index_t        i;
    fnet_netif_desc_t   netif;

    /***********************************
     * Initialize all IFs.
     ************************************/
    for(i = 0; fapp_netif_init_param_list[i].netif_desc; i++)
    {
        netif = fapp_netif_init_param_list[i].netif_desc;
        result = fnet_netif_init(netif, fapp_netif_init_param_list[i].netif_mac_addr, sizeof(fnet_mac_addr_t));
        if(result == FNET_ERR)
        {
            break;
        }
        else
        {
            fnet_netif_set_ip4_addr(netif, fapp_netif_init_param_list[i].netif_ip4_addr, fapp_netif_init_param_list[i].netif_ip4_subnet_mask);
            fnet_netif_set_ip4_gateway(netif, fapp_netif_init_param_list[i].netif_ip4_gateway);
#if FNET_CFG_DNS
            fnet_netif_set_ip4_dns(netif, fapp_netif_init_param_list[i].netif_ip4_dns);
#endif

#if FNET_CFG_LINK
            /* Start Link-Detection. */
            fapp_link_init(desc, netif);
#endif
        }
    }

    /* Set default interface.*/
    {
        fnet_netif_desc_t netif_desc;

        netif_desc = fnet_netif_get_by_name(FAPP_CFG_PARAMS_NETIF_NAME);

        if(netif_desc)
        {
            fnet_netif_set_default(netif_desc);
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Print detailed address information about network interface.
************************************************************************/
void fapp_netif_info_print( fnet_shell_desc_t desc, fnet_netif_desc_t netif)
{
    fnet_char_t             name[FNET_NETIF_NAMELEN];
    fnet_bool_t             is_connected;
    fnet_netif_type_t       netif_type;

    netif_type = fnet_netif_get_type(netif);
    fnet_shell_println(desc, "%s Interface%s:", fapp_netif_type_str[netif_type], ((netif == fnet_netif_get_default()) ? " <default>" : ""));

    fnet_netif_get_name(netif, name, sizeof(name));
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Name", name);

    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "ScopeID", fnet_netif_get_scope_id(netif));

    is_connected = fnet_netif_is_connected(netif);
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Media State", fapp_netif_connection_state_str[is_connected]);

    /* Wi-Fi specific information */
    if(netif_type == FNET_NETIF_TYPE_WIFI)
    {
        fnet_wifi_op_mode_t     wifi_op_mode;
        fnet_uint32_t           wifi_version;

        /* Operation mode. */
        wifi_op_mode = fnet_wifi_get_op_mode(netif);
        if(wifi_op_mode)
        {
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Wi-Fi Mode", fapp_wifi_op_mode[wifi_op_mode]);
        }

        /* Firmware version number. */
        wifi_version = fnet_wifi_fw_get_version(netif);
        if(wifi_version)
        {
            fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_H, "Wi-Fi FW Version", wifi_version);
        }
    }

    /* MAC address */
    if((netif_type == FNET_NETIF_TYPE_WIFI) || (netif_type == FNET_NETIF_TYPE_ETHERNET))
    {
        fnet_char_t     mac_str[FNET_MAC_ADDR_STR_SIZE];
        fnet_mac_addr_t macaddr;

        fnet_netif_get_hw_addr(netif, macaddr, sizeof(fnet_mac_addr_t));
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "MAC Address", fnet_mac_to_str(macaddr, mac_str));
    }

    /* IP address information */
    fapp_netif_addr_print(desc, AF_SUPPORTED, netif, FNET_TRUE);

#if FNET_CFG_IP4
    {
        fnet_ip4_addr_t ip_addr;
        fnet_char_t     ip_str[FNET_IP4_ADDR_STR_SIZE];

        ip_addr = fnet_netif_get_ip4_subnet_mask(netif);
        fnet_inet_ntoa(*(struct fnet_in_addr *)( &ip_addr), ip_str);
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "IPv4 Subnet mask", ip_str);

        ip_addr = fnet_netif_get_ip4_gateway(netif);
        fnet_inet_ntoa(*(struct fnet_in_addr *)( &ip_addr), ip_str);
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "IPv4 Gateway", ip_str);

#if FNET_CFG_DNS
        ip_addr = fnet_netif_get_ip4_dns(netif);
        fnet_inet_ntoa(*(struct fnet_in_addr *)( &ip_addr), ip_str);
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "IPv4 DNS", ip_str);
#endif
    }
#endif /* FNET_CFG_IP4 */

#if FAPP_CFG_DHCP_CMD && FNET_CFG_DHCP_SRV && FNET_CFG_IP4
    fapp_dhcp_srv_info(desc, netif);
#endif

#if FAPP_CFG_DHCPC_CMD && FNET_CFG_DHCP_CLN && FNET_CFG_IP4
    fapp_dhcp_cln_info(desc, netif);
#endif

#if FAPP_CFG_AUTOIP_CMD && FNET_CFG_AUTOIP && FNET_CFG_IP4
    fapp_autoip_info(desc, netif);
#endif

#if FAPP_CFG_LLMNR_CMD && FNET_CFG_LLMNR
    fapp_llmnr_info(desc, netif);
#endif

#if FAPP_CFG_MDNS_CMD && FNET_CFG_MDNS
    fapp_mdns_info(desc, netif);
#endif
    fnet_shell_println(desc, "");
}

/************************************************************************
* DESCRIPTION: Print Interface IP addresses.
************************************************************************/
void fapp_netif_addr_print(fnet_shell_desc_t desc, fnet_address_family_t family, fnet_netif_desc_t netif, fnet_bool_t print_type)
{
    fnet_char_t    ip_str[FNET_IP_ADDR_STR_SIZE] = {0};

    if(netif == FNET_NULL)
    {
        netif = fnet_netif_get_default();
    }

#if FNET_CFG_IP4
    if((family & AF_INET) == AF_INET)
    {
        fnet_ip4_addr_t local_ip;

        local_ip = fnet_netif_get_ip4_addr(netif);
        fnet_shell_printf(desc, FAPP_SHELL_INFO_FORMAT_S, "IPv4 Address", fnet_inet_ntoa(*(struct fnet_in_addr *)(&local_ip), ip_str) );
        if(print_type)
        {
            fnet_shell_println(desc, " <%s>", fapp_netif_ip_addr_type_str[fnet_netif_get_ip4_addr_type(netif)]);
        }
        else
        {
            fnet_shell_println(desc, "");
        }
    }
#endif /* FNET_CFG_IP4 */
#if FNET_CFG_IP6
    if((family & AF_INET6) == AF_INET6)
    {
        fnet_bool_t                 result;
        fnet_index_t                n;
        fnet_netif_ip6_addr_info_t  addr_info;

        /* Print all assigned IPv6 addreses.*/
        n = 0u;
        for(;;)
        {
            result = fnet_netif_get_ip6_addr (netif, n, &addr_info);

            if(result == FNET_TRUE)
            {
                fnet_inet_ntop(AF_INET6, (fnet_uint8_t *)(&addr_info.address), ip_str, sizeof(ip_str));

                fnet_shell_printf(desc, FAPP_SHELL_INFO_FORMAT_S, "IPv6 Address", ip_str);
                if(print_type)
                {
                    fnet_shell_println(desc, " <%s>", fapp_netif_ip_addr_type_str[addr_info.type]);
                }
                else
                {
                    fnet_shell_println(desc, "");
                }
            }
            else
            {
                break;
            }

            n++;
        }
    }
#endif /* FNET_CFG_IP6 */
}

/************************************************************************
* DESCRIPTION: "info" command
************************************************************************/
#if FAPP_CFG_INFO_CMD
void fapp_netif_info_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_netif_desc_t           netif = FNET_NULL; /* By default is "default" netif*/
    fnet_index_t                i;

    /* [-n <if name>] */
    for(i = 1u; i < argc; i++)
    {
        if (!fnet_strcmp(argv[i], "-n")) /*[-n <if name>] */
        {
            i++;
            if(i < argc)
            {
                netif = fnet_netif_get_by_name(argv[i]);
            }
            else
            {
                goto ERROR_PARAMETER;
            }
        }
        else/* Wrong parameter.*/
        {
            goto ERROR_PARAMETER;
        }
    }

    if(netif)
    {
        fapp_netif_info_print(desc, netif);
    }
    else
    {
        /* Print info for all initialized interfaces.*/
        for(i = 0; (netif = fnet_netif_get_by_number(i)); i++)
        {
            fapp_netif_info_print(desc, netif);
        }
    }

#if ((FAPP_CFG_HTTP_CMD || FAPP_CFG_HTTP_TLS_CMD) && FNET_CFG_HTTP) || (FAPP_CFG_TELNET_CMD && FNET_CFG_TELNET) || (FAPP_CFG_TFTP_CMD && FNET_CFG_TFTP_SRV) || (FAPP_CFG_BENCH_CMD && FNET_CFG_BENCH_SRV)
    fnet_shell_println(desc, "Services:");
    /* General services.*/
#if (FAPP_CFG_HTTP_CMD || FAPP_CFG_HTTP_TLS_CMD) && FNET_CFG_HTTP
    fapp_http_info(desc);
#endif

#if FAPP_CFG_TELNET_CMD && FNET_CFG_TELNET
    fapp_telnet_info(desc);
#endif

#if FAPP_CFG_TFTP_CMD && FNET_CFG_TFTP_SRV
    fapp_tftp_srv_info(desc);
#endif

#if FAPP_CFG_BENCH_CMD && FNET_CFG_BENCH_SRV
    fapp_bench_srv_info(desc);
#endif
#endif

    return;

ERROR_PARAMETER:
    fnet_shell_println(desc, FAPP_PARAM_ERR, argv[i]);
    return;
}
#endif /* FAPP_CFG_INFO_CMD */

#if FAPP_CFG_STAT_CMD
/************************************************************************
* DESCRIPTION: Print detailed different statistic information about
*              network interface.
************************************************************************/
static void fapp_netif_stat_print( fnet_shell_desc_t desc, fnet_netif_desc_t netif)
{
    struct fnet_netif_statistics    statistics;
    fnet_char_t name[FNET_NETIF_NAMELEN];

    fnet_shell_println(desc, "%s Interface%s:", fapp_netif_type_str[fnet_netif_get_type(netif)],
                       ((netif == fnet_netif_get_default()) ? " <default>" : ""));

    fnet_netif_get_name(netif, name, sizeof(name));
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "Name", name);
    /* Print Packet statistics. */
    if(fnet_netif_get_statistics(netif, &statistics) == FNET_OK)
    {
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "TX Packets", statistics.tx_packet);
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "RX Packets", statistics.rx_packet);
    }
#if FNET_CFG_IP4
    {
        fnet_arp_entry_info_t   entry_info;
        fnet_char_t             addr_str[FNET_IP4_ADDR_STR_SIZE];
        fnet_char_t             mac_str[FNET_MAC_ADDR_STR_SIZE];
        fnet_index_t                        i;

        fnet_shell_println(desc, " ARP Cache:");
        for(i = 0U; fnet_arp_get_entry (netif, i, &entry_info ) == FNET_TRUE; i++)
        {
            fnet_shell_println(desc, "\t[%d] %s = %s", i,
                               fnet_inet_ntop(AF_INET, &entry_info.ip_addr, addr_str, sizeof(addr_str)),
                               fnet_mac_to_str(entry_info.mac_addr, mac_str));
        }
        if(i == 0U)
        {
            fnet_shell_println(desc, "\t<empty>");
        }
    }
#endif
#if FNET_CFG_IP6
    {
        fnet_index_t                        i;
        fnet_netif_ip6_prefix_t             ip6_prefix;
        fnet_netif_ip6_neighbor_cache_t     ip6_neighbor_cache;
        fnet_char_t                         numaddr[FNET_IP6_ADDR_STR_SIZE];
        fnet_char_t                         mac_str[FNET_MAC_ADDR_STR_SIZE];

        /* Print content of IPv6 Prefix List. */
        fnet_shell_println(desc, " IPv6 Prefix List:");
        for(i = 0U; fnet_netif_get_ip6_prefix(netif, i, &ip6_prefix) == FNET_TRUE; i++)
        {
            fnet_shell_println(desc, "\t[%d] %s/%d", i,
                               fnet_inet_ntop(AF_INET6, &ip6_prefix.prefix, numaddr, sizeof(numaddr)), ip6_prefix.prefix_length);
        }
        if(i == 0U)
        {
            fnet_shell_println(desc, "\t<empty>");
        }

        /* Print content of IPv6 Neighbor Cache. */
        fnet_shell_println(desc, " IPv6 Neighbor Cache:");
        for(i = 0U; fnet_netif_get_ip6_neighbor_cache(netif, i, &ip6_neighbor_cache) == FNET_TRUE; i++)
        {
            if(i == 0U)
            {
                fnet_shell_println(desc, "\nIPv6 Neighbor Cache:");
            }
            fnet_shell_println(desc, "\t[%d] %s = %s (%s)", i,
                               fnet_inet_ntop(AF_INET6, &ip6_neighbor_cache.ip_addr, numaddr, sizeof(numaddr)),
                               fnet_mac_to_str(ip6_neighbor_cache.ll_addr, mac_str),
                               (ip6_neighbor_cache.is_router == FNET_TRUE) ? "router" : "host");
        }
        if(i == 0U)
        {
            fnet_shell_println(desc, "\t<empty>");
        }
    }
#endif
    fnet_shell_println(desc, "");
}

/************************************************************************
* DESCRIPTION: "stat" command
************************************************************************/
void fapp_netif_stat_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_netif_desc_t           netif = FNET_NULL; /* By default is "default" netif*/
    fnet_index_t                i;

    /* [-n <if name>] */
    for(i = 1u; i < argc; i++)
    {
        if (!fnet_strcmp(argv[i], "-n")) /*[-n <if name>] */
        {
            i++;
            if(i < argc)
            {
                netif = fnet_netif_get_by_name(argv[i]);
            }
            else
            {
                goto ERROR_PARAMETER;
            }
        }
        else/* Wrong parameter.*/
        {
            goto ERROR_PARAMETER;
        }
    }

    if(netif)
    {
        fapp_netif_stat_print(desc, netif);
    }
    else
    {
        /* Print info for all initialized interfaces.*/
        for(i = 0; (netif = fnet_netif_get_by_number(i)); i++)
        {
            fapp_netif_stat_print(desc, netif);
        }
    }

    return;

ERROR_PARAMETER:
    fnet_shell_println(desc, FAPP_PARAM_ERR, argv[i]);
    return;
}
#endif /* FAPP_CFG_STAT_CMD */


