/**************************************************************************
*
* Copyright 2005-2015 by Andrey Butok. Freescale Semiconductor, Inc.
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
**********************************************************************/ /*!
*
* @file fnet_mcf.h
*
* @author Andrey Butok
*
* @brief Private. ColdFire Peripheral Registers definitions.
*
***************************************************************************/

#ifndef _FNET_MCF_H_

#define _FNET_MCF_H_

#include "fnet.h"

#if FNET_MCF
/*********************************************************************
*
* The basic data types.
*
*********************************************************************/
typedef unsigned char fnet_uint8_t;       /*  8 bits */

typedef unsigned short int fnet_uint16_t; /* 16 bits */
typedef unsigned long int fnet_uint32_t;  /* 32 bits */

typedef signed char fnet_int8_t;          /*  8 bits */
typedef signed short int fnet_int16_t;    /* 16 bits */
typedef signed long int fnet_int32_t;     /* 32 bits */

typedef volatile fnet_uint8_t fnet_vuint8_t;     /*  8 bits */
typedef volatile fnet_uint16_t fnet_vuint16_t;   /* 16 bits */
typedef volatile fnet_uint32_t fnet_vuint32_t;   /* 32 bits */


/*********************************************************************
*
* MCF ASM utility functions
*
*********************************************************************/
void fnet_mcf_vbr_wr(fnet_uint32_t);
void fnet_mcf_cacr_wr(fnet_uint32_t);
fnet_uint32_t fnet_mcf_sp_rd(void);
fnet_uint16_t fnet_mcf_sr_rd(void);
void fnet_mcf_sr_wr(fnet_uint16_t);
void fnet_mcf_illegal(void);
void fnet_mcf_nop(void);


/*********************************************************************
*
* Common M68K & ColdFire definitions.
*
*********************************************************************/
#define FNET_MCF_SR_T           (0x8000)
#define FNET_MCF_SR_S           (0x2000)
#define FNET_MCF_SR_M           (0x1000)
#define FNET_MCF_SR_IPL         (0x0700)
#define FNET_MCF_SR_IPL_0       (0x0000)
#define FNET_MCF_SR_IPL_1       (0x0100)
#define FNET_MCF_SR_IPL_2       (0x0200)
#define FNET_MCF_SR_IPL_3       (0x0300)
#define FNET_MCF_SR_IPL_4       (0x0400)
#define FNET_MCF_SR_IPL_5       (0x0500)
#define FNET_MCF_SR_IPL_6       (0x0600)
#define FNET_MCF_SR_IPL_7       (0x0700)
#define FNET_MCF_SR_X           (0x0010)
#define FNET_MCF_SR_N           (0x0008)
#define FNET_MCF_SR_Z           (0x0004)
#define FNET_MCF_SR_V           (0x0002)
#define FNET_MCF_SR_C           (0x0001)

#define FNET_MCF_CACR_CENB      (0x80000000)
#define FNET_MCF_CACR_CPDI      (0x10000000)
#define FNET_MCF_CACR_CPD       (0x10000000)
#define FNET_MCF_CACR_CFRZ      (0x08000000)
#define FNET_MCF_CACR_CINV      (0x01000000)
#define FNET_MCF_CACR_DIDI      (0x00800000)
#define FNET_MCF_CACR_DISD      (0x00400000)
#define FNET_MCF_CACR_INVI      (0x00200000)
#define FNET_MCF_CACR_INVD      (0x00100000)
#define FNET_MCF_CACR_CEIB      (0x00000400)
#define FNET_MCF_CACR_DCM_WR    (0x00000000)
#define FNET_MCF_CACR_DCM_CB    (0x00000100)
#define FNET_MCF_CACR_DCM_IP    (0x00000200)
#define FNET_MCF_CACR_DCM       (0x00000200)
#define FNET_MCF_CACR_DCM_II    (0x00000300)
#define FNET_MCF_CACR_DBWE      (0x00000100)
#define FNET_MCF_CACR_DWP       (0x00000020)
#define FNET_MCF_CACR_EUST      (0x00000010)
#define FNET_MCF_CACR_CLNF_00   (0x00000000)
#define FNET_MCF_CACR_CLNF_01   (0x00000002)
#define FNET_MCF_CACR_CLNF_10   (0x00000004)
#define FNET_MCF_CACR_CLNF_11   (0x00000006)

#define FNET_MCF_ACR_AB(a)      ((a)&0xFF000000)
#define FNET_MCF_ACR_AM(a)      (((a)&0xFF000000) >> 8)
#define FNET_MCF_ACR_EN         (0x00008000)
#define FNET_MCF_ACR_SM_USER    (0x00000000)
#define FNET_MCF_ACR_SM_SUPER   (0x00002000)
#define FNET_MCF_ACR_SM_IGNORE  (0x00006000)
#define FNET_MCF_ACR_ENIB       (0x00000080)
#define FNET_MCF_ACR_CM         (0x00000040)
#define FNET_MCF_ACR_DCM_WR     (0x00000000)
#define FNET_MCF_ACR_DCM_CB     (0x00000020)
#define FNET_MCF_ACR_DCM_IP     (0x00000040)
#define FNET_MCF_ACR_DCM_II     (0x00000060)
#define FNET_MCF_ACR_CM         (0x00000040)
#define FNET_MCF_ACR_BWE        (0x00000020)
#define FNET_MCF_ACR_WP         (0x00000004)

#define FNET_MCF_RAMBAR_BA(a)   ((a)&0xFFFFC000)
#define FNET_MCF_RAMBAR_PRI_00  (0x00000000)
#define FNET_MCF_RAMBAR_PRI_01  (0x00004000)
#define FNET_MCF_RAMBAR_PRI_10  (0x00008000)
#define FNET_MCF_RAMBAR_PRI_11  (0x0000C000)
#define FNET_MCF_RAMBAR_WP      (0x00000100)
#define FNET_MCF_RAMBAR_CI      (0x00000020)
#define FNET_MCF_RAMBAR_SC      (0x00000010)
#define FNET_MCF_RAMBAR_SD      (0x00000008)
#define FNET_MCF_RAMBAR_UC      (0x00000004)
#define FNET_MCF_RAMBAR_UD      (0x00000002)
#define FNET_MCF_RAMBAR_V       (0x00000001)


/*********************************************************************
*
* Fast Ethernet Controller (FEC)
*
*********************************************************************/

/* Register read/write macros */
#if FNET_CFG_CPU_MCF54418

#else /* Other MCFs */
    #define FNET_FEC0_BASE_ADDR                   ((fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001004]))
#endif

#if 0 /* Only for reference */
    #define FNET_MCF_FEC_EIMR                  (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001008]))
    #define FNET_MCF_FEC_RDAR                  (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001010]))
    #define FNET_MCF_FEC_TDAR                  (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001014]))
    #define FNET_MCF_FEC_ECR                   (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001024]))
    #define FNET_MCF_FEC_MMFR                  (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001040]))
    #define FNET_MCF_FEC_MSCR                  (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001044]))
    #define FNET_MCF_FEC_MIBC                  (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001064]))
    #define FNET_MCF_FEC_RCR                   (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001084]))
    #define FNET_MCF_FEC_TCR                   (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0010C4]))
    #define FNET_MCF_FEC_PALR                  (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0010E4]))
    #define FNET_MCF_FEC_PAUR                  (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0010E8]))
    #define FNET_MCF_FEC_OPD                   (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0010EC]))
    #define FNET_MCF_FEC_IAUR                  (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001118]))
    #define FNET_MCF_FEC_IALR                  (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x00111C]))
    #define FNET_MCF_FEC_GAUR                  (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001120]))
    #define FNET_MCF_FEC_GALR                  (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001124]))
    #define FNET_MCF_FEC_TFWR                  (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001144]))
    #define FNET_MCF_FEC_FRBR                  (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x00114C]))
    #define FNET_MCF_FEC_FRSR                  (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001150]))
    #define FNET_MCF_FEC_ERDSR                 (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001180]))
    #define FNET_MCF_FEC_ETDSR                 (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001184]))
    #define FNET_MCF_FEC_EMRBR                 (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001188]))
    #define FNET_MCF_FEC_RMON_T_DROP           (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001200]))
    #define FNET_MCF_FEC_RMON_T_PACKETS        (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001204]))
    #define FNET_MCF_FEC_RMON_T_BC_PKT         (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001208]))
    #define FNET_MCF_FEC_RMON_T_MC_PKT         (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x00120C]))
    #define FNET_MCF_FEC_RMON_T_CRC_ALIGN      (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001210]))
    #define FNET_MCF_FEC_RMON_T_UNDERSIZE      (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001214]))
    #define FNET_MCF_FEC_RMON_T_OVERSIZE       (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001218]))
    #define FNET_MCF_FEC_RMON_T_FRAG           (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x00121C]))
    #define FNET_MCF_FEC_RMON_T_JAB            (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001220]))
    #define FNET_MCF_FEC_RMON_T_COL            (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001224]))
    #define FNET_MCF_FEC_RMON_T_P64            (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001228]))
    #define FNET_MCF_FEC_RMON_T_P65TO127       (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x00122C]))
    #define FNET_MCF_FEC_RMON_T_P128TO255      (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001230]))
    #define FNET_MCF_FEC_RMON_T_P256TO511      (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001234]))
    #define FNET_MCF_FEC_RMON_T_P512TO1023     (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001238]))
    #define FNET_MCF_FEC_RMON_T_P1024TO2047    (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x00123C]))
    #define FNET_MCF_FEC_RMON_T_P_GTE2048      (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001240]))
    #define FNET_MCF_FEC_RMON_T_OCTETS         (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001244]))
    #define FNET_MCF_FEC_IEEE_T_DROP           (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001248]))
    #define FNET_MCF_FEC_IEEE_T_FRAME_OK       (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x00124C]))
    #define FNET_MCF_FEC_IEEE_T_1COL           (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001250]))
    #define FNET_MCF_FEC_IEEE_T_MCOL           (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001254]))
    #define FNET_MCF_FEC_IEEE_T_DEF            (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001258]))
    #define FNET_MCF_FEC_IEEE_T_LCOL           (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x00125C]))
    #define FNET_MCF_FEC_IEEE_T_EXCOL          (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001260]))
    #define FNET_MCF_FEC_IEEE_T_MACERR         (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001264]))
    #define FNET_MCF_FEC_IEEE_T_CSERR          (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001268]))
    #define FNET_MCF_FEC_IEEE_T_SQE            (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x00126C]))
    #define FNET_MCF_FEC_IEEE_T_FDXFC          (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001270]))
    #define FNET_MCF_FEC_IEEE_T_OCTETS_OK      (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001274]))
    #define FNET_MCF_FEC_RMON_R_PACKETS        (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001284]))
    #define FNET_MCF_FEC_RMON_R_BC_PKT         (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001288]))
    #define FNET_MCF_FEC_RMON_R_MC_PKT         (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x00128C]))
    #define FNET_MCF_FEC_RMON_R_CRC_ALIGN      (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001290]))
    #define FNET_MCF_FEC_RMON_R_UNDERSIZE      (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001294]))
    #define FNET_MCF_FEC_RMON_R_OVERSIZE       (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x001298]))
    #define FNET_MCF_FEC_RMON_R_FRAG           (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x00129C]))
    #define FNET_MCF_FEC_RMON_R_JAB            (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0012A0]))
    #define FNET_MCF_FEC_RMON_R_RESVD_0        (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0012A4]))
    #define FNET_MCF_FEC_RMON_R_P64            (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0012A8]))
    #define FNET_MCF_FEC_RMON_R_P65TO127       (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0012AC]))
    #define FNET_MCF_FEC_RMON_R_P128TO255      (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0012B0]))
    #define FNET_MCF_FEC_RMON_R_P256TO511      (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0012B4]))
    #define FNET_MCF_FEC_RMON_R_512TO1023      (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0012B8]))
    #define FNET_MCF_FEC_RMON_R_P_GTE2048      (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0012C0]))
    #define FNET_MCF_FEC_RMON_R_1024TO2047     (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0012BC]))
    #define FNET_MCF_FEC_RMON_R_OCTETS         (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0012C4]))
    #define FNET_MCF_FEC_IEEE_R_DROP           (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0012C8]))
    #define FNET_MCF_FEC_IEEE_R_FRAME_OK       (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0012CC]))
    #define FNET_MCF_FEC_IEEE_R_CRC            (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0012D0]))
    #define FNET_MCF_FEC_IEEE_R_ALIGN          (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0012D4]))
    #define FNET_MCF_FEC_IEEE_R_MACERR         (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0012D8]))
    #define FNET_MCF_FEC_IEEE_R_FDXFC          (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0012DC]))
    #define FNET_MCF_FEC_IEEE_R_OCTETS_OK      (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x0012E0]))
#endif /* 0 */



/*********************************************************************
*
* Ethernet Physical Transceiver (EPHY)
*
*********************************************************************/
/* Register read/write macros */
#if FNET_CFG_CPU_MCF52235
    #define FNET_MCF_EPHY_EPHYCTL0                    (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x1E0000]))
    #define FNET_MCF_EPHY_EPHYCTL1                    (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x1E0001]))
    #define FNET_MCF_EPHY_EPHYSR                      (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x1E0002]))
#endif


/* Bit definitions and macros for MCF_EPHY_EPHYCTL0 */
#define FNET_MCF_EPHY_EPHYCTL0_EPHYIEN            (0x1)
#define FNET_MCF_EPHY_EPHYCTL0_EPHYWAI            (0x4)
#define FNET_MCF_EPHY_EPHYCTL0_LEDEN              (0x8)
#define FNET_MCF_EPHY_EPHYCTL0_DIS10              (0x10)
#define FNET_MCF_EPHY_EPHYCTL0_DIS100             (0x20)
#define FNET_MCF_EPHY_EPHYCTL0_ANDIS              (0x40)
#define FNET_MCF_EPHY_EPHYCTL0_EPHYEN             (0x80)

/* Bit definitions and macros for MCF_EPHY_EPHYCTL1 */
#define FNET_MCF_EPHY_EPHYCTL1_PHYADD(x)          (((x)&0x1F)<<0)

/* Bit definitions and macros for MCF_EPHY_EPHYSR */
#define FNET_MCF_EPHY_EPHYSR_EPHYIF               (0x1)
#define FNET_MCF_EPHY_EPHYSR_10DIS                (0x10)
#define FNET_MCF_EPHY_EPHYSR_100DIS               (0x20)

/********************************************************************/

/*********************************************************************
*
* Interrupt Controller 0 (INTC0)
*
*********************************************************************/

/* Register read/write macros */
#if FNET_CFG_CPU_MCF54418

    #define FNET_MCF_INTC0_IPRH                       (*(fnet_vuint32_t*)(0xFC048000))
    #define FNET_MCF_INTC0_IPRL                       (*(fnet_vuint32_t*)(0xFC048004))
    #define FNET_MCF_INTC0_IMRH                       (*(fnet_vuint32_t*)(0xFC048008))
    #define FNET_MCF_INTC0_IMRL                       (*(fnet_vuint32_t*)(0xFC04800C))
    #define FNET_MCF_INTC0_IMR(x)                     (*(fnet_vuint32_t*)(0xFC04800C-((x)*0x4)))
    #define FNET_MCF_INTC0_INTFRCH                    (*(fnet_vuint32_t*)(0xFC048010))
    #define FNET_MCF_INTC0_INTFRCL                    (*(fnet_vuint32_t*)(0xFC048014))
    #define FNET_MCF_INTC0_ICR0                       (*(fnet_vuint8_t *)(0xFC048040))
    /*...*/
    #define FNET_MCF_INTC0_ICR63                      (*(fnet_vuint8_t *)(0xFC04807F))
    #define FNET_MCF_INTC0_ICR(x)                     (*(fnet_vuint8_t *)(0xFC048040 + ((x)*0x001)))
    #define FNET_MCF_INTC0_SWIACK                     (*(fnet_vuint8_t *)(0xFC0480E0))

#else /* Other MCFs */

    #define FNET_MCF_INTC0_IPRH         (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x000C00]))
    #define FNET_MCF_INTC0_IPRL         (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x000C04]))
    #define FNET_MCF_INTC0_IMRH         (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x000C08]))
    #define FNET_MCF_INTC0_IMRL         (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x000C0C]))
    #define FNET_MCF_INTC0_IMR(x)       (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x000C0C-((x)*0x4)]))
    #define FNET_MCF_INTC0_INTFRCH      (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x000C10]))
    #define FNET_MCF_INTC0_INTFRCL      (*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x000C14]))
    #define FNET_MCF_INTC0_ICR0         (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x000C40]))
    /*...*/
    #define FNET_MCF_INTC0_ICR63        (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x000C7F]))
    #define FNET_MCF_INTC0_ICR(x)       (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x000C40+((x)*0x001)]))
    #define FNET_MCF_INTC0_SWIACK       (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x000CE0]))

#endif

/* Bit definitions and macros for FNET_MCF_INTC0_IPRH */
#define FNET_MCF_INTC0_IPRH_INT32          (0x00000001)
#define FNET_MCF_INTC0_IPRH_INT33          (0x00000002)
#define FNET_MCF_INTC0_IPRH_INT34          (0x00000004)
#define FNET_MCF_INTC0_IPRH_INT35          (0x00000008)
#define FNET_MCF_INTC0_IPRH_INT36          (0x00000010)
#define FNET_MCF_INTC0_IPRH_INT37          (0x00000020)
#define FNET_MCF_INTC0_IPRH_INT38          (0x00000040)
#define FNET_MCF_INTC0_IPRH_INT39          (0x00000080)
#define FNET_MCF_INTC0_IPRH_INT40          (0x00000100)
#define FNET_MCF_INTC0_IPRH_INT41          (0x00000200)
#define FNET_MCF_INTC0_IPRH_INT42          (0x00000400)
#define FNET_MCF_INTC0_IPRH_INT43          (0x00000800)
#define FNET_MCF_INTC0_IPRH_INT44          (0x00001000)
#define FNET_MCF_INTC0_IPRH_INT45          (0x00002000)
#define FNET_MCF_INTC0_IPRH_INT46          (0x00004000)
#define FNET_MCF_INTC0_IPRH_INT47          (0x00008000)
#define FNET_MCF_INTC0_IPRH_INT48          (0x00010000)
#define FNET_MCF_INTC0_IPRH_INT49          (0x00020000)
#define FNET_MCF_INTC0_IPRH_INT50          (0x00040000)
#define FNET_MCF_INTC0_IPRH_INT51          (0x00080000)
#define FNET_MCF_INTC0_IPRH_INT52          (0x00100000)
#define FNET_MCF_INTC0_IPRH_INT53          (0x00200000)
#define FNET_MCF_INTC0_IPRH_INT54          (0x00400000)
#define FNET_MCF_INTC0_IPRH_INT55          (0x00800000)
#define FNET_MCF_INTC0_IPRH_INT56          (0x01000000)
#define FNET_MCF_INTC0_IPRH_INT57          (0x02000000)
#define FNET_MCF_INTC0_IPRH_INT58          (0x04000000)
#define FNET_MCF_INTC0_IPRH_INT59          (0x08000000)
#define FNET_MCF_INTC0_IPRH_INT60          (0x10000000)
#define FNET_MCF_INTC0_IPRH_INT61          (0x20000000)
#define FNET_MCF_INTC0_IPRH_INT62          (0x40000000)
#define FNET_MCF_INTC0_IPRH_INT63          (0x80000000)

/* Bit definitions and macros for FNET_MCF_INTC0_IPRL */
#define FNET_MCF_INTC0_IPRL_INT1           (0x00000002)
#define FNET_MCF_INTC0_IPRL_INT2           (0x00000004)
#define FNET_MCF_INTC0_IPRL_INT3           (0x00000008)
#define FNET_MCF_INTC0_IPRL_INT4           (0x00000010)
#define FNET_MCF_INTC0_IPRL_INT5           (0x00000020)
#define FNET_MCF_INTC0_IPRL_INT6           (0x00000040)
#define FNET_MCF_INTC0_IPRL_INT7           (0x00000080)
#define FNET_MCF_INTC0_IPRL_INT8           (0x00000100)
#define FNET_MCF_INTC0_IPRL_INT9           (0x00000200)
#define FNET_MCF_INTC0_IPRL_INT10          (0x00000400)
#define FNET_MCF_INTC0_IPRL_INT11          (0x00000800)
#define FNET_MCF_INTC0_IPRL_INT12          (0x00001000)
#define FNET_MCF_INTC0_IPRL_INT13          (0x00002000)
#define FNET_MCF_INTC0_IPRL_INT14          (0x00004000)
#define FNET_MCF_INTC0_IPRL_INT15          (0x00008000)
#define FNET_MCF_INTC0_IPRL_INT16          (0x00010000)
#define FNET_MCF_INTC0_IPRL_INT17          (0x00020000)
#define FNET_MCF_INTC0_IPRL_INT18          (0x00040000)
#define FNET_MCF_INTC0_IPRL_INT19          (0x00080000)
#define FNET_MCF_INTC0_IPRL_INT20          (0x00100000)
#define FNET_MCF_INTC0_IPRL_INT21          (0x00200000)
#define FNET_MCF_INTC0_IPRL_INT22          (0x00400000)
#define FNET_MCF_INTC0_IPRL_INT23          (0x00800000)
#define FNET_MCF_INTC0_IPRL_INT24          (0x01000000)
#define FNET_MCF_INTC0_IPRL_INT25          (0x02000000)
#define FNET_MCF_INTC0_IPRL_INT26          (0x04000000)
#define FNET_MCF_INTC0_IPRL_INT27          (0x08000000)
#define FNET_MCF_INTC0_IPRL_INT28          (0x10000000)
#define FNET_MCF_INTC0_IPRL_INT29          (0x20000000)
#define FNET_MCF_INTC0_IPRL_INT30          (0x40000000)
#define FNET_MCF_INTC0_IPRL_INT31          (0x80000000)

