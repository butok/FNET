/**************************************************************************
*
* Copyright 2017 by Andrey Butok. FNET Community.
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
*  DHCP common code.
*
***************************************************************************/
#include "fnet.h"

#if (FNET_CFG_DHCP_CLN || FNET_CFG_DHCP_SRV) && FNET_CFG_IP4

#include "fnet_dhcp_prv.h"

/* The first four octets of the vendor information field have been assigned to the "magic cookie".*/
const fnet_uint8_t fnet_dhcp_magic_cookie[4] = {99, 130, 83, 99};

/************************************************************************
* DESCRIPTION: Add DHCPv4 option.
* Returns:
*   - ponter to the position after the option.
*   - FNET_NULL if no space for the option (+ END option).
************************************************************************/
fnet_uint8_t *_fnet_dhcp_add_option(fnet_uint8_t *option_buffer, fnet_size_t option_buffer_size, fnet_uint8_t option_code, fnet_uint8_t option_length,  const void *option_value)
{
    FNET_ASSERT(option_buffer != FNET_NULl);
    FNET_ASSERT(option_value != FNET_NULl);

    fnet_uint8_t    *result = FNET_NULL;

    if(option_buffer_size >= (2U/*type+length*/ + 1U/* END option*/ + option_length))
    {
        option_buffer[0] = option_code;
        option_buffer[1] = option_length;
        fnet_memcpy(&option_buffer[2], option_value, (fnet_size_t)option_length);
        result = &option_buffer[2U + option_length];
    }
    return result;
}

/************************************************************************
* DESCRIPTION: Print DHCP header. For debug needs.
************************************************************************/
#if FNET_CFG_DEBUG_TRACE_DHCP_SRV && FNET_CFG_DEBUG_TRACE
void _fnet_dhcp_trace(fnet_uint8_t *str, fnet_dhcp_header_t *header )
{
    fnet_char_t     ip_str[FNET_IP4_ADDR_STR_SIZE];
    fnet_index_t    i;

    fnet_printf(FNET_SERIAL_ESC_FG_GREEN"%s", str); /* Print app-specific header.*/
    fnet_println("[DHCP header]"FNET_SERIAL_ESC_ATTR_RESET);
    fnet_println("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+");
    fnet_println("| (op)      %3u | (htype)   %3u | hlen      %3u | (hops)    %3u |", header->op, header->htype, header->hlen,  header->hops);
    fnet_println("+---------------+---------------+---------------+---------------+");
    fnet_println("|(xid)                                             0x%010u |", fnet_ntohl(header->xid));
    fnet_println("+-------------------------------+-------------------------------+");
    fnet_println("|(secs)                   %5u |(flags)                  %5u |", fnet_ntohs(header->secs), fnet_ntohs(header->flags));
    fnet_println("+-------------------------------+-------------------------------+");
    fnet_println("|(ciaddr)                                       "FNET_SERIAL_ESC_FG_BLUE"%15s"FNET_SERIAL_ESC_ATTR_RESET" |",
                 fnet_inet_ntoa(*(struct fnet_in_addr *)(&header->ciaddr), ip_str));
    fnet_println("+---------------------------------------------------------------+");
    fnet_println("|(yiaddr)                                       "FNET_SERIAL_ESC_FG_BLUE"%15s"FNET_SERIAL_ESC_ATTR_RESET" |",
                 fnet_inet_ntoa(*(struct fnet_in_addr *)(&header->yiaddr), ip_str));
    fnet_println("+---------------------------------------------------------------+");
    fnet_println("|(siaddr)                                       "FNET_SERIAL_ESC_FG_BLUE"%15s"FNET_SERIAL_ESC_ATTR_RESET" |",
                 fnet_inet_ntoa(*(struct fnet_in_addr *)(&header->siaddr), ip_str));
    fnet_println("+---------------------------------------------------------------+");
    fnet_println("|(giaddr)                                       "FNET_SERIAL_ESC_FG_BLUE"%15s"FNET_SERIAL_ESC_ATTR_RESET" |",
                 fnet_inet_ntoa(*(struct fnet_in_addr *)(&header->giaddr), ip_str));
    fnet_println("+---------------------------------------------------------------+");
    fnet_printf("|(chaddr)                       ");
    for (i = 0u; i < 16u; i++)
    {
        fnet_printf("%02X", header->chaddr[i]);
    }

    fnet_println("|");
    fnet_println("+---------------------------------------------------------------+");
    fnet_println("| (sname) \t %s", header->sname);
    fnet_println("+---------------------------------------------------------------+");
    fnet_println("| (file) \t %s", header->file);
    fnet_println("+---------------------------------------------------------------+");
}
#endif

#endif /* (FNET_CFG_DHCP_CLN || FNET_CFG_DHCP_SRV) && FNET_CFG_IP4 */
