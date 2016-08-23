/**************************************************************************
*
* Copyright 2016 by Andrey Butok. FNET Community.
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
#include "fnet.h"

#if FNET_CFG_AUTOIP && FNET_CFG_IP4

#if FNET_CFG_DEBUG_AUTOIP && FNET_CFG_DEBUG
    #define FNET_DEBUG_AUTOIP   FNET_DEBUG
#else
    #define FNET_DEBUG_AUTOIP(...)    do{}while(0)
#endif

/************************************************************************
*     Definitions
************************************************************************/

#define FNET_AUTOIP_ERR_IS_INITIALIZED    "ERROR: No free service."

/**************************************************************************/ /*!
 * @brief Constants defined in the section 9 of RFC3927 @n
 * They are  not intended to be user configurable.
 ******************************************************************************/
#define FNET_AUTOIP_PROBE_WAIT            (1)  /* second  (initial random delay) */
#define FNET_AUTOIP_PROBE_NUM             (3)  /* (number of probe packets) */
#define FNET_AUTOIP_PROBE_MIN             (1)  /* second  (minimum delay till repeated probe) */
#define FNET_AUTOIP_PROBE_MAX             (2)  /* seconds (maximum delay till repeated probe) */
#define FNET_AUTOIP_ANNOUNCE_WAIT         (2)  /* seconds (delay before announcing) */
#define FNET_AUTOIP_ANNOUNCE_NUM          (2)  /* (number of announcement packets) */
#define FNET_AUTOIP_ANNOUNCE_INTERVAL     (2)  /* seconds (time between announcement packets) */
#define FNET_AUTOIP_MAX_CONFLICTS         (10) /* (max conflicts before rate limiting) */
#define FNET_AUTOIP_RATE_LIMIT_INTERVAL   (6)  /* seconds  (delay between successive attempts). RFC says 60 secons, we decided to minimise it. */
#define FNET_AUTOIP_DEFEND_INTERVAL       (10) /* seconds  (minimum interval between defensive ARPs).*/

/**************************************************************************/ /*!
 * @brief Link-Local states.@n
 ******************************************************************************/
typedef enum
{
    FNET_AUTOIP_STATE_DISABLED = 0,     /* @brief The Link-Local service is not initialized */
    FNET_AUTOIP_STATE_INIT,             /* @brief The Link-Local service is
                                        * initialized. A random address is generated.*/
    FNET_AUTOIP_STATE_WAIT,             /* @brief Wait zero to PROBE_WAIT seconds*/
    FNET_AUTOIP_STATE_PROBE,            /* @brief Sends out @ref FNET_AUTOIP_PROBE_NUM
                                        * probes and checks whether the address
                                        * is in use by some other device*/
    FNET_AUTOIP_STATE_ANNOUNCE,         /* @brief Sends  out @ref FNET_AUTOIP_ANNOUNCE_NUM
                                        * frames to make itself known in the network*/
    /* The defend state is probably the same as BOUND Basically, we're not giving up the address*/
    FNET_AUTOIP_STATE_BOUND,            /* @brief The Link-Local address is set.*/
    FNET_AUTOIP_STATE_DEFEND_2,            /* @brief Host in DEFEND_INTERVAL.*/
    FNET_AUTOIP_STATE_DEFEND            /* @brief Host in DEFEND_INTERVAL.*/
} fnet_autoip_state_t;

