; ---------------------------------------------------------------------------------------
;  @file:    startup_MK60D10.s
;  @purpose: CMSIS Cortex-M4 Core Device Startup File
;            MK60D10
;  @version: 1.8
;  @date:    2014-10-14
;  @build:   b151210
; ---------------------------------------------------------------------------------------
;
; Copyright (c) 1997 - 2015 , Freescale Semiconductor, Inc.
; All rights reserved.
;
; Redistribution and use in source and binary forms, with or without modification,
; are permitted provided that the following conditions are met:
;
; o Redistributions of source code must retain the above copyright notice, this list
;   of conditions and the following disclaimer.
;
; o Redistributions in binary form must reproduce the above copyright notice, this
;   list of conditions and the following disclaimer in the documentation and/or
;   other materials provided with the distribution.
;
; o Neither the name of Freescale Semiconductor, Inc. nor the names of its
;   contributors may be used to endorse or promote products derived from this
;   software without specific prior written permission.
;
; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
; ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
; WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
; DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
; ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
; (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
; ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
; (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
; SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler                                   ;NMI Handler
        DCD     HardFault_Handler                             ;Hard Fault Handler
        DCD     MemManage_Handler                             ;MPU Fault Handler
        DCD     BusFault_Handler                              ;Bus Fault Handler
        DCD     UsageFault_Handler                            ;Usage Fault Handler
__vector_table_0x1c
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     SVC_Handler                                   ;SVCall Handler
        DCD     DebugMon_Handler                              ;Debug Monitor Handler
        DCD     0                                             ;Reserved
        DCD     PendSV_Handler                                ;PendSV Handler
        DCD     SysTick_Handler                               ;SysTick Handler

                                                              ;External Interrupts
        DCD     DMA0_IRQHandler                               ;DMA channel 0 transfer complete
        DCD     DMA1_IRQHandler                               ;DMA channel 1 transfer complete
        DCD     DMA2_IRQHandler                               ;DMA channel 2 transfer complete
        DCD     DMA3_IRQHandler                               ;DMA channel 3 transfer complete
        DCD     DMA4_IRQHandler                               ;DMA channel 4 transfer complete
        DCD     DMA5_IRQHandler                               ;DMA channel 5 transfer complete
        DCD     DMA6_IRQHandler                               ;DMA channel 6 transfer complete
        DCD     DMA7_IRQHandler                               ;DMA channel 7 transfer complete
        DCD     DMA8_IRQHandler                               ;DMA channel 8 transfer complete
        DCD     DMA9_IRQHandler                               ;DMA channel 9 transfer complete
        DCD     DMA10_IRQHandler                              ;DMA channel 10 transfer complete
        DCD     DMA11_IRQHandler                              ;DMA channel 11 transfer complete
        DCD     DMA12_IRQHandler                              ;DMA channel 12 transfer complete
        DCD     DMA13_IRQHandler                              ;DMA channel 13 transfer complete
        DCD     DMA14_IRQHandler                              ;DMA channel 14 transfer complete
        DCD     DMA15_IRQHandler                              ;DMA channel 15 transfer complete
        DCD     DMA_Error_IRQHandler                          ;DMA channel 0 - 15 error
        DCD     MCM_IRQHandler                                ;MCM normal interrupt
        DCD     FTFL_IRQHandler                               ;FTFL command complete
        DCD     Read_Collision_IRQHandler                     ;FTFL read collision
        DCD     LVD_LVW_IRQHandler                            ;PMC controller low-voltage detect, low-voltage warning
        DCD     LLWU_IRQHandler                               ;Low leakage wakeup
        DCD     WDOG_EWM_IRQHandler                           ;Single interrupt vector for  WDOG and EWM
        DCD     RNG_IRQHandler                                ;Randon number generator
        DCD     I2C0_IRQHandler                               ;Inter-integrated circuit 0
        DCD     I2C1_IRQHandler                               ;Inter-integrated circuit 1
        DCD     SPI0_IRQHandler                               ;Serial peripheral Interface 0
        DCD     SPI1_IRQHandler                               ;Serial peripheral Interface 1
        DCD     SPI2_IRQHandler                               ;Serial peripheral Interface 1
        DCD     CAN0_ORed_Message_buffer_IRQHandler           ;CAN0 ORed message buffers
        DCD     CAN0_Bus_Off_IRQHandler                       ;CAN0 bus off
        DCD     CAN0_Error_IRQHandler                         ;CAN0 error
        DCD     CAN0_Tx_Warning_IRQHandler                    ;CAN0 Tx warning
        DCD     CAN0_Rx_Warning_IRQHandler                    ;CAN0 Rx warning
        DCD     CAN0_Wake_Up_IRQHandler                       ;CAN0 wake up
        DCD     I2S0_Tx_IRQHandler                            ;Integrated interchip sound 0 transmit interrupt
        DCD     I2S0_Rx_IRQHandler                            ;Integrated interchip sound 0 receive interrupt
        DCD     CAN1_ORed_Message_buffer_IRQHandler           ;CAN1 OR'd message buffers interrupt
        DCD     CAN1_Bus_Off_IRQHandler                       ;CAN1 bus off interrupt
        DCD     CAN1_Error_IRQHandler                         ;CAN1 error interrupt
        DCD     CAN1_Tx_Warning_IRQHandler                    ;CAN1 Tx warning interrupt
        DCD     CAN1_Rx_Warning_IRQHandler                    ;CAN1 Rx warning interrupt
        DCD     CAN1_Wake_Up_IRQHandler                       ;CAN1 wake up interrupt
        DCD     Reserved59_IRQHandler                         ;Reserved interrupt
        DCD     UART0_LON_IRQHandler                          ;UART0 LON
        DCD     UART0_RX_TX_IRQHandler                        ;UART0 receive/transmit interrupt
        DCD     UART0_ERR_IRQHandler                          ;UART0 error interrupt
        DCD     UART1_RX_TX_IRQHandler                        ;UART1 receive/transmit interrupt
        DCD     UART1_ERR_IRQHandler                          ;UART1 error interrupt
        DCD     UART2_RX_TX_IRQHandler                        ;UART2 receive/transmit interrupt
        DCD     UART2_ERR_IRQHandler                          ;UART2 error interrupt
        DCD     UART3_RX_TX_IRQHandler                        ;UART3 receive/transmit interrupt
        DCD     UART3_ERR_IRQHandler                          ;UART3 error interrupt
        DCD     UART4_RX_TX_IRQHandler                        ;UART4 receive/transmit interrupt
        DCD     UART4_ERR_IRQHandler                          ;UART4 error interrupt
        DCD     UART5_RX_TX_IRQHandler                        ;UART5 receive/transmit interrupt
        DCD     UART5_ERR_IRQHandler                          ;UART5 error interrupt
        DCD     ADC0_IRQHandler                               ;Analog-to-digital converter 0
        DCD     ADC1_IRQHandler                               ;Analog-to-digital converter 1
        DCD     CMP0_IRQHandler                               ;Comparator 0
        DCD     CMP1_IRQHandler                               ;Comparator 1
        DCD     CMP2_IRQHandler                               ;Comparator 2
        DCD     FTM0_IRQHandler                               ;FlexTimer module 0 fault, overflow and channels interrupt
        DCD     FTM1_IRQHandler                               ;FlexTimer module 1 fault, overflow and channels interrupt
        DCD     FTM2_IRQHandler                               ;FlexTimer module 2 fault, overflow and channels interrupt
        DCD     CMT_IRQHandler                                ;Carrier modulator transmitter
        DCD     RTC_IRQHandler                                ;Real time clock
        DCD     RTC_Seconds_IRQHandler                        ;Real time clock seconds
        DCD     PIT0_IRQHandler                               ;Periodic interrupt timer channel 0
        DCD     PIT1_IRQHandler                               ;Periodic interrupt timer channel 1
        DCD     PIT2_IRQHandler                               ;Periodic interrupt timer channel 2
        DCD     PIT3_IRQHandler                               ;Periodic interrupt timer channel 3
        DCD     PDB0_IRQHandler                               ;Programmable delay block
        DCD     USB0_IRQHandler                               ;USB OTG interrupt
        DCD     USBDCD_IRQHandler                             ;USB charger detect
        DCD     ENET_1588_Timer_IRQHandler                    ;Ethernet MAC IEEE 1588 timer
        DCD     ENET_Transmit_IRQHandler                      ;Ethernet MAC transmit
        DCD     ENET_Receive_IRQHandler                       ;Ethernet MAC receive
        DCD     ENET_Error_IRQHandler                         ;Ethernet MAC error and miscelaneous
        DCD     Reserved95_IRQHandler                         ;Reserved interrupt
        DCD     SDHC_IRQHandler                               ;Secured digital host controller
        DCD     DAC0_IRQHandler                               ;Digital-to-analog converter 0
        DCD     DAC1_IRQHandler                               ;Digital-to-analog converter 1
        DCD     TSI0_IRQHandler                               ;TSI0 Interrupt
        DCD     MCG_IRQHandler                                ;Multipurpose clock generator
        DCD     LPTMR0_IRQHandler                             ;Low power timer interrupt
        DCD     Reserved102_IRQHandler                        ;Reserved interrupt
        DCD     PORTA_IRQHandler                              ;Port A interrupt
        DCD     PORTB_IRQHandler                              ;Port B interrupt
        DCD     PORTC_IRQHandler                              ;Port C interrupt
        DCD     PORTD_IRQHandler                              ;Port D interrupt
        DCD     PORTE_IRQHandler                              ;Port E interrupt
        DCD     Reserved108_IRQHandler                        ;Reserved interrupt
        DCD     Reserved109_IRQHandler                        ;Reserved interrupt
        DCD     SWI_IRQHandler                                ;Software interrupt
        DCD     Reserved111_IRQHandler                        ;Reserved interrupt
        DCD     Reserved112_IRQHandler                        ;Reserved interrupt
        DCD     Reserved113_IRQHandler                        ;Reserved interrupt
        DCD     Reserved114_IRQHandler                        ;Reserved interrupt
        DCD     Reserved115_IRQHandler                        ;Reserved interrupt
        DCD     Reserved116_IRQHandler                        ;Reserved interrupt
        DCD     Reserved117_IRQHandler                        ;Reserved interrupt
        DCD     Reserved118_IRQHandler                        ;Reserved interrupt
        DCD     Reserved119_IRQHandler                        ;Reserved interrupt
        DCD     DefaultISR                                    ;120
        DCD     DefaultISR                                    ;121
        DCD     DefaultISR                                    ;122
        DCD     DefaultISR                                    ;123
        DCD     DefaultISR                                    ;124
        DCD     DefaultISR                                    ;125
        DCD     DefaultISR                                    ;126
        DCD     DefaultISR                                    ;127
        DCD     DefaultISR                                    ;128
        DCD     DefaultISR                                    ;129
        DCD     DefaultISR                                    ;130
        DCD     DefaultISR                                    ;131
        DCD     DefaultISR                                    ;132
        DCD     DefaultISR                                    ;133
        DCD     DefaultISR                                    ;134
        DCD     DefaultISR                                    ;135
        DCD     DefaultISR                                    ;136
        DCD     DefaultISR                                    ;137
        DCD     DefaultISR                                    ;138
        DCD     DefaultISR                                    ;139
        DCD     DefaultISR                                    ;140
        DCD     DefaultISR                                    ;141
        DCD     DefaultISR                                    ;142
        DCD     DefaultISR                                    ;143
        DCD     DefaultISR                                    ;144
        DCD     DefaultISR                                    ;145
        DCD     DefaultISR                                    ;146
        DCD     DefaultISR                                    ;147
        DCD     DefaultISR                                    ;148
        DCD     DefaultISR                                    ;149
        DCD     DefaultISR                                    ;150
        DCD     DefaultISR                                    ;151
        DCD     DefaultISR                                    ;152
        DCD     DefaultISR                                    ;153
        DCD     DefaultISR                                    ;154
        DCD     DefaultISR                                    ;155
        DCD     DefaultISR                                    ;156
        DCD     DefaultISR                                    ;157
        DCD     DefaultISR                                    ;158
        DCD     DefaultISR                                    ;159
        DCD     DefaultISR                                    ;160
        DCD     DefaultISR                                    ;161
        DCD     DefaultISR                                    ;162
        DCD     DefaultISR                                    ;163
        DCD     DefaultISR                                    ;164
        DCD     DefaultISR                                    ;165
        DCD     DefaultISR                                    ;166
        DCD     DefaultISR                                    ;167
        DCD     DefaultISR                                    ;168
        DCD     DefaultISR                                    ;169
        DCD     DefaultISR                                    ;170
        DCD     DefaultISR                                    ;171
        DCD     DefaultISR                                    ;172
        DCD     DefaultISR                                    ;173
        DCD     DefaultISR                                    ;174
        DCD     DefaultISR                                    ;175
        DCD     DefaultISR                                    ;176
        DCD     DefaultISR                                    ;177
        DCD     DefaultISR                                    ;178
        DCD     DefaultISR                                    ;179
        DCD     DefaultISR                                    ;180
        DCD     DefaultISR                                    ;181
        DCD     DefaultISR                                    ;182
        DCD     DefaultISR                                    ;183
        DCD     DefaultISR                                    ;184
        DCD     DefaultISR                                    ;185
        DCD     DefaultISR                                    ;186
        DCD     DefaultISR                                    ;187
        DCD     DefaultISR                                    ;188
        DCD     DefaultISR                                    ;189
        DCD     DefaultISR                                    ;190
        DCD     DefaultISR                                    ;191
        DCD     DefaultISR                                    ;192
        DCD     DefaultISR                                    ;193
        DCD     DefaultISR                                    ;194
        DCD     DefaultISR                                    ;195
        DCD     DefaultISR                                    ;196
        DCD     DefaultISR                                    ;197
        DCD     DefaultISR                                    ;198
        DCD     DefaultISR                                    ;199
        DCD     DefaultISR                                    ;200
        DCD     DefaultISR                                    ;201
        DCD     DefaultISR                                    ;202
        DCD     DefaultISR                                    ;203
        DCD     DefaultISR                                    ;204
        DCD     DefaultISR                                    ;205
        DCD     DefaultISR                                    ;206
        DCD     DefaultISR                                    ;207
        DCD     DefaultISR                                    ;208
        DCD     DefaultISR                                    ;209
        DCD     DefaultISR                                    ;210
        DCD     DefaultISR                                    ;211
        DCD     DefaultISR                                    ;212
        DCD     DefaultISR                                    ;213
        DCD     DefaultISR                                    ;214
        DCD     DefaultISR                                    ;215
        DCD     DefaultISR                                    ;216
        DCD     DefaultISR                                    ;217
        DCD     DefaultISR                                    ;218
        DCD     DefaultISR                                    ;219
        DCD     DefaultISR                                    ;220
        DCD     DefaultISR                                    ;221
        DCD     DefaultISR                                    ;222
        DCD     DefaultISR                                    ;223
        DCD     DefaultISR                                    ;224
        DCD     DefaultISR                                    ;225
        DCD     DefaultISR                                    ;226
        DCD     DefaultISR                                    ;227
        DCD     DefaultISR                                    ;228
        DCD     DefaultISR                                    ;229
        DCD     DefaultISR                                    ;230
        DCD     DefaultISR                                    ;231
        DCD     DefaultISR                                    ;232
        DCD     DefaultISR                                    ;233
        DCD     DefaultISR                                    ;234
        DCD     DefaultISR                                    ;235
        DCD     DefaultISR                                    ;236
        DCD     DefaultISR                                    ;237
        DCD     DefaultISR                                    ;238
        DCD     DefaultISR                                    ;239
        DCD     DefaultISR                                    ;240
        DCD     DefaultISR                                    ;241
        DCD     DefaultISR                                    ;242
        DCD     DefaultISR                                    ;243
        DCD     DefaultISR                                    ;244
        DCD     DefaultISR                                    ;245
        DCD     DefaultISR                                    ;246
        DCD     DefaultISR                                    ;247
        DCD     DefaultISR                                    ;248
        DCD     DefaultISR                                    ;249
        DCD     DefaultISR                                    ;250
        DCD     DefaultISR                                    ;251
        DCD     DefaultISR                                    ;252
        DCD     DefaultISR                                    ;253
        DCD     DefaultISR                                    ;254
        DCD     0xFFFFFFFF                                    ; Reserved for user TRIM value
__Vectors_End

        SECTION FlashConfig:CODE
__FlashConfig
        DCD 0xFFFFFFFF
        DCD 0xFFFFFFFF
        DCD 0xFFFFFFFF
        DCD 0xFFFFFFFE
__FlashConfig_End

__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        CPSID   I               ; Mask interrupts
        LDR     R0, =0xE000ED08
        LDR     R1, =__vector_table
        STR     R1, [R0]
        LDR     R0, =SystemInit
        BLX     R0
        CPSIE   I               ; Unmask interrupts
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B .

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B .

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
        B .

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
        B .

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
        B .

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B .

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
        B .

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B .

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B .

        PUBWEAK DMA0_IRQHandler
        PUBWEAK DMA0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_IRQHandler
        LDR     R0, =DMA0_DriverIRQHandler
        BX      R0

        PUBWEAK DMA1_IRQHandler
        PUBWEAK DMA1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA1_IRQHandler
        LDR     R0, =DMA1_DriverIRQHandler
        BX      R0

        PUBWEAK DMA2_IRQHandler
        PUBWEAK DMA2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA2_IRQHandler
        LDR     R0, =DMA2_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_IRQHandler
        PUBWEAK DMA3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_IRQHandler
        LDR     R0, =DMA3_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_IRQHandler
        PUBWEAK DMA4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_IRQHandler
        LDR     R0, =DMA4_DriverIRQHandler
        BX      R0

        PUBWEAK DMA5_IRQHandler
        PUBWEAK DMA5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA5_IRQHandler
        LDR     R0, =DMA5_DriverIRQHandler
        BX      R0

        PUBWEAK DMA6_IRQHandler
        PUBWEAK DMA6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA6_IRQHandler
        LDR     R0, =DMA6_DriverIRQHandler
        BX      R0

        PUBWEAK DMA7_IRQHandler
        PUBWEAK DMA7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA7_IRQHandler
        LDR     R0, =DMA7_DriverIRQHandler
        BX      R0

        PUBWEAK DMA8_IRQHandler
        PUBWEAK DMA8_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA8_IRQHandler
        LDR     R0, =DMA8_DriverIRQHandler
        BX      R0

        PUBWEAK DMA9_IRQHandler
        PUBWEAK DMA9_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA9_IRQHandler
        LDR     R0, =DMA9_DriverIRQHandler
        BX      R0

        PUBWEAK DMA10_IRQHandler
        PUBWEAK DMA10_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA10_IRQHandler
        LDR     R0, =DMA10_DriverIRQHandler
        BX      R0

        PUBWEAK DMA11_IRQHandler
        PUBWEAK DMA11_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA11_IRQHandler
        LDR     R0, =DMA11_DriverIRQHandler
        BX      R0

        PUBWEAK DMA12_IRQHandler
        PUBWEAK DMA12_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA12_IRQHandler
        LDR     R0, =DMA12_DriverIRQHandler
        BX      R0

        PUBWEAK DMA13_IRQHandler
        PUBWEAK DMA13_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA13_IRQHandler
        LDR     R0, =DMA13_DriverIRQHandler
        BX      R0

        PUBWEAK DMA14_IRQHandler
        PUBWEAK DMA14_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA14_IRQHandler
        LDR     R0, =DMA14_DriverIRQHandler
        BX      R0

        PUBWEAK DMA15_IRQHandler
        PUBWEAK DMA15_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA15_IRQHandler
        LDR     R0, =DMA15_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_Error_IRQHandler
        PUBWEAK DMA_Error_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_Error_IRQHandler
        LDR     R0, =DMA_Error_DriverIRQHandler
        BX      R0

        PUBWEAK MCM_IRQHandler
        PUBWEAK FTFL_IRQHandler
        PUBWEAK Read_Collision_IRQHandler
        PUBWEAK LVD_LVW_IRQHandler
        PUBWEAK LLWU_IRQHandler
        PUBWEAK WDOG_EWM_IRQHandler
        PUBWEAK RNG_IRQHandler
        PUBWEAK I2C0_IRQHandler
        PUBWEAK I2C0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2C0_IRQHandler
        LDR     R0, =I2C0_DriverIRQHandler
        BX      R0

        PUBWEAK I2C1_IRQHandler
        PUBWEAK I2C1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2C1_IRQHandler
        LDR     R0, =I2C1_DriverIRQHandler
        BX      R0

        PUBWEAK SPI0_IRQHandler
        PUBWEAK SPI0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SPI0_IRQHandler
        LDR     R0, =SPI0_DriverIRQHandler
        BX      R0

        PUBWEAK SPI1_IRQHandler
        PUBWEAK SPI1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SPI1_IRQHandler
        LDR     R0, =SPI1_DriverIRQHandler
        BX      R0

        PUBWEAK SPI2_IRQHandler
        PUBWEAK SPI2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SPI2_IRQHandler
        LDR     R0, =SPI2_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_ORed_Message_buffer_IRQHandler
        PUBWEAK CAN0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_ORed_Message_buffer_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_Bus_Off_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_Bus_Off_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_Error_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_Error_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_Tx_Warning_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_Tx_Warning_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_Rx_Warning_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_Rx_Warning_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_Wake_Up_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_Wake_Up_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK I2S0_Tx_IRQHandler
        PUBWEAK I2S0_Tx_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2S0_Tx_IRQHandler
        LDR     R0, =I2S0_Tx_DriverIRQHandler
        BX      R0

        PUBWEAK I2S0_Rx_IRQHandler
        PUBWEAK I2S0_Rx_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2S0_Rx_IRQHandler
        LDR     R0, =I2S0_Rx_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_ORed_Message_buffer_IRQHandler
        PUBWEAK CAN1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_ORed_Message_buffer_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_Bus_Off_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_Bus_Off_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_Error_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_Error_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_Tx_Warning_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_Tx_Warning_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_Rx_Warning_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_Rx_Warning_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_Wake_Up_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_Wake_Up_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved59_IRQHandler
        PUBWEAK UART0_LON_IRQHandler
        PUBWEAK UART0_LON_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART0_LON_IRQHandler
        LDR     R0, =UART0_LON_DriverIRQHandler
        BX      R0

        PUBWEAK UART0_RX_TX_IRQHandler
        PUBWEAK UART0_RX_TX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART0_RX_TX_IRQHandler
        LDR     R0, =UART0_RX_TX_DriverIRQHandler
        BX      R0

        PUBWEAK UART0_ERR_IRQHandler
        PUBWEAK UART0_ERR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART0_ERR_IRQHandler
        LDR     R0, =UART0_ERR_DriverIRQHandler
        BX      R0

        PUBWEAK UART1_RX_TX_IRQHandler
        PUBWEAK UART1_RX_TX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART1_RX_TX_IRQHandler
        LDR     R0, =UART1_RX_TX_DriverIRQHandler
        BX      R0

        PUBWEAK UART1_ERR_IRQHandler
        PUBWEAK UART1_ERR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART1_ERR_IRQHandler
        LDR     R0, =UART1_ERR_DriverIRQHandler
        BX      R0

        PUBWEAK UART2_RX_TX_IRQHandler
        PUBWEAK UART2_RX_TX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART2_RX_TX_IRQHandler
        LDR     R0, =UART2_RX_TX_DriverIRQHandler
        BX      R0

        PUBWEAK UART2_ERR_IRQHandler
        PUBWEAK UART2_ERR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART2_ERR_IRQHandler
        LDR     R0, =UART2_ERR_DriverIRQHandler
        BX      R0

        PUBWEAK UART3_RX_TX_IRQHandler
        PUBWEAK UART3_RX_TX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART3_RX_TX_IRQHandler
        LDR     R0, =UART3_RX_TX_DriverIRQHandler
        BX      R0

        PUBWEAK UART3_ERR_IRQHandler
        PUBWEAK UART3_ERR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART3_ERR_IRQHandler
        LDR     R0, =UART3_ERR_DriverIRQHandler
        BX      R0

        PUBWEAK UART4_RX_TX_IRQHandler
        PUBWEAK UART4_RX_TX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART4_RX_TX_IRQHandler
        LDR     R0, =UART4_RX_TX_DriverIRQHandler
        BX      R0

        PUBWEAK UART4_ERR_IRQHandler
        PUBWEAK UART4_ERR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART4_ERR_IRQHandler
        LDR     R0, =UART4_ERR_DriverIRQHandler
        BX      R0

        PUBWEAK UART5_RX_TX_IRQHandler
        PUBWEAK UART5_RX_TX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART5_RX_TX_IRQHandler
        LDR     R0, =UART5_RX_TX_DriverIRQHandler
        BX      R0

        PUBWEAK UART5_ERR_IRQHandler
        PUBWEAK UART5_ERR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART5_ERR_IRQHandler
        LDR     R0, =UART5_ERR_DriverIRQHandler
        BX      R0

        PUBWEAK ADC0_IRQHandler
        PUBWEAK ADC1_IRQHandler
        PUBWEAK CMP0_IRQHandler
        PUBWEAK CMP1_IRQHandler
        PUBWEAK CMP2_IRQHandler
        PUBWEAK FTM0_IRQHandler
        PUBWEAK FTM1_IRQHandler
        PUBWEAK FTM2_IRQHandler
        PUBWEAK CMT_IRQHandler
        PUBWEAK RTC_IRQHandler
        PUBWEAK RTC_Seconds_IRQHandler
        PUBWEAK PIT0_IRQHandler
        PUBWEAK PIT1_IRQHandler
        PUBWEAK PIT2_IRQHandler
        PUBWEAK PIT3_IRQHandler
        PUBWEAK PDB0_IRQHandler
        PUBWEAK USB0_IRQHandler
        PUBWEAK USBDCD_IRQHandler
        PUBWEAK ENET_1588_Timer_IRQHandler
        PUBWEAK ENET_1588_Timer_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET_1588_Timer_IRQHandler
        LDR     R0, =ENET_1588_Timer_DriverIRQHandler
        BX      R0

        PUBWEAK ENET_Transmit_IRQHandler
        PUBWEAK ENET_Transmit_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET_Transmit_IRQHandler
        LDR     R0, =ENET_Transmit_DriverIRQHandler
        BX      R0

        PUBWEAK ENET_Receive_IRQHandler
        PUBWEAK ENET_Receive_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET_Receive_IRQHandler
        LDR     R0, =ENET_Receive_DriverIRQHandler
        BX      R0

        PUBWEAK ENET_Error_IRQHandler
        PUBWEAK ENET_Error_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET_Error_IRQHandler
        LDR     R0, =ENET_Error_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved95_IRQHandler
        PUBWEAK SDHC_IRQHandler
        PUBWEAK SDHC_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SDHC_IRQHandler
        LDR     R0, =SDHC_DriverIRQHandler
        BX      R0

        PUBWEAK DAC0_IRQHandler
        PUBWEAK DAC1_IRQHandler
        PUBWEAK TSI0_IRQHandler
        PUBWEAK MCG_IRQHandler
        PUBWEAK LPTMR0_IRQHandler
        PUBWEAK Reserved102_IRQHandler
        PUBWEAK PORTA_IRQHandler
        PUBWEAK PORTB_IRQHandler
        PUBWEAK PORTC_IRQHandler
        PUBWEAK PORTD_IRQHandler
        PUBWEAK PORTE_IRQHandler
        PUBWEAK Reserved108_IRQHandler
        PUBWEAK Reserved109_IRQHandler
        PUBWEAK SWI_IRQHandler
        PUBWEAK Reserved111_IRQHandler
        PUBWEAK Reserved112_IRQHandler
        PUBWEAK Reserved113_IRQHandler
        PUBWEAK Reserved114_IRQHandler
        PUBWEAK Reserved115_IRQHandler
        PUBWEAK Reserved116_IRQHandler
        PUBWEAK Reserved117_IRQHandler
        PUBWEAK Reserved118_IRQHandler
        PUBWEAK Reserved119_IRQHandler
        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA0_DriverIRQHandler
DMA1_DriverIRQHandler
DMA2_DriverIRQHandler
DMA3_DriverIRQHandler
DMA4_DriverIRQHandler
DMA5_DriverIRQHandler
DMA6_DriverIRQHandler
DMA7_DriverIRQHandler
DMA8_DriverIRQHandler
DMA9_DriverIRQHandler
DMA10_DriverIRQHandler
DMA11_DriverIRQHandler
DMA12_DriverIRQHandler
DMA13_DriverIRQHandler
DMA14_DriverIRQHandler
DMA15_DriverIRQHandler
DMA_Error_DriverIRQHandler
MCM_IRQHandler
FTFL_IRQHandler
Read_Collision_IRQHandler
LVD_LVW_IRQHandler
LLWU_IRQHandler
WDOG_EWM_IRQHandler
RNG_IRQHandler
I2C0_DriverIRQHandler
I2C1_DriverIRQHandler
SPI0_DriverIRQHandler
SPI1_DriverIRQHandler
SPI2_DriverIRQHandler
CAN0_DriverIRQHandler
I2S0_Tx_DriverIRQHandler
I2S0_Rx_DriverIRQHandler
CAN1_DriverIRQHandler
Reserved59_IRQHandler
UART0_LON_DriverIRQHandler
UART0_RX_TX_DriverIRQHandler
UART0_ERR_DriverIRQHandler
UART1_RX_TX_DriverIRQHandler
UART1_ERR_DriverIRQHandler
UART2_RX_TX_DriverIRQHandler
UART2_ERR_DriverIRQHandler
UART3_RX_TX_DriverIRQHandler
UART3_ERR_DriverIRQHandler
UART4_RX_TX_DriverIRQHandler
UART4_ERR_DriverIRQHandler
UART5_RX_TX_DriverIRQHandler
UART5_ERR_DriverIRQHandler
ADC0_IRQHandler
ADC1_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
CMP2_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
CMT_IRQHandler
RTC_IRQHandler
RTC_Seconds_IRQHandler
PIT0_IRQHandler
PIT1_IRQHandler
PIT2_IRQHandler
PIT3_IRQHandler
PDB0_IRQHandler
USB0_IRQHandler
USBDCD_IRQHandler
ENET_1588_Timer_DriverIRQHandler
ENET_Transmit_DriverIRQHandler
ENET_Receive_DriverIRQHandler
ENET_Error_DriverIRQHandler
Reserved95_IRQHandler
SDHC_DriverIRQHandler
DAC0_IRQHandler
DAC1_IRQHandler
TSI0_IRQHandler
MCG_IRQHandler
LPTMR0_IRQHandler
Reserved102_IRQHandler
PORTA_IRQHandler
PORTB_IRQHandler
PORTC_IRQHandler
PORTD_IRQHandler
PORTE_IRQHandler
Reserved108_IRQHandler
Reserved109_IRQHandler
SWI_IRQHandler
Reserved111_IRQHandler
Reserved112_IRQHandler
Reserved113_IRQHandler
Reserved114_IRQHandler
Reserved115_IRQHandler
Reserved116_IRQHandler
Reserved117_IRQHandler
Reserved118_IRQHandler
Reserved119_IRQHandler
DefaultISR
        B DefaultISR

        END
