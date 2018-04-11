/**************************************************************************
*
* Copyright 2008-2018 by Andrey Butok. FNET Community.
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
* FNET Demo Application Configuration.
*
***************************************************************************/
#ifndef _FAPP_CONFIG_H_

#define _FAPP_CONFIG_H_
#include "fnet_config.h"
#include "fapp_user_config.h"

/**************************************************************************/ /*!
 * @def      FAPP_CFG_NAME
 * @brief    Name of the application.
 * @showinitializer
 ******************************************************************************/
#ifndef FAPP_CFG_NAME
    #define FAPP_CFG_NAME                       "FNET Application"
#endif

/**************************************************************************/ /*!
 * @def      FAPP_CFG_SHELL_PROMPT
 * @brief    Prompt message used by the application shell.
 * @showinitializer
 ******************************************************************************/
#ifndef FAPP_CFG_SHELL_PROMPT
    #define FAPP_CFG_SHELL_PROMPT   "FNET> "
#endif

/**************************************************************************/ /*!
 * @def      FAPP_CFG_PARAMS_READ_FLASH
 * @brief    Reading of the configuration parameters from the Flash memory during
 *           the application bootup:
 *               - @c 1 = is enabled. @n
 *                        During bootup, the application read
 *                        and use the most recently stored customer
 *                        configuration data from the flash.
 *               - @c 0 = is disabled. @n
 *                        During bootup, the application uses
 *                        compile-time parameters by default.
 * @showinitializer
 ******************************************************************************/
#ifndef FAPP_CFG_PARAMS_READ_FLASH
    #define FAPP_CFG_PARAMS_READ_FLASH          (0)
#endif

/**************************************************************************/ /*!
 * @def      FAPP_CFG_PARAMS_REWRITE_FLASH
 * @brief    Rewriting of the configuration parameters in the Flash memory
 *           duiring flashing of the application:
 *               - @c 1 = is enabled. @n
 *                        It will allocated the default parameter structure
 *                        which  will rewrite the configurated parameters
 *                        present in the Flash memory during flashing of
 *                        the application.
 *               - @c 0 = is disabled. @n
 *                        The configuration parameters that present in the
 *                        flash memory will stay untouchable during flashing.
 * @showinitializer
 ******************************************************************************/
#ifndef FAPP_CFG_PARAMS_REWRITE_FLASH
    #define FAPP_CFG_PARAMS_REWRITE_FLASH       (0)
#endif

/**************************************************************************/ /*!
 * @def      FAPP_CFG_CHECK_FLASH_BEFORE_WRITE
 * @brief    Checking of the flash memory if is erased before write:
 *               - @c 1 = is enabled (default).
 *               - @c 0 = is disabled.
 * @showinitializer
 ******************************************************************************/
#ifndef FAPP_CFG_CHECK_FLASH_BEFORE_WRITE
    #define FAPP_CFG_CHECK_FLASH_BEFORE_WRITE       (1)
#endif

/**************************************************************************/ /*!
 * @def      FAPP_CFG_BOOTLOADER
 * @brief    Bootloader:
 *               - @c 1 = is enabled. @n
 *                        The application startup behaviour is defined
 *                        by the @ref fapp_params_boot_mode_t mode.
 *               - @c 0 = is disabled. @n
 * @showinitializer
 ******************************************************************************/
#ifndef FAPP_CFG_BOOTLOADER
    #define FAPP_CFG_BOOTLOADER                (0)
#endif

/* Startup script. */
#ifndef FAPP_CFG_STARTUP_SCRIPT
    #define FAPP_CFG_STARTUP_SCRIPT             ""
#endif

/* On connect/unconnect scrpts:
* @note "%s" will be replaced by the interface name.*/
/* All interfaces */
#ifndef FAPP_CFG_LINK_CONNECT_SCRIPT
    #define FAPP_CFG_LINK_CONNECT_SCRIPT        "dhcpc autoip -n %s; mdns -n %s; llmnr -n %s;"
#endif
#ifndef FAPP_CFG_LINK_DISCONNECT_SCRIPT
    #define FAPP_CFG_LINK_DISCONNECT_SCRIPT     "mdns -n %s release; llmnr -n %s release; dhcpc -n %s release; autoip -n %s release;"
#endif
/* Wi-Fi interface connect/unconnect scrpts, in the access point operation mode.*/
#ifndef FAPP_CFG_LINK_CONNECT_WIFI_ACCESS_POINT_SCRIPT
    #define FAPP_CFG_LINK_CONNECT_WIFI_ACCESS_POINT_SCRIPT      "dhcp -n %s; mdns -n %s; llmnr -n %s;"