/* Bit definitions and macros for FNET_MCF_INTC0_IMRH */
#define FNET_MCF_INTC0_IMRH_INT_MASK32     (0x00000001)
#define FNET_MCF_INTC0_IMRH_INT_MASK33     (0x00000002)
#define FNET_MCF_INTC0_IMRH_INT_MASK34     (0x00000004)
#define FNET_MCF_INTC0_IMRH_INT_MASK35     (0x00000008)
#define FNET_MCF_INTC0_IMRH_INT_MASK36     (0x00000010)
#define FNET_MCF_INTC0_IMRH_INT_MASK37     (0x00000020)
#define FNET_MCF_INTC0_IMRH_INT_MASK38     (0x00000040)
#define FNET_MCF_INTC0_IMRH_INT_MASK39     (0x00000080)
#define FNET_MCF_INTC0_IMRH_INT_MASK40     (0x00000100)
#define FNET_MCF_INTC0_IMRH_INT_MASK41     (0x00000200)
#define FNET_MCF_INTC0_IMRH_INT_MASK42     (0x00000400)
#define FNET_MCF_INTC0_IMRH_INT_MASK43     (0x00000800)
#define FNET_MCF_INTC0_IMRH_INT_MASK44     (0x00001000)
#define FNET_MCF_INTC0_IMRH_INT_MASK45     (0x00002000)
#define FNET_MCF_INTC0_IMRH_INT_MASK46     (0x00004000)
#define FNET_MCF_INTC0_IMRH_INT_MASK47     (0x00008000)
#define FNET_MCF_INTC0_IMRH_INT_MASK48     (0x00010000)
#define FNET_MCF_INTC0_IMRH_INT_MASK49     (0x00020000)
#define FNET_MCF_INTC0_IMRH_INT_MASK50     (0x00040000)
#define FNET_MCF_INTC0_IMRH_INT_MASK51     (0x00080000)
#define FNET_MCF_INTC0_IMRH_INT_MASK52     (0x00100000)
#define FNET_MCF_INTC0_IMRH_INT_MASK53     (0x00200000)
#define FNET_MCF_INTC0_IMRH_INT_MASK54     (0x00400000)
#define FNET_MCF_INTC0_IMRH_INT_MASK55     (0x00800000)
#define FNET_MCF_INTC0_IMRH_INT_MASK56     (0x01000000)
#define FNET_MCF_INTC0_IMRH_INT_MASK57     (0x02000000)
#define FNET_MCF_INTC0_IMRH_INT_MASK58     (0x04000000)
#define FNET_MCF_INTC0_IMRH_INT_MASK59     (0x08000000)
#define FNET_MCF_INTC0_IMRH_INT_MASK60     (0x10000000)
#define FNET_MCF_INTC0_IMRH_INT_MASK61     (0x20000000)
#define FNET_MCF_INTC0_IMRH_INT_MASK62     (0x40000000)
#define FNET_MCF_INTC0_IMRH_INT_MASK63     (0x80000000)

/* Bit definitions and macros for FNET_MCF_INTC0_IMRL */
#define FNET_MCF_INTC0_IMRL_MASKALL        (0x00000001)
#define FNET_MCF_INTC0_IMRL_INT_MASK1      (0x00000002)
#define FNET_MCF_INTC0_IMRL_INT_MASK2      (0x00000004)
#define FNET_MCF_INTC0_IMRL_INT_MASK3      (0x00000008)
#define FNET_MCF_INTC0_IMRL_INT_MASK4      (0x00000010)
#define FNET_MCF_INTC0_IMRL_INT_MASK5      (0x00000020)
#define FNET_MCF_INTC0_IMRL_INT_MASK6      (0x00000040)
#define FNET_MCF_INTC0_IMRL_INT_MASK7      (0x00000080)
#define FNET_MCF_INTC0_IMRL_INT_MASK8      (0x00000100)
#define FNET_MCF_INTC0_IMRL_INT_MASK9      (0x00000200)
#define FNET_MCF_INTC0_IMRL_INT_MASK10     (0x00000400)
#define FNET_MCF_INTC0_IMRL_INT_MASK11     (0x00000800)
#define FNET_MCF_INTC0_IMRL_INT_MASK12     (0x00001000)
#define FNET_MCF_INTC0_IMRL_INT_MASK13     (0x00002000)
#define FNET_MCF_INTC0_IMRL_INT_MASK14     (0x00004000)
#define FNET_MCF_INTC0_IMRL_INT_MASK15     (0x00008000)
#define FNET_MCF_INTC0_IMRL_INT_MASK16     (0x00010000)
#define FNET_MCF_INTC0_IMRL_INT_MASK17     (0x00020000)
#define FNET_MCF_INTC0_IMRL_INT_MASK18     (0x00040000)
#define FNET_MCF_INTC0_IMRL_INT_MASK19     (0x00080000)
#define FNET_MCF_INTC0_IMRL_INT_MASK20     (0x00100000)
#define FNET_MCF_INTC0_IMRL_INT_MASK21     (0x00200000)
#define FNET_MCF_INTC0_IMRL_INT_MASK22     (0x00400000)
#define FNET_MCF_INTC0_IMRL_INT_MASK23     (0x00800000)
#define FNET_MCF_INTC0_IMRL_INT_MASK24     (0x01000000)
#define FNET_MCF_INTC0_IMRL_INT_MASK25     (0x02000000)
#define FNET_MCF_INTC0_IMRL_INT_MASK26     (0x04000000)
#define FNET_MCF_INTC0_IMRL_INT_MASK27     (0x08000000)
#define FNET_MCF_INTC0_IMRL_INT_MASK28     (0x10000000)
#define FNET_MCF_INTC0_IMRL_INT_MASK29     (0x20000000)
#define FNET_MCF_INTC0_IMRL_INT_MASK30     (0x40000000)
#define FNET_MCF_INTC0_IMRL_INT_MASK31     (0x80000000)

/* Bit definitions and macros for FNET_MCF_INTC0_INTFRCH */
#define FNET_MCF_INTC0_INTFRCH_INTFRC32    (0x00000001)
#define FNET_MCF_INTC0_INTFRCH_INTFRC33    (0x00000002)
#define FNET_MCF_INTC0_INTFRCH_INTFRC34    (0x00000004)
#define FNET_MCF_INTC0_INTFRCH_INTFRC35    (0x00000008)
#define FNET_MCF_INTC0_INTFRCH_INTFRC36    (0x00000010)
#define FNET_MCF_INTC0_INTFRCH_INTFRC37    (0x00000020)
#define FNET_MCF_INTC0_INTFRCH_INTFRC38    (0x00000040)
#define FNET_MCF_INTC0_INTFRCH_INTFRC39    (0x00000080)
#define FNET_MCF_INTC0_INTFRCH_INTFRC40    (0x00000100)
#define FNET_MCF_INTC0_INTFRCH_INTFRC41    (0x00000200)
#define FNET_MCF_INTC0_INTFRCH_INTFRC42    (0x00000400)
#define FNET_MCF_INTC0_INTFRCH_INTFRC43    (0x00000800)
#define FNET_MCF_INTC0_INTFRCH_INTFRC44    (0x00001000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC45    (0x00002000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC46    (0x00004000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC47    (0x00008000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC48    (0x00010000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC49    (0x00020000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC50    (0x00040000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC51    (0x00080000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC52    (0x00100000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC53    (0x00200000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC54    (0x00400000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC55    (0x00800000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC56    (0x01000000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC57    (0x02000000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC58    (0x04000000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC59    (0x08000000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC60    (0x10000000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC61    (0x20000000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC62    (0x40000000)
#define FNET_MCF_INTC0_INTFRCH_INTFRC63    (0x80000000)

/* Bit definitions and macros for FNET_MCF_INTC0_INTFRCL */
#define FNET_MCF_INTC0_INTFRCL_INTFRC1     (0x00000002)
#define FNET_MCF_INTC0_INTFRCL_INTFRC2     (0x00000004)
#define FNET_MCF_INTC0_INTFRCL_INTFRC3     (0x00000008)
#define FNET_MCF_INTC0_INTFRCL_INTFRC4     (0x00000010)
#define FNET_MCF_INTC0_INTFRCL_INTFRC5     (0x00000020)
#define FNET_MCF_INTC0_INTFRCL_INT6        (0x00000040)
#define FNET_MCF_INTC0_INTFRCL_INT7        (0x00000080)
#define FNET_MCF_INTC0_INTFRCL_INT8        (0x00000100)
#define FNET_MCF_INTC0_INTFRCL_INT9        (0x00000200)
#define FNET_MCF_INTC0_INTFRCL_INT10       (0x00000400)
#define FNET_MCF_INTC0_INTFRCL_INTFRC11    (0x00000800)
#define FNET_MCF_INTC0_INTFRCL_INTFRC12    (0x00001000)
#define FNET_MCF_INTC0_INTFRCL_INTFRC13    (0x00002000)
#define FNET_MCF_INTC0_INTFRCL_INTFRC14    (0x00004000)
#define FNET_MCF_INTC0_INTFRCL_INT15       (0x00008000)
#define FNET_MCF_INTC0_INTFRCL_INTFRC16    (0x00010000)
#define FNET_MCF_INTC0_INTFRCL_INTFRC17    (0x00020000)
#define FNET_MCF_INTC0_INTFRCL_INTFRC18    (0x00040000)
#define FNET_MCF_INTC0_INTFRCL_INTFRC19    (0x00080000)
#define FNET_MCF_INTC0_INTFRCL_INTFRC20    (0x00100000)
#define FNET_MCF_INTC0_INTFRCL_INTFRC21    (0x00200000)
#define FNET_MCF_INTC0_INTFRCL_INTFRC22    (0x00400000)
#define FNET_MCF_INTC0_INTFRCL_INTFRC23    (0x00800000)
#define FNET_MCF_INTC0_INTFRCL_INTFRC24    (0x01000000)
#define FNET_MCF_INTC0_INTFRCL_INTFRC25    (0x02000000)
#define FNET_MCF_INTC0_INTFRCL_INTFRC26    (0x04000000)
#define FNET_MCF_INTC0_INTFRCL_INTFRC27    (0x08000000)
#define FNET_MCF_INTC0_INTFRCL_INTFRC28    (0x10000000)
#define FNET_MCF_INTC0_INTFRCL_INTFRC29    (0x20000000)
#define FNET_MCF_INTC0_INTFRCL_INTFRC30    (0x40000000)
#define FNET_MCF_INTC0_INTFRCL_INTFRC31    (0x80000000)

/* Bit definitions and macros for FNET_MCF_INTC0_IRLR */
#define FNET_MCF_INTC0_IRLR_IRQ(x)         (((x)&0x7F)<<1)

/* Bit definitions and macros for FNET_MCF_INTC0_IACKLPR */
#define FNET_MCF_INTC0_IACKLPR_PRI(x)      (((x)&0x0F)<<0)
#define FNET_MCF_INTC0_IACKLPR_LEVEL(x)    (((x)&0x07)<<4)

/* Bit definitions and macros for FNET_MCF_INTC0_ICRn */
#if FNET_CFG_CPU_MCF54418
    #define FNET_MCF_INTC0_ICRn_IP(x)          (0)
    #define FNET_MCF_INTC0_ICRn_IL(x)          (((x)&0x07)<<0)
#else /* Other MCFs */
    #define FNET_MCF_INTC0_ICRn_IP(x)          (((x)&0x07)<<0)
    #define FNET_MCF_INTC0_ICRn_IL(x)          (((x)&0x07)<<3)
#endif

#define FNET_MCF_INTC0_IMR_INT_MASK(x)      (0x00000001<<(x%32))

/*********************************************************************
*
* Edge Port Module (EPORT)
*
*********************************************************************/

/* Register read/write macros */
#if FNET_CFG_CPU_MCF54418
#else /* Other MCFs */
    #define FNET_MCF_EPORT_EPPAR    (*(fnet_vuint16_t*)(&FNET_CFG_MCF_IPSBAR[0x130000]))
    #define FNET_MCF_EPORT_EPDDR    (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x130002]))
    #define FNET_MCF_EPORT_EPIER    (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x130003]))
    #define FNET_MCF_EPORT_EPDR     (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x130004]))
    #define FNET_MCF_EPORT_EPPDR    (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x130005]))
    #define FNET_MCF_EPORT_EPFR     (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x130006]))
#endif

/* Bit definitions and macros for FNET_MCF_EPORT_EPPAR */
#define FNET_MCF_EPORT_EPPAR_EPPA1(x)         (((x)&0x0003)<<2)
#define FNET_MCF_EPORT_EPPAR_EPPA2(x)         (((x)&0x0003)<<4)
#define FNET_MCF_EPORT_EPPAR_EPPA3(x)         (((x)&0x0003)<<6)
#define FNET_MCF_EPORT_EPPAR_EPPA4(x)         (((x)&0x0003)<<8)
#define FNET_MCF_EPORT_EPPAR_EPPA5(x)         (((x)&0x0003)<<10)
#define FNET_MCF_EPORT_EPPAR_EPPA6(x)         (((x)&0x0003)<<12)
#define FNET_MCF_EPORT_EPPAR_EPPA7(x)         (((x)&0x0003)<<14)
#define FNET_MCF_EPORT_EPPAR_EPPAx_LEVEL      (0)
#define FNET_MCF_EPORT_EPPAR_EPPAx_RISING     (1)
#define FNET_MCF_EPORT_EPPAR_EPPAx_FALLING    (2)
#define FNET_MCF_EPORT_EPPAR_EPPAx_BOTH       (3)

/* Bit definitions and macros for FNET_MCF_EPORT_EPDDR */
#define FNET_MCF_EPORT_EPDDR_EPDD1            (0x02)
#define FNET_MCF_EPORT_EPDDR_EPDD2            (0x04)
#define FNET_MCF_EPORT_EPDDR_EPDD3            (0x08)
#define FNET_MCF_EPORT_EPDDR_EPDD4            (0x10)
#define FNET_MCF_EPORT_EPDDR_EPDD5            (0x20)
#define FNET_MCF_EPORT_EPDDR_EPDD6            (0x40)
#define FNET_MCF_EPORT_EPDDR_EPDD7            (0x80)

/* Bit definitions and macros for FNET_MCF_EPORT_EPIER */
#define FNET_MCF_EPORT_EPIER_EPIE1            (0x02)
#define FNET_MCF_EPORT_EPIER_EPIE2            (0x04)
#define FNET_MCF_EPORT_EPIER_EPIE3            (0x08)
#define FNET_MCF_EPORT_EPIER_EPIE4            (0x10)
#define FNET_MCF_EPORT_EPIER_EPIE5            (0x20)
#define FNET_MCF_EPORT_EPIER_EPIE6            (0x40)
#define FNET_MCF_EPORT_EPIER_EPIE7            (0x80)

/* Bit definitions and macros for FNET_MCF_EPORT_EPDR */
#define FNET_MCF_EPORT_EPDR_EPD1              (0x02)
#define FNET_MCF_EPORT_EPDR_EPD2              (0x04)
#define FNET_MCF_EPORT_EPDR_EPD3              (0x08)
#define FNET_MCF_EPORT_EPDR_EPD4              (0x10)
#define FNET_MCF_EPORT_EPDR_EPD5              (0x20)
#define FNET_MCF_EPORT_EPDR_EPD6              (0x40)
#define FNET_MCF_EPORT_EPDR_EPD7              (0x80)

/* Bit definitions and macros for FNET_MCF_EPORT_EPPDR */
#define FNET_MCF_EPORT_EPPDR_EPPD1            (0x02)
#define FNET_MCF_EPORT_EPPDR_EPPD2            (0x04)
#define FNET_MCF_EPORT_EPPDR_EPPD3            (0x08)
#define FNET_MCF_EPORT_EPPDR_EPPD4            (0x10)
#define FNET_MCF_EPORT_EPPDR_EPPD5            (0x20)
#define FNET_MCF_EPORT_EPPDR_EPPD6            (0x40)
#define FNET_MCF_EPORT_EPPDR_EPPD7            (0x80)

/* Bit definitions and macros for FNET_MCF_EPORT_EPFR */
#define FNET_MCF_EPORT_EPFR_EPF1              (0x02)
#define FNET_MCF_EPORT_EPFR_EPF2              (0x04)
#define FNET_MCF_EPORT_EPFR_EPF3              (0x08)
#define FNET_MCF_EPORT_EPFR_EPF4              (0x10)
#define FNET_MCF_EPORT_EPFR_EPF5              (0x20)
#define FNET_MCF_EPORT_EPFR_EPF6              (0x40)
#define FNET_MCF_EPORT_EPFR_EPF7              (0x80)

/********************************************************************/

/*********************************************************************
*
* DMA Timers (TIMER)
*
*********************************************************************/

/* Register read/write macros */
#if FNET_CFG_CPU_MCF54418

    #define FNET_MCF_DTIM_DTMR(x)		(*(fnet_vuint16_t*)(0xFC070000 + ((x)*0x4000)))
    #define FNET_MCF_DTIM_DTXMR(x) 		(*(fnet_vuint8_t *)(0xFC070002 + ((x)*0x4000)))
    #define FNET_MCF_DTIM_DTER(x)      	(*(fnet_vuint8_t *)(0xFC070003 + ((x)*0x4000)))
    #define FNET_MCF_DTIM_DTRR(x)     	(*(fnet_vuint32_t*)(0xFC070004 + ((x)*0x4000)))
    #define FNET_MCF_DTIM_DTCR(x)      	(*(fnet_vuint32_t*)(0xFC070008 + ((x)*0x4000)))
    #define FNET_MCF_DTIM_DTCN(x)      	(*(fnet_vuint32_t*)(0xFC07000C + ((x)*0x4000)))

#else /* Other MCFs */

    #define FNET_MCF_DTIM_DTMR(x)     	(*(fnet_vuint16_t*)(&FNET_CFG_MCF_IPSBAR[0x000400+((x)*0x040)]))
    #define FNET_MCF_DTIM_DTXMR(x)    	(*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x000402+((x)*0x040)]))
    #define FNET_MCF_DTIM_DTER(x)     	(*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x000403+((x)*0x040)]))
    #define FNET_MCF_DTIM_DTRR(x)     	(*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x000404+((x)*0x040)]))
    #define FNET_MCF_DTIM_DTCR(x)     	(*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x000408+((x)*0x040)]))
    #define FNET_MCF_DTIM_DTCN(x)     	(*(fnet_vuint32_t*)(&FNET_CFG_MCF_IPSBAR[0x00040C+((x)*0x040)]))

#endif

/* Bit definitions and macros for FNET_MCF_DTIM_DTMR */
#define FNET_MCF_DTIM_DTMR_RST          (0x0001)
#define FNET_MCF_DTIM_DTMR_CLK(x)       (((x)&0x0003)<<1)
#define FNET_MCF_DTIM_DTMR_FRR          (0x0008)
#define FNET_MCF_DTIM_DTMR_ORRI         (0x0010)
#define FNET_MCF_DTIM_DTMR_OM           (0x0020)
#define FNET_MCF_DTIM_DTMR_CE(x)        (((x)&0x0003)<<6)
#define FNET_MCF_DTIM_DTMR_PS(x)        (((x)&0x00FF)<<8)
#define FNET_MCF_DTIM_DTMR_CE_ANY       (0x00C0)
#define FNET_MCF_DTIM_DTMR_CE_FALL      (0x0080)
#define FNET_MCF_DTIM_DTMR_CE_RISE      (0x0040)
#define FNET_MCF_DTIM_DTMR_CE_NONE      (0x0000)
#define FNET_MCF_DTIM_DTMR_CLK_DTIN     (0x0006)
#define FNET_MCF_DTIM_DTMR_CLK_DIV16    (0x0004)
#define FNET_MCF_DTIM_DTMR_CLK_DIV1     (0x0002)
#define FNET_MCF_DTIM_DTMR_CLK_STOP     (0x0000)

/* Bit definitions and macros for FNET_MCF_DTIM_DTXMR */
#define FNET_MCF_DTIM_DTXMR_MODE16      (0x01)
#define FNET_MCF_DTIM_DTXMR_DMAEN       (0x80)

/* Bit definitions and macros for FNET_MCF_DTIM_DTER */
#define FNET_MCF_DTIM_DTER_CAP          (0x01)
#define FNET_MCF_DTIM_DTER_REF          (0x02)

/*********************************************************************
*
* Programmable Interrupt Timer (PIT)
*
*********************************************************************/

