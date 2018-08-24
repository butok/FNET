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
