/***************************************************************************
*
* FNET User configuration file.
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
#define FNET_CFG_DHCP_CLN           (FNET_CPU_NETIF_NUMBER)

/*****************************************************************************
* Auto-IP service support.
******************************************************************************/
#define FNET_CFG_AUTOIP             (FNET_CPU_NETIF_NUMBER)

/*****************************************************************************
* HTTP Server service support.
******************************************************************************/
#define FNET_CFG_HTTP                       (1)
#define FNET_CFG_HTTP_AUTHENTICATION_BASIC  (1) /* Enable HTTP authentication.*/
#define FNET_CFG_HTTP_POST                  (1) /* Enable HTTP POST-method support.*/

/*****************************************************************************
* Telnet Server service support.
******************************************************************************/
#define FNET_CFG_TELNET                     (1)

/*****************************************************************************
* Flash Module driver support. It is valid only if a platform-specific 
* Flash driver is availble (FNET_CFG_CPU_FLASH).
******************************************************************************/
#define FNET_CFG_FLASH                      (1)

/*****************************************************************************
* DNS client/resolver service support.
******************************************************************************/
#define FNET_CFG_DNS                        (1)

/*****************************************************************************
* Link-Local Multicast Name Resolution (LLMNR) server/responder support.
******************************************************************************/
#define FNET_CFG_LLMNR                      (FNET_CPU_NETIF_NUMBER)
#define FNET_CFG_LLMNR_HOSTNAME_TTL         (2u)

/*****************************************************************************
* Multicast DNS (mDNS) "Bonjour" server/responder support.
******************************************************************************/
#define FNET_CFG_MDNS                       (FNET_CPU_NETIF_NUMBER)

/*****************************************************************************
* PING service support.
******************************************************************************/
#define FNET_CFG_PING                       (1)

/*****************************************************************************
* SNTP client support.
******************************************************************************/
#define FNET_CFG_SNTP                       (1)

/*****************************************************************************
* Link-Detection service support.
******************************************************************************/
#define FNET_CFG_LINK                       (FNET_CPU_NETIF_NUMBER)

/*****************************************************************************
* "Socket Rx" event
******************************************************************************/
#define FNET_CFG_SOCKET_CALLBACK_ON_RX      (1)

/*****************************************************************************
* DHCP Server service support.
******************************************************************************/
#define FNET_CFG_DHCP_SRV                   (1) /* Used mainly by Wi-Fi interface in Access Point mode. */

/*****************************************************************************
 * QCA4002 Wi-Fi module interface
 ******************************************************************************/
#define FNET_CFG_CPU_WIFI                   (1)
#define FNET_CFG_TIMER_POLL_AUTOMATIC       (0) /* Disable stack timer in interrupt. Required by the QCA driver, otherwise it causes assert during TX. */

#define FNET_CFG_DEBUG_QCA  1 /* QCA debug messages */
#define FNET_CFG_DEBUG      1

#if 0 /* You may disable Ethernet interface.*/
#define FNET_CFG_CPU_ETH0   (0)
#endif

#endif /* _FNET_USER_CONFIG_H_ */