/* Register read/write macros */
#if FNET_CFG_CPU_MCF54418
#else /* Other MCFs */
    #define FNET_MCF_PIT0_PCSR                        (*(fnet_vuint16_t*)(&FNET_CFG_MCF_IPSBAR[0x00150000]))
    #define FNET_MCF_PIT0_PMR                         (*(fnet_vuint16_t*)(&FNET_CFG_MCF_IPSBAR[0x00150002]))
    #define FNET_MCF_PIT0_PCNTR                       (*(fnet_vuint16_t*)(&FNET_CFG_MCF_IPSBAR[0x00150004]))

    #define FNET_MCF_PIT1_PCSR                        (*(fnet_vuint16_t*)(&FNET_CFG_MCF_IPSBAR[0x00160000]))
    #define FNET_MCF_PIT1_PMR                         (*(fnet_vuint16_t*)(&FNET_CFG_MCF_IPSBAR[0x00160002]))
    #define FNET_MCF_PIT1_PCNTR                       (*(fnet_vuint16_t*)(&FNET_CFG_MCF_IPSBAR[0x00160004]))

    #define FNET_MCF_PIT_PCSR(x)                      (*(fnet_vuint16_t*)(&FNET_CFG_MCF_IPSBAR[0x00150000 + ((x)*0x10000)]))
    #define FNET_MCF_PIT_PMR(x)                       (*(fnet_vuint16_t*)(&FNET_CFG_MCF_IPSBAR[0x00150002 + ((x)*0x10000)]))
    #define FNET_MCF_PIT_PCNTR(x)                     (*(fnet_vuint16_t*)(&FNET_CFG_MCF_IPSBAR[0x00150004 + ((x)*0x10000)]))
#endif

/* Bit definitions and macros for FNET_MCF_PIT_PCSR */
#define FNET_MCF_PIT_PCSR_EN                      (0x1)
#define FNET_MCF_PIT_PCSR_RLD                     (0x2)
#define FNET_MCF_PIT_PCSR_PIF                     (0x4)
#define FNET_MCF_PIT_PCSR_PIE                     (0x8)
#define FNET_MCF_PIT_PCSR_OVW                     (0x10)
#define FNET_MCF_PIT_PCSR_DBG                     (0x20)
#define FNET_MCF_PIT_PCSR_DOZE                    (0x40)
#define FNET_MCF_PIT_PCSR_PRE(x)                  (((x)&0xF)<<0x8)

/* Bit definitions and macros for FNET_MCF_PIT_PMR */
#define FNET_MCF_PIT_PMR_PM(x)                    (((x)&0xFFFF)<<0)

/* Bit definitions and macros for FNET_MCF_PIT_PCNTR */
#define FNET_MCF_PIT_PCNTR_PC(x)                  (((x)&0xFFFF)<<0)


/*********************************************************************
*
* Universal Asynchronous Receiver Transmitter (UART)
*
*********************************************************************/

/* Register read/write macros */
#if FNET_CFG_CPU_MCF54418

    #define FNET_MCF_UART_UMR(x)                      (*(fnet_vuint8_t *)(0xEC060000 + (((x)<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
    #define FNET_MCF_UART_USR(x)                      (*(fnet_vuint8_t *)(0xEC060004 + (((x)<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
    #define FNET_MCF_UART_UCSR(x)                     (*(fnet_vuint8_t *)(0xEC060004 + (((x)<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
    #define FNET_MCF_UART_UCR(x)                      (*(fnet_vuint8_t *)(0xEC060008 + (((x)<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
    #define FNET_MCF_UART_URB(x)                      (*(fnet_vuint8_t *)(0xEC06000C + (((x)<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
    #define FNET_MCF_UART_UTB(x)                      (*(fnet_vuint8_t *)(0xEC06000C + (((x)<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
    #define FNET_MCF_UART_UACR(x)                     (*(fnet_vuint8_t *)(0xEC060010 + (((x)<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
    #define FNET_MCF_UART_UIPCR(x)                    (*(fnet_vuint8_t *)(0xEC060010 + (((x)<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
    #define FNET_MCF_UART_UIMR(x)                     (*(fnet_vuint8_t *)(0xEC060014 + (((x)<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
    #define FNET_MCF_UART_UISR(x)                     (*(fnet_vuint8_t *)(0xEC060014 + (((x)<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
    #define FNET_MCF_UART_UBG1(x)                     (*(fnet_vuint8_t *)(0xEC060018 + (((x)<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
    #define FNET_MCF_UART_UBG2(x)                     (*(fnet_vuint8_t *)(0xEC06001C + (((x)<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
    #define FNET_MCF_UART_UIP(x)                      (*(fnet_vuint8_t *)(0xEC060034 + (((x)<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
    #define FNET_MCF_UART_UOP1(x)                     (*(fnet_vuint8_t *)(0xEC060038 + (((x)<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
    #define FNET_MCF_UART_UOP0(x)                     (*(fnet_vuint8_t *)(0xEC06003C + (((x)<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))

#else /* Other MCFs */

    #define FNET_MCF_UART_UMR(x)                      (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x200 + ((x)*0x40)]))
    #define FNET_MCF_UART_USR(x)                      (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x204 + ((x)*0x40)]))
    #define FNET_MCF_UART_UCSR(x)                     (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x204 + ((x)*0x40)]))
    #define FNET_MCF_UART_UCR(x)                      (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x208 + ((x)*0x40)]))
    #define FNET_MCF_UART_URB(x)                      (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x20C + ((x)*0x40)]))
    #define FNET_MCF_UART_UTB(x)                      (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x20C + ((x)*0x40)]))
    #define FNET_MCF_UART_UIPCR(x)                    (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x210 + ((x)*0x40)]))
    #define FNET_MCF_UART_UACR(x)                     (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x210 + ((x)*0x40)]))
    #define FNET_MCF_UART_UIMR(x)                     (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x214 + ((x)*0x40)]))
    #define FNET_MCF_UART_UISR(x)                     (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x214 + ((x)*0x40)]))
    #define FNET_MCF_UART_UBG1(x)                     (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x218 + ((x)*0x40)]))
    #define FNET_MCF_UART_UBG2(x)                     (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x21C + ((x)*0x40)]))
    #define FNET_MCF_UART_UIP(x)                      (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x234 + ((x)*0x40)]))
    #define FNET_MCF_UART_UOP1(x)                     (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x238 + ((x)*0x40)]))
    #define FNET_MCF_UART_UOP0(x)                     (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x23C + ((x)*0x40)]))

#endif

/* Bit definitions and macros for MCF_UART_UMR */
#define FNET_MCF_UART_UMR_BC(x)                   (((x)&0x3)<<0)
#define FNET_MCF_UART_UMR_BC_5                    (0)
#define FNET_MCF_UART_UMR_BC_6                    (0x1)
#define FNET_MCF_UART_UMR_BC_7                    (0x2)
#define FNET_MCF_UART_UMR_BC_8                    (0x3)
#define FNET_MCF_UART_UMR_PT                      (0x4)
#define FNET_MCF_UART_UMR_PM(x)                   (((x)&0x3)<<0x3)
#define FNET_MCF_UART_UMR_ERR                     (0x20)
#define FNET_MCF_UART_UMR_RXIRQ                   (0x40)
#define FNET_MCF_UART_UMR_RXRTS                   (0x80)
#define FNET_MCF_UART_UMR_PM_MULTI_ADDR           (0x1C)
#define FNET_MCF_UART_UMR_PM_MULTI_DATA           (0x18)
#define FNET_MCF_UART_UMR_PM_NONE                 (0x10)
#define FNET_MCF_UART_UMR_PM_FORCE_HI             (0xC)
#define FNET_MCF_UART_UMR_PM_FORCE_LO             (0x8)
#define FNET_MCF_UART_UMR_PM_ODD                  (0x4)
#define FNET_MCF_UART_UMR_PM_EVEN                 (0)
#define FNET_MCF_UART_UMR_SB(x)                   (((x)&0xF)<<0)
#define FNET_MCF_UART_UMR_SB_STOP_BITS_1          (0x7)
#define FNET_MCF_UART_UMR_SB_STOP_BITS_15         (0x8)
#define FNET_MCF_UART_UMR_SB_STOP_BITS_2          (0xF)
#define FNET_MCF_UART_UMR_TXCTS                   (0x10)
#define FNET_MCF_UART_UMR_TXRTS                   (0x20)
#define FNET_MCF_UART_UMR_CM(x)                   (((x)&0x3)<<0x6)
#define FNET_MCF_UART_UMR_CM_NORMAL               (0)
#define FNET_MCF_UART_UMR_CM_ECHO                 (0x40)
#define FNET_MCF_UART_UMR_CM_LOCAL_LOOP           (0x80)
#define FNET_MCF_UART_UMR_CM_REMOTE_LOOP          (0xC0)

/* Bit definitions and macros for MCF_UART_USR */
#define FNET_MCF_UART_USR_RXRDY                   (0x1)
#define FNET_MCF_UART_USR_FFULL                   (0x2)
#define FNET_MCF_UART_USR_TXRDY                   (0x4)
#define FNET_MCF_UART_USR_TXEMP                   (0x8)
#define FNET_MCF_UART_USR_OE                      (0x10)
#define FNET_MCF_UART_USR_PE                      (0x20)
#define FNET_MCF_UART_USR_FE                      (0x40)
#define FNET_MCF_UART_USR_RB                      (0x80)

/* Bit definitions and macros for MCF_UART_UCSR */
#define FNET_MCF_UART_UCSR_TCS(x)                 (((x)&0xF)<<0)
#define FNET_MCF_UART_UCSR_TCS_SYS_CLK            (0xD)
#define FNET_MCF_UART_UCSR_TCS_CTM16              (0xE)
#define FNET_MCF_UART_UCSR_TCS_CTM                (0xF)
#define FNET_MCF_UART_UCSR_RCS(x)                 (((x)&0xF)<<0x4)
#define FNET_MCF_UART_UCSR_RCS_SYS_CLK            (0xD0)
#define FNET_MCF_UART_UCSR_RCS_CTM16              (0xE0)
#define FNET_MCF_UART_UCSR_RCS_CTM                (0xF0)

/* Bit definitions and macros for MCF_UART_UCR */
#define FNET_MCF_UART_UCR_RC(x)                   (((x)&0x3)<<0)
#define FNET_MCF_UART_UCR_RX_ENABLED              (0x1)
#define FNET_MCF_UART_UCR_RX_DISABLED             (0x2)
#define FNET_MCF_UART_UCR_TC(x)                   (((x)&0x3)<<0x2)
#define FNET_MCF_UART_UCR_TX_ENABLED              (0x4)
#define FNET_MCF_UART_UCR_TX_DISABLED             (0x8)
#define FNET_MCF_UART_UCR_MISC(x)                 (((x)&0x7)<<0x4)
#define FNET_MCF_UART_UCR_NONE                    (0)
#define FNET_MCF_UART_UCR_RESET_MR                (0x10)
#define FNET_MCF_UART_UCR_RESET_RX                (0x20)
#define FNET_MCF_UART_UCR_RESET_TX                (0x30)
#define FNET_MCF_UART_UCR_RESET_ERROR             (0x40)
#define FNET_MCF_UART_UCR_RESET_BKCHGINT          (0x50)
#define FNET_MCF_UART_UCR_START_BREAK             (0x60)
#define FNET_MCF_UART_UCR_STOP_BREAK              (0x70)

/* Bit definitions and macros for MCF_UART_URB */
#define FNET_MCF_UART_URB_RB(x)                   (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_UART_UTB */
#define FNET_MCF_UART_UTB_TB(x)                   (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_UART_UIPCR */
#define FNET_MCF_UART_UIPCR_CTS                   (0x1)
#define FNET_MCF_UART_UIPCR_COS                   (0x10)

/* Bit definitions and macros for MCF_UART_UACR */
#define FNET_MCF_UART_UACR_IEC                    (0x1)

/* Bit definitions and macros for MCF_UART_UIMR */
#define FNET_MCF_UART_UIMR_TXRDY                  (0x1)
#define FNET_MCF_UART_UIMR_FFULL_RXRDY            (0x2)
#define FNET_MCF_UART_UIMR_DB                     (0x4)
#define FNET_MCF_UART_UIMR_COS                    (0x80)

/* Bit definitions and macros for MCF_UART_UISR */
#define FNET_MCF_UART_UISR_TXRDY                  (0x1)
#define FNET_MCF_UART_UISR_FFULL_RXRDY            (0x2)
#define FNET_MCF_UART_UISR_DB                     (0x4)
#define FNET_MCF_UART_UISR_COS                    (0x80)

/* Bit definitions and macros for MCF_UART_UBG1 */
#define FNET_MCF_UART_UBG1_Divider_MSB(x)         (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_UART_UBG2 */
#define FNET_MCF_UART_UBG2_Divider_LSB(x)         (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_UART_UIP */
#define FNET_MCF_UART_UIP_CTS                     (0x1)

/* Bit definitions and macros for MCF_UART_UOP1 */
#define FNET_MCF_UART_UOP1_RTS                    (0x1)

/* Bit definitions and macros for MCF_UART_UOP0 */
#define FNET_MCF_UART_UOP0_RTS                    (0x1)


/*********************************************************************
*
* ColdFire Flash Module (CFM)
*
*********************************************************************/

/* Register read/write macros */
#if FNET_CFG_CPU_MCF54418
#else /* Other MCFs */
    #define FNET_MCF_CFM_CFMMCR                       (*(fnet_vuint16_t *)(&FNET_CFG_MCF_IPSBAR[0x001D0000]))
    #define FNET_MCF_CFM_CFMCLKD                      (*(fnet_vuint8_t  *)(&FNET_CFG_MCF_IPSBAR[0x001D0002]))
    #define FNET_MCF_CFM_CFMSEC                       (*(fnet_vuint32_t *)(&FNET_CFG_MCF_IPSBAR[0x001D0008]))
    #define FNET_MCF_CFM_CFMPROT                      (*(fnet_vuint32_t *)(&FNET_CFG_MCF_IPSBAR[0x001D0010]))
    #define FNET_MCF_CFM_CFMSACC                      (*(fnet_vuint32_t *)(&FNET_CFG_MCF_IPSBAR[0x001D0014]))
    #define FNET_MCF_CFM_CFMDACC                      (*(fnet_vuint32_t *)(&FNET_CFG_MCF_IPSBAR[0x001D0018]))
    #define FNET_MCF_CFM_CFMUSTAT                     (*(fnet_vuint8_t  *)(&FNET_CFG_MCF_IPSBAR[0x001D0020]))
    #define FNET_MCF_CFM_CFMCMD                       (*(fnet_vuint8_t  *)(&FNET_CFG_MCF_IPSBAR[0x001D0024]))
    #define FNET_MCF_CFM_CFMCLKSEL                    (*(fnet_vuint16_t *)(&FNET_CFG_MCF_IPSBAR[0x001D004A]))
#endif

/* Bit definitions and macros for FNET_MCF_CFM_CFMMCR */
#define FNET_MCF_CFM_CFMMCR_KEYACC                (0x20)
#define FNET_MCF_CFM_CFMMCR_CCIE                  (0x40)
#define FNET_MCF_CFM_CFMMCR_CBEIE                 (0x80)
#define FNET_MCF_CFM_CFMMCR_AEIE                  (0x100)
#define FNET_MCF_CFM_CFMMCR_PVIE                  (0x200)
#define FNET_MCF_CFM_CFMMCR_LOCK                  (0x400)

/* Bit definitions and macros for FNET_MCF_CFM_CFMCLKD */
#define FNET_MCF_CFM_CFMCLKD_DIV(x)               (((x)&0x3F)<<0)
#define FNET_MCF_CFM_CFMCLKD_PRDIV8               (0x40)
#define FNET_MCF_CFM_CFMCLKD_DIVLD                (0x80)

/* Bit definitions and macros for FNET_MCF_CFM_CFMSEC */
#define FNET_MCF_CFM_CFMSEC_SEC(x)                (((x)&0xFFFF)<<0)
#define FNET_MCF_CFM_CFMSEC_SECSTAT               (0x40000000)
#define FNET_MCF_CFM_CFMSEC_KEYEN                 (0x80000000)

/* Bit definitions and macros for FNET_MCF_CFM_CFMPROT */
#define FNET_MCF_CFM_CFMPROT_PROTECT(x)           (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for FNET_MCF_CFM_CFMSACC */
#define FNET_MCF_CFM_CFMSACC_SUPV(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for FNET_MCF_CFM_CFMDACC */
#define FNET_MCF_CFM_CFMDACC_DACC(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for FNET_MCF_CFM_CFMUSTAT */
#define FNET_MCF_CFM_CFMUSTAT_BLANK               (0x4)
#define FNET_MCF_CFM_CFMUSTAT_ACCERR              (0x10)
#define FNET_MCF_CFM_CFMUSTAT_PVIOL               (0x20)
#define FNET_MCF_CFM_CFMUSTAT_CCIF                (0x40)
#define FNET_MCF_CFM_CFMUSTAT_CBEIF               (0x80)

/* Bit definitions and macros for FNET_MCF_CFM_CFMCMD */
#define FNET_MCF_CFM_CFMCMD_CMD(x)                (((x)&0x7F)<<0)
#define FNET_MCF_CFM_CFMCMD_BLANK_CHECK           (0x5)
#define FNET_MCF_CFM_CFMCMD_PAGE_ERASE_VERIFY     (0x6)
#define FNET_MCF_CFM_CFMCMD_WORD_PROGRAM          (0x20)
#define FNET_MCF_CFM_CFMCMD_PAGE_ERASE            (0x40)
#define FNET_MCF_CFM_CFMCMD_MASS_ERASE            (0x41)

/* Bit definitions and macros for FNET_MCF_CFM_CFMCLKSEL */
#define FNET_MCF_CFM_CFMCLKSEL_CLKSEL(x)          (((x)&0x3)<<0)


/*********************************************************************
*
* Reset Controller Module (RCM)
*
*********************************************************************/

/* Register read/write macros */
#if FNET_CFG_CPU_MCF54418
    #define FNET_MCF_RCM_RCR                          (*(fnet_vuint8_t *)(0xEC090000))
    #define FNET_MCF_RCM_RSR                          (*(fnet_vuint8_t *)(0xEC090001))
#else /* Other MCFs */
    #define FNET_MCF_RCM_RCR                          (*(fnet_vuint8_t  *)(&FNET_CFG_MCF_IPSBAR[0x00110000]))
    #define FNET_MCF_RCM_RSR                          (*(fnet_vuint8_t  *)(&FNET_CFG_MCF_IPSBAR[0x00110001]))
#endif


/* Bit definitions and macros for MCF_RCM_RCR */
#define FNET_MCF_RCM_RCR_LVDE                     (0x1)
#define FNET_MCF_RCM_RCR_LVDRE                    (0x4)
#define FNET_MCF_RCM_RCR_LVDIE                    (0x8)
#define FNET_MCF_RCM_RCR_LVDF                     (0x10)
#define FNET_MCF_RCM_RCR_FRCRSTOUT                (0x40)
#define FNET_MCF_RCM_RCR_SOFTRST                  (0x80)

/* Bit definitions and macros for MCF_RCM_RSR */
#define FNET_MCF_RCM_RSR_LOL                      (0x1)
#define FNET_MCF_RCM_RSR_LOC                      (0x2)
#define FNET_MCF_RCM_RSR_EXT                      (0x4)
#define FNET_MCF_RCM_RSR_POR                      (0x8)
#define FNET_MCF_RCM_RSR_WDR                      (0x10)
#define FNET_MCF_RCM_RSR_SOFT                     (0x20)
#define FNET_MCF_RCM_RSR_LVD                      (0x40)


/*********************************************************************
*
* MCFV1 Peripheral Registers
*
*********************************************************************/
#if FNET_CFG_CPU_MCF51CN128 /* Lasko  registers map*/

    #define FNET_MCF_PTADS_PTR      (0xFFFF800A)
    #define FNET_MCF_PTBDS_PTR      (0xFFFF801A)
    #define FNET_MCF_PTCDS_PTR      (0xFFFF802A)
    #define FNET_MCF_PTAPF1_PTR     (0xFFFF80C0)
    #define FNET_MCF_PTAPF2_PTR     (0xFFFF80C1)
    #define FNET_MCF_PTBPF1_PTR     (0xFFFF80C2)
    #define FNET_MCF_PTBPF2_PTR     (0xFFFF80C3)
    #define FNET_MCF_PTCPF2_PTR     (0xFFFF80C5)
    #define FNET_MCF_PTDPF2_PTR     (0xFFFF80C7)

    #define FNET_MCF_SCI1BD_PTR     (0xFFFF8160)
    #define FNET_MCF_SCI1C1_PTR     (0xFFFF8162)
    #define FNET_MCF_SCI1C2_PTR     (0xFFFF8163)
    #define FNET_MCF_SCI1S1_PTR     (0xFFFF8164)
    #define FNET_MCF_SCI1C3_PTR     (0xFFFF8166)
    #define FNET_MCF_SCI1D_PTR      (0xFFFF8167)

    #define FNET_MCF_SCI2S1_PTR     (0xFFFF8184)
    #define FNET_MCF_SCI2D_PTR      (0xFFFF8187)

    #define FNET_MCF_MCGC1_PTR      (0xFFFF8240)
    #define FNET_MCF_MCGTRM_PTR     (0xFFFF8242)
    #define FNET_MCF_NVMCGTRM_PTR   (0x000003FF)

    #define FNET_MCF_RTCSC_PTR      (0xFFFF82C0)
    #define FNET_MCF_RTCCNT_PTR     (0xFFFF82C1)
    #define FNET_MCF_RTCMOD_PTR     (0xFFFF82C2)

    #define FNET_MCF_PTAIFE_PTR     (0xFFFF800B)
    #define FNET_MCF_PTBIFE_PTR     (0xFFFF801B)
    #define FNET_MCF_PTCIFE_PTR     (0xFFFF802B)
    #define FNET_MCF_PTEPF1_PTR     (0xFFFF80C8)

    #define FNET_MCF_SCGC1_PTR      (0xFFFF8108)
    #define FNET_MCF_SCGC2_PTR      (0xFFFF8109)


    /* Redefine pointers to the registers for V1 */
    #undef FNET_FEC0_BASE_ADDR
    #define FNET_FEC0_BASE_ADDR        ((fnet_vuint32_t*)(0xFFFFE004))

    #undef FNET_MCF_CFM_CFMUSTAT
    #define FNET_MCF_CFM_CFMUSTAT                     (*(fnet_vuint8_t  *)(0xFFFF82E5))
    #undef FNET_MCF_CFM_CFMCMD
    #define FNET_MCF_CFM_CFMCMD                       (*(fnet_vuint8_t  *)(0xFFFF82E6))
    #undef FNET_MCF_CFM_CFMCLKD
    #define FNET_MCF_CFM_CFMCLKD                      (*(fnet_vuint8_t  *)(0xFFFF82E0))

