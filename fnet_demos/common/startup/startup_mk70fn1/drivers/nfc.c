/*
 * File:        nfc.c
 * Purpose:     Provide common NAND flash controller routines
 * 
 * Notes:       nand.h is used to specify parameters associated
 *              with the particular NAND flash populated on a 
 *              board and the board itself. When porting this
 *              driver to a new board make sure to make any 
 *              needed updates to the nand.h file.
 */

#include "nfc.h"

/********************************************************************/
/* Initialization routine for the NFC. This function will enable
 * NFC pin functions for all of the required pins and perform 
 * basic initialization of the NFC based on the port size and 
 * chip select specified by the nand.h file.
 *
 * Parameters:
 *  none
 */
void nand_init(void)
{
    /* Enable the NFC clock gate in the SIM */
    SIM_SCGC3 |= SIM_SCGC3_NFC_MASK; 	
    
    /* Disable the MPU to allow NFC to access memory */
    MPU_CESR &= 0xFFFFFFFE;  		

    /* Set the NFC clock divder according to defines in nand.h */
    SIM_CLKDIV4 |= SIM_CLKDIV4_NFCDIV(NFCDIV) |SIM_CLKDIV4_NFCFRAC(NFCFRAC);

    /* Make sure that NFC_CE is defined */
    #if (!(defined(NFC_CE)))
      #error "NFC_CE must be defined in nand.h"
    #endif
  
    /* Enable the chip select in the NFC and enable the pad for the CS */
    if(NFC_CE == NFC_CE0)
    {
        NFC_RAR |= (NFC_RAR_CS0_MASK | NFC_RAR_RB0_MASK);
	  
	PORTC_PCR17 =  PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK;  /* Enable NFC_CE0 pad */     
    }
    else /* (NFC_CE == NFC_CE1) */
    {
        NFC_RAR |= (NFC_RAR_CS1_MASK | NFC_RAR_RB1_MASK);
	  
	PORTC_PCR18 =  PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK;  /* Enable NFC_CE0 pad */     
    }	
    
    /* Enable all of the NFC control signal pads */
    PORTC_PCR16 =  PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK; /* Enable the NFC_RB pad */
    PORTD_PCR8 =  PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK; /* Enable the NFC_CLE pad */
    PORTD_PCR9 =  PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK; /* Enable the NFC_ALE pad */
    PORTD_PCR10 =  PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK; /* Enable the NFC_RE pad */
    PORTC_PCR11 =  PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_WE pad */

    /* Configure bit width in NFC and enable the NFC_IO pins based on the
     * defined port size in nand.h.
     */
    if (DATA_WIDTH == NFC_16BIT) 
    {
	NFC_CFG |= NFC_CFG_BITWIDTH_MASK;

        PORTB_PCR20 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA15 pad */
    	PORTB_PCR21 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA14 pad */
    	PORTB_PCR22 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA13 pad */
    	PORTB_PCR23 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA12 pad */
    	PORTC_PCR0 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA11 pad */ 
    	PORTC_PCR1 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA10 pad */
    	PORTC_PCR2 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA9 pad */
    	PORTC_PCR4 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA8 pad */
    	PORTC_PCR5 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA7 pad */
    	PORTC_PCR6 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA6 pad */
	PORTC_PCR7 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA5 pad */
    	PORTC_PCR8 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA4 pad */
	PORTC_PCR9 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA3 pad */
    	PORTC_PCR10 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA2 pad */
    	PORTD_PCR4 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA1 pad */
    	PORTD_PCR5 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA0 pad */

        /* Set the sector size in the NFC to match the memory */
        NFC_SECSZ = (PAGE_SIZE|(ECC_SIZE+1));
    }
    else /* (DATA_WIDTH == NFC_8BIT) */
    {
	NFC_CFG &= ~NFC_CFG_BITWIDTH_MASK;
        
    	PORTC_PCR5 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA7 pad */
    	PORTC_PCR6 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA6 pad */
	PORTC_PCR7 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA5 pad */
    	PORTC_PCR8 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA4 pad */
	PORTC_PCR9 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA3 pad */
    	PORTC_PCR10 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA2 pad */
    	PORTD_PCR4 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA1 pad */
    	PORTD_PCR5 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK; /* Enable the NFC_DATA0 pad */

        /* Set the sector size in the NFC to match the memory */
        NFC_SECSZ = (PAGE_SIZE | ECC_SIZE);
    }
    
    /* Configure NFC for EDO mode if defined in nand.h */
    #if defined(NFC_EDO)
        NFC_CFG |= NFC_CFG_FAST_MASK;
    #endif
    
    /* Disable auto address and buff num increment */
    NFC_CFG &= ~(NFC_CFG_AIAD_MASK | NFC_CFG_AIBN_MASK);   
    
    /* Set NFC to STOP if a write error is detected */
    NFC_CFG |= NFC_CFG_STOPWERR_MASK;
    
    /* Disable row address increment */
    NFC_RAI = 0;
    
    /* Disable swap */
    NFC_SWAP = 0;
}  
/********************************************************************/
/* Sends a reset command to the NAND flash. This function should 
 * always be called after NFC initialization before any other
 * command is sent to the NAND flash.
 *
 * Parameters:
 *  none
 */
