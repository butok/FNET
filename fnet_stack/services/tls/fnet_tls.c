/**************************************************************************
*
* Copyright 2014-2016 by Andrey Butok. FNET Community.
*
***************************************************************************
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License Version 3
* or later (the "LGPL").
*
* As a special exception, the copyright holders of the FNET project give you
* permission to link the FNET sources with independent modules to produce an
* executable, regardless of the license terms of these independent modules,
* and to copy and distribute the resulting executable under terms of your
* choice, provided that you also meet, for each linked independent module,
* the terms and conditions of the license of that module.
* An independent module is a module which is not derived from or based
* on this library.
* If you modify the FNET sources, you may extend this exception
* to your version of the FNET sources, but you are not obligated
* to do so. If you do not wish to do so, delete this
* exception statement from your version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* You should have received a copy of the GNU General Public License
* and the GNU Lesser General Public License along with this program.
* If not, see <http://www.gnu.org/licenses/>.
*
**********************************************************************/ /*!
*
* @brief FNET TLS library implementation. Using mbedTLS.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_TLS 

#if FNET_CFG_DEBUG_TLS && FNET_CFG_DEBUG
    #define FNET_DEBUG_TLS   FNET_DEBUG
#else
    #define FNET_DEBUG_TLS(...)    do{}while(0)
#endif

#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/certs.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_cache.h"
#include "mbedtls/debug.h"

/************************************************************************
*     Definitions
************************************************************************/
#define FNET_TLS_ERR_IS_INITIALIZED    "ERROR: No free context."

/************************************************************************
*    TLS interface structure.
*************************************************************************/
struct fnet_tls_if
{
    fnet_bool_t                 is_enabled;
    mbedtls_entropy_context     entropy_context;
    mbedtls_ctr_drbg_context    ctr_drbg_context;
    mbedtls_ssl_config          ssl_config;
    mbedtls_x509_crt            x509_crt;
    mbedtls_pk_context          pk_context;
#if defined(MBEDTLS_SSL_CACHE_C)
    mbedtls_ssl_cache_context   cache_context;
#endif
};

static int fnet_tls_mbedtls_send(void *ctx, unsigned char const *buf, size_t len);
static int fnet_tls_mbedtls_recv(void *ctx, unsigned char *buf, size_t len);

/* TLS context interface list. */
static struct fnet_tls_if fnet_tls_if_list[FNET_CFG_TLS_MAX];
/* TLS sockets interface list. */
static mbedtls_ssl_context fnet_tls_socket_if_list[FNET_CFG_TLS_SOCKET_MAX];