#endif /*FNET_CFG_CPU_MCF51CN128*/

#if FNET_CFG_MCF_V1
/*** PTADS - Port A Drive Strength Selection Register ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t PTADS0      : 1;                                      /* Output Drive Strength Selection for Port A Bit 0 */
        fnet_uint8_t PTADS1      : 1;                                      /* Output Drive Strength Selection for Port A Bit 1 */
        fnet_uint8_t PTADS2      : 1;                                      /* Output Drive Strength Selection for Port A Bit 2 */
        fnet_uint8_t PTADS3      : 1;                                      /* Output Drive Strength Selection for Port A Bit 3 */
        fnet_uint8_t PTADS4      : 1;                                      /* Output Drive Strength Selection for Port A Bit 4 */
        fnet_uint8_t PTADS5      : 1;                                      /* Output Drive Strength Selection for Port A Bit 5 */
        fnet_uint8_t PTADS6      : 1;                                      /* Output Drive Strength Selection for Port A Bit 6 */
        fnet_uint8_t PTADS7      : 1;                                      /* Output Drive Strength Selection for Port A Bit 7 */
    } Bits;
} FNET_MCF_PTADSSTR;
extern volatile FNET_MCF_PTADSSTR _FNET_MCF_PTADS @FNET_MCF_PTADS_PTR;
#define FNET_MCF_PTADS                           _FNET_MCF_PTADS.Byte
#define FNET_MCF_PTADS_PTADS0                    _FNET_MCF_PTADS.Bits.PTADS0
#define FNET_MCF_PTADS_PTADS1                    _FNET_MCF_PTADS.Bits.PTADS1
#define FNET_MCF_PTADS_PTADS2                    _FNET_MCF_PTADS.Bits.PTADS2
#define FNET_MCF_PTADS_PTADS3                    _FNET_MCF_PTADS.Bits.PTADS3
#define FNET_MCF_PTADS_PTADS4                    _FNET_MCF_PTADS.Bits.PTADS4
#define FNET_MCF_PTADS_PTADS5                    _FNET_MCF_PTADS.Bits.PTADS5
#define FNET_MCF_PTADS_PTADS6                    _FNET_MCF_PTADS.Bits.PTADS6
#define FNET_MCF_PTADS_PTADS7                    _FNET_MCF_PTADS.Bits.PTADS7

#define FNET_MCF_PTADS_PTADS0_MASK               1
#define FNET_MCF_PTADS_PTADS1_MASK               2
#define FNET_MCF_PTADS_PTADS2_MASK               4
#define FNET_MCF_PTADS_PTADS3_MASK               8
#define FNET_MCF_PTADS_PTADS4_MASK               16
#define FNET_MCF_PTADS_PTADS5_MASK               32
#define FNET_MCF_PTADS_PTADS6_MASK               64
#define FNET_MCF_PTADS_PTADS7_MASK               128

/*** PTBDS - Port B Drive Strength Selection Register ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t PTBDS0      : 1;                                      /* Output Drive Strength Selection for Port B Bit 0 */
        fnet_uint8_t PTBDS1      : 1;                                      /* Output Drive Strength Selection for Port B Bit 1 */
        fnet_uint8_t PTBDS2      : 1;                                      /* Output Drive Strength Selection for Port B Bit 2 */
        fnet_uint8_t PTBDS3      : 1;                                      /* Output Drive Strength Selection for Port B Bit 3 */
        fnet_uint8_t PTBDS4      : 1;                                      /* Output Drive Strength Selection for Port B Bit 4 */
        fnet_uint8_t PTBDS5      : 1;                                      /* Output Drive Strength Selection for Port B Bit 5 */
        fnet_uint8_t PTBDS6      : 1;                                      /* Output Drive Strength Selection for Port B Bit 6 */
        fnet_uint8_t PTBDS7      : 1;                                      /* Output Drive Strength Selection for Port B Bit 7 */
    } Bits;
} FNET_MCF_PTBDSSTR;
extern volatile FNET_MCF_PTBDSSTR _FNET_MCF_PTBDS @FNET_MCF_PTBDS_PTR;
#define FNET_MCF_PTBDS                           _FNET_MCF_PTBDS.Byte
#define FNET_MCF_PTBDS_PTBDS0                    _FNET_MCF_PTBDS.Bits.PTBDS0
#define FNET_MCF_PTBDS_PTBDS1                    _FNET_MCF_PTBDS.Bits.PTBDS1
#define FNET_MCF_PTBDS_PTBDS2                    _FNET_MCF_PTBDS.Bits.PTBDS2
#define FNET_MCF_PTBDS_PTBDS3                    _FNET_MCF_PTBDS.Bits.PTBDS3
#define FNET_MCF_PTBDS_PTBDS4                    _FNET_MCF_PTBDS.Bits.PTBDS4
#define FNET_MCF_PTBDS_PTBDS5                    _FNET_MCF_PTBDS.Bits.PTBDS5
#define FNET_MCF_PTBDS_PTBDS6                    _FNET_MCF_PTBDS.Bits.PTBDS6
#define FNET_MCF_PTBDS_PTBDS7                    _FNET_MCF_PTBDS.Bits.PTBDS7

#define FNET_MCF_PTBDS_PTBDS0_MASK               1
#define FNET_MCF_PTBDS_PTBDS1_MASK               2
#define FNET_MCF_PTBDS_PTBDS2_MASK               4
#define FNET_MCF_PTBDS_PTBDS3_MASK               8
#define FNET_MCF_PTBDS_PTBDS4_MASK               16
#define FNET_MCF_PTBDS_PTBDS5_MASK               32
#define FNET_MCF_PTBDS_PTBDS6_MASK               64
#define FNET_MCF_PTBDS_PTBDS7_MASK               128

/*** PTCDS - Port C Drive Strength Selection Register ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t PTCDS0      : 1;                                      /* Output Drive Strength Selection for Port C Bit 0 */
        fnet_uint8_t PTCDS1      : 1;                                      /* Output Drive Strength Selection for Port C Bit 1 */
        fnet_uint8_t PTCDS2      : 1;                                      /* Output Drive Strength Selection for Port C Bit 2 */
        fnet_uint8_t PTCDS3      : 1;                                      /* Output Drive Strength Selection for Port C Bit 3 */
        fnet_uint8_t PTCDS4      : 1;                                      /* Output Drive Strength Selection for Port C Bit 4 */
        fnet_uint8_t PTCDS5      : 1;                                      /* Output Drive Strength Selection for Port C Bit 5 */
        fnet_uint8_t PTCDS6      : 1;                                      /* Output Drive Strength Selection for Port C Bit 6 */
        fnet_uint8_t PTCDS7      : 1;                                      /* Output Drive Strength Selection for Port C Bit 7 */
    } Bits;
} FNET_MCF_PTCDSSTR;
extern volatile FNET_MCF_PTCDSSTR _FNET_MCF_PTCDS @FNET_MCF_PTCDS_PTR;
#define FNET_MCF_PTCDS                           _FNET_MCF_PTCDS.Byte
#define FNET_MCF_PTCDS_PTCDS0                    _FNET_MCF_PTCDS.Bits.PTCDS0
#define FNET_MCF_PTCDS_PTCDS1                    _FNET_MCF_PTCDS.Bits.PTCDS1
#define FNET_MCF_PTCDS_PTCDS2                    _FNET_MCF_PTCDS.Bits.PTCDS2
#define FNET_MCF_PTCDS_PTCDS3                    _FNET_MCF_PTCDS.Bits.PTCDS3
#define FNET_MCF_PTCDS_PTCDS4                    _FNET_MCF_PTCDS.Bits.PTCDS4
#define FNET_MCF_PTCDS_PTCDS5                    _FNET_MCF_PTCDS.Bits.PTCDS5
#define FNET_MCF_PTCDS_PTCDS6                    _FNET_MCF_PTCDS.Bits.PTCDS6
#define FNET_MCF_PTCDS_PTCDS7                    _FNET_MCF_PTCDS.Bits.PTCDS7

#define FNET_MCF_PTCDS_PTCDS0_MASK               1
#define FNET_MCF_PTCDS_PTCDS1_MASK               2
#define FNET_MCF_PTCDS_PTCDS2_MASK               4
#define FNET_MCF_PTCDS_PTCDS3_MASK               8
#define FNET_MCF_PTCDS_PTCDS4_MASK               16
#define FNET_MCF_PTCDS_PTCDS5_MASK               32
#define FNET_MCF_PTCDS_PTCDS6_MASK               64
#define FNET_MCF_PTCDS_PTCDS7_MASK               128



/*** PTAPF1 - Port A Routing Register 1 ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t A40         : 1;                                      /* Port PTA4 Pin Mux Controls, bit 0 */
        fnet_uint8_t A41         : 1;                                      /* Port PTA4 Pin Mux Controls, bit 1 */
        fnet_uint8_t A50         : 1;                                      /* Port PTA5 Pin Mux Controls, bit 0 */
        fnet_uint8_t A51         : 1;                                      /* Port PTA5 Pin Mux Controls, bit 1 */
        fnet_uint8_t A60         : 1;                                      /* Port PTA6 Pin Mux Controls, bit 0 */
        fnet_uint8_t A61         : 1;                                      /* Port PTA6 Pin Mux Controls, bit 1 */
        fnet_uint8_t A70         : 1;                                      /* Port PTA7 Pin Mux Controls, bit 0 */
        fnet_uint8_t A71         : 1;                                      /* Port PTA7 Pin Mux Controls, bit 1 */
    } Bits;
    struct
    {
        fnet_uint8_t grpA4   : 2;
        fnet_uint8_t grpA5   : 2;
        fnet_uint8_t grpA6   : 2;
        fnet_uint8_t grpA7   : 2;
    } MergedBits;
} FNET_MCF_PTAPF1STR;
extern volatile FNET_MCF_PTAPF1STR _FNET_MCF_PTAPF1 @FNET_MCF_PTAPF1_PTR;
#define FNET_MCF_PTAPF1                          _FNET_MCF_PTAPF1.Byte
#define FNET_MCF_PTAPF1_A40                      _FNET_MCF_PTAPF1.Bits.A40
#define FNET_MCF_PTAPF1_A41                      _FNET_MCF_PTAPF1.Bits.A41
#define FNET_MCF_PTAPF1_A50                      _FNET_MCF_PTAPF1.Bits.A50
#define FNET_MCF_PTAPF1_A51                      _FNET_MCF_PTAPF1.Bits.A51
#define FNET_MCF_PTAPF1_A60                      _FNET_MCF_PTAPF1.Bits.A60
#define FNET_MCF_PTAPF1_A61                      _FNET_MCF_PTAPF1.Bits.A61
#define FNET_MCF_PTAPF1_A70                      _FNET_MCF_PTAPF1.Bits.A70
#define FNET_MCF_PTAPF1_A71                      _FNET_MCF_PTAPF1.Bits.A71
#define FNET_MCF_PTAPF1_A4                       _FNET_MCF_PTAPF1.MergedBits.grpA4
#define FNET_MCF_PTAPF1_A5                       _FNET_MCF_PTAPF1.MergedBits.grpA5
#define FNET_MCF_PTAPF1_A6                       _FNET_MCF_PTAPF1.MergedBits.grpA6
#define FNET_MCF_PTAPF1_A7                       _FNET_MCF_PTAPF1.MergedBits.grpA7
#define FNET_MCF_PTAPF1_A                        FNET_MCF_PTAPF1_A5

#define FNET_MCF_PTAPF1_A40_MASK                 1
#define FNET_MCF_PTAPF1_A41_MASK                 2
#define FNET_MCF_PTAPF1_A50_MASK                 4
#define FNET_MCF_PTAPF1_A51_MASK                 8
#define FNET_MCF_PTAPF1_A60_MASK                 16
#define FNET_MCF_PTAPF1_A61_MASK                 32
#define FNET_MCF_PTAPF1_A70_MASK                 64
#define FNET_MCF_PTAPF1_A71_MASK                 128
#define FNET_MCF_PTAPF1_A4_MASK                  3
#define FNET_MCF_PTAPF1_A4_BITNUM                0
#define FNET_MCF_PTAPF1_A5_MASK                  12
#define FNET_MCF_PTAPF1_A5_BITNUM                2
#define FNET_MCF_PTAPF1_A6_MASK                  48
#define FNET_MCF_PTAPF1_A6_BITNUM                4
#define FNET_MCF_PTAPF1_A7_MASK                  192
#define FNET_MCF_PTAPF1_A7_BITNUM                6

/*** PTAPF2 - Port A Routing Register 2 ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t A00         : 1;                                      /* Port PTA0 Pin Mux Controls, bit 0 */
        fnet_uint8_t A01         : 1;                                      /* Port PTA0 Pin Mux Controls, bit 1 */
        fnet_uint8_t A10         : 1;                                      /* Port PTA1 Pin Mux Controls, bit 0 */
        fnet_uint8_t A11         : 1;                                      /* Port PTA1 Pin Mux Controls, bit 1 */
        fnet_uint8_t A20         : 1;                                      /* Port PTA2 Pin Mux Controls, bit 0 */
        fnet_uint8_t A21         : 1;                                      /* Port PTA2 Pin Mux Controls, bit 1 */
        fnet_uint8_t A30         : 1;                                      /* Port PTA3 Pin Mux Controls, bit 0 */
        fnet_uint8_t A31         : 1;                                      /* Port PTA3 Pin Mux Controls, bit 1 */
    } Bits;
    struct
    {
        fnet_uint8_t grpA0   : 2;
        fnet_uint8_t grpA1   : 2;
        fnet_uint8_t grpA2   : 2;
        fnet_uint8_t grpA3   : 2;
    } MergedBits;
} FNET_MCF_PTAPF2STR;
extern volatile FNET_MCF_PTAPF2STR _FNET_MCF_PTAPF2 @FNET_MCF_PTAPF2_PTR;
#define FNET_MCF_PTAPF2                          _FNET_MCF_PTAPF2.Byte
#define FNET_MCF_PTAPF2_A00                      _FNET_MCF_PTAPF2.Bits.A00
#define FNET_MCF_PTAPF2_A01                      _FNET_MCF_PTAPF2.Bits.A01
#define FNET_MCF_PTAPF2_A10                      _FNET_MCF_PTAPF2.Bits.A10
#define FNET_MCF_PTAPF2_A11                      _FNET_MCF_PTAPF2.Bits.A11
#define FNET_MCF_PTAPF2_A20                      _FNET_MCF_PTAPF2.Bits.A20
#define FNET_MCF_PTAPF2_A21                      _FNET_MCF_PTAPF2.Bits.A21
#define FNET_MCF_PTAPF2_A30                      _FNET_MCF_PTAPF2.Bits.A30
#define FNET_MCF_PTAPF2_A31                      _FNET_MCF_PTAPF2.Bits.A31
#define FNET_MCF_PTAPF2_A0                       _FNET_MCF_PTAPF2.MergedBits.grpA0
#define FNET_MCF_PTAPF2_A1                       _FNET_MCF_PTAPF2.MergedBits.grpA1
#define FNET_MCF_PTAPF2_A2                       _FNET_MCF_PTAPF2.MergedBits.grpA2
#define FNET_MCF_PTAPF2_A3                       _FNET_MCF_PTAPF2.MergedBits.grpA3
#define FNET_MCF_PTAPF2_A                        FNET_MCF_PTAPF2_A1

#define FNET_MCF_PTAPF2_A00_MASK                 1
#define FNET_MCF_PTAPF2_A01_MASK                 2
#define FNET_MCF_PTAPF2_A10_MASK                 4
#define FNET_MCF_PTAPF2_A11_MASK                 8
#define FNET_MCF_PTAPF2_A20_MASK                 16
#define FNET_MCF_PTAPF2_A21_MASK                 32
#define FNET_MCF_PTAPF2_A30_MASK                 64
#define FNET_MCF_PTAPF2_A31_MASK                 128
#define FNET_MCF_PTAPF2_A0_MASK                  3
#define FNET_MCF_PTAPF2_A0_BITNUM                0
#define FNET_MCF_PTAPF2_A1_MASK                  12
#define FNET_MCF_PTAPF2_A1_BITNUM                2
#define FNET_MCF_PTAPF2_A2_MASK                  48
#define FNET_MCF_PTAPF2_A2_BITNUM                4
#define FNET_MCF_PTAPF2_A3_MASK                  192
#define FNET_MCF_PTAPF2_A3_BITNUM                6

/*** PTBPF1 - Port B Routing Register 1 ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t B40         : 1;                                      /* Port PTB4 Pin Mux Controls, bit 0 */
        fnet_uint8_t B41         : 1;                                      /* Port PTB4 Pin Mux Controls, bit 1 */
        fnet_uint8_t B50         : 1;                                      /* Port PTB5 Pin Mux Controls, bit 0 */
        fnet_uint8_t B51         : 1;                                      /* Port PTB5 Pin Mux Controls, bit 1 */
        fnet_uint8_t B60         : 1;                                      /* Port PTB6 Pin Mux Controls, bit 0 */
        fnet_uint8_t B61         : 1;                                      /* Port PTB6 Pin Mux Controls, bit 1 */
        fnet_uint8_t B70         : 1;                                      /* Port PTB7 Pin Mux Controls, bit 0 */
        fnet_uint8_t B71         : 1;                                      /* Port PTB7 Pin Mux Controls, bit 1 */
    } Bits;
    struct
    {
        fnet_uint8_t grpB4   : 2;
        fnet_uint8_t grpB5   : 2;
        fnet_uint8_t grpB6   : 2;
        fnet_uint8_t grpB7   : 2;
    } MergedBits;
} FNET_MCF_PTBPF1STR;
extern volatile FNET_MCF_PTBPF1STR _FNET_MCF_PTBPF1 @FNET_MCF_PTBPF1_PTR;
#define FNET_MCF_PTBPF1                          _FNET_MCF_PTBPF1.Byte
#define FNET_MCF_PTBPF1_B40                      _FNET_MCF_PTBPF1.Bits.B40
#define FNET_MCF_PTBPF1_B41                      _FNET_MCF_PTBPF1.Bits.B41
#define FNET_MCF_PTBPF1_B50                      _FNET_MCF_PTBPF1.Bits.B50
#define FNET_MCF_PTBPF1_B51                      _FNET_MCF_PTBPF1.Bits.B51
#define FNET_MCF_PTBPF1_B60                      _FNET_MCF_PTBPF1.Bits.B60
#define FNET_MCF_PTBPF1_B61                      _FNET_MCF_PTBPF1.Bits.B61
#define FNET_MCF_PTBPF1_B70                      _FNET_MCF_PTBPF1.Bits.B70
#define FNET_MCF_PTBPF1_B71                      _FNET_MCF_PTBPF1.Bits.B71
#define FNET_MCF_PTBPF1_B4                       _FNET_MCF_PTBPF1.MergedBits.grpB4
#define FNET_MCF_PTBPF1_B5                       _FNET_MCF_PTBPF1.MergedBits.grpB5
#define FNET_MCF_PTBPF1_B6                       _FNET_MCF_PTBPF1.MergedBits.grpB6
#define FNET_MCF_PTBPF1_B7                       _FNET_MCF_PTBPF1.MergedBits.grpB7
#define FNET_MCF_PTBPF1_B                        FNET_MCF_PTBPF1_B5

#define FNET_MCF_PTBPF1_B40_MASK                 1
#define FNET_MCF_PTBPF1_B41_MASK                 2
#define FNET_MCF_PTBPF1_B50_MASK                 4
#define FNET_MCF_PTBPF1_B51_MASK                 8
#define FNET_MCF_PTBPF1_B60_MASK                 16
#define FNET_MCF_PTBPF1_B61_MASK                 32
#define FNET_MCF_PTBPF1_B70_MASK                 64
#define FNET_MCF_PTBPF1_B71_MASK                 128
#define FNET_MCF_PTBPF1_B4_MASK                  3
#define FNET_MCF_PTBPF1_B4_BITNUM                0
#define FNET_MCF_PTBPF1_B5_MASK                  12
#define FNET_MCF_PTBPF1_B5_BITNUM                2
#define FNET_MCF_PTBPF1_B6_MASK                  48
#define FNET_MCF_PTBPF1_B6_BITNUM                4
#define FNET_MCF_PTBPF1_B7_MASK                  192
#define FNET_MCF_PTBPF1_B7_BITNUM                6

