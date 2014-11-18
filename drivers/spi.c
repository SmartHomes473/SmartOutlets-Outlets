/*************************************************************
 *
 * SPI device driver for MSP430G2553.
 *
 * Author: Nick Jurgens <njurgens@umich.edu>
 *
 *************************************************************/

#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>

#include "msp430g2553.h"
#include "config.h"
#include "sys/interrupts.h"
#include "drivers/spi.h"
#include "drivers/usci.h"


// SPI IO pins
#define SPI_CLK BIT5
#define SPI_MISO BIT6
#define SPI_MOSI BIT7
#define SPI_SEL BIT0


/**
 * Initialize SPI on USCI_B0.
 */
void SPI_init ( uint8_t options )
{
	// Reset USCI_B0
	UCB0CTL1 = UCSWRST;

	// Configure IO pins for SPI mode
	P1SEL |= SPI_CLK | SPI_MISO | SPI_MOSI;
	P1SEL2 |= SPI_CLK | SPI_MISO | SPI_MOSI;
	P2SEL &= ~SPI_SEL;
	P2SEL2 &= ~SPI_SEL;

	// Slave select high
	P2OUT |= SPI_SEL;
	P2DIR |= SPI_SEL;

	// Configure USCI_B0 as an 8-bit 4-wire SPI master
	UCB0CTL0 |= UCCKPH | UCMSB | UCMST | UCSYNC;
	//UCB0CTL0 |= UCMSB | UCMST | UCSYNC;

	// Configure clock
	UCB0CTL1 |= UCSSEL_2;
	UCB0BR0 |= 0x08;
	UCB0BR1 |= 0x00;

	// Initialize USCI_B0 buffer
	disable_interrupts();
	USCI_B0->buffer_head = USCI_B0->buffer;
	USCI_B0->buffer_tail = USCI_B0->buffer;
	USCI_B0->data_ready = 0;
	enable_interrupts();

	// Enable USCI_B0;
	UCB0CTL1 &= ~UCSWRST;

	// Enable interrupts
	IE2 |= UCB0RXIE;
}


