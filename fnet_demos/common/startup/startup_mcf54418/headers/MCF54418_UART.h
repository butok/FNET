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

#ifndef __MCF54418_UART_H__
#define __MCF54418_UART_H__


/*********************************************************************
*
* Universal Asynchronous Receiver Transmitter (UART)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_UART4_UMR                        (*(vuint8 *)(0xEC060000))
#define MCF_UART4_UMR1                       (*(vuint8 *)(0xEC060000))
#define MCF_UART4_UMR2                       (*(vuint8 *)(0xEC060000))
#define MCF_UART4_USR                        (*(vuint8 *)(0xEC060004))
#define MCF_UART4_UCSR                       (*(vuint8 *)(0xEC060004))
#define MCF_UART4_UCR                        (*(vuint8 *)(0xEC060008))
#define MCF_UART4_URB                        (*(vuint8 *)(0xEC06000C))
#define MCF_UART4_UTB                        (*(vuint8 *)(0xEC06000C))
#define MCF_UART4_UACR                       (*(vuint8 *)(0xEC060010))
#define MCF_UART4_UIPCR                      (*(vuint8 *)(0xEC060010))
#define MCF_UART4_UIMR                       (*(vuint8 *)(0xEC060014))
#define MCF_UART4_UISR                       (*(vuint8 *)(0xEC060014))
#define MCF_UART4_UBG1                       (*(vuint8 *)(0xEC060018))
#define MCF_UART4_UBG2                       (*(vuint8 *)(0xEC06001C))
#define MCF_UART4_UIP                        (*(vuint8 *)(0xEC060034))
#define MCF_UART4_UOP1                       (*(vuint8 *)(0xEC060038))
#define MCF_UART4_UOP0                       (*(vuint8 *)(0xEC06003C))

#define MCF_UART5_UMR                        (*(vuint8 *)(0xEC064000))
#define MCF_UART5_UMR1                       (*(vuint8 *)(0xEC064000))
#define MCF_UART5_UMR2                       (*(vuint8 *)(0xEC064000))
#define MCF_UART5_USR                        (*(vuint8 *)(0xEC064004))
#define MCF_UART5_UCSR                       (*(vuint8 *)(0xEC064004))
#define MCF_UART5_UCR                        (*(vuint8 *)(0xEC064008))
#define MCF_UART5_URB                        (*(vuint8 *)(0xEC06400C))
#define MCF_UART5_UTB                        (*(vuint8 *)(0xEC06400C))
#define MCF_UART5_UACR                       (*(vuint8 *)(0xEC064010))
#define MCF_UART5_UIPCR                      (*(vuint8 *)(0xEC064010))
#define MCF_UART5_UIMR                       (*(vuint8 *)(0xEC064014))
#define MCF_UART5_UISR                       (*(vuint8 *)(0xEC064014))
#define MCF_UART5_UBG1                       (*(vuint8 *)(0xEC064018))
#define MCF_UART5_UBG2                       (*(vuint8 *)(0xEC06401C))
#define MCF_UART5_UIP                        (*(vuint8 *)(0xEC064034))
#define MCF_UART5_UOP1                       (*(vuint8 *)(0xEC064038))
#define MCF_UART5_UOP0                       (*(vuint8 *)(0xEC06403C))

#define MCF_UART6_UMR                        (*(vuint8 *)(0xEC068000))
#define MCF_UART6_UMR1                       (*(vuint8 *)(0xEC068000))
#define MCF_UART6_UMR2                       (*(vuint8 *)(0xEC068000))
#define MCF_UART6_USR                        (*(vuint8 *)(0xEC068004))
#define MCF_UART6_UCSR                       (*(vuint8 *)(0xEC068004))
#define MCF_UART6_UCR                        (*(vuint8 *)(0xEC068008))
#define MCF_UART6_URB                        (*(vuint8 *)(0xEC06800C))
#define MCF_UART6_UTB                        (*(vuint8 *)(0xEC06800C))
#define MCF_UART6_UACR                       (*(vuint8 *)(0xEC068010))
#define MCF_UART6_UIPCR                      (*(vuint8 *)(0xEC068010))
#define MCF_UART6_UIMR                       (*(vuint8 *)(0xEC068014))
#define MCF_UART6_UISR                       (*(vuint8 *)(0xEC068014))
#define MCF_UART6_UBG1                       (*(vuint8 *)(0xEC068018))
#define MCF_UART6_UBG2                       (*(vuint8 *)(0xEC06801C))
#define MCF_UART6_UIP                        (*(vuint8 *)(0xEC068034))
#define MCF_UART6_UOP1                       (*(vuint8 *)(0xEC068038))
#define MCF_UART6_UOP0                       (*(vuint8 *)(0xEC06803C))

#define MCF_UART7_UMR                        (*(vuint8 *)(0xEC06C000))
#define MCF_UART7_UMR1                       (*(vuint8 *)(0xEC06C000))
#define MCF_UART7_UMR2                       (*(vuint8 *)(0xEC06C000))
#define MCF_UART7_USR                        (*(vuint8 *)(0xEC06C004))
#define MCF_UART7_UCSR                       (*(vuint8 *)(0xEC06C004))
#define MCF_UART7_UCR                        (*(vuint8 *)(0xEC06C008))
#define MCF_UART7_URB                        (*(vuint8 *)(0xEC06C00C))
#define MCF_UART7_UTB                        (*(vuint8 *)(0xEC06C00C))
#define MCF_UART7_UACR                       (*(vuint8 *)(0xEC06C010))
#define MCF_UART7_UIPCR                      (*(vuint8 *)(0xEC06C010))
#define MCF_UART7_UIMR                       (*(vuint8 *)(0xEC06C014))
#define MCF_UART7_UISR                       (*(vuint8 *)(0xEC06C014))
#define MCF_UART7_UBG1                       (*(vuint8 *)(0xEC06C018))
#define MCF_UART7_UBG2                       (*(vuint8 *)(0xEC06C01C))
#define MCF_UART7_UIP                        (*(vuint8 *)(0xEC06C034))
#define MCF_UART7_UOP1                       (*(vuint8 *)(0xEC06C038))
#define MCF_UART7_UOP0                       (*(vuint8 *)(0xEC06C03C))

#define MCF_UART8_UMR                        (*(vuint8 *)(0xEC070000))
#define MCF_UART8_UMR1                       (*(vuint8 *)(0xEC070000))
#define MCF_UART8_UMR2                       (*(vuint8 *)(0xEC070000))
#define MCF_UART8_USR                        (*(vuint8 *)(0xEC070004))
#define MCF_UART8_UCSR                       (*(vuint8 *)(0xEC070004))
#define MCF_UART8_UCR                        (*(vuint8 *)(0xEC070008))
#define MCF_UART8_URB                        (*(vuint8 *)(0xEC07000C))
#define MCF_UART8_UTB                        (*(vuint8 *)(0xEC07000C))
#define MCF_UART8_UACR                       (*(vuint8 *)(0xEC070010))
#define MCF_UART8_UIPCR                      (*(vuint8 *)(0xEC070010))
#define MCF_UART8_UIMR                       (*(vuint8 *)(0xEC070014))
#define MCF_UART8_UISR                       (*(vuint8 *)(0xEC070014))
#define MCF_UART8_UBG1                       (*(vuint8 *)(0xEC070018))
#define MCF_UART8_UBG2                       (*(vuint8 *)(0xEC07001C))
#define MCF_UART8_UIP                        (*(vuint8 *)(0xEC070034))
#define MCF_UART8_UOP1                       (*(vuint8 *)(0xEC070038))
#define MCF_UART8_UOP0                       (*(vuint8 *)(0xEC07003C))

#define MCF_UART9_UMR                        (*(vuint8 *)(0xEC074000))
#define MCF_UART9_UMR1                       (*(vuint8 *)(0xEC074000))
#define MCF_UART9_UMR2                       (*(vuint8 *)(0xEC074000))
#define MCF_UART9_USR                        (*(vuint8 *)(0xEC074004))
#define MCF_UART9_UCSR                       (*(vuint8 *)(0xEC074004))
#define MCF_UART9_UCR                        (*(vuint8 *)(0xEC074008))
#define MCF_UART9_URB                        (*(vuint8 *)(0xEC07400C))
#define MCF_UART9_UTB                        (*(vuint8 *)(0xEC07400C))
#define MCF_UART9_UACR                       (*(vuint8 *)(0xEC074010))
#define MCF_UART9_UIPCR                      (*(vuint8 *)(0xEC074010))
#define MCF_UART9_UIMR                       (*(vuint8 *)(0xEC074014))
#define MCF_UART9_UISR                       (*(vuint8 *)(0xEC074014))
#define MCF_UART9_UBG1                       (*(vuint8 *)(0xEC074018))
#define MCF_UART9_UBG2                       (*(vuint8 *)(0xEC07401C))
#define MCF_UART9_UIP                        (*(vuint8 *)(0xEC074034))
#define MCF_UART9_UOP1                       (*(vuint8 *)(0xEC074038))
#define MCF_UART9_UOP0                       (*(vuint8 *)(0xEC07403C))

#define MCF_UART0_UMR                        (*(vuint8 *)(0xFC060000))
#define MCF_UART0_UMR1                       (*(vuint8 *)(0xFC060000))
#define MCF_UART0_UMR2                       (*(vuint8 *)(0xFC060000))
#define MCF_UART0_USR                        (*(vuint8 *)(0xFC060004))
#define MCF_UART0_UCSR                       (*(vuint8 *)(0xFC060004))
#define MCF_UART0_UCR                        (*(vuint8 *)(0xFC060008))
#define MCF_UART0_URB                        (*(vuint8 *)(0xFC06000C))
#define MCF_UART0_UTB                        (*(vuint8 *)(0xFC06000C))
#define MCF_UART0_UACR                       (*(vuint8 *)(0xFC060010))
#define MCF_UART0_UIPCR                      (*(vuint8 *)(0xFC060010))
#define MCF_UART0_UIMR                       (*(vuint8 *)(0xFC060014))
#define MCF_UART0_UISR                       (*(vuint8 *)(0xFC060014))
#define MCF_UART0_UBG1                       (*(vuint8 *)(0xFC060018))
#define MCF_UART0_UBG2                       (*(vuint8 *)(0xFC06001C))
#define MCF_UART0_UIP                        (*(vuint8 *)(0xFC060034))
#define MCF_UART0_UOP1                       (*(vuint8 *)(0xFC060038))
#define MCF_UART0_UOP0                       (*(vuint8 *)(0xFC06003C))

#define MCF_UART1_UMR                        (*(vuint8 *)(0xFC064000))
#define MCF_UART1_UMR1                       (*(vuint8 *)(0xFC064000))
#define MCF_UART1_UMR2                       (*(vuint8 *)(0xFC064000))
#define MCF_UART1_USR                        (*(vuint8 *)(0xFC064004))
#define MCF_UART1_UCSR                       (*(vuint8 *)(0xFC064004))
#define MCF_UART1_UCR                        (*(vuint8 *)(0xFC064008))
#define MCF_UART1_URB                        (*(vuint8 *)(0xFC06400C))
#define MCF_UART1_UTB                        (*(vuint8 *)(0xFC06400C))
#define MCF_UART1_UACR                       (*(vuint8 *)(0xFC064010))
#define MCF_UART1_UIPCR                      (*(vuint8 *)(0xFC064010))
#define MCF_UART1_UIMR                       (*(vuint8 *)(0xFC064014))
#define MCF_UART1_UISR                       (*(vuint8 *)(0xFC064014))
#define MCF_UART1_UBG1                       (*(vuint8 *)(0xFC064018))
#define MCF_UART1_UBG2                       (*(vuint8 *)(0xFC06401C))
#define MCF_UART1_UIP                        (*(vuint8 *)(0xFC064034))
#define MCF_UART1_UOP1                       (*(vuint8 *)(0xFC064038))
#define MCF_UART1_UOP0                       (*(vuint8 *)(0xFC06403C))

#define MCF_UART2_UMR                        (*(vuint8 *)(0xFC068000))
#define MCF_UART2_UMR1                       (*(vuint8 *)(0xFC068000))
#define MCF_UART2_UMR2                       (*(vuint8 *)(0xFC068000))
#define MCF_UART2_USR                        (*(vuint8 *)(0xFC068004))
#define MCF_UART2_UCSR                       (*(vuint8 *)(0xFC068004))
#define MCF_UART2_UCR                        (*(vuint8 *)(0xFC068008))
#define MCF_UART2_URB                        (*(vuint8 *)(0xFC06800C))
#define MCF_UART2_UTB                        (*(vuint8 *)(0xFC06800C))
#define MCF_UART2_UACR                       (*(vuint8 *)(0xFC068010))
#define MCF_UART2_UIPCR                      (*(vuint8 *)(0xFC068010))
#define MCF_UART2_UIMR                       (*(vuint8 *)(0xFC068014))
#define MCF_UART2_UISR                       (*(vuint8 *)(0xFC068014))
#define MCF_UART2_UBG1                       (*(vuint8 *)(0xFC068018))
#define MCF_UART2_UBG2                       (*(vuint8 *)(0xFC06801C))
#define MCF_UART2_UIP                        (*(vuint8 *)(0xFC068034))
#define MCF_UART2_UOP1                       (*(vuint8 *)(0xFC068038))
#define MCF_UART2_UOP0                       (*(vuint8 *)(0xFC06803C))

#define MCF_UART3_UMR                        (*(vuint8 *)(0xFC06C000))
#define MCF_UART3_UMR1                       (*(vuint8 *)(0xFC06C000))
#define MCF_UART3_UMR2                       (*(vuint8 *)(0xFC06C000))
#define MCF_UART3_USR                        (*(vuint8 *)(0xFC06C004))
#define MCF_UART3_UCSR                       (*(vuint8 *)(0xFC06C004))
#define MCF_UART3_UCR                        (*(vuint8 *)(0xFC06C008))
#define MCF_UART3_URB                        (*(vuint8 *)(0xFC06C00C))
#define MCF_UART3_UTB                        (*(vuint8 *)(0xFC06C00C))
#define MCF_UART3_UACR                       (*(vuint8 *)(0xFC06C010))
#define MCF_UART3_UIPCR                      (*(vuint8 *)(0xFC06C010))
#define MCF_UART3_UIMR                       (*(vuint8 *)(0xFC06C014))
#define MCF_UART3_UISR                       (*(vuint8 *)(0xFC06C014))
#define MCF_UART3_UBG1                       (*(vuint8 *)(0xFC06C018))
#define MCF_UART3_UBG2                       (*(vuint8 *)(0xFC06C01C))
#define MCF_UART3_UIP                        (*(vuint8 *)(0xFC06C034))
#define MCF_UART3_UOP1                       (*(vuint8 *)(0xFC06C038))
#define MCF_UART3_UOP0                       (*(vuint8 *)(0xFC06C03C))


/* Other macros */
#define MCF_UART_UMR(x)                      (*(vuint8 *)(0xEC060000 + ((x<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
#define MCF_UART_USR(x)                      (*(vuint8 *)(0xEC060004 + ((x<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
#define MCF_UART_UCSR(x)                     (*(vuint8 *)(0xEC060004 + ((x<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
#define MCF_UART_UCR(x)                      (*(vuint8 *)(0xEC060008 + ((x<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
#define MCF_UART_URB(x)                      (*(vuint8 *)(0xEC06000C + ((x<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
#define MCF_UART_UTB(x)                      (*(vuint8 *)(0xEC06000C + ((x<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
#define MCF_UART_UACR(x)                     (*(vuint8 *)(0xEC060010 + ((x<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
#define MCF_UART_UIPCR(x)                    (*(vuint8 *)(0xEC060010 + ((x<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
#define MCF_UART_UIMR(x)                     (*(vuint8 *)(0xEC060014 + ((x<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
#define MCF_UART_UISR(x)                     (*(vuint8 *)(0xEC060014 + ((x<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
#define MCF_UART_UBG1(x)                     (*(vuint8 *)(0xEC060018 + ((x<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
#define MCF_UART_UBG2(x)                     (*(vuint8 *)(0xEC06001C + ((x<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
#define MCF_UART_UIP(x)                      (*(vuint8 *)(0xEC060034 + ((x<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
#define MCF_UART_UOP1(x)                     (*(vuint8 *)(0xEC060038 + ((x<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))
#define MCF_UART_UOP0(x)                     (*(vuint8 *)(0xEC06003C + ((x<4)? (0x10000000 + ((x)*0x4000)) : ((x-4)*0x4000))))


