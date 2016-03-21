/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
* Copyright 2008-2010 by Andrey Butok. Freescale Semiconductor, Inc.
* Copyright 2003 by Andrey Butok. Motorola SPS.
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
* @file fnet_udp.c
*
* @author Andrey Butok
*
* @brief UDP protocol implementation.
*
***************************************************************************/

#include "fnet.h"
#include "fnet_udp.h"
#include "fnet_ip_prv.h"
#include "fnet_checksum.h"
#include "fnet_prot.h"
#include "fnet_icmp.h"

#if FNET_CFG_UDP

/************************************************************************
*     Function Prototypes
*************************************************************************/
static fnet_return_t fnet_udp_attach( fnet_socket_if_t *sk );
static fnet_return_t fnet_udp_detach( fnet_socket_if_t *sk );
static fnet_return_t fnet_udp_connect( fnet_socket_if_t *sk, struct sockaddr *foreign_addr);
static fnet_int32_t fnet_udp_snd( fnet_socket_if_t *sk, fnet_uint8_t *buf, fnet_size_t len, fnet_flag_t flags, const struct sockaddr *addr);
static fnet_int32_t fnet_udp_rcv(fnet_socket_if_t *sk, fnet_uint8_t *buf, fnet_size_t len, fnet_flag_t flags, struct sockaddr *addr);
static void fnet_udp_control_input(fnet_prot_notify_t command, struct sockaddr *src_addr,  struct sockaddr *dest_addr, fnet_netbuf_t *nb);
static fnet_return_t fnet_udp_shutdown( fnet_socket_if_t *sk, fnet_sd_flags_t how );
static void fnet_udp_input( fnet_netif_t *netif, struct sockaddr *foreign_addr,  struct sockaddr *local_addr, fnet_netbuf_t *nb, fnet_netbuf_t *ip_nb);
static void fnet_udp_release(void);
static fnet_error_t fnet_udp_output(struct sockaddr *src_addr, const struct sockaddr *dest_addr, fnet_socket_option_t *sockoption, fnet_netbuf_t *nb );

#if FNET_CFG_DEBUG_TRACE_UDP && FNET_CFG_DEBUG_TRACE
    static void fnet_udp_trace(fnet_uint8_t *str, fnet_udp_header_t *udp_hdr);
#else
    #define fnet_udp_trace(str, udp_hdr)    do{}while(0)
#endif

/************************************************************************
*     Global Data Structures
*************************************************************************/

/************************************************************************
 * Protocol API structures.
 ************************************************************************/
static const fnet_socket_prot_if_t fnet_udp_socket_api =
{
    FNET_FALSE,              /* Flag that protocol is connection oriented.*/
    fnet_udp_attach,        /* Protocol "attach" function.*/
    fnet_udp_detach,        /* Protocol "detach" function.*/
    fnet_udp_connect,       /* Protocol "connect" function.*/
    0,                      /* Protocol "accept" function.*/
    fnet_udp_rcv,           /* Protocol "receive" function.*/
    fnet_udp_snd,           /* Protocol "send" function.*/
    fnet_udp_shutdown,      /* Protocol "shutdown" function.*/
    fnet_ip_setsockopt,     /* Protocol "setsockopt" function.*/
    fnet_ip_getsockopt,     /* Protocol "getsockopt" function.*/
    0                       /* Protocol "listen" function.*/
};

fnet_prot_if_t fnet_udp_prot_if =
{
    0,                      /* Pointer to the head of the protocol's socket list.*/
    AF_SUPPORTED,           /* Address domain family.*/
    SOCK_DGRAM,             /* Socket type used for.*/
    FNET_IP_PROTOCOL_UDP,   /* Protocol number.*/
    0,                      /* Protocol initialization function.*/
    fnet_udp_release,       /* Protocol release function.*/
    fnet_udp_input,         /* Protocol input function.*/
    fnet_udp_control_input, /* Protocol input control function.*/
    0,                      /* protocol drain function.*/
    &fnet_udp_socket_api    /* Socket API */
};