#endif
#ifndef FAPP_CFG_LINK_DISCONNECT_WIFI_ACCESS_POINT_SCRIPT
    #define FAPP_CFG_LINK_DISCONNECT_WIFI_ACCESS_POINT_SCRIPT   "mdns -n %s release; llmnr -n %s release; dhcp -n %s release;"
#endif

/**************************************************************************/ /*!
 * @def      FAPP_CFG_FREERTOS
 * @brief    FreeRTOS support:
 *               - @c 1 = is enabled.
 *               - @c 0 = is disabled (bare-metal).
 * @showinitializer
 ******************************************************************************/
#ifndef FAPP_CFG_FREERTOS
    #define FAPP_CFG_FREERTOS                   (0)
#endif

/**************************************************************************/ /*!
 * @def      FAPP_CFG_FREERTOS_TASK_PRIORITY
 * @brief    FreeRTOS task priority
 * @showinitializer
 ******************************************************************************/
#ifndef FAPP_CFG_FREERTOS_TASK_PRIORITY
    #define FAPP_CFG_FREERTOS_TASK_PRIORITY     (configMAX_PRIORITIES - 3)
#endif

/**************************************************************************/ /*!
 * @def      FAPP_CFG_FREERTOS_TASK_STACK_SIZE
 * @brief    FreeRTOS task stack size
 * @showinitializer
 ******************************************************************************/
#ifndef FAPP_CFG_FREERTOS_TASK_STACK_SIZE
    #define FAPP_CFG_FREERTOS_TASK_STACK_SIZE   (0x2000)
#endif

/**************************************************************************/ /*!
 * @def      FAPP_CFG_FREERTOS_TASK_POLL_PERIOD_MS
 * @brief    Polling period for calling the fnet_service_poll() function.
 * @showinitializer
 ******************************************************************************/
#ifndef FAPP_CFG_FREERTOS_TASK_POLL_PERIOD_MS
    #define FAPP_CFG_FREERTOS_TASK_POLL_PERIOD_MS (1) /* ms */
#endif

/************************************************************************
* Memory regions.
*************************************************************************/

#ifndef FAPP_CFG_MEM_REGION_LIST
#if FNET_CFG_FLASH && FNET_CFG_CPU_FLASH
#define FAPP_CFG_MEM_REGION_LIST                {.description = "FLASH", \
            .address = FNET_CFG_CPU_FLASH_ADDRESS + FAPP_CFG_BOOTLOADER_SIZE, /* Reseved for bootloader */\
                       .size = FNET_CFG_CPU_FLASH_SIZE - FAPP_CFG_BOOTLOADER_SIZE - FAPP_CFG_FLASH_PARAMS_SIZE, /* Reseved for bootloader and parameters */ \
                               .memcpy = fnet_flash_memcpy, \
                                         .erase = fnet_flash_erase, \
                                                 .flush = fnet_flash_flush},
#else
#define FAPP_CFG_MEM_REGION_LIST
#endif
#endif

/************************************************************************
* Default application parameters (allocated in the Flash memory).
*************************************************************************/

/* Block size reserved for application parameters stored in flash.*/
#ifndef FAPP_CFG_FLASH_PARAMS_SIZE
    #define FAPP_CFG_FLASH_PARAMS_SIZE      FNET_CFG_CPU_FLASH_PAGE_SIZE
#endif
/* Pointer to application parameters stored in flash.*/
#ifndef FAPP_CFG_FLASH_PARAMS_ADDRESS
    #define FAPP_CFG_FLASH_PARAMS_ADDRESS   (FNET_CFG_CPU_FLASH_ADDRESS + FNET_CFG_CPU_FLASH_SIZE - FAPP_CFG_FLASH_PARAMS_SIZE) /* Last sector of the flash.*/
#endif

/* Default interface name .*/
#ifndef FAPP_CFG_PARAMS_NETIF_NAME
    #if FNET_CFG_CPU_ETH0
        #define FAPP_CFG_PARAMS_NETIF_NAME             FNET_CFG_CPU_ETH0_NAME
    #elif FNET_CFG_CPU_ETH1
        #define FAPP_CFG_PARAMS_NETIF_NAME             FNET_CFG_CPU_ETH1_NAME
    #elif FNET_CFG_CPU_WIFI
        #define FAPP_CFG_PARAMS_NETIF_NAME             FNET_CFG_CPU_WIFI_NAME
    #elif FNET_CFG_LOOPBACK
        #define FAPP_CFG_PARAMS_NETIF_NAME             FNET_CFG_LOOPBACK_NAME
    #else
        #define FAPP_CFG_PARAMS_NETIF_NAME             ""
    #endif
