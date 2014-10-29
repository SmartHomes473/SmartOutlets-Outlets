/*************************************************************
 *
 * SmartOutlets outlet firmware.
 *
 * Author: Nick Jurgens <njurgens@umich.edu>
 *
 *************************************************************/
#include <stdint.h>

#include "msp430g2553.h"
#include "sys/init.h"
#include "sys/interrupts.h"
#include "drivers/uart.h"

int main(void)
{
	// Initialize MSP430
	MSP430_init();

	// TODO: initialize all IO ports as outputs
	RELAY_init();

	// Start UART
	UART_init(UART_9600_BAUD);

	// Enable interrupts
	enable_interrupts();

	while(1) {
		uint8_t buff[1];

		int foo = UART_recv(buff, 1, '\0', USCI_NONE);

		if (foo > 0) {
			switch (buff[0]) {
				case 'i':
					RELAY_on();
					UART_send("ON", 2);
					break;

				case 'o':
					RELAY_off();
					UART_send("OFF", 3);
					break;
			}
		}
	};
}

