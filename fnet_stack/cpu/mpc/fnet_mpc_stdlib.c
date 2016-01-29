/**************************************************************************
* 
* Copyright 2011-2015 by Andrey Butok. FNET Community.
* Copyright 2011 by Andrey Butok,Gordon Jahn. Freescale Semiconductor, Inc.
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
* @file fnet_mpc_stdlib.c
*
* @author Andrey Butok
*
* @date Dec-17-2012
*
* @version 0.1.1.0
*
* @brief MPC optimized/specific standard functions.
*
***************************************************************************/

#include "fnet.h"

#if FNET_MPC
#include "fnet_mpc.h"
#include "fnet_stdlib.h"

/****************************************************************/
#if FNET_CFG_OVERLOAD_MEMCPY
void fnet_memcpy(FNET_COMP_PACKED_VAR void *to_ptr, FNET_COMP_PACKED_VAR const void *from_ptr, fnet_size_t number_of_bytes);
{
	int longs, bytes;
	fnet_uint32_t *dpl = (fnet_uint32 *)dest;
	fnet_uint32_t *spl = (fnet_uint32 *)src;
	fnet_uint8_t  *dpb, *spb;

    bytes = (n & 0x3);
    longs = ((n - bytes) >> 2);

    for (longs++; --longs;)
        *dpl++ = *spl++;
		
    dpb = (fnet_uint8 *)dpl;
    spb = (fnet_uint8 *)spl;

    for (bytes++; --bytes;)
        *dpb++ = *spb++;

}
#endif

#endif /*FNET_MPC*/
