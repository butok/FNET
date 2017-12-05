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

#ifndef pin_mux_H_
#define pin_mux_H_

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
  kPIN_MUX_DirectionInput = 0U,         /* Input direction */
  kPIN_MUX_DirectionOutput = 1U,        /* Output direction */
  kPIN_MUX_DirectionInputOrOutput = 2U  /* Input or output direction */
} pin_mux_direction_t;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
        
void BOARD_InitDbgUartPins(void);

void BOARD_InitSwPins(void);

void BOARD_InitLedPins(void);

void BOARD_InitPorts(void);

void BOARD_InitI2cPins(void);

void BOARD_InitUartPins(void);    


/* PORTC3 (number 73), IRQ */
#define BOARD_INITGT202SHIELD_IRQ_GPIO                                     GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITGT202SHIELD_IRQ_PORT                                     PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITGT202SHIELD_IRQ_GPIO_PIN                                    3U   /*!< PORTC pin index: 3 */
#define BOARD_INITGT202SHIELD_IRQ_PIN_NAME                                  PTC3   /*!< Pin name */
#define BOARD_INITGT202SHIELD_IRQ_LABEL                                    "IRQ"   /*!< Label */
#define BOARD_INITGT202SHIELD_IRQ_NAME                                     "IRQ"   /*!< Identifier name */
#define BOARD_INITGT202SHIELD_IRQ_DIRECTION              kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTC12 (number 84), PWRON */
#define BOARD_INITGT202SHIELD_PWRON_GPIO                                   GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITGT202SHIELD_PWRON_PORT                                   PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITGT202SHIELD_PWRON_GPIO_PIN                                 12U   /*!< PORTC pin index: 12 */
#define BOARD_INITGT202SHIELD_PWRON_PIN_NAME                               PTC12   /*!< Pin name */
#define BOARD_INITGT202SHIELD_PWRON_LABEL                                "PWRON"   /*!< Label */
#define BOARD_INITGT202SHIELD_PWRON_NAME                                 "PWRON"   /*!< Identifier name */
#define BOARD_INITGT202SHIELD_PWRON_DIRECTION           kPIN_MUX_DirectionOutput   /*!< Direction */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitGT202Shield(void);

/* PORTB9 (number 57), IRQ */
#define BOARD_INITSILEX2401SHIELD_IRQ_GPIO                                 GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITSILEX2401SHIELD_IRQ_PORT                                 PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITSILEX2401SHIELD_IRQ_GPIO_PIN                                9U   /*!< PORTB pin index: 9 */
#define BOARD_INITSILEX2401SHIELD_IRQ_PIN_NAME                              PTB9   /*!< Pin name */
#define BOARD_INITSILEX2401SHIELD_IRQ_LABEL                                "IRQ"   /*!< Label */
#define BOARD_INITSILEX2401SHIELD_IRQ_NAME                                 "IRQ"   /*!< Identifier name */
#define BOARD_INITSILEX2401SHIELD_IRQ_DIRECTION          kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTB23 (number 69), PWRON */
#define BOARD_INITSILEX2401SHIELD_PWRON_GPIO                               GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITSILEX2401SHIELD_PWRON_PORT                               PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITSILEX2401SHIELD_PWRON_GPIO_PIN                             23U   /*!< PORTB pin index: 23 */
#define BOARD_INITSILEX2401SHIELD_PWRON_PIN_NAME                           PTB23   /*!< Pin name */
#define BOARD_INITSILEX2401SHIELD_PWRON_LABEL                            "PWRON"   /*!< Label */
#define BOARD_INITSILEX2401SHIELD_PWRON_NAME                             "PWRON"   /*!< Identifier name */
#define BOARD_INITSILEX2401SHIELD_PWRON_DIRECTION       kPIN_MUX_DirectionOutput   /*!< Direction */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSilex2401Shield(void);

    
#if defined(__cplusplus)
}
#endif /* __cplusplus */
/* END pin_mux. */
#endif /* #ifndef __pin_mux_H_ */
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
