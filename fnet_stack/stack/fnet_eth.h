/**************************************************************************
*
* Copyright 2008-2018 by Andrey Butok. FNET Community.
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
* Ethernet platform independent API definitions.
*
***************************************************************************/

#ifndef _FNET_ETH_H_

#define _FNET_ETH_H_

/**************************************************************************
*     Definitions
***************************************************************************/

/*! @addtogroup fnet_netif
*/
/*! @{ */

/**************************************************************************/ /*!
 * @def     FNET_MAC_ADDR_STR_SIZE
 * @brief   Size of the string buffer that will contain
 *          null-terminated ASCII string of MAC address, represented
 *          by six groups of two hexadecimal digits, separated by colons (:).
 * @see fnet_mac_to_str
 * @showinitializer
 ******************************************************************************/
#define FNET_MAC_ADDR_STR_SIZE       (18)

/**************************************************************************/ /*!
 * @def     FNET_ETH_MTU
 * @brief   The largest allowed MTU by Ethernet at the network layer.
 * @showinitializer
 ******************************************************************************/
#define FNET_ETH_MTU                (1500u)

/**************************************************************************/ /*!
 * @brief Media Access Control (MAC) address  type.
 ******************************************************************************/
typedef fnet_uint8_t fnet_mac_addr_t[6]; /* MAC address type.*/

/**************************************************************************/ /*!
 * @def FNET_MAC_ADDR_INIT
 * @param a First octet of an MAC address.
 * @param b Second octet of an MAC address.
 * @param c Third octet of an MAC address.
 * @param d Fourth octet of an MAC address.
 * @param e Fifth  octet of an MAC address.
 * @param f Sixth octet of an MAC address.
 * @hideinitializer
 * @brief Converts the standard notation @c a:b:c:d:e:f to fnet_mac_addr_t value.
 ******************************************************************************/
#define FNET_MAC_ADDR_INIT(a, b, c, d, e, f)   { (a), (b), (c), (d), (e), (f) }

/* MAC address is multicast. */
#define FNET_MAC_ADDR_IS_MULTICAST(a)	((((a)[0]) == 0x01U)?FNET_TRUE:FNET_FALSE)

/* MAC address is broadcast. */
#define FNET_MAC_ADDR_IS_BROADCAST(a)	(((((a)[0]) == 0xFFU) && \
        (((a)[1]) == 0xFFU)&& \
        (((a)[1]) == 0xFFU)&& \
        (((a)[1]) == 0xFFU)&& \
        (((a)[1]) == 0xFFU)&& \
        (((a)[1]) == 0xFFU))? FNET_TRUE:FNET_FALSE)
/* Copying address. */
#define FNET_MAC_ADDR_COPY(from_addr, to_addr)  \
    (fnet_memcpy(&to_addr[0], &from_addr[0], sizeof(fnet_mac_addr_t)))

/* Useful mac addresses */
extern const fnet_mac_addr_t fnet_eth_null_addr;
extern const fnet_mac_addr_t fnet_eth_broadcast;

/******************************************************************************
*     Function Prototypes
*******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************************************************/ /*!
 *
 * @brief    Converts a 6 byte MAC address into a null terminated string.
 *
 *
 * @param[in] addr       MAC address.
 *
 * @param[out] str_mac    Pointer to a character buffer will contain the resulting
 *                   text address in standard ":" notation. @n
 *                   The @c str_mac buffer must be at least 18 bytes long
 *                   (@ref FNET_MAC_ADDR_STR_SIZE).
 *
 * @return This function always returns the @c str_mac.
 *
 * @see fnet_str_to_mac()
 ******************************************************************************
 * This function takes an MAC-48 address, specified by the @c addr
 * parameter, and returns a null-terminated ASCII string, represented
 * by six groups of two hexadecimal digits, separated by colons (:),
 * in transmission order (e.g. 01:23:45:67:89:ab ), into buffer pointed to by the
 * @c str_mac.
 ******************************************************************************/
fnet_char_t *fnet_mac_to_str( const fnet_mac_addr_t addr, fnet_char_t *str_mac );

/***************************************************************************/ /*!
 *
 * @brief    Converts a null terminated string to a 6 byte MAC address
 *
 * @param[in] str_mac    Null-terminated MAC address string as pairs of
 *                      hexadecimal numbers separated by colons.
 *
 * @param[out] addr      Buffer will contain a suitable
 *                      binary representation of the @c str_mac MAC address .
 *
 * @return This function returns:
 *   - @ref FNET_OK if no error occurs.
 *   - @ref FNET_ERR if the string in the @c str_mac parameter does not contain
 *     a legitimate MAC address.
 *
 * @see fnet_mac_to_str()
 ******************************************************************************
 * This function interprets the character string specified by the @c str_mac
 * parameter. This string represents a numeric MAC address expressed
 * in six groups of two hexadecimal digits, separated by colons (:),
 * in transmission order. The value returned, pointed to by the @c addr,
 * is a number suitable for use as an MAC address.
 ******************************************************************************/
fnet_return_t fnet_str_to_mac( const fnet_char_t *str_mac, fnet_mac_addr_t addr );

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif
