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
***************************************************************************
*
*  FNET Shell Demo implementation.
*
***************************************************************************/

#include "fapp.h"

#include "fapp_prv.h"

#if FAPP_CFG_SETGET_CMD
    #include "fapp_setget.h"
#endif

#include "fapp_mem.h"

#if ((FAPP_CFG_DHCPC_CMD && FNET_CFG_DHCP_CLN) || (FAPP_CFG_DHCPC_CMD && FNET_CFG_DHCP_CLN)) && FNET_CFG_IP4
    #include "fapp_dhcp.h"
#endif

#if FAPP_CFG_AUTOIP_CMD && FNET_CFG_AUTOIP && FNET_CFG_IP4
    #include "fapp_autoip.h"
#endif

#if ((FAPP_CFG_HTTP_CMD || FAPP_CFG_HTTP_TLS_CMD) && FNET_CFG_HTTP)|| (FAPP_CFG_EXP_CMD && FNET_CFG_FS)
    #include "fapp_http.h"
    #include "fapp_fs.h"
#endif

#if FAPP_CFG_TFTPC_CMD || FAPP_CFG_TFTPCUP_CMD || FAPP_CFG_TFTP_CMD
    #include "fapp_tftp.h"
#endif

#if FAPP_CFG_TELNET_CMD && FNET_CFG_TELNET
    #include "fapp_telnet.h"
#endif

#if FAPP_CFG_DNS_CMD && FNET_CFG_DNS
    #include "fapp_dns.h"
#endif

#if FAPP_CFG_BENCH_CMD
    #include "fapp_bench.h"
#endif

#if FNET_CFG_PING && (FAPP_CFG_PING_CMD || FAPP_CFG_PING6_CMD)
    #include "fapp_ping.h"
#endif

#if FAPP_CFG_LLMNR_CMD && FNET_CFG_LLMNR
    #include "fapp_llmnr.h"
#endif

#if FAPP_CFG_MDNS_CMD && FNET_CFG_MDNS
    #include "fapp_mdns.h"
#endif

#if FAPP_CFG_SNTP_CMD && FNET_CFG_SNTP
    #include "fapp_sntp.h"
#endif

#if FNET_CFG_CPU_WIFI
    #include "fapp_wifi.h"
#endif

#include "fapp_netif.h"

/************************************************************************
*     Definitions.
*************************************************************************/
const fnet_char_t FAPP_DELIMITER_STR[] = "************************************************";
const fnet_char_t FAPP_CANCELLED_STR[] = "Cancelled";
const fnet_char_t FAPP_TOCANCEL_STR[] = "Press [Ctr+C] to cancel.";
const fnet_char_t FAPP_UPDATED_IP_STR[] = " IPv4 parameters updated :";

/* Error mesages */
const fnet_char_t FAPP_PARAM_ERR[] = "Error: Invalid paremeter \'%s\'";
const fnet_char_t FAPP_INIT_ERR[]  = "Error: %s initialization is failed!";
const fnet_char_t FAPP_SHELL_INFO_FORMAT_S[]  = " %-20s : %s";
const fnet_char_t FAPP_SHELL_INFO_FORMAT_D[]  = " %-20s : %u";
const fnet_char_t FAPP_SHELL_INFO_FORMAT_H[]  = " %-20s : 0x%8x";
const fnet_char_t FAPP_SHELL_CANCELED_CTRL_C[]  = "Canceled by [Ctrl+C]!";

/* Service release command */
const fnet_char_t FAPP_COMMAND_RELEASE [] = "release";

#define FAPP_GO_STR             "go 0x%08X"

#define FAPP_SAVE_STR           "Application parameters saved"
#define FAPP_SAVE_FAILED_STR    "Parameters saving failed!"

