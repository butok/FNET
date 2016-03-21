/**************************************************************************
*
* Copyright 2008-2015 by Andrey Butok. Freescale Semiconductor, Inc.
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
* @file fnet_mcf_cache.c
*
* @author Andrey Butok
*
* @brief ColdFire cache invalidation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_MCF

/************************************************************************
* NAME: fnet_cpu_cache_invalidate
*
* DESCRIPTION: Invalidate cache.
*************************************************************************/
#if FNET_CFG_CPU_CACHE
void fnet_cpu_cache_invalidate()
{
    fnet_mcf_cacr_wr((0 | FNET_CFG_MCF_CACHE_CACR | FNET_MCF_CACR_CINV));
}
#else
void fnet_cpu_cache_invalidate()
{}
#endif


#endif /*FNET_MCF*/
