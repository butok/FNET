/* Coldfire C Header File
 * Copyright Freescale Semiconductor Inc
 * All rights reserved.
 *
 * 2008/05/23 Revision: 0.5
 *
 * (c) Copyright UNIS, a.s. 1997-2008
 * UNIS, a.s.
 * Jundrovska 33
 * 624 00 Brno
 * Czech Republic
 * http      : www.processorexpert.com
 * mail      : info@processorexpert.com
 */

#ifndef __MCF52259_CANMB_H__
#define __MCF52259_CANMB_H__


/*********************************************************************
*
* Flex Controller Area Network Module (FlexCAN) message buffers
*
*********************************************************************/

/* Register read/write macros */
#define MCF_CANMB_CODE0                      (*(vuint8 *)(0x40170080))
#define MCF_CANMB_CTRL0                      (*(vuint8 *)(0x40170081))
#define MCF_CANMB_TIME0                      (*(vuint16*)(0x40170082))
#define MCF_CANMB_ID0                        (*(vuint32*)(0x40170084))
#define MCF_CANMB_DATA_WORD_1_0              (*(vuint16*)(0x40170088))
#define MCF_CANMB_DATA_WORD_2_0              (*(vuint16*)(0x4017008A))
#define MCF_CANMB_DATA_WORD_3_0              (*(vuint16*)(0x4017008C))
#define MCF_CANMB_DATA_WORD_4_0              (*(vuint16*)(0x4017008E))
#define MCF_CANMB_CODE1                      (*(vuint8 *)(0x40170090))
#define MCF_CANMB_CTRL1                      (*(vuint8 *)(0x40170091))
#define MCF_CANMB_TIME1                      (*(vuint16*)(0x40170092))
#define MCF_CANMB_ID1                        (*(vuint32*)(0x40170094))
#define MCF_CANMB_DATA_WORD_1_1              (*(vuint16*)(0x40170098))
#define MCF_CANMB_DATA_WORD_2_1              (*(vuint16*)(0x4017009A))
#define MCF_CANMB_DATA_WORD_3_1              (*(vuint16*)(0x4017009C))
#define MCF_CANMB_DATA_WORD_4_1              (*(vuint16*)(0x4017009E))
#define MCF_CANMB_CODE2                      (*(vuint8 *)(0x401700A0))
#define MCF_CANMB_CTRL2                      (*(vuint8 *)(0x401700A1))
#define MCF_CANMB_TIME2                      (*(vuint16*)(0x401700A2))
#define MCF_CANMB_ID2                        (*(vuint32*)(0x401700A4))
#define MCF_CANMB_DATA_WORD_1_2              (*(vuint16*)(0x401700A8))
#define MCF_CANMB_DATA_WORD_2_2              (*(vuint16*)(0x401700AA))
#define MCF_CANMB_DATA_WORD_3_2              (*(vuint16*)(0x401700AC))
#define MCF_CANMB_DATA_WORD_4_2              (*(vuint16*)(0x401700AE))
#define MCF_CANMB_CODE3                      (*(vuint8 *)(0x401700B0))
#define MCF_CANMB_CTRL3                      (*(vuint8 *)(0x401700B1))
#define MCF_CANMB_TIME3                      (*(vuint16*)(0x401700B2))
#define MCF_CANMB_ID3                        (*(vuint32*)(0x401700B4))
#define MCF_CANMB_DATA_WORD_1_3              (*(vuint16*)(0x401700B8))
#define MCF_CANMB_DATA_WORD_2_3              (*(vuint16*)(0x401700BA))
#define MCF_CANMB_DATA_WORD_3_3              (*(vuint16*)(0x401700BC))
#define MCF_CANMB_DATA_WORD_4_3              (*(vuint16*)(0x401700BE))
#define MCF_CANMB_CODE4                      (*(vuint8 *)(0x401700C0))
#define MCF_CANMB_CTRL4                      (*(vuint8 *)(0x401700C1))
#define MCF_CANMB_TIME4                      (*(vuint16*)(0x401700C2))
#define MCF_CANMB_ID4                        (*(vuint32*)(0x401700C4))
#define MCF_CANMB_DATA_WORD_1_4              (*(vuint16*)(0x401700C8))
#define MCF_CANMB_DATA_WORD_2_4              (*(vuint16*)(0x401700CA))
#define MCF_CANMB_DATA_WORD_3_4              (*(vuint16*)(0x401700CC))
#define MCF_CANMB_DATA_WORD_4_4              (*(vuint16*)(0x401700CE))
#define MCF_CANMB_CODE5                      (*(vuint8 *)(0x401700D0))
#define MCF_CANMB_CTRL5                      (*(vuint8 *)(0x401700D1))
#define MCF_CANMB_TIME5                      (*(vuint16*)(0x401700D2))
#define MCF_CANMB_ID5                        (*(vuint32*)(0x401700D4))
#define MCF_CANMB_DATA_WORD_1_5              (*(vuint16*)(0x401700D8))
#define MCF_CANMB_DATA_WORD_2_5              (*(vuint16*)(0x401700DA))
#define MCF_CANMB_DATA_WORD_3_5              (*(vuint16*)(0x401700DC))
#define MCF_CANMB_DATA_WORD_4_5              (*(vuint16*)(0x401700DE))
#define MCF_CANMB_CODE6                      (*(vuint8 *)(0x401700E0))
#define MCF_CANMB_CTRL6                      (*(vuint8 *)(0x401700E1))
#define MCF_CANMB_TIME6                      (*(vuint16*)(0x401700E2))
#define MCF_CANMB_ID6                        (*(vuint32*)(0x401700E4))
#define MCF_CANMB_DATA_WORD_1_6              (*(vuint16*)(0x401700E8))
#define MCF_CANMB_DATA_WORD_2_6              (*(vuint16*)(0x401700EA))
#define MCF_CANMB_DATA_WORD_3_6              (*(vuint16*)(0x401700EC))
#define MCF_CANMB_DATA_WORD_4_6              (*(vuint16*)(0x401700EE))
#define MCF_CANMB_CODE7                      (*(vuint8 *)(0x401700F0))
#define MCF_CANMB_CTRL7                      (*(vuint8 *)(0x401700F1))
#define MCF_CANMB_TIME7                      (*(vuint16*)(0x401700F2))
#define MCF_CANMB_ID7                        (*(vuint32*)(0x401700F4))
#define MCF_CANMB_DATA_WORD_1_7              (*(vuint16*)(0x401700F8))
#define MCF_CANMB_DATA_WORD_2_7              (*(vuint16*)(0x401700FA))
#define MCF_CANMB_DATA_WORD_3_7              (*(vuint16*)(0x401700FC))
#define MCF_CANMB_DATA_WORD_4_7              (*(vuint16*)(0x401700FE))
#define MCF_CANMB_CODE8                      (*(vuint8 *)(0x40170100))
#define MCF_CANMB_CTRL8                      (*(vuint8 *)(0x40170101))
#define MCF_CANMB_TIME8                      (*(vuint16*)(0x40170102))
#define MCF_CANMB_ID8                        (*(vuint32*)(0x40170104))
#define MCF_CANMB_DATA_WORD_1_8              (*(vuint16*)(0x40170108))
#define MCF_CANMB_DATA_WORD_2_8              (*(vuint16*)(0x4017010A))
#define MCF_CANMB_DATA_WORD_3_8              (*(vuint16*)(0x4017010C))
#define MCF_CANMB_DATA_WORD_4_8              (*(vuint16*)(0x4017010E))
#define MCF_CANMB_CODE9                      (*(vuint8 *)(0x40170110))
#define MCF_CANMB_CTRL9                      (*(vuint8 *)(0x40170111))
#define MCF_CANMB_TIME9                      (*(vuint16*)(0x40170112))
#define MCF_CANMB_ID9                        (*(vuint32*)(0x40170114))
#define MCF_CANMB_DATA_WORD_1_9              (*(vuint16*)(0x40170118))
#define MCF_CANMB_DATA_WORD_2_9              (*(vuint16*)(0x4017011A))
#define MCF_CANMB_DATA_WORD_3_9              (*(vuint16*)(0x4017011C))
#define MCF_CANMB_DATA_WORD_4_9              (*(vuint16*)(0x4017011E))
#define MCF_CANMB_CODE10                     (*(vuint8 *)(0x40170120))
#define MCF_CANMB_CTRL10                     (*(vuint8 *)(0x40170121))
#define MCF_CANMB_TIME10                     (*(vuint16*)(0x40170122))
#define MCF_CANMB_ID10                       (*(vuint32*)(0x40170124))
#define MCF_CANMB_DATA_WORD_1_10             (*(vuint16*)(0x40170128))
#define MCF_CANMB_DATA_WORD_2_10             (*(vuint16*)(0x4017012A))
#define MCF_CANMB_DATA_WORD_3_10             (*(vuint16*)(0x4017012C))
#define MCF_CANMB_DATA_WORD_4_10             (*(vuint16*)(0x4017012E))
#define MCF_CANMB_CODE11                     (*(vuint8 *)(0x40170130))
#define MCF_CANMB_CTRL11                     (*(vuint8 *)(0x40170131))
#define MCF_CANMB_TIME11                     (*(vuint16*)(0x40170132))
#define MCF_CANMB_ID11                       (*(vuint32*)(0x40170134))
#define MCF_CANMB_DATA_WORD_1_11             (*(vuint16*)(0x40170138))
#define MCF_CANMB_DATA_WORD_2_11             (*(vuint16*)(0x4017013A))
#define MCF_CANMB_DATA_WORD_3_11             (*(vuint16*)(0x4017013C))
#define MCF_CANMB_DATA_WORD_4_11             (*(vuint16*)(0x4017013E))
#define MCF_CANMB_CODE12                     (*(vuint8 *)(0x40170140))
#define MCF_CANMB_CTRL12                     (*(vuint8 *)(0x40170141))
#define MCF_CANMB_TIME12                     (*(vuint16*)(0x40170142))
#define MCF_CANMB_ID12                       (*(vuint32*)(0x40170144))
#define MCF_CANMB_DATA_WORD_1_               (*(vuint16*)(0x40170148))
#define MCF_CANMB_DATA_WORD_2_12             (*(vuint16*)(0x4017014A))
#define MCF_CANMB_DATA_WORD_3_12             (*(vuint16*)(0x4017014C))
#define MCF_CANMB_DATA_WORD_4_12             (*(vuint16*)(0x4017014E))
#define MCF_CANMB_CODE13                     (*(vuint8 *)(0x40170150))
#define MCF_CANMB_CTRL13                     (*(vuint8 *)(0x40170151))
#define MCF_CANMB_TIME13                     (*(vuint16*)(0x40170152))
#define MCF_CANMB_ID13                       (*(vuint32*)(0x40170154))
#define MCF_CANMB_DATA_WORD_1_13             (*(vuint16*)(0x40170158))
#define MCF_CANMB_DATA_WORD_2_13             (*(vuint16*)(0x4017015A))
#define MCF_CANMB_DATA_WORD_3_13             (*(vuint16*)(0x4017015C))
#define MCF_CANMB_DATA_WORD_4_13             (*(vuint16*)(0x4017015E))
#define MCF_CANMB_CODE14                     (*(vuint8 *)(0x40170160))
#define MCF_CANMB_CTRL14                     (*(vuint8 *)(0x40170161))
#define MCF_CANMB_TIME14                     (*(vuint16*)(0x40170162))
#define MCF_CANMB_ID14                       (*(vuint32*)(0x40170164))
#define MCF_CANMB_DATA_WORD_1_14             (*(vuint16*)(0x40170168))
#define MCF_CANMB_DATA_WORD_2_14             (*(vuint16*)(0x4017016A))
#define MCF_CANMB_DATA_WORD_3_14             (*(vuint16*)(0x4017016C))
#define MCF_CANMB_DATA_WORD_4_14             (*(vuint16*)(0x4017016E))
#define MCF_CANMB_CODE15                     (*(vuint8 *)(0x40170170))
#define MCF_CANMB_CTRL15                     (*(vuint8 *)(0x40170171))
#define MCF_CANMB_TIME15                     (*(vuint16*)(0x40170172))
#define MCF_CANMB_ID15                       (*(vuint32*)(0x40170174))
#define MCF_CANMB_DATA_WORD_1_15             (*(vuint16*)(0x40170178))
#define MCF_CANMB_DATA_WORD_2_15             (*(vuint16*)(0x4017017A))
#define MCF_CANMB_DATA_WORD_3_15             (*(vuint16*)(0x4017017C))
#define MCF_CANMB_DATA_WORD_4_15             (*(vuint16*)(0x4017017E))
#define MCF_CANMB_CODE(x)                    (*(vuint8 *)(0x40170080 + ((x)*0x10)))
#define MCF_CANMB_CTRL(x)                    (*(vuint8 *)(0x40170081 + ((x)*0x10)))
#define MCF_CANMB_TIME(x)                    (*(vuint16*)(0x40170082 + ((x)*0x10)))
#define MCF_CANMB_ID(x)                      (*(vuint32*)(0x40170084 + ((x)*0x10)))
#define MCF_CANMB_DATA_WORD_1(x)             (*(vuint16*)(0x40170088 + ((x)*0x10)))
#define MCF_CANMB_DATA_WORD_2(x)             (*(vuint16*)(0x4017008A + ((x)*0x10)))
#define MCF_CANMB_DATA_WORD_3(x)             (*(vuint16*)(0x4017008C + ((x)*0x10)))
#define MCF_CANMB_DATA_WORD_4(x)             (*(vuint16*)(0x4017008E + ((x)*0x10)))