/************************************************************************
* NAME: fnet_udp_release
*
* DESCRIPTION: This function releases all sockets associated
*              with UDP protocol.
*************************************************************************/
static void fnet_udp_release( void )
{
    while(fnet_udp_prot_if.head)
    {
        fnet_socket_release(&fnet_udp_prot_if.head, fnet_udp_prot_if.head);
    }
}

/************************************************************************
* NAME: fnet_udp_output
*
* DESCRIPTION: UDP output function
*************************************************************************/
static fnet_error_t fnet_udp_output(  struct sockaddr *src_addr, const struct sockaddr *dest_addr,
                                      fnet_socket_option_t *sockoption, fnet_netbuf_t *nb )
{
    fnet_netbuf_t                           *nb_header;
    fnet_udp_header_t                       *udp_header;
    fnet_error_t                            error =  FNET_ERR_OK;
    FNET_COMP_PACKED_VAR fnet_uint16_t      *checksum_p;
    fnet_netif_t                            *netif = FNET_NULL;
    fnet_scope_id_t                         scope_id = 0u;

    /* Check Scope ID.*/
    if(dest_addr->sa_scope_id) /* Take scope id from destination address.*/
    {
        scope_id = dest_addr->sa_scope_id;
    }
    else  /* Take scope id from source address.*/
    {
        scope_id = src_addr->sa_scope_id;
    }
    netif = (fnet_netif_t *)fnet_netif_get_by_scope_id(scope_id); /* It can be FNET_NULL, in case scope_id is 0.*/

    /* Construct UDP header.*/
    if((nb_header = fnet_netbuf_new(sizeof(fnet_udp_header_t), FNET_TRUE)) == 0)
    {
        fnet_netbuf_free_chain(nb);
        return (FNET_ERR_NOMEM);
    }

    udp_header = (fnet_udp_header_t *)nb_header->data_ptr;

    udp_header->source_port = src_addr->sa_port;             /* Source port number.*/
    udp_header->destination_port = dest_addr->sa_port;       /* Destination port number.*/
    nb = fnet_netbuf_concat(nb_header, nb);
    udp_header->length = fnet_htons((fnet_uint16_t)nb->total_length);  /* Length.*/

    /* Checksum calculation.*/
    udp_header->checksum = 0u;

#if FNET_CFG_UDP_CHECKSUM

#if FNET_CFG_CPU_ETH_HW_TX_IP_CHECKSUM
    if( 0
#if FNET_CFG_IP4
        || ( (dest_addr->sa_family == AF_INET)
             && ((netif = fnet_ip_route(((struct sockaddr_in *)(dest_addr))->sin_addr.s_addr)) != FNET_NULL)
             && (netif->features & FNET_NETIF_FEATURE_HW_TX_PROTOCOL_CHECKSUM)
             && (fnet_ip_will_fragment(netif, nb->total_length) == FNET_FALSE) /* Fragmented packets are not inspected.*/  )
#endif
#if FNET_CFG_IP6
        || ( (dest_addr->sa_family == AF_INET6)
             && (netif || (((netif = fnet_ip6_route(&((struct sockaddr_in6 *)(src_addr))->sin6_addr.s6_addr, &((struct sockaddr_in6 *)(dest_addr))->sin6_addr.s6_addr))) != FNET_NULL) )
             && (netif->features & FNET_NETIF_FEATURE_HW_TX_PROTOCOL_CHECKSUM)
             && (fnet_ip6_will_fragment(netif, nb->total_length) == FNET_FALSE) /* Fragmented packets are not inspected.*/  )
#endif
      )
    {
        nb->flags |= FNET_NETBUF_FLAG_HW_PROTOCOL_CHECKSUM;
        checksum_p = 0;
    }
    else
#endif /* FNET_CFG_CPU_ETH_HW_TX_IP_CHECKSUM */
    {
        udp_header->checksum = fnet_checksum_pseudo_start( nb, FNET_HTONS((fnet_uint16_t)FNET_IP_PROTOCOL_UDP), (fnet_uint16_t)nb->total_length );
        checksum_p = &udp_header->checksum;
    }
#endif /* FNET_CFG_UDP_CHECKSUM */

#if FNET_CFG_IP4
    if(dest_addr->sa_family == AF_INET)
    {
        error = fnet_ip_output(netif,
                               ((const struct sockaddr_in *)(src_addr))->sin_addr.s_addr,
                               ((const struct sockaddr_in *)(dest_addr))->sin_addr.s_addr,
                               FNET_IP_PROTOCOL_UDP, sockoption->ip_opt.tos,
#if FNET_CFG_MULTICAST
                               (fnet_uint8_t)((FNET_IP4_ADDR_IS_MULTICAST(((const struct sockaddr_in *)(dest_addr))->sin_addr.s_addr) ? sockoption->ip_opt.ttl_multicast : sockoption->ip_opt.ttl)),
#else
                               sockoption->ip_opt.ttl,
#endif /* FNET_CFG_MULTICAST */
                               nb, FNET_UDP_DF, sockoption->so_dontroute,
                               checksum_p
                              );
    }
    else
#endif
#if FNET_CFG_IP6
        if(dest_addr->sa_family == AF_INET6)
        {
            error = fnet_ip6_output( netif,
                                     fnet_socket_addr_is_unspecified(src_addr) ? FNET_NULL : & ((struct sockaddr_in6 *)(src_addr))->sin6_addr.s6_addr,
                                     &((const struct sockaddr_in6 *)(dest_addr))->sin6_addr.s6_addr,
                                     FNET_IP_PROTOCOL_UDP,
                                     FNET_IP6_ADDR_IS_MULTICAST(&((const struct sockaddr_in6 *)(dest_addr))->sin6_addr.s6_addr) ? sockoption->ip6_opt.hops_multicast : sockoption->ip6_opt.hops_unicast,
                                     nb,
                                     checksum_p
                                   );
        }
        else
#endif
        {}

    return (error);
}

