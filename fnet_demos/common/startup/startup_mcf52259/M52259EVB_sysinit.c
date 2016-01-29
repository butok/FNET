/*
 * File:    m52259evb_sysinit.c
 * Purpose: Reset configuration of the M52259EVB
 *
 * Notes:   
 */

#include "support_common.h"
#include "exceptions.h"

/********************************************************************/


/********************************************************************/

static void pll_init(void)
{

/*Required if booting with internal relaxation oscillator & pll off, clkmod[1:0]=00 & xtal=1 */

	MCF_CLOCK_OCLR = 0xC0;   //turn on crystal
	MCF_CLOCK_CCLR = 0x00;    //switch to crystal 
    MCF_CLOCK_OCHR = 0x00; //turn off relaxation osc

/*---------------------------------------------------------------------------------*/


	/* The PLL pre divider - 48MHz / 6 = 8MHz */
	MCF_CLOCK_CCHR =0x05;
	 
	 
	/* The PLL pre-divider affects this!!! 
	 * Multiply 48Mhz reference crystal /CCHR by 10 to acheive system clock of 80Mhz
	 */

	MCF_CLOCK_SYNCR &= ~(MCF_CLOCK_SYNCR_PLLEN);

    MCF_CLOCK_SYNCR |= MCF_CLOCK_SYNCR_CLKSRC | MCF_CLOCK_SYNCR_PLLMODE;
	
	MCF_CLOCK_SYNCR |= MCF_CLOCK_SYNCR_MFD(3) | MCF_CLOCK_SYNCR_RFD(0);
	
	MCF_CLOCK_SYNCR |= MCF_CLOCK_SYNCR_PLLEN;

	
	while (!(MCF_CLOCK_SYNSR & MCF_CLOCK_SYNSR_LOCK))
	{
	}
}

/********************************************************************/

static void gpio_init(void) {

    /* Enable the proper UART pins */
    switch (TERMINAL_PORT)
    {
        case 2:
            MCF_GPIO_PUCPAR = 0
                | MCF_GPIO_PUCPAR_URXD2_URXD2
                | MCF_GPIO_PUCPAR_UTXD2_UTXD2;
            break;
        case 1:
            MCF_GPIO_PUBPAR = 0
                | MCF_GPIO_PUBPAR_URXD1_URXD1
                | MCF_GPIO_PUBPAR_UTXD1_UTXD1;
            break;
        case 0:
        default:
            MCF_GPIO_PUAPAR = 0
                | MCF_GPIO_PUAPAR_URXD0_URXD0
                | MCF_GPIO_PUAPAR_UTXD0_UTXD0;
    }
  
}

/********************************************************************/
/* 
 * PLL min/max specifications
 */
#define MAX_FVCO    80000   /* KHz */
#define MAX_FSYS    80000   /* KHz */
#define MAX_FREF    10000   /* KHz */
#define MIN_FREF    2000    /* KHz */
#define MAX_MFD     18      /* Multiplier (not encoded) */
#define MIN_MFD     4       /* Multiplier (not encoded) */
#define MAX_RFD     128     /* Divider (not encoded) */
#define MIN_RFD     1       /* Divider (not encoded) */

/*
 * Low Power Divider specifications
 */
#define MIN_LPD     (1 << 0)    /* Divider (not encoded) */
#define MAX_LPD     (1 << 15)   /* Divider (not encoded) */

/* 
 * Operating Parameters
 * Pass to clock_pll() the 'flags' argument
 */
#define PLL_DISABLE   (0x0001)
#define PLL_CLKSRC    (0x0004)
#define PLL_FWKUP     (0x0020)
#define PLL_DISCLK    (0x0040)
#define PLL_LOCEN     (0x0080)
#define PLL_LOCRE     (0x0800)
#define PLL_LOLRE     (0x8000)
#define PLL_ALLFLAGS  (0x88E4)

/********************************************************************/
/*
 * Functions provided by this driver
 */
int
clock_pll (int, int, int);

int
clock_lpd (int);

/********************************************************************/


/********************************************************************/
/*
 * Initialize the Low Power Divider circuit
 * 
 * Parameters:
 *  div     Desired system frequency divider
 *
 * Return Value:
 *  The divider value applied
 *  -1 if divider was out of valid bounds
 */
int
clock_lpd (int div)
{

    int i, j;

    /* Check bounds of divider */
    if ((div < MIN_LPD) || (div > MAX_LPD))
        return (1 << (MCF_CLOCK_LPDR & 0x0F));

    /* Round divider down to nearest power of two */
    for (i = 0, j = div; j != 1; j >>= 1, i++) 
    {};
    
    /* Apply the divider to the system clock */
    MCF_CLOCK_LPDR = (uint8)MCF_CLOCK_LPDR_LPD(i);

    return (1 << i);
}
/********************************************************************/


/********************************************************************/
/* Initialize the PLL
 * 
 * Parameters:
 *  fref    PLL reference clock frequency in KHz
 *  fsys    Desired PLL output frequency in KHz
 *  flags   Operating parameters
 *
 * Return Value:
 *  The resulting output system frequency
 */