/************************************************************************
*    AutoIP interface structure.
*************************************************************************/
typedef struct fnet_autoip_if
{
    fnet_autoip_state_t             state;
    fnet_netif_desc_t               netif;
    fnet_bool_t                     enabled;
    fnet_poll_desc_t                service_descriptor;
    fnet_ip4_addr_t                 ipaddr;
    fnet_time_t                     init_time;              /* Time when initialization started */
    fnet_time_t                     probe_time;             /* Time when probing started */
    fnet_time_t                     announce_time;          /* Time when the first announcement was sent */
    fnet_time_t                     wait_time;              /* How to long to wait until the next step */
    fnet_index_t                    probes;                 /* Number of probes sent */
    fnet_index_t                    announcements;          /* Number of announcements sent */
    fnet_index_t                    collisions;             /* Number of collisions detected */
#if FNET_CFG_AUTOIP_DEFEND_INTERVAL
    fnet_time_t                     collision_timestamp;
#endif
    fnet_bool_t                     reuse_addr;
    fnet_autoip_callback_t          callback_updated;        /* Optional ponter to the handler
                                                            * callback function, that is
                                                            * called when the Auto-IP service has
                                                            * updated the IP parameters.*/
    void                            *callback_updated_param; /* Optional user-application specific parameter.
                                                            * It's passed to callback_updated
                                                            * event handler callback.*/
    fnet_autoip_callback_t          callback_probe;        /* Optional ponter to the handler
                                                            * callback function, that is
                                                            * called when the Auto-IP service probes address.*/
    void                            *callback_probe_param; /* Optional user-application specific parameter.
                                                            * It's passed to callback_probe
                                                            * event handler callback.*/
} fnet_autoip_if_t;

static fnet_bool_t fnet_autoip_is_collision(fnet_autoip_if_t *llif);
static void fnet_autoip_announce(fnet_autoip_if_t *llif);
static void fnet_autoip_probe(fnet_autoip_if_t *llif);

/* The LLMNR Server interface */
static struct fnet_autoip_if fnet_autoip_if_list[FNET_CFG_AUTOIP_MAX];

/************************************************************************
* NAME: fnet_autoip_random_address
*
* DESCRIPTION: Returns a pseudo-random address in the 169.245/16 network
************************************************************************/
static fnet_ip4_addr_t fnet_autoip_generate_random_address(void)
{
    fnet_ip4_addr_t result;
    fnet_uint32_t random_value = fnet_rand();

    /* RFC 3927  : When a host wishes to configure an IPv4 Link-Local address, it
    * selects an address using a pseudo-random number generator with a
    * uniform distribution in the range from 169.254.1.0 to 169.254.254.255
    * inclusive. */
    result = FNET_AUTOIP_NETWORK | FNET_IP4_ADDR_INIT(0U, 0U, ((random_value >> 8) & 0xFE) + 1, random_value & 0xFF);
    return result;
}

/************************************************************************
* NAME: fnet_autoip_get_random_wait_time
*
* DESCRIPTION: Generates a random wait time between min and max
* (actually, it is not a time, but number of ticks)
************************************************************************/
static unsigned long fnet_autoip_get_random_wait_time(unsigned min, unsigned max)
{
    unsigned long result;
    result = fnet_rand();
    result %= (max - min) * FNET_TIMER_TICKS_IN_SEC;
    result += min * FNET_TIMER_TICKS_IN_SEC;
    return result;
}

/************************************************************************
* NAME: fnet_autoip_probe
*
* DESCRIPTION: Probes for IP address availability
* It sends an ARP request on the specified interface, having the fields
* set to the following values:
* - sender's hardware address: the hardware address of the interface
* - sender's protocol (IP) address: all zeros (INADDR_ANY)
* - target hardware address: all zeros (fnet_eth_null_addr)
* - target protocol address: the IP address being probed
************************************************************************/
static void fnet_autoip_probe(fnet_autoip_if_t *llif)
{
    /* TODO: The probes need to be sent with the IP source = 0; otherwise
     * they will pollute the arp caches on the other devices on the network.
     * Make sure that the IP address of the interface is still 0.0.0.0 (for
     * example, the DHCP client may have changed it in the meantime.
     */
    llif->probe_time = fnet_timer_ticks();
    llif->wait_time = fnet_autoip_get_random_wait_time(FNET_AUTOIP_PROBE_MIN, FNET_AUTOIP_PROBE_MAX);

    /* RFC 3927 : the term "ARP Probe" is used to refer to an ARP
    * Request packet, broadcast on the local link, with an all-zero 'sender
    * IP address'.  The 'sender hardware address' MUST contain the hardware
    * address of the interface sending the packet.  The 'target hardware
    * address' field is ignored and SHOULD be set to all zeroes.  The
    * 'target IP address' field MUST be set to the address being probed.*/
    fnet_arp_send_request(llif->netif, llif->ipaddr );

    /* Rise event. */
    if(llif->callback_probe)
    {
        llif->callback_probe((fnet_autoip_desc_t)llif, llif->netif, llif->callback_probe_param);
    }

    llif->probes++;
}