#endif

#ifndef FAPP_CFG_PARAMS_IP_ADDR
    #define FAPP_CFG_PARAMS_IP_ADDR             FAPP_CFG_ETH0_IP4_ADDR    /* Defined by FNET */
#endif

#ifndef FAPP_CFG_PARAMS_IP_MASK
    #define FAPP_CFG_PARAMS_IP_MASK             FAPP_CFG_ETH0_IP4_MASK    /* Defined by FNET */
#endif

#ifndef FAPP_CFG_PARAMS_IP_GW
    #define FAPP_CFG_PARAMS_IP_GW               FAPP_CFG_ETH0_IP4_GW      /* Defined by FNET */
#endif

#ifndef FAPP_CFG_PARAMS_IP_DNS
    #define FAPP_CFG_PARAMS_IP_DNS              FAPP_CFG_ETH0_IP4_DNS     /* Defined by FNET */
#endif

#ifndef FAPP_CFG_PARAMS_MAC_ADDR
    #define FAPP_CFG_PARAMS_MAC_ADDR            {0,0,0,0,0,0} /* Defined by FNET */
#endif

#ifndef FAPP_CFG_PARAMS_HOST_NAME
    #define FAPP_CFG_PARAMS_HOST_NAME           "fnet"
#endif

/* Bootloader parameters. */
#ifndef FAPP_CFG_PARAMS_BOOT_MODE
    #define FAPP_CFG_PARAMS_BOOT_MODE           FAPP_PARAMS_BOOT_MODE_STOP
#endif

#ifndef FAPP_CFG_PARAMS_BOOT_DELAY
    #define FAPP_CFG_PARAMS_BOOT_DELAY          (5) /*sec*/
#endif

#ifndef FAPP_CFG_PARAMS_BOOT_GO_ADDRESS
    #define FAPP_CFG_PARAMS_BOOT_GO_ADDRESS     (0)
#endif

#ifndef FAPP_CFG_PARAMS_BOOT_SCRIPT
    #define FAPP_CFG_PARAMS_BOOT_SCRIPT         ""
#endif

/* TFTP parameters */
#ifndef FAPP_CFG_PARAMS_TFTP_SERVER
    #define FAPP_CFG_PARAMS_TFTP_SERVER         {0,0,0,{0}}
#endif

#ifndef FAPP_CFG_PARAMS_TFTP_FILE_TYPE
    #define FAPP_CFG_PARAMS_TFTP_FILE_TYPE      FAPP_PARAMS_TFTP_FILE_TYPE_SREC
#endif

#ifndef FAPP_CFG_PARAMS_TFTP_FILE_NAME
    #define FAPP_CFG_PARAMS_TFTP_FILE_NAME      ""
#endif

#ifndef FAPP_CFG_PARAMS_TFTP_FILE_RAW_ADDRESS
    #define FAPP_CFG_PARAMS_TFTP_FILE_RAW_ADDRESS    (0)
#endif

/* Wi-Fi parameters */
#ifndef FAPP_CFG_PARAMS_WIFI_SSID
    #define FAPP_CFG_PARAMS_WIFI_SSID               "test"
#endif
#ifndef FAPP_CFG_PARAMS_WIFI_WPA_PASSPHRASE
    #define FAPP_CFG_PARAMS_WIFI_WPA_PASSPHRASE     ""
#endif

/* Wi-Fi Access Point parameters */
#ifndef FAPP_CFG_WIFI_AC_SSID
    #define FAPP_CFG_WIFI_AC_SSID                   "FNET"
#endif
#ifndef FAPP_CFG_WIFI_AC_WPA_PASSPHRASE
    #define FAPP_CFG_WIFI_AC_WPA_PASSPHRASE         ""                  /* Open Wi-Fi */
#endif
#ifndef FAPP_CFG_WIFI_AC_WPA_MODE
    #define FAPP_CFG_WIFI_AC_WPA_MODE               FNET_WIFI_WPA2      /* Open Wi-Fi */
#endif

/************************************************************************
*    "info" command.
*************************************************************************/
#ifndef FAPP_CFG_INFO_CMD
    #define FAPP_CFG_INFO_CMD           (0)
#endif

/************************************************************************
*    "stat" command.
*************************************************************************/
#ifndef FAPP_CFG_STAT_CMD
    #define FAPP_CFG_STAT_CMD           (0)
