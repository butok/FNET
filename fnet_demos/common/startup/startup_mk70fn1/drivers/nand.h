/*
 * File:        nand.h
 * Purpose:     Defines used to specify the parameters for a particular
 *              NAND flash on a given board. This version of the file
 *              is for the TWR-K70F120M and TWR-K60F120M. 
 * 
 * Notes:       This file should be updated as needed when the nfc driver
 *              is being ported to a new board.
 */

#ifndef __NAND_H__
#define __NAND_H__

#include "nfc.h"

/* Define the desired NFC clock dividers */
#define NFCDIV          0x05
#define NFCFRAC         0x00


/* Define the NAND Flash device I/O width */
#define DATA_WIDTH 		NFC_16BIT

/* Define the NAND Flash device CE# */
#define NFC_CE 			NFC_CE0

/* Define to select EDO mode - comment out if EDO mode is not desired */
//#define NFC_EDO                       1

/* Define the Page and ECC size for the memory */
#define PAGE_SIZE PAGE_2K
#define ECC_SIZE  PAGE_64


#endif /* __NAND_H__ */
