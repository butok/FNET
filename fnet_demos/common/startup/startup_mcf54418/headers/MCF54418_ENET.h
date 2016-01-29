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

#ifndef __MCF54418_ENET_H__
#define __MCF54418_ENET_H__


/*********************************************************************
*
* Ethernet MAC-NET Core (ENET)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_ENET0_EIR                        (*(vuint32*)(0xFC0D4004))
#define MCF_ENET0_EIMR                       (*(vuint32*)(0xFC0D4008))
#define MCF_ENET0_RDAR                       (*(vuint32*)(0xFC0D4010))
#define MCF_ENET0_TDAR                       (*(vuint32*)(0xFC0D4014))
#define MCF_ENET0_ECR                        (*(vuint32*)(0xFC0D4024))
#define MCF_ENET0_MMFR                       (*(vuint32*)(0xFC0D4040))
#define MCF_ENET0_MSCR                       (*(vuint32*)(0xFC0D4044))
#define MCF_ENET0_MIBC                       (*(vuint32*)(0xFC0D4064))
#define MCF_ENET0_RCR                        (*(vuint32*)(0xFC0D4084))
#define MCF_ENET0_TCR                        (*(vuint32*)(0xFC0D40C4))
#define MCF_ENET0_PALR                       (*(vuint32*)(0xFC0D40E4))
#define MCF_ENET0_PAUR                       (*(vuint32*)(0xFC0D40E8))
#define MCF_ENET0_OPD                        (*(vuint32*)(0xFC0D40EC))
#define MCF_ENET0_IAUR                       (*(vuint32*)(0xFC0D4118))
#define MCF_ENET0_IALR                       (*(vuint32*)(0xFC0D411C))
#define MCF_ENET0_GAUR                       (*(vuint32*)(0xFC0D4120))
#define MCF_ENET0_GALR                       (*(vuint32*)(0xFC0D4124))
#define MCF_ENET0_TFWR                       (*(vuint32*)(0xFC0D4144))
#define MCF_ENET0_RDSR                       (*(vuint32*)(0xFC0D4180))
#define MCF_ENET0_TDSR                       (*(vuint32*)(0xFC0D4184))
#define MCF_ENET0_MRBR                       (*(vuint32*)(0xFC0D4188))
#define MCF_ENET0_RSFL                       (*(vuint32*)(0xFC0D4190))
#define MCF_ENET0_RSEM                       (*(vuint32*)(0xFC0D4194))
#define MCF_ENET0_RAEM                       (*(vuint32*)(0xFC0D4198))
#define MCF_ENET0_RAFL                       (*(vuint32*)(0xFC0D419C))
#define MCF_ENET0_TSEM                       (*(vuint32*)(0xFC0D41A0))
#define MCF_ENET0_TAEM                       (*(vuint32*)(0xFC0D41A4))
#define MCF_ENET0_TAFL                       (*(vuint32*)(0xFC0D41A8))
#define MCF_ENET0_TIPG                       (*(vuint32*)(0xFC0D41AC))
#define MCF_ENET0_FTRL                       (*(vuint32*)(0xFC0D41B0))
#define MCF_ENET0_TACC                       (*(vuint32*)(0xFC0D41C0))
#define MCF_ENET0_RACC                       (*(vuint32*)(0xFC0D41C4))
#define MCF_ENET0_RMON_T_DROP                (*(vuint32*)(0xFC0D4200))
#define MCF_ENET0_RMON_T_PACKETS             (*(vuint32*)(0xFC0D4204))
#define MCF_ENET0_RMON_T_BC_PKT              (*(vuint32*)(0xFC0D4208))
#define MCF_ENET0_RMON_T_MC_PKT              (*(vuint32*)(0xFC0D420C))
#define MCF_ENET0_RMON_T_CRC_ALIGN           (*(vuint32*)(0xFC0D4210))
#define MCF_ENET0_RMON_T_UNDERSIZE           (*(vuint32*)(0xFC0D4214))
#define MCF_ENET0_RMON_T_OVERSIZE            (*(vuint32*)(0xFC0D4218))
#define MCF_ENET0_RMON_T_FRAG                (*(vuint32*)(0xFC0D421C))
#define MCF_ENET0_RMON_T_JAB                 (*(vuint32*)(0xFC0D4220))
#define MCF_ENET0_RMON_T_COL                 (*(vuint32*)(0xFC0D4224))
#define MCF_ENET0_RMON_T_P64                 (*(vuint32*)(0xFC0D4228))
#define MCF_ENET0_RMON_T_P65TO127            (*(vuint32*)(0xFC0D422C))
#define MCF_ENET0_RMON_T_P128TO255           (*(vuint32*)(0xFC0D4230))
#define MCF_ENET0_RMON_T_P256TO511           (*(vuint32*)(0xFC0D4234))
#define MCF_ENET0_RMON_T_P512TO1023          (*(vuint32*)(0xFC0D4238))
#define MCF_ENET0_RMON_T_P1024TO2047         (*(vuint32*)(0xFC0D423C))
#define MCF_ENET0_RMON_T_P_GTE2048           (*(vuint32*)(0xFC0D4240))
#define MCF_ENET0_RMON_T_OCTETS              (*(vuint32*)(0xFC0D4244))
#define MCF_ENET0_IEEE_T_DROP                (*(vuint32*)(0xFC0D4248))
#define MCF_ENET0_IEEE_T_FRAME_OK            (*(vuint32*)(0xFC0D424C))
#define MCF_ENET0_IEEE_T_1COL                (*(vuint32*)(0xFC0D4250))
#define MCF_ENET0_IEEE_T_MCOL                (*(vuint32*)(0xFC0D4254))
#define MCF_ENET0_IEEE_T_DEF                 (*(vuint32*)(0xFC0D4258))
#define MCF_ENET0_IEEE_T_LCOL                (*(vuint32*)(0xFC0D425C))
#define MCF_ENET0_IEEE_T_EXCOL               (*(vuint32*)(0xFC0D4260))
#define MCF_ENET0_IEEE_T_MACERR              (*(vuint32*)(0xFC0D4264))
#define MCF_ENET0_IEEE_T_CSERR               (*(vuint32*)(0xFC0D4268))
#define MCF_ENET0_IEEE_T_SQE                 (*(vuint32*)(0xFC0D426C))
#define MCF_ENET0_IEEE_T_FDXFC               (*(vuint32*)(0xFC0D4270))
#define MCF_ENET0_IEEE_T_OCTETS_OK           (*(vuint32*)(0xFC0D4274))
#define MCF_ENET0_RMON_R_PACKETS             (*(vuint32*)(0xFC0D4284))
#define MCF_ENET0_RMON_R_BC_PKT              (*(vuint32*)(0xFC0D4288))
#define MCF_ENET0_RMON_R_MC_PKT              (*(vuint32*)(0xFC0D428C))
#define MCF_ENET0_RMON_R_CRC_ALIGN           (*(vuint32*)(0xFC0D4290))
#define MCF_ENET0_RMON_R_UNDERSIZE           (*(vuint32*)(0xFC0D4294))
#define MCF_ENET0_RMON_R_OVERSIZE            (*(vuint32*)(0xFC0D4298))
#define MCF_ENET0_RMON_R_FRAG                (*(vuint32*)(0xFC0D429C))
#define MCF_ENET0_RMON_R_JAB                 (*(vuint32*)(0xFC0D42A0))
#define MCF_ENET0_RMON_R_RESVD_0             (*(vuint32*)(0xFC0D42A4))
#define MCF_ENET0_RMON_R_P64                 (*(vuint32*)(0xFC0D42A8))
#define MCF_ENET0_RMON_R_P65TO127            (*(vuint32*)(0xFC0D42AC))
#define MCF_ENET0_RMON_R_P128TO255           (*(vuint32*)(0xFC0D42B0))
#define MCF_ENET0_RMON_R_P256TO511           (*(vuint32*)(0xFC0D42B4))
#define MCF_ENET0_RMON_R_P512TO1023          (*(vuint32*)(0xFC0D42B8))
#define MCF_ENET0_RMON_R_P1024TO2047         (*(vuint32*)(0xFC0D42BC))
#define MCF_ENET0_RMON_R_P_GTE2048           (*(vuint32*)(0xFC0D42C0))
#define MCF_ENET0_RMON_R_OCTETS              (*(vuint32*)(0xFC0D42C4))
#define MCF_ENET0_IEEE_R_DROP                (*(vuint32*)(0xFC0D42C8))
#define MCF_ENET0_IEEE_R_FRAME_OK            (*(vuint32*)(0xFC0D42CC))
#define MCF_ENET0_IEEE_R_CRC                 (*(vuint32*)(0xFC0D42D0))
#define MCF_ENET0_IEEE_R_ALIGN               (*(vuint32*)(0xFC0D42D4))
#define MCF_ENET0_IEEE_R_MACERR              (*(vuint32*)(0xFC0D42D8))
#define MCF_ENET0_IEEE_R_FDXFC               (*(vuint32*)(0xFC0D42DC))
#define MCF_ENET0_IEEE_R_OCTETS_OK           (*(vuint32*)(0xFC0D42E0))
#define MCF_ENET0_ATCR                       (*(vuint32*)(0xFC0D4400))
#define MCF_ENET0_ATVR                       (*(vuint32*)(0xFC0D4404))
#define MCF_ENET0_ATOFF                      (*(vuint32*)(0xFC0D4408))
#define MCF_ENET0_ATPER                      (*(vuint32*)(0xFC0D440C))
#define MCF_ENET0_ATCOR                      (*(vuint32*)(0xFC0D4410))
#define MCF_ENET0_ATINC                      (*(vuint32*)(0xFC0D4414))
#define MCF_ENET0_ATSTMP                     (*(vuint32*)(0xFC0D4418))
#define MCF_ENET0_SMACL0                     (*(vuint32*)(0xFC0D4500))
#define MCF_ENET0_SMACU0                     (*(vuint32*)(0xFC0D4504))
#define MCF_ENET0_SMACL1                     (*(vuint32*)(0xFC0D4508))
#define MCF_ENET0_SMACU1                     (*(vuint32*)(0xFC0D450C))
#define MCF_ENET0_SMACL2                     (*(vuint32*)(0xFC0D4510))
#define MCF_ENET0_SMACU2                     (*(vuint32*)(0xFC0D4514))
#define MCF_ENET0_SMACL3                     (*(vuint32*)(0xFC0D4518))
#define MCF_ENET0_SMACU3                     (*(vuint32*)(0xFC0D451C))
#define MCF_ENET0_SMACL(x)                   (*(vuint32*)(0xFC0D4500 + ((x)*0x8)))
#define MCF_ENET0_SMACU(x)                   (*(vuint32*)(0xFC0D4504 + ((x)*0x8)))

#define MCF_ENET1_EIR                        (*(vuint32*)(0xFC0D8004))
#define MCF_ENET1_EIMR                       (*(vuint32*)(0xFC0D8008))
#define MCF_ENET1_RDAR                       (*(vuint32*)(0xFC0D8010))
#define MCF_ENET1_TDAR                       (*(vuint32*)(0xFC0D8014))
#define MCF_ENET1_ECR                        (*(vuint32*)(0xFC0D8024))
#define MCF_ENET1_MMFR                       (*(vuint32*)(0xFC0D8040))
#define MCF_ENET1_MSCR                       (*(vuint32*)(0xFC0D8044))
#define MCF_ENET1_MIBC                       (*(vuint32*)(0xFC0D8064))
#define MCF_ENET1_RCR                        (*(vuint32*)(0xFC0D8084))
#define MCF_ENET1_TCR                        (*(vuint32*)(0xFC0D80C4))
#define MCF_ENET1_PALR                       (*(vuint32*)(0xFC0D80E4))
#define MCF_ENET1_PAUR                       (*(vuint32*)(0xFC0D80E8))
#define MCF_ENET1_OPD                        (*(vuint32*)(0xFC0D80EC))
#define MCF_ENET1_IAUR                       (*(vuint32*)(0xFC0D8118))
#define MCF_ENET1_IALR                       (*(vuint32*)(0xFC0D811C))
#define MCF_ENET1_GAUR                       (*(vuint32*)(0xFC0D8120))
#define MCF_ENET1_GALR                       (*(vuint32*)(0xFC0D8124))
#define MCF_ENET1_TFWR                       (*(vuint32*)(0xFC0D8144))
#define MCF_ENET1_RDSR                       (*(vuint32*)(0xFC0D8180))
#define MCF_ENET1_TDSR                       (*(vuint32*)(0xFC0D8184))
#define MCF_ENET1_MRBR                       (*(vuint32*)(0xFC0D8188))
#define MCF_ENET1_RSFL                       (*(vuint32*)(0xFC0D8190))
#define MCF_ENET1_RSEM                       (*(vuint32*)(0xFC0D8194))
#define MCF_ENET1_RAEM                       (*(vuint32*)(0xFC0D8198))
#define MCF_ENET1_RAFL                       (*(vuint32*)(0xFC0D819C))
#define MCF_ENET1_TSEM                       (*(vuint32*)(0xFC0D81A0))
#define MCF_ENET1_TAEM                       (*(vuint32*)(0xFC0D81A4))
#define MCF_ENET1_TAFL                       (*(vuint32*)(0xFC0D81A8))
#define MCF_ENET1_TIPG                       (*(vuint32*)(0xFC0D81AC))
#define MCF_ENET1_FTRL                       (*(vuint32*)(0xFC0D81B0))
#define MCF_ENET1_TACC                       (*(vuint32*)(0xFC0D81C0))
#define MCF_ENET1_RACC                       (*(vuint32*)(0xFC0D81C4))
#define MCF_ENET1_RMON_T_DROP                (*(vuint32*)(0xFC0D8200))
#define MCF_ENET1_RMON_T_PACKETS             (*(vuint32*)(0xFC0D8204))
#define MCF_ENET1_RMON_T_BC_PKT              (*(vuint32*)(0xFC0D8208))
#define MCF_ENET1_RMON_T_MC_PKT              (*(vuint32*)(0xFC0D820C))
#define MCF_ENET1_RMON_T_CRC_ALIGN           (*(vuint32*)(0xFC0D8210))
#define MCF_ENET1_RMON_T_UNDERSIZE           (*(vuint32*)(0xFC0D8214))
#define MCF_ENET1_RMON_T_OVERSIZE            (*(vuint32*)(0xFC0D8218))
#define MCF_ENET1_RMON_T_FRAG                (*(vuint32*)(0xFC0D821C))
#define MCF_ENET1_RMON_T_JAB                 (*(vuint32*)(0xFC0D8220))
#define MCF_ENET1_RMON_T_COL                 (*(vuint32*)(0xFC0D8224))
#define MCF_ENET1_RMON_T_P64                 (*(vuint32*)(0xFC0D8228))
#define MCF_ENET1_RMON_T_P65TO127            (*(vuint32*)(0xFC0D822C))
#define MCF_ENET1_RMON_T_P128TO255           (*(vuint32*)(0xFC0D8230))
#define MCF_ENET1_RMON_T_P256TO511           (*(vuint32*)(0xFC0D8234))
#define MCF_ENET1_RMON_T_P512TO1023          (*(vuint32*)(0xFC0D8238))
#define MCF_ENET1_RMON_T_P1024TO2047         (*(vuint32*)(0xFC0D823C))
#define MCF_ENET1_RMON_T_P_GTE2048           (*(vuint32*)(0xFC0D8240))
#define MCF_ENET1_RMON_T_OCTETS              (*(vuint32*)(0xFC0D8244))
#define MCF_ENET1_IEEE_T_DROP                (*(vuint32*)(0xFC0D8248))
#define MCF_ENET1_IEEE_T_FRAME_OK            (*(vuint32*)(0xFC0D824C))
#define MCF_ENET1_IEEE_T_1COL                (*(vuint32*)(0xFC0D8250))
#define MCF_ENET1_IEEE_T_MCOL                (*(vuint32*)(0xFC0D8254))
#define MCF_ENET1_IEEE_T_DEF                 (*(vuint32*)(0xFC0D8258))
#define MCF_ENET1_IEEE_T_LCOL                (*(vuint32*)(0xFC0D825C))
#define MCF_ENET1_IEEE_T_EXCOL               (*(vuint32*)(0xFC0D8260))
#define MCF_ENET1_IEEE_T_MACERR              (*(vuint32*)(0xFC0D8264))
#define MCF_ENET1_IEEE_T_CSERR               (*(vuint32*)(0xFC0D8268))
#define MCF_ENET1_IEEE_T_SQE                 (*(vuint32*)(0xFC0D826C))
#define MCF_ENET1_IEEE_T_FDXFC               (*(vuint32*)(0xFC0D8270))
#define MCF_ENET1_IEEE_T_OCTETS_OK           (*(vuint32*)(0xFC0D8274))
#define MCF_ENET1_RMON_R_PACKETS             (*(vuint32*)(0xFC0D8284))
#define MCF_ENET1_RMON_R_BC_PKT              (*(vuint32*)(0xFC0D8288))
#define MCF_ENET1_RMON_R_MC_PKT              (*(vuint32*)(0xFC0D828C))
#define MCF_ENET1_RMON_R_CRC_ALIGN           (*(vuint32*)(0xFC0D8290))
#define MCF_ENET1_RMON_R_UNDERSIZE           (*(vuint32*)(0xFC0D8294))
#define MCF_ENET1_RMON_R_OVERSIZE            (*(vuint32*)(0xFC0D8298))
#define MCF_ENET1_RMON_R_FRAG                (*(vuint32*)(0xFC0D829C))
#define MCF_ENET1_RMON_R_JAB                 (*(vuint32*)(0xFC0D82A0))
#define MCF_ENET1_RMON_R_RESVD_0             (*(vuint32*)(0xFC0D82A4))
#define MCF_ENET1_RMON_R_P64                 (*(vuint32*)(0xFC0D82A8))
#define MCF_ENET1_RMON_R_P65TO127            (*(vuint32*)(0xFC0D82AC))
#define MCF_ENET1_RMON_R_P128TO255           (*(vuint32*)(0xFC0D82B0))
#define MCF_ENET1_RMON_R_P256TO511           (*(vuint32*)(0xFC0D82B4))
#define MCF_ENET1_RMON_R_P512TO1023          (*(vuint32*)(0xFC0D82B8))
#define MCF_ENET1_RMON_R_P1024TO2047         (*(vuint32*)(0xFC0D82BC))
#define MCF_ENET1_RMON_R_P_GTE2048           (*(vuint32*)(0xFC0D82C0))
#define MCF_ENET1_RMON_R_OCTETS              (*(vuint32*)(0xFC0D82C4))
#define MCF_ENET1_IEEE_R_DROP                (*(vuint32*)(0xFC0D82C8))
#define MCF_ENET1_IEEE_R_FRAME_OK            (*(vuint32*)(0xFC0D82CC))
#define MCF_ENET1_IEEE_R_CRC                 (*(vuint32*)(0xFC0D82D0))
#define MCF_ENET1_IEEE_R_ALIGN               (*(vuint32*)(0xFC0D82D4))
#define MCF_ENET1_IEEE_R_MACERR              (*(vuint32*)(0xFC0D82D8))
#define MCF_ENET1_IEEE_R_FDXFC               (*(vuint32*)(0xFC0D82DC))
#define MCF_ENET1_IEEE_R_OCTETS_OK           (*(vuint32*)(0xFC0D82E0))
#define MCF_ENET1_ATCR                       (*(vuint32*)(0xFC0D8400))
#define MCF_ENET1_ATVR                       (*(vuint32*)(0xFC0D8404))
#define MCF_ENET1_ATOFF                      (*(vuint32*)(0xFC0D8408))
#define MCF_ENET1_ATPER                      (*(vuint32*)(0xFC0D840C))
#define MCF_ENET1_ATCOR                      (*(vuint32*)(0xFC0D8410))
#define MCF_ENET1_ATINC                      (*(vuint32*)(0xFC0D8414))
#define MCF_ENET1_ATSTMP                     (*(vuint32*)(0xFC0D8418))
#define MCF_ENET1_SMACL0                     (*(vuint32*)(0xFC0D8500))
#define MCF_ENET1_SMACU0                     (*(vuint32*)(0xFC0D8504))
#define MCF_ENET1_SMACL1                     (*(vuint32*)(0xFC0D8508))
#define MCF_ENET1_SMACU1                     (*(vuint32*)(0xFC0D850C))
#define MCF_ENET1_SMACL2                     (*(vuint32*)(0xFC0D8510))
#define MCF_ENET1_SMACU2                     (*(vuint32*)(0xFC0D8514))
#define MCF_ENET1_SMACL3                     (*(vuint32*)(0xFC0D8518))
#define MCF_ENET1_SMACU3                     (*(vuint32*)(0xFC0D851C))
#define MCF_ENET1_SMACL(x)                   (*(vuint32*)(0xFC0D8500 + ((x)*0x8)))
#define MCF_ENET1_SMACU(x)                   (*(vuint32*)(0xFC0D8504 + ((x)*0x8)))

#define MCF_ENET_EIR(x)                      (*(vuint32*)(0xFC0D4004 + ((x)*0x4000)))
#define MCF_ENET_EIMR(x)                     (*(vuint32*)(0xFC0D4008 + ((x)*0x4000)))
#define MCF_ENET_RDAR(x)                     (*(vuint32*)(0xFC0D4010 + ((x)*0x4000)))
#define MCF_ENET_TDAR(x)                     (*(vuint32*)(0xFC0D4014 + ((x)*0x4000)))
#define MCF_ENET_ECR(x)                      (*(vuint32*)(0xFC0D4024 + ((x)*0x4000)))
#define MCF_ENET_MMFR(x)                     (*(vuint32*)(0xFC0D4040 + ((x)*0x4000)))
#define MCF_ENET_MSCR(x)                     (*(vuint32*)(0xFC0D4044 + ((x)*0x4000)))
#define MCF_ENET_MIBC(x)                     (*(vuint32*)(0xFC0D4064 + ((x)*0x4000)))
#define MCF_ENET_RCR(x)                      (*(vuint32*)(0xFC0D4084 + ((x)*0x4000)))
#define MCF_ENET_TCR(x)                      (*(vuint32*)(0xFC0D40C4 + ((x)*0x4000)))
#define MCF_ENET_PALR(x)                     (*(vuint32*)(0xFC0D40E4 + ((x)*0x4000)))
#define MCF_ENET_PAUR(x)                     (*(vuint32*)(0xFC0D40E8 + ((x)*0x4000)))
#define MCF_ENET_OPD(x)                      (*(vuint32*)(0xFC0D40EC + ((x)*0x4000)))
#define MCF_ENET_IAUR(x)                     (*(vuint32*)(0xFC0D4118 + ((x)*0x4000)))
#define MCF_ENET_IALR(x)                     (*(vuint32*)(0xFC0D411C + ((x)*0x4000)))
#define MCF_ENET_GAUR(x)                     (*(vuint32*)(0xFC0D4120 + ((x)*0x4000)))
#define MCF_ENET_GALR(x)                     (*(vuint32*)(0xFC0D4124 + ((x)*0x4000)))
#define MCF_ENET_TFWR(x)                     (*(vuint32*)(0xFC0D4144 + ((x)*0x4000)))
#define MCF_ENET_RDSR(x)                     (*(vuint32*)(0xFC0D4180 + ((x)*0x4000)))
#define MCF_ENET_TDSR(x)                     (*(vuint32*)(0xFC0D4184 + ((x)*0x4000)))
#define MCF_ENET_MRBR(x)                     (*(vuint32*)(0xFC0D4188 + ((x)*0x4000)))
#define MCF_ENET_RSFL(x)                     (*(vuint32*)(0xFC0D4190 + ((x)*0x4000)))
#define MCF_ENET_RSEM(x)                     (*(vuint32*)(0xFC0D4194 + ((x)*0x4000)))
#define MCF_ENET_RAEM(x)                     (*(vuint32*)(0xFC0D4198 + ((x)*0x4000)))
#define MCF_ENET_RAFL(x)                     (*(vuint32*)(0xFC0D419C + ((x)*0x4000)))
#define MCF_ENET_TSEM(x)                     (*(vuint32*)(0xFC0D41A0 + ((x)*0x4000)))
#define MCF_ENET_TAEM(x)                     (*(vuint32*)(0xFC0D41A4 + ((x)*0x4000)))
#define MCF_ENET_TAFL(x)                     (*(vuint32*)(0xFC0D41A8 + ((x)*0x4000)))
#define MCF_ENET_TIPG(x)                     (*(vuint32*)(0xFC0D41AC + ((x)*0x4000)))
#define MCF_ENET_FTRL(x)                     (*(vuint32*)(0xFC0D41B0 + ((x)*0x4000)))
#define MCF_ENET_TACC(x)                     (*(vuint32*)(0xFC0D41C0 + ((x)*0x4000)))
#define MCF_ENET_RACC(x)                     (*(vuint32*)(0xFC0D41C4 + ((x)*0x4000)))
#define MCF_ENET_RMON_T_DROP(x)              (*(vuint32*)(0xFC0D4200 + ((x)*0x4000)))
#define MCF_ENET_RMON_T_PACKETS(x)           (*(vuint32*)(0xFC0D4204 + ((x)*0x4000)))
#define MCF_ENET_RMON_T_BC_PKT(x)            (*(vuint32*)(0xFC0D4208 + ((x)*0x4000)))
#define MCF_ENET_RMON_T_MC_PKT(x)            (*(vuint32*)(0xFC0D420C + ((x)*0x4000)))
#define MCF_ENET_RMON_T_CRC_ALIGN(x)         (*(vuint32*)(0xFC0D4210 + ((x)*0x4000)))
#define MCF_ENET_RMON_T_UNDERSIZE(x)         (*(vuint32*)(0xFC0D4214 + ((x)*0x4000)))
#define MCF_ENET_RMON_T_OVERSIZE(x)          (*(vuint32*)(0xFC0D4218 + ((x)*0x4000)))
#define MCF_ENET_RMON_T_FRAG(x)              (*(vuint32*)(0xFC0D421C + ((x)*0x4000)))
#define MCF_ENET_RMON_T_JAB(x)               (*(vuint32*)(0xFC0D4220 + ((x)*0x4000)))
#define MCF_ENET_RMON_T_COL(x)               (*(vuint32*)(0xFC0D4224 + ((x)*0x4000)))
#define MCF_ENET_RMON_T_P64(x)               (*(vuint32*)(0xFC0D4228 + ((x)*0x4000)))
#define MCF_ENET_RMON_T_P65TO127(x)          (*(vuint32*)(0xFC0D422C + ((x)*0x4000)))
#define MCF_ENET_RMON_T_P128TO255(x)         (*(vuint32*)(0xFC0D4230 + ((x)*0x4000)))
#define MCF_ENET_RMON_T_P256TO511(x)         (*(vuint32*)(0xFC0D4234 + ((x)*0x4000)))
#define MCF_ENET_RMON_T_P512TO1023(x)        (*(vuint32*)(0xFC0D4238 + ((x)*0x4000)))
#define MCF_ENET_RMON_T_P1024TO2047(x)       (*(vuint32*)(0xFC0D423C + ((x)*0x4000)))
#define MCF_ENET_RMON_T_P_GTE2048(x)         (*(vuint32*)(0xFC0D4240 + ((x)*0x4000)))
#define MCF_ENET_RMON_T_OCTETS(x)            (*(vuint32*)(0xFC0D4244 + ((x)*0x4000)))
#define MCF_ENET_IEEE_T_DROP(x)              (*(vuint32*)(0xFC0D4248 + ((x)*0x4000)))
#define MCF_ENET_IEEE_T_FRAME_OK(x)          (*(vuint32*)(0xFC0D424C + ((x)*0x4000)))
#define MCF_ENET_IEEE_T_1COL(x)              (*(vuint32*)(0xFC0D4250 + ((x)*0x4000)))
#define MCF_ENET_IEEE_T_MCOL(x)              (*(vuint32*)(0xFC0D4254 + ((x)*0x4000)))
#define MCF_ENET_IEEE_T_DEF(x)               (*(vuint32*)(0xFC0D4258 + ((x)*0x4000)))
#define MCF_ENET_IEEE_T_LCOL(x)              (*(vuint32*)(0xFC0D425C + ((x)*0x4000)))
#define MCF_ENET_IEEE_T_EXCOL(x)             (*(vuint32*)(0xFC0D4260 + ((x)*0x4000)))
#define MCF_ENET_IEEE_T_MACERR(x)            (*(vuint32*)(0xFC0D4264 + ((x)*0x4000)))
#define MCF_ENET_IEEE_T_CSERR(x)             (*(vuint32*)(0xFC0D4268 + ((x)*0x4000)))
#define MCF_ENET_IEEE_T_SQE(x)               (*(vuint32*)(0xFC0D426C + ((x)*0x4000)))
#define MCF_ENET_IEEE_T_FDXFC(x)             (*(vuint32*)(0xFC0D4270 + ((x)*0x4000)))
#define MCF_ENET_IEEE_T_OCTETS_OK(x)         (*(vuint32*)(0xFC0D4274 + ((x)*0x4000)))
#define MCF_ENET_RMON_R_PACKETS(x)           (*(vuint32*)(0xFC0D4284 + ((x)*0x4000)))
#define MCF_ENET_RMON_R_BC_PKT(x)            (*(vuint32*)(0xFC0D4288 + ((x)*0x4000)))
#define MCF_ENET_RMON_R_MC_PKT(x)            (*(vuint32*)(0xFC0D428C + ((x)*0x4000)))
#define MCF_ENET_RMON_R_CRC_ALIGN(x)         (*(vuint32*)(0xFC0D4290 + ((x)*0x4000)))
#define MCF_ENET_RMON_R_UNDERSIZE(x)         (*(vuint32*)(0xFC0D4294 + ((x)*0x4000)))
#define MCF_ENET_RMON_R_OVERSIZE(x)          (*(vuint32*)(0xFC0D4298 + ((x)*0x4000)))
#define MCF_ENET_RMON_R_FRAG(x)              (*(vuint32*)(0xFC0D429C + ((x)*0x4000)))
#define MCF_ENET_RMON_R_JAB(x)               (*(vuint32*)(0xFC0D42A0 + ((x)*0x4000)))
#define MCF_ENET_RMON_R_RESVD_0(x)           (*(vuint32*)(0xFC0D42A4 + ((x)*0x4000)))
#define MCF_ENET_RMON_R_P64(x)               (*(vuint32*)(0xFC0D42A8 + ((x)*0x4000)))
#define MCF_ENET_RMON_R_P65TO127(x)          (*(vuint32*)(0xFC0D42AC + ((x)*0x4000)))
#define MCF_ENET_RMON_R_P128TO255(x)         (*(vuint32*)(0xFC0D42B0 + ((x)*0x4000)))
#define MCF_ENET_RMON_R_P256TO511(x)         (*(vuint32*)(0xFC0D42B4 + ((x)*0x4000)))
#define MCF_ENET_RMON_R_P512TO1023(x)        (*(vuint32*)(0xFC0D42B8 + ((x)*0x4000)))
#define MCF_ENET_RMON_R_P1024TO2047(x)       (*(vuint32*)(0xFC0D42BC + ((x)*0x4000)))
#define MCF_ENET_RMON_R_P_GTE2048(x)         (*(vuint32*)(0xFC0D42C0 + ((x)*0x4000)))
#define MCF_ENET_RMON_R_OCTETS(x)            (*(vuint32*)(0xFC0D42C4 + ((x)*0x4000)))
#define MCF_ENET_IEEE_R_DROP(x)              (*(vuint32*)(0xFC0D42C8 + ((x)*0x4000)))
#define MCF_ENET_IEEE_R_FRAME_OK(x)          (*(vuint32*)(0xFC0D42CC + ((x)*0x4000)))
#define MCF_ENET_IEEE_R_CRC(x)               (*(vuint32*)(0xFC0D42D0 + ((x)*0x4000)))
#define MCF_ENET_IEEE_R_ALIGN(x)             (*(vuint32*)(0xFC0D42D4 + ((x)*0x4000)))
#define MCF_ENET_IEEE_R_MACERR(x)            (*(vuint32*)(0xFC0D42D8 + ((x)*0x4000)))
#define MCF_ENET_IEEE_R_FDXFC(x)             (*(vuint32*)(0xFC0D42DC + ((x)*0x4000)))
#define MCF_ENET_IEEE_R_OCTETS_OK(x)         (*(vuint32*)(0xFC0D42E0 + ((x)*0x4000)))
#define MCF_ENET_ATCR(x)                     (*(vuint32*)(0xFC0D4400 + ((x)*0x4000)))
#define MCF_ENET_ATVR(x)                     (*(vuint32*)(0xFC0D4404 + ((x)*0x4000)))
#define MCF_ENET_ATOFF(x)                    (*(vuint32*)(0xFC0D4408 + ((x)*0x4000)))
#define MCF_ENET_ATPER(x)                    (*(vuint32*)(0xFC0D440C + ((x)*0x4000)))
#define MCF_ENET_ATCOR(x)                    (*(vuint32*)(0xFC0D4410 + ((x)*0x4000)))
#define MCF_ENET_ATINC(x)                    (*(vuint32*)(0xFC0D4414 + ((x)*0x4000)))
#define MCF_ENET_ATSTMP(x)                   (*(vuint32*)(0xFC0D4418 + ((x)*0x4000)))
#define MCF_ENET_SMACL0(x)                   (*(vuint32*)(0xFC0D4500 + ((x)*0x4000)))
#define MCF_ENET_SMACU0(x)                   (*(vuint32*)(0xFC0D4504 + ((x)*0x4000)))
#define MCF_ENET_SMACL1(x)                   (*(vuint32*)(0xFC0D4508 + ((x)*0x4000)))
#define MCF_ENET_SMACU1(x)                   (*(vuint32*)(0xFC0D450C + ((x)*0x4000)))
#define MCF_ENET_SMACL2(x)                   (*(vuint32*)(0xFC0D4510 + ((x)*0x4000)))
#define MCF_ENET_SMACU2(x)                   (*(vuint32*)(0xFC0D4514 + ((x)*0x4000)))
#define MCF_ENET_SMACL3(x)                   (*(vuint32*)(0xFC0D4518 + ((x)*0x4000)))
#define MCF_ENET_SMACU3(x)                   (*(vuint32*)(0xFC0D451C + ((x)*0x4000)))


/* Bit definitions and macros for MCF_ENET_EIR */
#define MCF_ENET_EIR_TS_TIMER                (0x8000)
#define MCF_ENET_EIR_TS_AVAIL                (0x10000)
#define MCF_ENET_EIR_WAKEUP                  (0x20000)
#define MCF_ENET_EIR_PLR                     (0x40000)
#define MCF_ENET_EIR_UN                      (0x80000)
#define MCF_ENET_EIR_RL                      (0x100000)
#define MCF_ENET_EIR_LC                      (0x200000)
#define MCF_ENET_EIR_EBERR                   (0x400000)
#define MCF_ENET_EIR_MII                     (0x800000)
#define MCF_ENET_EIR_RXB                     (0x1000000)
#define MCF_ENET_EIR_RXF                     (0x2000000)
#define MCF_ENET_EIR_TXB                     (0x4000000)
#define MCF_ENET_EIR_TXF                     (0x8000000)
#define MCF_ENET_EIR_GRA                     (0x10000000)
#define MCF_ENET_EIR_BABT                    (0x20000000)
#define MCF_ENET_EIR_BABR                    (0x40000000)
#define MCF_ENET_EIR_CLEAR_ALL               (0xFFFFFFFF)