#endif

/************************************************************************
*    "dhcpc" command.
*************************************************************************/
#ifndef FAPP_CFG_DHCPC_CMD
    #define FAPP_CFG_DHCPC_CMD           (0)
#endif

#ifndef FAPP_CFG_DHCPC_CMD_DISCOVER_MAX
    #define FAPP_CFG_DHCPC_CMD_DISCOVER_MAX  (-1) /* -1 means infinite. */
#endif


/************************************************************************
*    "dhcp" command.
*************************************************************************/
#ifndef FAPP_CFG_DHCP_CMD
    #define FAPP_CFG_DHCP_CMD           (0)
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default IPv4 address for an interface running the DHCP server.
 ******************************************************************************/
#ifndef FAPP_CFG_DHCP_CMD_IP4_ADDR
    #define FAPP_CFG_DHCP_CMD_IP4_ADDR  (FNET_IP4_ADDR_INIT(192U, 168U, 0U, 1U))
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default IP Subnetmask for an interface running the DHCP server.
 ******************************************************************************/
#ifndef FAPP_CFG_DHCP_CMD_IP4_MASK
    #define FAPP_CFG_DHCP_CMD_IP4_MASK          (FNET_IP4_ADDR_INIT(255U, 255U, 255U, 0U))
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default Gateway IP address for an interface running the DHCP server.
 ******************************************************************************/
#ifndef FAPP_CFG_DHCP_CMD_IP4_GW
    #define FAPP_CFG_DHCP_CMD_IP4_GW            (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default DNS IP address for an interface running the DHCP server.
 ******************************************************************************/
#ifndef FAPP_CFG_DHCP_CMD_IP4_DNS
    #define FAPP_CFG_DHCP_CMD_IP4_DNS           (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))
#endif

/************************************************************************
*    "autoip" command.
*************************************************************************/
#ifndef FAPP_CFG_AUTOIP_CMD
    #define FAPP_CFG_AUTOIP_CMD         (0)
#endif

/************************************************************************
*    "http" command.
*************************************************************************/
#ifndef FAPP_CFG_HTTP_CMD
    #define FAPP_CFG_HTTP_CMD           (0)
#endif

/************************************************************************
*    "https" command.
*************************************************************************/
#ifndef FAPP_CFG_HTTP_TLS_CMD
    #define FAPP_CFG_HTTP_TLS_CMD       (0)
#endif

/************************************************************************
*    "exp" command.
*************************************************************************/
#ifndef FAPP_CFG_EXP_CMD
    #define FAPP_CFG_EXP_CMD            (0)
#endif

/************************************************************************
*    "tftpc" command.
*************************************************************************/
#ifndef FAPP_CFG_TFTPC_CMD
    #define FAPP_CFG_TFTPC_CMD          (0)
#endif

/************************************************************************
*    "tftpup" command.
*************************************************************************/
#ifndef FAPP_CFG_TFTPCUP_CMD
    #define FAPP_CFG_TFTPCUP_CMD        (0)
#endif

/************************************************************************
*    "tftps" command.
*************************************************************************/
#ifndef FAPP_CFG_TFTP_CMD
    #define FAPP_CFG_TFTP_CMD           (0)
#endif

/************************************************************************
*    "telnet" command.
*************************************************************************/
#ifndef FAPP_CFG_TELNET_CMD
    #define FAPP_CFG_TELNET_CMD         (0)
#endif

/************************************************************************
*    "mem" command.
*************************************************************************/
#ifndef FAPP_CFG_MEM_CMD
    #define FAPP_CFG_MEM_CMD            (0)
#endif

/************************************************************************
*    "erase" command.
*************************************************************************/
#ifndef FAPP_CFG_ERASE_CMD
    #define FAPP_CFG_ERASE_CMD          (0)
#endif

/************************************************************************
*    "save" command.
*************************************************************************/
#ifndef FAPP_CFG_SAVE_CMD
    #define FAPP_CFG_SAVE_CMD           (0)
#endif

/************************************************************************
*    "go" command.
*************************************************************************/
#ifndef FAPP_CFG_GO_CMD
    #define FAPP_CFG_GO_CMD             (0)
#endif

/************************************************************************
*    "reset" command.
*************************************************************************/
#ifndef FAPP_CFG_RESET_CMD
    #define FAPP_CFG_RESET_CMD          (0)
#endif

/************************************************************************
*    "reboot" command.
*************************************************************************/
#ifndef FAPP_CFG_REBOOT_CMD
    #define FAPP_CFG_REBOOT_CMD         (0)