/************************************************************************
*     Function Prototypes
*************************************************************************/
#if FAPP_CFG_REINIT_CMD
    void fapp_reinit_cmd ( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
#endif
#if FAPP_CFG_DEBUG_CMD
    void fapp_debug_cmd ( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
#endif
#if FAPP_CFG_BIND_CMD && FNET_CFG_IP6
    static void fapp_bind_cmd ( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
#endif
#if FAPP_CFG_RESET_CMD || FAPP_CFG_REBOOT_CMD
    static void fapp_reset_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
#endif
#if FAPP_CFG_SAVE_CMD && FNET_CFG_FLASH && FNET_CFG_CPU_FLASH
    static void fapp_save_cmd ( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
#endif
#if FAPP_CFG_UNBIND_CMD && FNET_CFG_IP6
    static void fapp_unbind_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
#endif
#if FAPP_CFG_EXP_CMD && FNET_CFG_FS
    static void fapp_exp_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
#endif


/************************************************************************
*     The table of the main shell commands.
*************************************************************************/
const struct fnet_shell_command fapp_cmd_table [] =
{
    { .name = "?",       .min_args = 0u, .max_args = 0u,     .cmd_ptr = fapp_help_cmd,    .description = "Display this help message", .syntax = ""},
#if FAPP_CFG_SETGET_CMD
    { .name = "set",        .min_args = 0u, .max_args = 2u,     .cmd_ptr = fapp_set_cmd,     .description = "Set parameter", .syntax = "[<parameter> <value>]"},
    { .name = "get",        .min_args = 0u, .max_args = 1u,     .cmd_ptr = fapp_get_cmd,     .description = "Get parameters", .syntax = "[<parameter>]" },
#endif
#if FAPP_CFG_BIND_CMD && FNET_CFG_IP6
    { .name = "bind",       .min_args = 1u, .max_args = 3u,     .cmd_ptr = fapp_bind_cmd,    .description = "Bind IPv6 Address", .syntax = "[-n <if name>] <IP6 address>"},
#endif
#if FAPP_CFG_UNBIND_CMD && FNET_CFG_IP6
    { .name = "unbind",     .min_args = 1u, .max_args = 3u,     .cmd_ptr = fapp_unbind_cmd,  .description = "Unbind IPv6 Address", .syntax = "[-n <if name>] <IP6 address>"},
#endif
#if FAPP_CFG_INFO_CMD
    { .name = "info",       .min_args = 0u, .max_args = 2u,     .cmd_ptr = fapp_netif_info_cmd,    .description = "Show interface info", .syntax = "[-n <if name>]"},
#endif
#if FAPP_CFG_STAT_CMD
    { .name = "stat",       .min_args = 0u, .max_args = 2u,     .cmd_ptr = fapp_netif_stat_cmd,    .description = "Show interface statistics", .syntax = "[-n <if name>]"},
#endif
#if FAPP_CFG_DHCPC_CMD && FNET_CFG_DHCP_CLN && FNET_CFG_IP4
    { .name = "dhcpc",      .min_args = 0u, .max_args = 3u,     .cmd_ptr = fapp_dhcp_cln_cmd,    .description = "Start DHCPv4 client", .syntax = "[-n <if name>] [release|autoip]"},
#endif
#if FAPP_CFG_DHCP_CMD && FNET_CFG_DHCP_SRV && FNET_CFG_IP4
    { .name = "dhcp",       .min_args = 0u, .max_args = 3u,     .cmd_ptr = fapp_dhcp_srv_cmd,    .description = "Start DHCPv4 server", .syntax = "[-n <if name>] [release|pool]"},
#endif
#if FAPP_CFG_AUTOIP_CMD && FNET_CFG_AUTOIP && FNET_CFG_IP4
    { .name = "autoip",     .min_args = 0u, .max_args = 3u,     .cmd_ptr = fapp_autoip_cmd,  .description = "Start Auto-IP service", .syntax = "[-n <if name>] [release]"},
#endif
#if FAPP_CFG_HTTP_CMD && FNET_CFG_HTTP
    { .name = "http",       .min_args = 0u, .max_args = 1u,     .cmd_ptr = fapp_http_cmd,    .description = "Start HTTP server", .syntax = "[release]"},
#endif
#if FAPP_CFG_HTTP_TLS_CMD && FNET_CFG_HTTP && FNET_CFG_HTTP_TLS && FNET_CFG_TLS
    { .name = "https",      .min_args = 0u, .max_args = 1u,     .cmd_ptr = fapp_http_tls_cmd,   .description = "Start HTTPS server", .syntax = "[release]"},
#endif
#if FAPP_CFG_EXP_CMD && FNET_CFG_FS
    { .name = "exp",        .min_args = 0u, .max_args = 1u,     .cmd_ptr = fapp_exp_cmd,     .description = "File Explorer submenu...", .syntax = ""},
#endif
#if FAPP_CFG_TFTPC_CMD
    { .name = "tftpc",      .min_args = 0u, .max_args = 3u,     .cmd_ptr = fapp_tftp_cln_cmd,    .description = "TFTP firmware loader", .syntax = "[<image name>[<server ip>[<type>]]]"},
#endif
#if FAPP_CFG_TFTPCUP_CMD
    { .name = "tftpcup",    .min_args = 0u, .max_args = 3u,     .cmd_ptr = fapp_tftp_cln_cmd,    .description = "TFTP firmware uploader", .syntax = "[<image name>[<server ip>[<type>]]]"},
#endif
#if FAPP_CFG_TFTP_CMD
    { .name = "tftp",       .min_args = 0u, .max_args = 1u,     .cmd_ptr = fapp_tftp_srv_cmd,   .description = "TFTP firmware server", .syntax = "[release]"},
#endif
#if FAPP_CFG_TELNET_CMD && FNET_CFG_TELNET
    { .name = "telnet",     .min_args = 0u, .max_args = 1u,     .cmd_ptr = fapp_telnet_cmd,  .description = "Start Telnet server", .syntax = "[release]"},
#endif
#if FAPP_CFG_DNS_CMD && FNET_CFG_DNS
    { .name = "dns",        .min_args = 1u, .max_args = 6u,     .cmd_ptr = fapp_dns_cmd,     .description = "Resolve IPv4|6 address of <host name>", .syntax = "[-n <if name>] [-s <server ip>] [4|6] <host name>"},
#endif
#if FAPP_CFG_LLMNR_CMD && FNET_CFG_LLMNR
    { .name = "llmnr",      .min_args = 0u, .max_args = 3u,     .cmd_ptr = fapp_llmnr_cmd,    .description = "Start LLMNR server", .syntax = "[-n <if name>] [release]"},
#endif
#if FAPP_CFG_MDNS_CMD && FNET_CFG_MDNS
    { .name = "mdns",       .min_args = 0u, .max_args = 3u,     .cmd_ptr = fapp_mdns_cmd,     .description = "Start MDNS server", .syntax = "[-n <if name>] [release]"},
#endif
#if FAPP_CFG_MDNS_CN_CMD && FNET_CFG_MDNS
    { .name = "mdnscn",     .min_args = 0u, .max_args = 2u,     .cmd_ptr = fapp_mdns_change_name_cmd,  .description = "Change MDNS service name, Bonjour test", .syntax = "[-n <if name>]"},
#endif
#if FAPP_CFG_SNTP_CMD && FNET_CFG_SNTP
    { .name = "sntp",       .min_args = 1u, .max_args = 1u,     .cmd_ptr = fapp_sntp_cmd,     .description = "Resolve time over SNTP", .syntax = "<server ip>"},
#endif
#if FAPP_CFG_MEM_CMD
    { .name = "mem",        .min_args = 0u, .max_args = 0u,     .cmd_ptr = fapp_mem_cmd,     .description = "Show memory map", .syntax = ""},
#endif
#if FAPP_CFG_ERASE_CMD
    { .name = "erase",      .min_args = 1u, .max_args = 2u,     .cmd_ptr = fapp_mem_erase_cmd,   .description = "Erase flash memory", .syntax = "all|[0x<erase address> <bytes>]"},
#endif
#if FAPP_CFG_SAVE_CMD && FNET_CFG_FLASH && FNET_CFG_CPU_FLASH
    { .name = "save",       .min_args = 0u, .max_args = 0u,     .cmd_ptr = fapp_save_cmd,    .description = "Save parameters to the FLASH", .syntax = ""},
#endif
#if FAPP_CFG_GO_CMD
    { .name = "go",         .min_args = 0u, .max_args = 1u,     .cmd_ptr = fapp_go_cmd,      .description = "Start application at address", .syntax = "[0x<address>]"},
#endif
#if FAPP_CFG_RESET_CMD
    { .name = "reset",      .min_args = 0u, .max_args = 0u,     .cmd_ptr = fapp_reset_cmd,   .description = "Reset the board", .syntax = ""},
#endif
#if FAPP_CFG_REBOOT_CMD /* Used only for IPv6 TAHI Testing.*/
    { .name = "reboot",     .min_args = 0u, .max_args = 0u,     .cmd_ptr = fapp_reset_cmd,   .description = "Reset the board", .syntax = ""},
#endif
#if FAPP_CFG_BENCH_CMD
#if FNET_CFG_BENCH_CLN
    { .name = "benchtx",    .min_args = 2u, .max_args = 10u,    .cmd_ptr = fapp_bench_cln_cmd, .description = "Benchmark transmitter/client", .syntax = "[-n <if name>] -a <remote ip> [tcp|udp] [-m <message size>] [-mn <number of messages>]"},
#endif
#if FNET_CFG_BENCH_SRV
    { .name = "benchrx",    .min_args = 0u, .max_args = 5u,     .cmd_ptr = fapp_bench_srv_cmd, .description = "Benchmark receiver/server", .syntax = "[[-n <if name>] [-a <if ip address>] [tcp|udp]] | release"},
#endif
#endif
#if FAPP_CFG_REINIT_CMD   /* Used to test FNET release/init only. */
    { "reinit",             .min_args = 0u, .max_args = 0u,     .cmd_ptr = fapp_reinit_cmd,  .description = "Reinit application", .syntax = ""},
#endif
#if FNET_CFG_CPU_WIFI
    { .name = "connect",    .min_args = 0u, .max_args = 2u,     .cmd_ptr = fapp_wifi_connect_cmd,  .description = "Connect to Wi-Fi access point", .syntax = "[<ssid> [<passphrase>]]"},
    { .name = "disconnect", .min_args = 0u, .max_args = 0u,     .cmd_ptr = fapp_wifi_disconnect_cmd,  .description = "Turn off the Wi-Fi interface radio", .syntax = ""},
    { .name = "ap",         .min_args = 0u, .max_args = 3u,     .cmd_ptr = fapp_wifi_ap_cmd,  .description = "Turn on Wi-Fi Access Point", .syntax = "[<ssid> [<passphrase> [wpa|wpa2]]]"},
#if FNET_CFG_CPU_WIFI_FW_UPDATE
    { .name = "fw",         .min_args = 0u, .max_args = 0u,     .cmd_ptr = fapp_wifi_fw_cmd,  .description = "Update Wi-Fi firmware", .syntax = ""},
#endif
#endif
#if FNET_CFG_PING && FAPP_CFG_PING6_CMD /*absolutly the same as the "ping" command */
    { .name = "ping6",      .min_args = 1u, .max_args = 14u,    .cmd_ptr = fapp_ping_cmd,   .description = "Send ICMP ECHO requests", .syntax = "[-c <count>][-i <seconds>]\n\r\t[-p <pattern>][-s <size>]\n\r\t[-h <hoplimit/ttl>] <ip>\t"}, /* -s -n should be ignored.*/
#endif
#if FAPP_CFG_DEBUG_CMD   /* Used for DEBUGING needs only. */
    { .name = "d",          .min_args = 0u, .max_args = 0u,     .cmd_ptr = fapp_debug_cmd,   .description = "Print debug info.", .syntax = ""},
#endif
#if FNET_CFG_PING && FAPP_CFG_PING_CMD
    { .name = "ping",       .min_args = 1u, .max_args = 14u,    .cmd_ptr = fapp_ping_cmd,   .description = "Send ICMP ECHO requests", .syntax = "[-c <count>][-i <seconds>]\n\r\t\t[-p <pattern>][-s <size>]\n\r\t\t[-h <hoplimit/ttl>] <ip>\t"}, /* -s -n should be ignored.*/
#endif
    { .name = 0}
};

/* Connection state string */
const fnet_char_t *const fapp_is_enabled_str[] =    {"off",  /* Disabled */
                                                     "on"    /* Enabled */
                                                    };

/* Shell command-line buffer.*/
static fnet_char_t fapp_cmd_line_buffer[FAPP_CFG_SHELL_MAX_LINE_LENGTH];

/************************************************************************
*     The main shell control data structure.
*************************************************************************/
static const struct fnet_shell fapp_shell =
{
    fapp_cmd_table,         /* cmd_table */
    FAPP_CFG_SHELL_PROMPT,  /* prompt_str */
    fapp_shell_on_init,        /* shell_init */
};

/************************************************************************
* DESCRIPTION: Initialize main command line shell.
************************************************************************/
fnet_shell_desc_t fapp_shell_init(void)
{
    struct fnet_shell_params    shell_params;

    /* Init main shell. */
    shell_params.shell = &fapp_shell;
    shell_params.cmd_line_buffer = fapp_cmd_line_buffer;
    shell_params.cmd_line_buffer_size = sizeof(fapp_cmd_line_buffer);
    shell_params.stream = FNET_SERIAL_STREAM_DEFAULT;
    shell_params.echo = FNET_TRUE;

    return fnet_shell_init(&shell_params);
}

/************************************************************************
* DESCRIPTION: Reset command reboots the system.
************************************************************************/
#if FAPP_CFG_RESET_CMD || FAPP_CFG_REBOOT_CMD
static void fapp_reset_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    FNET_COMP_UNUSED_ARG(desc);
    FNET_COMP_UNUSED_ARG(argc);
    FNET_COMP_UNUSED_ARG(argv);

    fnet_release(); /* Release FNET stack (it's optional).*/

    fnet_cpu_reset();
}
#endif

/************************************************************************
* DESCRIPTION: Print your DEBUG info.
************************************************************************/
#if FAPP_CFG_DEBUG_CMD
void fapp_debug_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    FNET_COMP_UNUSED_ARG(desc);
    FNET_COMP_UNUSED_ARG(argc);
    FNET_COMP_UNUSED_ARG(argv);

    /* Put here you debugging information.*/
#if 0
    fnet_fec_debug_mii_print_regs(fnet_netif_get_default());
#endif

#if 0
    {
        fnet_index_t    i = 0;
        fnet_ip6_addr_t addr_dns;
        fnet_uint8_t    ip_str[FNET_IP_ADDR_STR_SIZE] = {0};

        while(fnet_netif_get_ip6_dns(fnet_netif_get_default(), i, &addr_dns ) == FNET_TRUE)
        {
            fnet_inet_ntop(AF_INET6, (fnet_uint8_t *)(&addr_dns), ip_str, sizeof(ip_str));
            fnet_shell_println(desc, "DNS[%d]=%s", i, ip_str);
            i++;
        }
    }
#endif

#if 0 /* Test ctacrypt library.*/
    extern void ctaocrypt_test(void);

    ctaocrypt_test();
#endif

#if 0 /* test mempool */

    {
        static unsigned char mybuffer[3000];
        static fnet_mempool_desc_t fnet_mempool_main = 0; /* Main memory pool. */

        void *p1, *p2, *p3;
        fnet_mempool_main = fnet_mempool_init(mybuffer, 2048, FNET_MEMPOOL_ALIGN_16);
        fnet_printf("fnet_mempool_free_mem_status %d\n", fnet_mempool_free_mem_status(fnet_mempool_main));
        fnet_printf("fnet_mempool_malloc_max %d\n", fnet_mempool_malloc_max(fnet_mempool_main));
        p1 = fnet_mempool_malloc(fnet_mempool_main, 1600);
        fnet_printf("Malloc returned(1600) %x\n", p1);

        fnet_printf("fnet_mempool_free_mem_status %d\n", fnet_mempool_free_mem_status(fnet_mempool_main));
        fnet_printf("fnet_mempool_malloc_max %d\n", fnet_mempool_malloc_max(fnet_mempool_main));
        p2 = fnet_mempool_malloc(fnet_mempool_main, 368);
        fnet_printf("Malloc returned(368) %x\n", p2);

        fnet_printf("fnet_mempool_free_mem_status %d\n", fnet_mempool_free_mem_status(fnet_mempool_main));
        fnet_printf("fnet_mempool_malloc_max %d\n", fnet_mempool_malloc_max(fnet_mempool_main));
        p3 = fnet_mempool_malloc(fnet_mempool_main, 1);
        fnet_printf("Malloc returned(1) %x\n", p3);

        fnet_printf("fnet_mempool_free_mem_status %d\n", fnet_mempool_free_mem_status(fnet_mempool_main));
        fnet_printf("fnet_mempool_malloc_max %d\n", fnet_mempool_malloc_max(fnet_mempool_main));
        fnet_mempool_free(fnet_mempool_main, p1);
        fnet_printf("freed p1\n");

        fnet_printf("fnet_mempool_free_mem_status %d\n", fnet_mempool_free_mem_status(fnet_mempool_main));
        fnet_printf("fnet_mempool_malloc_max %d\n", fnet_mempool_malloc_max(fnet_mempool_main));
        fnet_mempool_free(fnet_mempool_main, p2);
        fnet_printf("freed p2\n");

        fnet_printf("fnet_mempool_free_mem_status %d\n", fnet_mempool_free_mem_status(fnet_mempool_main));
        fnet_printf("fnet_mempool_malloc_max %d\n", fnet_mempool_malloc_max(fnet_mempool_main));
        fnet_printf("Complete\n");
    }
#endif

}
#endif

/************************************************************************
* DESCRIPTION: Main Shell initial function.
************************************************************************/
void fapp_shell_on_init( fnet_shell_desc_t desc )
{
    fnet_shell_println(desc, "\n%s", FAPP_DELIMITER_STR);
    fnet_shell_println(desc, " %s", FAPP_CFG_NAME);

    /* FNET Header */
    fnet_shell_println(desc, FAPP_DELIMITER_STR);

    fnet_shell_println(desc, " %s for %s", FNET_DESCRIPTION, FNET_CPU_STR);
    fnet_shell_println(desc, " Version %s", FNET_VERSION);
    fnet_shell_println(desc, " Built %s by %s", FNET_BUILD_DATE, FNET_COMP_STR);
#if 0 /* Optional information */
    fnet_shell_println(desc, " %s", FNET_COPYRIGHT);
    fnet_shell_println(desc, " %s", FNET_LICENSE);
#endif
    fnet_shell_println(desc, "\n Enter '?' for command list.");
    fnet_shell_println(desc, "%s", FAPP_DELIMITER_STR);
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
#if FAPP_CFG_GO_CMD
void fapp_go_cmd ( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_uint32_t   address;
    fnet_char_t    *p;

    if (argc == 2u)
    {
        address = fnet_strtoul(argv[1], &p, 16u);

        if (p == argv[1])
        {
            fnet_shell_println(desc, FAPP_PARAM_ERR, argv[1] );
            return;
        }
    }
    else
    {
        address = fapp_params_boot_config.go_address;
    }

    fapp_go(desc, address);
}
#endif

/************************************************************************
* DESCRIPTION:   Jump to application entry point.
************************************************************************/
#if FAPP_CFG_GO_CMD || FAPP_CFG_BOOTLOADER || FAPP_CFG_SETGET_CMD_BOOT
void fapp_go ( fnet_shell_desc_t desc, fnet_uint32_t address )
{
    if((fapp_params_boot_config.go_address == 0u) || (*(fnet_uint32_t *)address == 0xffffffffu))
    {
        fnet_printf("\nThere is no code on user application startup vector.\n");
    }
    else
    {

        fnet_shell_println(desc, FAPP_GO_STR, address);

        fnet_release(); /* Release the FNET stack.*/

        (( void(*)(void) )FNET_CPU_ADDR_TO_INSTRUCTION(address))(); /* Jump. */
    }
}
#endif

/************************************************************************
* DESCRIPTION: Save environment variables to persistent storage.
************************************************************************/
#if FAPP_CFG_SAVE_CMD && FNET_CFG_FLASH && FNET_CFG_CPU_FLASH
static void fapp_save_cmd ( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    (void)argc;
    (void)argv;

    if(fapp_params_to_flash() == FNET_OK)
    {
        fnet_shell_println(desc, FAPP_SAVE_STR);
    }
    else
    {
        fnet_shell_script_stop(desc);   /* Critical error. Clear script. */
        fnet_shell_println(desc, FAPP_SAVE_FAILED_STR);
    }
}
#endif

/************************************************************************
* DESCRIPTION: Display command help.
************************************************************************/
void fapp_help_cmd ( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    (void)argc;
    (void)argv;

    fnet_shell_help(desc);
}

/************************************************************************
* DESCRIPTION: Used to test FNET release/init only.
************************************************************************/
#if FAPP_CFG_REINIT_CMD
void fapp_reinit_cmd ( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    (void)argc;
    (void)argv;

    fapp_release(desc); /* FAPP release. */

    fapp_init();        /* FAPP reinit.*/
}
#endif

/************************************************************************
* DESCRIPTION: Binds IPv6 address to the default interface.
************************************************************************/
#if FAPP_CFG_BIND_CMD && FNET_CFG_IP6
static void fapp_bind_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_netif_desc_t   netif = fnet_netif_get_default();
    fnet_ip6_addr_t     addr;
    fnet_index_t        i;

    /* Last argument must be IPv6 address.*/
    i = (argc - 1u);
    if(fnet_inet_pton(AF_INET6, argv[i], &addr, sizeof(addr)) == FNET_ERR)
    {
        goto ERROR_PARAMETER;
    }
    else
    {
        if(argc > 2u) /* There are additional parameters */
        {
            for(i = 1u; i < (argc - 1)/*avoid the last parameter.*/ ; i++)
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
        }
    }

    /* Bind Ipv6 address.*/
    if(fnet_netif_bind_ip6_addr(netif, &addr, FNET_NETIF_IP_ADDR_TYPE_MANUAL) == FNET_ERR)
    {
        fnet_shell_println(desc, "Failed!");
    }
    return;

ERROR_PARAMETER:
    fnet_shell_println(desc, FAPP_PARAM_ERR, argv[i]);
    return;
}
#endif

/************************************************************************
* DESCRIPTION: Unbinds IPv6 address from the default interface.
************************************************************************/
#if FAPP_CFG_UNBIND_CMD && FNET_CFG_IP6
static void fapp_unbind_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_netif_desc_t   netif = fnet_netif_get_default();
    fnet_ip6_addr_t     addr;
    fnet_index_t        i;

    /* Last argument must be IPv6 address.*/
    i = (argc - 1u);
    if(fnet_inet_pton(AF_INET6, argv[i], &addr, sizeof(addr)) == FNET_ERR)
    {
        goto ERROR_PARAMETER;
    }
    else
    {
        if(argc > 2u) /* There are additional parameters */
        {
            for(i = 1u; i < (argc - 1)/*avoid the last parameter.*/ ; i++)
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
        }
    }

    /* Unbind Ipv6 address.*/
    if(fnet_netif_unbind_ip6_addr(netif, &addr) == FNET_ERR)
    {
        fnet_shell_println(desc, "Failed!");
    }
    return;

ERROR_PARAMETER:
    fnet_shell_println(desc, FAPP_PARAM_ERR, argv[i]);
    return;
}
#endif

/************************************************************************
* DESCRIPTION: Start file explorer shell.
************************************************************************/
#if FAPP_CFG_EXP_CMD && FNET_CFG_FS
static void fapp_exp_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    FNET_COMP_UNUSED_ARG(argc);
    FNET_COMP_UNUSED_ARG(argv);

    fnet_shell_switch(desc, &fapp_fs_shell);
}
#endif