/*** PTBPF2 - Port B Routing Register 2 ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t B00         : 1;                                      /* Port PTB0 Pin Mux Controls, bit 0 */
        fnet_uint8_t B01         : 1;                                      /* Port PTB0 Pin Mux Controls, bit 1 */
        fnet_uint8_t B10         : 1;                                      /* Port PTB1 Pin Mux Controls, bit 0 */
        fnet_uint8_t B11         : 1;                                      /* Port PTB1 Pin Mux Controls, bit 1 */
        fnet_uint8_t B20         : 1;                                      /* Port PTB2 Pin Mux Controls, bit 0 */
        fnet_uint8_t B21         : 1;                                      /* Port PTB2 Pin Mux Controls, bit 1 */
        fnet_uint8_t B30         : 1;                                      /* Port PTB3 Pin Mux Controls, bit 0 */
        fnet_uint8_t B31         : 1;                                      /* Port PTB3 Pin Mux Controls, bit 1 */
    } Bits;
    struct
    {
        fnet_uint8_t grpB0   : 2;
        fnet_uint8_t grpB1   : 2;
        fnet_uint8_t grpB2   : 2;
        fnet_uint8_t grpB3   : 2;
    } MergedBits;
} FNET_MCF_PTBPF2STR;
extern volatile FNET_MCF_PTBPF2STR _FNET_MCF_PTBPF2 @FNET_MCF_PTBPF2_PTR;
#define FNET_MCF_PTBPF2                          _FNET_MCF_PTBPF2.Byte
#define FNET_MCF_PTBPF2_B00                      _FNET_MCF_PTBPF2.Bits.B00
#define FNET_MCF_PTBPF2_B01                      _FNET_MCF_PTBPF2.Bits.B01
#define FNET_MCF_PTBPF2_B10                      _FNET_MCF_PTBPF2.Bits.B10
#define FNET_MCF_PTBPF2_B11                      _FNET_MCF_PTBPF2.Bits.B11
#define FNET_MCF_PTBPF2_B20                      _FNET_MCF_PTBPF2.Bits.B20
#define FNET_MCF_PTBPF2_B21                      _FNET_MCF_PTBPF2.Bits.B21
#define FNET_MCF_PTBPF2_B30                      _FNET_MCF_PTBPF2.Bits.B30
#define FNET_MCF_PTBPF2_B31                      _FNET_MCF_PTBPF2.Bits.B31
#define FNET_MCF_PTBPF2_B0                       _FNET_MCF_PTBPF2.MergedBits.grpB0
#define FNET_MCF_PTBPF2_B1                       _FNET_MCF_PTBPF2.MergedBits.grpB1
#define FNET_MCF_PTBPF2_B2                       _FNET_MCF_PTBPF2.MergedBits.grpB2
#define FNET_MCF_PTBPF2_B3                       _FNET_MCF_PTBPF2.MergedBits.grpB3
#define FNET_MCF_PTBPF2_B                        FNET_MCF_PTBPF2_B1

#define FNET_MCF_PTBPF2_B00_MASK                 1
#define FNET_MCF_PTBPF2_B01_MASK                 2
#define FNET_MCF_PTBPF2_B10_MASK                 4
#define FNET_MCF_PTBPF2_B11_MASK                 8
#define FNET_MCF_PTBPF2_B20_MASK                 16
#define FNET_MCF_PTBPF2_B21_MASK                 32
#define FNET_MCF_PTBPF2_B30_MASK                 64
#define FNET_MCF_PTBPF2_B31_MASK                 128
#define FNET_MCF_PTBPF2_B0_MASK                  3
#define FNET_MCF_PTBPF2_B0_BITNUM                0
#define FNET_MCF_PTBPF2_B1_MASK                  12
#define FNET_MCF_PTBPF2_B1_BITNUM                2
#define FNET_MCF_PTBPF2_B2_MASK                  48
#define FNET_MCF_PTBPF2_B2_BITNUM                4
#define FNET_MCF_PTBPF2_B3_MASK                  192
#define FNET_MCF_PTBPF2_B3_BITNUM                6

/*** PTCPF2 - Port C Routing Register 2 ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t C00         : 1;                                      /* Port PTC0 Pin Mux Controls, bit 0 */
        fnet_uint8_t C01         : 1;                                      /* Port PTC0 Pin Mux Controls, bit 1 */
        fnet_uint8_t C10         : 1;                                      /* Port PTC1 Pin Mux Controls, bit 0 */
        fnet_uint8_t C11         : 1;                                      /* Port PTC1 Pin Mux Controls, bit 1 */
        fnet_uint8_t C20         : 1;                                      /* Port PTC2 Pin Mux Controls, bit 0 */
        fnet_uint8_t C21         : 1;                                      /* Port PTC2 Pin Mux Controls, bit 1 */
        fnet_uint8_t C30         : 1;                                      /* Port PTC3 Pin Mux Controls, bit 0 */
        fnet_uint8_t C31         : 1;                                      /* Port PTC3 Pin Mux Controls, bit 1 */
    } Bits;
    struct
    {
        fnet_uint8_t grpC0   : 2;
        fnet_uint8_t grpC1   : 2;
        fnet_uint8_t grpC2   : 2;
        fnet_uint8_t grpC3   : 2;
    } MergedBits;
} FNET_MCF_PTCPF2STR;
extern volatile FNET_MCF_PTCPF2STR _FNET_MCF_PTCPF2 @FNET_MCF_PTCPF2_PTR;
#define FNET_MCF_PTCPF2                          _FNET_MCF_PTCPF2.Byte
#define FNET_MCF_PTCPF2_C00                      _FNET_MCF_PTCPF2.Bits.C00
#define FNET_MCF_PTCPF2_C01                      _FNET_MCF_PTCPF2.Bits.C01
#define FNET_MCF_PTCPF2_C10                      _FNET_MCF_PTCPF2.Bits.C10
#define FNET_MCF_PTCPF2_C11                      _FNET_MCF_PTCPF2.Bits.C11
#define FNET_MCF_PTCPF2_C20                      _FNET_MCF_PTCPF2.Bits.C20
#define FNET_MCF_PTCPF2_C21                      _FNET_MCF_PTCPF2.Bits.C21
#define FNET_MCF_PTCPF2_C30                      _FNET_MCF_PTCPF2.Bits.C30
#define FNET_MCF_PTCPF2_C31                      _FNET_MCF_PTCPF2.Bits.C31
#define FNET_MCF_PTCPF2_C0                       _FNET_MCF_PTCPF2.MergedBits.grpC0
#define FNET_MCF_PTCPF2_C1                       _FNET_MCF_PTCPF2.MergedBits.grpC1
#define FNET_MCF_PTCPF2_C2                       _FNET_MCF_PTCPF2.MergedBits.grpC2
#define FNET_MCF_PTCPF2_C3                       _FNET_MCF_PTCPF2.MergedBits.grpC3
#define FNET_MCF_PTCPF2_C                        FNET_MCF_PTCPF2_C1

#define FNET_MCF_PTCPF2_C00_MASK                 1
#define FNET_MCF_PTCPF2_C01_MASK                 2
#define FNET_MCF_PTCPF2_C10_MASK                 4
#define FNET_MCF_PTCPF2_C11_MASK                 8
#define FNET_MCF_PTCPF2_C20_MASK                 16
#define FNET_MCF_PTCPF2_C21_MASK                 32
#define FNET_MCF_PTCPF2_C30_MASK                 64
#define FNET_MCF_PTCPF2_C31_MASK                 128
#define FNET_MCF_PTCPF2_C0_MASK                  3
#define FNET_MCF_PTCPF2_C0_BITNUM                0
#define FNET_MCF_PTCPF2_C1_MASK                  12
#define FNET_MCF_PTCPF2_C1_BITNUM                2
#define FNET_MCF_PTCPF2_C2_MASK                  48
#define FNET_MCF_PTCPF2_C2_BITNUM                4
#define FNET_MCF_PTCPF2_C3_MASK                  192
#define FNET_MCF_PTCPF2_C3_BITNUM                6


/*** PTDPF2 - Port D Routing Register 2; 0xFFFF80C7 ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t D00         : 1;                                      /* Port PTD0 Pin Mux Controls, bit 0 */
        fnet_uint8_t D01         : 1;                                      /* Port PTD0 Pin Mux Controls, bit 1 */
        fnet_uint8_t D10         : 1;                                      /* Port PTD1 Pin Mux Controls, bit 0 */
        fnet_uint8_t D11         : 1;                                      /* Port PTD1 Pin Mux Controls, bit 1 */
        fnet_uint8_t D20         : 1;                                      /* Port PTD2 Pin Mux Controls, bit 0 */
        fnet_uint8_t D21         : 1;                                      /* Port PTD2 Pin Mux Controls, bit 1 */
        fnet_uint8_t D30         : 1;                                      /* Port PTD3 Pin Mux Controls, bit 0 */
        fnet_uint8_t D31         : 1;                                      /* Port PTD3 Pin Mux Controls, bit 1 */
    } Bits;
    struct
    {
        fnet_uint8_t grpD0   : 2;
        fnet_uint8_t grpD1   : 2;
        fnet_uint8_t grpD2   : 2;
        fnet_uint8_t grpD3   : 2;
    } MergedBits;
} FNET_MCF_PTDPF2STR;
extern volatile FNET_MCF_PTDPF2STR _FNET_MCF_PTDPF2 @FNET_MCF_PTDPF2_PTR;
#define FNET_MCF_PTDPF2                          _FNET_MCF_PTDPF2.Byte
#define FNET_MCF_PTDPF2_D00                      _FNET_MCF_PTDPF2.Bits.D00
#define FNET_MCF_PTDPF2_D01                      _FNET_MCF_PTDPF2.Bits.D01
#define FNET_MCF_PTDPF2_D10                      _FNET_MCF_PTDPF2.Bits.D10
#define FNET_MCF_PTDPF2_D11                      _FNET_MCF_PTDPF2.Bits.D11
#define FNET_MCF_PTDPF2_D20                      _FNET_MCF_PTDPF2.Bits.D20
#define FNET_MCF_PTDPF2_D21                      _FNET_MCF_PTDPF2.Bits.D21
#define FNET_MCF_PTDPF2_D30                      _FNET_MCF_PTDPF2.Bits.D30
#define FNET_MCF_PTDPF2_D31                      _FNET_MCF_PTDPF2.Bits.D31
#define FNET_MCF_PTDPF2_D0                       _FNET_MCF_PTDPF2.MergedBits.grpD0
#define FNET_MCF_PTDPF2_D1                       _FNET_MCF_PTDPF2.MergedBits.grpD1
#define FNET_MCF_PTDPF2_D2                       _FNET_MCF_PTDPF2.MergedBits.grpD2
#define FNET_MCF_PTDPF2_D3                       _FNET_MCF_PTDPF2.MergedBits.grpD3

#define FNET_MCF_PTDPF2_D00_MASK                 1
#define FNET_MCF_PTDPF2_D01_MASK                 2
#define FNET_MCF_PTDPF2_D10_MASK                 4
#define FNET_MCF_PTDPF2_D11_MASK                 8
#define FNET_MCF_PTDPF2_D20_MASK                 16
#define FNET_MCF_PTDPF2_D21_MASK                 32
#define FNET_MCF_PTDPF2_D30_MASK                 64
#define FNET_MCF_PTDPF2_D31_MASK                 128
#define FNET_MCF_PTDPF2_D0_MASK                  3
#define FNET_MCF_PTDPF2_D0_BITNUM                0
#define FNET_MCF_PTDPF2_D1_MASK                  12
#define FNET_MCF_PTDPF2_D1_BITNUM                2
#define FNET_MCF_PTDPF2_D2_MASK                  48
#define FNET_MCF_PTDPF2_D2_BITNUM                4
#define FNET_MCF_PTDPF2_D3_MASK                  192
#define FNET_MCF_PTDPF2_D3_BITNUM                6

/*** PTEPF1 - Port E Routing Register 1; 0xFFFF80C8 ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t E40         : 1;                                      /* Port PTE4 Pin Mux Controls, bit 0 */
        fnet_uint8_t E41         : 1;                                      /* Port PTE4 Pin Mux Controls, bit 1 */
        fnet_uint8_t E50         : 1;                                      /* Port PTE5 Pin Mux Controls, bit 0 */
        fnet_uint8_t E51         : 1;                                      /* Port PTE5 Pin Mux Controls, bit 1 */
        fnet_uint8_t E60         : 1;                                      /* Port PTE6 Pin Mux Controls, bit 0 */
        fnet_uint8_t E61         : 1;                                      /* Port PTE6 Pin Mux Controls, bit 1 */
        fnet_uint8_t E70         : 1;                                      /* Port PTE7 Pin Mux Controls, bit 0 */
        fnet_uint8_t E71         : 1;                                      /* Port PTE7 Pin Mux Controls, bit 1 */
    } Bits;
    struct
    {
        fnet_uint8_t grpE4   : 2;
        fnet_uint8_t grpE5   : 2;
        fnet_uint8_t grpE6   : 2;
        fnet_uint8_t grpE7   : 2;
    } MergedBits;
} FNET_MCF_PTEPF1STR;
extern volatile FNET_MCF_PTEPF1STR _FNET_MCF_PTEPF1 @FNET_MCF_PTEPF1_PTR;
#define FNET_MCF_PTEPF1                          _FNET_MCF_PTEPF1.Byte
#define FNET_MCF_PTEPF1_E40                      _FNET_MCF_PTEPF1.Bits.E40
#define FNET_MCF_PTEPF1_E41                      _FNET_MCF_PTEPF1.Bits.E41
#define FNET_MCF_PTEPF1_E50                      _FNET_MCF_PTEPF1.Bits.E50
#define FNET_MCF_PTEPF1_E51                      _FNET_MCF_PTEPF1.Bits.E51
#define FNET_MCF_PTEPF1_E60                      _FNET_MCF_PTEPF1.Bits.E60
#define FNET_MCF_PTEPF1_E61                      _FNET_MCF_PTEPF1.Bits.E61
#define FNET_MCF_PTEPF1_E70                      _FNET_MCF_PTEPF1.Bits.E70
#define FNET_MCF_PTEPF1_E71                      _FNET_MCF_PTEPF1.Bits.E71
#define FNET_MCF_PTEPF1_E4                       _FNET_MCF_PTEPF1.MergedBits.grpE4
#define FNET_MCF_PTEPF1_E5                       _FNET_MCF_PTEPF1.MergedBits.grpE5
#define FNET_MCF_PTEPF1_E6                       _FNET_MCF_PTEPF1.MergedBits.grpE6
#define FNET_MCF_PTEPF1_E7                       _FNET_MCF_PTEPF1.MergedBits.grpE7

#define FNET_MCF_PTEPF1_E40_MASK                 1
#define FNET_MCF_PTEPF1_E41_MASK                 2
#define FNET_MCF_PTEPF1_E50_MASK                 4
#define FNET_MCF_PTEPF1_E51_MASK                 8
#define FNET_MCF_PTEPF1_E60_MASK                 16
#define FNET_MCF_PTEPF1_E61_MASK                 32
#define FNET_MCF_PTEPF1_E70_MASK                 64
#define FNET_MCF_PTEPF1_E71_MASK                 128
#define FNET_MCF_PTEPF1_E4_MASK                  3
#define FNET_MCF_PTEPF1_E4_BITNUM                0
#define FNET_MCF_PTEPF1_E5_MASK                  12
#define FNET_MCF_PTEPF1_E5_BITNUM                2
#define FNET_MCF_PTEPF1_E6_MASK                  48
#define FNET_MCF_PTEPF1_E6_BITNUM                4
#define FNET_MCF_PTEPF1_E7_MASK                  192
#define FNET_MCF_PTEPF1_E7_BITNUM                6



/*** PTAIFE - Port A Input Filter Enable Register; 0xFFFF800B ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t PTAIFE0     : 1;                                      /* Port A Input Filter Enable Bit 0 */
        fnet_uint8_t PTAIFE1     : 1;                                      /* Port A Input Filter Enable Bit 1 */
        fnet_uint8_t PTAIFE2     : 1;                                      /* Port A Input Filter Enable Bit 2 */
        fnet_uint8_t PTAIFE3     : 1;                                      /* Port A Input Filter Enable Bit 3 */
        fnet_uint8_t PTAIFE4     : 1;                                      /* Port A Input Filter Enable Bit 4 */
        fnet_uint8_t PTAIFE5     : 1;                                      /* Port A Input Filter Enable Bit 5 */
        fnet_uint8_t PTAIFE6     : 1;                                      /* Port A Input Filter Enable Bit 6 */
        fnet_uint8_t PTAIFE7     : 1;                                      /* Port A Input Filter Enable Bit 7 */
    } Bits;
} FNET_MCF_PTAIFESTR;
extern volatile FNET_MCF_PTAIFESTR _FNET_MCF_PTAIFE @FNET_MCF_PTAIFE_PTR;
#define FNET_MCF_PTAIFE                 _FNET_MCF_PTAIFE.Byte
#define FNET_MCF_PTAIFE_PTAIFE0         _FNET_MCF_PTAIFE.Bits.PTAIFE0
#define FNET_MCF_PTAIFE_PTAIFE1         _FNET_MCF_PTAIFE.Bits.PTAIFE1
#define FNET_MCF_PTAIFE_PTAIFE2         _FNET_MCF_PTAIFE.Bits.PTAIFE2
#define FNET_MCF_PTAIFE_PTAIFE3         _FNET_MCF_PTAIFE.Bits.PTAIFE3
#define FNET_MCF_PTAIFE_PTAIFE4         _FNET_MCF_PTAIFE.Bits.PTAIFE4
#define FNET_MCF_PTAIFE_PTAIFE5         _FNET_MCF_PTAIFE.Bits.PTAIFE5
#define FNET_MCF_PTAIFE_PTAIFE6         _FNET_MCF_PTAIFE.Bits.PTAIFE6
#define FNET_MCF_PTAIFE_PTAIFE7         _FNET_MCF_PTAIFE.Bits.PTAIFE7

#define FNET_MCF_PTAIFE_PTAIFE0_MASK             1
#define FNET_MCF_PTAIFE_PTAIFE1_MASK             2
#define FNET_MCF_PTAIFE_PTAIFE2_MASK             4
#define FNET_MCF_PTAIFE_PTAIFE3_MASK             8
#define FNET_MCF_PTAIFE_PTAIFE4_MASK             16
#define FNET_MCF_PTAIFE_PTAIFE5_MASK             32
#define FNET_MCF_PTAIFE_PTAIFE6_MASK             64
#define FNET_MCF_PTAIFE_PTAIFE7_MASK             128


/*** PTBIFE - Port B Input Filter Enable Register; 0xFFFF801B ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t PTBIFE0     : 1;                                      /* Port A Input Filter Enable Bit 0 */
        fnet_uint8_t PTBIFE1     : 1;                                      /* Port A Input Filter Enable Bit 1 */
        fnet_uint8_t PTBIFE2     : 1;                                      /* Port A Input Filter Enable Bit 2 */
        fnet_uint8_t PTBIFE3     : 1;                                      /* Port A Input Filter Enable Bit 3 */
        fnet_uint8_t PTBIFE4     : 1;                                      /* Port A Input Filter Enable Bit 4 */
        fnet_uint8_t PTBIFE5     : 1;                                      /* Port A Input Filter Enable Bit 5 */
        fnet_uint8_t PTBIFE6     : 1;                                      /* Port A Input Filter Enable Bit 6 */
        fnet_uint8_t PTBIFE7     : 1;                                      /* Port A Input Filter Enable Bit 7 */
    } Bits;
} FNET_MCF_PTBIFESTR;
extern volatile FNET_MCF_PTBIFESTR _FNET_MCF_PTBIFE @FNET_MCF_PTBIFE_PTR;
#define FNET_MCF_PTBIFE                          _FNET_MCF_PTBIFE.Byte
#define FNET_MCF_PTBIFE_PTBIFE0                  _FNET_MCF_PTBIFE.Bits.PTBIFE0
#define FNET_MCF_PTBIFE_PTBIFE1                  _FNET_MCF_PTBIFE.Bits.PTBIFE1
#define FNET_MCF_PTBIFE_PTBIFE2                  _FNET_MCF_PTBIFE.Bits.PTBIFE2
#define FNET_MCF_PTBIFE_PTBIFE3                  _FNET_MCF_PTBIFE.Bits.PTBIFE3
#define FNET_MCF_PTBIFE_PTBIFE4                  _FNET_MCF_PTBIFE.Bits.PTBIFE4
#define FNET_MCF_PTBIFE_PTBIFE5                  _FNET_MCF_PTBIFE.Bits.PTBIFE5
#define FNET_MCF_PTBIFE_PTBIFE6                  _FNET_MCF_PTBIFE.Bits.PTBIFE6
#define FNET_MCF_PTBIFE_PTBIFE7                  _FNET_MCF_PTBIFE.Bits.PTBIFE7

