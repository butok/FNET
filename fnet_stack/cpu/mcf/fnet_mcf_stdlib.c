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
**********************************************************************/ /*!
*
* @file fnet_mcf_stdlib.c
*
* @author Andrey Butok
*
* @brief ColdFire optimized/specific standard functions.
*
***************************************************************************/
#include "fnet.h"

#if FNET_MCF

#include "fnet_stdlib.h"

/****************************************************************/
#if FNET_CFG_OVERLOAD_MEMCPY
void *fnet_memcpy (void *dest, const void *src, unsigned n)
{
	int longs, bytes;
	fnet_uint32 *dpl = (fnet_uint32 *)dest;
	fnet_uint32 *spl = (fnet_uint32 *)src;
	fnet_uint8  *dpb, *spb;

    bytes = (n & 0x3);
    longs = ((n - bytes) >> 2);

    for (longs++; --longs;)
        *dpl++ = *spl++;
		
    dpb = (fnet_uint8 *)dpl;
    spb = (fnet_uint8 *)spl;

    for (bytes++; --bytes;)
        *dpb++ = *spb++;

	return dest;
}
#endif

#endif /*FNET_MCF*/