/************************************************************************
* NAME: fnet_autoip_announce
*
* DESCRIPTION: Claims an IP address
* It sends an ARP request on the specified interface, having the fields set
* as they were in the probe request, with the only exception that the sender
* protocol address is set to the address being announced.
************************************************************************/
static void fnet_autoip_announce(fnet_autoip_if_t *llif)
{
    /* TODO: Double check: is this gonna work?
     */
    llif->announce_time = fnet_timer_ticks();
    fnet_arp_send_request(llif->netif, llif->ipaddr );
    llif->announcements++;
}

/************************************************************************
* NAME: fnet_autoip_is_collision
*
* DESCRIPTION: Detects a conflict (somebody else is using the same IP address)
* It doesn't return anything but increments the conflicts field of llif
************************************************************************/
static fnet_bool_t fnet_autoip_is_collision(fnet_autoip_if_t *llif)
{
    fnet_bool_t  result;

    /* Check ARP cache.*/
    if((fnet_arp_get_mac(llif->netif, llif->ipaddr, FNET_NULL) == FNET_TRUE)
       || (fnet_netif_is_ip4_addr_conflict(llif->netif) == FNET_TRUE))
    {
        /* Address Conflict is detected.*/
        llif->collisions++;
        result = FNET_TRUE;
    }
    else
    {
        result = FNET_FALSE;
    }

    return result;
}

/************************************************************************
* NAME: fnet_autoip_apply_params
*
* DESCRIPTION: Apply Link-Local parameters to the interface.
************************************************************************/
static void fnet_autoip_apply_params(fnet_autoip_if_t *llif)
{
    fnet_netif_set_ip4_addr(llif->netif, llif->ipaddr);
    fnet_netif_set_ip4_subnet_mask(llif->netif, FNET_AUTOIP_NETMASK);

    fnet_netif_set_ip4_addr_type(llif->netif, FNET_NETIF_IP_ADDR_TYPE_AUTOCONFIGURABLE);

    /* Rise event. */
    if(llif->callback_updated)
    {
        llif->callback_updated((fnet_autoip_desc_t)llif, llif->netif, llif->callback_updated_param);
    }
}