/************************************************************************
* NAME: fnet_udp_input
*
* DESCRIPTION: UDP input function.
*************************************************************************/
static void fnet_udp_input(fnet_netif_t *netif, struct sockaddr *foreign_addr,  struct sockaddr *local_addr, fnet_netbuf_t *nb, fnet_netbuf_t *ip_nb)
{
    fnet_udp_header_t   *udp_header = (fnet_udp_header_t *)nb->data_ptr;
    fnet_socket_if_t       *sock;
    fnet_socket_if_t       *last;
    fnet_size_t         udp_length;
    fnet_netbuf_t       *nb_tmp;

    if((netif != 0) && (nb != 0))
    {
        /* The header must reside in contiguous area of memory.*/
        if(fnet_netbuf_pullup(&nb, sizeof(fnet_udp_header_t)) == FNET_ERR)
        {
            goto BAD;
        }

        udp_length = fnet_ntohs(udp_header->length);

        if(nb->total_length >= udp_length)  /* Check the amount of data.*/
        {
            if(nb->total_length > udp_length)
            {
                /* Logical size and the physical size of the packet should be the same.*/
                fnet_netbuf_trim(&nb, (fnet_int32_t)(udp_length - nb->total_length));
            }

#if FNET_CFG_UDP_CHECKSUM
            if((udp_header->checksum != 0u)
#if FNET_CFG_CPU_ETH_HW_RX_PROTOCOL_CHECKSUM || FNET_CFG_CPU_ETH_HW_TX_PROTOCOL_CHECKSUM
               && ((nb->flags & FNET_NETBUF_FLAG_HW_PROTOCOL_CHECKSUM) == 0)
#endif
              )
            {
                fnet_uint16_t sum;

                sum = fnet_checksum_pseudo_start( nb, FNET_HTONS((fnet_uint16_t)FNET_IP_PROTOCOL_UDP), (fnet_uint16_t)udp_length );
                sum = fnet_checksum_pseudo_end( sum, &foreign_addr->sa_data[0], &local_addr->sa_data[0],
                                                (fnet_size_t)((local_addr->sa_family == AF_INET) ? sizeof(fnet_ip4_addr_t) : sizeof(fnet_ip6_addr_t)));

                if(sum)
                {
                    goto BAD;
                }
            }
#endif
            fnet_udp_trace("RX", udp_header); /* Trace UDP header.*/

            local_addr->sa_port = udp_header->destination_port;
            foreign_addr->sa_port = udp_header->source_port;

            fnet_netbuf_trim(&nb, (fnet_int32_t)sizeof(fnet_udp_header_t));

            /* Demultiplex broadcast & multicast datagrams.*/
            if((fnet_socket_addr_is_broadcast(local_addr, netif)) || (fnet_socket_addr_is_multicast(local_addr)))
            {
                last = 0;

                for (sock = fnet_udp_prot_if.head; sock != 0; sock = sock->next)
                {
                    /* Compare local port number.*/
                    if(sock->local_addr.sa_port != local_addr->sa_port)
                    {
                        continue; /* => ignore.*/
                    }

#if FNET_CFG_MULTICAST
                    if(fnet_socket_addr_is_multicast(local_addr))
                    {
                        fnet_index_t        m;
                        fnet_bool_t         for_us = FNET_FALSE;

#if FNET_CFG_IP4
                        if(local_addr->sa_family == AF_INET)
                        {
                            for(m = 0u; m < FNET_CFG_MULTICAST_SOCKET_MAX; m++)
                            {
                                if(sock->ip4_multicast_entry[m])
                                {
                                    if((sock->ip4_multicast_entry[m]->group_addr == ((struct sockaddr_in *)(local_addr))->sin_addr.s_addr) &&  (sock->ip4_multicast_entry[m]->netif == netif ))
                                    {
                                        for_us = FNET_TRUE;
                                    }
                                }
                            }
                        }
                        else
#endif
#if FNET_CFG_IP6
                            if(local_addr->sa_family == AF_INET6)
                            {
                                for(m = 0u; m < FNET_CFG_MULTICAST_SOCKET_MAX; m++)
                                {
                                    if(sock->ip6_multicast_entry[m])
                                    {

                                        if(FNET_IP6_ADDR_EQUAL(&sock->ip6_multicast_entry[m]->group_addr, &((struct sockaddr_in6 *)(local_addr))->sin6_addr.s6_addr) && (sock->ip6_multicast_entry[m]->netif == netif))
                                        {
                                            for_us = FNET_TRUE;
                                        }
                                    }
                                }
                            }
                            else
#endif
                            {}

                        if(for_us == FNET_FALSE)
                        {
                            continue;
                        }
                    }
                    else
#endif /* FNET_CFG_MULTICAST */
                    {
                        /* Compare local address.*/
                        if(!fnet_socket_addr_is_unspecified(&sock->local_addr))
                        {
                            if(!fnet_socket_addr_are_equal(&sock->local_addr, local_addr))
                            {
                                continue;
                            }
                        }

                        /* Compare foreign address and port number.*/
                        if(!fnet_socket_addr_is_unspecified(&sock->foreign_addr))
                        {
                            if((!fnet_socket_addr_are_equal(&sock->foreign_addr, foreign_addr)) || (sock->foreign_addr.sa_port != foreign_addr->sa_port))
                            {
                                continue;
                            }
                        }
                    }

                    if((last != 0) && (last->receive_buffer.is_shutdown == FNET_FALSE))
                    {
                        if((nb_tmp = fnet_netbuf_copy(nb, 0u, FNET_NETBUF_COPYALL, FNET_FALSE)) != 0)
                        {
                            if(fnet_socket_buffer_append_address(&(last->receive_buffer), nb_tmp, foreign_addr) == FNET_ERR)
                            {
                                fnet_netbuf_free_chain(nb_tmp);
                            }
                        }
                    }
                    last = sock;
                }

                if(last == 0)
                {
                    goto BAD;
                }

                if(last->receive_buffer.is_shutdown) /* Is shutdown.*/
                {
                    goto BAD;
                }

                if(fnet_socket_buffer_append_address(&(last->receive_buffer), nb, foreign_addr) == FNET_ERR)
                {
                    goto BAD;
                }

                fnet_netbuf_free_chain(ip_nb);
            }
            else /* For unicast datagram.*/
            {
                sock = fnet_socket_lookup(fnet_udp_prot_if.head, local_addr, foreign_addr, FNET_IP_PROTOCOL_UDP);

                if(sock)
                {
                    if(sock->receive_buffer.is_shutdown) /* Is shutdown.*/
                    {
                        goto BAD;
                    }
                    if(fnet_socket_buffer_append_address(&(sock->receive_buffer), nb, foreign_addr) == FNET_ERR)
                    {
                        goto BAD;
                    }

                    fnet_netbuf_free_chain(ip_nb);
                }
                else
                {
                    fnet_netbuf_free_chain(nb); /* No match was found, send ICMP destination port unreachable.*/
#if FNET_CFG_IP4
                    if(local_addr->sa_family == AF_INET)
                    {
                        fnet_icmp_error(netif, FNET_ICMP_UNREACHABLE, FNET_ICMP_UNREACHABLE_PORT, ip_nb);
                    }
                    else
#endif
#if FNET_CFG_IP6
                        if(local_addr->sa_family == AF_INET6)
                        {
                            fnet_icmp6_error(netif, FNET_ICMP6_TYPE_DEST_UNREACH, FNET_ICMP6_CODE_DU_PORT_UNREACH, 0u, ip_nb );
                        }
                        else
#endif
                        {}
                }
            }
        }
        else
        {
            goto BAD;
        }
    }
    else
    {
    BAD:
        fnet_netbuf_free_chain(ip_nb);
        fnet_netbuf_free_chain(nb);
    }
}

