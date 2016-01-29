/********************************************************************/
/* Coldfire C Header File
 *
 *     Date      : 2010/07/19
 *     Revision  : 0.4
 *
 *     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 *     http      : www.freescale.com
 *     mail      : support@freescale.com
 */

#ifndef __MCF54418_H__
#define __MCF54418_H__


/********************************************************************/
/*
 * The basic data types
 */

typedef unsigned char           uint8;   /*  8 bits */
typedef unsigned short int      uint16;  /* 16 bits */
typedef unsigned long int       uint32;  /* 32 bits */

typedef signed char             int8;    /*  8 bits */
typedef signed short int        int16;   /* 16 bits */
typedef signed long int         int32;   /* 32 bits */

typedef volatile uint8          vuint8;  /*  8 bits */
typedef volatile uint16         vuint16; /* 16 bits */
typedef volatile uint32         vuint32; /* 32 bits */

#ifdef __cplusplus
extern "C" {
#endif

#pragma define_section system ".system" far_absolute RW

/***
 * MCF54418 Derivative Memory map definitions from linker command files:
 * __MMUBAR, __RAMBAR, __RAMBAR_SIZE linker symbols must be defined in the
 * linker command file.
 */

extern __declspec(system)  uint8 __MMUBAR[];
extern __declspec(system)  uint8 __RAMBAR[];
extern __declspec(system)  uint8 __RAMBAR_SIZE[];

#define MMUBAR_ADDRESS   (uint32)__MMUBAR
#define RAMBAR_ADDRESS   (uint32)__RAMBAR
#define RAMBAR_SIZE      (uint32)__RAMBAR_SIZE


#include "MCF54418_MMU.h"
#include "MCF54418_RGPIO.h"
#include "MCF54418_OW.h"
#include "MCF54418_I2C.h"
#include "MCF54418_DSPI.h"
#include "MCF54418_UART.h"
#include "MCF54418_PWM.h"
#include "MCF54418_RCM.h"
#include "MCF54418_CCM.h"
#include "MCF54418_GPIO.h"
#include "MCF54418_PAD.h"
#include "MCF54418_SCM.h"
#include "MCF54418_XBS.h"
#include "MCF54418_FBCS.h"
#include "MCF54418_FlexCAN.h"
#include "MCF54418_CANMB.h"
#include "MCF54418_PMM.h"
#include "MCF54418_eDMA.h"
#include "MCF54418_INTC.h"
#include "MCF54418_GIACR.h"
#include "MCF54418_DTIM.h"
#include "MCF54418_PIT.h"
#include "MCF54418_EPORT.h"
#include "MCF54418_ADC.h"
#include "MCF54418_DAC.h"
#include "MCF54418_SBF.h"
#include "MCF54418_RTC.h"
#include "MCF54418_SIM.h"
#include "MCF54418_USB_OTG.h"
#include "MCF54418_USB_HOST.h"
#include "MCF54418_DDR.h"
#include "MCF54418_SSI.h"
#include "MCF54418_CLOCK.h"
#include "MCF54418_RNG.h"
#include "MCF54418_eSDHC.h"
#include "MCF54418_ENET.h"
#include "MCF54418_ESW.h"
#include "MCF54418_NFC.h"

#ifdef __cplusplus
}
#endif


#endif /* __MCF54418_H__ */
