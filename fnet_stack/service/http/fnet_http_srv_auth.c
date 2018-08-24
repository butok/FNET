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
*  FNET HTTP Server Authentication implementation.
*
***************************************************************************/

#include "fnet.h"

#if FNET_CFG_HTTP_SRV && FNET_CFG_HTTP_SRV_AUTHENTICATION_BASIC  && FNET_CFG_HTTP_SRV_VERSION_MAJOR

#include "fnet_http_srv_prv.h"
#include "fnet_http_srv_auth_prv.h"

/************************************************************************
*     Definitions
************************************************************************/
static void _fnet_http_srv_auth_decode_base64(fnet_char_t *src);
static fnet_size_t _fnet_http_srv_auth_scheme_basic_generate(struct fnet_http_srv_if *http, fnet_uint8_t *buffer, fnet_size_t buffer_size);
static fnet_return_t _fnet_http_srv_auth_scheme_basic_validate (const fnet_http_srv_auth_t *auth_entry, fnet_char_t *auth_param);
static fnet_uint8_t _fnet_http_srv_decode_base64_char(fnet_uint8_t c);

/************************************************************************
*     Authentication scheme table
************************************************************************/

#define FNET_HTTP_SRV_AUTH_SCHEME_TABLE_SIZE (sizeof(fnet_http_srv_auth_scheme_table)/sizeof(struct fnet_http_srv_auth_scheme_if))

static const struct fnet_http_srv_auth_scheme_if  fnet_http_srv_auth_scheme_table[] =
{
    {FNET_HTTP_SRV_AUTH_SCHEME_BASIC, "Basic", _fnet_http_srv_auth_scheme_basic_validate, _fnet_http_srv_auth_scheme_basic_generate},
    /* TBD FNET_HTTP_SRV_AUTH_SCHEME_DIGEST, "Digest" */
};