#define FNET_MCF_PTBIFE_PTBIFE0_MASK             1
#define FNET_MCF_PTBIFE_PTBIFE1_MASK             2
#define FNET_MCF_PTBIFE_PTBIFE2_MASK             4
#define FNET_MCF_PTBIFE_PTBIFE3_MASK             8
#define FNET_MCF_PTBIFE_PTBIFE4_MASK             16
#define FNET_MCF_PTBIFE_PTBIFE5_MASK             32
#define FNET_MCF_PTBIFE_PTBIFE6_MASK             64
#define FNET_MCF_PTBIFE_PTBIFE7_MASK             128


/*** PTCIFE - Port C Input Filter Enable Register; 0xFFFF802B ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t PTCIFE0     : 1;                                      /* Port A Input Filter Enable Bit 0 */
        fnet_uint8_t PTCIFE1     : 1;                                      /* Port A Input Filter Enable Bit 1 */
        fnet_uint8_t PTCIFE2     : 1;                                      /* Port A Input Filter Enable Bit 2 */
        fnet_uint8_t PTCIFE3     : 1;                                      /* Port A Input Filter Enable Bit 3 */
        fnet_uint8_t PTCIFE4     : 1;                                      /* Port A Input Filter Enable Bit 4 */
        fnet_uint8_t PTCIFE5     : 1;                                      /* Port A Input Filter Enable Bit 5 */
        fnet_uint8_t PTCIFE6     : 1;                                      /* Port A Input Filter Enable Bit 6 */
        fnet_uint8_t PTCIFE7     : 1;                                      /* Port A Input Filter Enable Bit 7 */
    } Bits;
} FNET_MCF_PTCIFESTR;
extern volatile FNET_MCF_PTCIFESTR _FNET_MCF_PTCIFE @FNET_MCF_PTCIFE_PTR;
#define FNET_MCF_PTCIFE                          _FNET_MCF_PTCIFE.Byte
#define FNET_MCF_PTCIFE_PTCIFE0                  _FNET_MCF_PTCIFE.Bits.PTCIFE0
#define FNET_MCF_PTCIFE_PTCIFE1                  _FNET_MCF_PTCIFE.Bits.PTCIFE1
#define FNET_MCF_PTCIFE_PTCIFE2                  _FNET_MCF_PTCIFE.Bits.PTCIFE2
#define FNET_MCF_PTCIFE_PTCIFE3                  _FNET_MCF_PTCIFE.Bits.PTCIFE3
#define FNET_MCF_PTCIFE_PTCIFE4                  _FNET_MCF_PTCIFE.Bits.PTCIFE4
#define FNET_MCF_PTCIFE_PTCIFE5                  _FNET_MCF_PTCIFE.Bits.PTCIFE5
#define FNET_MCF_PTCIFE_PTCIFE6                  _FNET_MCF_PTCIFE.Bits.PTCIFE6
#define FNET_MCF_PTCIFE_PTCIFE7                  _FNET_MCF_PTCIFE.Bits.PTCIFE7

#define FNET_MCF_PTCIFE_PTCIFE0_MASK             1
#define FNET_MCF_PTCIFE_PTCIFE1_MASK             2
#define FNET_MCF_PTCIFE_PTCIFE2_MASK             4
#define FNET_MCF_PTCIFE_PTCIFE3_MASK             8
#define FNET_MCF_PTCIFE_PTCIFE4_MASK             16
#define FNET_MCF_PTCIFE_PTCIFE5_MASK             32
#define FNET_MCF_PTCIFE_PTCIFE6_MASK             64
#define FNET_MCF_PTCIFE_PTCIFE7_MASK             128


/*********************************************************************
*
* Multipurpose Clock Generator (MCG) MCFV1
*
*********************************************************************/
/*** MCGC1 - MCG Control Register 1 ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t IREFSTEN    : 1;                                      /* Internal Reference Stop Enable */
        fnet_uint8_t IRCLKEN     : 1;                                      /* Internal Reference Clock Enable */
        fnet_uint8_t IREFS       : 1;                                      /* Internal Reference Select */
        fnet_uint8_t RDIV0       : 1;                                      /* Reference Divider, bit 0 */
        fnet_uint8_t RDIV1       : 1;                                      /* Reference Divider, bit 1 */
        fnet_uint8_t RDIV2       : 1;                                      /* Reference Divider, bit 2 */
        fnet_uint8_t CLKS0       : 1;                                      /* Clock Source Select, bit 0 */
        fnet_uint8_t CLKS1       : 1;                                      /* Clock Source Select, bit 1 */
    } Bits;
    struct
    {
        fnet_uint8_t         : 1;
        fnet_uint8_t         : 1;
        fnet_uint8_t         : 1;
        fnet_uint8_t grpRDIV : 3;
        fnet_uint8_t grpCLKS : 2;
    } MergedBits;
} FNET_MCF_MCGC1STR;
extern volatile FNET_MCF_MCGC1STR _FNET_MCF_MCGC1 @FNET_MCF_MCGC1_PTR;
#define FNET_MCF_MCGC1                           _FNET_MCF_MCGC1.Byte
#define FNET_MCF_MCGC1_IREFSTEN                  _FNET_MCF_MCGC1.Bits.IREFSTEN
#define FNET_MCF_MCGC1_IRCLKEN                   _FNET_MCF_MCGC1.Bits.IRCLKEN
#define FNET_MCF_MCGC1_IREFS                     _FNET_MCF_MCGC1.Bits.IREFS
#define FNET_MCF_MCGC1_RDIV0                     _FNET_MCF_MCGC1.Bits.RDIV0
#define FNET_MCF_MCGC1_RDIV1                     _FNET_MCF_MCGC1.Bits.RDIV1
#define FNET_MCF_MCGC1_RDIV2                     _FNET_MCF_MCGC1.Bits.RDIV2
#define FNET_MCF_MCGC1_CLKS0                     _FNET_MCF_MCGC1.Bits.CLKS0
#define FNET_MCF_MCGC1_CLKS1                     _FNET_MCF_MCGC1.Bits.CLKS1
#define FNET_MCF_MCGC1_RDIV                      _FNET_MCF_MCGC1.MergedBits.grpRDIV
#define FNET_MCF_MCGC1_CLKS                      _FNET_MCF_MCGC1.MergedBits.grpCLKS

#define FNET_MCF_MCGC1_IREFSTEN_MASK             1
#define FNET_MCF_MCGC1_IRCLKEN_MASK              2
#define FNET_MCF_MCGC1_IREFS_MASK                4
#define FNET_MCF_MCGC1_RDIV0_MASK                8
#define FNET_MCF_MCGC1_RDIV1_MASK                16
#define FNET_MCF_MCGC1_RDIV2_MASK                32
#define FNET_MCF_MCGC1_CLKS0_MASK                64
#define FNET_MCF_MCGC1_CLKS1_MASK                128
#define FNET_MCF_MCGC1_RDIV_MASK                 56
#define FNET_MCF_MCGC1_RDIV_BITNUM               3
#define FNET_MCF_MCGC1_CLKS_MASK                 192
#define FNET_MCF_MCGC1_CLKS_BITNUM               6

/*** MCGTRM - MCG Trim Register ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t TRIM0       : 1;                                      /* MCG Trim Setting, bit 0 */
        fnet_uint8_t TRIM1       : 1;                                      /* MCG Trim Setting, bit 1 */
        fnet_uint8_t TRIM2       : 1;                                      /* MCG Trim Setting, bit 2 */
        fnet_uint8_t TRIM3       : 1;                                      /* MCG Trim Setting, bit 3 */
        fnet_uint8_t TRIM4       : 1;                                      /* MCG Trim Setting, bit 4 */
        fnet_uint8_t TRIM5       : 1;                                      /* MCG Trim Setting, bit 5 */
        fnet_uint8_t TRIM6       : 1;                                      /* MCG Trim Setting, bit 6 */
        fnet_uint8_t TRIM7       : 1;                                      /* MCG Trim Setting, bit 7 */
    } Bits;
} FNET_MCF_MCGTRMSTR;
extern volatile FNET_MCF_MCGTRMSTR _FNET_MCF_MCGTRM @FNET_MCF_MCGTRM_PTR;
#define FNET_MCF_MCGTRM                          _FNET_MCF_MCGTRM.Byte
#define FNET_MCF_MCGTRM_TRIM0                    _FNET_MCF_MCGTRM.Bits.TRIM0
#define FNET_MCF_MCGTRM_TRIM1                    _FNET_MCF_MCGTRM.Bits.TRIM1
#define FNET_MCF_MCGTRM_TRIM2                    _FNET_MCF_MCGTRM.Bits.TRIM2
#define FNET_MCF_MCGTRM_TRIM3                    _FNET_MCF_MCGTRM.Bits.TRIM3
#define FNET_MCF_MCGTRM_TRIM4                    _FNET_MCF_MCGTRM.Bits.TRIM4
#define FNET_MCF_MCGTRM_TRIM5                    _FNET_MCF_MCGTRM.Bits.TRIM5
#define FNET_MCF_MCGTRM_TRIM6                    _FNET_MCF_MCGTRM.Bits.TRIM6
#define FNET_MCF_MCGTRM_TRIM7                    _FNET_MCF_MCGTRM.Bits.TRIM7

#define FNET_MCF_MCGTRM_TRIM0_MASK               1
#define FNET_MCF_MCGTRM_TRIM1_MASK               2
#define FNET_MCF_MCGTRM_TRIM2_MASK               4
#define FNET_MCF_MCGTRM_TRIM3_MASK               8
#define FNET_MCF_MCGTRM_TRIM4_MASK               16
#define FNET_MCF_MCGTRM_TRIM5_MASK               32
#define FNET_MCF_MCGTRM_TRIM6_MASK               64
#define FNET_MCF_MCGTRM_TRIM7_MASK               128

/*** NVMCGTRM - Nonvolatile MCG Trim Register ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t TRIM0       : 1;                                      /* MCG Trim Setting, bit 0 */
        fnet_uint8_t TRIM1       : 1;                                      /* MCG Trim Setting, bit 1 */
        fnet_uint8_t TRIM2       : 1;                                      /* MCG Trim Setting, bit 2 */
        fnet_uint8_t TRIM3       : 1;                                      /* MCG Trim Setting, bit 3 */
        fnet_uint8_t TRIM4       : 1;                                      /* MCG Trim Setting, bit 4 */
        fnet_uint8_t TRIM5       : 1;                                      /* MCG Trim Setting, bit 5 */
        fnet_uint8_t TRIM6       : 1;                                      /* MCG Trim Setting, bit 6 */
        fnet_uint8_t TRIM7       : 1;                                      /* MCG Trim Setting, bit 7 */
    } Bits;
} FNET_MCF_NVMCGTRMSTR;
/* Tip for register initialization in the user code:  const byte NVMCGTRM_INIT @0x000003FF = <NVMCGTRM_INITVAL>; */
#define _FNET_MCF_NVMCGTRM (*(const FNET_MCF_NVMCGTRMSTR *)FNET_MCF_NVMCGTRM_PTR)
#define FNET_MCF_NVMCGTRM                        _FNET_MCF_NVMCGTRM.Byte
#define FNET_MCF_NVMCGTRM_TRIM0                  _FNET_MCF_NVMCGTRM.Bits.TRIM0
#define FNET_MCF_NVMCGTRM_TRIM1                  _FNET_MCF_NVMCGTRM.Bits.TRIM1
#define FNET_MCF_NVMCGTRM_TRIM2                  _FNET_MCF_NVMCGTRM.Bits.TRIM2
#define FNET_MCF_NVMCGTRM_TRIM3                  _FNET_MCF_NVMCGTRM.Bits.TRIM3
#define FNET_MCF_NVMCGTRM_TRIM4                  _FNET_MCF_NVMCGTRM.Bits.TRIM4
#define FNET_MCF_NVMCGTRM_TRIM5                  _FNET_MCF_NVMCGTRM.Bits.TRIM5
#define FNET_MCF_NVMCGTRM_TRIM6                  _FNET_MCF_NVMCGTRM.Bits.TRIM6
#define FNET_MCF_NVMCGTRM_TRIM7                  _FNET_MCF_NVMCGTRM.Bits.TRIM7

#define FNET_MCF_NVMCGTRM_TRIM0_MASK             1
#define FNET_MCF_NVMCGTRM_TRIM1_MASK             2
#define FNET_MCF_NVMCGTRM_TRIM2_MASK             4
#define FNET_MCF_NVMCGTRM_TRIM3_MASK             8
#define FNET_MCF_NVMCGTRM_TRIM4_MASK             16
#define FNET_MCF_NVMCGTRM_TRIM5_MASK             32
#define FNET_MCF_NVMCGTRM_TRIM6_MASK             64
#define FNET_MCF_NVMCGTRM_TRIM7_MASK             128


/*********************************************************************
*
* Serial Communication Interface (SCI) MCFV1
*
*********************************************************************/

/*** SCI_S1 - SCI Status Register 1 ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t PF          : 1;                                      /* Parity Error Flag */
        fnet_uint8_t FE          : 1;                                      /* Framing Error Flag */
        fnet_uint8_t NF          : 1;                                      /* Noise Flag */
        fnet_uint8_t OR          : 1;                                      /* Receiver Overrun Flag */
        fnet_uint8_t IDLE        : 1;                                      /* Idle Line Flag */
        fnet_int8_t RDRF        : 1;                                      /* Receive Data Register Full Flag */
        fnet_uint8_t TC          : 1;                                      /* Transmission Complete Flag */
        fnet_uint8_t TDRE        : 1;                                      /* Transmit Data Register Empty Flag */
    } Bits;
} FNET_MCF_SCI_S1STR;

#define _FNET_MCF_SCI_S1(x)                         (*(volatile FNET_MCF_SCI_S1STR*)(FNET_MCF_SCI1S1_PTR+((x)*0x20)))//(*(vuint8 *)(&FNET_CFG_MCF_IPSBAR[0x204 + ((x)*0x40)]))

#define FNET_MCF_SCI_S1(x)                          _FNET_MCF_SCI_S1(x).Byte
#define FNET_MCF_SCI_S1_PF(x)                       _FNET_MCF_SCI_S1(x).Bits.PF
#define FNET_MCF_SCI_S1_FE(x)                       _FNET_MCF_SCI_S1(x).Bits.FE
#define FNET_MCF_SCI_S1_NF(x)                       _FNET_MCF_SCI_S1(x).Bits.NF
#define FNET_MCF_SCI_S1_OR(x)                       _FNET_MCF_SCI_S1(x).Bits.OR
#define FNET_MCF_SCI_S1_IDLE(x)                     _FNET_MCF_SCI_S1(x).Bits.IDLE
#define FNET_MCF_SCI_S1_RDRF(x)                     _FNET_MCF_SCI_S1(x).Bits.RDRF
#define FNET_MCF_SCI_S1_TC(x)                       _FNET_MCF_SCI_S1(x).Bits.TC
#define FNET_MCF_SCI_S1_TDRE(x)                     _FNET_MCF_SCI_S1(x).Bits.TDRE


#define FNET_MCF_SCI_S1_PF_MASK                     (1)
#define FNET_MCF_SCI_S1_FE_MASK                     (2)
#define FNET_MCF_SCI_S1_NF_MASK                     (4)
#define FNET_MCF_SCI_S1_OR_MASK                     (8)
#define FNET_MCF_SCI_S1_IDLE_MASK                   (16)
#define FNET_MCF_SCI_S1_RDRF_MASK                   (32)
#define FNET_MCF_SCI_S1_TC_MASK                     (64)
#define FNET_MCF_SCI_S1_TDRE_MASK                   (128)

/*** SCI_D - SCI Data Register ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t R0_T0       : 1;                                      /* Receive/Transmit Data Bit 0 */
        fnet_uint8_t R1_T1       : 1;                                      /* Receive/Transmit Data Bit 1 */
        fnet_uint8_t R2_T2       : 1;                                      /* Receive/Transmit Data Bit 2 */
        fnet_uint8_t R3_T3       : 1;                                      /* Receive/Transmit Data Bit 3 */
        fnet_uint8_t R4_T4       : 1;                                      /* Receive/Transmit Data Bit 4 */
        fnet_uint8_t R5_T5       : 1;                                      /* Receive/Transmit Data Bit 5 */
        fnet_uint8_t R6_T6       : 1;                                      /* Receive/Transmit Data Bit 6 */
        fnet_uint8_t R7_T7       : 1;                                      /* Receive/Transmit Data Bit 7 */
    } Bits;
} FNET_MCF_SCI_DSTR;

#define _FNET_MCF_SCI_D(x)                          (*(volatile FNET_MCF_SCI_DSTR*)(FNET_MCF_SCI1D_PTR+((x)*0x20)))//(*(vuint8 *)(&FNET_CFG_MCF_IPSBAR[0x204 + ((x)*0x40)]))
#define FNET_MCF_SCI_D(x)                           _FNET_MCF_SCI_D(x).Byte
#define FNET_MCF_SCI_D_R0_T0(x)                     _FNET_MCF_SCI_D(x).Bits.R0_T0
#define FNET_MCF_SCI_D_R1_T1(x)                     _FNET_MCF_SCI_D(x).Bits.R1_T1
#define FNET_MCF_SCI_D_R2_T2(x)                     _FNET_MCF_SCI_D(x).Bits.R2_T2
#define FNET_MCF_SCI_D_R3_T3(x)                     _FNET_MCF_SCI_D(x).Bits.R3_T3
#define FNET_MCF_SCI_D_R4_T4(x)                     _FNET_MCF_SCI_D(x).Bits.R4_T4
#define FNET_MCF_SCI_D_R5_T5(x)                     _FNET_MCF_SCI_D(x).Bits.R5_T5
#define FNET_MCF_SCI_D_R6_T6(x)                     _FNET_MCF_SCI_D(x).Bits.R6_T6
#define FNET_MCF_SCI_D_R7_T7(x)                     _FNET_MCF_SCI_D(x).Bits.R7_T7


#define FNET_MCF_SCI_D_R0_T0_MASK                   (1)
#define FNET_MCF_SCI_D_R1_T1_MASK                   (2)
#define FNET_MCF_SCI_D_R2_T2_MASK                   (4)
#define FNET_MCF_SCI_D_R3_T3_MASK                   (8)
#define FNET_MCF_SCI_D_R4_T4_MASK                   (16)
#define FNET_MCF_SCI_D_R5_T5_MASK                   (32)
#define FNET_MCF_SCI_D_R6_T6_MASK                   (64)
#define FNET_MCF_SCI_D_R7_T7_MASK                   (128)


