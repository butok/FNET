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
*  FNET Shell Demo implementation.
*
***************************************************************************/

#include "fapp.h"
#include "fapp_prv.h"
#include "fapp_mem.h"
#include "fapp_params.h"

#if FNET_CFG_FLASH && FNET_CFG_CPU_FLASH
    #define fapp_params_erase   fnet_flash_erase
    #define fapp_params_memcpy  fnet_flash_memcpy
    #define fapp_params_flush   fnet_flash_flush
#endif

/* Default parameter values.
* One-time setup done as a part of the manufacturing process/flashing
* Default application parameters (in ROM) set during flashing.
*/
#if FAPP_CFG_PARAMS_REWRITE_FLASH

#if FNET_CFG_COMP_CW
    #if FNET_MK
        #pragma define_section fapp_params ".fapp_params" ".fapp_params" ".fapp_params" far_abs RW
        __declspec(fapp_params)
    #else
        #pragma define_section fapp_params ".fapp_params" far_abs RW
        __declspec(fapp_params)
    #endif
#endif
#if FNET_CFG_COMP_IAR
    #pragma segment="fapp_params"
    #pragma location="fapp_params"
    #if FNET_MK
        __root
    #endif
#endif
#if FNET_CFG_COMP_UV || FNET_CFG_COMP_GNUC || FNET_CFG_COMP_GHS
    __attribute__((section(".fapp_params")))
#endif
static const struct fapp_params_flash fapp_params_config
#if FNET_CFG_COMP_UV || FNET_CFG_COMP_GNUC
    __attribute__((used))
#endif
    =
{
    .signature = FAPP_PARAMS_SIGNATURE,
    .fnet_params =
    {
        .netif_name = FAPP_CFG_PARAMS_NETIF_NAME, /* Default interface */
        .address = FAPP_CFG_PARAMS_IP_ADDR,
        .netmask = FAPP_CFG_PARAMS_IP_MASK,
        .gateway = FAPP_CFG_PARAMS_IP_GW,
        .dns = FAPP_CFG_PARAMS_IP_DNS,
        .mac = FAPP_CFG_PARAMS_MAC_ADDR,
        .host_name = FAPP_CFG_PARAMS_HOST_NAME
    },
    .boot_params =
    {
        .mode = FAPP_CFG_PARAMS_BOOT_MODE,
        .delay = FAPP_CFG_PARAMS_BOOT_DELAY,
        .go_address = FAPP_CFG_PARAMS_BOOT_GO_ADDRESS,
        .script = FAPP_CFG_PARAMS_BOOT_SCRIPT
    },
    .tftp_params =
    {
        .server_addr = FAPP_CFG_PARAMS_TFTP_SERVER,
        .file_type = FAPP_CFG_PARAMS_TFTP_FILE_TYPE,
        .file_raw_address = FAPP_CFG_PARAMS_TFTP_FILE_RAW_ADDRESS,
        .file_name = FAPP_CFG_PARAMS_TFTP_FILE_NAME
    },
    .wifi_params =
    {
        .ssid = FAPP_CFG_PARAMS_WIFI_SSID,
        .passphrase = FAPP_CFG_PARAMS_WIFI_WPA_PASSPHRASE
    }
};
#endif /* FAPP_CFG_PARAMS_REWRITE_FLASH */


/* Local confiruration parameters.
* Will be overwritten by parameters from flash if FAPP_CFG_PARAMS_READ_FLASH set to 1.
*/
fnet_char_t fapp_params_host_name[FAPP_PARAMS_HOST_NAME_SIZE] = FAPP_CFG_PARAMS_HOST_NAME;

#if FAPP_CFG_PARAMS_BOOT
struct fapp_params_boot fapp_params_boot_config =
{
    .mode = FAPP_CFG_PARAMS_BOOT_MODE,
    .delay = FAPP_CFG_PARAMS_BOOT_DELAY,
    .go_address = FAPP_CFG_PARAMS_BOOT_GO_ADDRESS,
    .script = FAPP_CFG_PARAMS_BOOT_SCRIPT
};
#endif

#if FAPP_CFG_PARAMS_TFTP
struct fapp_params_tftp fapp_params_tftp_config =
{
    .server_addr = FAPP_CFG_PARAMS_TFTP_SERVER,
    .file_type = FAPP_CFG_PARAMS_TFTP_FILE_TYPE,
    .file_raw_address = FAPP_CFG_PARAMS_TFTP_FILE_RAW_ADDRESS,
    .file_name = FAPP_CFG_PARAMS_TFTP_FILE_NAME
};
#endif

#if FAPP_CFG_PARAMS_WIFI
struct fapp_params_wifi fapp_params_wifi_config =
{
    .ssid = FAPP_CFG_PARAMS_WIFI_SSID,
    .passphrase = FAPP_CFG_PARAMS_WIFI_WPA_PASSPHRASE
};
#endif

