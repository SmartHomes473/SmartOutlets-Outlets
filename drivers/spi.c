/*************************************************************
 *
 * SPI device driver for MSP430G2553.
 *
 * Author: Nick Jurgens <njurgens@umich.edu>
 *
 */

#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>

#include "msp430g2553.h"
#include "sys/interrupts.h"
#include "drivers/spi.h"

// SPI IO pins
#define SPI_CLK BIT5
#define SPI_MISO BIT6
#define SPI_MOSI BIT7


// SPI uses a circular buffer to store incomming bytes.
static volatile uint8_t  SPI_buffer[SPI_BUF_SIZE];
static volatile uint8_t *SPI_buffer_head = SPI_buffer;
static volatile uint8_t *SPI_buffer_tail = SPI_buffer;
static volatile uint8_t  SPI_buffer_data_ready = 0;


// Macro to write byte DATA over SPI
#define __SPI_send_byte(DATA)\
{\
	while (!(IFG2&UCB0TXIFG));\
	UCB0TXBUF = DATA;\
}


// Macros to increment a pointer to the SPI's circular buffer.
#define __SPI_buffer_incr_ptr(PTR)\
{\
	++PTR;\
	if (PTR == SPI_buffer + SPI_BUF_SIZE) {\
		PTR = SPI_buffer;\
	}\
}
#define SPI_buffer_incr_head() __SPI_buffer_incr_ptr(SPI_buffer_head)
#define SPI_buffer_incr_tail() __SPI_buffer_incr_ptr(SPI_buffer_tail)


/**
 * Initialize SPI on USCI_B0.
 */
void SPI_init ( )
{
	// Reset USCI_B0
	UCB0CTL1 = UCSWRST;

	// Configure IO pins for SPI mode
	P1SEL |= SPI_CLK | SPI_MISO | SPI_MOSI;
	P1SEL2 |= SPI_CLK | SPI_MISO | SPI_MOSI;

	// Configure USCI_B0 as an 8-bit 3-wire SPI master
	UCB0CTL0 |= UCCKPH | UCMSB | UCMST | UCSYNC;

	// Configure clock
	UCB0CTL1 |= UCSSEL_2;
	UCB0BR0 |= 0x02;
	UCB0BR1 |= 0x00;

	// Enable USCI_B1;
	UCB0CTL1 &= ~UCSWRST;
}


/**
 * Send an array of bytes over SPI.
 *
 * @param data Pointer to array of bytes to send.
 * @param len Length of data array.
 */
void SPI_send ( const uint8_t* data, size_t len )
{
	while (len-- != 0) __SPI_send_byte(*data++);
}


/**
 * Read from SPI.
 *
 * Reads up to some number of bytes or until a delimiter is received.
 *
 * @param buffer Pointer to buffer to read bytes into.
 * @param n Number of bytes to read.
 * @param delim Delimiter to watch for.
 *
 * @returns Bytes read.
 */
ssize_t SPI_recv ( uint8_t *buffer, size_t n, uint8_t delim, uint8_t options )
{
	ssize_t index;
	bool is_blocking, break_on_delim;

	// get options
	is_blocking = options & SPI_BLOCKING;
	break_on_delim = options & SPI_DELIM;

	index = 0;

	while (index < n) {
		// don't block if there isn't any data and we're non-blocking
		if (!is_blocking && !SPI_buffer_data_ready) {
			break;
		}

		// otherwise, block while waiting for data
		while (!SPI_buffer_data_ready);

		/*
		 * This is a fairly unsophisticated attempt at mutual exclusion.  We
		 * need to be sure that the buffer isn't being modified by an IRQ while
		 * we're reading from it.  Because there's only one thread of execution
		 * the best way to do this is to disable interrupts.
		 *
		 * FIXME: only disable interrupts for USCI_B0
		 */
		disable_interrupts();

		// read the byte at the head of the buffer
		buffer[index] = *SPI_buffer_head;

		// incrememnt the head
		SPI_buffer_incr_head();

		// clear SPI_read_ready if there is no data in the buffer
		if (SPI_buffer_head == SPI_buffer_tail) {
			SPI_buffer_data_ready = 0;
		}

		// re-enable interrupts
		enable_interrupts();

		// increment bytes read
		++index;

		// if we're looking for a delimiter and we find it, stop reading
		if (break_on_delim && buffer[index] == delim) {
			break;
		}
	}

	return index;
}
