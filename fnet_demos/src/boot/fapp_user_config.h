/**********************************************************************/
/*!
*
* @file fapp_user_config.h
*
* @brief FNET Application User configuration file.
* It should be used to change any default configuration parameter of FAPP.
*
***************************************************************************/

#ifndef _FAPP_USER_CONFIG_H_

#define _FAPP_USER_CONFIG_H_

#define FAPP_CFG_BOOTLOADER_SIZE       (52*1024) /* First 52 KBs of FLASH are reserved by 
                                                   * the Bootloader application.*/

#define FAPP_CFG_NAME                   "FNET Bootloader"
#define FAPP_CFG_SHELL_PROMPT           "BOOT> "

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

/* Enable startup boot mode. */
#define FAPP_CFG_BOOTLOADER             (1)

#define FAPP_CFG_PREINSTALL_INTERRUPTS  (1)

/*  "dhcpc" command.*/
#define FAPP_CFG_DHCPC_CMD               (1)
#define FAPP_CFG_DHCPC_CMD_DISCOVER_MAX  (5)

/*  "set/get" command.*/
#define FAPP_CFG_SETGET_CMD_IP          (1)
#define FAPP_CFG_SETGET_CMD_GATEWAY     (1)
#define FAPP_CFG_SETGET_CMD_NETMASK     (1)
#define FAPP_CFG_SETGET_CMD_MAC         (1)
#define FAPP_CFG_SETGET_CMD_BOOT        (1)
#define FAPP_CFG_SETGET_CMD_DELAY       (1)
#define FAPP_CFG_SETGET_CMD_SCRIPT      (1)
#define FAPP_CFG_SETGET_CMD_RAW         (1)
#define FAPP_CFG_SETGET_CMD_TFTP        (1)
#define FAPP_CFG_SETGET_CMD_IMAGE       (1)
#define FAPP_CFG_SETGET_CMD_TYPE        (1)
#define FAPP_CFG_SETGET_CMD_GO          (1)

/*  "info" command. */
#define FAPP_CFG_INFO_CMD               (1)

/*  "tftpc" command.*/
#define FAPP_CFG_TFTPC_CMD              (1)

/*  "tftpcup" command.*/
#define FAPP_CFG_TFTPCUP_CMD            (1)

/*  "tftp" command.*/
#define FAPP_CFG_TFTP_CMD               (1)

/*  "mem" command.*/
#define FAPP_CFG_MEM_CMD                (1)

/*  "erase" command.*/
#define FAPP_CFG_ERASE_CMD              (1)

/*  "save" command.*/
#define FAPP_CFG_SAVE_CMD               (1)

/*  "go" command.*/
#define FAPP_CFG_GO_CMD                 (1)

/*  "reset" command.*/
#define FAPP_CFG_RESET_CMD              (1)

/* Reading of the configuration parameters from the Flash
 * memory during the application bootup.*/
#define FAPP_CFG_PARAMS_READ_FLASH       (1)

/* Rewriting of the configuration parameters in the Flash
 * memory duiring flashing of the application. */
#define FAPP_CFG_PARAMS_REWRITE_FLASH    (1)

/* Default parameters values*/
#define FAPP_CFG_PARAMS_TFTP_FILE_NAME  "fnet_firmware.srec"

/* Remote TFTP Server Address.*/
#if FNET_CFG_IP4
    #define FAPP_CFG_PARAMS_TFTP_SERVER     {.sa_family = AF_INET, .sa_data = {192,168,0,200}}
#else /* FNET_CFG_IP6 */
    #define FAPP_CFG_PARAMS_TFTP_SERVER     {.sa_family = AF_INET6, .sa_data = {0xfe,0x80,0,0,0,0,0,0,0xa0,0x9a,0x5,0x30,0xab,0xf7,0xa8,0xfd}}
#endif

#define FAPP_CFG_PARAMS_BOOT_MODE       FAPP_PARAMS_BOOT_MODE_STOP
#define FAPP_CFG_PARAMS_BOOT_SCRIPT     "dhcpc; erase all; tftpc; set boot go; save; go"

/* Script on TFTP server "WRITE request" received.*/
#define FAPP_CFG_TFTPS_ON_WRITE_REQUEST_SCRIPT      "erase all"
/* Script on TFTP server "WRITE request" successfully  completed.*/
#define FAPP_CFG_TFTPS_AFTER_WRITE_REQUEST_SCRIPT   "set boot go; save; go"

/* Default startup script.*/
#define FAPP_CFG_STARTUP_SCRIPT         "tftp"

#endif /* _FAPP_USER_CONFIG_H_ */

