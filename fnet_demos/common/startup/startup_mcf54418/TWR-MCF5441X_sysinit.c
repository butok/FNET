 /*********************************************************************
 * File:    TWR-MCF5441X_sysinit.c
 * Purpose:		Power-on Reset configuration of the TWR-MCF5441X.
 ********************************************************************/


#include "support_common.h"
#include "exceptions.h"

#ifdef __cplusplus
#pragma cplusplus off
#endif // __cplusplus


/********************************************************************/
/********************************************************************/
static void fbcs_init (void)
{
    /* NAND Flash connected to FBCS0 */
    MCF_FBCS0_CSAR = FLASH0_ADDRESS;
    MCF_FBCS0_CSCR = (MCF_FBCS_CSCR_PS_16
                     | MCF_FBCS_CSCR_BEM
                     | MCF_FBCS_CSCR_AA
                     | MCF_FBCS_CSCR_WS(0x1F));
    MCF_FBCS0_CSMR = (MCF_FBCS_CSMR_BAM_256M
                     | MCF_FBCS_CSMR_V);

}
/********************************************************************/
static void sdramc_init (void)
{
    uint32 myctr; 		//generic counter variable

    /* Enable clocks for DDR controller (slot number #46) */
    MCF_PMM_PPMCR0 = 0UL
        | SDRAM_PMM_SLOT_NUMBER;

    /* Configure DDR2 pins for full strength 1.8V DDR2 */
    MCF_PAD_MSCR_SDRAMC = 0UL 
        | MCF_PAD_MSCR_SDRAMC_MSC_FS_DDR2;

	/* Disable RCR */
    MCF_DDR_RCR_CTRL = 0UL;
    /* Force RCR soft reset */
	MCF_DDR_RCR_CTRL = 0UL
	    | MCF_DDR_RCR_CTRL_RCR_RST;

    /* DDR IO pad control */
    MCF_DDR_PADCR = 0x0200UL
	    | MCF_DDR_PADCR_SPARE_DLY_CTRL1_HIGH
        | MCF_DDR_PADCR_PAD_ODT_CS_75_OHM
        | MCF_DDR_PADCR_OFFSET_VALUE_700_350_MV;

    /* Initialize DDR controller */
    MCF_DDR_CR00 = 0UL
        | MCF_DDR_CR00_AGE_EN
        | MCF_DDR_CR00_ADDCOL_EN
        | MCF_DDR_CR00_RP0_LOW
        | MCF_DDR_CR00_WP0_LOW;        

    MCF_DDR_CR01 = 0UL
        | MCF_DDR_CR01_RP1_LOW
        | MCF_DDR_CR01_WP1_LOW;

    MCF_DDR_CR02 = 0UL
        | MCF_DDR_CR02_CCAPEN_EN
        | MCF_DDR_CR02_BIGEND_BIG
        | MCF_DDR_CR02_BNKSPT_EN;

    MCF_DDR_CR03 = 0UL
        | MCF_DDR_CR03_DQSBEN_DIFF
        | MCF_DDR_CR03_DLLBYP;

    MCF_DDR_CR04 = 0UL
        | MCF_DDR_CR04_QKREF_INTERRUPT
        | MCF_DDR_CR04_8BNK_8_BANKS
        | MCF_DDR_CR04_DRVDQS_DRIVE;

    MCF_DDR_CR06 = 0UL
        | MCF_DDR_CR06_ODTEN
        | MCF_DDR_CR06_TACEN;

    MCF_DDR_CR07 = 0UL
        | MCF_DDR_CR07_PLEN_EN;

    MCF_DDR_CR08 = 0UL
        | MCF_DDR_CR08_REDUC_8BIT
        | MCF_DDR_CR08_PRIEN_EN;

    MCF_DDR_CR09 = 0UL
        | MCF_DDR_CR09_RWEN_EN;

    MCF_DDR_CR10 = 0UL
        | MCF_DDR_CR10_TREFEN_EN
        | MCF_DDR_CR10_SWPEN_EN;

    MCF_DDR_CR11 = 0UL
        | MCF_DDR_CR11_P0TYP_1_TO_2;

    MCF_DDR_CR12 = 0UL
        | MCF_DDR_CR12_P1TYP_1_TO_2
        | MCF_DDR_CR12_CSMAP;

    MCF_DDR_CR14 = 0UL
        | MCF_DDR_CR14_ODTWRCS;

    MCF_DDR_CR15 = 0UL
        | MCF_DDR_CR15_ODTRES_75_OHM
        | MCF_DDR_CR15_ADDPINS( SDRAM_ROWS_MAX - SDRAM_ROWS_USED );

    MCF_DDR_CR16 = 0UL
        | MCF_DDR_CR16_COLSIZ( SDRAM_COLS_MAX - SDRAM_COLS_USED )
        | MCF_DDR_CR16_CASLAT( SDRAM_CL )
        | MCF_DDR_CR16_AGECNT( SDRAM_CMD_AGEING );

    MCF_DDR_CR17 = 0UL
        | MCF_DDR_CR17_DFICTLDLY( SDRAM_DFIDLY )
        | MCF_DDR_CR17_CKEPW( SDRAM_CKEPW )
        | MCF_DDR_CR17_CMDAGE( SDRAM_CMD_AGEING );

    MCF_DDR_CR18 = 0UL
        | MCF_DDR_CR18_TRRD( SDRAM_TRRD )
        | MCF_DDR_CR18_DFICLKEN( SDRAM_DFICLKEN );

    MCF_DDR_CR19 = 0UL
        | MCF_DDR_CR19_APREBIT( SDRAM_COLS_USED )
        | MCF_DDR_CR19_WRLAT( SDRAM_WL )
        | MCF_DDR_CR19_TWTR( SDRAM_TWTR )
        | MCF_DDR_CR19_TRTP( SDRAM_TRTP );

    MCF_DDR_CR20 = 0UL
        | MCF_DDR_CR20_AREFINIT( SDRAM_AREFC )
        | MCF_DDR_CR20_LATGATE( (2*SDRAM_CL) - 1 )
        | MCF_DDR_CR20_LATLIN( 2*SDRAM_CL );

    MCF_DDR_CR21 = 0UL
        | MCF_DDR_CR21_PHYRDLAT( SDRAM_PHYRL ) 
        | MCF_DDR_CR21_TDAL( SDRAM_TWR + SDRAM_TRP );

    MCF_DDR_CR22 = 0UL
        | MCF_DDR_CR22_TRP( SDRAM_TRP )
        | MCF_DDR_CR22_RDDATABASE( SDRAM_DFIRDBASE )
        | MCF_DDR_CR22_PHYWRLATBASE( SDRAM_DFIWRBASE );

    MCF_DDR_CR23 = 0UL
        | MCF_DDR_CR23_TWR( SDRAM_TWR );

    MCF_DDR_CR24 = 0UL
        | MCF_DDR_CR24_TMRD( SDRAM_TMRD )
        | MCF_DDR_CR24_TFAW( SDRAM_TFAW );

    MCF_DDR_CR25 = 0UL
        | MCF_DDR_CR25_TRASMIN( SDRAM_TRASMIN )
        | MCF_DDR_CR25_TRC( SDRAM_TRC );

    MCF_DDR_CR26 = 0UL
        | MCF_DDR_CR26_TRFC( SDRAM_TRFC )
        | MCF_DDR_CR26_TRCD_INT( SDRAM_TRCD );

    MCF_DDR_CR28 = 0UL
        | MCF_DDR_CR28_P0WRCNT( 0x10U )
        | MCF_DDR_CR28_P0RDCNT( 0x10U );

    MCF_DDR_CR29 = 0UL
        | MCF_DDR_CR29_P1WRCNT( 0x10U )
        | MCF_DDR_CR29_P1RDCNT( 0x10U );

    MCF_DDR_CR31 = 0UL
        | MCF_DDR_CR31_TREF( SDRAM_TREFI );

    MCF_DDR_CR41 = 0UL
        | MCF_DDR_CR41_TCPD( SDRAM_TCPD );

    MCF_DDR_CR42 = 0UL
        | MCF_DDR_CR42_TRASMAX( SDRAM_TRAS_MAX )
        | MCF_DDR_CR42_TPDEX( SDRAM_TPDEX );

    MCF_DDR_CR43 = 0UL
        | MCF_DDR_CR43_TXSR( SDRAM_TXSR )
        | MCF_DDR_CR43_TXSNR( SDRAM_TXSNR );

    MCF_DDR_CR45 = 0UL
        | MCF_DDR_CR45_TINIT( SDRAM_INIT_CYCLES );

    MCF_DDR_CR56 = 0UL
        | MCF_DDR_CR56_DRAMCLASS_DDR2;

    MCF_DDR_CR57 = 0UL
        | MCF_DDR_CR57_TMOD( SDRAM_TMOD )
        | MCF_DDR_CR57_WRLATADJ( SDRAM_WL )
        | MCF_DDR_CR57_RDLATADJ( SDRAM_CL );

    MCF_DDR_CR58 = 0UL
        | MCF_DDR_CR58_EMRS1D( 0x4004U );

    MCF_DDR_CR59 = 0UL
        | MCF_DDR_CR59_EMRS3D( 0xC000U )
		| MCF_DDR_CR59_EMRS1D_B( 0x4004U );

    MCF_DDR_CR60 = 0UL
        | MCF_DDR_CR60_MRSD( 0x0642U )
		| MCF_DDR_CR60_EMRS3D_B( 0xC000U );

    //Start Configuration
    MCF_DDR_CR09 = 0 
        | MCF_DDR_CR09_START
        | MCF_DDR_CR09_RWEN;

    //DELAY, Wait a bit
    for (myctr=200000; myctr >0; myctr--)
    {
    }

}/********************************************************************/



/********************************************************************/
void __initialize_hardware(void)
{
	/*******************************************************
	*	Out of reset, the low-level assembly code calls this 
	*	routine to initialize the derivative modules for the  
	*	board. 
	********************************************************/

//DM	gpio_init();
	fbcs_init();
	sdramc_init();
    
	/* enabling peripheral clocks */
	MCF_PMM_PPMLR0 &= ~(MCF_PMM_PPMLR0_CD18);	/* ICTRL0 */
	MCF_PMM_PPMLR0 &= ~(MCF_PMM_PPMLR0_CD19);   /* ICTRL1 */
    MCF_PMM_PPMLR0 &= ~(MCF_PMM_PPMLR0_CD20);   /* ICTRL2 */

	initialize_exceptions();
}
