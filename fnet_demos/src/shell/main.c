
#include "fapp.h"

/********************************************************************/

int main (void)
{
    /* Init UART. */
    fnet_cpu_serial_init(FNET_CFG_CPU_SERIAL_PORT_DEFAULT, 115200u);

    /* Enable Interrupts.*/
    fnet_cpu_irq_enable(0u);

    /* Run application. */
    fapp_main();

    return(0);
}