/* Bit definitions and macros for MCF_UART_UMR */
#define MCF_UART_UMR_BC(x)                   (((x)&0x3)<<0)
#define MCF_UART_UMR_BC_5                    (0)
#define MCF_UART_UMR_BC_6                    (0x1)
#define MCF_UART_UMR_BC_7                    (0x2)
#define MCF_UART_UMR_BC_8                    (0x3)
#define MCF_UART_UMR_PT                      (0x4)
#define MCF_UART_UMR_PM(x)                   (((x)&0x3)<<0x3)
#define MCF_UART_UMR_ERR                     (0x20)
#define MCF_UART_UMR_RXIRQ                   (0x40)
#define MCF_UART_UMR_RXRTS                   (0x80)
#define MCF_UART_UMR_PM_MULTI_ADDR           (0x1C)
#define MCF_UART_UMR_PM_MULTI_DATA           (0x18)
#define MCF_UART_UMR_PM_NONE                 (0x10)
#define MCF_UART_UMR_PM_FORCE_HI             (0xC)
#define MCF_UART_UMR_PM_FORCE_LO             (0x8)
#define MCF_UART_UMR_PM_ODD                  (0x4)
#define MCF_UART_UMR_PM_EVEN                 (0)
#define MCF_UART_UMR_SB(x)                   (((x)&0xF)<<0)
#define MCF_UART_UMR_SB_STOP_BITS_1          (0x7)
#define MCF_UART_UMR_SB_STOP_BITS_15         (0x8)
#define MCF_UART_UMR_SB_STOP_BITS_2          (0xF)
#define MCF_UART_UMR_TXCTS                   (0x10)
#define MCF_UART_UMR_TXRTS                   (0x20)
#define MCF_UART_UMR_CM(x)                   (((x)&0x3)<<0x6)
#define MCF_UART_UMR_CM_NORMAL               (0)
#define MCF_UART_UMR_CM_ECHO                 (0x40)
#define MCF_UART_UMR_CM_LOCAL_LOOP           (0x80)
#define MCF_UART_UMR_CM_REMOTE_LOOP          (0xC0)

