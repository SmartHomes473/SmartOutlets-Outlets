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

	// Start UART
	UART_init(UART_9600_BAUD);

	UART_send("hello!", 7);

	// Enable interrupts
	enable_interrupts();

	while(1) {
		uint8_t buff[26];

		UART_recv(buff, 26, '\n', UART_BLOCKING | UART_DELIM);

		UART_send(buff, 26);
	};
}

