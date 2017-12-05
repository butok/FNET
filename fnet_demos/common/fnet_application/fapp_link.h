/**************************************************************************
*
* Copyright 2016-2017 by Andrey Butok. FNET Community.
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
*  FNET Shell Demo (Link).
*
***************************************************************************/

/************************************************************************
*     Private Definitions.
*************************************************************************/
#ifndef _FAPP_LINK_H_

#define _FAPP_LINK_H_

#if FNET_CFG_LINK

#if defined(__cplusplus)
extern "C" {
#endif

void fapp_link_init(fnet_shell_desc_t desc, fnet_netif_desc_t netif);
void fapp_link_release(void);
void fapp_link_info(fnet_shell_desc_t desc, fnet_netif_desc_t  netif);

#if defined(__cplusplus)
}
#endif

#endif /* FAPP_CFG_LINK */

#endif /* _FAPP_LINK_H_ */