/* Bit definitions and macros for MCF_ENET_EIMR */
#define MCF_ENET_EIMR_TS_TIMER               (0x8000)
#define MCF_ENET_EIMR_TS_AVAIL               (0x10000)
#define MCF_ENET_EIMR_WAKEUP                 (0x20000)
#define MCF_ENET_EIMR_PLR                    (0x40000)
#define MCF_ENET_EIMR_UN                     (0x80000)
#define MCF_ENET_EIMR_RL                     (0x100000)
#define MCF_ENET_EIMR_LC                     (0x200000)
#define MCF_ENET_EIMR_EBERR                  (0x400000)
#define MCF_ENET_EIMR_MII                    (0x800000)
#define MCF_ENET_EIMR_RXB                    (0x1000000)
#define MCF_ENET_EIMR_RXF                    (0x2000000)
#define MCF_ENET_EIMR_TXB                    (0x4000000)
#define MCF_ENET_EIMR_TXF                    (0x8000000)
#define MCF_ENET_EIMR_GRA                    (0x10000000)
#define MCF_ENET_EIMR_BABT                   (0x20000000)
#define MCF_ENET_EIMR_BABR                   (0x40000000)
#define MCF_ENET_EIMR_MASK_ALL               (0)
#define MCF_ENET_EIMR_UNMASK_ALL             (0xFFFFFFFF)

