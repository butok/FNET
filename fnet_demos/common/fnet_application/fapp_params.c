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
* @file fapp_params.c
*
* @author Andrey Butok
*
* @brief FNET Shell Demo implementation.
*
***************************************************************************/

#include "fapp.h"
#include "fapp_prv.h"
#include "fapp_mem.h"
#include "fapp_params.h"


#if FNET_CFG_FLASH
    #define fapp_params_erase   fnet_flash_erase
    #define fapp_params_memcpy  fnet_flash_memcpy
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
#if FNET_CFG_COMP_UV || FNET_CFG_COMP_GNUC
    __attribute__((section(".fapp_params")))
#endif
static const struct fapp_params_flash fapp_params_config
#if FNET_CFG_COMP_UV || FNET_CFG_COMP_GNUC
    __attribute__((used))
#endif
    =
{
    FAPP_PARAMS_SIGNATURE,
    {
        FAPP_CFG_PARAMS_NETIF_NAME, /* Default interface */
        FAPP_CFG_PARAMS_IP_ADDR,    /* address */
        FAPP_CFG_PARAMS_IP_MASK,    /* netmask */
        FAPP_CFG_PARAMS_IP_GW,      /* gateway */
        FAPP_CFG_PARAMS_IP_DNS,     /* DNS */
        FAPP_CFG_PARAMS_MAC_ADDR,   /* MAC address */
        FAPP_CFG_PARAMS_HOST_NAME   /* Host name*/
    },
    {
        FAPP_CFG_PARAMS_BOOT_MODE,          /* boot */
        FAPP_CFG_PARAMS_BOOT_DELAY,         /* boot_delay */
        FAPP_CFG_PARAMS_BOOT_GO_ADDRESS,    /* go_address */
        FAPP_CFG_PARAMS_BOOT_SCRIPT         /* boot_script */
    },
    {
        FAPP_CFG_PARAMS_TFTP_SERVER,            /* tftp_ip */
        FAPP_CFG_PARAMS_TFTP_FILE_TYPE,         /* image_type */
        FAPP_CFG_PARAMS_TFTP_FILE_RAW_ADDRESS,  /* raw_address */
        FAPP_CFG_PARAMS_TFTP_FILE_NAME,         /* image */
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
    FAPP_CFG_PARAMS_BOOT_MODE,          /* mode */
    FAPP_CFG_PARAMS_BOOT_DELAY,         /* delay */
    FAPP_CFG_PARAMS_BOOT_GO_ADDRESS,    /* go_address */
    FAPP_CFG_PARAMS_BOOT_SCRIPT         /* boot_script */
};
#endif

#if FAPP_CFG_PARAMS_TFTP
struct fapp_params_tftp fapp_params_tftp_config =
{
    FAPP_CFG_PARAMS_TFTP_SERVER,            /* tftp_ip */
    FAPP_CFG_PARAMS_TFTP_FILE_TYPE,         /* image_type */
    FAPP_CFG_PARAMS_TFTP_FILE_RAW_ADDRESS,  /* raw_address */
    FAPP_CFG_PARAMS_TFTP_FILE_NAME,         /* image */
};
#endif


/************************************************************************
* NAME: fapp_params_to_flash
*
* DESCRIPTION: Save current configuration parameters to the flash.
************************************************************************/
#if FAPP_CFG_SAVE_CMD && FNET_CFG_FLASH
fnet_return_t fapp_params_to_flash(void)
{
    struct fapp_params_fnet     fnet_params;
    struct fapp_params_flash    *fapp_params = (struct fapp_params_flash *)FAPP_FLASH_PARAMS_ADDRESS;
    fnet_netif_desc_t           netif = fnet_netif_get_default();

#if FNET_CFG_IP4
    /* Save IP address only if it was allocated manually/statically. */
    if(fnet_netif_get_ip4_addr_type(netif) == FNET_NETIF_IP_ADDR_TYPE_MANUAL)
    {
        fnet_params.address = fnet_netif_get_ip4_addr(netif);    /* Take the current value. */
    }
    else
    {
        fnet_params.address = fapp_params->fnet_params.address; /* Preserve the old value.*/
    }

    fnet_params.netmask = fnet_netif_get_ip4_subnet_mask(netif);
    fnet_params.gateway = fnet_netif_get_ip4_gateway(netif);
#else
    /* Preserve the old value.*/
    fnet_params = fapp_params->fnet_params;
#endif

    fnet_netif_get_hw_addr(netif, fnet_params.mac, sizeof(fnet_mac_addr_t));

    /* Host name.*/
    fnet_strncpy(fnet_params.host_name, fapp_params_host_name, FAPP_PARAMS_HOST_NAME_SIZE);

    /* Erase one paage allocated for configuration parameters.*/
    fapp_params_erase( (void *)(fapp_params), sizeof(struct fapp_params_flash));

    /* Simple check if erased. */
    if( fnet_memcmp((void *)(fapp_params), FAPP_PARAMS_SIGNATURE, sizeof(FAPP_PARAMS_SIGNATURE)) != 0 )
    {
        /* Write FNET parameters to the flash.*/
        fapp_params_memcpy( (void *)&fapp_params->fnet_params, &fnet_params, sizeof(struct fapp_params_fnet)  );

        /* Write BOOT parameters to the flash.*/
#if FAPP_CFG_PARAMS_BOOT
        fapp_params_memcpy( (void *)&fapp_params->boot_params, &fapp_params_boot_config, sizeof(struct fapp_params_boot)  );
#endif

        /* Write TFTP parameters to the flash.*/
#if FAPP_CFG_PARAMS_TFTP
        fapp_params_memcpy( (void *)&fapp_params->tftp_params, &fapp_params_tftp_config, sizeof(struct fapp_params_tftp)  );
#endif

        /* Write Signature.*/
        fapp_params_memcpy( (void *)&fapp_params->signature, FAPP_PARAMS_SIGNATURE, sizeof(FAPP_PARAMS_SIGNATURE)  );

        /* Simple check if it was written. */
        if( fnet_memcmp((void *)(fapp_params), FAPP_PARAMS_SIGNATURE, sizeof(FAPP_PARAMS_SIGNATURE)) == 0 )
        {
            return FNET_OK;
        }
    }

    return FNET_ERR;
}
#endif

/************************************************************************
* NAME: fapp_params_from_flash
*
* DESCRIPTION: Load configuration parameters from flash.
************************************************************************/
#if FAPP_CFG_PARAMS_READ_FLASH
fnet_return_t fapp_params_from_flash(void)
{
    struct fapp_params_flash    *fnet_params = (struct fapp_params_flash *)FAPP_FLASH_PARAMS_ADDRESS;
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
        fnet_netif_set_ip4_addr(netif, fnet_params->fnet_params.address);
        fnet_netif_set_ip4_gateway(netif, fnet_params->fnet_params.gateway);
        fnet_netif_set_ip4_subnet_mask(netif, fnet_params->fnet_params.netmask);
#endif /* FNET_CFG_IP4 */

        /* Host name.*/
        fnet_strncpy(fapp_params_host_name, fnet_params->fnet_params.host_name, FAPP_PARAMS_HOST_NAME_SIZE);

#if FAPP_CFG_PARAMS_BOOT
        fapp_params_boot_config = fnet_params->boot_params; /* Boot parameters. */
#endif
#if FAPP_CFG_PARAMS_TFTP
        fapp_params_tftp_config = fnet_params->tftp_params; /* TFTP loader parameters. */
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

