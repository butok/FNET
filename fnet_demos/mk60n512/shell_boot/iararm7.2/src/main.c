
#include "fapp.h"

/********************************************************************/
int main (void)
{
    /* Init UART */
    fnet_cpu_serial_init(FNET_CFG_CPU_SERIAL_PORT_DEFAULT, 115200);

    fnet_cpu_irq_enable(0);
    
    /* Run application */
    fapp_main();
  
    return(0);
}