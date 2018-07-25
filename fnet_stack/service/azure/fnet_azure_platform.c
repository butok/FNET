/**************************************************************************
*
* Copyright 2018 by Andrey Butok. FNET Community.
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
*  Azure-IOT adapter API.
*
***************************************************************************/
#include "fnet.h"

#if FNET_CFG_AZURE
#include <stdlib.h>
#include "azure_c_shared_utility/platform.h"

extern const IO_INTERFACE_DESCRIPTION *fnet_azure_tlsio_get_interface_description(void);

int platform_init(void)
{
    return 0;
}

const IO_INTERFACE_DESCRIPTION *platform_get_default_tlsio(void)
{
#if FNET_CFG_TLS
    return fnet_azure_tlsio_get_interface_description();
#else
    return FNET_NULL;
#endif
}

STRING_HANDLE platform_get_platform_info(void)
{
    /* Expected format: "(<runtime name>; <operating system name>; <platform>)" */
    return STRING_construct("(native; FNET; undefined)");
}

void platform_deinit(void)
{
    return;
}

#endif /* FNET_CFG_AZURE */
