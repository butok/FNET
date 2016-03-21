/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
* Copyright 2008-2010 by Andrey Butok. Freescale Semiconductor, Inc.
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
* @file fapp_setget.c
*
* @author Andrey Butok
*
* @brief FNET Application Shell Set/Get routines.
*
***************************************************************************/

#include "fapp.h"
#include "fapp_prv.h"
#include "fapp_tftp.h"
#include "fapp_config.h"
#include "fapp_setget.h"
#include "fapp_params_prv.h"
#include "fnet.h"

#if FAPP_CFG_SETGET_CMD

/************************************************************************
*     Definitions.
*************************************************************************/
#define FAPP_SET_OPT_FORMAT (" %-8s: %s")
#define FAPP_GET_OPT_FORMAT (" %-8s: ")
#define FAPP_GET_SOPT_FORMAT ("%s")
#define FAPP_GET_DOPT_FORMAT ("%d")
#define FAPP_GET_XOPT_FORMAT ("0x%x")

/************************************************************************
*     Function Prototypes
*************************************************************************/
#if FAPP_CFG_SETGET_CMD_NETIF
    static void fapp_set_cmd_netif(fnet_shell_desc_t desc, fnet_char_t *value );
    static void fapp_get_cmd_netif(fnet_shell_desc_t desc);
#endif
#if FAPP_CFG_SETGET_CMD_IP && FNET_CFG_IP4
    static void fapp_set_cmd_ip(fnet_shell_desc_t desc, fnet_char_t *value );
    static void fapp_get_cmd_ip(fnet_shell_desc_t desc);
#endif
#if FAPP_CFG_SETGET_CMD_GATEWAY && FNET_CFG_IP4
    static void fapp_set_cmd_gateway(fnet_shell_desc_t desc, fnet_char_t *value );
    static void fapp_get_cmd_gateway(fnet_shell_desc_t desc);
#endif
#if FAPP_CFG_SETGET_CMD_NETMASK && FNET_CFG_IP4
    static void fapp_set_cmd_netmask(fnet_shell_desc_t desc, fnet_char_t *value );
    static void fapp_get_cmd_netmask(fnet_shell_desc_t desc);
#endif
#if FAPP_CFG_SETGET_CMD_MAC
    static void fapp_set_cmd_mac(fnet_shell_desc_t desc, fnet_char_t *value );
    static void fapp_get_cmd_mac(fnet_shell_desc_t desc);
#endif
#if FAPP_CFG_SETGET_CMD_HOSTNAME
    static void fapp_set_cmd_hostname(fnet_shell_desc_t desc, fnet_char_t *value );
    static void fapp_get_cmd_hostname(fnet_shell_desc_t desc);
#endif
#if FAPP_CFG_SETGET_CMD_DNS && FNET_CFG_DNS && FNET_CFG_IP4
    static void fapp_set_cmd_dns(fnet_shell_desc_t desc, fnet_char_t *value );
    static void fapp_get_cmd_dns(fnet_shell_desc_t desc);
#endif
#if FAPP_CFG_SETGET_CMD_BOOT
    static void fapp_set_cmd_boot(fnet_shell_desc_t desc, fnet_char_t *value );
    static void fapp_get_cmd_boot(fnet_shell_desc_t desc);
#endif
#if FAPP_CFG_SETGET_CMD_SCRIPT
    static void fapp_set_cmd_bootscript(fnet_shell_desc_t desc, fnet_char_t *value );
    static void fapp_get_cmd_bootscript(fnet_shell_desc_t desc);
#endif
#if FAPP_CFG_SETGET_CMD_DELAY
    static void fapp_set_cmd_bootdelay(fnet_shell_desc_t desc, fnet_char_t *value );
    static void fapp_get_cmd_bootdelay(fnet_shell_desc_t desc);
#endif
#if FAPP_CFG_SETGET_CMD_TFTP
    static void fapp_set_cmd_tftp(fnet_shell_desc_t desc, fnet_char_t *value );
    static void fapp_get_cmd_tftp(fnet_shell_desc_t desc);
#endif
#if FAPP_CFG_SETGET_CMD_IMAGE
    static void fapp_set_cmd_image(fnet_shell_desc_t desc, fnet_char_t *value );
    static void fapp_get_cmd_image(fnet_shell_desc_t desc);
