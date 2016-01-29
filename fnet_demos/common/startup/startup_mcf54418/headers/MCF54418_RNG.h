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

#ifndef __MCF54418_RNG_H__
#define __MCF54418_RNG_H__


/*********************************************************************
*
* Random Number Generator (RNG)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_RNG_RNGVER                       (*(vuint32*)(0xFC0C4000))
#define MCF_RNG_RNGCMD                       (*(vuint32*)(0xFC0C4004))
#define MCF_RNG_RNGCR                        (*(vuint32*)(0xFC0C4008))
#define MCF_RNG_RNGSR                        (*(vuint32*)(0xFC0C400C))
#define MCF_RNG_RNGESR                       (*(vuint32*)(0xFC0C4010))
#define MCF_RNG_RNGOUT                       (*(vuint32*)(0xFC0C4014))
#define MCF_RNG_RNGER                        (*(vuint32*)(0xFC0C4018))


/* Bit definitions and macros for MCF_RNG_RNGVER */
#define MCF_RNG_RNGVER_MINOR(x)              (((x)&0xFF)<<0)
#define MCF_RNG_RNGVER_MAJOR(x)              (((x)&0xFF)<<0x8)
#define MCF_RNG_RNGVER_TYPE(x)               (((x)&0xF)<<0x1C)

/* Bit definitions and macros for MCF_RNG_RNGCMD */
#define MCF_RNG_RNGCMD_ST                    (0x1)
#define MCF_RNG_RNGCMD_GS                    (0x2)
#define MCF_RNG_RNGCMD_CI                    (0x10)
#define MCF_RNG_RNGCMD_CE                    (0x20)
#define MCF_RNG_RNGCMD_SR                    (0x40)

/* Bit definitions and macros for MCF_RNG_RNGCR */
#define MCF_RNG_RNGCR_FUFMOD(x)              (((x)&0x3)<<0)
#define MCF_RNG_RNGCR_AR                     (0x10)
#define MCF_RNG_RNGCR_MSKDN                  (0x20)
#define MCF_RNG_RNGCR_MSKERR                 (0x40)

/* Bit definitions and macros for MCF_RNG_RNGSR */
#define MCF_RNG_RNGSR_BUSY                   (0x2)
#define MCF_RNG_RNGSR_SLP                    (0x4)
#define MCF_RNG_RNGSR_RS                     (0x8)
#define MCF_RNG_RNGSR_STDN                   (0x10)
#define MCF_RNG_RNGSR_SDN                    (0x20)
#define MCF_RNG_RNGSR_NSDN                   (0x40)
#define MCF_RNG_RNGSR_FL(x)                  (((x)&0xF)<<0x8)
#define MCF_RNG_RNGSR_FS(x)                  (((x)&0xF)<<0xC)
#define MCF_RNG_RNGSR_ERR                    (0x10000)
#define MCF_RNG_RNGSR_SELFPF(x)              (((x)&0x3)<<0x16)
#define MCF_RNG_RNGSR_STATPF(x)              (((x)&0xFF)<<0x18)

/* Bit definitions and macros for MCF_RNG_RNGESR */
#define MCF_RNG_RNGESR_LFE                   (0x1)
#define MCF_RNG_RNGESR_OSCE                  (0x2)
#define MCF_RNG_RNGESR_STE                   (0x4)
#define MCF_RNG_RNGESR_SATE                  (0x8)
#define MCF_RNG_RNGESR_FUF                   (0x10)

/* Bit definitions and macros for MCF_RNG_RNGOUT */
#define MCF_RNG_RNGOUT_RANDOM_OUTPUT(x)      (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_RNG_RNGER */
#define MCF_RNG_RNGER_ENT(x)                 (((x)&0xFFFFFFFF)<<0)


#endif /* __MCF54418_RNG_H__ */