/* Bit definitions and macros for MCF_ENET_RDAR */
#define MCF_ENET_RDAR_RDAR                   (0x1000000)

/* Bit definitions and macros for MCF_ENET_TDAR */
#define MCF_ENET_TDAR_TDAR                   (0x1000000)

/* Bit definitions and macros for MCF_ENET_ECR */
#define MCF_ENET_ECR_RESET                   (0x1)
#define MCF_ENET_ECR_ETHEREN                 (0x2)
#define MCF_ENET_ECR_MAGICEN                 (0x4)
#define MCF_ENET_ECR_SLEEP                   (0x8)
#define MCF_ENET_ECR_1588EN                  (0x10)
#define MCF_ENET_ECR_DBGEN                   (0x40)
#define MCF_ENET_ECR_STOPEN                  (0x80)

/* Bit definitions and macros for MCF_ENET_MMFR */
#define MCF_ENET_MMFR_DATA(x)                (((x)&0xFFFF)<<0)
#define MCF_ENET_MMFR_TA(x)                  (((x)&0x3)<<0x10)
#define MCF_ENET_MMFR_TA_10                  (0x20000)
#define MCF_ENET_MMFR_RA(x)                  (((x)&0x1F)<<0x12)
#define MCF_ENET_MMFR_PA(x)                  (((x)&0x1F)<<0x17)
#define MCF_ENET_MMFR_OP(x)                  (((x)&0x3)<<0x1C)
#define MCF_ENET_MMFR_OP_WRITE               (0x10000000)
#define MCF_ENET_MMFR_OP_READ                (0x20000000)
#define MCF_ENET_MMFR_ST(x)                  (((x)&0x3)<<0x1E)
#define MCF_ENET_MMFR_ST_01                  (0x40000000)

