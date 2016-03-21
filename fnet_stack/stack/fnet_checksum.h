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
* @file fnet_checksum.h
*
* @author Andrey Butok
*
* @brief Private. Internet checksum API.
*
***************************************************************************/


#ifndef _FNET_CHECKSUM_H_
#define	_FNET_CHECKSUM_H_

#include "fnet.h"
#include "fnet_netbuf.h"

#if defined(__cplusplus)
extern "C" {
#endif

fnet_uint16_t fnet_checksum_buf(fnet_uint8_t *buf, fnet_size_t buf_len);
fnet_uint16_t fnet_checksum_pseudo_buf(fnet_uint8_t *buf, fnet_uint16_t buf_len, fnet_uint16_t protocol, const fnet_uint8_t *ip_src, const fnet_uint8_t *ip_dest, fnet_size_t addr_size);
fnet_uint16_t fnet_checksum(fnet_netbuf_t *nb, fnet_size_t len);
fnet_uint16_t fnet_checksum_pseudo_start( fnet_netbuf_t *nb, fnet_uint16_t protocol, fnet_uint16_t protocol_len );
fnet_uint16_t fnet_checksum_pseudo_end( fnet_uint16_t sum_s, const fnet_uint8_t *ip_src, const fnet_uint8_t *ip_dest, fnet_size_t addr_size );

#if defined(__cplusplus)
}
#endif

#endif