/************************************************************************
* DESCRIPTION: Initializes TLS context.
************************************************************************/
fnet_tls_desc_t fnet_tls_init(struct fnet_tls_params *params)
{
    fnet_index_t        i;
    struct fnet_tls_if  *tls_if = FNET_NULL;
    fnet_int32_t        endpoint;
    const unsigned char custom[] = "FNET";

    if(params == 0)
    {
        goto ERROR;
    }

    /* Try to find free TLS context. */
    for(i = 0u; i < FNET_CFG_TLS_MAX; i++)
    {
        if(fnet_tls_if_list[i].is_enabled == FNET_FALSE)
        {
            tls_if = &fnet_tls_if_list[i];
            break;
        }
    }

    /* Is TLS context is already initialized. */
    if(tls_if == FNET_NULL)
    {
        FNET_DEBUG_TLS(FNET_TLS_ERR_IS_INITIALIZED);
        goto ERROR;
    }

#if 0 /* Not yet implemented.*/
    switch(params->role)
    {
        case FNET_TLS_ROLE_SERVER:
            endpoint = MBEDTLS_SSL_IS_SERVER ;
            break;
        case FNET_TLS_ROLE_CLIENT:
            goto ERROR; /* Not supported yet. endpoint = MBEDTLS_SSL_IS_CLIENT*/
        default:
            goto ERROR;
    }
#else
    endpoint = MBEDTLS_SSL_IS_SERVER ;
#endif

    /* Set all the fields to zero */
    fnet_memset_zero(tls_if, sizeof(*tls_if));
    
    /* Initialize an SSL configuration context */
    mbedtls_ssl_config_init(&tls_if->ssl_config);
#if defined(MBEDTLS_SSL_CACHE_C)
    /* Initialize an SSL cache context */
    mbedtls_ssl_cache_init( &tls_if->cache_context );
#endif
    /* Initialize a certificate */
    mbedtls_x509_crt_init(&tls_if->x509_crt);
    /* Initialize public key container */
    mbedtls_pk_init(&tls_if->pk_context);
    /* Initialize entropy context */
    mbedtls_entropy_init(&tls_if->entropy_context);
    /* Initialize CTR_DRBG context */
    mbedtls_ctr_drbg_init(&tls_if->ctr_drbg_context);

    if (params->certificate_buffer && params->certificate_buffer_size)
    {
        /* Parse one or more certificates */
        if (mbedtls_x509_crt_parse(&tls_if->x509_crt, params->certificate_buffer, params->certificate_buffer_size) != 0)
        {
            goto ERROR;
        }
    }

    if(params->private_key_buffer && params->private_key_buffer_size)
    {
        /* Parse a private key in PEM or DER format */
        if (mbedtls_pk_parse_key(&tls_if->pk_context, params->private_key_buffer, params->private_key_buffer_size, NULL, 0) != 0)
        {
            goto ERROR_1;
        }
    }
    
    /* Seed and setup entropy source for future reseeds.  */
    if (mbedtls_ctr_drbg_seed(&tls_if->ctr_drbg_context, mbedtls_entropy_func, &tls_if->entropy_context, custom, sizeof(custom)) != 0)
    {
        goto ERROR_2;
    }

    /* Load default SSL configuration values. */
    if (mbedtls_ssl_config_defaults(&tls_if->ssl_config, endpoint, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT) != 0)
    {
        goto ERROR_3;
    }

    /* Set the random number generator callback */
    mbedtls_ssl_conf_rng(&tls_if->ssl_config, mbedtls_ctr_drbg_random, &tls_if->ctr_drbg_context);
 
#if defined(MBEDTLS_SSL_CACHE_C)
    /* Set the session cache callbacks (server-side only) */
    mbedtls_ssl_conf_session_cache( &tls_if->ssl_config, &tls_if->cache_context, mbedtls_ssl_cache_get, mbedtls_ssl_cache_set );
#endif
    /* Set the data required to verify peer certificate */
    mbedtls_ssl_conf_ca_chain(&tls_if->ssl_config, tls_if->x509_crt.next, NULL); //?? //DM

    /* Set own certificate chain and private key */
    if (mbedtls_ssl_conf_own_cert(&tls_if->ssl_config, &tls_if->x509_crt, &tls_if->pk_context) != 0)
    {
        goto ERROR_4;
    }

    tls_if->is_enabled = FNET_TRUE;
    return (fnet_tls_desc_t)tls_if;

ERROR_4:
    mbedtls_ssl_config_free(&tls_if->ssl_config);
#if defined(MBEDTLS_SSL_CACHE_C)
    mbedtls_ssl_cache_free(&tls_if->cache_context);
#endif
ERROR_3:
    mbedtls_entropy_free(&tls_if->entropy_context);
    mbedtls_ctr_drbg_free(&tls_if->ctr_drbg_context);
ERROR_2:
    mbedtls_pk_free(&tls_if->pk_context);
ERROR_1:
    mbedtls_x509_crt_free(&tls_if->x509_crt);
ERROR:
    return FNET_NULL;;
}

/************************************************************************
* DESCRIPTION: Release TLS context.
************************************************************************/
void fnet_tls_release(fnet_tls_desc_t tls_desc)
{
    struct fnet_tls_if  *tls_if = (struct fnet_tls_if  *)tls_desc;

    if(tls_if && (tls_if->is_enabled == FNET_TRUE ) )
    {
        mbedtls_x509_crt_free(&tls_if->x509_crt);
        mbedtls_pk_free(&tls_if->pk_context);
        mbedtls_ssl_config_free(&tls_if->ssl_config);
    #if defined(MBEDTLS_SSL_CACHE_C)
        mbedtls_ssl_cache_free(&tls_if->cache_context);
    #endif
        mbedtls_ctr_drbg_free(&tls_if->ctr_drbg_context);
        mbedtls_entropy_free(&tls_if->entropy_context);

        /* Set all the fields to zero */
        fnet_memset_zero(tls_if, sizeof(*tls_if));
    }
}