#endif
#if FAPP_CFG_SETGET_CMD_TYPE
    static void fapp_set_cmd_image_type(fnet_shell_desc_t desc, fnet_char_t *value );
    static void fapp_get_cmd_image_type(fnet_shell_desc_t desc);
#endif
#if FAPP_CFG_SETGET_CMD_GO
    static void fapp_set_cmd_go(fnet_shell_desc_t desc, fnet_char_t *value );
    static void fapp_get_cmd_go(fnet_shell_desc_t desc);
#endif
#if FAPP_CFG_SETGET_CMD_RAW
    static void fapp_set_cmd_raw(fnet_shell_desc_t desc, fnet_char_t *value );
    static void fapp_get_cmd_raw(fnet_shell_desc_t desc);
#endif

#if FNET_CFG_IP4
    static void fapp_set_ip(fnet_shell_desc_t desc, fnet_char_t *value, void (*set_ip)( fnet_netif_desc_t netif_desc, fnet_ip4_addr_t ipaddr ));
    static void fapp_get_ip(fnet_shell_desc_t desc, fnet_ip4_addr_t (*get_address)( fnet_netif_desc_t netif_desc ));
#endif

/************************************************************************
*     The set/show parameterr's entry control data structure definition.
*************************************************************************/
typedef struct
{
    fnet_char_t *option;
    void (*set)(fnet_shell_desc_t desc, fnet_char_t *value);
    void (*get)(fnet_shell_desc_t desc);
    fnet_char_t *syntax;
}
fapp_setget_cmd_t;

/************************************************************************
*    The table of the set/show parameterr's.
*************************************************************************/
static const fapp_setget_cmd_t fapp_setget_cmd_table [] =
{
#if FAPP_CFG_SETGET_CMD_NETIF
    { "netif", fapp_set_cmd_netif, fapp_get_cmd_netif, "<default-if name>" },
#endif
#if FAPP_CFG_SETGET_CMD_IP && FNET_CFG_IP4
    { "ip", fapp_set_cmd_ip, fapp_get_cmd_ip, "<default-if IPv4 address>" },
#endif
#if FAPP_CFG_SETGET_CMD_NETMASK && FNET_CFG_IP4
    { "netmask", fapp_set_cmd_netmask, fapp_get_cmd_netmask, "<default-if IPv4 netmask address>" },
#endif
#if FAPP_CFG_SETGET_CMD_GATEWAY && FNET_CFG_IP4
    { "gateway", fapp_set_cmd_gateway, fapp_get_cmd_gateway, "<default-if IPv4 gateway address>" },
#endif
#if FAPP_CFG_SETGET_CMD_MAC
    { "mac", fapp_set_cmd_mac, fapp_get_cmd_mac, "<default-if Ethernet address>" },
#endif
#if FAPP_CFG_SETGET_CMD_HOSTNAME
    { "host", fapp_set_cmd_hostname, fapp_get_cmd_hostname, "<host name>" },
#endif

    /* DNS set/get parameters. */
#if FAPP_CFG_SETGET_CMD_DNS && FNET_CFG_DNS && FNET_CFG_IP4
    { "dns", fapp_set_cmd_dns, fapp_get_cmd_dns, "<DNS server IP address>" },
#endif

    /* Bootloader set/get parameters. */
#if FAPP_CFG_SETGET_CMD_BOOT
    { "boot", fapp_set_cmd_boot, fapp_get_cmd_boot, "<stop|go|script>" },
#endif
#if FAPP_CFG_SETGET_CMD_DELAY
    { "delay", fapp_set_cmd_bootdelay, fapp_get_cmd_bootdelay, "<seconds>" },
#endif
#if FAPP_CFG_SETGET_CMD_SCRIPT
    { "script", fapp_set_cmd_bootscript, fapp_get_cmd_bootscript, "<command script>" },
#endif
#if FAPP_CFG_SETGET_CMD_RAW
    { "raw", fapp_set_cmd_raw, fapp_get_cmd_raw, "0x<address>" },
#endif

    /* TFTP set/get parameters */
#if FAPP_CFG_SETGET_CMD_TFTP
    { "tftp", fapp_set_cmd_tftp, fapp_get_cmd_tftp, "<TFTP server IP address>" },
#endif
#if FAPP_CFG_SETGET_CMD_IMAGE
    { "image", fapp_set_cmd_image, fapp_get_cmd_image, "<Image-file name to load with TFTP>" },
#endif
#if FAPP_CFG_SETGET_CMD_TYPE
    { "type", fapp_set_cmd_image_type, fapp_get_cmd_image_type, "<srec|hex|bin|raw>" },
#endif
#if FAPP_CFG_SETGET_CMD_GO
    { "go", fapp_set_cmd_go, fapp_get_cmd_go, "0x<entry point address>" },
#endif
};

