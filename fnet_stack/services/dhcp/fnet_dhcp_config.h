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

/**************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 **************************************************************************/

#ifndef _FNET_DHCP_CONFIG_H_

#define _FNET_DHCP_CONFIG_H_

/*! @addtogroup fnet_dhcp_config */
/*! @{ */

/**************************************************************************/ /*!
 * @def      FNET_CFG_DHCP
 * @brief    DHCP Client service support:
 *               - @c 1 = is enabled.
 *               - @b @c 0 = is disabled (Default value).
 ******************************************************************************/
#ifndef FNET_CFG_DHCP
    #define FNET_CFG_DHCP       (0)
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_DHCP_MAX
 * @brief   Maximum number of the DHCP Servers that can be run simultaneously.@n
 *          Default value is @b @c 1. @n
 *          @note It is not posible to run several DHCP servers on the same networking interface.
 *
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_DHCP_MAX
    #define FNET_CFG_DHCP_MAX                 (1u)
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_DHCP_BOOTP
 * @brief   DHCP/BOOTP-service used protocol:
 *               - @c 1 = BOOTP (Bootstrap) protocol, defined by RFC951.
 *               - @b @c 0 = DHCP protocol (extension of BOOTP), defined by RFC2131 (Default value).@n
 *          @n
 *          BOOTP typically provides fixed allocation of a single IP address
 *          for each client, permanently reserving this address in the BOOTP
 *          server database.@n
 *          DHCP typically provides dynamic, leased allocation of available
 *          IP addresses, reserving each DHCP client address temporarily
 *          in the DHCP server database.@n
 *          DHCP can automatically rebind or renew their leases while BOOTP requires a system restart.@n
 *          Code size of BOOTP is less than DHCP by about 1.5KB.
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_DHCP_BOOTP
    #define FNET_CFG_DHCP_BOOTP                 (0)
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_DHCP_PORT_CLIENT
 * @brief   DHCP client port number (in network byte order).@n
 *          The client listen DHCP messages from a server on this port.@n
 *          Default value is @b @c 68.
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_DHCP_PORT_CLIENT
    #define FNET_CFG_DHCP_PORT_CLIENT           (FNET_HTONS(68U))
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_DHCP_PORT_SERVER
 * @brief   DHCP server port number (in network byte order).@n
 *          DHCP messages from a client to a server are sent to this port.@n
 *          Default value is @b @c 67.
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_DHCP_PORT_SERVER
    #define FNET_CFG_DHCP_PORT_SERVER           (FNET_HTONS(67U))
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_DHCP_RESPONSE_TIMEOUT
 * @brief   Timeout for the response from the remote DHCP server (in seconds).@n
 *           @n
 *          The recommended value is from @c 1 till @c 10 seconds.@n
 *          If the DHCP client does not receive any response from a DHCP server during this time,
 *          it sends new discover message. @n
 *          Default value is @b @c 4.
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_DHCP_RESPONSE_TIMEOUT
    #define FNET_CFG_DHCP_RESPONSE_TIMEOUT      (4U)
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_DHCP_OVERLOAD
 * @brief   DHCP "Option overload" support [RFC 2132 9.3]:
 *               - @c 1 = is enabled.
 *               - @b @c 0 = is disabled (Default value).@n
 *          @n
 *          It's recomended to disable this option, as it's not used in practice.@n @n
 *          [RFC 2132 9.3]: "This option is used to indicate that the DHCP 'sname' or 'file'
 *          fields are being overloaded by using them to carry DHCP options. A
 *          DHCP server inserts this option if the returned parameters will
 *          exceed the usual space allotted for options.@n
 *          If this option is present, the client interprets the specified
 *          additional fields after it concludes interpretation of the standard
 *          option fields."
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_DHCP_OVERLOAD
    #define FNET_CFG_DHCP_OVERLOAD              (0)
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_DHCP_BROADCAST
 * @brief   DHCP "Broadcast Address Option" support [RFC 2132 5.3]:
 *               - @c 1 = is enabled.
 *               - @b @c 0 = is disabled (Default value).@n
 *          @n
 *          [RFC 2132 9.3]: "This option specifies the broadcast address
 *          in use on the client's subnet."
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_DHCP_BROADCAST
    #define FNET_CFG_DHCP_BROADCAST             (0)
#endif

/*! @} */

#endif /* _FNET_DHCP_CONFIG_H_ */
