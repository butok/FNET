/**************************************************************************
*
* Copyright 2012-2016 by Andrey Butok. FNET Community.
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
* @file fnet_brtos_config.h
*
* @author Andrey Butok
*
* @date Nov-6-2012
*
* @version 0.0.2.0
*
* @brief Default BRTOS-specific configuration. @n
*        Experimental. Not supported.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_BRTOS_CONFIG_H_
#define _FNET_BRTOS_CONFIG_H_

#ifndef FNET_CFG_OS_ISR
    #define FNET_CFG_OS_ISR     (1)
#endif
#ifndef FNET_CFG_OS_MUTEX
    #define FNET_CFG_OS_MUTEX   (1)
#endif

#endif /* _FNET_BRTOS_CONFIG_H_ */
