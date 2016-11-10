/**************************************************************************
*
* Copyright 2015 by Andrey Butok. FNET Community.
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
**********************************************************************/ /*!
*
* @file fnet_ssl_config.h
*
* @author Andrey Butok
*
* @brief FNET SSL library configuration file.
*
***************************************************************************/

/**************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 **************************************************************************/

#ifndef _FNET_SSL_CONFIG_H_

#define _FNET_SSL_CONFIG_H_

/* TBD under development */

/**************************************************************************/ /*!
 * @def      FNET_CFG_SSL //TBD under development
 * @internal
 * @brief    SSL Library support:
 *               - @c 1 = is enabled.
 *               - @b @c 0 = is disabled (Default value).
 ******************************************************************************/
#ifndef FNET_CFG_SSL
    #define FNET_CFG_SSL       (0)
#endif


#endif /* _FNET_SSL_CONFIG_H_ */