#endif

/************************************************************************
*    "bench" command.
*************************************************************************/
#ifndef FAPP_CFG_BENCH_CMD
    #define FAPP_CFG_BENCH_CMD          (0)
#endif

/************************************************************************
*    "reinit" command. Used to test FNET release/init only.
*************************************************************************/
#ifndef FAPP_CFG_REINIT_CMD
    #define FAPP_CFG_REINIT_CMD         (0)
#endif

/************************************************************************
*    "test" command. Used to test Telnet sending. For debug needs only
*************************************************************************/
#ifndef FAPP_CFG_TELNET_TEST_CMD
    #define FAPP_CFG_TELNET_TEST_CMD    (0)
#endif

/************************************************************************
*    "dns" command.
*************************************************************************/
#ifndef FAPP_CFG_DNS_CMD
    #define FAPP_CFG_DNS_CMD            (0)
#endif

/************************************************************************
*    "sntp" command.
*************************************************************************/
#ifndef FAPP_CFG_SNTP_CMD
    #define FAPP_CFG_SNTP_CMD           (0)
#endif

/************************************************************************
*    "bind" command.
*************************************************************************/
#ifndef FAPP_CFG_BIND_CMD
    #define FAPP_CFG_BIND_CMD           (0)
#endif

/************************************************************************
*    "unbind" command.
*************************************************************************/
#ifndef FAPP_CFG_UNBIND_CMD
    #define FAPP_CFG_UNBIND_CMD         (0)
#endif

/************************************************************************
*    "ping" command.
*************************************************************************/
#ifndef FAPP_CFG_PING_CMD
    #define FAPP_CFG_PING_CMD           (0)
#endif

/************************************************************************
*    "ping6" command. The same as "ping".
*************************************************************************/
#ifndef FAPP_CFG_PING6_CMD
    #define FAPP_CFG_PING6_CMD          (0)
#endif

/************************************************************************
*    "llmnr" command.
*************************************************************************/
#ifndef FAPP_CFG_LLMNR_CMD
    #define FAPP_CFG_LLMNR_CMD          (0)
#endif

/************************************************************************
*    "mdns" command.
*************************************************************************/
#ifndef FAPP_CFG_MDNS_CMD
    #define FAPP_CFG_MDNS_CMD           (0)
#endif

/************************************************************************
*    "mdnscn" command. Manual Name Change test.
*    It is used only for Bonjour Conformance Testing.
*************************************************************************/
#ifndef FAPP_CFG_MDNS_CN_CMD
    #define FAPP_CFG_MDNS_CN_CMD        (0)
#endif

/************************************************************************
*    "d" command. Used for DEBUGGING needs only.
*************************************************************************/
#ifndef FAPP_CFG_DEBUG_CMD
    #define FAPP_CFG_DEBUG_CMD          (0)
#endif

/************************************************************************
*    "set/get" command.
*************************************************************************/
/* FNET TCP/IP stack set/get parameters. */
#ifndef FAPP_CFG_SETGET_CMD_NETIF
    #define FAPP_CFG_SETGET_CMD_NETIF       (0)
#endif
#ifndef FAPP_CFG_SETGET_CMD_IP
    #define FAPP_CFG_SETGET_CMD_IP          (0)
#endif
#ifndef FAPP_CFG_SETGET_CMD_GATEWAY
    #define FAPP_CFG_SETGET_CMD_GATEWAY     (0)
#endif
#ifndef FAPP_CFG_SETGET_CMD_NETMASK
    #define FAPP_CFG_SETGET_CMD_NETMASK     (0)
#endif
#ifndef FAPP_CFG_SETGET_CMD_MAC
    #define FAPP_CFG_SETGET_CMD_MAC         (0)
#endif
#ifndef FAPP_CFG_SETGET_CMD_HOSTNAME
    #define FAPP_CFG_SETGET_CMD_HOSTNAME    (0)
#endif

/* Bootloader set/get parameters. */
#ifndef FAPP_CFG_SETGET_CMD_BOOT
    #define FAPP_CFG_SETGET_CMD_BOOT    (0)
#endif
#ifndef FAPP_CFG_SETGET_CMD_DELAY
    #define FAPP_CFG_SETGET_CMD_DELAY   (0)
#endif
#ifndef FAPP_CFG_SETGET_CMD_SCRIPT
    #define FAPP_CFG_SETGET_CMD_SCRIPT  (0)
#endif
#ifndef FAPP_CFG_SETGET_CMD_RAW
    #define FAPP_CFG_SETGET_CMD_RAW     (0)