/************************************************************************
* NAME: fnet_autoip_change_state
*
* DESCRIPTION: Change state of the Link-Local service.
************************************************************************/
static void fnet_autoip_change_state( fnet_autoip_if_t *llif, fnet_autoip_state_t state )
{
    llif->state = state;
    switch (state)
    {
        case FNET_AUTOIP_STATE_INIT:
            llif->init_time = fnet_timer_ticks();
            /* TODO: Is it a good idea to reset the IP address here?
             * What if the interface already has an IP address?
             * If that's the case, probably, the best thing to do is to quit. */
            fnet_netif_set_ip4_addr(llif->netif, INADDR_ANY); /* Set IP address to zero */
            break;
        case FNET_AUTOIP_STATE_WAIT:
            llif->wait_time = fnet_autoip_get_random_wait_time(0, FNET_AUTOIP_PROBE_WAIT);
            if(llif->collisions >= FNET_AUTOIP_MAX_CONFLICTS)
            {
                llif->wait_time += FNET_AUTOIP_RATE_LIMIT_INTERVAL * FNET_TIMER_TICKS_IN_SEC;
            }
            break;
        case FNET_AUTOIP_STATE_PROBE:
            llif->probes = 0;
            /* Send the first probe right away */
            fnet_autoip_probe(llif);
            break;
        case FNET_AUTOIP_STATE_ANNOUNCE:
            llif->collisions = 0; /* Reset collisions counter.*/
            fnet_autoip_apply_params(llif);
            /* Let the world know our IP address */
            fnet_autoip_announce(llif);
            break;
#if FNET_CFG_AUTOIP_DEFEND_INTERVAL
        case FNET_AUTOIP_STATE_DEFEND:
            /* Record the time when the conflicting ARP packet was received. */
            llif->collision_timestamp = fnet_timer_ticks();
            /* Clear conflict flag and broadcast ARP announcement.*/
            fnet_netif_clear_ip4_addr_conflict(llif->netif);
            fnet_autoip_announce(llif);
            break;
#endif
        case FNET_AUTOIP_STATE_DISABLED:
            llif->enabled = FNET_FALSE;
            fnet_poll_service_unregister(llif->service_descriptor); /* Delete service. */
            break;
        case FNET_AUTOIP_STATE_BOUND:
        default:
            break;
    }
}