/* Bit definitions and macros for MCF_UART_USR */
#define MCF_UART_USR_RXRDY                   (0x1)
#define MCF_UART_USR_FFULL                   (0x2)
#define MCF_UART_USR_TXRDY                   (0x4)
#define MCF_UART_USR_TXEMP                   (0x8)
#define MCF_UART_USR_OE                      (0x10)
#define MCF_UART_USR_PE                      (0x20)
#define MCF_UART_USR_FE                      (0x40)
#define MCF_UART_USR_RB                      (0x80)

/* Bit definitions and macros for MCF_UART_UCSR */
#define MCF_UART_UCSR_TCS(x)                 (((x)&0xF)<<0)
#define MCF_UART_UCSR_TCS_SYS_CLK            (0xD)
#define MCF_UART_UCSR_TCS_CTM16              (0xE)
#define MCF_UART_UCSR_TCS_CTM                (0xF)
#define MCF_UART_UCSR_RCS(x)                 (((x)&0xF)<<0x4)
#define MCF_UART_UCSR_RCS_SYS_CLK            (0xD0)
#define MCF_UART_UCSR_RCS_CTM16              (0xE0)
#define MCF_UART_UCSR_RCS_CTM                (0xF0)

/* Bit definitions and macros for MCF_UART_UCR */
#define MCF_UART_UCR_RC(x)                   (((x)&0x3)<<0)
#define MCF_UART_UCR_RX_ENABLED              (0x1)
#define MCF_UART_UCR_RX_DISABLED             (0x2)
#define MCF_UART_UCR_TC(x)                   (((x)&0x3)<<0x2)
#define MCF_UART_UCR_TX_ENABLED              (0x4)
#define MCF_UART_UCR_TX_DISABLED             (0x8)
#define MCF_UART_UCR_MISC(x)                 (((x)&0x7)<<0x4)
#define MCF_UART_UCR_NONE                    (0)
#define MCF_UART_UCR_RESET_MR                (0x10)
#define MCF_UART_UCR_RESET_RX                (0x20)
#define MCF_UART_UCR_RESET_TX                (0x30)
#define MCF_UART_UCR_RESET_ERROR             (0x40)
#define MCF_UART_UCR_RESET_BKCHGINT          (0x50)
#define MCF_UART_UCR_START_BREAK             (0x60)
#define MCF_UART_UCR_STOP_BREAK              (0x70)

