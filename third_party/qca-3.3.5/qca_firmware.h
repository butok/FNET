#ifndef __QCA_FIRMWARE_H__
#define __QCA_FIRMWARE_H__

#include <stdint.h>

/* AB: Revers-engineered from flashotp binary file*/
#if 1 /*ver 3.3.5*/
	#define QCA_FW_VERSION	        0x33050074
	#define QCA_FW_BUFFER_SIZE	    107768
#else /*ver 3.3.4 */
	#define QCA_FW_VERSION	        0x3304005B
	#define QCA_FW_BUFFER_SIZE	    102960
#endif

/* AB: Revers-engineered */
#if 0 /* SPI Flash 4 bits  */
    #define QCA_FW_4BITFLASH	    1
#else /* SPI Flash 1 bits  */
    #define QCA_FW_4BITFLASH	    0
#endif


extern const uint8_t qca_firmware[QCA_FW_BUFFER_SIZE];	    /* QCA Firmware */

#define QCA_FW_BUFFER   qca_firmware

#endif /* __QCA_FIRMWARE_H__ */