void nfc_reset_cmd(void) 
{
    /* Clear all status and error bits in the NFC_ISR register */
    NFC_ISR |= ( NFC_ISR_WERRCLR_MASK
                | NFC_ISR_DONECLR_MASK
                | NFC_ISR_IDLECLR_MASK );

    /* Write the NFC_CMD2 register with the command byte and code for a reset */
    NFC_CMD2 = NFC_CMD2_BYTE1(RESET_CMD_BYTE) | NFC_CMD2_CODE(NFC_RESET_CMD_CODE);
	
    /* Set Start Bit to send reset to the NAND flash */
    NFC_CMD2 |= NFC_CMD2_BUSY_START_MASK;

    /* Wait for start/busy bit to clear indicating command is done */ 
    while (NFC_CMD2 & NFC_CMD2_BUSY_START_MASK);
}
/********************************************************************/
/* Reads the NAND flash ID for the specified buffer.
 *
 * Parameters:
 *  fls_bufno   NAND flash internal buffer number
 *  fls_id0     first 32 bits of the flash ID (return value)
 *  fls_id1     second 32 bits of the flash ID (return value)
 */
void nfc_read_flash_id(uint8 flash_bufno, uint32* flash_id0, uint32* flash_id1) 
{
    /* Clear all status and error bits in the NFC_ISR register */
    NFC_ISR |= ( NFC_ISR_WERRCLR_MASK
                | NFC_ISR_DONECLR_MASK
                | NFC_ISR_IDLECLR_MASK );

    /* Write the NFC_CMD2 register with the command byte and code for a reset */
    NFC_CMD2 = NFC_CMD2_BYTE1(READ_ID_CMD_BYTE) 
              | NFC_CMD2_CODE(NFC_READ_ID_CMD_CODE)
              | NFC_CMD2_BUFNO(flash_bufno);
	
    /* Set Start Bit to send command to the NAND flash */
    NFC_CMD2 |= NFC_CMD2_BUSY_START_MASK;

    /* Wait for start/busy bit to clear indicating command is done */ 
    while (NFC_CMD2 & NFC_CMD2_BUSY_START_MASK);
 
    /* Load the flash ID values into fls_id0 and fls_id1 */
    *((vuint32 *)(flash_id0)) = NFC_SR1;
    *((vuint32 *)(flash_id1)) = NFC_SR2;
}
/********************************************************************/
/* Erases the NFC block containing the specified address. This function
 * will only erase one block at a time. If multiple blocks need to be
 * erased, then the function should be called once per block to erase.
 *
 * Parameters:
 *  row_addr   NAND flash row addr for the block to erase (up to 24 bits)
 */
void nfc_block_erase(uint32 row_addr)
{
    /* Clear all status and error bits in the NFC_ISR register */
    NFC_ISR |= ( NFC_ISR_WERRCLR_MASK
                | NFC_ISR_DONECLR_MASK
                | NFC_ISR_IDLECLR_MASK );
    
    /* Disable ECC during block erase */
    NFC_CFG &= ~NFC_CFG_ECCMODE(0x7);
    
    /* Make sure the column address is cleared - not needed for block erase */
    NFC_CAR = 0x0;
	
    /* Set the chip select to use */
    if(NFC_CE == NFC_CE0)
    {
        NFC_RAR = (NFC_RAR_CS0_MASK | NFC_RAR_RB0_MASK);
     }
    else /* (NFC_CE == NFC_CE1) */
    {
        NFC_RAR = (NFC_RAR_CS1_MASK | NFC_RAR_RB1_MASK);
    }	

    /* Set the row address */
    NFC_RAR |= row_addr;      

    /* Write the NFC_CMD2 register with the command byte and code for an erase */
    NFC_CMD2 = (NFC_CMD2_BYTE1(BLOCK_ERASE_CMD_BYTE1) 
              | NFC_CMD2_CODE(NFC_BLOCK_ERASE_CMD_CODE) );
    
    /* Write the NFC_CMD1 register with the command byte2 and byte3 for an erase */
    NFC_CMD1 = NFC_CMD1_BYTE2(BLOCK_ERASE_CMD_BYTE2) 
                | NFC_CMD1_BYTE3(READ_STATUS_CMD_BYTE);    
	
    /* Set Start Bit to send command to the NAND flash */
    NFC_CMD2 |= NFC_CMD2_BUSY_START_MASK;

    /* Wait for start/busy bit to clear indicating command is done */ 
    while (NFC_CMD2 & NFC_CMD2_BUSY_START_MASK);  
}
/********************************************************************/
/* Programs a single page worth of data from the NFC into the NAND
 * flash memory at the specified NFC address. This function will only
 * program one NAND page at a time. If multiple pages need to be
 * programmed, then the function should be called once per page to
 * write. Data must be loaded into the NFC's buffers before calling
 * this function.
 *
 * Parameters:
 *  flash_bufno first NFC internal buffer to program to the NAND (auto increment will be used )
 *  row_addr    NAND flash row addr for the block to program (up to 24 bits)
 *  col_addr    NAND flash col addr for the page to program (up to 16 bits)
 *
 * NOTE: the column address should be aligned to the NAND page size
 */
