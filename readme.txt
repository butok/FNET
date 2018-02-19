FNET TCP/IP Stack.

Please read the FNET Quick Start chapter. You can find it in the /fnet_doc/fnet_user_manual.chm file.

FNET project directory structure:
	fnet                        - FNET root directory.
	|-- fnet_stack                - FNET TCP/IP stack.
	|    |-- stack                  - FNET TCP/IP stack platform-independent source code.
	|    |-- service                - FNET Services.
	|    |    |-- autoip              - Auto-IP service.
	|    |    |-- bench               - Benchmark client and server.
	|    |    |-- dhcp                - DHCPv4 client and server.
	|    |    |-- dns                 - DNS client/resolver.
	|    |    |-- flash               - Flash Memory driver.
	|    |    |-- fs                  - File System driver.
	|    |    |-- http                - HTTP Server service.
	|    |    |-- link                - Link-Detection service.
	|    |    |-- llmnr               - LLMNR server/responder.
	|    |    |-- mdns                - mDNS server/responder.
	|    |    |-- ping                - PING service.
	|    |    |-- poll                - Polling Mechanism library.
	|    |    |-- serial              - Serial Input/Output driver.
	|    |    |-- shell               - Command Shell service.
	|    |    |-- sntp                - SNTP client.
	|    |    |-- telnet              - TELNET server.
	|    |    |-- tftp                - TFTP server and client.
	|    |    |-- tls                 - TLS library.
	|    |-- port                   - FNET port-specific source code.
	|         |-- cpu                 - FNET platform-specific source code.
	|         |    |-- netif            - Platform-specific network interface drivers.
	|         |    |    |-- fec           - FEC/ENET Ethernet module driver.
	|         |    |    |-- qca           - QCA4002 Wi-Fi module driver, using "third_party/qca-x.x.x".
	|         |    |-- mcf              - ColdFire-specific source code.
	|         |    |-- mk               - Kinetis-specific source code.
	|         |    |-- mpc              - Power Architecture-specific source code.
	|         |-- compiler            - Compiler-specific source code.
	|-- fnet_doc                  - FNET documentation.
	|-- fnet_demos                - FNET demo projects.
	|    |-- common                 - Common source code used by demos.
	|    |    |-- fnet_application    - Demo application source code.
	|    |    |-- fnet_webpage        - Demo web page source code.
	|    |    |-- startup             - Platform-specific startup source code.
	|    |-- boards                 - Demo projects.
	|         |-- <board_name>        - Demo projects for a <board_name> board.
	|              |-- boot           - FNET TFTP Bootloader.
	|              |-- benchmark      - TCP/UDP Throughput Benchmark application. 
	|              |-- shell          - "Shell" demo is a fully featured shell. It allows
	|              |                    view/change various parameters (e.g. IP address,
	|              |                    subnet mask), explore mounted file systems, run 
	|              |                    DHCP client, HTTP and Telnet servers.
	|              |-- shell_boot     - Example application used for the FNET TFTP Bootloader
	|              |                    demonstration. This demo has the same features as
	|              |                    the "Shell" demo, but it`s modified to be able to
	|              |                    work with the FNET Bootloader.
	|              |-- shell_mbedtls  - Example application used for the HTTPS server demonstration.
	|              |                    This demo has the same features as the "Shell" demo,
	|              |                    but in addition it includes the mbedTLS library.
	|              |-- shell_freertos - Example application using FreeRTOS.
	|              |                    This demo has the same features as the "Shell" demo.
	|              |-- shell_freertos_wifi_qca - Example application used for the Wi-Fi interface demonstration.
	|                                           It has the same features as the "Shell FreeRTOS" demo,
	|                                           but in addition it supports the Wi-Fi network interface using the Qualcomm QCA4002 module (GT202 board).
	|-- fnet_tools                - FNET PC tools.
	|-- third_party               - Optional, third-party libraries.
		 |-- mbedtls-x.x.x          - mbedTLS library, used by FNET TLS.
		 |-- freertos-x.x.x         - FreeRTOS, used by the shell_freertos example.
		 |-- qca-x.x.x              - Qualcomm QCA4002/4 driver, used by shell_freertos_wifi_qca example.
