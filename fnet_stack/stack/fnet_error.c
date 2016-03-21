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
* @file fnet_error.c
*
* @author Andrey Butok
*
* @brief Socket error API implementation.
*
***************************************************************************/

#include "fnet.h"
#include "fnet_error.h"

static fnet_error_t FNET_ERR_NUMBER = FNET_ERR_OK;

/************************************************************************
* NAME: fnet_error_get
*
* DESCRIPTION: This function returns the last error that occurred.
*************************************************************************/
fnet_error_t fnet_error_get( void )
{
    return (FNET_ERR_NUMBER);
}

/************************************************************************
* NAME: fnet_error_set
*
* DESCRIPTION: This function sets the error code that can
*              be retrieved through the fnet_error_get().
*************************************************************************/
void fnet_error_set(fnet_error_t error )
{
    FNET_ERR_NUMBER = error;
}
