/*************************************************************
 *
 * USCI device driver for MSP430G2553.
 *
 * Author: Nick Jurgens <njurgens@umich.edu>
 *
 *************************************************************/

#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

#include "msp430g2553.h"
#include "config.h"
#include "sys/interrupts.h"
#include "drivers/usci.h"


// USCI instances
static usci_t __USCI_A0 = {
	.UCxx = (usci_mem_t*)&UCA0CTL0,
	.TXIFG = UCA0TXIFG
};
static usci_t __USCI_B0 = {
	.UCxx = (usci_mem_t*)&UCB0CTL0,
	.TXIFG = UCB0TXIFG
};


// Externally available pointers to USCI instances
usci_t *USCI_A0 = &__USCI_A0;
usci_t *USCI_B0 = &__USCI_B0;


// Macro to increment a pointer to the UART's circular buffer.
#define __USCI_buffer_incr_ptr(USCI, PTR)\
{\
	++PTR;\
	if (PTR == USCI->buffer + USCI_BUFFER_SIZE) {\
		PTR = USCI->buffer;\
	}\
}
#define USCI_buffer_incr_head(USCI) __USCI_buffer_incr_ptr(USCI, USCI->buffer_head)
#define USCI_buffer_incr_tail(USCI) __USCI_buffer_incr_ptr(USCI, USCI->buffer_tail)


/**
 * Send an array of bytes over USCI.
 *
 * @param data Pointer to array of bytes to send.
 * @param len Length of data array.
 */
void send ( usci_t *usci, const uint8_t *data, size_t len )
{
	while (len-- != 0) {
		// wait for USCI to be ready
		while (!(IFG2&usci->TXIFG));
		usci->UCxx->TXBUF = *data++;
	}
}


/**
 * Read from USCI.
 *
 * Reads up to some number of bytes or until a delimiter is received.
 *
 * TODO: document options
 *
 * @param buffer Pointer to buffer to read bytes into.
 * @param n Number of bytes to read.
 * @param delim Delimiter to watch for.
 * @param options Recv options
 *
 * @returns Bytes read.
 */
ssize_t recv ( usci_t *usci, uint8_t *buffer, size_t n, uint8_t delim, uint8_t options )
{
	bool is_blocking, break_on_delim;
	ssize_t index;

	// get options
	is_blocking = options & USCI_BLOCKING;
	break_on_delim = options & USCI_DELIM;

	index = 0;

	while (index < n) {

		// don't block if there isn't any data and we're non-blocking
		if (!is_blocking && !usci->data_ready) {
			break;
		}

		// otherwise, block while waiting for data
		while (!usci->data_ready);

		/*
		 * This is a fairly unsophisticated attempt at mutual exclusion.  We
		 * need to be sure that the buffer isn't being modified by an IRQ while
		 * we're reading from it.  Because there's only one thread of execution
		 * the best way to do this is to disable interrupts.
		 *
		 * FIXME: only disable interrupts for USCI_x0
		 */
		disable_interrupts();

		// read the byte at the head of the buffer
		buffer[index] = *usci->buffer_head;

		// incrememnt the head
		USCI_buffer_incr_head(usci);

		// clear SPI_read_ready if there is no data in the buffer
		if (usci->buffer_head == usci->buffer_tail) {
			usci->data_ready = 0;
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

/**
 * Blocks until TX is complete.
 */
void flush ( usci_t *usci )
{
	// wait for USCI to be inactive
	while (usci->UCxx->STAT&UCBUSY);
}


__attribute__((interrupt(USCIAB0RX_VECTOR)))
void USCI0RX_ISR(void)
{
	// USCI_A0 receive interrupt
	if (IFG2&UCA0RXIFG) {
		// write the data into the buffer
		*USCI_A0->buffer_tail = UCA0RXBUF;

		// Increment the tail
		USCI_buffer_incr_tail(USCI_A0);

		// Data is available to read
		USCI_A0->data_ready = 1;
	}

	// USCI_B0 receive interrupt
	if (IFG2&UCB0RXIFG) {
		// write the data into the buffer
		*USCI_B0->buffer_tail = UCB0RXBUF;

		// Increment the tail
		USCI_buffer_incr_tail(USCI_B0);

		// Data is available to read
		USCI_B0->data_ready = 1;
	}
}
