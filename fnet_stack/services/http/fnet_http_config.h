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
* @file fnet_http_config.h
*
* @author Andrey Butok
*
* @brief FNET HTTP Server configuration file.
*
***************************************************************************/

/**************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 **************************************************************************/

#ifndef _FNET_HTTP_CONFIG_H_

#define _FNET_HTTP_CONFIG_H_

/*! @addtogroup fnet_http_config */
/*! @{ */

/**************************************************************************/ /*!
 * @def      FNET_CFG_HTTP
 * @brief    HTTP Server service support:
 *               - @c 1 = is enabled.
 *               - @b @c 0 = is disabled (Default value).
 ******************************************************************************/
#ifndef FNET_CFG_HTTP
    #define FNET_CFG_HTTP                   (0)
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_HTTP_MAX
 * @brief   Maximum number of the HTTP Servers that can be run simultaneously.@n
 *          Default value @b @c 1.
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_HTTP_MAX
    #define FNET_CFG_HTTP_MAX               (1u)
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_HTTP_SESSION_MAX
 * @brief   Maximum number of simultaneous user-session that can be handled
 *          by the HTTP server.@n
 *          Default value is @b @c 3.
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_HTTP_SESSION_MAX
    #define FNET_CFG_HTTP_SESSION_MAX       (3u)
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_HTTP_SSI
 * @brief   HTTP Server SSI (Server Side Includes) support:
 *               - @b @c 1 = is enabled (Default value).
 *               - @c 0 = is disabled.
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_HTTP_SSI
    #define FNET_CFG_HTTP_SSI               (1)
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_HTTP_CGI
 * @brief   HTTP Server CGI (Common Gateway Interface) support:
 *               - @b @c 1 = is enabled (Default value).
 *               - @c 0 = is disabled.
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_HTTP_CGI
    #define FNET_CFG_HTTP_CGI               (1)
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_HTTP_PORT
 * @brief   Default HTTP port number (in network byte order).@n
 *          It can be changed during the HTTP server initialization by the
 *          @ref fnet_http_init() function.@n
 *          Default value FNET_HTONS(80).
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_HTTP_PORT
    #define FNET_CFG_HTTP_PORT              (FNET_HTONS(80u))
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_HTTP_REQUEST_SIZE_MAX
 * @brief   Maximum size of an incoming request.@n
 *          Also it defines the maximum number of bytes to use for internal
 *          buffering (parsing, receive and transmit buffering).@n
 *          Default value @b @c 300.
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_HTTP_REQUEST_SIZE_MAX
    #define FNET_CFG_HTTP_REQUEST_SIZE_MAX  (300u)
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_HTTP_VERSION_MAJOR
 * @brief   Hypertext Transfer Protocol HTTP version 1.x support:
 *               - @c 1 = is enabled.
 *               - @b @c 0 = is disabled. It means HTTP/0.9 (Default value).
 ******************************************************************************/
#ifndef FNET_CFG_HTTP_VERSION_MAJOR
    #define FNET_CFG_HTTP_VERSION_MAJOR     (1u)
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_HTTP_AUTHENTICATION_BASIC
 * @brief   The HTTP/1.x Basic Authentification Scheme (RFC2617) support:
 *               - @b @c 1 = is enabled (default value).
 *               - @c 0 = is disabled.
 ******************************************************************************/
#ifndef FNET_CFG_HTTP_AUTHENTICATION_BASIC
    #define FNET_CFG_HTTP_AUTHENTICATION_BASIC  (0)
#endif

/**************************************************************************/ /*!
 * @def     FNET_CFG_HTTP_POST
 * @brief   The HTTP/1.x POST method support:
 *               - @c 1 = is enabled.
 *               - @b @c 0 = is disabled (Default value).
 * @showinitializer
 ******************************************************************************/
#ifndef FNET_CFG_HTTP_POST
    #define FNET_CFG_HTTP_POST                  (0)
#endif

/*! @} */

#endif /* _FNET_HTTP_CONFIG_H_ */