/*** SCI1BD - SCI1 Baud Rate Register; 0xFFFF8160 ***/
typedef union
{
    fnet_uint16_t Word;
    /* Overlapped registers: */
    struct
    {
        /*** SCI1BDH - SCI1 Baud Rate Register High; 0xFFFF8160 ***/
        union
        {
            fnet_uint8_t Byte;
            struct
            {
                fnet_uint8_t SBR8        : 1;                                      /* Baud Rate Modulo Divisor Bit 8 */
                fnet_uint8_t SBR9        : 1;                                      /* Baud Rate Modulo Divisor Bit 9 */
                fnet_uint8_t SBR10       : 1;                                      /* Baud Rate Modulo Divisor Bit 10 */
                fnet_uint8_t SBR11       : 1;                                      /* Baud Rate Modulo Divisor Bit 11 */
                fnet_uint8_t SBR12       : 1;                                      /* Baud Rate Modulo Divisor Bit 12 */
                fnet_uint8_t             : 1;
                fnet_uint8_t RXEDGIE     : 1;                                      /* RxD Input Active Edge Interrupt Enable (for RXEDGIF) */
                fnet_uint8_t LBKDIE      : 1;                                      /* LIN Break Detect Interrupt Enable (for LBKDIF) */
            } Bits;
            struct
            {
                fnet_uint8_t grpSBR_8 : 5;
                fnet_uint8_t     : 1;
                fnet_uint8_t     : 1;
                fnet_uint8_t     : 1;
            } MergedBits;
        } FNET_MCF_SCI_BDHSTR;
#define FNET_MCF_SCI_BDH(x)                     _FNET_MCF_SCI_BD(x).Overlap_STR.FNET_MCF_SCI_BDHSTR.Byte
#define FNET_MCF_SCI_BDH_SBR8(x)                _FNET_MCF_SCI_BD(x).Overlap_STR.FNET_MCF_SCI_BDHSTR.Bits.SBR8
#define FNET_MCF_SCI_BDH_SBR9(x)                _FNET_MCF_SCI_BD(x).Overlap_STR.FNET_MCF_SCI_BDHSTR.Bits.SBR9
#define FNET_MCF_SCI_BDH_SBR10(x)               _FNET_MCF_SCI_BD(x).Overlap_STR.FNET_MCF_SCI_BDHSTR.Bits.SBR10
#define FNET_MCF_SCI_BDH_SBR11(x)               _FNET_MCF_SCI_BD(x).Overlap_STR.FNET_MCF_SCI_BDHSTR.Bits.SBR11
#define FNET_MCF_SCI_BDH_SBR12(x)               _FNET_MCF_SCI_BD(x).Overlap_STR.FNET_MCF_SCI_BDHSTR.Bits.SBR12
#define FNET_MCF_SCI_BDH_RXEDGIE(x)             _FNET_MCF_SCI_BD(x).Overlap_STR.FNET_MCF_SCI_BDHSTR.Bits.RXEDGIE
#define FNET_MCF_SCI_BDH_LBKDIE(x)              _FNET_MCF_SCI_BD(x).Overlap_STR.FNET_MCF_SCI_BDHSTR.Bits.LBKDIE
#define FNET_MCF_SCI_BDH_SBR_8(x)               _FNET_MCF_SCI_BD(x).Overlap_STR.FNET_MCF_SCI_BDHSTR.MergedBits.grpSBR_8
#define FNET_MCF_SCI_BDH_SBR(x)                 FNET_MCF_SCI_BDH_SBR_8

#define FNET_MCF_SCI_BDH_SBR8_MASK           1
#define FNET_MCF_SCI_BDH_SBR9_MASK           2
#define FNET_MCF_SCI_BDH_SBR10_MASK          4
#define FNET_MCF_SCI_BDH_SBR11_MASK          8
#define FNET_MCF_SCI_BDH_SBR12_MASK          16
#define FNET_MCF_SCI_BDH_RXEDGIE_MASK        64
#define FNET_MCF_SCI_BDH_LBKDIE_MASK         128
#define FNET_MCF_SCI_BDH_SBR_8_MASK          31
#define FNET_MCF_SCI_BDH_SBR_8_BITNUM        0


        /*** SCI1BDL - SCI1 Baud Rate Register Low; 0xFFFF8161 ***/
        union
        {
            fnet_uint8_t Byte;
            struct
            {
                fnet_uint8_t SBR0        : 1;                                      /* Baud Rate Modulo Divisor Bit 0 */
                fnet_uint8_t SBR1        : 1;                                      /* Baud Rate Modulo Divisor Bit 1 */
                fnet_uint8_t SBR2        : 1;                                      /* Baud Rate Modulo Divisor Bit 2 */
                fnet_uint8_t SBR3        : 1;                                      /* Baud Rate Modulo Divisor Bit 3 */
                fnet_uint8_t SBR4        : 1;                                      /* Baud Rate Modulo Divisor Bit 4 */
                fnet_uint8_t SBR5        : 1;                                      /* Baud Rate Modulo Divisor Bit 5 */
                fnet_uint8_t SBR6        : 1;                                      /* Baud Rate Modulo Divisor Bit 6 */
                fnet_uint8_t SBR7        : 1;                                      /* Baud Rate Modulo Divisor Bit 7 */
            } Bits;
        } FNET_MCF_SCI_BDLSTR;
#define FNET_MCF_SCI_BDL(x)                     _FNET_MCF_SCI_BD(x).Overlap_STR.FNET_MCF_SCI_BDLSTR.Byte
#define FNET_MCF_SCI_BDL_SBR0(x)                _FNET_MCF_SCI_BD(x).Overlap_STR.FNET_MCF_SCI_BDLSTR.Bits.SBR0
#define FNET_MCF_SCI_BDL_SBR1(x)                _FNET_MCF_SCI_BD(x).Overlap_STR.FNET_MCF_SCI_BDLSTR.Bits.SBR1
#define FNET_MCF_SCI_BDL_SBR2(x)                _FNET_MCF_SCI_BD(x).Overlap_STR.FNET_MCF_SCI_BDLSTR.Bits.SBR2
#define FNET_MCF_SCI_BDL_SBR3(x)                _FNET_MCF_SCI_BD(x).Overlap_STR.FNET_MCF_SCI_BDLSTR.Bits.SBR3
#define FNET_MCF_SCI_BDL_SBR4(x)                _FNET_MCF_SCI_BD(x).Overlap_STR.FNET_MCF_SCI_BDLSTR.Bits.SBR4
#define FNET_MCF_SCI_BDL_SBR5(x)                _FNET_MCF_SCI_BD(x).Overlap_STR.FNET_MCF_SCI_BDLSTR.Bits.SBR5
#define FNET_MCF_SCI_BDL_SBR6(x)                _FNET_MCF_SCI_BD(x).Overlap_STR.FNET_MCF_SCI_BDLSTR.Bits.SBR6
#define FNET_MCF_SCI_BDL_SBR7(x)                _FNET_MCF_SCI_BD(x).Overlap_STR.FNET_MCF_SCI_BDLSTR.Bits.SBR7

#define FNET_MCF_SCI_BDL_SBR0_MASK           1
#define FNET_MCF_SCI_BDL_SBR1_MASK           2
#define FNET_MCF_SCI_BDL_SBR2_MASK           4
#define FNET_MCF_SCI_BDL_SBR3_MASK           8
#define FNET_MCF_SCI_BDL_SBR4_MASK           16
#define FNET_MCF_SCI_BDL_SBR5_MASK           32
#define FNET_MCF_SCI_BDL_SBR6_MASK           64
#define FNET_MCF_SCI_BDL_SBR7_MASK           128

    } Overlap_STR;

    struct
    {
        fnet_uint16_t SBR0        : 1;                                      /* Baud Rate Modulo Divisor Bit 0 */
        fnet_uint16_t SBR1        : 1;                                      /* Baud Rate Modulo Divisor Bit 1 */
        fnet_uint16_t SBR2        : 1;                                      /* Baud Rate Modulo Divisor Bit 2 */
        fnet_uint16_t SBR3        : 1;                                      /* Baud Rate Modulo Divisor Bit 3 */
        fnet_uint16_t SBR4        : 1;                                      /* Baud Rate Modulo Divisor Bit 4 */
        fnet_uint16_t SBR5        : 1;                                      /* Baud Rate Modulo Divisor Bit 5 */
        fnet_uint16_t SBR6        : 1;                                      /* Baud Rate Modulo Divisor Bit 6 */
        fnet_uint16_t SBR7        : 1;                                      /* Baud Rate Modulo Divisor Bit 7 */
        fnet_uint16_t SBR8        : 1;                                      /* Baud Rate Modulo Divisor Bit 8 */
        fnet_uint16_t SBR9        : 1;                                      /* Baud Rate Modulo Divisor Bit 9 */
        fnet_uint16_t SBR10       : 1;                                      /* Baud Rate Modulo Divisor Bit 10 */
        fnet_uint16_t SBR11       : 1;                                      /* Baud Rate Modulo Divisor Bit 11 */
        fnet_uint16_t SBR12       : 1;                                      /* Baud Rate Modulo Divisor Bit 12 */
        fnet_uint16_t             : 1;
        fnet_uint16_t RXEDGIE     : 1;                                      /* RxD Input Active Edge Interrupt Enable (for RXEDGIF) */
        fnet_uint16_t LBKDIE      : 1;                                      /* LIN Break Detect Interrupt Enable (for LBKDIF) */
    } Bits;
    struct
    {
        fnet_uint16_t grpSBR  : 13;
        fnet_uint16_t         : 1;
        fnet_uint16_t         : 1;
        fnet_uint16_t         : 1;
    } MergedBits;
} FNET_MCF_SCI_BDSTR;

#define _FNET_MCF_SCI_BD(x)                         (*(volatile FNET_MCF_SCI_BDSTR*)(FNET_MCF_SCI1BD_PTR+((x)*0x20)))

#define FNET_MCF_SCI_BD(x)                          _FNET_MCF_SCI_BD(x).Word
#define FNET_MCF_SCI_BD_SBR0(x)                     _FNET_MCF_SCI_BD(x).Bits.SBR0
#define FNET_MCF_SCI_BD_SBR1(x)                     _FNET_MCF_SCI_BD(x).Bits.SBR1
#define FNET_MCF_SCI_BD_SBR2(x)                     _FNET_MCF_SCI_BD(x).Bits.SBR2
#define FNET_MCF_SCI_BD_SBR3(x)                     _FNET_MCF_SCI_BD(x).Bits.SBR3
#define FNET_MCF_SCI_BD_SBR4(x)                     _FNET_MCF_SCI_BD(x).Bits.SBR4
#define FNET_MCF_SCI_BD_SBR5(x)                     _FNET_MCF_SCI_BD(x).Bits.SBR5
#define FNET_MCF_SCI_BD_SBR6(x)                     _FNET_MCF_SCI_BD(x).Bits.SBR6
#define FNET_MCF_SCI_BD_SBR7(x)                     _FNET_MCF_SCI_BD(x).Bits.SBR7
#define FNET_MCF_SCI_BD_SBR8(x)                     _FNET_MCF_SCI_BD(x).Bits.SBR8
#define FNET_MCF_SCI_BD_SBR9(x)                     _FNET_MCF_SCI_BD(x).Bits.SBR9
#define FNET_MCF_SCI_BD_SBR10(x)                    _FNET_MCF_SCI_BD(x).Bits.SBR10
#define FNET_MCF_SCI_BD_SBR11(x)                    _FNET_MCF_SCI_BD(x).Bits.SBR11
#define FNET_MCF_SCI_BD_SBR12(x)                    _FNET_MCF_SCI_BD(x).Bits.SBR12
#define FNET_MCF_SCI_BD_RXEDGIE(x)                  _FNET_MCF_SCI_BD(x).Bits.RXEDGIE
#define FNET_MCF_SCI_BD_LBKDIE(x)                   _FNET_MCF_SCI_BD(x).Bits.LBKDIE
#define FNET_MCF_SCI_BD_SBR(x)                      _FNET_MCF_SCI_BD(x).MergedBits.grpSBR

#define FNET_MCF_SCI_BD_SBR0_MASK                1
#define FNET_MCF_SCI_BD_SBR1_MASK                2
#define FNET_MCF_SCI_BD_SBR2_MASK                4
#define FNET_MCF_SCI_BD_SBR3_MASK                8
#define FNET_MCF_SCI_BD_SBR4_MASK                16
#define FNET_MCF_SCI_BD_SBR5_MASK                32
#define FNET_MCF_SCI_BD_SBR6_MASK                64
#define FNET_MCF_SCI_BD_SBR7_MASK                128
#define FNET_MCF_SCI_BD_SBR8_MASK                256
#define FNET_MCF_SCI_BD_SBR9_MASK                512
#define FNET_MCF_SCI_BD_SBR10_MASK               1024
#define FNET_MCF_SCI_BD_SBR11_MASK               2048
#define FNET_MCF_SCI_BD_SBR12_MASK               4096
#define FNET_MCF_SCI_BD_RXEDGIE_MASK             16384
#define FNET_MCF_SCI_BD_LBKDIE_MASK              32768
#define FNET_MCF_SCI_BD_SBR_MASK                 8191
#define FNET_MCF_SCI_BD_SBR_BITNUM               0


/*** SCI1C1 - SCI1 Control Register 1; 0xFFFF8162 ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t PT          : 1;                                      /* Parity Type */
        fnet_uint8_t PE          : 1;                                      /* Parity Enable */
        fnet_uint8_t ILT         : 1;                                      /* Idle Line Type Select */
        fnet_uint8_t WAKE        : 1;                                      /* Receiver Wakeup Method Select */
        fnet_uint8_t M           : 1;                                      /* 9-Bit or 8-Bit Mode Select */
        fnet_uint8_t RSRC        : 1;                                      /* Receiver Source Select */
        fnet_uint8_t SCISWAI     : 1;                                      /* SCI Stops in Wait Mode */
        fnet_uint8_t LOOPS       : 1;                                      /* Loop Mode Select */
    } Bits;
} FNET_MCF_SCI_C1STR;

#define _FNET_MCF_SCI_C1(x)                         (*(volatile FNET_MCF_SCI_C1STR*)(FNET_MCF_SCI1C1_PTR+((x)*0x20)))

#define FNET_MCF_SCI_C1(x)                          _FNET_MCF_SCI_C1(x).Byte
#define FNET_MCF_SCI_C1_PT(x)                       _FNET_MCF_SCI_C1(x).Bits.PT
#define FNET_MCF_SCI_C1_PE(x)                       _FNET_MCF_SCI_C1(x).Bits.PE
#define FNET_MCF_SCI_C1_ILT(x)                      _FNET_MCF_SCI_C1(x).Bits.ILT
#define FNET_MCF_SCI_C1_WAKE(x)                     _FNET_MCF_SCI_C1(x).Bits.WAKE
#define FNET_MCF_SCI_C1_M(x)                        _FNET_MCF_SCI_C1(x).Bits.M
#define FNET_MCF_SCI_C1_RSRC(x)                     _FNET_MCF_SCI_C1(x).Bits.RSRC
#define FNET_MCF_SCI_C1_SCISWAI(x)                  _FNET_MCF_SCI_C1(x).Bits.SCISWAI
#define FNET_MCF_SCI_C1_LOOPS(x)                    _FNET_MCF_SCI_C1(x).Bits.LOOPS

#define FNET_MCF_SCI_C1_PT_MASK                  1
#define FNET_MCF_SCI_C1_PE_MASK                  2
#define FNET_MCF_SCI_C1_ILT_MASK                 4
#define FNET_MCF_SCI_C1_WAKE_MASK                8
#define FNET_MCF_SCI_C1_M_MASK                   16
#define FNET_MCF_SCI_C1_RSRC_MASK                32
#define FNET_MCF_SCI_C1_SCISWAI_MASK             64
#define FNET_MCF_SCI_C1_LOOPS_MASK               128

/*** SCI1C2 - SCI1 Control Register 2; 0xFFFF8163 ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t SBK         : 1;                                      /* Send Break */
        fnet_uint8_t RWU         : 1;                                      /* Receiver Wakeup Control */
        fnet_uint8_t RE          : 1;                                      /* Receiver Enable */
        fnet_uint8_t TE          : 1;                                      /* Transmitter Enable */
        fnet_uint8_t ILIE        : 1;                                      /* Idle Line Interrupt Enable (for IDLE) */
        fnet_uint8_t RIE         : 1;                                      /* Receiver Interrupt Enable (for RDRF) */
        fnet_uint8_t TCIE        : 1;                                      /* Transmission Complete Interrupt Enable (for TC) */
        fnet_uint8_t TIE         : 1;                                      /* Transmit Interrupt Enable (for TDRE) */
    } Bits;
} FNET_MCF_SCI_C2STR;

#define _FNET_MCF_SCI_C2(x)                         (*(volatile FNET_MCF_SCI_C2STR*)(FNET_MCF_SCI1C2_PTR+((x)*0x20)))

#define FNET_MCF_SCI_C2(x)                          _FNET_MCF_SCI_C2(x).Byte
#define FNET_MCF_SCI_C2_SBK(x)                      _FNET_MCF_SCI_C2(x).Bits.SBK
#define FNET_MCF_SCI_C2_RWU(x)                      _FNET_MCF_SCI_C2(x).Bits.RWU
#define FNET_MCF_SCI_C2_RE(x)                       _FNET_MCF_SCI_C2(x).Bits.RE
#define FNET_MCF_SCI_C2_TE(x)                       _FNET_MCF_SCI_C2(x).Bits.TE
#define FNET_MCF_SCI_C2_ILIE(x)                     _FNET_MCF_SCI_C2(x).Bits.ILIE
#define FNET_MCF_SCI_C2_RIE(x)                      _FNET_MCF_SCI_C2(x).Bits.RIE
#define FNET_MCF_SCI_C2_TCIE(x)                     _FNET_MCF_SCI_C2(x).Bits.TCIE
#define FNET_MCF_SCI_C2_TIE(x)                      _FNET_MCF_SCI_C2(x).Bits.TIE

#define FNET_MCF_SCI_C2_SBK_MASK                 1
#define FNET_MCF_SCI_C2_RWU_MASK                 2
#define FNET_MCF_SCI_C2_RE_MASK                  4
#define FNET_MCF_SCI_C2_TE_MASK                  8
#define FNET_MCF_SCI_C2_ILIE_MASK                16
#define FNET_MCF_SCI_C2_RIE_MASK                 32
#define FNET_MCF_SCI_C2_TCIE_MASK                64
#define FNET_MCF_SCI_C2_TIE_MASK                 128


/*** SCI1C3 - SCI1 Control Register 3; 0xFFFF8166 ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t PEIE        : 1;                                      /* Parity Error Interrupt Enable */
        fnet_uint8_t FEIE        : 1;                                      /* Framing Error Interrupt Enable */
        fnet_uint8_t NEIE        : 1;                                      /* Noise Error Interrupt Enable */
        fnet_uint8_t ORIE        : 1;                                      /* Overrun Interrupt Enable */
        fnet_uint8_t TXINV       : 1;                                      /* Transmit Data Inversion */
        fnet_uint8_t TXDIR       : 1;                                      /* TxD Pin Direction in Single-Wire Mode */
        fnet_uint8_t T8          : 1;                                      /* Ninth Data Bit for Transmitter */
        fnet_uint8_t R8          : 1;                                      /* Ninth Data Bit for Receiver */
    } Bits;
} FNET_MCF_SCI_C3STR;

#define _FNET_MCF_SCI_C3(x)                         (*(volatile FNET_MCF_SCI_C3STR*)(FNET_MCF_SCI1C3_PTR+((x)*0x20)))

#define FNET_MCF_SCI_C3(x)                          _FNET_MCF_SCI_C3(x).Byte
#define FNET_MCF_SCI_C3_PEIE(x)                     _FNET_MCF_SCI_C3(x).Bits.PEIE
#define FNET_MCF_SCI_C3_FEIE(x)                     _FNET_MCF_SCI_C3(x).Bits.FEIE
#define FNET_MCF_SCI_C3_NEIE(x)                     _FNET_MCF_SCI_C3(x).Bits.NEIE
#define FNET_MCF_SCI_C3_ORIE(x)                     _FNET_MCF_SCI_C3(x).Bits.ORIE
#define FNET_MCF_SCI_C3_TXINV(x)                    _FNET_MCF_SCI_C3(x).Bits.TXINV
#define FNET_MCF_SCI_C3_TXDIR(x)                    _FNET_MCF_SCI_C3(x).Bits.TXDIR
#define FNET_MCF_SCI_C3_T8(x)                       _FNET_MCF_SCI_C3(x).Bits.T8
#define FNET_MCF_SCI_C3_R8(x)                       _FNET_MCF_SCI_C3(x).Bits.R8

#define FNET_MCF_SCI_C3_PEIE_MASK                1
#define FNET_MCF_SCI_C3_FEIE_MASK                2
#define FNET_MCF_SCI_C3_NEIE_MASK                4
#define FNET_MCF_SCI_C3_ORIE_MASK                8
#define FNET_MCF_SCI_C3_TXINV_MASK               16
#define FNET_MCF_SCI_C3_TXDIR_MASK               32
#define FNET_MCF_SCI_C3_T8_MASK                  64
#define FNET_MCF_SCI_C3_R8_MASK                  128



/*********************************************************************
*
* Teal-Time Counter (RTC) MCFV1
*
*********************************************************************/

/*** RTCSC - RTC Status and Control Register ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t RTCPS0      : 1;                                      /* Real-Time Clock Prescaler Select, bit 0 */
        fnet_uint8_t RTCPS1      : 1;                                      /* Real-Time Clock Prescaler Select, bit 1 */
        fnet_uint8_t RTCPS2      : 1;                                      /* Real-Time Clock Prescaler Select, bit 2 */
        fnet_uint8_t RTCPS3      : 1;                                      /* Real-Time Clock Prescaler Select, bit 3 */
        fnet_uint8_t RTIE        : 1;                                      /* Real-Time Interrupt Enable */
        fnet_uint8_t RTCLKS0     : 1;                                      /* Real-Time Clock Source Select, bit 0 */
        fnet_uint8_t RTCLKS1     : 1;                                      /* Real-Time Clock Source Select, bit 1 */
        fnet_uint8_t RTIF        : 1;                                      /* Real-Time Interrupt Flag */
    } Bits;
    struct
    {
        fnet_uint8_t grpRTCPS : 4;
        fnet_uint8_t         : 1;
        fnet_uint8_t grpRTCLKS : 2;
        fnet_uint8_t         : 1;
    } MergedBits;
} FNET_MCF_RTCSCSTR;
extern volatile FNET_MCF_RTCSCSTR _FNET_MCF_RTCSC @FNET_MCF_RTCSC_PTR;
#define FNET_MCF_RTCSC                           _FNET_MCF_RTCSC.Byte
#define FNET_MCF_RTCSC_RTCPS0                    _FNET_MCF_RTCSC.Bits.RTCPS0
#define FNET_MCF_RTCSC_RTCPS1                    _FNET_MCF_RTCSC.Bits.RTCPS1
#define FNET_MCF_RTCSC_RTCPS2                    _FNET_MCF_RTCSC.Bits.RTCPS2
#define FNET_MCF_RTCSC_RTCPS3                    _FNET_MCF_RTCSC.Bits.RTCPS3
#define FNET_MCF_RTCSC_RTIE                      _FNET_MCF_RTCSC.Bits.RTIE
#define FNET_MCF_RTCSC_RTCLKS0                   _FNET_MCF_RTCSC.Bits.RTCLKS0
#define FNET_MCF_RTCSC_RTCLKS1                   _FNET_MCF_RTCSC.Bits.RTCLKS1
#define FNET_MCF_RTCSC_RTIF                      _FNET_MCF_RTCSC.Bits.RTIF
#define FNET_MCF_RTCSC_RTCPS                     _FNET_MCF_RTCSC.MergedBits.grpRTCPS
#define FNET_MCF_RTCSC_RTCLKS                    _FNET_MCF_RTCSC.MergedBits.grpRTCLKS