#endif

/* TFTP set/get parameters */
#ifndef FAPP_CFG_SETGET_CMD_TFTP
    #define FAPP_CFG_SETGET_CMD_TFTP    (0)
#endif
#ifndef FAPP_CFG_SETGET_CMD_IMAGE
    #define FAPP_CFG_SETGET_CMD_IMAGE   (0)
#endif
#ifndef FAPP_CFG_SETGET_CMD_TYPE
    #define FAPP_CFG_SETGET_CMD_TYPE    (0)
#endif
#ifndef FAPP_CFG_SETGET_CMD_GO
    #define FAPP_CFG_SETGET_CMD_GO      (0)
#endif

/* Wi-Fi set/get parameters */
#ifndef FAPP_CFG_SETGET_CMD_SSID
    #define FAPP_CFG_SETGET_CMD_SSID        (0)
#endif
#ifndef FAPP_CFG_SETGET_CMD_PASSPHRASE
    #define FAPP_CFG_SETGET_CMD_PASSPHRASE  (0)
#endif

/* DNS set/get parameters */
#ifndef FAPP_CFG_SETGET_CMD_DNS
    #define FAPP_CFG_SETGET_CMD_DNS     (0)
#endif

#ifndef FAPP_CFG_SETGET_CMD
#define FAPP_CFG_SETGET_CMD             (FAPP_CFG_SETGET_CMD_IP|FAPP_CFG_SETGET_CMD_GATEWAY|\
        FAPP_CFG_SETGET_CMD_NETMASK|FAPP_CFG_SETGET_CMD_MAC|\
        FAPP_CFG_SETGET_CMD_BOOT|FAPP_CFG_SETGET_CMD_DELAY|\
        FAPP_CFG_SETGET_CMD_SCRIPT|FAPP_CFG_SETGET_CMD_RAW|\
        FAPP_CFG_SETGET_CMD_TFTP|FAPP_CFG_SETGET_CMD_IMAGE|\
        FAPP_CFG_SETGET_CMD_TYPE|FAPP_CFG_SETGET_CMD_GO|\
        FAPP_CFG_SETGET_CMD_DNS|FAPP_CFG_SETGET_CMD_HOSTNAME|\
        FAPP_CFG_SETGET_CMD_SSID|FAPP_CFG_SETGET_CMD_PASSPHRASE)
#endif


#ifndef FAPP_CFG_PARAMS_BOOT
#define FAPP_CFG_PARAMS_BOOT            (FAPP_CFG_SETGET_CMD_BOOT|FAPP_CFG_SETGET_CMD_DELAY|\
        FAPP_CFG_SETGET_CMD_SCRIPT|FAPP_CFG_SETGET_CMD_RAW|\
        FAPP_CFG_SETGET_CMD_GO|FAPP_CFG_BOOTLOADER )
#endif

#ifndef FAPP_CFG_PARAMS_TFTP
#define FAPP_CFG_PARAMS_TFTP            (FAPP_CFG_SETGET_CMD_TFTP|FAPP_CFG_SETGET_CMD_IMAGE|\
        FAPP_CFG_SETGET_CMD_TYPE|FAPP_CFG_SETGET_CMD_GO|\
        FAPP_CFG_TFTPC_CMD|FAPP_CFG_TFTPCUP_CMD|FAPP_CFG_TFTP_CMD)
#endif

#ifndef FAPP_CFG_PARAMS_WIFI
    #define FAPP_CFG_PARAMS_WIFI            (FAPP_CFG_SETGET_CMD_SSID|FAPP_CFG_SETGET_CMD_PASSPHRASE)
#endif


#ifndef FAPP_CFG_TFTPS_ON_WRITE_REQUEST_SCRIPT
    #define FAPP_CFG_TFTPS_ON_WRITE_REQUEST_SCRIPT      ""
#endif

#ifndef FAPP_CFG_TFTPS_AFTER_WRITE_REQUEST_SCRIPT
    #define FAPP_CFG_TFTPS_AFTER_WRITE_REQUEST_SCRIPT   ""
#endif

/************************************************************************
*    TFTP data handlers
*************************************************************************/
#ifndef FAPP_CFG_TFTP_RX_RAW
    #define FAPP_CFG_TFTP_RX_RAW            (0)
#endif

#ifndef FAPP_CFG_TFTP_TX_RAW
    #define FAPP_CFG_TFTP_TX_RAW            (0)
#endif

#ifndef FAPP_CFG_TFTP_RX_BIN
    #define FAPP_CFG_TFTP_RX_BIN            (0)
