/*
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* o Redistributions of source code must retain the above copyright notice, this list
*   of conditions and the following disclaimer.
*
* o Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* o Neither the name of Freescale Semiconductor, Inc. nor the names of its
*   contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#if (FSL_PORT_DRIVER_VERSION != MAKE_VERSION(2, 0, 2))    
    #error A new version of PORT driver is detected
#endif

/*******************************************************************************
* Code
******************************************************************************/

void BOARD_InitPorts(void)
{
    /* Enable enet port clocks */
    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortB);
    CLOCK_EnableClock(kCLOCK_PortC);      
    CLOCK_EnableClock(kCLOCK_PortE); 
    CLOCK_EnableClock(kCLOCK_PortD); 
}

void BOARD_InitLedPins(void)
{       
    /* Affects PORTB_PCR22 register */
    PORT_SetPinMux(PORTB,22u,kPORT_MuxAsGpio);
    /* Affects PORTB_PCR21 register */    
    PORT_SetPinMux(PORTB,21u,kPORT_MuxAsGpio);            
    /* Affects PORTE_PCR26 register */
    PORT_SetPinMux(PORTE,26u,kPORT_MuxAsGpio);            
}


void BOARD_InitSwPins(void)
{
    port_pin_config_t pinConfig = {0};
    pinConfig.pullSelect = kPORT_PullUp;
    pinConfig.mux = kPORT_MuxAsGpio;
    pinConfig.passiveFilterEnable = kPORT_PassiveFilterEnable;
    pinConfig.openDrainEnable = kPORT_OpenDrainDisable;
    
    /* I2C0 */
    /* Affects PORTE_PCR24 register */
    PORT_SetPinConfig(PORTA, 4U, &pinConfig);
    /* Affects PORTE_PCR25 register */
    PORT_SetPinConfig(PORTC, 6U, &pinConfig);        
}

void BOARD_InitI2cPins(void)
{
    port_pin_config_t pinConfig = {0};
    pinConfig.pullSelect = kPORT_PullUp;
    pinConfig.openDrainEnable = kPORT_OpenDrainEnable;
    pinConfig.mux = kPORT_MuxAlt5;
    
    /* I2C0 */
    /* Affects PORTE_PCR24 register */   
    PORT_SetPinConfig(PORTE, 24U, &pinConfig);
    /* Affects PORTE_PCR25 register */   
    PORT_SetPinConfig(PORTE, 25U, &pinConfig);        
}

void BOARD_InitUartPins(void)
{
    /* UART2 */
    /* Affects PORTD_PCR2 register */
    PORT_SetPinMux(PORTD,2u,kPORT_MuxAlt3);
    /* Affects PORTD_PCR3 register */
    PORT_SetPinMux(PORTD,3u,kPORT_MuxAlt3);       
}

void BOARD_InitDbgUartPins(void)
{
    /* UART0 */
    /* Affects PORTD_PCR2 register */
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt3);
    /* Affects PORTB_PCR17 register */
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt3);       
}

#if 0 //DM

#define PCR_PE_ENABLED                0x01u   /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
#define PCR_PS_UP                     0x01u   /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitGT202Shield:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '93', peripheral: SPI0, signal: PCS0_SS, pin_signal: PTD0/LLWU_P12/SPI0_PCS0/UART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b}
  - {pin_num: '94', peripheral: SPI0, signal: SCK, pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/UART2_CTS_b/FTM3_CH1/FB_CS0_b}
  - {pin_num: '95', peripheral: SPI0, signal: SOUT, pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/I2C0_SCL}
  - {pin_num: '96', peripheral: SPI0, signal: SIN, pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/I2C0_SDA}
  - {pin_num: '73', peripheral: GPIOC, signal: 'GPIO, 3', pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK, direction: INPUT, pull_select: up,
    pull_enable: enable}
  - {pin_num: '84', peripheral: GPIOC, signal: 'GPIO, 12', pin_signal: PTC12/UART4_RTS_b/FB_AD27/FTM3_FLT0, direction: OUTPUT, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitGT202Shield
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitGT202Shield(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTC, PIN12_IDX, kPORT_MuxAsGpio);         /* PORTC12 (pin 84) is configured as PTC12 */
  PORTC->PCR[12] = ((PORTC->PCR[12] &
    (~(PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))               /* Mask bits to zero which are setting */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
    );
  PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAsGpio);          /* PORTC3 (pin 73) is configured as PTC3 */
  PORTC->PCR[3] = ((PORTC->PCR[3] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
    );
  PORT_SetPinMux(PORTD, PIN0_IDX, kPORT_MuxAlt2);            /* PORTD0 (pin 93) is configured as SPI0_PCS0 */
  PORT_SetPinMux(PORTD, PIN1_IDX, kPORT_MuxAlt2);            /* PORTD1 (pin 94) is configured as SPI0_SCK */
  PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_MuxAlt2);            /* PORTD2 (pin 95) is configured as SPI0_SOUT */
  PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_MuxAlt2);            /* PORTD3 (pin 96) is configured as SPI0_SIN */
}


#define PCR_PE_ENABLED                0x01u   /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
#define PCR_PS_UP                     0x01u   /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN9_IDX                         9u   /*!< Pin number for pin 9 in a port */
#define PIN23_IDX                       23u   /*!< Pin number for pin 23 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitSilex2401Shield:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '96', peripheral: SPI0, signal: SIN, pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/I2C0_SDA}
  - {pin_num: '95', peripheral: SPI0, signal: SOUT, pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/I2C0_SCL}
  - {pin_num: '94', peripheral: SPI0, signal: SCK, pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/UART2_CTS_b/FTM3_CH1/FB_CS0_b}
  - {pin_num: '93', peripheral: SPI0, signal: PCS0_SS, pin_signal: PTD0/LLWU_P12/SPI0_PCS0/UART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b}
  - {pin_num: '57', peripheral: GPIOB, signal: 'GPIO, 9', pin_signal: PTB9/SPI1_PCS1/UART3_CTS_b/FB_AD20, direction: INPUT, pull_select: up, pull_enable: enable}
  - {pin_num: '69', peripheral: GPIOB, signal: 'GPIO, 23', pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28, direction: OUTPUT, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitSilex2401Shield
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitSilex2401Shield(void) {
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTB, PIN23_IDX, kPORT_MuxAsGpio);         /* PORTB23 (pin 69) is configured as PTB23 */
  PORTB->PCR[23] = ((PORTB->PCR[23] &
    (~(PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))               /* Mask bits to zero which are setting */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
    );
  PORT_SetPinMux(PORTB, PIN9_IDX, kPORT_MuxAsGpio);          /* PORTB9 (pin 57) is configured as PTB9 */
  PORTB->PCR[9] = ((PORTB->PCR[9] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
    );
  PORT_SetPinMux(PORTD, PIN0_IDX, kPORT_MuxAlt2);            /* PORTD0 (pin 93) is configured as SPI0_PCS0 */
  PORT_SetPinMux(PORTD, PIN1_IDX, kPORT_MuxAlt2);            /* PORTD1 (pin 94) is configured as SPI0_SCK */
  PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_MuxAlt2);            /* PORTD2 (pin 95) is configured as SPI0_SOUT */
  PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_MuxAlt2);            /* PORTD3 (pin 96) is configured as SPI0_SIN */
}
#endif

/* END pin_mux. */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.4 [05.10]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
