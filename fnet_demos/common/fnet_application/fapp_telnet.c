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
* @file fapp_telnet.c
*
* @author Andrey Butok
*
* @brief FNET Shell Demo implementation.
*
***************************************************************************/

#include "fapp.h"
#include "fapp_prv.h"
#include "fapp_telnet.h"
#include "fapp_mem.h"

#if FAPP_CFG_SETGET_CMD
    #include "fapp_setget.h"
#endif
#if FAPP_CFG_DHCP_CMD

    #include "fapp_dhcp.h"

#endif
#if FAPP_CFG_HTTP_CMD || FAPP_CFG_EXP_CMD

    #include "fapp_http.h"
    #include "fapp_fs.h"

#endif

#if FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD

    #include "fapp_tftp.h"

#endif

#if FAPP_CFG_TELNET_CMD && FNET_CFG_TELNET

#define FAPP_TELNET_PROMPT_STR     FAPP_CFG_SHELL_PROMPT

#if FAPP_CFG_TELNET_TEST_CMD
    void fapp_telnet_test_cmd( fnet_shell_desc_t desc );
#endif



/************************************************************************
*     The main shell control data structure.
*************************************************************************/
const struct fnet_shell fapp_telnet_shell =
{
    fapp_cmd_table, /* You may replace by your own command table.*/
    FAPP_TELNET_PROMPT_STR,     /* prompt_str */
    fapp_shell_init,            /* shell_init */
};


static fnet_telnet_desc_t fapp_telnet_desc = 0; /* Telnet descriptor. */


/************************************************************************
* NAME: fapp_telnet_release
*
* DESCRIPTION: Releases TELNET server.
*************************************************************************/
void fapp_telnet_release(void)
{
    fnet_telnet_release(fapp_telnet_desc);
    fapp_telnet_desc = 0;
}

/************************************************************************
* NAME: fapp_telnet_cmd
*
* DESCRIPTION: Run Telnet server.
*************************************************************************/
void fapp_telnet_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    struct fnet_telnet_params   params;
    fnet_telnet_desc_t          telnet_desc;

    if(argc == 1u) /* By default is "init".*/
    {
        fnet_memset_zero(&params, sizeof(struct fnet_telnet_params));
        params.shell = &fapp_telnet_shell;

        /* Init Telnet server */
        telnet_desc = fnet_telnet_init(&params);
        if(telnet_desc != FNET_ERR)
        {
            fnet_shell_println(desc, FAPP_DELIMITER_STR);
            fnet_shell_println(desc, " Telnet Server started.");
            fapp_print_netif_addr(desc, AF_SUPPORTED, fnet_netif_get_default(), FNET_FALSE);
            fnet_shell_println(desc, FAPP_DELIMITER_STR);

            fapp_telnet_desc = telnet_desc;
        }
        else
        {
            fnet_shell_println(desc, FAPP_INIT_ERR, "Telnet");
        }

    }
    else if((argc == 2u) && (fnet_strcasecmp(&FAPP_COMMAND_RELEASE[0], argv[1]) == 0)) /* [release] */
    {
        fapp_telnet_release();
    }
    else
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, argv[1]);
    }
}

/************************************************************************
* NAME: fapp_telnet_exit_cmd
*
* DESCRIPTION:
************************************************************************/
#if 0
static void fapp_telnet_exit_cmd ( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    FNET_COMP_UNUSED_ARG(desc);
    FNET_COMP_UNUSED_ARG(argc);
    FNET_COMP_UNUSED_ARG(argv);

    fnet_telnet_close_session(fapp_telnet_desc);
}
#endif

/************************************************************************
* NAME: fapp_telnet_info
*
* DESCRIPTION:
*************************************************************************/
void fapp_telnet_info(fnet_shell_desc_t desc)
{
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "TELNET Server", fapp_enabled_str[fnet_telnet_enabled(fapp_telnet_desc)]);
}

#if FAPP_CFG_TELNET_TEST_CMD
/************************************************************************
* NAME: fapp_telnet_test_cmd
*
* DESCRIPTION: "test" command. Used to test Telnet sending.
* For debug needs only
************************************************************************/
void fapp_telnet_test_cmd( fnet_shell_desc_t desc )
{
    while(1)
    {
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "Free Heap", fnet_free_mem_status());
        fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_D, "MAX Heap", fnet_malloc_max());
    }
}
#endif

#endif











