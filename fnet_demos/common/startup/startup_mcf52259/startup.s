
#include    "fnet_comp_asm.h"

#define SUPPORT_ROM_TO_RAM  1


FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(__SP_AFTER_RESET)
FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(__RAMBAR)
FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(__initialize_hardware)
FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(_SP_INIT)
FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(_SDA_BASE)
FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(_END_BSS)
FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(_START_BSS)
FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(clear_mem)
FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(_END_SBSS)
FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(_START_SBSS)

FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(_S_romp)
FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(__copy_rom_sections_to_ram)
FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(__copy_rom_section)

FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(main)
FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(__IPSBAR)
FNET_COMP_ASM_EXTERN    FNET_COMP_ASM_PREFIX(__FLASHBAR)



#if FNET_CFG_COMP_CW
    #define ENTRY_POINT __startup
#endif /* FNET_CFG_COMP_CW */

#if FNET_CFG_COMP_IAR
    #define ENTRY_POINT __program_start

   
        RSEG    CSTACK:DATA(2)
        RSEG    NEAR_Z:DATA(0)
        RSEG    NEARPID_Z:DATA(0)
        RSEG    FAR_Z:DATA(0)
        RSEG    NEAR_I:DATA(0)
        RSEG    NEARPID_I:DATA(0)
        RSEG    FAR_I:DATA(0)
        RSEG    NEAR_ID:DATA(0)
        RSEG    NEARPID_ID:DATA(0)
        RSEG    FAR_ID:DATA(0)
        RSEG    PIDBASE:DATA(2)
        RSEG    RCODE:CODE(1)
        
        RSEG    FNET_RAMFUNC:CODE(1)
        RSEG    FNET_RAMFUNC_FLASH:CODE(1)

        PUBLIC  ?CLCF_V1_L00
?CLCF_V1_L00    EQU     1 /* Dummy */

        PUBLIC  ?CSTART_DEVICE
        PUBLIC  __program_start

#endif /* FNET_CFG_COMP_IAR */   



FNET_COMP_ASM_GLOBAL    ENTRY_POINT


FNET_COMP_ASM_CODE

/******************************************************************
 *    Startup routine for embedded application ...
 ******************************************************************/
#if FNET_CFG_COMP_IAR
?CSTART_DEVICE:
#endif
ENTRY_POINT:
	/* disable interrupts */
    move.w        #0x2700,sr

	/* Pre-init SP, in case memory for stack is not valid it should be setup using 
	   MEMORY_INIT before __initialize_hardware is called 
	*/
	movea.l         #FNET_COMP_ASM_PREFIX(__SP_AFTER_RESET), sp

;------------------------------------------------------------------------------
;       Device configuration.
;------------------------------------------------------------------------------
 
    /* Initialize memory */
    /* Initialize RAMBAR: locate SRAM and validate it */ 

	move.l	#FNET_COMP_ASM_PREFIX(__RAMBAR),d0
	add.l   #0x21, d0 

    movec.l	d0,RAMBAR
  
    /* Initialize IPSBAR */
    move.l  #FNET_COMP_ASM_PREFIX(__IPSBAR),d0

    andi.l  #0xC0000000,d0 /* need to mask */
 
    add.l   #0x1,d0

    move.l  d0,(0x40000000)

    /* Initialize FLASHBAR */
    move.l  #FNET_COMP_ASM_PREFIX(__FLASHBAR),d0
    andi.l  #0xFFF80000,d0 /* need to mask */
    add.l   #0x61,d0

    movec.l   d0,FLASHBAR
    
	/* initialize any hardware specific issues */
    jsr           (FNET_COMP_ASM_PREFIX(__initialize_hardware))   
  
	/* setup the stack pointer */
    movea.l         #FNET_COMP_ASM_PREFIX(_SP_INIT), sp


/*------------------------------------------------------------------------------
 *      Segment initialization
 *------------------------------------------------------------------------------*/
#if FNET_CFG_COMP_CW
	/* setup A5 */
    lea           FNET_COMP_ASM_PREFIX(_SDA_BASE),a5


	/* zero initialize the .bss section */

    lea           FNET_COMP_ASM_PREFIX(_END_BSS), a0
    lea           FNET_COMP_ASM_PREFIX(_START_BSS), a1
    suba.l        a1, a0
    move.l        a0, d0

    beq           __skip_bss__

    lea           FNET_COMP_ASM_PREFIX(_START_BSS), a0

    /* call clear_mem with base pointer in a0 and size in d0 */
    jsr           (FNET_COMP_ASM_PREFIX(clear_mem))

__skip_bss__:

	/* zero initialize the .sbss section */

    lea           FNET_COMP_ASM_PREFIX(_END_SBSS), a0
    lea           FNET_COMP_ASM_PREFIX(_START_SBSS), a1
    suba.l        a1, a0
    move.l        a0, d0

    beq           __skip_sbss__

    lea           FNET_COMP_ASM_PREFIX(_START_SBSS), a0

    /* call clear_mem with base pointer in a0 and size in d0 */
    jsr           (FNET_COMP_ASM_PREFIX(clear_mem))