/************************************************************************
* NAME: fnet_udp_attach
*
* DESCRIPTION: UDP attach function.
*************************************************************************/
static fnet_return_t fnet_udp_attach( fnet_socket_if_t *sk )
{
#if FNET_CFG_IP4
    sk->options.ip_opt.ttl = FNET_UDP_TTL;
#if FNET_CFG_MULTICAST
    sk->options.ip_opt.ttl_multicast = FNET_UDP_TTL_MULTICAST;
#endif /* FNET_CFG_MULTICAST */
    sk->options.ip_opt.tos = 0u;
#endif

#if FNET_CFG_IP6
    sk->options.ip6_opt.hops_unicast = 0u; /* Defined by ND.*/
    sk->options.ip6_opt.hops_multicast = FNET_UDP_TTL_MULTICAST;
#endif

    sk->send_buffer.count_max = FNET_UDP_TX_BUF_MAX;
    sk->receive_buffer.count_max = FNET_UDP_RX_BUF_MAX;
    return (FNET_OK);
}

/************************************************************************
* NAME: fnet_udp_detach
*
* DESCRIPTION: UDP close function.
*************************************************************************/
static fnet_return_t fnet_udp_detach( fnet_socket_if_t *sk )
{
    fnet_isr_lock();
    fnet_socket_release(&fnet_udp_prot_if.head, sk);
    fnet_isr_unlock();
    return (FNET_OK);
}