/* Bit definitions and macros for MCF_ENET_MSCR */
#define MCF_ENET_MSCR_MII_SPEED(x)           (((x)&0x3F)<<0x1)
#define MCF_ENET_MSCR_DIS_PRE                (0x80)
#define MCF_ENET_MSCR_HOLDTIME(x)            (((x)&0x7)<<0x8)

/* Bit definitions and macros for MCF_ENET_MIBC */
#define MCF_ENET_MIBC_MIB_CLEAR              (0x20000000)
#define MCF_ENET_MIBC_MIB_IDLE               (0x40000000)
#define MCF_ENET_MIBC_MIB_DIS                (0x80000000)

/* Bit definitions and macros for MCF_ENET_RCR */
#define MCF_ENET_RCR_LOOP                    (0x1)
#define MCF_ENET_RCR_DRT                     (0x2)
#define MCF_ENET_RCR_MII_MODE                (0x4)
#define MCF_ENET_RCR_PROM                    (0x8)
#define MCF_ENET_RCR_BC_REJ                  (0x10)
#define MCF_ENET_RCR_FCE                     (0x20)
#define MCF_ENET_RCR_RMII_MODE               (0x100)
#define MCF_ENET_RCR_RMII_10T                (0x200)
#define MCF_ENET_RCR_PADEN                   (0x1000)
#define MCF_ENET_RCR_PAUFWD                  (0x2000)
#define MCF_ENET_RCR_CRCFWD                  (0x4000)
#define MCF_ENET_RCR_CFEN                    (0x8000)
#define MCF_ENET_RCR_MAX_FL(x)               (((x)&0x3FFF)<<0x10)
#define MCF_ENET_RCR_NLC                     (0x40000000)
#define MCF_ENET_RCR_GRS                     (0x80000000)

