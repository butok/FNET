
/*
 *
 * FILE : MPC5668G_HWInit.c
 *
 * DESCRIPTION:
 *  This file contains all MPC5668G derivative needed initializations, 
 *  and all initializations for the MPC5668G boards which are supported.
 */

/*----------------------------------------------------------------------------*/
/* Includes                                                                   */
/*----------------------------------------------------------------------------*/

#include "MPC5668G.h"       /* MPC55xx platform development header            */
#include "MPC5668G_HWInit.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/* MPC5668G derivative specific hardware initialization */
/*******************************************************/

/* Symbol L2SRAM_LOCATION is defined in the application linker command file (.lcf) 
   It is defined to the start of the L2SRAM of the MPC5668G. 
*/
/*lint -esym(752, L2SRAM_LOCATION) */
extern long L2SRAM_LOCATION;  

#define TLB3_MAS0  0x10030000
#define TLB3_MAS1  0xC0000500
#define TLB3_MAS2  0x40000028
#define TLB3_MAS3  0x4000003F


__asm void INIT_Derivative(void) 
{
nofralloc

    /* Reinitialize MMU (SRAM TLB) - FADO BAM issue */ 
    //TLB 3
    lis r3,TLB3_MAS0@h /*  MAS0*/
    ori r3,r3,TLB3_MAS0@l
    mtspr   624, r3                
    lis r4,TLB3_MAS1@h /*  MAS1*/
    ori r4,r4,TLB3_MAS1@l
    mtspr   625, r4     
    lis r5,TLB3_MAS2@h /*  MAS2*/
    ori r5,r5,TLB3_MAS2@l
    mtspr   626, r5 
    lis r6,TLB3_MAS3@h /*  MAS3*/
    ori r6,r6,TLB3_MAS3@l
    mtspr   627, r6
    tlbwe   
    
    /* MPC5668G L2SRAM initialization code                         */
    lis r11,L2SRAM_LOCATION@h /* Base address of the L2SRAM, 64-bit word aligned */
    ori r11,r11,L2SRAM_LOCATION@l

    li r12,4736  /* Loop counter to get all of L2SRAM; 592k/4 bytes/32 GPRs = 4736 */
    mtctr r12

    init_l2sram_loop:
        stmw r0,0(r11)        /* Write all 32 GPRs to L2SRAM */
        addi r11,r11,128      /* Inc the ram ptr; 32 GPRs * 4 bytes = 128 */
        bdnz init_l2sram_loop /* Loop for all L2SRAM */

    blr
}

/*******************************************************/
/* MPC5668G boards specific hardware initializations    */
/*******************************************************/

/** This macro allows to use C defined address with the inline assembler */
#define MAKE_HLI_COMPATIBLE(hli_name, c_expr) enum { hli_name=/*lint -e30*/((int)(c_expr)) };

/*----------------------------------------------------------------------------*/
/* Function declarations                                                      */
/*----------------------------------------------------------------------------*/

/* Initialize a set of contiguous PCRs */
__asm void InitPCRs(void); 

/* Initialize the SIU External Bus Interface */
__asm void __initSIUExtBusInterface(void);

/* Write one MMU Table Entry */
__asm void WriteMMUTableEntry( void );


/*----------------------------------------------------------------------------*/
/* Function implementations                                                   */
/*----------------------------------------------------------------------------*/

__asm void INIT_ExternalBusAndMemory(void) 
{
nofralloc

    mflr     r28
    
    /* Initialize the SIU External Bus Interface */
    bl __initSIUExtBusInterface
    
    mtlr     r28
    
    blr
}


/*----------------------------------------------------------------------------*/
/* SIU External Bus Interface                                                 */
/*----------------------------------------------------------------------------*/

/* Initialize a set of contiguous PCRs:               */
/* r3: the firts PCR to initialize                    */
/* r4: the value to write in the PCRs                 */ 
/* r5: the number of PCRs to initialize               */
__asm void InitPCRs(void) 
{
nofralloc

    mtctr r5                   /* intialize ctr with the number of PCRs to initialize */
    pcr_init_loop:
        sth r4,0(r3)           /* Write r4 to current PCR address */
        addi r3,r3, 2          /* Inc the memory ptr by 2 to point to the next PCR */
        bdnz pcr_init_loop     /* Loop for ctr PCRs */

    blr
}

