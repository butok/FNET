/**************************************************************************
*
* Copyright 2011-2017 by Andrey Butok. FNET Community.
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
***************************************************************************
*
*  FNET Shell Demo implementation.
*
***************************************************************************/

#include "fapp.h"

#include "fapp_prv.h"

#include "fapp_mem.h"

#include "fapp_netif.h"

#if ((FAPP_CFG_HTTP_CMD || FAPP_CFG_HTTP_TLS_CMD) && FNET_CFG_HTTP)|| (FAPP_CFG_EXP_CMD && FNET_CFG_FS)
    #include "fapp_fs.h"
#endif

#if FAPP_CFG_FREERTOS
/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#endif

/************************************************************************
*     Definitions.
*************************************************************************/

#define FAPP_NET_ERR            "Error: Network Interface is not configurated!"
#define FAPP_GO_STR             "go 0x%08X"
#define FAPP_BOOT_STR           "Press any key to stop (%s): %3d"
#define FAPP_PARAMS_LOAD_STR    "[FAPP] Application parameters loaded from Flash."
#define FAPP_DUP_IP_WARN        "\n[FAPP] %s: %s has IP address conflict with another system on the network!\n"

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void fapp_init(void);
static void fapp_poll(void);

#if FAPP_CFG_BOOTLOADER || FAPP_CFG_SETGET_CMD_BOOT
    static void fapp_boot_mode_go(fnet_shell_desc_t desc);
    static void fapp_boot_mode_script(fnet_shell_desc_t desc);
#endif
static void fapp_boot(fnet_shell_desc_t desc);
#if FNET_CFG_IP4
    static void fapp_dup_ip_callback( fnet_netif_desc_t netif );
#endif

static fnet_shell_desc_t fapp_shell_desc = 0; /* Shell descriptor. */

#if FAPP_CFG_FREERTOS
/* RX activity event. */
#define FAPP_FREERTOS_EVENT_RX  (0x1)  
static EventGroupHandle_t fapp_freertos_event_group;
#endif

/******************************************************************************
 *  Testing of mutex nesting conflict.
 *  Used only for testing needs.
 ******************************************************************************/
#if FNET_CFG_MULTITHREADING && FAPP_CFG_TEST_MUTEX_CONFLICT
static  fnet_return_t fapp_test_mutex_init( fnet_mutex_t *counter);
static void fapp_test_mutex_free( fnet_mutex_t *counter );
static void fapp_test_mutex_lock( fnet_mutex_t *counter );
static void fapp_test_mutex_unlock( fnet_mutex_t *counter );
const fnet_mutex_api_t fapp_test_mutex_api = {.mutex_init = fapp_test_mutex_init,
                                              .mutex_free = fapp_test_mutex_free,
                                              .mutex_lock = fapp_test_mutex_lock,
                                              .mutex_unlock = fapp_test_mutex_unlock
                                             };

#endif

#if FAPP_CFG_BOOTLOADER || FAPP_CFG_SETGET_CMD_BOOT

/* Boot-mode string matching. */
#define FAPP_PARAMS_BOOT_MODE_STOP_STR      "stop"   /* Stop at the shell prompt */
#define FAPP_PARAMS_BOOT_MODE_GO_STR        "go"     /* Boot from flash.*/
#define FAPP_PARAMS_BOOT_MODE_SCRIPT_STR    "script" /* Start boot-script.*/

/* Boot modes. */
static const struct boot_mode boot_modes[] =
{
    {FAPP_PARAMS_BOOT_MODE_STOP,    FAPP_PARAMS_BOOT_MODE_STOP_STR, 0},
    {FAPP_PARAMS_BOOT_MODE_GO,      FAPP_PARAMS_BOOT_MODE_GO_STR, fapp_boot_mode_go},
    {FAPP_PARAMS_BOOT_MODE_SCRIPT,  FAPP_PARAMS_BOOT_MODE_SCRIPT_STR, fapp_boot_mode_script}
};

#define BOOT_MODE_COUNT     (sizeof(boot_modes)/sizeof(struct boot_mode))
#define BOOT_MODE_DEFAULT   boot_modes[0]


