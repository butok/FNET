/*
 * File:	dma_channels.h
 * Purpose:	DMA request macros for use on Kinetis processors.
 * 		This file gives default DMA channel assignments
 *		for all of the possible Kinetis module DMA requests.
 *
 *
 * Notes:	There are more DMA requests than DMA channels, so
 *		care should be taken to make sure that DMA channel
 *		assignments are unique for the modules that are 
 *		being used at any time. 
 */

#ifndef _DMA_CHANNELS_H
#define _DMA_CHANNELS_H

/********************************************************************/
/* NOTE: There are more DMA requests than DMA channels, so
 *       care should be taken to make sure that DMA channel
 *	 assignments are unique for the modules that are 
 *	 being used at any time. 
 *	
 *	 It is recommended that you read the appropriate DMAMUX_CHCFGn
 *	 register before updating it to verify it is 0x0. If the
 *	 DMAMUX_CHCFGn register is not zero, then that indicates the 
 *	 selected DMA channel might already be in use by another peripheral
 *	 (a more specific test would be to look for DMAMUX_CHCFGn[ENBL] set).
 *	 The module's DMA configuration routine can return an error
 *	 when this situation is detected.
 */
 
 
/* Default DMA channel assignments and module request macros */

/* UARTs */
#define DMA_UART0RX_CH			16  /* 0 - 31 are valid */
#define DMA_UART0TX_CH			17  /* 0 - 31 are valid */

#define DMA_UART1RX_CH			2   /* 0 - 31 are valid */    
#define DMA_UART1TX_CH			3   /* 0 - 31 are valid */

#define DMA_UART2RX_CH			8   /* 0 - 31 are valid */
#define DMA_UART2TX_CH			9   /* 0 - 31 are valid */

#define DMA_UART3RX_CH			14  /* 0 - 31 are valid */
#define DMA_UART3TX_CH			15  /* 0 - 31 are valid */

#define DMA_UART4RX_CH			27  /* 0 - 31 are valid */
#define DMA_UART4TX_CH			28  /* 0 - 31 are valid */

#define DMA_UART5RX_CH			29  /* 0 - 31 are valid */
#define DMA_UART5TX_CH			30  /* 0 - 31 are valid */

/* SSI/SAI */
#define DMA_SAI0RX_CH			18  /* 0 - 31 are valid */
#define DMA_SAI0TX_CH			19  /* 0 - 31 are valid */

/* DSPIs */
#define DMA_DSPI0RX_CH			20  /* 0 - 31 are valid */
#define DMA_DSPI0TX_CH			21  /* 0 - 31 are valid */

#define DMA_DSPI1RX_CH			6   /* 0 - 31 are valid */
#define DMA_DSPI1TX_CH			7   /* 0 - 31 are valid */

#define DMA_DSPI2RX_CH			10  /* 0 - 31 are valid */
#define DMA_DSPI2TX_CH			12  /* 0 - 31 are valid */

/* I2Cs */
#define DMA_I2C0_CH			0   /* 0 - 15 are valid */
#define DMA_I2C1_CH			1   /* 0 - 15 are valid */

/* FTMs */
#define DMA_FTM0CH0_CH			2   /* 0 - 15 are valid */
#define DMA_FTM0CH1_CH			3   /* 0 - 15 are valid */
#define DMA_FTM0CH2_CH			4   /* 0 - 15 are valid */
#define DMA_FTM0CH3_CH			5   /* 0 - 15 are valid */
#define DMA_FTM0CH4_CH			6   /* 0 - 15 are valid */
#define DMA_FTM0CH5_CH			7   /* 0 - 15 are valid */
#define DMA_FTM0CH6_CH			8   /* 0 - 15 are valid */
#define DMA_FTM0CH7_CH			9   /* 0 - 15 are valid */

#define DMA_FTM1CH0_CH			10  /* 0 - 15 are valid */
#define DMA_FTM1CH1_CH			11  /* 0 - 15 are valid */

#define DMA_FTM2CH0_CH			12  /* 0 - 15 are valid */
#define DMA_FTM2CH1_CH			13  /* 0 - 15 are valid */

#define DMA_FTM3CH0_CH			16  /* 16 - 31 are valid */
#define DMA_FTM3CH1_CH			17  /* 16 - 31 are valid */
#define DMA_FTM3CH2_CH			18  /* 16 - 31 are valid */
#define DMA_FTM3CH3_CH			19  /* 16 - 31 are valid */
#define DMA_FTM3CH4_CH			20  /* 16 - 31 are valid */
#define DMA_FTM3CH5_CH			21  /* 16 - 31 are valid */
#define DMA_FTM3CH6_CH			22  /* 16 - 31 are valid */
#define DMA_FTM3CH7_CH			23  /* 16 - 31 are valid */


/* Ethernet timers */
#define DMA_ENETTMR0_CH			22  /* 0 - 31 are valid */
#define DMA_ENETTMR1_CH			23  /* 0 - 31 are valid */
#define DMA_ENETTMR2_CH			29  /* 0 - 31 are valid */
#define DMA_ENETTMR3_CH			31  /* 0 - 31 are valid */

/* ADCs */
#define DMA_ADC0_CH			24  /* 0 - 31 are valid */
#define DMA_ADC1_CH			25  /* 0 - 31 are valid */

/* CMPs */
#define DMA_CMP0_CH			26  /* 0 - 31 are valid */
#define DMA_CMP1_CH			27  /* 0 - 31 are valid */
#define DMA_CMP2_CH			28  /* 0 - 31 are valid */

/* 12-bit DAC */
#define DMA_12bDAC0_CH			25  /* 0 - 31 are valid */
#define DMA_12bDAC1_CH                  26  /* 0 - 31 are valid */

/* CMT */
#define DMA_CMT_CH			14  /* 0 - 15 are valid */

/* PDB */
#define DMA_PDB_CH			15  /* 0 - 15 are valid */

/* GPIO Ports */
#define DMA_GPIOPORTA_CH		1   /* 0 - 15 are valid */
#define DMA_GPIOPORTB_CH		4   /* 0 - 15 are valid */
#define DMA_GPIOPORTC_CH		11  /* 0 - 15 are valid */
#define DMA_GPIOPORTD_CH		13  /* 0 - 15 are valid */
#define DMA_GPIOPORTE_CH		5   /* 0 - 15 are valid */
#define DMA_GPIOPORTF_CH                24  /* 16 - 31 are valid */

/********************************************************************/

#endif /* _DMA_CHANNELS_H */