#define FAPP_SET_CMD_NUM  ((fnet_index_t)(sizeof(fapp_setget_cmd_table)/sizeof(fapp_setget_cmd_t)))

/************************************************************************
* NAME: fapp_set_cmd_hostname
*
* DESCRIPTION: Sets default interface, using its name as parameter.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_NETIF
static void fapp_set_cmd_netif(fnet_shell_desc_t desc, fnet_char_t *value )
{
    fnet_netif_desc_t netif_desc;

    netif_desc = fnet_netif_get_by_name(value);

    if(netif_desc)
    {
        fnet_netif_set_default(netif_desc);
    }
    else
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, value);
    }
}
#endif

/************************************************************************
* NAME: fapp_get_cmd_netif
*
* DESCRIPTION: Gets name of the default interface.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_NETIF
static void fapp_get_cmd_netif(fnet_shell_desc_t desc)
{
    fnet_char_t name[FNET_NETIF_NAMELEN];

    fnet_netif_get_name(fnet_netif_get_default(), name, sizeof(name));
    fnet_shell_println(desc, FAPP_GET_SOPT_FORMAT, name);
}
#endif

/************************************************************************
* NAME: fapp_set_ip
*
* DESCRIPTION: Set IP address function.
************************************************************************/
#if FNET_CFG_IP4
static void fapp_set_ip(fnet_shell_desc_t desc, fnet_char_t *value, void (*set_ip)( fnet_netif_desc_t netif_desc, fnet_ip4_addr_t ipaddr ) )
{
    fnet_ip4_addr_t addr;

    if(fnet_inet_aton(value, (struct in_addr *) &addr) == FNET_OK)
    {
        set_ip(fnet_netif_get_default(), addr);
    }
    else
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, value);
    }
}
#endif

/************************************************************************
* NAME: fapp_get_ip
*
* DESCRIPTION: Print IP address.
************************************************************************/
#if FNET_CFG_IP4
static void fapp_get_ip(fnet_shell_desc_t desc, fnet_ip4_addr_t (*get_address)( fnet_netif_desc_t netif_desc ))
{
    fnet_char_t            ip_str[FNET_IP4_ADDR_STR_SIZE];
    struct in_addr  addr;

    addr.s_addr = get_address(fnet_netif_get_default());
    fnet_inet_ntoa(addr, ip_str);
    fnet_shell_println(desc, FAPP_GET_SOPT_FORMAT, ip_str);
}
#endif

