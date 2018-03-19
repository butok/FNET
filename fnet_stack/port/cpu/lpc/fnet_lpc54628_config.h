/**************************************************************************
*
* Copyright 2017-2018 by Andrey Butok. FNET Community.
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
*  K60DN512 specific configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_LPC54628_CONFIG_H_
#define _FNET_LPC54628_CONFIG_H_

#define FNET_LPC                                        (1)

/* System frequency in Hz. */
#ifndef FNET_CFG_CPU_CLOCK_HZ
    #define FNET_CFG_CPU_CLOCK_HZ                       (220000000)
#endif

/* LPC546xx devices has CTIMER0-4 */
#define  FNET_CFG_CPU_TIMER_NUMBER_MAX                  (4u)

/* TBD.*/
#define FNET_CFG_CPU_FLASH                              (0)

/* The platform does not have second Ethernet Module.*/
#define FNET_CFG_CPU_ETH1                               (0)

#endif /* _FNET_LPC54628_CONFIG_H_ */
