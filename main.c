//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.2|-->LED
//
//  J. Stevenson
//  Texas Instruments, Inc
//  July 2011
//  Built with Code Composer Studio v5
//***************************************************************************************

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
	UART_init_9600();

	UART_write_str("hello!\n");

	// Enable interrupts
	enable_interrupts();

	while(1) {
		uint8_t buff[26];

		UART_read(buff, 26, '\n');

		UART_write_bytes(buff, 26);
	};
}