/************************************************************************
* DESCRIPTION:
************************************************************************/
void _fnet_http_srv_auth_validate_uri(struct fnet_http_srv_if *http)
{
    const fnet_http_srv_auth_t          *auth_entry = http->auth_table;
    fnet_index_t                        i;
    struct fnet_http_srv_session_if     *session =  http->session_active;

    if(auth_entry) /* Check if the table is defined.*/
    {
        /* Check if authorization is required for the dir. */
        while(auth_entry->dir_name)
        {
            if ( !fnet_strcmp_splitter(session->request.uri.path, auth_entry->dir_name, '/' ) )
                /* Authorization is required.*/
            {
                /* Find Authentication scheme.*/
                for(i = 0u; i < FNET_HTTP_SRV_AUTH_SCHEME_TABLE_SIZE; i++)
                {
                    if(fnet_http_srv_auth_scheme_table[i].id == auth_entry->scheme)
                    {
                        session->response.auth_scheme = &fnet_http_srv_auth_scheme_table[i];
                        break; /* Scheme is found.*/
                    }
                }

                if(session->response.auth_scheme)
                {
                    session->response.auth_entry = auth_entry;
                }
                break; /* Exit.*/
            }
            auth_entry ++;
        }
    }
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
fnet_return_t _fnet_http_srv_auth_validate_credentials(struct fnet_http_srv_if *http, fnet_char_t *credentials)
{
    struct fnet_http_srv_session_if         *session =  http->session_active;
    const fnet_http_srv_auth_t              *auth_entry = session->response.auth_entry;
    const struct fnet_http_srv_auth_scheme_if  *scheme = session->response.auth_scheme;
    fnet_return_t                           result = FNET_ERR;

    while (*credentials == ' ')
    {
        credentials++; /* Strip leading spaces */
    }

    if ( !fnet_strcmp_splitter(credentials, scheme->name, ' ' ) )
    {
        fnet_char_t *auth_param = &credentials[fnet_strlen(scheme->name)];

        while (*auth_param == ' ')
        {
            auth_param++; /* Strip leading spaces */
        }

        /* Call scheme handler.*/
        result = scheme->validate(auth_entry, auth_param);
    }

    return result;
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
fnet_size_t _fnet_http_srv_auth_generate_challenge(struct fnet_http_srv_if *http, fnet_uint8_t *buffer, fnet_size_t buffer_size)
{
    fnet_size_t                     result = 0u;
    struct fnet_http_srv_session_if     *session =  http->session_active;

    /* Print auth-scheme.*/
    result += (fnet_size_t)fnet_snprintf((fnet_char_t *)buffer, buffer_size, "%s ", session->response.auth_scheme->name);
    /* Print auth-params.*/
    result += session->response.auth_scheme->generate(http, &buffer[result], (fnet_size_t)(buffer_size - result));

    return result;
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
static fnet_return_t _fnet_http_srv_auth_scheme_basic_validate (const fnet_http_srv_auth_t *auth_entry, fnet_char_t *auth_param)
{
    fnet_return_t   result =  FNET_ERR;
    fnet_char_t    *password;

    /* Base64 => "userid:password".*/
    _fnet_http_srv_auth_decode_base64(auth_param);

    if((password = fnet_strchr( auth_param, ':' )) != 0) /* Find end of the "userid".*/
    {
        *password++ = '\0'; /* Mark end of userid.*/
        if(fnet_strcmp(auth_param, auth_entry->userid) == 0) /* Check "userid".*/
        {
            if(fnet_strcmp(password, auth_entry->password) == 0) /* Check "password".*/
            {
                result = FNET_OK;
            }
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Decode a base64 character.
************************************************************************/
static fnet_uint8_t _fnet_http_srv_decode_base64_char(fnet_uint8_t c)
{
    fnet_uint8_t result;

    if((c >= 'A') && (c <= 'Z'))
    {
        result = (fnet_uint8_t)(c - 'A');
    }
    else if((c >= 'a') && (c <= 'z'))
    {
        result = (c - 'a' + 26u);
    }
    else if((c >= '0') && (c <= '9'))
    {
        result = (fnet_uint8_t)(c - '0' + 52u);
    }
    else if(c == '+')
    {
        result = 62u;
    }
    else
    {
        result = 63u;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Decode the base64 encoded string.
************************************************************************/
static void _fnet_http_srv_auth_decode_base64(fnet_char_t *src)
{
    fnet_char_t     *dest = src;
    fnet_index_t    k;
    fnet_size_t     length = fnet_strlen(src);


    for(k = 0u; k < length; k += 4u)
    {
        fnet_char_t c1 = 'A', c2 = 'A', c3 = 'A', c4 = 'A';
        fnet_uint8_t b1 = 0u, b2 = 0u, b3 = 0u, b4 = 0u;

        c1 = src[k];

        if((k + 1u) < length)
        {
            c2 = src[k + 1u];
        }
        if((k + 2u) < length)
        {
            c3 = src[k + 2u];
        }
        if((k + 3u) < length)
        {
            c4 = src[k + 3u];
        }

        b1 = _fnet_http_srv_decode_base64_char(c1);
        b2 = _fnet_http_srv_decode_base64_char(c2);
        b3 = _fnet_http_srv_decode_base64_char(c3);
        b4 = _fnet_http_srv_decode_base64_char(c4);

        *dest++ = (fnet_char_t)((fnet_uint8_t)(b1 << 2u) | (fnet_uint8_t)(b2 >> 4u) );

        if(c3 != '=')
        {
            *dest++ = (fnet_uint8_t)(((b2 & 0xfu) << 4) | (b3 >> 2) );
        }

        if(c4 != '=')
        {
            *dest++ = (fnet_uint8_t)(((b3 & 0x3u) << 6) | b4 );
        }
    }

    /* Mark end of line.*/
    *dest = (fnet_uint8_t)'\0';
}

/************************************************************************
* DESCRIPTION: Decode the base64 encoded string.
************************************************************************/
static fnet_size_t _fnet_http_srv_auth_scheme_basic_generate(struct fnet_http_srv_if *http, fnet_uint8_t *buffer, fnet_size_t buffer_size)
{
    fnet_size_t result = 0u;
    char *realm;

    if(http->session_active->response.auth_entry->realm)
    {
        /* User defined realm.*/
        realm = http->session_active->response.auth_entry->realm;
    }
    else
    {
        /* User directory name as realm.*/
        realm = http->session_active->response.auth_entry->dir_name;
    }


    result += (fnet_size_t)fnet_snprintf((fnet_char_t *)buffer, buffer_size, "realm=\"%s\"\r\n", realm);

    return result;
}

#endif