#endif

#ifndef FAPP_CFG_TFTP_TX_BIN
    #define FAPP_CFG_TFTP_TX_BIN            (0)
#endif

#ifndef FAPP_CFG_TFTP_RX_SREC
    #define FAPP_CFG_TFTP_RX_SREC           (1)
#endif

#ifndef FAPP_CFG_TFTP_TX_SREC
    #define FAPP_CFG_TFTP_TX_SREC           (1)
#endif

#ifndef FAPP_CFG_TFTP_RX_HEX
    #define FAPP_CFG_TFTP_RX_HEX            (0)
#endif

#ifndef FAPP_CFG_TFTP_TX_HEX
    #define FAPP_CFG_TFTP_TX_HEX            (0)
#endif

/************************************************************************/
#ifndef FAPP_CFG_SHELL_MAX_LINE_LENGTH
    #define FAPP_CFG_SHELL_MAX_LINE_LENGTH  (100)
#endif

/************************************************************************
*    Memory parameters
*************************************************************************/
/**************************************************************************/ /*!
 * @brief    Size of the internal static heap buffer, that is passed to @ref fnet_init().
 ******************************************************************************/
#ifndef FAPP_CFG_HEAP_SIZE
    #define FAPP_CFG_HEAP_SIZE                  (30U * 1024U)
#endif

/* Start address of the ROM memory, reserved/protected for the application. Used by the bootloader application.*/
#ifndef FAPP_CFG_APPLICATION_ADDRESS
    #define FAPP_CFG_APPLICATION_ADDRESS      FNET_CFG_CPU_FLASH_ADDRESS
#endif

/* ROM memory size, reserved/protected for the application. Used by the bootloader application.
* This value must be dividable by FNET_CFG_CPU_FLASH_PAGE_SIZE.
* On any change, must be updated the linker file of shell_boot application. */
#ifndef FAPP_CFG_BOOTLOADER_SIZE
    #define FAPP_CFG_BOOTLOADER_SIZE            (52*1024) /* 52 KB */
#endif

/************************************************************************
*    Network Interface Address parameters
*************************************************************************/
/**************************************************************************/ /*!
 * @brief    Defines the default MAC address of the Ethernet-0 interface.
 *           At runtime, it can be changed by @ref fnet_netif_set_hw_addr().
 *           It must be unique on local network.
 ******************************************************************************/
#ifndef FAPP_CFG_CPU_ETH0_MAC_ADDR
    #define FAPP_CFG_CPU_ETH0_MAC_ADDR         FNET_MAC_ADDR_INIT(0x00, 0x11, 0x22, 0x33, 0x44, 0x55)
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default MAC address of the Ethernet-1 interface.
 *           At runtime, it can be changed by @ref fnet_netif_set_hw_addr().
 *           It must be unique on local network.
 ******************************************************************************/
#ifndef FAPP_CFG_CPU_ETH1_MAC_ADDR
    #define FAPP_CFG_CPU_ETH1_MAC_ADDR        FNET_MAC_ADDR_INIT(0x22, 0x33, 0x44, 0x55, 0x66, 0x77)
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default IP address for the Ethernet-0 interface.
 *           At runtime, it can be changed by @ref fnet_netif_set_ip4_addr().
 ******************************************************************************/
#ifndef FAPP_CFG_ETH0_IP4_ADDR
    #define FAPP_CFG_ETH0_IP4_ADDR          (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default IP Subnetmask for the Ethernet-0 interface.
 *           At runtime, it can be changed by @ref fnet_netif_set_ip4_addr().
 ******************************************************************************/
#ifndef FAPP_CFG_ETH0_IP4_MASK
    #define FAPP_CFG_ETH0_IP4_MASK          (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default Gateway IP address for the Ethernet-0 interface.
 *           At runtime, it can be changed by @ref fnet_netif_set_ip4_gateway().
 ******************************************************************************/
#ifndef FAPP_CFG_ETH0_IP4_GW
    #define FAPP_CFG_ETH0_IP4_GW            (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default DNS IP address for the Ethernet-0 interface.
 *           At runtime, it can be changed by @ref fnet_netif_set_ip4_dns().
 ******************************************************************************/
#ifndef FAPP_CFG_ETH0_IP4_DNS
    #define FAPP_CFG_ETH0_IP4_DNS           (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default IP address for the Ethernet-1 interface.
 *           At runtime, it can be changed by @ref fnet_netif_set_ip4_addr().
 ******************************************************************************/
