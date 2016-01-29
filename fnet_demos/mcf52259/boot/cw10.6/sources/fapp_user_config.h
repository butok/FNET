
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

#define FAPP_CFG_APPLICATION_SIZE       (48*1024) /* First 48 KB of FLASH are reserved by 
                                                   * the Bootloader application.*/

#define FAPP_CFG_NAME                   "FNET Bootloader" 
#define FAPP_CFG_SHELL_PROMPT           "BOOT> "

/* Enable startup boot mode. */
#define FAPP_CFG_BOOTLOADER             (1)

#define FAPP_CFG_PREINSTALL_INTERRUPTS  (1)

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

/*  "info" command. */
#define FAPP_CFG_INFO_CMD               (1)

/*  "tftp" command.*/
#define FAPP_CFG_TFTP_CMD               (1)

/*  "tftpup" command.*/
#define FAPP_CFG_TFTPUP_CMD             (1)

/*  "tftps" command.*/
#define FAPP_CFG_TFTPS_CMD              (1)

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

/* Read init parameters from flash.*/
#define FAPP_CFG_PARAMS_READ_FLASH      (1)

/* Rewrite init parameters */
#define FAPP_CFG_PARAMS_REWRITE_FLASH   (1)

/* Default parameters values*/
#define FAPP_CFG_PARAMS_TFTP_FILE_NAME  "shell_boot_intflash.elf.S19"

#if FNET_CFG_IP4
    #define FAPP_CFG_PARAMS_TFTP_SERVER     {AF_INET, 0, {192,168,0,5}} 
#else /* FNET_CFG_IP6 */
    #define FAPP_CFG_PARAMS_TFTP_SERVER     {AF_INET6, 0, {0xfe,0x80,0,0,0,0,0,0,0xa0,0x9a,0x5,0x30,0xab,0xf7,0xa8,0xfd}}
#endif

#define FAPP_CFG_PARAMS_BOOT_MODE       FAPP_PARAMS_BOOT_MODE_STOP 
#define FAPP_CFG_PARAMS_BOOT_SCRIPT     "dhcp; erase all; tftp; set boot go; save; go"

/* Script on TFTP server "WRITE request" received.*/
#define FAPP_CFG_TFTPS_ON_WRITE_REQUEST_SCRIPT      "erase all"
/* Script on TFTP server "WRITE request" successfully  completed.*/
#define FAPP_CFG_TFTPS_AFTER_WRITE_REQUEST_SCRIPT   "set boot go; save; go"

/* Default startup script.*/
#define FAPP_CFG_STARTUP_SCRIPT_ENABLED (1)
#define FAPP_CFG_STARTUP_SCRIPT         "tftps"


#endif