/* Initialize the SIU External Bus Interface */
__asm void __initSIUExtBusInterface(void)
{ 
MAKE_HLI_COMPATIBLE(SIU_PCR0,&SIU.PCR[0].R)
MAKE_HLI_COMPATIBLE(SIU_PCR4,&SIU.PCR[4].R)
MAKE_HLI_COMPATIBLE(SIU_PCR28,&SIU.PCR[28].R)
MAKE_HLI_COMPATIBLE(SIU_PCR62,&SIU.PCR[62].R)
MAKE_HLI_COMPATIBLE(SIU_PCR64,&SIU.PCR[64].R)
MAKE_HLI_COMPATIBLE(SIU_PCR68,&SIU.PCR[68].R)
nofralloc

    mflr r27

    /*  This initializes the MPC5633 external bus
        Set up the pins
        Address bus PCR 4 - 27
        Configure address bus pins
    */
    lis r3,SIU_PCR4@h          /* First PCR Address bus is PCR 4 */
    ori r3,r3,SIU_PCR4@l
    li r5,24                   /* Loop counter to get all address bus PCR (4 to 27) -> 24 PCRs  */
    li r4, 0x0440              /* PCRs initialization value */
    bl InitPCRs

    /*  Data bus PCR 28-59
        Configure data bus pins
    */
    lis r3,SIU_PCR28@h         /* First PCR for data bus is PCR 28 */
    ori r3,r3,SIU_PCR28@l
    li r5,32                   /* Loop counter to get all data bus PCR (28-59) -> 32 PCRs  */
    li r4, 0x0440              /* PCRs initialization value */
    bl InitPCRs

    /*  Configure minimum bus control pins
        RD/WR  & BDIP PCR 62/63
    */
    lis r3,SIU_PCR62@h         /* First PCR for is PCR 62 */
    ori r3,r3,SIU_PCR62@l
    li r5,2                    /* Loop counter to get all PCR (62-63) -> 2 PCRs  */
    li r4, 0x0440              /* PCRs initialization value */
    bl InitPCRs

    /*  WE[0-4] PCR 64-67
    */
    lis r3,SIU_PCR64@h         /* First PCR for is PCR 64 */
    ori r3,r3,SIU_PCR64@l
    li r5,4                    /* Loop counter to get all PCR (64-67) -> 4 PCRs  */
    li r4, 0x0443              /* PCRs initialization value */
    bl InitPCRs

    /*  OE & TS
    */
    lis r3,SIU_PCR68@h         /* First PCR for is PCR 68 */
    ori r3,r3,SIU_PCR68@l
    li r5,2                    /* Loop counter to get all PCR (68-69) -> 2 PCRs  */
    li r4, 0x0443              /* PCRs initialization value */
    bl InitPCRs
    
    /*  Configure the chip selects
        CS[0-3]
    */
    lis r3,SIU_PCR0@h          /* First PCR for is PCR 0 */
    ori r3,r3,SIU_PCR0@l
    li r5,4                    /* Loop counter to get all PCR (0-3) -> 4 PCRs  */
    li r4, 0x0443              /* PCRs initialization value */
    bl InitPCRs
    
    mtlr r27
    
    blr
}


/*----------------------------------------------------------------------------*/
/* Writing to MMU Table Entries                                               */
/*----------------------------------------------------------------------------*/

/* Write one MMU Table Entry:               */
/* r3, r4, r5 and r6 must hold              */
/* the values of MAS0, MAS1, MAS2 and MAS3  */
__asm void WriteMMUTableEntry( void )
{ 
nofralloc

    /* Write MMU Assist Register 0 (MAS0); SPR 624 */
    mtspr   624, r3
    /* Write MMU Assist Register 1 (MAS1); SPR 625 */
    mtspr   625, r4
    /* Write MMU Assist Register 2 (MAS2); SPR 626 */
    mtspr   626, r5
    /* Write MMU Assist Register 3 (MAS3); SPR 627 */
    mtspr   627, r6
    /* Write the table entry */
    tlbwe

    blr
}


#ifdef __cplusplus
}
#endif