/************************************************************************
* NAME: fnet_udp_shutdown
*
* DESCRIPTION:  UDP shutdown function.
*************************************************************************/
static fnet_return_t fnet_udp_shutdown( fnet_socket_if_t *sk, fnet_sd_flags_t how )
{
    fnet_isr_lock();

    if((how & SD_READ) != 0u)
    {
        sk->receive_buffer.is_shutdown = FNET_TRUE;
        fnet_socket_buffer_release(&sk->receive_buffer);
    }

    if((how & SD_WRITE) != 0u)
    {
        sk->send_buffer.is_shutdown = FNET_TRUE;
    }

    fnet_isr_unlock();

    return (FNET_OK);
}

/************************************************************************
* NAME: fnet_udp_connect
*
* DESCRIPTION: UDP connect function.
*************************************************************************/
static fnet_return_t fnet_udp_connect( fnet_socket_if_t *sk, struct sockaddr *foreign_addr)
{
    fnet_isr_lock();

    fnet_memcpy(&sk->foreign_addr, foreign_addr, sizeof(sk->foreign_addr));
    sk->state = SS_CONNECTED;
    fnet_socket_buffer_release(&sk->receive_buffer);

    fnet_isr_unlock();
    return (FNET_OK);
}

/************************************************************************
* NAME: fnet_udp_snd
*
* DESCRIPTION: UDP send function.
*************************************************************************/
static fnet_int32_t fnet_udp_snd( fnet_socket_if_t *sk, fnet_uint8_t *buf, fnet_size_t len, fnet_flag_t flags, const struct sockaddr *addr)
{
    fnet_netbuf_t           *nb;
    fnet_error_t            error = FNET_ERR_OK;
    const struct sockaddr   *foreign_addr;
    fnet_bool_t             flags_save = FNET_FALSE;

    fnet_isr_lock();

#if FNET_CFG_TCP_URGENT
    if(flags & MSG_OOB)
    {
        error = FNET_ERR_OPNOTSUPP; /* Operation not supported.*/
        goto ERROR;
    }
#endif /* FNET_CFG_TCP_URGENT */

    if(len > sk->send_buffer.count_max)
    {
        error = FNET_ERR_MSGSIZE;   /* Message too long. */
        goto ERROR;
    }

    if(addr)
    {
        foreign_addr = addr;
    }
    else
    {
        foreign_addr = &sk->foreign_addr;
    }

    if((nb = fnet_netbuf_from_buf(buf, len, FNET_FALSE)) == 0)
    {
        error = FNET_ERR_NOMEM;     /* Cannot allocate memory.*/
        goto ERROR;
    }

    if(sk->local_addr.sa_port == 0u)
    {
        sk->local_addr.sa_port = fnet_socket_get_uniqueport(sk->protocol_interface->head, &sk->local_addr); /* Get ephemeral port.*/
    }

    if((flags & MSG_DONTROUTE) != 0u) /* Save */
    {
        flags_save = sk->options.so_dontroute;
        sk->options.so_dontroute = FNET_TRUE;
    }

    error = fnet_udp_output(&sk->local_addr, foreign_addr, &(sk->options), nb);

    if((flags & MSG_DONTROUTE) != 0u) /* Restore.*/
    {
        sk->options.so_dontroute = flags_save;
    }

    if((error == FNET_ERR_OK) && (sk->options.local_error == FNET_ERR_OK)) /* We get UDP or ICMP error.*/
    {
        fnet_isr_unlock();
        return (fnet_int32_t)(len);
    }

ERROR:
    fnet_socket_set_error(sk, error);
    fnet_isr_unlock();
    return (FNET_ERR);
}

