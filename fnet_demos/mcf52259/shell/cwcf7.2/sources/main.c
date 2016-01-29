
/*
 * File:		main.c
 * Purpose:		sample program
 *
 */
#include "fapp.h"


extern int /* Just to avoid ANSI Strict error.*/ main(void) 
{
    /* Init UART */
	fnet_cpu_serial_init(FNET_CFG_CPU_SERIAL_PORT_DEFAULT, 115200);
	
	/* Enable interrupts */
	fnet_cpu_irq_enable(0);

    /* Run application */
	fapp_main();
}