/* Bit definitions and macros for MCF_ENET_TCR */
#define MCF_ENET_TCR_GTS                     (0x1)
#define MCF_ENET_TCR_FDEN                    (0x4)
#define MCF_ENET_TCR_TFC_PAUSE               (0x8)
#define MCF_ENET_TCR_RFC_PAUSE               (0x10)
#define MCF_ENET_TCR_ADDSEL(x)               (((x)&0x7)<<0x5)
#define MCF_ENET_TCR_ADDSEL_NODE             (0)
#define MCF_ENET_TCR_ADDSEL_SMAC0            (0x80)
#define MCF_ENET_TCR_ADDSEL_SMAC1            (0xA0)
#define MCF_ENET_TCR_ADDSEL_SMAC2            (0xC0)
#define MCF_ENET_TCR_ADDSEL_SMAC3            (0xE0)
#define MCF_ENET_TCR_ADDINS                  (0x100)
#define MCF_ENET_TCR_CRCFWD                  (0x200)

/* Bit definitions and macros for MCF_ENET_PALR */
#define MCF_ENET_PALR_PADDR1(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_PAUR */
#define MCF_ENET_PAUR_TYPE(x)                (((x)&0xFFFF)<<0)
#define MCF_ENET_PAUR_PADDR2(x)              (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_ENET_OPD */
#define MCF_ENET_OPD_PAUSE_DUR(x)            (((x)&0xFFFF)<<0)
#define MCF_ENET_OPD_OPCODE(x)               (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_ENET_IAUR */
#define MCF_ENET_IAUR_IADDR1(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IALR */
#define MCF_ENET_IALR_IADDR2(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_GAUR */
#define MCF_ENET_GAUR_GADDR1(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_GALR */
#define MCF_ENET_GALR_GADDR2(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_TFWR */
#define MCF_ENET_TFWR_TFWR(x)                (((x)&0x3F)<<0)
#define MCF_ENET_TFWR_STRFWD                 (0x100)

