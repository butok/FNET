/*
 * File:    TWR-MCF5441X_sysinit.h
 * Purpose:		Generic Power-on Reset configuration
 *
 * Notes:
 *
 */

#ifndef __TWR_MCF5441X_SYSINIT_H__
#define __TWR_MCF5441X_SYSINIT_H__

#ifdef __cplusplus
extern "C" {
#endif


/***
 * System Bus Clock Info
 */
#define SYSTEM_CLOCK_KHZ  125000   /* system bus frequency in kHz */
#define SYSTEM_PERIOD     8        /* system bus period in ns */


/*
 * SDRAM Timing Parameters from DDR2 Specs
 */
#define SDRAM_CL          4         /*!< DDR2 CAS Latency */
#define SDRAM_WL          3         /*!< DDR2 Write Latency */
#define SDRAM_AL          1         /*!< DDR2 Additive Latency */
#define SDRAM_TWTR        2         /*!< DDR2 Write to Read interval (tWTR) in cycles */
#define SDRAM_TRTP        3         /*!< DDR2 Read to Precharge interval (tRTP) in cycles */
#define SDRAM_TRC         14        /*!< DDR2 Active to Active delay for the same bank (tRC) in cycles */
#define SDRAM_TRRD        2         /*!< DDR2 Active to Active delay for different banks (tRRD) in cycles */
#define SDRAM_TRCD (2*16/SYSTEM_PERIOD) /*!< DDR2 RAS to CAS delay (tRCD) in cycles */
#define SDRAM_TRP  (2*16/SYSTEM_PERIOD) /*!< DDR2 Precharge command period (tRP) in cycles */
#define SDRAM_TWR  (2*16/SYSTEM_PERIOD) /*!< DDR2 Write recovery time (tWR) in cycles */
#define SDRAM_TMRD   (16/SYSTEM_PERIOD) /*!< DDR2 Load Mode register command delay (tMRD) in cycles */
#define SDRAM_TFAW        10        /*!< DDR2 Four-bank Activation Window (tFAW) in cycles */
#define SDRAM_TRASMIN     12        /*!< DDR2 Minimum Row Active time in cycles */
#define SDRAM_TRFC        32        /*!< DDR2 Refresh command time (tRFC) in cycles */
#define SDRAM_TREFI       1945      /*!< DDR2 Between refresh commands interval in cycles */
#define SDRAM_AREFC       2         /*!< DDR2 Number of auto-refresh commands to execute during init */
#define SDRAM_TCPD        100       /*!< DDR2 Clock enable to Precharge delay time (tCPD) in cycles */
#define SDRAM_TXSR        200       /*!< DDR2 Self-refresh exit time (tXSR) in cycles */
#define SDRAM_TXSNR       35        /*!< DDR2 Self-refresh exit time (tXSNR) in cycles */
#define SDRAM_CKEPW       3         /*!< DDR2 Minimum CKE pulse width in cycles */
#define SDRAM_TMOD        3         /*!< DDR2 Wait time between mode commands (tMOD) in cycles */
#define SDRAM_INIT_CYCLES 50000     /*!< DDR2 Initialization time in cycles */
#define SDRAM_TRAS_MAX    17490     /*!< DDR2 Max row active time in cycles */
#define SDRAM_TPDEX       2         /*!< DDR2 Power-down exit command period in cycles */
#define SDRAM_CMD_AGEING  7         /*!< DDR2 Command ageing initial value */
#define SDRAM_PHYRL       5         /*!< DDR2 PHY Read Latency timing parameter */
#define SDRAM_DFIDLY      2         /*!< DDR2 DFI tCTRL_DELAY timing parameter */
#define SDRAM_DFICLKEN    1         /*!< DDR2 DFI tDRAM_CLK_ENABLE timing parameter */
#define SDRAM_DFIRDBASE   3         /*!< DDR2 DFI base value for the tRDDATA_EN timing parameter */
#define SDRAM_DFIWRBASE   2         /*!< DDR2 DFI base value for the tPHY_WRLAT timing parameter */
/*
 *  SDRAM Parameters used for pins setting
 */
#define SDRAM_ROWS_MAX    16        /* Max row pins */
#define SDRAM_ROWS_USED   14        /* Row pins used */
#define SDRAM_COLS_MAX    12        /* Max collumn pins */
#define SDRAM_COLS_USED   10        /* Collumn pins used */

#define SDRAM_PMM_SLOT_NUMBER 46    /* Power Management slot number */


/***
 * Board Memory map definitions from linker command files:
 * __SDRAM,__SDRAM_SIZE, __FLASH, __FLASH_SIZE linker
 * symbols must be defined in the linker command file.
 */
extern uint8  __FLASH0_ADDRESS[];
extern uint8  __FLASH0_SIZE[];
extern uint8  __FLASH1_ADDRESS[];
extern uint8  __FLASH1_SIZE[];

extern uint8  __SDRAM_ADDRESS[];
extern uint8  __SDRAM_ADDRESS_BANK0[];
extern uint8  __SDRAM_ADDRESS_BANK1[];
extern uint8  __SDRAM_SIZE[];


#define FLASH0_ADDRESS      (uint32)__FLASH0_ADDRESS
#define FLASH0_SIZE	        (uint32)__FLASH0_SIZE
#define FLASH1_ADDRESS      (uint32)__FLASH1_ADDRESS
#define FLASH1_SIZE	        (uint32)__FLASH1_SIZE
#define SDRAM_ADDRESS		(uint32)__SDRAM_ADDRESS
#define SDRAM_ADDRESS_BANK0	(uint32)__SDRAM_ADDRESS_BANK0
#define SDRAM_ADDRESS_BANK1	(uint32)__SDRAM_ADDRESS_BANK1
#define SDRAM_SIZE			(uint32)__SDRAM_SIZE


/********************************************************************/
/* __initialize_hardware Startup code routine
 *
 * __initialize_hardware is called by the startup code right after reset,
 * with interrupt disabled and SP pre-set to a valid memory area.
 * Here you should initialize memory and some peripherics;
 * at this point global variables are not initialized yet.
 * The startup code will initialize SP on return of this function.
 */
void __initialize_hardware(void);

/********************************************************************/
/* __initialize_system Startup code routine
 *
 * __initialize_system is called by the startup code when all languages
 * specific initialization are done to allow additional hardware setup.
 */
void __initialize_system(void);


void gpio_init(void);
void fbcs_init(void);
void sdramc_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __TWR_MCF5441X_SYSINIT_H__ */