/************************************************************************
* DESCRIPTION:
************************************************************************/
const struct boot_mode *fapp_boot_mode_by_index (fapp_params_boot_mode_t index)
{
    const struct boot_mode *result = &BOOT_MODE_DEFAULT;
    fnet_index_t            i;

    for(i = 0u; i < BOOT_MODE_COUNT; i++)
    {
        if( boot_modes[i].index == index )
        {
            result = &boot_modes[i];
            break;
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
const struct boot_mode *fapp_boot_mode_by_name (fnet_char_t *name)
{
    const struct boot_mode *result = 0;
    fnet_index_t            i;

    for(i = 0u; i < BOOT_MODE_COUNT; i++)
    {
        if(fnet_strcmp(boot_modes[i].name, name ) == 0)
        {
            result = &boot_modes[i];
            break;
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
static void fapp_boot_mode_go(fnet_shell_desc_t desc)
{
    fapp_go (desc, fapp_params_boot_config.go_address);
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
static void fapp_boot_mode_script(fnet_shell_desc_t desc)
{
    fnet_shell_println(desc, "\n%s", fapp_params_boot_config.script);
    if(fnet_shell_script(desc, fapp_params_boot_config.script) == FNET_ERR)
    {
        fnet_shell_println(desc, "Script error!");
    }
}
#endif /* FAPP_CFG_BOOTLOADER */

/************************************************************************
* DESCRIPTION:
************************************************************************/
static void fapp_boot(fnet_shell_desc_t desc)
{
    /* Bootloader. */
#if FAPP_CFG_BOOTLOADER

    /* The bootloader wait some time for a key over a serial connection.*/
    /* The wait time is given by the boot_delay parameter.*/
    fnet_time_t delay;

    const struct boot_mode *mode;

    mode = fapp_boot_mode_by_index (fapp_params_boot_config.mode);


    if(mode->handler)
    {
        delay = fapp_params_boot_config.delay;

        fnet_shell_printf(desc, FAPP_BOOT_STR, mode->name, delay);

        while(delay > 0u)
        {
            delay--;
            if( fnet_shell_getchar(desc) != FNET_ERR)
            {
                break;
            }

            fnet_timer_delay(FNET_TIMER_TICKS_IN_SEC); /* 1 sec. delay. */
            fnet_shell_printf(desc, "\b\b\b\b %3d", delay);
        }

        fnet_shell_println(desc, "");

        if(delay == 0u)
        {
            /* Auto-start*/
            mode->handler(desc);
        }
    }
    else
#endif
    {
        /* Default startup script. */
        if(fnet_strlen(FAPP_CFG_STARTUP_SCRIPT))
        {
            fnet_shell_println(desc, "Startup script: %s", FAPP_CFG_STARTUP_SCRIPT);
            if(fnet_shell_script(desc, FAPP_CFG_STARTUP_SCRIPT) == FNET_ERR)
            {
                fnet_shell_println(desc, "Script error!");
            }
        }
    }

    FNET_COMP_UNUSED_ARG(desc);
}

/************************************************************************
* DESCRIPTION: IP address is duplicated.
************************************************************************/
#if FNET_CFG_IP4
static void fapp_dup_ip_callback( fnet_netif_desc_t netif )
{
    fnet_char_t     name[FNET_NETIF_NAMELEN];
    fnet_char_t     ip_str[FNET_IP4_ADDR_STR_SIZE];
    fnet_ip4_addr_t addr;

    fnet_netif_get_name( netif, name, sizeof(name) );
    addr = fnet_netif_get_ip4_addr( netif );
    fnet_inet_ntoa(*(struct fnet_in_addr *)( &addr), ip_str);

    fnet_printf(FAPP_DUP_IP_WARN, name, ip_str);
}
#endif /* FNET_CFG_IP4 */

/************************************************************************
* DESCRIPTION: FNET Application initialization.
************************************************************************/
static void fapp_init(void)
{
    static fnet_uint8_t         stack_heap[FAPP_CFG_HEAP_SIZE];
    struct fnet_init_params     init_params;

    /* Input parameters for FNET stack initialization */
    fnet_memset_zero(&init_params, sizeof(init_params));
    init_params.netheap_ptr = stack_heap;
    init_params.netheap_size = sizeof(stack_heap);
#if FNET_CFG_MULTITHREADING && FAPP_CFG_TEST_MUTEX_CONFLICT
    init_params.mutex_api = &fapp_test_mutex_api;
#endif

    /* Init FNET stack */
    if(fnet_init(&init_params) == FNET_OK)
    {
        /* Add event handler on duplicated IP address */
    #if FNET_CFG_IP4
        fnet_netif_set_callback_on_ip4_addr_conflict(fapp_dup_ip_callback);
    #endif

        /* Init FS and mount FS Image. */
    #if (FAPP_CFG_EXP_CMD && FNET_CFG_FS) || ((FAPP_CFG_HTTP_CMD || FAPP_CFG_HTTP_TLS_CMD) && FNET_CFG_HTTP)
        fapp_fs_mount(); 
    #endif
    
        /* Init main shell. */
        fapp_shell_desc = fapp_shell_init();
        if(fapp_shell_desc)
        {
            /* Initialize network interfaces.*/
            if(fapp_netif_init(fapp_shell_desc) == FNET_OK)
            {
            #if FAPP_CFG_PARAMS_READ_FLASH && FNET_CFG_CPU_FLASH
                /* During bootup, the most recently stored customer configuration data will be read and used to configure the interfaces.*/
                if(fapp_params_from_flash() == FNET_OK)
                {
                    fnet_shell_println(fapp_shell_desc, FAPP_PARAMS_LOAD_STR);
                }
            #endif
        
                /* Check if we have atleast one initoalized networking interface.*/
                if(fnet_netif_get_default() == FNET_NULL)
                {
                    fnet_shell_println(fapp_shell_desc, FAPP_NET_ERR);
                }
    
                /* Start application */
                fapp_boot(fapp_shell_desc);
            }
            else
            {
                fnet_shell_println(fapp_shell_desc, FAPP_INIT_ERR, "NETIF");
            }
        }
        else
        {
            fnet_println(FAPP_INIT_ERR, "Shell");
        }
    }
    else
    {
        fnet_println(FAPP_INIT_ERR, "FNET");
    }
}

/************************************************************************
* DESCRIPTION: FNET Application release.
************************************************************************/
#if FAPP_CFG_REINIT_CMD
static void fapp_release(fnet_shell_desc_t desc)
{

#if FAPP_CFG_MDNS_CMD && FNET_CFG_MDNS          /* Release mDNS server. */
    fapp_mdns_release();
#endif

#if FAPP_CFG_LLMNR_CMD && FNET_CFG_LLMNR        /* Release LLMNR server. */
    fapp_llmnr_release();
#endif

#if FAPP_CFG_DHCPC_CMD && FNET_CFG_DHCP_CLN && FNET_CFG_IP4
    fapp_dhcp_cln_release();                    /* Release DHCP client. */
#endif

#if FAPP_CFG_AUTOIP_CMD && FNET_CFG_AUTOIP && FNET_CFG_IP4
    fapp_autoip_release();                      /* Release Auto-IP service. */
#endif

#if FAPP_CFG_TELNET_CMD && FNET_CFG_TELNET      /* Release TELNET server. */
    fapp_telnet_release();
#endif

#if FAPP_CFG_HTTP_CMD && FNET_CFG_HTTP          /* Release HTTP server. */
    fapp_http_release();
#endif

#if FAPP_CFG_HTTP_TLS_CMD && FNET_CFG_HTTP && FNET_CFG_HTTP_TLS && FNET_CFG_TLS /* Release HTTPS server. */
    fapp_http_tls_release();
#endif

#if FAPP_CFG_TFTP_CMD                          /* Release TFTP server. */
    fapp_tftp_srv_release();
#endif

#if FAPP_CFG_DHCP_CMD && FNET_CFG_DHCP_SRV && FNET_CFG_IP4
    fapp_dhcp_srv_release();                    /* Release DHCP server. */
#endif

#if (FAPP_CFG_EXP_CMD && FNET_CFG_FS) || ((FAPP_CFG_HTTP_CMD || FAPP_CFG_HTTP_TLS_CMD) && FNET_CFG_HTTP)
    fapp_fs_unmount();                          /* Unmount and release FS. */
#endif

    fnet_shell_release(desc);                   /* Release shell. */

    fnet_release();                             /* Release the FNET stack.*/
}
#endif /* FAPP_CFG_REINIT_CMD */


/************************************************************************
* DESCRIPTION: Application poll.
************************************************************************/
static void fapp_poll(void)
{
    #if !FNET_CFG_TIMER_POLL_AUTOMATIC
        fnet_timer_poll(); /* Poll FNET stack timeouts.*/
    #endif
        fnet_service_poll(); /* Poll registered services.*/

    #if FAPP_CFG_FREERTOS /* FrerRTOS task sleep */
        {
        #if 0 /* Sleep for some time. Simple version. */
            vTaskDelay(FAPP_CFG_FREERTOS_TASK_POLL_PERIOD/portTICK_PERIOD_MS);
        #else /* Sleep for some time or for the FNET RX event */
            /* Wait a maximum of FAPP_CFG_FREERTOS_TASK_POLL_PERIOD_MS ms for either any bit to be set within
                the event group.  Clear the bits before exiting. */
            xEventGroupWaitBits(
                        fapp_freertos_event_group,   /* The event group being tested. */
                        FAPP_FREERTOS_EVENT_RX, /* The bits within the event group to wait for. */
                        pdTRUE,        /* BIT_0 & BIT_4 should be cleared before returning. */
                        pdFALSE,       /* Don't wait for both bits, either bit will do. */
                        FAPP_CFG_FREERTOS_TASK_POLL_PERIOD_MS/portTICK_PERIOD_MS );/* Ticks to wait for either bit to be set. */

            /* Returned because Socket RX event or ticks passed.*/
        #endif
        }
    #endif
}

/************************************************************************
* DESCRIPTION: Bare-metal main() of the shell demo.
************************************************************************/
void fapp_main(void)
{
    /* Initilize FNET Demo application */
    fapp_init();
    
    while(1)
    {
        /* Application poll */
        fapp_poll();
    }
}

#if FAPP_CFG_FREERTOS

/************************************************************************
* DESCRIPTION: Socket RX callback.
************************************************************************/
static void fapp_socket_rx_callback(void)
{
    BaseType_t xHigherPriorityTaskWoken;

    if(xEventGroupSetBitsFromISR(fapp_freertos_event_group, FAPP_FREERTOS_EVENT_RX, &xHigherPriorityTaskWoken)!= pdFAIL)
    {
        /* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
        switch should be requested.  The macro used is port specific and will
        be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
        the documentation page for the port being used. */
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
}

/************************************************************************
* DESCRIPTION: FreeRTOS task.
************************************************************************/
static void fapp_task(void *params)
{
	/* FNET demo application */
    fapp_main();
}

/************************************************************************
* DESCRIPTION: Main entry point of the FreeRTOS shell demo.
************************************************************************/
void fapp_main_freertos( void ) 
{
    /* Create FNET demo application task */
    if(xTaskCreate(
            fapp_task,  /* pointer to the task */
            FAPP_CFG_NAME, /* task name for kernel awareness debugging */
            FAPP_CFG_FREERTOS_TASK_STACK_SIZE/sizeof(portSTACK_TYPE), /* task stack size */
            NULL, /* optional task startup argument */
            FAPP_CFG_FREERTOS_TASK_PRIORITY,  /* initial priority */
            NULL /* optional task handle to create */
        ) != pdPASS)
    {
        fnet_println("[FREERTOS] Failed to create task."); /* Probably out of memory */
    }
    else
    {
        /* Attempt to create the event group. */
        fapp_freertos_event_group = xEventGroupCreate();

        /* Was the event group created successfully? */
        if( fapp_freertos_event_group == NULL )
        {
            /* The event group was not created because there was insufficient heap available. */
            fnet_println("[FREERTOS] Failed to create event group.");
        }
        else
        {
            /* Registers the "socket layer activity" event handler.*/
            fnet_socket_set_callback_on_rx(fapp_socket_rx_callback);

            fnet_println("[FREERTOS] Starting scheduler.");
            vTaskStartScheduler();

            /* The code should never reach here. */ 
        }
    }
}
#endif /* FAPP_CFG_FREERTOS */

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
* DESCRIPTION: Print information on new IP from Auto-IP or DHCP service.
************************************************************************/
#if FNET_CFG_IP4 &&((FAPP_CFG_DHCPC_CMD && FNET_CFG_DHCP_CLN) || FNET_CFG_AUTOIP)
void fapp_addr_callback_updated(fnet_shell_desc_t desc, fnet_netif_desc_t netif)
{
    fnet_shell_unblock(desc); /* Unblock the shell. */

    /* Print updated parameters info. */
    fnet_shell_println( desc, "\n%s", FAPP_DELIMITER_STR);
    fnet_shell_println( desc, FAPP_UPDATED_IP_STR);
    fnet_shell_println( desc, FAPP_DELIMITER_STR);

    fapp_netif_info_print( desc, netif );
}
#endif

/******************************************************************************
 *  Testing of mutex nesting conflict.
 *  Used only for testing needs.
 ******************************************************************************/
#if FNET_CFG_MULTITHREADING && FAPP_CFG_TEST_MUTEX_CONFLICT

/* Create a new mutex */
static  fnet_return_t fapp_test_mutex_init( fnet_mutex_t *counter)
{
    *counter = 0;
    return FNET_OK;
}

/* Delete a mutex. */
static void fapp_test_mutex_free( fnet_mutex_t *counter )
{
    if(*counter != 0) /* Is locked */
    {
        fnet_println("Wrong mutex value: %d", *counter);
        while(1) /* Stop */
        {};
    }
}

/* Lock a mutex. */
static void fapp_test_mutex_lock( fnet_mutex_t *counter )
{
    if(*counter != 0) /* Is locked */
    {
        fnet_println("Wrong mutex value: %d", *counter);
        while(1) /* Stop */
        {};
    }
    *counter = (fnet_mutex_t)1; /* Lock */
}

/* Unlock a mutex. */
static void fapp_test_mutex_unlock( fnet_mutex_t *counter )
{
    if(*counter != (fnet_mutex_t)1) /* Is not locked */
    {
        fnet_println("Wrong mutex value: %d", *counter);
        while(1) /* Stop */
        {};
    }
    *counter = 0; /* Unlock */
}
#endif /* FNET_CFG_MULTITHREADING && FAPP_CFG_TEST_MUTEX_CONFLICT */