#ifndef FAPP_CFG_ETH1_IP4_ADDR
    #define FAPP_CFG_ETH1_IP4_ADDR          (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default IP Subnetmask for the Ethernet-1 interface.
 *           At runtime, it can be changed by @ref fnet_netif_set_ip4_addr().
 ******************************************************************************/
#ifndef FAPP_CFG_ETH1_IP4_MASK
    #define FAPP_CFG_ETH1_IP4_MASK          (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default Gateway IP address for the Ethernet-1 interface.
 *           At runtime, it can be changed by @ref fnet_netif_set_ip4_gateway().
 ******************************************************************************/
#ifndef FAPP_CFG_ETH1_IP4_GW
    #define FAPP_CFG_ETH1_IP4_GW            (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default DNS IP address for the Ethernet-1 interface.
 *           At runtime, it can be changed by @ref fnet_netif_set_ip4_dns().
 ******************************************************************************/
#ifndef FAPP_CFG_ETH1_IP4_DNS
    #define FAPP_CFG_ETH1_IP4_DNS           (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default IP address for the Wi-Fi interface.
 *           At runtime, it can be changed by @ref fnet_netif_set_ip4_addr().
 ******************************************************************************/
#ifndef FAPP_CFG_WIFI_IP4_ADDR
    #define FAPP_CFG_WIFI_IP4_ADDR          (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default IP Subnetmask for the Wi-Fi interface.
 *           At runtime, it can be changed by @ref fnet_netif_set_ip4_addr().
 ******************************************************************************/
#ifndef FAPP_CFG_WIFI_IP4_MASK
    #define FAPP_CFG_WIFI_IP4_MASK          (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default Gateway IP address for the Wi-Fi interface.
 *           At runtime, it can be changed by @ref fnet_netif_set_ip4_gateway().
 ******************************************************************************/
#ifndef FAPP_CFG_WIFI_IP4_GW
    #define FAPP_CFG_WIFI_IP4_GW            (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default DNS IP address for the Wi-Fi interface.
 *           At runtime, it can be changed by @ref fnet_netif_set_ip4_dns().
 ******************************************************************************/
#ifndef FAPP_CFG_WIFI_IP4_DNS
    #define FAPP_CFG_WIFI_IP4_DNS           (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default IP address for the Wi-Fi interface in Access Point mode.
 *           At runtime, it can be changed by @ref fnet_netif_set_ip4_addr().
 ******************************************************************************/
#ifndef FAPP_CFG_WIFI_AP_IP4_ADDR
    #define FAPP_CFG_WIFI_AP_IP4_ADDR          (FNET_IP4_ADDR_INIT(192U, 168U, 0U, 1U))
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default IP Subnetmask for the Wi-Fi interface in Access Point mode.
 *           At runtime, it can be changed by @ref fnet_netif_set_ip4_addr().
 ******************************************************************************/
#ifndef FAPP_CFG_WIFI_AP_IP4_MASK
    #define FAPP_CFG_WIFI_AP_IP4_MASK          (FNET_IP4_ADDR_INIT(255U, 255U, 255U, 0U))
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default Gateway IP address for the Wi-Fi interface in Access Point mode.
 *           At runtime, it can be changed by @ref fnet_netif_set_ip4_gateway().
 ******************************************************************************/
#ifndef FAPP_CFG_WIFI_AP_IP4_GW
    #define FAPP_CFG_WIFI_AP_IP4_GW            (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))
#endif

/**************************************************************************/ /*!
 * @brief    Defines the default DNS IP address for the Wi-Fi interface in Access Point mode.
 *           At runtime, it can be changed by @ref fnet_netif_set_ip4_dns().
 ******************************************************************************/
#ifndef FAPP_CFG_WIFI_AP_IP4_DNS
    #define FAPP_CFG_WIFI_AP_IP4_DNS           (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))
#endif

/**************************************************************************/ /*!
 * @brief   Testing of mutex nesting conflict:
 *               - @b @c 1 = is enabled.
 *               - @c 0 = is disabled (Default value).@n
 * Valid only if FNET_CFG_MULTITHREADING is 1.
 * @note Used only for testing needs.
 ******************************************************************************/
#ifndef FAPP_CFG_TEST_MUTEX_CONFLICT
    #define FAPP_CFG_TEST_MUTEX_CONFLICT    0
#endif


#ifdef FNET_CFG_HEAP_SIZE
    #error "FAPP_CFG_DEFAULT_IF parameter is obsolete. It is covered by FAPP_CFG_PARAMS_NETIF_NAME."
#endif

#endif
