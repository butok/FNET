/**********************************************************************/
/*!
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
* Compiler support FNET_CFG_COMP_<name> is defined in project options.
******************************************************************************/
/*****************************************************************************
* Processor type FNET_CFG_CPU_<name> is defined in project options.
******************************************************************************/
/*****************************************************************************
*  Default serial port number FNET_CFG_CPU_SERIAL_PORT_DEFAULT is defined 
*  in project options.
******************************************************************************/

/*****************************************************************************
* IPv4 and/or IPv6 protocol support.
******************************************************************************/
#define FNET_CFG_IP4                (1)
#define FNET_CFG_IP6                (1)

/*****************************************************************************
* IP address for the Ethernet interface.
* At runtime it can be changed by the fnet_netif_set_ip4_addr() or
* by the DHCP client service.
******************************************************************************/
#define FNET_CFG_ETH0_IP4_ADDR      (FNET_IP4_ADDR_INIT(192U, 168U, 0U, 22U))

/*****************************************************************************
* IP Subnet mask for the Ethernet interface.
* At runtime it can be changed by the fnet_netif_set_ip4_subnet_mask() or
* by the DHCP client service.
******************************************************************************/
#define FNET_CFG_ETH0_IP4_MASK      (FNET_IP4_ADDR_INIT(255U, 255U, 255U, 0U))

/*****************************************************************************
* Gateway IP address for the Ethernet interface.
* At runtime it can be changed by the fnet_netif_set_ip4_gateway() or
* by the DHCP client service.
******************************************************************************/
#define FNET_CFG_ETH0_IP4_GW        (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))

/*****************************************************************************
* DNS server IP address for the Ethernet interface.
* At runtime it can be changed by the fnet_netif_set_ip4_dns() or
* by the DHCP client service.
* It is used only if FNET_CFG_DNS is set to 1.
******************************************************************************/
#define FNET_CFG_ETH0_IP4_DNS       (FNET_IP4_ADDR_INIT(0U, 0U, 0U, 0U))

/*****************************************************************************
* Size of the internal static heap buffer.
* This definition is used only if the fnet_init_static() was
* used for the FNET initialization.
******************************************************************************/
#define FNET_CFG_HEAP_SIZE          (30U * 1024U)

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
#define FNET_CFG_FLASH                      (1)

/*****************************************************************************
* Multicast & IGMP & MLD support.
******************************************************************************/
#define FNET_CFG_IGMP               (1)
#define FNET_CFG_MLD                (1)


#endif /* _FNET_USER_CONFIG_H_ */