/* Bit definitions and macros for MCF_ENET_RDSR */
#define MCF_ENET_RDSR_R_DES_START(x)         (((x)&0x1FFFFFFF)<<0x3)

/* Bit definitions and macros for MCF_ENET_TDSR */
#define MCF_ENET_TDSR_X_DES_START(x)         (((x)&0x1FFFFFFF)<<0x3)

/* Bit definitions and macros for MCF_ENET_MRBR */
#define MCF_ENET_MRBR_R_BUF_SIZE(x)          (((x)&0x3FF)<<0x4)

/* Bit definitions and macros for MCF_ENET_RSFL */
#define MCF_ENET_RSFL_RX_SECTION_FULL(x)     (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_ENET_RSEM */
#define MCF_ENET_RSEM_RX_SECTION_EMPTY(x)    (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_ENET_RAEM */
#define MCF_ENET_RAEM_RX_ALMOST_EMPTY(x)     (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_ENET_RAFL */
#define MCF_ENET_RAFL_RX_ALMOST_FULL(x)      (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_ENET_TSEM */
#define MCF_ENET_TSEM_TX_SECTION_EMPTY(x)    (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_ENET_TAEM */
#define MCF_ENET_TAEM_TX_ALMOST_EMPTY(x)     (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_ENET_TAFL */
#define MCF_ENET_TAFL_TX_ALMOST_FULL(x)      (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_ENET_TIPG */
#define MCF_ENET_TIPG_IPG(x)                 (((x)&0x1F)<<0)

