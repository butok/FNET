/**************************************************************************
*
* Copyright 2015 by Andrey Butok. FNET Community.
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
* @file fnet_ssl.h
*
* @author Andrey Butok
*
*
***************************************************************************/
#ifndef _FNET_SSL_H_

#define _FNET_SSL_H_

#include "fnet_config.h"

#if FNET_CFG_SSL /* TBD under development */

#include "fnet.h"

/**************************************************************************/ /*!
 * @brief SSL session descriptor.
 * @see fnet_ssl_session_init()
 ******************************************************************************/
typedef long fnet_ssl_session_desc_t;

/**************************************************************************/ /*!
 * @brief SSL Socket descriptor.
 ******************************************************************************/
typedef long SSL_SOCKET;

/**************************************************************************/ /*!
 * @brief    SSL Session types
 ******************************************************************************/
typedef enum
{
    FNET_SSL_SESSION_ROLE_SERVER            = 1,     /**< @brief SSL Server */
    FNET_SSL_SESSION_ROLE_CLIENT            = 2      /**< @brief SSL Client */
} fnet_ssl_session_role_t;

/**************************************************************************/ /*!
 * @brief Input parameters structure for @ref fnet_ssl_session_init()
 ******************************************************************************/
struct fnet_ssl_session_params
{
    fnet_ssl_session_role_t session_role;           /**< @brief SSL Session type. Client or server.*/
    char                    *cert_file_path;        /**< @brief Certificate file path (null-terminated string). It is optional. */
    char                    *priv_key_file_path;    /**< @brief Private Key file path (null-terminated string). It is optional. */
    char                    *ca_cert_file_path;     /**< @brief CA (Certificate Authority) certificate file path (null-terminated string). It is optional. */
};


fnet_ssl_session_desc_t fnet_ssl_session_init(struct fnet_ssl_session_params *params);
void fnet_ssl_session_release(fnet_ssl_session_desc_t ssl_desc);

SSL_SOCKET fnet_ssl_socket(fnet_ssl_session_desc_t session_desc, SOCKET sock);
int fnet_ssl_close(SSL_SOCKET ssl_sock);
int fnet_ssl_recv(SSL_SOCKET ssl_sock, char *buf, int len, int flags);
int fnet_ssl_send(SSL_SOCKET ssl_sock, char *buf, int len, int flags);

#endif /* FNET_CFG_SSL */

#endif /* _FNET_SSL_H_ */
