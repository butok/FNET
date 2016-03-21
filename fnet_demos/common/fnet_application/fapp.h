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
* @file fapp.h
*
* @author Andrey Butok
*
* @brief FNET Shell Demo API.
*
***************************************************************************/

#ifndef _FAPP_H_

#define _FAPP_H_

#include "fapp_config.h"
#include "fnet.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**************************************************************************/ /*!
 * @def FAPP_VERSION
 * @brief Current version number of the FNET Demoi Application.
 *        The resulting value format is xx.xx.xx = major.minor.revision, as a
 *        string.
 ******************************************************************************/
void fapp_main( void ); /* Main entry point of the shell demo. */

#if defined(__cplusplus)
}
#endif

#endif
