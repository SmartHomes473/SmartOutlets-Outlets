/*************************************************************
 *
 * UART device driver for MSP430G2553.
 *
 * Author: Nick Jurgens <njurgens@umich.edu>
 *
 *************************************************************/

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "msp430g2553.h"
#include "config.h"
#include "sys/interrupts.h"
#include "drivers/uart.h"


/**
 * Initialize UART at 9600 BAUD on USCI_A0.
 */
void UART_init ( uint8_t options )
{
	uint8_t br0, br1, mctl;

	// Reset USCI_A0
	UCA0CTL1 = UCSWRST;

	// Configure IO pins for UART mode
	P1SEL = BIT1 | BIT2;
	P1SEL2 = BIT1 | BIT2;

	// http://mspgcc.sourceforge.net/baudrate.html
	switch (options) {
		case UART_600_BAUD:
			br0 = 104;
			br1 = 0;
			mctl = UCBRF0 | UCBRF1 | UCOS16;
			break;

		case UART_9600_BAUD:
		default:
			br0 = 104;
			br1 = 0;
			mctl = UCBRS0;
	}

	// Configure clock and UART modulation
	UCA0CTL1 |= UCSSEL_2;
	UCA0BR0 = br0;
	UCA0BR1 = br1;
	UCA0MCTL = mctl;

	// Initialize USCI_B0 buffer
	disable_interrupts();
	USCI_A0->buffer_head = USCI_A0->buffer;
	USCI_A0->buffer_tail = USCI_A0->buffer;
	USCI_A0->data_ready = 0;
	enable_interrupts();

	// Enable USCI_A0
	UCA0CTL1 &= ~UCSWRST;

	// Enable interrupts
	IE2 |= UCA0RXIE;
}