/* Bit definitions and macros for MCF_UART_URB */
#define MCF_UART_URB_RB(x)                   (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_UART_UTB */
#define MCF_UART_UTB_TB(x)                   (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_UART_UACR */
#define MCF_UART_UACR_IEC                    (0x1)

/* Bit definitions and macros for MCF_UART_UIPCR */
#define MCF_UART_UIPCR_CTS                   (0x1)
#define MCF_UART_UIPCR_COS                   (0x10)

/* Bit definitions and macros for MCF_UART_UIMR */
#define MCF_UART_UIMR_TXRDY                  (0x1)
#define MCF_UART_UIMR_FFULL_RXRDY            (0x2)
#define MCF_UART_UIMR_DB                     (0x4)
#define MCF_UART_UIMR_COS                    (0x80)

/* Bit definitions and macros for MCF_UART_UISR */
#define MCF_UART_UISR_TXRDY                  (0x1)
#define MCF_UART_UISR_FFULL_RXRDY            (0x2)
#define MCF_UART_UISR_DB                     (0x4)
#define MCF_UART_UISR_COS                    (0x80)

/* Bit definitions and macros for MCF_UART_UBG1 */
#define MCF_UART_UBG1_Divider_MSB(x)         (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_UART_UBG2 */
#define MCF_UART_UBG2_Divider_LSB(x)         (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_UART_UIP */
#define MCF_UART_UIP_CTS                     (0x1)

/* Bit definitions and macros for MCF_UART_UOP1 */
#define MCF_UART_UOP1_RTS                    (0x1)

/* Bit definitions and macros for MCF_UART_UOP0 */
#define MCF_UART_UOP0_RTS                    (0x1)


#endif /* __MCF54418_UART_H__ */