static void fnet_autoip_state_machine( void *fnet_autoip_if_p )
{
    fnet_autoip_if_t *llif = (fnet_autoip_if_t *)fnet_autoip_if_p;

    switch (llif->state)
    {
        case FNET_AUTOIP_STATE_INIT:
            if(llif->reuse_addr == FNET_FALSE)
            {
                /* Generate new address.*/
                llif->ipaddr = fnet_autoip_generate_random_address();
            }
            else
            {
                /* re-use the previously assigned Link-Local address. */

                /* Reset flag.*/
                llif->reuse_addr = FNET_FALSE;
            }

            /* RFC 3927 : After it has selected an IPv4 Link-Local address, a host MUST test to
            * see if the IPv4 Link-Local address is already in use before beginning
            * to use it.*/
            fnet_autoip_change_state(llif, FNET_AUTOIP_STATE_WAIT);
            break;
        case FNET_AUTOIP_STATE_WAIT:
            if (fnet_timer_get_interval(llif->init_time, fnet_timer_ticks()) > llif->wait_time)
            {
                fnet_autoip_change_state(llif, FNET_AUTOIP_STATE_PROBE);
            }
            break;
        case FNET_AUTOIP_STATE_PROBE:
            /* RFC3927: Send out PROBE_NUM probes,
             * spaced randomly PROBE_MIN to PROBE_MAX seconds apart.
             * The first probe has already been sent on the state transition.
             */
            if (fnet_autoip_is_collision(llif) == FNET_TRUE)
            {
                fnet_autoip_change_state(llif, FNET_AUTOIP_STATE_INIT);  /* Restart */
            }
            else
            {
                /* No collisions detected yet */
                if (llif->probes >= FNET_AUTOIP_PROBE_NUM)
                {
                    /* We've sent all probes, move to the next state */
                    fnet_autoip_change_state(llif, FNET_AUTOIP_STATE_ANNOUNCE);
                }
                else
                {
                    /* There are still probes to be sent, after wait interval. */
                    if (fnet_timer_get_interval(llif->probe_time, fnet_timer_ticks()) > llif->wait_time)
                    {
                        fnet_autoip_probe(llif);
                    }
                }
            }
            break;
        case FNET_AUTOIP_STATE_ANNOUNCE:
            if (llif->announcements >= FNET_AUTOIP_ANNOUNCE_NUM)
            {
                fnet_autoip_change_state(llif, FNET_AUTOIP_STATE_BOUND);
            }
            else
            {
                if (fnet_netif_is_ip4_addr_conflict(llif->netif))
                {
                    fnet_autoip_change_state(llif, FNET_AUTOIP_STATE_INIT);
                }
                else if (fnet_timer_get_interval(llif->announce_time, fnet_timer_ticks())
                         > FNET_AUTOIP_ANNOUNCE_INTERVAL * FNET_TIMER_TICKS_IN_SEC)
                {
                    fnet_autoip_announce(llif);
                }
            }
            break;
        case FNET_AUTOIP_STATE_BOUND:
            if(fnet_netif_get_ip4_addr_type(llif->netif) != FNET_NETIF_IP_ADDR_TYPE_AUTOCONFIGURABLE) /* If user changed  address parameters manually or by DHCP.*/
            {
                fnet_autoip_change_state(llif, FNET_AUTOIP_STATE_DISABLED);
            }
            else if (fnet_netif_is_ip4_addr_conflict(llif->netif))
            {
#if FNET_CFG_AUTOIP_DEFEND_INTERVAL
                /* Upon receiving a conflicting ARP packet, if a host has not seen any
                 * other conflicting ARP packets within the last DEFEND_INTERVAL (10)
                 * seconds, it attempts to defend its address by
                 * recording the time that the conflicting ARP packet was received, and
                 * then broadcasting one single ARP announcement, giving its own IP and
                 * hardware addresses as the sender addresses of the ARP.  Having done
                 * this, the host can then continue to use the address normally without
                 * any further special action.  However, if this is not the first
                 * conflicting ARP packet the host has seen, and the time recorded for
                 * the previous conflicting ARP packet is recent, within DEFEND_INTERVAL
                 * seconds, then the host MUST immediately cease using this address and
                 * configure a new IPv4 Link-Local address as described above.  This is
                 * necessary to ensure that two hosts do not get stuck in an endless
                 * loop with both hosts trying to defend the same address.*/
                fnet_autoip_change_state(llif, FNET_AUTOIP_STATE_DEFEND);
#else
                /* Upon receiving a conflicting ARP packet, a host
                 * immediately configure a new IPv4 Link-Local address.*/
                fnet_autoip_change_state(llif, FNET_AUTOIP_STATE_INIT); /* Reset */
#endif
            }
            break;
#if FNET_CFG_AUTOIP_DEFEND_INTERVAL
        case FNET_AUTOIP_STATE_DEFEND:
            if(fnet_netif_get_ip4_addr_type(llif->netif) != FNET_NETIF_IP_ADDR_TYPE_AUTOCONFIGURABLE) /* If user changed  address parameters manually or by DHCP.*/
            {
                fnet_autoip_change_state(llif, FNET_AUTOIP_STATE_DISABLED);
            }
            else if (fnet_netif_is_ip4_addr_conflict(llif->netif))
            {
                /* The host MUST immediately cease using this address and
                 * configure a new IPv4 Link-Local address as described above.  This is
                 * necessary to ensure that two hosts do not get stuck in an endless
                 * loop with both hosts trying to defend the same address.*/
                fnet_autoip_change_state(llif, FNET_AUTOIP_STATE_INIT); /* Reset */
            }
            else if(fnet_timer_get_interval(llif->collision_timestamp, fnet_timer_ticks()) > (FNET_AUTOIP_DEFEND_INTERVAL * FNET_TIMER_TICKS_IN_SEC))
            {
                /* Return to bound state.*/
                fnet_autoip_change_state(llif, FNET_AUTOIP_STATE_BOUND);
            }
            break;
#endif
        case FNET_AUTOIP_STATE_DISABLED:
        default:
            break;
    }
}

