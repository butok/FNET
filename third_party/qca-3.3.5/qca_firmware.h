#ifndef __QCA_FIRMWARE_H__
#define __QCA_FIRMWARE_H__

/* AB: Revers-engineered from flashotp binary file*/
/****************** GT202/QCA4002 **********************************/
#if WIFISHIELD_IS_GT202 /* GT202/QCA4002 */

#include <stdint.h>

#if 1 /*ver 3.3.6*/
	#define QCA_FW_VERSION	        0x33060074
	#define QCA_FW_BUFFER_SIZE	    116877
#elif 1 /*ver 3.3.5*/
	#define QCA_FW_VERSION	        0x33050074
	#define QCA_FW_BUFFER_SIZE	    107768
#else /*ver 3.3.4 */
	#define QCA_FW_VERSION	        0x3304005B
	#define QCA_FW_BUFFER_SIZE	    102960
#endif

extern const uint8_t qca_firmware[QCA_FW_BUFFER_SIZE];	    /* QCA Firmware */
#define QCA_FW_BUFFER   qca_firmware

/****************** Silex2401/QCA4004 ********************************/
#elif WIFISHIELD_IS_SILEX2401

#include <stdint.h>

/* ver 3.3.4  Silex-specidic firmware (Not public QCA version)*/
#define QCA_FW_VERSION	        0x33048009
#define QCA_FW_BUFFER_SIZE	    109472

extern const uint8_t qca_firmware[QCA_FW_BUFFER_SIZE];	    /* QCA Firmware */
#define QCA_FW_BUFFER   qca_firmware

#else /* FW image not supported */

#define QCA_FW_BUFFER   0
#define QCA_FW_BUFFER_SIZE  0

#endif

#endif /* __QCA_FIRMWARE_H__ */
