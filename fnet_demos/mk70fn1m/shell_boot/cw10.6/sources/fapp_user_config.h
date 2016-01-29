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

#define FAPP_CFG_NAME                   "FNET Shell Demo (bootloader aware)" 
#define FAPP_CFG_SHELL_PROMPT           "SHELLB> " 

/*  "dhcp" command.*/
#define FAPP_CFG_DHCP_CMD               (1)
#define FAPP_CFG_DHCP_CMD_DISCOVER_MAX  (5)

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
#define FAPP_CFG_SETGET_CMD_DNS         (1)

/*  "info" command. */
#define FAPP_CFG_INFO_CMD               (1)

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

/* Reading of the configuration parameters from the Flash 
 * memory during the application bootup.*/
#define FAPP_CFG_PARAMS_READ_FLASH      0 //DM (1)

#endif

