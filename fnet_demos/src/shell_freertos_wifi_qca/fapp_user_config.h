/***************************************************************************
*
* FNET Application User configuration file.
* It should be used to change any default configuration parameter of FAPP.
*
***************************************************************************/

#ifndef _FAPP_USER_CONFIG_H_

#define _FAPP_USER_CONFIG_H_

#define FAPP_CFG_NAME                   "FNET Shell Application (FreeRTOS-WIFI)"
#define FAPP_CFG_SHELL_PROMPT           "SHELL> "

#define FAPP_CFG_FREERTOS               (1) /* Enable FreeRTOS */

/*!!! Make sure that the network interface  MAC address has unique value (FAPP_CFG_CPU_ETH0_MAC_ADDR). !!!*/

#if 0 /* IPv4 address parameters are set automatically by DHCP or AutoIP */
    /*****************************************************************************
    * IP address for the Ethernet interface.
    * At runtime it can be changed by the fnet_netif_set_ip4_addr(),
    * by the DHCP client service, or by the Auto-IP service.
    ******************************************************************************/
    #define FAPP_CFG_ETH0_IP4_ADDR      (FNET_IP4_ADDR_INIT(192U, 168U, 0U, 22U))

    /*****************************************************************************
    * IP Subnet mask for the Ethernet interface.
    * At runtime it can be changed by the fnet_netif_set_ip4_addr(),
    * by the DHCP client service, or by the Auto-IP service.
    ******************************************************************************/
    #define FAPP_CFG_ETH0_IP4_MASK      (FNET_IP4_ADDR_INIT(255U, 255U, 255U, 0U))

    /*****************************************************************************
    * Gateway IP address for the Ethernet interface.
    * At runtime it can be changed by the fnet_netif_set_ip4_gateway(),
    * by the DHCP client service, or by the Auto-IP service.
    ******************************************************************************/
    #define FAPP_CFG_ETH0_IP4_GW        (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))

    /*****************************************************************************
    * DNS server IP address for the Ethernet interface.
    * At runtime it can be changed by the fnet_netif_set_ip4_dns() or
    * by the DHCP client service.
    * It is used only if FNET_CFG_DNS is set to 1.
    ******************************************************************************/
    #define FAPP_CFG_ETH0_IP4_DNS       (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))
#endif

/*  "dhcpc" command.*/
#define FAPP_CFG_DHCPC_CMD              (1)
#define FAPP_CFG_DHCPC_CMD_DISCOVER_MAX (3)

/*  "dhcp" command.*/
#define FAPP_CFG_DHCP_CMD               (1)

/*  "autoip" command.*/
#define FAPP_CFG_AUTOIP_CMD             (1)

/*  "set/get" command.*/
#define FAPP_CFG_SETGET_CMD_IP          (1)
#define FAPP_CFG_SETGET_CMD_GATEWAY     (1)
#define FAPP_CFG_SETGET_CMD_NETMASK     (1)
#define FAPP_CFG_SETGET_CMD_MAC         (1)
#define FAPP_CFG_SETGET_CMD_HOSTNAME    (1)
#define FAPP_CFG_SETGET_CMD_SSID        (1)
#define FAPP_CFG_SETGET_CMD_PASSPHRASE  (1)
#define FAPP_CFG_SETGET_CMD_NETIF       (1)

/*  "info" command. */
#define FAPP_CFG_INFO_CMD               (1)

/*  "stat" command.*/
#define FAPP_CFG_STAT_CMD               (1)

/*  "http" command.*/
#define FAPP_CFG_HTTP_CMD               (1)

/*  "exp" command.*/
#define FAPP_CFG_EXP_CMD                (1)

/*  "save" command.*/
#define FAPP_CFG_SAVE_CMD               (1)

/*  "reset" command.*/
#define FAPP_CFG_RESET_CMD              (1)

/*  "telnet" command.*/
#define FAPP_CFG_TELNET_CMD             (1)

/*  "dns" command.*/
#define FAPP_CFG_DNS_CMD                (1)

/*  "ping" command.*/
#define FAPP_CFG_PING_CMD               (1)

/*  "bind" command.*/
#define FAPP_CFG_BIND_CMD               (1)

/*  "unbind" command.*/
#define FAPP_CFG_UNBIND_CMD             (1)

/*  "llmnr" command.*/
#define FAPP_CFG_LLMNR_CMD              (1)

/*  "mdns" command.*/
#define FAPP_CFG_MDNS_CMD               (1)

/*  "sntp" command.*/
#define FAPP_CFG_SNTP_CMD                (1)

/* Reading of the configuration parameters from the Flash
 * memory during the application bootup.*/
#define FAPP_CFG_PARAMS_READ_FLASH       (1)

/* Rewriting of the configuration parameters in the Flash
 * memory duiring flashing of the application. */
#define FAPP_CFG_PARAMS_REWRITE_FLASH    (1)

#if 1 /* On startup script.*/
    #define FAPP_CFG_STARTUP_SCRIPT          "http;"     /* For example "http; telnet" */
#endif

/* On connect/disconnect scripts:*/
#define FAPP_CFG_LINK_CONNECT_SCRIPT     "dhcpc autoip -n %s; mdns -n %s; llmnr -n %s;" /* "%s" will be replaced by the interface name.*/
#define FAPP_CFG_LINK_DISCONNECT_SCRIPT   "mdns -n %s release; llmnr -n %s release; dhcpc -n %s release; autoip -n %s release;" /* "%s" will be replaced by the interface name.*/

/* Wi-Fi parameters */ 
/* Default network interface set to Wi-Fi */
#define FAPP_CFG_PARAMS_NETIF_NAME              FNET_CFG_CPU_WIFI_NAME
/* Default parameters for "connect" command */
#define FAPP_CFG_PARAMS_WIFI_SSID               "ssid"
#define FAPP_CFG_PARAMS_WIFI_WPA_PASSPHRASE     "password"

/* Wi-Fi interface connect/disconnect scripts, in the access point operation mode.*/
#define FAPP_CFG_LINK_CONNECT_WIFI_ACCESS_POINT_SCRIPT      "dhcp -n %s; mdns -n %s; llmnr -n %s;" /* "%s" will be replaced by the interface name.*/
#define FAPP_CFG_LINK_DISCONNECT_WIFI_ACCESS_POINT_SCRIPT   "mdns -n %s release; llmnr -n; dhcp -n %s release;" /* "%s" will be replaced by the interface name.*/


#endif /* _FAPP_USER_CONFIG_H_ */

