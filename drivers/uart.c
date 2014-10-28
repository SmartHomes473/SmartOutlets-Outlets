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
	// Reset USCI_A0
	UCA0CTL1 = UCSWRST;

	// Configure IO pins for UART mode
	P1SEL = BIT1 | BIT2;
	P1SEL2 = BIT1 | BIT2;

	// TODO: implement setting baud rate via options

	// Configure clock and UART modulation
	UCA0CTL1 |= UCSSEL_2;
	UCA0BR0 = 104;
	UCA0BR1 = 0;
	UCA0MCTL = UCBRS0;

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
