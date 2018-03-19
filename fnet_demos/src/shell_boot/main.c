
#include "fapp.h"

#if !FNET_CFG_CPU_VECTOR_TABLE_IS_IN_RAM
   #warning For the shell_boot application the vector table must be in RAM.
#endif

/********************************************************************/

int main (void)
{
    /* Board-specific HW initialization. Default serial port initialization. Interrupt enabling. */
    fapp_hw_init();

    /* Run application. */
    fapp_main();

    return(0);
}