__skip_sbss__:

	/* copy all ROM sections to their RAM locations ... */
    #if SUPPORT_ROM_TO_RAM

    	/*
    	 * _S_romp is a null terminated array of
    	 * typedef struct RomInfo {
         *      unsigned long	Source;
         *      unsigned long	Target;
         *      unsigned long 	Size;
         *  } RomInfo;
         *
         * Watch out if you're rebasing using _PICPID_DELTA
         */

        lea           FNET_COMP_ASM_PREFIX(_S_romp), a0
        move.l        a0, d0
        beq           __skip_rom_copy__            
        jsr           (FNET_COMP_ASM_PREFIX(__copy_rom_sections_to_ram))

    #else

    	/*
       * There's a single block to copy from ROM to RAM, perform
       * the copy directly without using the __S_romp structure
       */

        lea           FNET_COMP_ASM_PREFIX(_DATA_RAM), a0
        lea           FNET_COMP_ASM_PREFIX(_DATA_ROM), a1
        
        cmpa          a0,a1
        beq           __skip_rom_copy__
                  
        move.l        #FNET_COMP_ASM_PREFIX(_DATA_END), d0
        sub.l         a0, d0
                      
        jsr           (FNET_COMP_ASM_PREFIX(__copy_rom_section))

    #endif
__skip_rom_copy__:

#endif /* FNET_CFG_COMP_CW */

/*------------------------------------------------------------------------------
 *      Segment initialization
 *------------------------------------------------------------------------------*/
#if FNET_CFG_COMP_IAR
        MOVE.L  #sizeof(NEAR_Z), D0
        BEQ.B   NO_NEAR_Z
        MOVEA.L #SFB(NEAR_Z), A0
NZ_LOOP:
        CLR.B   (A0)+
        SUBQ    #1, D0
        BNE.B   NZ_LOOP
NO_NEAR_Z:

        MOVE.L  #sizeof(NEARPID_Z), D0
        BEQ.B   NO_NEARPID_Z
        MOVEA.L #SFB(NEARPID_Z), A0
NPZ_LOOP:
        CLR.B   (A0)+
        SUBQ    #1, D0
        BNE.B   NPZ_LOOP
NO_NEARPID_Z:

        MOVE.L  #sizeof(FAR_Z), D0
        BEQ.B   NO_FAR_Z
        MOVEA.L #SFB(FAR_Z), A0
FZ_LOOP:
        CLR.B   (A0)+
        SUBQ    #1, D0
        BNE.B   FZ_LOOP
NO_FAR_Z:

        MOVE.L  #sizeof(NEAR_I), D0
        BEQ.B   NO_NEAR_I
        MOVEA.L #SFB(NEAR_I), A0
        MOVEA.L #SFB(NEAR_ID), A1
NI_LOOP:
        MOVE.B  (A1)+, (A0)+
        SUBQ    #1, D0
        BNE.B   NI_LOOP
NO_NEAR_I:

        MOVE.L  #sizeof(NEARPID_I), D0
        BEQ.B   NO_NEARPID_I
        MOVEA.L #SFB(NEARPID_I), A0
        MOVEA.L #SFB(NEARPID_ID), A1
NPI_LOOP:
        MOVE.B  (A1)+, (A0)+
        SUBQ    #1, D0
        BNE.B   NPI_LOOP
NO_NEARPID_I:

        MOVE.L  #sizeof(FAR_I), D0
        BEQ.B   NO_FAR_I
        MOVEA.L #SFB(FAR_I), A0
        MOVEA.L #SFB(FAR_ID), A1
FI_LOOP:
        MOVE.B  (A1)+, (A0)+
        SUBQ    #1, D0
        BNE.B   FI_LOOP
NO_FAR_I:

/* FNET RAM function(s) copy.*/ 
        MOVE.L  #sizeof(FNET_RAMFUNC), D0
        BEQ.B   NO_FNET_RAMFUNC
        MOVEA.L #SFB(FNET_RAMFUNC), A0
        MOVEA.L #SFB(FNET_RAMFUNC_FLASH), A1
FNET_RAMFUNC_LOOP:
        MOVE.B  (A1)+, (A0)+
        SUBQ    #1, D0
        BNE.B   FNET_RAMFUNC_LOOP
NO_FNET_RAMFUNC:

#endif /* FNET_CFG_COMP_IAR */

/*------------------------------------------------------------------------------
 *      Call the actual "main" function
 *------------------------------------------------------------------------------*/	
	jsr			  (FNET_COMP_ASM_PREFIX(main))

	/* should never reach here but just in case */
	FNET_COMP_ASM_DC16 0x4afc ;illegal


FNET_COMP_ASM_END


