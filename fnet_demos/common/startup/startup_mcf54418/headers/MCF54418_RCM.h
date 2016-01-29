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

#ifndef __MCF54418_RCM_H__
#define __MCF54418_RCM_H__


/*********************************************************************
*
* Reset Controller Module (RCM)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_RCM_RCR                          (*(vuint8 *)(0xEC090000))
#define MCF_RCM_RSR                          (*(vuint8 *)(0xEC090001))


/* Bit definitions and macros for MCF_RCM_RCR */
#define MCF_RCM_RCR_FRCRSTOUT                (0x40)
#define MCF_RCM_RCR_SOFTRST                  (0x80)

/* Bit definitions and macros for MCF_RCM_RSR */
#define MCF_RCM_RSR_LOL                      (0x1)
#define MCF_RCM_RSR_WDRCORE                  (0x2)
#define MCF_RCM_RSR_EXT                      (0x4)
#define MCF_RCM_RSR_POR                      (0x8)
#define MCF_RCM_RSR_LOC                      (0x10)
#define MCF_RCM_RSR_SOFT                     (0x20)


#endif /* __MCF54418_RCM_H__ */