/************************************************************************
* DESCRIPTION: Create TLS connection using sock. Return TLS connection handle
************************************************************************/
fnet_tls_socket_t fnet_tls_socket(fnet_tls_desc_t tls_desc, fnet_socket_t sock)
{
    struct fnet_tls_if  *tls_if = (struct fnet_tls_if  *)tls_desc;
    fnet_tls_socket_t   result = FNET_NULL;
    fnet_index_t        i;
    mbedtls_ssl_context *tls_socket = FNET_NULL;

    if(tls_if)
    {
        for (i = 0u; i < FNET_CFG_TLS_SOCKET_MAX; i++) /* Find the empty descriptor.*/
        {
            if(fnet_tls_socket_if_list[i].conf == 0)
            {
                tls_socket = &fnet_tls_socket_if_list[i];
                break;
            }
        }

        if(tls_socket)
        {
            /* Set up an SSL context */
            if(mbedtls_ssl_setup(tls_socket, &tls_if->ssl_config) == 0)
            {
                /* Set the underlying BIO callbacks for write, read */
                mbedtls_ssl_set_bio(tls_socket, (void *)sock, fnet_tls_mbedtls_send, fnet_tls_mbedtls_recv, NULL);
#if 0  /* Start handshake.*/
                {
                    fnet_int32_t res;
                    /* Perform the SSL handshake */
                    do
                    {
                        res = mbedtls_ssl_handshake(tls_socket);
                        /* If this function returns something other than 0 or
                        * MBEDTLS_ERR_SSL_WANT_READ/WRITE, then the ssl context
                        * becomes unusable, and you should free it.*/
                        if(res == 0)
                        {
                            /* Handshake is complete.*/
                            result = tls_socket; 
                        }
                        else if ((res != MBEDTLS_ERR_SSL_WANT_READ) && (res != MBEDTLS_ERR_SSL_WANT_WRITE))
                        {
                            /* Handshake is failed.*/
                            FNET_DEBUG_TLS("TLS: mbedtls_ssl_handshake() failed = %d", res);
                            fnet_tls_socket_close(tls_socket);
                            result = FNET_NULL;
                        }
                    }
                    while((res == MBEDTLS_ERR_SSL_WANT_READ) || (res == MBEDTLS_ERR_SSL_WANT_WRITE));
                }
#else  /* Handshake postponed to recv() */ 
                result = tls_socket; 
#endif
            }
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Close TLS socket
************************************************************************/
void fnet_tls_socket_close(fnet_tls_socket_t tls_sock)
{
    if(tls_sock)
    {
#if 0
        /* Notify the peer that the connection is being closed */
        mbedtls_ssl_close_notify(tls_sock);
#endif
        /* Free referenced items in an SSL context and clear memory.*/
        mbedtls_ssl_free(tls_sock);
    }
}

/************************************************************************
* DESCRIPTION: Receive data from TLS layer.
************************************************************************/
fnet_int32_t fnet_tls_socket_recv(fnet_tls_socket_t tls_sock, fnet_uint8_t *buf, fnet_size_t len)
{
    fnet_int32_t     result;

    result = mbedtls_ssl_read(tls_sock, buf, len);

    /* If this function returns something other than a positive
     * value or MBEDTLS_ERR_SSL_WANT_READ/WRITE, the ssl context becomes unusable*/

    /* Convert to right FNET result.*/
    if((result == MBEDTLS_ERR_SSL_WANT_READ) || (result == MBEDTLS_ERR_SSL_WANT_WRITE) )
    {
        result = 0;
    }
    else if(result <= 0)
    {
        result = FNET_ERR;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Send data through TLS layer.
************************************************************************/
fnet_int32_t fnet_tls_socket_send(fnet_tls_socket_t tls_sock, fnet_uint8_t *buf, fnet_size_t len)
{
    fnet_int32_t     result;

    result = mbedtls_ssl_write(tls_sock, buf, len);

    /* If this function returns something other than a positive
     * value or MBEDTLS_ERR_SSL_WANT_READ/WRITE, the ssl context becomes unusable*/

    /* Convert to right FNET result.*/
    if((result == MBEDTLS_ERR_SSL_WANT_READ) || (result == MBEDTLS_ERR_SSL_WANT_WRITE))
    {
        result = 0;
    }
    else if(result <= 0)
    {
        result = FNET_ERR;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Write callback
************************************************************************/
static int fnet_tls_mbedtls_send(void *ctx, unsigned char const *buf, size_t len)
{
    int result;

    result = fnet_socket_send((fnet_socket_t)ctx, buf, len, 0);

    FNET_DEBUG_TLS("TLS: TX(%d) %d", len, result);
    /* Convert to right mbeTLS result.*/
    if(result == 0)
    {
        result = MBEDTLS_ERR_SSL_WANT_WRITE;
    }
    else if(result == FNET_ERR)
    {
        result = 0; /* Close connection.*/
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Read callback
************************************************************************/
static int fnet_tls_mbedtls_recv(void *ctx, unsigned char *buf, size_t len)
{
    int result;

    result = fnet_socket_recv((fnet_socket_t)ctx, buf, len, 0);

    FNET_DEBUG_TLS("TLS: RX(%d) %d", len, result);
    /* Convert to right mbeTLS result.*/
    if(result == 0)
    {
        result = MBEDTLS_ERR_SSL_WANT_READ;
    }
    else if(result == FNET_ERR)
    {
        result = 0; /* Close connection.*/
    }
    return result;
}

/* Entropy poll callback for mbedTLS */
#if defined(MBEDTLS_ENTROPY_HARDWARE_ALT)

int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
    fnet_uint32_t rnd_data;

    if(len > sizeof(rnd_data))
    {
        len = sizeof(rnd_data);
    }

    rnd_data = fnet_rand();
    fnet_memcpy(output, &rnd_data, len);

    *olen = len;
    return 0;
}

#endif

#endif /* FNET_CFG_TLS*/