/************************************************************************
* NAME: fapp_set_cmd_ip
*
* DESCRIPTION: Set IP address for default network interface.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_IP && FNET_CFG_IP4
static void fapp_set_cmd_ip(fnet_shell_desc_t desc, fnet_char_t *value )
{
    fapp_set_ip(desc, value, fnet_netif_set_ip4_addr );
}
#endif
/************************************************************************
* NAME: fapp_get_cmd_ip
*
* DESCRIPTION: Gets IP address for default network interface.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_IP && FNET_CFG_IP4
static void fapp_get_cmd_ip(fnet_shell_desc_t desc)
{
    fapp_get_ip(desc, fnet_netif_get_ip4_addr);
}
#endif
/************************************************************************
* NAME: fapp_set_cmd_gateway
*
* DESCRIPTION: Sets gateway for default network interface.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_GATEWAY && FNET_CFG_IP4
static void fapp_set_cmd_gateway(fnet_shell_desc_t desc, fnet_char_t *value )
{
    fapp_set_ip(desc, value, fnet_netif_set_ip4_gateway );
}
#endif
/************************************************************************
* NAME: fapp_set_cmd_gateway
*
* DESCRIPTION: Gets gateway for default network interface.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_GATEWAY && FNET_CFG_IP4
static void fapp_get_cmd_gateway(fnet_shell_desc_t desc)
{
    fapp_get_ip(desc, fnet_netif_get_ip4_gateway);
}
#endif
/************************************************************************
* NAME: fapp_set_cmd_netmask
*
* DESCRIPTION: Sets netmask for default network interface.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_NETMASK && FNET_CFG_IP4
static void fapp_set_cmd_netmask(fnet_shell_desc_t desc, fnet_char_t *value )
{
    fapp_set_ip(desc, value, fnet_netif_set_ip4_subnet_mask);
}
#endif
/************************************************************************
* NAME: fapp_get_cmd_netmask
*
* DESCRIPTION: Gets netmask for default network interface.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_NETMASK && FNET_CFG_IP4
static void fapp_get_cmd_netmask(fnet_shell_desc_t desc)
{
    fapp_get_ip(desc, fnet_netif_get_ip4_subnet_mask);
}
#endif

/************************************************************************
* NAME: fapp_set_cmd_mac
*
* DESCRIPTION: Sets MAC address for default network interface.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_MAC
static void fapp_set_cmd_mac(fnet_shell_desc_t desc, fnet_char_t *value)
{
    fnet_mac_addr_t macaddr;

    if((fnet_str_to_mac(value, macaddr) != FNET_OK) ||
       (fnet_netif_set_hw_addr(fnet_netif_get_default(), macaddr, sizeof(fnet_mac_addr_t)) != FNET_OK))
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, value);
    }
}
#endif

/************************************************************************
* NAME: fapp_set_cmd_hostname
*
* DESCRIPTION: Sets host name, used by LLMNR.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_HOSTNAME
static void fapp_set_cmd_hostname(fnet_shell_desc_t desc, fnet_char_t *value )
{
    (void)desc;
    fnet_strncpy(fapp_params_host_name, value, FAPP_PARAMS_HOST_NAME_SIZE);
}
#endif

/************************************************************************
* NAME: fapp_get_cmd_hostname
*
* DESCRIPTION: Gets host name, used by LLMNR.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_HOSTNAME
static void fapp_get_cmd_hostname(fnet_shell_desc_t desc)
{
    fnet_shell_println(desc, FAPP_GET_SOPT_FORMAT, fapp_params_host_name);
}
#endif

/************************************************************************
* NAME: fapp_set_cmd_dns
*
* DESCRIPTION: Gets DNS-server IP address.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_DNS && FNET_CFG_DNS && FNET_CFG_IP4
static void fapp_get_cmd_dns(fnet_shell_desc_t desc)
{
    fapp_get_ip(desc, fnet_netif_get_ip4_dns);
}
#endif

/************************************************************************
* NAME: fapp_set_cmd_dns
*
* DESCRIPTION: Sets DNS-server IP address.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_DNS && FNET_CFG_DNS && FNET_CFG_IP4
static void fapp_set_cmd_dns(fnet_shell_desc_t desc, fnet_char_t *value)
{
    fapp_set_ip(desc, value, fnet_netif_set_ip4_dns);
}
#endif

/************************************************************************
* NAME: fapp_set_cmd_mac
*
* DESCRIPTION: Gets MAC address for default network interface.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_MAC
static void fapp_get_cmd_mac(fnet_shell_desc_t desc)
{
    fnet_char_t     mac_str[FNET_MAC_ADDR_STR_SIZE];
    fnet_mac_addr_t macaddr;

    fnet_netif_get_hw_addr(fnet_netif_get_default(), macaddr, sizeof(fnet_mac_addr_t));
    fnet_shell_println(desc, FAPP_GET_SOPT_FORMAT, fnet_mac_to_str(macaddr, mac_str));
}
#endif

/************************************************************************
* NAME: fapp_set_cmd_bootscript
*
* DESCRIPTION: Sets boot-command script .
************************************************************************/
#if FAPP_CFG_SETGET_CMD_SCRIPT
static void fapp_set_cmd_bootscript(fnet_shell_desc_t desc, fnet_char_t *value )
{
    (void)desc;
    fnet_strncpy( fapp_params_boot_config.script, value, FAPP_PARAMS_BOOT_SCRIPT_SIZE );
}
#endif
/************************************************************************
* NAME: fapp_get_cmd_bootscript
*
* DESCRIPTION: Gets boot-command script .
************************************************************************/
#if FAPP_CFG_SETGET_CMD_SCRIPT
static void fapp_get_cmd_bootscript(fnet_shell_desc_t desc)
{
    fnet_shell_println(desc, FAPP_GET_SOPT_FORMAT, fapp_params_boot_config.script);
}
#endif
/************************************************************************
* NAME: fapp_set_cmd_boot
*
* DESCRIPTION: Sets boot mode.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_BOOT
static void fapp_set_cmd_boot(fnet_shell_desc_t desc, fnet_char_t *value )
{
    const struct boot_mode *mode = fapp_boot_mode_by_name (value);

    if(mode == 0)
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, value);
    }
    else
    {
        fapp_params_boot_config.mode = mode->index;
    }

}
#endif
/************************************************************************
* NAME: fapp_get_cmd_boot
*
* DESCRIPTION: Gets boot mode.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_BOOT
static void fapp_get_cmd_boot(fnet_shell_desc_t desc)
{
    const struct boot_mode *mode = fapp_boot_mode_by_index(fapp_params_boot_config.mode);
    fnet_shell_println(desc, FAPP_GET_SOPT_FORMAT, mode->name);
}
#endif
/************************************************************************
* NAME: fapp_set_cmd_bootdelay
*
* DESCRIPTION: Sets boot delay.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_DELAY
static void fapp_set_cmd_bootdelay(fnet_shell_desc_t desc, fnet_char_t *value )
{
    fnet_time_t     delay;
    fnet_char_t     *p = 0;

    delay = fnet_strtoul(value, &p, 0u);
    if ((delay == 0u) && (p == value))
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, value); /* Print error mesage. */
    }
    else
    {
        fapp_params_boot_config.delay = delay;
    }
}
#endif
/************************************************************************
* NAME: fapp_get_cmd_bootdelay
*
* DESCRIPTION: Gets boot delay.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_DELAY
static void fapp_get_cmd_bootdelay(fnet_shell_desc_t desc)
{
    fnet_shell_println(desc, FAPP_GET_DOPT_FORMAT, fapp_params_boot_config.delay);
}
#endif

/************************************************************************
* NAME: fapp_set_cmd_tftp
*
* DESCRIPTION: Set IP address for default network interface.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_TFTP
static void fapp_set_cmd_tftp(fnet_shell_desc_t desc, fnet_char_t *value )
{
    struct sockaddr s_addr;

    if(fnet_inet_ptos(value, &s_addr) == FNET_ERR)
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, value); /* Print error. */
    }

    fnet_memcpy(&fapp_params_tftp_config.server_addr, &s_addr, sizeof(fapp_params_tftp_config.server_addr));
}
#endif
/************************************************************************
* NAME: fapp_get_cmd_tftp
*
* DESCRIPTION:
************************************************************************/
#if FAPP_CFG_SETGET_CMD_TFTP
static void fapp_get_cmd_tftp(fnet_shell_desc_t desc)
{
    fnet_char_t ip_str[FNET_IP_ADDR_STR_SIZE];

    fnet_shell_println(desc, FAPP_GET_SOPT_FORMAT, fnet_inet_ntop(fapp_params_tftp_config.server_addr.sa_family, fapp_params_tftp_config.server_addr.sa_data, ip_str, sizeof(ip_str)) );
}
#endif
/************************************************************************
* NAME: fapp_set_cmd_image
*
* DESCRIPTION:
************************************************************************/
#if FAPP_CFG_SETGET_CMD_IMAGE
static void fapp_set_cmd_image(fnet_shell_desc_t desc, fnet_char_t *value )
{
    (void)desc;
    fnet_strncpy( fapp_params_tftp_config.file_name, value, FAPP_PARAMS_TFTP_FILE_NAME_SIZE );
}
#endif
/************************************************************************
* NAME: fapp_get_cmd_image
*
* DESCRIPTION:
************************************************************************/
#if FAPP_CFG_SETGET_CMD_IMAGE
static void fapp_get_cmd_image(fnet_shell_desc_t desc)
{
    fnet_shell_println(desc, FAPP_GET_SOPT_FORMAT, fapp_params_tftp_config.file_name);
}
#endif
/************************************************************************
* NAME: fapp_set_cmd_image_type
*
* DESCRIPTION:
************************************************************************/
#if FAPP_CFG_SETGET_CMD_TYPE
static void fapp_set_cmd_image_type(fnet_shell_desc_t desc, fnet_char_t *value )
{
    const struct image_type *type = fapp_tftp_image_type_by_name (value);

    if(type == 0)
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, value);
    }
    else
    {
        fapp_params_tftp_config.file_type = type->index;
    }
}
#endif
/************************************************************************
* NAME: fapp_get_cmd_image_type
*
* DESCRIPTION:
************************************************************************/
#if FAPP_CFG_SETGET_CMD_TYPE
static void fapp_get_cmd_image_type(fnet_shell_desc_t desc)
{
    const struct image_type *type = fapp_tftp_image_type_by_index(fapp_params_tftp_config.file_type);
    fnet_shell_println(desc, FAPP_GET_SOPT_FORMAT, type->name);
}
#endif
/************************************************************************
* NAME: fapp_set_cmd_go
*
* DESCRIPTION: Sets 'go' entry point.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_GO
static void fapp_set_cmd_go(fnet_shell_desc_t desc, fnet_char_t *value )
{
    fnet_uint32_t   address;
    fnet_char_t     *p = 0;

    address = fnet_strtoul(value, &p, 16u);
    if ((address == 0u) && (p == value))
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, value); /* Print error mesage. */
    }
    else
    {
        fapp_params_boot_config.go_address = address;
    }
}
#endif
/************************************************************************
* NAME: fapp_get_cmd_go
*
* DESCRIPTION: Gets 'go' entry point.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_GO
static void fapp_get_cmd_go(fnet_shell_desc_t desc)
{
    fnet_shell_println(desc, FAPP_GET_XOPT_FORMAT, fapp_params_boot_config.go_address);
}
#endif
/************************************************************************
* NAME: fapp_set_cmd_raw
*
* DESCRIPTION: Sets the default download address for raw binary files.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_RAW
static void fapp_set_cmd_raw(fnet_shell_desc_t desc, fnet_char_t *value )
{
    fnet_uint32_t   address;
    fnet_char_t     *p = 0;

    address = fnet_strtoul(value, &p, 16u);
    if ((address == 0u) && (p == value))
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, value); /* Print error mesage. */
    }
    else
    {
        fapp_params_tftp_config.file_raw_address = address;
    }
}
#endif
/************************************************************************
* NAME: fapp_get_cmd_raw
*
* DESCRIPTION: Gets the default download address for raw binary files.
************************************************************************/
#if FAPP_CFG_SETGET_CMD_RAW
static void fapp_get_cmd_raw(fnet_shell_desc_t desc)
{
    fnet_shell_println(desc, FAPP_GET_XOPT_FORMAT, fapp_params_tftp_config.file_raw_address);
}
#endif
/************************************************************************
* NAME: fapp_set_cmd
*
* DESCRIPTION: Sets system options.
************************************************************************/
void fapp_set_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_index_t index;

    if(argc == 1u) /* Print all set options with syntax description. */
    {
        fnet_shell_println(desc, "Valid 'set' options:");

        for (index = 0U; index < FAPP_SET_CMD_NUM; ++index)
        {
            fnet_shell_println(desc, FAPP_SET_OPT_FORMAT, fapp_setget_cmd_table[index].option, fapp_setget_cmd_table[index].syntax);
        }
    }
    else if(argc != 3u)
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, "");
    }
    else /* Set parameter */
    {
        for (index = 0U; index < FAPP_SET_CMD_NUM; index++)
        {
            if(fnet_strcasecmp(fapp_setget_cmd_table[index].option, argv[1]) == 0)
            {
                fapp_setget_cmd_table[index].set(desc, argv[2]);
                /* Print the result value. */
                fnet_shell_printf(desc, FAPP_GET_OPT_FORMAT, fapp_setget_cmd_table[index].option);
                fapp_setget_cmd_table[index].get(desc);
                goto EXIT;
            }
        }

        fnet_shell_println(desc, FAPP_PARAM_ERR, argv[1]);
    }
EXIT:
    return;
}

/************************************************************************
* NAME: fapp_show
*
* DESCRIPTION: Shows all system settings.
************************************************************************/
void fapp_get_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_index_t index;

    if(argc == 1u) /* Print all prameters. */
    {
        for (index = 0U; index < FAPP_SET_CMD_NUM; ++index)
        {
            fnet_shell_printf(desc, FAPP_GET_OPT_FORMAT, fapp_setget_cmd_table[index].option);
            fapp_setget_cmd_table[index].get(desc);
        }
    }
    else /* Print one parameter. */
    {

        for (index = 0U; index < FAPP_SET_CMD_NUM; index++)
        {
            if(fnet_strcasecmp(fapp_setget_cmd_table[index].option, argv[1]) == 0)
            {
                fnet_shell_printf(desc, FAPP_GET_OPT_FORMAT, fapp_setget_cmd_table[index].option);
                fapp_setget_cmd_table[index].get(desc);
                return;
            }
        }

        fnet_shell_println(desc, FAPP_PARAM_ERR, argv[1]);
    }

}

#endif