/* Bit definitions and macros for MCF_ENET_FTRL */
#define MCF_ENET_FTRL_TRUNC_FL(x)            (((x)&0x3FFF)<<0)

/* Bit definitions and macros for MCF_ENET_TACC */
#define MCF_ENET_TACC_SHIFT16                (0x1)
#define MCF_ENET_TACC_IPCHK                  (0x8)
#define MCF_ENET_TACC_PROCHK                 (0x10)

/* Bit definitions and macros for MCF_ENET_RACC */
#define MCF_ENET_RACC_PADREM                 (0x1)
#define MCF_ENET_RACC_IPDIS                  (0x2)
#define MCF_ENET_RACC_PRODIS                 (0x4)
#define MCF_ENET_RACC_LINEDIS                (0x40)
#define MCF_ENET_RACC_SHIFT16                (0x80)

/* Bit definitions and macros for MCF_ENET_RMON_T_DROP */
#define MCF_ENET_RMON_T_DROP_Value(x)        (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_T_PACKETS */
#define MCF_ENET_RMON_T_PACKETS_Value(x)     (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_T_BC_PKT */
#define MCF_ENET_RMON_T_BC_PKT_Value(x)      (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_T_MC_PKT */
#define MCF_ENET_RMON_T_MC_PKT_Value(x)      (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_T_CRC_ALIGN */
#define MCF_ENET_RMON_T_CRC_ALIGN_Value(x)   (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_T_UNDERSIZE */
#define MCF_ENET_RMON_T_UNDERSIZE_Value(x)   (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_T_OVERSIZE */
#define MCF_ENET_RMON_T_OVERSIZE_Value(x)    (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_T_FRAG */
#define MCF_ENET_RMON_T_FRAG_Value(x)        (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_T_JAB */
#define MCF_ENET_RMON_T_JAB_Value(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_T_COL */
#define MCF_ENET_RMON_T_COL_Value(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_T_P64 */
#define MCF_ENET_RMON_T_P64_Value(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_T_P65TO127 */
#define MCF_ENET_RMON_T_P65TO127_Value(x)    (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_T_P128TO255 */
#define MCF_ENET_RMON_T_P128TO255_Value(x)   (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_T_P256TO511 */
#define MCF_ENET_RMON_T_P256TO511_Value(x)   (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_T_P512TO1023 */
#define MCF_ENET_RMON_T_P512TO1023_Value(x)  (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_T_P1024TO2047 */
#define MCF_ENET_RMON_T_P1024TO2047_Value(x) (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_T_P_GTE2048 */
#define MCF_ENET_RMON_T_P_GTE2048_Value(x)   (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_T_OCTETS */
#define MCF_ENET_RMON_T_OCTETS_Value(x)      (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_T_DROP */
#define MCF_ENET_IEEE_T_DROP_Value(x)        (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_T_FRAME_OK */
#define MCF_ENET_IEEE_T_FRAME_OK_Value(x)    (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_T_1COL */
#define MCF_ENET_IEEE_T_1COL_Value(x)        (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_T_MCOL */
#define MCF_ENET_IEEE_T_MCOL_Value(x)        (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_T_DEF */
#define MCF_ENET_IEEE_T_DEF_Value(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_T_LCOL */
#define MCF_ENET_IEEE_T_LCOL_Value(x)        (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_T_EXCOL */
#define MCF_ENET_IEEE_T_EXCOL_Value(x)       (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_T_MACERR */
#define MCF_ENET_IEEE_T_MACERR_Value(x)      (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_T_CSERR */
#define MCF_ENET_IEEE_T_CSERR_Value(x)       (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_T_SQE */
#define MCF_ENET_IEEE_T_SQE_Value(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_T_FDXFC */
#define MCF_ENET_IEEE_T_FDXFC_Value(x)       (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_T_OCTETS_OK */
#define MCF_ENET_IEEE_T_OCTETS_OK_Value(x)   (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_R_PACKETS */
#define MCF_ENET_RMON_R_PACKETS_Value(x)     (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_R_BC_PKT */
#define MCF_ENET_RMON_R_BC_PKT_Value(x)      (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_R_MC_PKT */
#define MCF_ENET_RMON_R_MC_PKT_Value(x)      (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_R_CRC_ALIGN */
#define MCF_ENET_RMON_R_CRC_ALIGN_Value(x)   (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_R_UNDERSIZE */
#define MCF_ENET_RMON_R_UNDERSIZE_Value(x)   (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_R_OVERSIZE */
#define MCF_ENET_RMON_R_OVERSIZE_Value(x)    (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_R_FRAG */
#define MCF_ENET_RMON_R_FRAG_Value(x)        (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_R_JAB */
#define MCF_ENET_RMON_R_JAB_Value(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_R_RESVD_0 */
#define MCF_ENET_RMON_R_RESVD_0_Value(x)     (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_R_P64 */
#define MCF_ENET_RMON_R_P64_Value(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_R_P65TO127 */
#define MCF_ENET_RMON_R_P65TO127_Value(x)    (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_R_P128TO255 */
#define MCF_ENET_RMON_R_P128TO255_Value(x)   (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_R_P256TO511 */
#define MCF_ENET_RMON_R_P256TO511_Value(x)   (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_R_P512TO1023 */
#define MCF_ENET_RMON_R_P512TO1023_Value(x)  (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_R_P1024TO2047 */
#define MCF_ENET_RMON_R_P1024TO2047_Value(x) (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_R_P_GTE2048 */
#define MCF_ENET_RMON_R_P_GTE2048_Value(x)   (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_RMON_R_OCTETS */
#define MCF_ENET_RMON_R_OCTETS_Value(x)      (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_R_DROP */
#define MCF_ENET_IEEE_R_DROP_Value(x)        (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_R_FRAME_OK */
#define MCF_ENET_IEEE_R_FRAME_OK_Value(x)    (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_R_CRC */
#define MCF_ENET_IEEE_R_CRC_Value(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_R_ALIGN */
#define MCF_ENET_IEEE_R_ALIGN_Value(x)       (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_R_MACERR */
#define MCF_ENET_IEEE_R_MACERR_Value(x)      (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_R_FDXFC */
#define MCF_ENET_IEEE_R_FDXFC_Value(x)       (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_IEEE_R_OCTETS_OK */
#define MCF_ENET_IEEE_R_OCTETS_OK_Value(x)   (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_ATCR */
#define MCF_ENET_ATCR_EN                     (0x1)
#define MCF_ENET_ATCR_OFFEN                  (0x4)
#define MCF_ENET_ATCR_OFFRST                 (0x8)
#define MCF_ENET_ATCR_PEREN                  (0x10)
#define MCF_ENET_ATCR_PINPER                 (0x80)
#define MCF_ENET_ATCR_RESTART                (0x200)
#define MCF_ENET_ATCR_CAPTURE                (0x800)
#define MCF_ENET_ATCR_SLAVE                  (0x2000)

/* Bit definitions and macros for MCF_ENET_ATVR */
#define MCF_ENET_ATVR_ATIME(x)               (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_ATOFF */
#define MCF_ENET_ATOFF_OFFSET(x)             (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_ATPER */
#define MCF_ENET_ATPER_PERIOD(x)             (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_ATCOR */
#define MCF_ENET_ATCOR_COR(x)                (((x)&0x7FFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_ATINC */
#define MCF_ENET_ATINC_INC(x)                (((x)&0x7F)<<0)
#define MCF_ENET_ATINC_INC_CORR(x)           (((x)&0x7F)<<0x8)

/* Bit definitions and macros for MCF_ENET_ATSTMP */
#define MCF_ENET_ATSTMP_TIMESTAMP(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_SMACL */
#define MCF_ENET_SMACL_SMACL(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_ENET_SMACU */
#define MCF_ENET_SMACU_SMACU(x)              (((x)&0xFFFF)<<0x10)


#endif /* __MCF54418_ENET_H__ */
