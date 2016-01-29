
/* CFM init */

/* Each flash logical sector can be protected from program
 * and erase operations by setting the PROTECT[M] bit.
 */
#include "fapp_mem.h"

/* Sector Size.*/
#define FAPP_CFM_SECTOR_SIZE    (FNET_CFG_CPU_FLASH_SIZE/32) 

/* Number of sectors occupied by the application.*/
#define FAPP_APPLICATION_SECTOR_NUMBER    (FAPP_APPLICATION_SIZE/FAPP_CFM_SECTOR_SIZE+(FAPP_APPLICATION_SIZE%FAPP_CFM_SECTOR_SIZE?1:0))

/* CFM Protection register value.*/
#define FAPP_CFMPROT_REG        ((1<<FAPP_APPLICATION_SECTOR_NUMBER)-1)


#if FNET_CFG_COMP_CW
    #pragma define_section cfmconfig ".cfmconfig"  far_absolute R
    #pragma explicit_zero_data on
    __declspec(cfmconfig)
#endif
#if FNET_CFG_COMP_IAR
    #pragma segment="CFMCONFIG"
    #pragma location = "CFMCONFIG"
#endif
const unsigned long _cfm[6] =
{
    0, /*KEY_UPPER (0x00000400) */
    0, /*KEY_LOWER (0x00000404 */
#if FAPP_CFG_CFM_PROTECTION	
    FAPP_CFMPROT_REG,   /* CFMPROT (0x00000408) */
#else
    0, /* No protection. */
#endif	
    0, /*CFMSACC (0x0000040C) */
    0, /*CFMDACC (0x00000410) */
    0, /*CFMSEC (0x00000414) */
};