/************************************************************************
* DESCRIPTION: Save current configuration parameters to the flash.
************************************************************************/
#if FAPP_CFG_SAVE_CMD && FNET_CFG_FLASH && FNET_CFG_CPU_FLASH
fnet_return_t fapp_params_to_flash(void)
{
    struct fapp_params_flash    *fapp_params_flash_p = (struct fapp_params_flash *)FAPP_CFG_FLASH_PARAMS_ADDRESS;
    fnet_netif_desc_t           netif = fnet_netif_get_default();
    struct fapp_params_flash    fapp_params_ram = {{0}}; /* Tmp buffer */

    /**** Prepare fapp_params_ram, which will be written to Flash.****/
#if FNET_CFG_IP4
    /* Save IP address only if it was allocated manually/statically. */
    if(fnet_netif_get_ip4_addr_type(netif) == FNET_NETIF_IP_ADDR_TYPE_MANUAL)
    {
        fapp_params_ram.fnet_params.address = fnet_netif_get_ip4_addr(netif);    /* Take the current value. */
    }
    else
    {
        fapp_params_ram.fnet_params.address = fapp_params_flash_p->fnet_params.address; /* Preserve the old value.*/
    }

    fapp_params_ram.fnet_params.netmask = fnet_netif_get_ip4_subnet_mask(netif);
    fapp_params_ram.fnet_params.gateway = fnet_netif_get_ip4_gateway(netif);
#if FNET_CFG_DNS
    fapp_params_ram.fnet_params.dns = fnet_netif_get_ip4_dns(netif);
#endif
#else
    /* Preserve the old value.*/
    fapp_params_ram.fnet_params = fapp_params_flash_p->fnet_params;
#endif

    fnet_netif_get_hw_addr(netif, fapp_params_ram.fnet_params.mac, sizeof(fnet_mac_addr_t));
    fnet_netif_get_name(netif, fapp_params_ram.fnet_params.netif_name, sizeof(fapp_params_ram.fnet_params.netif_name));

    /* Host name.*/
    fnet_strncpy(fapp_params_ram.fnet_params.host_name, fapp_params_host_name, FAPP_PARAMS_HOST_NAME_SIZE);

    /* Write Signature.*/
    fnet_memcpy( fapp_params_ram.signature, FAPP_PARAMS_SIGNATURE, sizeof(FAPP_PARAMS_SIGNATURE));

    /* BOOT parameters */
#if FAPP_CFG_PARAMS_BOOT
    fnet_memcpy( &fapp_params_ram.boot_params, &fapp_params_boot_config, sizeof(struct fapp_params_boot)  );
#endif
    /* TFTP parameters*/
#if FAPP_CFG_PARAMS_TFTP
    fnet_memcpy(&fapp_params_ram.tftp_params, &fapp_params_tftp_config, sizeof(struct fapp_params_tftp)  );
#endif
    /* Wi-Fi parameters*/
#if FAPP_CFG_PARAMS_WIFI
    fnet_memcpy(&fapp_params_ram.wifi_params, &fapp_params_wifi_config, sizeof(struct fapp_params_wifi)  );
#endif

    /**** Erase one paage allocated for configuration parameters.****/
    fapp_params_erase( (void *)(fapp_params_flash_p), sizeof(struct fapp_params_flash));

    /* Simple check if erased. */
    if( fnet_memcmp((void *)(fapp_params_flash_p), FAPP_PARAMS_SIGNATURE, sizeof(FAPP_PARAMS_SIGNATURE)) != 0 )
    {
        /**** Write all parameters to flash. ****/
        if(fapp_params_memcpy( fapp_params_flash_p, &fapp_params_ram, sizeof(struct fapp_params_flash)) == FNET_ERR)
        {
            return FNET_ERR;
        }

        if(fapp_params_flush() == FNET_ERR)
        {
            return FNET_ERR;
        }

        /* Simple check if it was written. */
        if( fnet_memcmp((void *)(fapp_params_flash_p), FAPP_PARAMS_SIGNATURE, sizeof(FAPP_PARAMS_SIGNATURE)) == 0 )
        {
            return FNET_OK;
        }
    }

    return FNET_ERR;
}
#endif

/************************************************************************
* DESCRIPTION: Load configuration parameters from flash.
************************************************************************/
#if FAPP_CFG_PARAMS_READ_FLASH
fnet_return_t fapp_params_from_flash(void)
{
    struct fapp_params_flash    *fnet_params = (struct fapp_params_flash *)FAPP_CFG_FLASH_PARAMS_ADDRESS;
    fnet_return_t               result;
    fnet_netif_desc_t           netif;

    /* Check signature. */
    if(fnet_strncmp( fnet_params->signature, FAPP_PARAMS_SIGNATURE, FAPP_PARAMS_SIGNATURE_SIZE ) == 0)
    {
        /* FNET stack parameters. */
        fnet_netif_set_default(fnet_netif_get_by_name(fnet_params->fnet_params.netif_name));

        netif = fnet_netif_get_default();
        fnet_netif_set_hw_addr(netif, fnet_params->fnet_params.mac, sizeof(fnet_mac_addr_t));

#if FNET_CFG_IP4
        fnet_netif_set_ip4_addr(netif, fnet_params->fnet_params.address, fnet_params->fnet_params.netmask);
        fnet_netif_set_ip4_gateway(netif, fnet_params->fnet_params.gateway);
#endif /* FNET_CFG_IP4 */

        /* Host name.*/
        fnet_strncpy(fapp_params_host_name, fnet_params->fnet_params.host_name, FAPP_PARAMS_HOST_NAME_SIZE);

#if FAPP_CFG_PARAMS_BOOT
        fapp_params_boot_config = fnet_params->boot_params; /* Boot parameters. */
#endif
#if FAPP_CFG_PARAMS_TFTP
        fapp_params_tftp_config = fnet_params->tftp_params; /* TFTP loader parameters. */
#endif
#if FAPP_CFG_PARAMS_WIFI
        fapp_params_wifi_config = fnet_params->wifi_params; /* Wi-Fi parameters. */
#endif

        result = FNET_OK;
    }
    else
    {
        result = FNET_ERR;
    }

    return result;

}
#endif
