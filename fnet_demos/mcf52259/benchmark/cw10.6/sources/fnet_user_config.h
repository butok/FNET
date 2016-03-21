/**********************************************************************/ /*!
*
* @file fnet_user_config.h
*
* @brief FNET User configuration file.
* It should be used to change any default configuration parameter.
*
***************************************************************************/

#ifndef _FNET_USER_CONFIG_H_

#define _FNET_USER_CONFIG_H_


/*****************************************************************************
* Enable CW compiler support.
******************************************************************************/
#define FNET_CFG_COMP_CW            (1)

/*****************************************************************************
* Processor type.
* Selected processor definition should be only one and must be defined as 1.
* All others may be defined but must have 0 value.
******************************************************************************/
#define FNET_CFG_CPU_MCF52259       (1)

/*****************************************************************************
* IPv4 and/or IPv6 protocol support.
******************************************************************************/
#define FNET_CFG_IP4                (1)
#define FNET_CFG_IP6                (1)

/*****************************************************************************
* IP address for the Ethernet interface.
* At runtime it can be changed by the fnet_netif_set_address() or
* by the DHCP client service.
******************************************************************************/
#define FNET_CFG_ETH0_IP4_ADDR      (FNET_IP4_ADDR_INIT(192, 168, 0, 22))

/*****************************************************************************
* IP Subnet mask for the Ethernet interface.
* At runtime it can be changed by the fnet_netif_set_netmask() or
* by the DHCP client service.
******************************************************************************/
#define FNET_CFG_ETH0_IP4_MASK      (FNET_IP4_ADDR_INIT(255, 255, 255, 0))

/*****************************************************************************
* Gateway IP address for the Ethernet interface.
* At runtime it can be changed by the fnet_netif_set_gateway() or
* by the DHCP client service.
******************************************************************************/
#define FNET_CFG_ETH0_IP4_GW        (FNET_IP4_ADDR_INIT(0, 0, 0, 0))

/*****************************************************************************
* Size of the internal static heap buffer.
* This definition is used only if the fnet_init_static() was
* used for the FNET initialization.
******************************************************************************/
#define FNET_CFG_HEAP_SIZE          (35 * 1024)

/*****************************************************************************
* TCP protocol support.
* You can disable it to save a substantial amount of code if
* your application only needs UDP. By default it is enabled.
******************************************************************************/
#define FNET_CFG_TCP                (1)

/*****************************************************************************
* UDP protocol support.
* You can disable it to save a some amount of code if your
* application only needs TCP. By default it is enabled.
******************************************************************************/
#define FNET_CFG_UDP                (1)

/*****************************************************************************
* UDP checksum.
* If enabled, the UDP checksum will be generated for transmitted
* datagrams and be verified on received UDP datagrams.
* You can disable it to speedup UDP applications.
* By default it is enabled.
******************************************************************************/
#define FNET_CFG_UDP_CHECKSUM       (1)

/*****************************************************************************
* IP fragmentation.
* If the IP fragmentation is enabled, the IP will attempt to reassemble IP
* packet fragments and will able to generate fragmented IP packets.
* If disabled, the IP will  silently discard fragmented IP packets..
******************************************************************************/
#define FNET_CFG_IP4_FRAGMENTATION  (1)

/*****************************************************************************
* DHCP Client service support.
******************************************************************************/
#define FNET_CFG_DHCP               (1)

/*****************************************************************************
* HTTP Server service support.
******************************************************************************/
#define FNET_CFG_HTTP               (0)

/*****************************************************************************
* Telnet Server service support.
******************************************************************************/
#define FNET_CFG_TELNET             (0)

/*****************************************************************************
* Flash Module driver support.
******************************************************************************/
#define FNET_CFG_FLASH              (1)

/*****************************************************************************
* Multicast & IGMP & MLD support.
******************************************************************************/
#define FNET_CFG_IGMP               (1)
#define FNET_CFG_MLD                (1)


#ifdef TWRMCF52259 /*TWR-SER board port*/
    #define FNET_CFG_CPU_SERIAL_PORT_DEFAULT    (1)
#endif

#endif /* _FNET_USER_CONFIG_H_ */