/************************************************************************
* NAME: fnet_udp_rcv
*
* DESCRIPTION :UDP receive function.
*************************************************************************/
static fnet_int32_t fnet_udp_rcv(fnet_socket_if_t *sk, fnet_uint8_t *buf, fnet_size_t len, fnet_flag_t flags, struct sockaddr *addr)
{
    fnet_error_t    error = FNET_ERR_OK;
    fnet_int32_t    length;
    struct sockaddr foreign_addr;

    fnet_memset_zero ((void *)&foreign_addr, sizeof(foreign_addr));

#if FNET_CFG_TCP_URGENT
    if(flags & MSG_OOB)
    {
        error = FNET_ERR_OPNOTSUPP; /* Operation not supported.*/
        goto ERROR;
    }
#endif /* FNET_CFG_TCP_URGENT */

    if((length = fnet_socket_buffer_read_address(&(sk->receive_buffer), buf,
                 len, &foreign_addr, ((flags & MSG_PEEK) == 0u) ? FNET_TRUE : FNET_FALSE)) == FNET_ERR)
    {
        /* The message was too large to fit into the specified buffer and was truncated.*/
        error = FNET_ERR_MSGSIZE;
        goto ERROR;
    }

    if(sk->options.local_error == FNET_ERR_OK)
    {
        if(addr)
        {
            fnet_socket_addr_copy(&foreign_addr, addr);
        }

        return (length);
    }
    else /* We get UDP or ICMP error.*/
    {
        error = sk->options.local_error;
    }

ERROR:
    fnet_socket_set_error(sk, error);
    return (FNET_ERR);
}