void nfc_page_program(uint8 flash_bufno, uint32 row_addr, uint16 col_addr)
{
    /* Clear all status and error bits in the NFC_ISR register */
    NFC_ISR |= ( NFC_ISR_WERRCLR_MASK
                | NFC_ISR_DONECLR_MASK
                | NFC_ISR_IDLECLR_MASK );
        
    /* Make sure ECC is enabled before reading */
    NFC_CFG |= NFC_CFG_ECCMODE(0x7);

    /* Set the chip select to use */
    if(NFC_CE == NFC_CE0)
    {
        NFC_RAR = (NFC_RAR_CS0_MASK | NFC_RAR_RB0_MASK);
     }
    else /* (NFC_CE == NFC_CE1) */
    {
        NFC_RAR = (NFC_RAR_CS1_MASK | NFC_RAR_RB1_MASK);
    }	

    /* Set the row address */
    NFC_RAR |= row_addr;      
    
    /* Set the column address */
    NFC_CAR = col_addr;
            
    /* Write the NFC_CMD2 register with the command byte and code for an erase */
    NFC_CMD2 = (NFC_CMD2_BYTE1(PROGRAM_PAGE_CMD_BYTE1) 
              | NFC_CMD2_CODE(NFC_PROGRAM_PAGE_CMD_CODE) 
              | NFC_CMD2_BUFNO(flash_bufno)                );
    
    /* Write the NFC_CMD1 register with the command byte2 and byte3 for an erase */
    NFC_CMD1 = NFC_CMD1_BYTE2(PROGRAM_PAGE_CMD_BYTE2) 
                | NFC_CMD1_BYTE3(READ_STATUS_CMD_BYTE);    

    /* Set Start Bit to send command to the NAND flash */
    NFC_CMD2 |= NFC_CMD2_BUSY_START_MASK;

    /* Wait for start/busy bit to clear indicating command is done */ 
    while (NFC_CMD2 & NFC_CMD2_BUSY_START_MASK);  
}
/********************************************************************/
/* Reads a single page worth of data from the NAND flash at the 
 * specified address into the NFC's buffers. This function will only
 * read one NAND page at a time. If multiple pages need to be
 * read, then the function should be called once per page to
 * read. Data will be loaded into the NFC's buffers after the function
 * completes.
 *
 * Parameters:
 *  flash_bufno first NFC internal buffer to load data to (auto increment will be used )
 *  row_addr    NAND flash row addr for the block to program (up to 24 bits)
 *  col_addr    NAND flash col addr for the page to program (up to 16 bits)
 *
 * NOTE: the column address should be aligned to the NAND page size
 */
void nfc_page_read(uint8 flash_bufno, uint32 row_addr, uint16 col_addr)
{
    /* Clear all status and error bits in the NFC_ISR register */
    NFC_ISR |= ( NFC_ISR_WERRCLR_MASK
                | NFC_ISR_DONECLR_MASK
                | NFC_ISR_IDLECLR_MASK );
    
    /* Make sure ECC is enabled before reading */
    NFC_CFG |= NFC_CFG_ECCMODE(0x7);
        
    /* Set the chip select to use */
    if(NFC_CE == NFC_CE0)
    {
        NFC_RAR = (NFC_RAR_CS0_MASK | NFC_RAR_RB0_MASK);
     }
    else /* (NFC_CE == NFC_CE1) */
    {
        NFC_RAR = (NFC_RAR_CS1_MASK | NFC_RAR_RB1_MASK);
    }	

    /* Set the row address */
    NFC_RAR |= row_addr;      
    
    /* Set the column address */
    NFC_CAR = col_addr;
    
    /* Write the NFC_CMD2 register with the command byte and code for an erase */
    NFC_CMD2 = (NFC_CMD2_BYTE1(PAGE_READ_CMD_BYTE1) 
              | NFC_CMD2_CODE(NFC_READ_PAGE_CMD_CODE) 
              | NFC_CMD2_BUFNO(flash_bufno)                );
    
    /* Write the NFC_CMD1 register with the command byte2 and byte3 for an erase */
    NFC_CMD1 = NFC_CMD1_BYTE2(PAGE_READ_CMD_BYTE2) 
                | NFC_CMD1_BYTE3(READ_STATUS_CMD_BYTE);    

    /* Set Start Bit to send command to the NAND flash */
    NFC_CMD2 |= NFC_CMD2_BUSY_START_MASK;

    /* Wait for start/busy bit to clear indicating command is done */ 
    while (NFC_CMD2 & NFC_CMD2_BUSY_START_MASK);  
}
/********************************************************************/
