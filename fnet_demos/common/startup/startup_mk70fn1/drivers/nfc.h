/*
 * File:        nfc.h
 * Purpose:     Provide common NAND flash controller routines
 * 
 * Notes:
 */

#ifndef __NFC_H__
#define __NFC_H__

#include "common.h"
#include "nand.h"

/* Defines for NAND command values */
#define NFC_READ_PAGE_CMD_CODE        	    0x7EF8
#define NFC_PROGRAM_PAGE_CMD_CODE           0x7FD8
#define NFC_DMA_PROGRAM_PAGE_CMD_CODE       0xFFC0  
#define NFC_BLOCK_ERASE_CMD_CODE      	    0x4ED8    
#define NFC_READ_ID_CMD_CODE          	    0x4804
#define NFC_RESET_CMD_CODE            	    0x4040

#define PAGE_READ_CMD_BYTE1       	    0x00
#define PAGE_READ_CMD_BYTE2       	    0x30
#define PROGRAM_PAGE_CMD_BYTE1    	    0x80
#define PROGRAM_PAGE_CMD_BYTE2    	    0x10
#define READ_STATUS_CMD_BYTE      	    0x70
#define BLOCK_ERASE_CMD_BYTE1     	    0x60
#define BLOCK_ERASE_CMD_BYTE2     	    0xD0

#define READ_ID_CMD_BYTE                    0x90
#define RESET_CMD_BYTE                      0xFF

/* Defines for NFC chip selects and port size */
#define NFC_CE0                       0
#define NFC_CE1                       1
#define NFC_8BIT		      0
#define NFC_16BIT		      1

/* Define an NFC error code. In the future this code could be changed
 * to a list of specific error codes, but for now there is just one 
 * generic code.
 */
#define NFC_ERR                       99    
#define NFC_GOOD                      0

/* Defines for page sizes - used for page size and ECC page size */
#define PAGE_4K              	0x1000
#define PAGE_2K              	0x0800
#define PAGE_1K             	0x0400
#define PAGE_512		0x0200
#define PAGE_128             	0x0080
#define PAGE_64              	0x0040
#define PAGE_32              	0x0020
#define PAGE_16 		0x0010



/* Function prototypes */
void nand_init(void);
void nfc_reset_cmd(void);
void nfc_read_flash_id(uint8, uint32*, uint32*);
void nfc_block_erase(uint32);
void nfc_page_program(uint8, uint32, uint16);
void nfc_page_read(uint8, uint32, uint16);

#endif /* __NFC_H__ */
