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
*  FNET Service API.
*
***************************************************************************/

#include "fnet.h"

/************************************************************************
*     Definitions
*************************************************************************/

/* Polling list element type definition */

typedef struct
{
    fnet_service_poll_t service;
    void *service_cookie;
} fnet_poll_list_entry_t;

/* Polling interface structure */
static struct
{
    fnet_poll_list_entry_t list[FNET_CFG_SERVICE_MAX]; /* Polling list.*/
} fnet_poll_if;

/************************************************************************
* DESCRIPTION: This function calls all registered service routines in
*              the polling list.
*************************************************************************/
void fnet_service_poll( void )
{
    fnet_index_t i;

    for (i = 0u; i < FNET_CFG_SERVICE_MAX; i++)
    {
        if(fnet_poll_if.list[i].service)
        {
            fnet_poll_if.list[i].service(fnet_poll_if.list[i].service_cookie);
        }
    }
}

/************************************************************************
* DESCRIPTION: This function adds service routine into the polling list.
*************************************************************************/
fnet_service_desc_t fnet_service_register( fnet_service_poll_t service, void *service_cookie )
{
    fnet_index_t            i = 0u;
    fnet_poll_list_entry_t  *poll_entry = 0;

    if(service)
    {
        while((i < FNET_CFG_SERVICE_MAX) && (fnet_poll_if.list[i].service))
        {
            i++;
        }

        if(i != FNET_CFG_SERVICE_MAX)
        {
            fnet_poll_if.list[i].service = service;
            fnet_poll_if.list[i].service_cookie = service_cookie;
            poll_entry = &fnet_poll_if.list[i];
        }
    }

    return (fnet_service_desc_t)poll_entry;
}

/************************************************************************
* DESCRIPTION: This function removes service routine from the polling list.
*************************************************************************/
void fnet_service_unregister( fnet_service_desc_t desc )
{
    fnet_poll_list_entry_t  *poll_entry = (fnet_poll_list_entry_t *)desc;

    if(poll_entry)
    {
        poll_entry->service = 0;
    }
}
