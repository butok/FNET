/**********************************************************************/ /*!
*
* @file fapp_user_config.h
*
* @brief FNET Application User configuration file.
* It should be used to change any default configuration parameter of FAPP.
*
***************************************************************************/

#ifndef _FAPP_USER_CONFIG_H_

#define _FAPP_USER_CONFIG_H_

#define FAPP_CFG_NAME                   "FNET Throughput Benchmark"
#define FAPP_CFG_SHELL_PROMPT           "BENCH> "

/*!!! Make sure that the network interface MAC address has unique value (FAPP_CFG_CPU_ETH0_MAC_ADDR). !!!*/

#if 1 /* IPv4 address parameters are set manually */
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

/*  "dhcp" command.*/
#define FAPP_CFG_DHCP_CMD               (1)
#define FAPP_CFG_DHCP_CMD_DISCOVER_MAX  (5)

/*  "set/get" command.*/
#define FAPP_CFG_SETGET_CMD_IP          (1)
#define FAPP_CFG_SETGET_CMD_GATEWAY     (1)
#define FAPP_CFG_SETGET_CMD_NETMASK     (1)
#define FAPP_CFG_SETGET_CMD_MAC         (1)

/*  "info" command. */
#define FAPP_CFG_INFO_CMD               (1)

/*  "stat" command.*/
#define FAPP_CFG_STAT_CMD               (1)

/*  "http" command.*/
#define FAPP_CFG_HTTP_CMD               (0)

/*  "exp" command.*/
#define FAPP_CFG_EXP_CMD                (0)

/*  "save" command.*/
#define FAPP_CFG_SAVE_CMD               (1)

/*  "reset" command.*/
#define FAPP_CFG_RESET_CMD              (1)

/*  "telnet" command.*/
#define FAPP_CFG_TELNET_CMD             (0)

/*  "bench" command.*/
#define FAPP_CFG_BENCH_CMD              (1)

/* Reading of the configuration parameters from the Flash
 * memory during the application bootup.*/
#define FAPP_CFG_PARAMS_READ_FLASH       (1)

/* Rewriting of the configuration parameters in the Flash
 * memory duiring flashing of the application. */
#define FAPP_CFG_PARAMS_REWRITE_FLASH    (1)

#if 0 /* To run servers on startup set to 1. */
    #define FAPP_CFG_STARTUP_SCRIPT_ENABLED	(1)
    #define FAPP_CFG_STARTUP_SCRIPT "llmnr"     /* For example "http; telnet" */
#endif

#endif /* _FAPP_USER_CONFIG_H_ */