/************************************************************************
* NAME: fnet_udp_control_input
*
* DESCRIPTION: This function processes the ICMP errors.
*************************************************************************/
static void fnet_udp_control_input(fnet_prot_notify_t command, struct sockaddr *src_addr,  struct sockaddr *dest_addr, fnet_netbuf_t *nb)
{
    fnet_udp_header_t   *udp_header;
    fnet_error_t        error;
    fnet_socket_if_t       *sock;

    if(src_addr && dest_addr && nb)
    {
        udp_header = (fnet_udp_header_t *)nb->data_ptr;

        switch(command)
        {
            case FNET_PROT_NOTIFY_MSGSIZE:          /* Message size forced drop.*/
                error = FNET_ERR_MSGSIZE;
                break;
            case FNET_PROT_NOTIFY_UNREACH_HOST:     /* No route to host.*/
            case FNET_PROT_NOTIFY_UNREACH_NET:      /* No route to network.*/
            case FNET_PROT_NOTIFY_UNREACH_SRCFAIL:  /* Source route failed.*/
                error = FNET_ERR_HOSTUNREACH;
                break;
            case FNET_PROT_NOTIFY_UNREACH_PROTOCOL: /* Dst says bad protocol.*/
            case FNET_PROT_NOTIFY_UNREACH_PORT:     /* Bad port #.*/
                error = FNET_ERR_CONNRESET;
                break;
            case FNET_PROT_NOTIFY_PARAMPROB:        /* Header incorrect.*/
                error = FNET_ERR_NOPROTOOPT;          /* Bad protocol option.*/
                break;
            default:
                return;
        }

        for (sock = fnet_udp_prot_if.head; sock != 0; sock = sock->next)
        {
            if((fnet_socket_addr_are_equal(&sock->foreign_addr, dest_addr))
               && (sock->foreign_addr.sa_port == udp_header->destination_port)
               && (sock->local_addr.sa_port != udp_header->source_port)
               && (fnet_socket_addr_are_equal(&sock->local_addr, src_addr)))
            {
                continue;
            }

            sock->options.local_error = error;
        }
    }
}

/************************************************************************
* NAME: fnet_udp_trace
*
* DESCRIPTION: Prints UDP header. For debugging purposes.
*************************************************************************/
#if FNET_CFG_DEBUG_TRACE_UDP && FNET_CFG_DEBUG_TRACE
static void fnet_udp_trace(fnet_uint8_t *str, fnet_udp_header_t *udp_hdr)
{

    fnet_printf(FNET_SERIAL_ESC_FG_GREEN"%s", str); /* Print app-specific header.*/
    fnet_println("[UDP header]"FNET_SERIAL_ESC_FG_BLACK);
    fnet_println("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+");
    fnet_println("|(SrcPort)                  "FNET_SERIAL_ESC_FG_BLUE"%3u"FNET_SERIAL_ESC_FG_BLACK" |(DestPort)                 "FNET_SERIAL_ESC_FG_BLUE"%3u"FNET_SERIAL_ESC_FG_BLACK" |",
                 fnet_ntohs(udp_hdr->source_port),
                 fnet_ntohs(udp_hdr->destination_port));
    fnet_println("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+");
    fnet_println("|(Lenghth)                  "FNET_SERIAL_ESC_FG_BLUE"%3u"FNET_SERIAL_ESC_FG_BLACK" |(Checksum)              0x%04X |",
                 fnet_ntohs(udp_hdr->length),
                 fnet_ntohs(udp_hdr->checksum));
    fnet_println("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+");
}
#endif /* FNET_CFG_DEBUG_TRACE_UDP */

#endif