/* Other macros */
#define MCF_CANMB_BYTE(x,y)                  (*(vuint8 *)(0x40170088 + ((x)*0x10)+y))


/* Bit definitions and macros for MCF_CANMB_CODE */
#define MCF_CANMB_CODE_CODE(x)               (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_CANMB_CTRL */
#define MCF_CANMB_CTRL_LENGTH(x)             (((x)&0xF)<<0)
#define MCF_CANMB_CTRL_RTR                   (0x10)
#define MCF_CANMB_CTRL_IDE                   (0x20)
#define MCF_CANMB_CTRL_SRR                   (0x40)

/* Bit definitions and macros for MCF_CANMB_TIME */
#define MCF_CANMB_TIME_TIME_STAMP(x)         (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_CANMB_ID */
#define MCF_CANMB_ID_EXT(x)                  (((x)&0x3FFFF)<<0)
#define MCF_CANMB_ID_STD(x)                  (((x)&0x7FF)<<0x12)

/* Bit definitions and macros for MCF_CANMB_DATA_WORD_1 */
#define MCF_CANMB_DATA_WORD_1_DATA_BYTE_1(x) (((x)&0xFF)<<0)
#define MCF_CANMB_DATA_WORD_1_DATA_BYTE_0(x) (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_CANMB_DATA_WORD_2 */
#define MCF_CANMB_DATA_WORD_2_DATA_BYTE_3(x) (((x)&0xFF)<<0)
#define MCF_CANMB_DATA_WORD_2_DATA_BYTE_2(x) (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_CANMB_DATA_WORD_3 */
#define MCF_CANMB_DATA_WORD_3_DATA_BYTE_5(x) (((x)&0xFF)<<0)
#define MCF_CANMB_DATA_WORD_3_DATA_BYTE_4(x) (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_CANMB_DATA_WORD_4 */
#define MCF_CANMB_DATA_WORD_4_DATA_BYTE_7(x) (((x)&0xFF)<<0)
#define MCF_CANMB_DATA_WORD_4_DATA_BYTE_6(x) (((x)&0xFF)<<0x8)


#endif /* __MCF52259_CANMB_H__ */