/************************************************************************
* NAME: fnet_autoip_init
*
* DESCRIPTION: Auto-IP service initialization.
*
************************************************************************/
fnet_autoip_desc_t fnet_autoip_init ( struct fnet_autoip_params *params)
{
    fnet_autoip_if_t    *autoip_if = 0;
    fnet_index_t        i;

    if ((params == 0) || (params->netif_desc == 0))
    {
        goto ERROR;
    }

    /* Try to find free Auto-IP Service. */
    for(i = 0u; i < FNET_CFG_AUTOIP_MAX; i++)
    {
        if(fnet_autoip_if_list[i].enabled == FNET_FALSE)
        {
            autoip_if = &fnet_autoip_if_list[i];
        }
        else
        {
            if(fnet_autoip_if_list[i].netif == params->netif_desc)
            {
                /* It is not posible to run several Auto-IP services on the same networking interface.*/
                autoip_if = 0;
                break;
            }
        }
    }

    /* Is Auto-IP service already initialized. */
    if(autoip_if == 0)
    {
        FNET_DEBUG_AUTOIP(FNET_AUTOIP_ERR_IS_INITIALIZED);
        goto ERROR;
    }

    /* set all the fields to zero */
    fnet_memset_zero(autoip_if, sizeof(fnet_autoip_if_t));

    autoip_if->netif = params->netif_desc;

    autoip_if->service_descriptor = fnet_poll_service_register(fnet_autoip_state_machine, (void *) autoip_if);
    if (autoip_if->service_descriptor == 0)
    {
        goto ERROR;
    }

    /* Check if the proposed address has the link-local mask*/
    if((params->ip_address & FNET_AUTOIP_NETMASK) == FNET_AUTOIP_NETWORK)
    {
        autoip_if->reuse_addr = FNET_TRUE;
        autoip_if->ipaddr = params->ip_address;
    }

    fnet_autoip_change_state(autoip_if, FNET_AUTOIP_STATE_INIT);

    autoip_if->enabled = FNET_TRUE;

    return (fnet_autoip_desc_t)autoip_if;

ERROR:
    return 0;
}

/************************************************************************
* NAME: fnet_autoip_release
*
* DESCRIPTION: Auto-IP service release.
************************************************************************/
void fnet_autoip_release( fnet_autoip_desc_t desc )
{
    struct fnet_autoip_if   *autoip_if = (struct fnet_autoip_if *) desc;

    if (autoip_if && (autoip_if->enabled == FNET_TRUE))
    {
        fnet_autoip_change_state(autoip_if, FNET_AUTOIP_STATE_DISABLED);
        fnet_autoip_state_machine(autoip_if); /* 1 pass. */
    }
}

/************************************************************************
* NAME: fnet_autoip_set_callback_updated
*
* DESCRIPTION: Registers the "IPv4 parameters updated" Auto-IP event handler callback.
************************************************************************/
void fnet_autoip_set_callback_updated (fnet_autoip_desc_t desc, fnet_autoip_callback_t callback_updated, void *param)
{
    struct fnet_autoip_if   *autoip_if = (struct fnet_autoip_if *) desc;

    if(autoip_if)
    {
        autoip_if->callback_updated = callback_updated;
        autoip_if->callback_updated_param = param;
    }
}

/************************************************************************
* NAME: fnet_autoip_set_callback_probe
*
* DESCRIPTION: Registers the "Address probing" Auto-IP event handler callback.
************************************************************************/
void fnet_autoip_set_callback_probe (fnet_autoip_desc_t desc, fnet_autoip_callback_t callback_probe, void *param)
{
    struct fnet_autoip_if   *autoip_if = (struct fnet_autoip_if *) desc;

    if(autoip_if)
    {
        autoip_if->callback_probe = callback_probe;
        autoip_if->callback_probe_param = param;
    }
}

/************************************************************************
* NAME: fnet_autoip_is_enabled
*
* DESCRIPTION: This function returns FNET_TRUE if the Auto-IP service
*              is enabled/initialised.
************************************************************************/
fnet_bool_t fnet_autoip_is_enabled(fnet_autoip_desc_t desc)
{
    struct fnet_autoip_if   *autoip_if = (struct fnet_autoip_if *) desc;
    fnet_bool_t             result;

    if(autoip_if)
    {
        result = autoip_if->enabled;
    }
    else
    {
        result = FNET_FALSE;
    }

    return result;
}

#endif /* FNET_CFG_AUTOIP && FNET_CFG_IP4 */
