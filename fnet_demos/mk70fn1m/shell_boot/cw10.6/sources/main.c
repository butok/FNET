/*
 * File:		hello_world.c
 * Purpose:		Main process
 *
 */

//#include "common.h"

#include "fapp.h"

/********************************************************************/
void main(void)
{
	
    /* Initialize UART */
	fnet_cpu_serial_init(FNET_CFG_CPU_SERIAL_PORT_DEFAULT, 115200);

	/* Enables interrupts.*/
    fnet_cpu_irq_enable(0);

    /* Run application */
    fapp_main();
}