int
clock_pll (int fref, int fsys, int flags)
{
    int syncr, mfd_max, mfd_min, rfd_max;
    int i, temp, fout, mfd, rfd, done;
    
    /* Check for the disable flag */
    if (flags & PLL_DISABLE)
    {
        MCF_CLOCK_SYNCR &= ~MCF_CLOCK_SYNCR_PLLEN;
        return fref;
    }
    
    /* Check bounds of reference clock */
    if((fref >= MAX_FREF) || (fref <= MIN_FREF))
        return fref;
        
    if (fsys == 0)
    {
        /* Return current PLL output */
        if ((MCF_CLOCK_SYNCR & MCF_CLOCK_SYNCR_CLKSRC) &&
            (MCF_CLOCK_SYNCR & MCF_CLOCK_SYNCR_PLLEN))
        {
            mfd = (MCF_CLOCK_SYNCR & 0x7000) >> 12;
            mfd = MIN_MFD + (mfd * 2);
            rfd = (MCF_CLOCK_SYNCR & 0x0700) >> 8;
            rfd = 1 << rfd;
            return (fref * mfd / rfd);
        }
        else
            return fref;            
    }
    
    /* Check bounds of requested system clock */
    if (fsys > MAX_FSYS)
        fsys = MAX_FSYS;

    /* Determine maximum possible multiplier (must be even) */
    mfd_max = MAX_FVCO / fref;
    mfd_max &= ~1;
    if (mfd_max > MAX_MFD)
        mfd_max = MAX_MFD;
    
    /* Determine maximum possible output based on max multiplier */
    fout = fref * mfd_max;

    /* Determine target output based on fsys and max possible */ 
    if (fout > fsys)
        fout = fsys;
    
    /* Determine the minimum multiplier */
    for (mfd_min = mfd_max; 
         mfd_min > MIN_MFD && fout < (fref * mfd_min);)
         mfd_min-=2;
         
    /* Set preliminary divider maximum */         
    rfd_max = MAX_RFD;

    /*
     * Loop across the valid MFD and RFD settings starting with
     * the max MFD and min RFD and find the closest match less-than
     * or equal-to the desired output frequency
     */
    done = FALSE;
    for (mfd = mfd_max; mfd >= mfd_min; mfd-=2)
    {
        for (rfd = MIN_RFD; rfd < rfd_max; rfd <<= 1)
        {
            temp = fref * mfd / rfd;
            
            if (fout == temp)
            {
                /* exact match */
                done = TRUE;
                break;
            }
            if (fout > temp)
            {
                /* new upper bound for the rfd */
                rfd_max = rfd;
                /* is this the nearest match? */
                if ((rfd != 1) && ((fout - temp) < (fref / (rfd>>1))))
                    done = TRUE;
                break;
            }
        }
        if (done)
            break;
     }
    if (!done)
    {
        /* 
         * Fell out of loop before finding an exact match or getting 
         * as close as possible. Adjust mfd to nearest match.
         */
        mfd += 2;
    }
    
    fout = fref * mfd / rfd;

    /* Encode MFD and RFD settings */
    mfd = (mfd - MIN_MFD) / 2;
    for (i = 0; rfd > MIN_RFD; rfd >>= 1, i++) {};
    rfd = i;

    /*
     * Temp fix for PLL bug
     */
    (*(vuint8 *)(&__IPSBAR[0x120006])) |= 0x04;

    /* 
     * Initialize the PLL to generate the new system clock frequency 
     * A higher divider is used first with the desired MFD.  Once 
     * locked, the desired RFD is applied
     */
    syncr = MCF_CLOCK_SYNCR & ~(0x7700);
    temp = (MCF_CLOCK_SYNCR & 0x7000) >> 12;
    MCF_CLOCK_SYNCR = (uint16)(syncr 
        | MCF_CLOCK_SYNCR_RFD(rfd + 1)
        | MCF_CLOCK_SYNCR_MFD(temp)
        | MCF_CLOCK_SYNCR_PLLEN);
    MCF_CLOCK_SYNCR = (uint16)(syncr 
        | MCF_CLOCK_SYNCR_RFD(rfd + 1)
        | MCF_CLOCK_SYNCR_MFD(mfd)
        | MCF_CLOCK_SYNCR_PLLEN);
        
   	/* Wait for the PLL to lock */	
	while (!(MCF_CLOCK_SYNSR & MCF_CLOCK_SYNSR_LOCK)) {};

    /* Finish off the initialization */
    MCF_CLOCK_SYNCR = (uint16)(syncr
        | MCF_CLOCK_SYNCR_RFD(rfd)
        | MCF_CLOCK_SYNCR_MFD(mfd)
        | MCF_CLOCK_SYNCR_CLKSRC
        | MCF_CLOCK_SYNCR_PLLEN);

    return fout;
}

/********************************************************************/
static void 
scm_init(void)
{
	/*
	 * Enable on-chip modules to access internal SRAM
	 */
	MCF_SCM_RAMBAR = (0
		| MCF_SCM_RAMBAR_BA(RAMBAR_ADDRESS)
		| MCF_SCM_RAMBAR_BDE);

}

/********************************************************************/
/* Initialize                                                       */
/********************************************************************/

void __initialize_hardware(void)
{

	pll_init();

	gpio_init();
	scm_init(); //Added AB
	
	initialize_exceptions();

}
