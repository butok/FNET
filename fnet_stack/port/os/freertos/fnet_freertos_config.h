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
* @file fnet_freertos_config.h
*
* @author Christophe Graulle, Andrey Butok
*
* @brief Default FreeRTOS-specific configuration. @n
*        Experimental. Not supported.
*
***************************************************************************/


#ifndef FNET_FREERTOS_CONFIG_H_
#define FNET_FREERTOS_CONFIG_H_

#ifndef FNET_CFG_OS_ISR
    #define FNET_CFG_OS_ISR     (0)
#endif
#ifndef FNET_CFG_OS_MUTEX
    #define FNET_CFG_OS_MUTEX   (0)
#endif
#ifndef FNET_CFG_OS_EVENT
    #define FNET_CFG_OS_EVENT   (1)
#endif

#endif /* FNET_FREERTOS_CONFIG_H_ */