#define FNET_MCF_RTCSC_RTCPS0_MASK               1
#define FNET_MCF_RTCSC_RTCPS1_MASK               2
#define FNET_MCF_RTCSC_RTCPS2_MASK               4
#define FNET_MCF_RTCSC_RTCPS3_MASK               8
#define FNET_MCF_RTCSC_RTIE_MASK                 16
#define FNET_MCF_RTCSC_RTCLKS0_MASK              32
#define FNET_MCF_RTCSC_RTCLKS1_MASK              64
#define FNET_MCF_RTCSC_RTIF_MASK                 128
#define FNET_MCF_RTCSC_RTCPS_MASK                15
#define FNET_MCF_RTCSC_RTCPS_BITNUM              0
#define FNET_MCF_RTCSC_RTCLKS_MASK               96
#define FNET_MCF_RTCSC_RTCLKS_BITNUM             5


/*** RTCCNT - RTC Counter Register ***/
typedef union
{
    fnet_uint8_t Byte;
} FNET_MCF_RTCCNTSTR;
extern volatile FNET_MCF_RTCCNTSTR _FNET_MCF_RTCCNT @FNET_MCF_RTCCNT_PTR;
#define FNET_RTCCNT                          _FNET_MCF_RTCCNT.Byte


/*** RTCMOD - RTC Modulo Register ***/
typedef union
{
    fnet_uint8_t Byte;
} FNET_MCF_RTCMODSTR;
extern volatile FNET_MCF_RTCMODSTR _FNET_MCF_RTCMOD @FNET_MCF_RTCMOD_PTR;
#define FNET_MCF_RTCMOD                          _FNET_MCF_RTCMOD.Byte




/*********************************************************************
*
* System Clock Gating (SCG) MCFV1
*
*********************************************************************/

/*** SCGC1 - System Clock Gating Control 1 Register; 0xFFFF8108 ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t SCI1        : 1;                                      /* SCI1 Clock Gate Control */
        fnet_uint8_t SCI2        : 1;                                      /* SCI2 Clock Gate Control */
        fnet_uint8_t IIC1        : 1;                                      /* IIC1 Clock Gate Control */
        fnet_uint8_t IIC2        : 1;                                      /* IIC2 Clock Gate Control */
        fnet_uint8_t ADC         : 1;                                      /* ADC Clock Gate Control */
        fnet_uint8_t TPM1        : 1;                                      /* TPM1 Clock Gate Control */
        fnet_uint8_t TPM2        : 1;                                      /* TPM2 Clock Gate Control */
        fnet_uint8_t MTIM1       : 1;                                      /* MTIM1 Clock Gate Control */
    } Bits;
    struct
    {
        fnet_uint8_t grpSCI_1 : 2;
        fnet_uint8_t grpIIC_1 : 2;
        fnet_uint8_t         : 1;
        fnet_uint8_t grpTPM_1 : 2;
        fnet_uint8_t grpMTIM_1 : 1;
    } MergedBits;
} FNET_MCF_SCGC1STR;
extern volatile FNET_MCF_SCGC1STR _FNET_MCF_SCGC1 @FNET_MCF_SCGC1_PTR;
#define FNET_MCF_SCGC1                           _FNET_MCF_SCGC1.Byte
#define FNET_MCF_SCGC1_SCI1                      _FNET_MCF_SCGC1.Bits.SCI1
#define FNET_MCF_SCGC1_SCI2                      _FNET_MCF_SCGC1.Bits.SCI2
#define FNET_MCF_SCGC1_IIC1                      _FNET_MCF_SCGC1.Bits.IIC1
#define FNET_MCF_SCGC1_IIC2                      _FNET_MCF_SCGC1.Bits.IIC2
#define FNET_MCF_SCGC1_ADC                       _FNET_MCF_SCGC1.Bits.ADC
#define FNET_MCF_SCGC1_TPM1                      _FNET_MCF_SCGC1.Bits.TPM1
#define FNET_MCF_SCGC1_TPM2                      _FNET_MCF_SCGC1.Bits.TPM2
#define FNET_MCF_SCGC1_MTIM1                     _FNET_MCF_SCGC1.Bits.MTIM1
#define FNET_MCF_SCGC1_SCI_1                     _FNET_MCF_SCGC1.MergedBits.grpSCI_1
#define FNET_MCF_SCGC1_IIC_1                     _FNET_MCF_SCGC1.MergedBits.grpIIC_1
#define FNET_MCF_SCGC1_TPM_1                     _FNET_MCF_SCGC1.MergedBits.grpTPM_1
#define FNET_MCF_SCGC1_SCI                       FNET_MCF_SCGC1_SCI_1
#define FNET_MCF_SCGC1_IIC                       FNET_MCF_SCGC1_IIC_1
#define FNET_MCF_SCGC1_TPM                       FNET_MCF_SCGC1_TPM_1

#define FNET_MCF_SCGC1_SCI1_MASK                 1
#define FNET_MCF_SCGC1_SCI2_MASK                 2
#define FNET_MCF_SCGC1_IIC1_MASK                 4
#define FNET_MCF_SCGC1_IIC2_MASK                 8
#define FNET_MCF_SCGC1_ADC_MASK                  16
#define FNET_MCF_SCGC1_TPM1_MASK                 32
#define FNET_MCF_SCGC1_TPM2_MASK                 64
#define FNET_MCF_SCGC1_MTIM1_MASK                128
#define FNET_MCF_SCGC1_SCI_1_MASK                3
#define FNET_MCF_SCGC1_SCI_1_BITNUM              0
#define FNET_MCF_SCGC1_IIC_1_MASK                12
#define FNET_MCF_SCGC1_IIC_1_BITNUM              2
#define FNET_MCF_SCGC1_TPM_1_MASK                96
#define FNET_MCF_SCGC1_TPM_1_BITNUM              5


/*** SCGC2 - System Clock Gating Control 2 Register; 0xFFFF8109 ***/
typedef union
{
    fnet_uint8_t Byte;
    struct
    {
        fnet_uint8_t SPI1        : 1;                                      /* SPI1 Clock Gate Control */
        fnet_uint8_t SPI2        : 1;                                      /* SPI2 Clock Gate Control */
        fnet_uint8_t RTC         : 1;                                      /* RTC Clock Gate Control */
        fnet_uint8_t KBI1        : 1;                                      /* KBI1 Clock Gate Control */
        fnet_uint8_t KBI2        : 1;                                      /* KBI2 Clock Gate Control */
        fnet_uint8_t IRQ         : 1;                                      /* IRQ Clock Gate Control */
        fnet_uint8_t FTSR        : 1;                                      /* FTSR Clock Gate Control */
        fnet_uint8_t SCI3        : 1;                                      /* SCI3 Clock Gate Control */
    } Bits;
    struct
    {
        fnet_uint8_t grpSPI_1 : 2;
        fnet_uint8_t         : 1;
        fnet_uint8_t grpKBI_1 : 2;
        fnet_uint8_t         : 1;
        fnet_uint8_t         : 1;
        fnet_uint8_t grpSCI_3 : 1;
    } MergedBits;
} FNET_MCF_SCGC2STR;
extern volatile FNET_MCF_SCGC2STR _FNET_MCF_SCGC2 @FNET_MCF_SCGC2_PTR;
#define FNET_MCF_SCGC2                           _FNET_MCF_SCGC2.Byte
#define FNET_MCF_SCGC2_SPI1                      _FNET_MCF_SCGC2.Bits.SPI1
#define FNET_MCF_SCGC2_SPI2                      _FNET_MCF_SCGC2.Bits.SPI2
#define FNET_MCF_SCGC2_RTC                       _FNET_MCF_SCGC2.Bits.RTC
#define FNET_MCF_SCGC2_KBI1                      _FNET_MCF_SCGC2.Bits.KBI1
#define FNET_MCF_SCGC2_KBI2                      _FNET_MCF_SCGC2.Bits.KBI2
#define FNET_MCF_SCGC2_IRQ                       _FNET_MCF_SCGC2.Bits.IRQ
#define FNET_MCF_SCGC2_FTSR                      _FNET_MCF_SCGC2.Bits.FTSR
#define FNET_MCF_SCGC2_SCI3                      _FNET_MCF_SCGC2.Bits.SCI3
#define FNET_MCF_SCGC2_SPI_1                     _FNET_MCF_SCGC2.MergedBits.grpSPI_1
#define FNET_MCF_SCGC2_KBI_1                     _FNET_MCF_SCGC2.MergedBits.grpKBI_1
#define FNET_MCF_SCGC2_SPI                       FNET_MCF_SCGC2_SPI_1
#define FNET_MCF_SCGC2_KBI                       FNET_MCF_SCGC2_KBI_1

#define FNET_MCF_SCGC2_SPI1_MASK                 1
#define FNET_MCF_SCGC2_SPI2_MASK                 2
#define FNET_MCF_SCGC2_RTC_MASK                  4
#define FNET_MCF_SCGC2_KBI1_MASK                 8
#define FNET_MCF_SCGC2_KBI2_MASK                 16
#define FNET_MCF_SCGC2_IRQ_MASK                  32
#define FNET_MCF_SCGC2_FTSR_MASK                 64
#define FNET_MCF_SCGC2_SCI3_MASK                 128
#define FNET_MCF_SCGC2_SPI_1_MASK                3
#define FNET_MCF_SCGC2_SPI_1_BITNUM              0
#define FNET_MCF_SCGC2_KBI_1_MASK                24
#define FNET_MCF_SCGC2_KBI_1_BITNUM              3






#endif /* FNET_CFG_MCF_V1 */





/*********************************************************************
*
* Power Management (PMM)
*
*********************************************************************/
#if FNET_CFG_CPU_MCF54418
    /* Register read/write macros */
    #define FNET_MCF_PMM_PPMLR0                       (*(fnet_vuint32_t*)(0xFC040034))
    #define FNET_MCF_PMM_PPMLR1                       (*(fnet_vuint32_t*)(0xFC04003C))

    #define FNET_MCF_PMM_PPMLR0_CD18                  (0x40000)
    #define FNET_MCF_PMM_PPMLR0_CD19                  (0x80000)
    #define FNET_MCF_PMM_PPMLR0_CD20                  (0x100000)
    #define FNET_MCF_PMM_PPMLR0_CD24                  (0x1000000)
    #define FNET_MCF_PMM_PPMLR0_CD25                  (0x2000000)
    #define FNET_MCF_PMM_PPMLR0_CD26                  (0x4000000)
    #define FNET_MCF_PMM_PPMLR0_CD27                  (0x8000000)
    #define FNET_MCF_PMM_PPMLR0_CD28                  (0x10000000)
    #define FNET_MCF_PMM_PPMLR0_CD29                  (0x20000000)
    #define FNET_MCF_PMM_PPMLR0_CD30                  (0x40000000)
    #define FNET_MCF_PMM_PPMLR0_CD31                  (0x80000000)

    #define FNET_MCF_PMM_PPMLR1_CD24                  (0x1000000)
    #define FNET_MCF_PMM_PPMLR1_CD25                  (0x2000000)
    #define FNET_MCF_PMM_PPMLR1_CD26                  (0x4000000)
    #define FNET_MCF_PMM_PPMLR1_CD27                  (0x8000000)

#endif /* FNET_CFG_CPU_MCF54418 */


/*********************************************************************
*
* GPIO
*
*********************************************************************/

/**************** MCF5282 ****************/
#define FNET_MCF5282_GPIO_PEHLPAR       (*(fnet_vuint8_t  *)(&FNET_CFG_MCF_IPSBAR[0x100058]))
#define FNET_MCF5282_GPIO_PASPAR        (*(fnet_vuint16_t *)(&FNET_CFG_MCF_IPSBAR[0x100056]))

/**************** MCF523x ****************/
#define FNET_MCF523x_GPIO_PAR_FECI2C    (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x100047]))

/**************** MCF5225x ****************/
#define FNET_MCF5225X_GPIO_PNQPAR       (*(fnet_vuint16_t*)(&FNET_CFG_MCF_IPSBAR[0x100068]))
#define FNET_MCF5225X_GPIO_PTIPAR       (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x100064]))
#define FNET_MCF5225X_GPIO_PTJPAR       (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x100066]))

#define FNET_MCF5225X_GPIO_DDRTI        (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x10001C]))
#define FNET_MCF5225X_GPIO_DDRTJ        (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x10001E]))
#define FNET_MCF5225X_GPIO_PORTTI       (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x100004]))
#define FNET_MCF5225X_GPIO_PORTTJ       (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x100006]))

#define FNET_MCF522XX_GPIO_PUCPAR       (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x100073]))
#if FNET_CFG_CPU_MCF52235 /* Dual Function Pin Assignment.*/
    #define FNET_MCF522XX_GPIO_PUCPAR_URXD2_URXD2       (0x2)
#else /* Quad-Function Pin Assignment.*/
    #define FNET_MCF522XX_GPIO_PUCPAR_URXD2_URXD2       (0x4)
#endif
#define FNET_MCF522XX_GPIO_PUCPAR_UTXD2_UTXD2           (0x1)

#define FNET_MCF522XX_GPIO_PUBPAR       (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x100072]))
#define FNET_MCF522XX_GPIO_PUBPAR_URXD1_URXD1           (0x4)
#define FNET_MCF522XX_GPIO_PUBPAR_UTXD1_UTXD1           (0x1)

#define FNET_MCF522XX_GPIO_PUAPAR       (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x100071]))
#define FNET_MCF522XX_GPIO_PUAPAR_URXD0_URXD0           (0x4)
#define FNET_MCF522XX_GPIO_PUAPAR_UTXD0_UTXD0           (0x1)

/**************** MCF5223x ****************/
#define FNET_MCF5223X_GPIO_PLDPAR       (*(fnet_vuint8_t *)(&FNET_CFG_MCF_IPSBAR[0x100075]))
/* Bit definitions and macros for MCF_GPIO_PLDPAR */
#define FNET_MCF5223X_GPIO_PLDPAR_ACTLED_ACTLED        (0x1)
#define FNET_MCF5223X_GPIO_PLDPAR_LINKLED_LINKLED      (0x2)
#define FNET_MCF5223X_GPIO_PLDPAR_SPDLED_SPDLED        (0x4)
#define FNET_MCF5223X_GPIO_PLDPAR_DUPLED_DUPLED        (0x8)
#define FNET_MCF5223X_GPIO_PLDPAR_COLLED_COLLED        (0x10)
#define FNET_MCF5223X_GPIO_PLDPAR_RXLED_RXLED          (0x20)
#define FNET_MCF5223X_GPIO_PLDPAR_TXLED_TXLED          (0x40)

/**************** MCF5441x ****************/
#define FNET_MCF5441X_GPIO_PAR_DSPIOWH                  (*(fnet_vuint8_t *)(0xEC09404E))
#define FNET_MCF5441X_GPIO_PAR_TIMER                    (*(fnet_vuint8_t *)(0xEC094050))
#define FNET_MCF5441X_GPIO_PAR_UART2                    (*(fnet_vuint8_t *)(0xEC094051))
#define FNET_MCF5441X_GPIO_PAR_UART1                    (*(fnet_vuint8_t *)(0xEC094052))
#define FNET_MCF5441X_GPIO_PAR_UART0                    (*(fnet_vuint8_t *)(0xEC094053))
#define FNET_MCF5441X_GPIO_PAR_FEC                      (*(fnet_vuint8_t *)(0xEC09405E))

#define FNET_MCF5441X_GPIO_SRCR_FEC                     (*(fnet_vuint8_t *)(0xEC09406D))

#define FNET_MCF5441X_GPIO_PODR_G                       (*(fnet_vuint8_t *)(0xEC094006))
#define FNET_MCF5441X_GPIO_PDDR_G                       (*(fnet_vuint8_t *)(0xEC094012))


#define FNET_MCF5441X_GPIO_PAR_UART0_PAR_TXD_U0TXD      (0x3)
#define FNET_MCF5441X_GPIO_PAR_UART0_PAR_RXD_U0RXD      (0xC)
#define FNET_MCF5441X_GPIO_PAR_UART0_PAR_RTS_U4RXD      (0x20)
#define FNET_MCF5441X_GPIO_PAR_UART0_PAR_CTS_U4TXD      (0x80)

#define FNET_MCF5441X_GPIO_PAR_UART1_PAR_TXD_U1TXD      (0x3)
#define FNET_MCF5441X_GPIO_PAR_UART1_PAR_RXD_U1RXD      (0xC)
#define FNET_MCF5441X_GPIO_PAR_UART1_PAR_RTS_U5RXD      (0x20)
#define FNET_MCF5441X_GPIO_PAR_UART1_PAR_CTS_U5TXD      (0x80)

#define FNET_MCF5441X_GPIO_PAR_UART2_PAR_TXD_U2TXD      (0x3)
#define FNET_MCF5441X_GPIO_PAR_UART2_PAR_RXD_U2RXD      (0xC)
#define FNET_MCF5441X_GPIO_PAR_UART2_PAR_RTS_U6RXD      (0x20)
#define FNET_MCF5441X_GPIO_PAR_UART2_PAR_CTS_U6TXD      (0x80)

#define FNET_MCF5441X_GPIO_PAR_DSPIOWH_PAR_SOUT_U3TXD   (0x20)
#define FNET_MCF5441X_GPIO_PAR_DSPIOWH_PAR_SIN_U3RXD    (0x80)

/* Bit definitions and macros for FNET_MCF5441X_GPIO_PAR_TIMER */
#define FNET_MCF5441X_GPIO_PAR_TIMER_T0OUT     (0x2)
#define FNET_MCF5441X_GPIO_PAR_TIMER_T0IN      (0x3)
#define FNET_MCF5441X_GPIO_PAR_TIMER_T1OUT     (0x8)
#define FNET_MCF5441X_GPIO_PAR_TIMER_T1IN      (0xC)
#define FNET_MCF5441X_GPIO_PAR_TIMER_T2OUT     (0x20)
#define FNET_MCF5441X_GPIO_PAR_TIMER_T2IN      (0x30)
#define FNET_MCF5441X_GPIO_PAR_TIMER_T3OUT     (0x80)
#define FNET_MCF5441X_GPIO_PAR_TIMER_T3IN      (0xC0)

/* Bit definitions and macros for FNET_MCF5441X_GPIO_PAR_FEC */
#define FNET_MCF5441X_GPIO_PAR_FEC_PAR_FEC_MII_FULL       				(0)
#define FNET_MCF5441X_GPIO_PAR_FEC_PAR_FEC_MII_NON_FULL         		(0x1)
#define FNET_MCF5441X_GPIO_PAR_FEC_PAR_FEC_RMII0_RMII1_FULL 			(0x3)
#define FNET_MCF5441X_GPIO_PAR_FEC_PAR_FEC_RMII0_RMII1_NON_FULL 		(0x4)
#define FNET_MCF5441X_GPIO_PAR_FEC_PAR_FEC_RMII0_FULL_RMII1_NON_FULL	(0x5)
#define FNET_MCF5441X_GPIO_PAR_FEC_PAR_FEC_RMII0_NON_FULL_RMII1_FULL 	(0x6)
#define FNET_MCF5441X_GPIO_PAR_FEC_PAR_FEC_RMII0_FULL   				(0x7)
#define FNET_MCF5441X_GPIO_PAR_FEC_PAR_FEC_RMII0_FULL_ULPI 				(0x8)
#define FNET_MCF5441X_GPIO_PAR_FEC_PAR_FEC_RMII0_NON_MII 				(0x9)
#define FNET_MCF5441X_GPIO_PAR_FEC_PAR_FEC_RMII0_NON_MII_ULPI 			(0xA)
#define FNET_MCF5441X_GPIO_PAR_FEC_PAR_FEC_RMII1_FULL   				(0xB)
#define FNET_MCF5441X_GPIO_PAR_FEC_PAR_FEC_RMII1_NON_MII 				(0xC)

/* Bit definitions and macros for FNET_MCF5441X_GPIO_SRCR_FEC */
#define FNET_MCF5441X_GPIO_SRCR_FEC_SRE_RMII1(x)         (((x)&0x3)<<0)
#define FNET_MCF5441X_GPIO_SRCR_FEC_SRE_RMII1_SR_LOWEST  (0)
#define FNET_MCF5441X_GPIO_SRCR_FEC_SRE_RMII1_SR_LOW     (0x1)
#define FNET_MCF5441X_GPIO_SRCR_FEC_SRE_RMII1_SR_HIGH    (0x2)
#define FNET_MCF5441X_GPIO_SRCR_FEC_SRE_RMII1_SR_HIGHEST (0x3)
#define FNET_MCF5441X_GPIO_SRCR_FEC_SRE_RMII0(x)         (((x)&0x3)<<0x2)
#define FNET_MCF5441X_GPIO_SRCR_FEC_SRE_RMII0_SR_LOWEST  (0)
#define FNET_MCF5441X_GPIO_SRCR_FEC_SRE_RMII0_SR_LOW     (0x4)
#define FNET_MCF5441X_GPIO_SRCR_FEC_SRE_RMII0_SR_HIGH    (0x8)
#define FNET_MCF5441X_GPIO_SRCR_FEC_SRE_RMII0_SR_HIGHEST (0xC)

/* Bit definitions and macros for GPIO_G */
#define FNET_MCF5441X_GPIO_PDDR_G_PDDR_G4                (0x10)
#define FNET_MCF5441X_GPIO_PODR_G_PODR_G4                (0x10)


#endif /* FNET_MCF */
#endif
