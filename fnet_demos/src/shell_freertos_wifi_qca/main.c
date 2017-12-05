
#include "fapp.h"

/********************************************************************/

int main (void)
{
#if FNET_MK /* Board specific initialization */
    #include "clock_config.h"
    /* Init clock to run mode */
    BOARD_BootClockRUN();
#endif

    /* Init UART. */
    fnet_cpu_serial_init(FNET_CFG_CPU_SERIAL_PORT_DEFAULT, 115200u);

    /* Enable Interrupts.*/
    fnet_cpu_irq_enable(0u);

    /* Run application. */
    fapp_main_freertos();

    return(0);
}
