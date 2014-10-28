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


#define UART_TX BIT1
#define UART_RX BIT2


// Macro to write a byte to UART
#define __UART_write_byte(DATA)\
{\
	while (!(IFG2&UCA0TXIFG));\
	UCA0TXBUF = DATA;\
}


// Macro to increment a pointer to the UART's circular buffer.
#define __UART_buffer_incr_ptr(PTR)\
{\
	++PTR;\
	if (PTR == UART_buffer + UART_BUF_SIZE) {\
		PTR = UART_buffer;\
	}\
}
#define UART_buffer_incr_head() __UART_buffer_incr_ptr(UART_buffer_head)
#define UART_buffer_incr_tail() __UART_buffer_incr_ptr(UART_buffer_tail)


/*
 * UART uses a circular buffer to store incomming bytes.
 */
static volatile uint8_t  UART_buffer[UART_BUF_SIZE];
static volatile uint8_t *UART_buffer_head = UART_buffer;
static volatile uint8_t *UART_buffer_tail = UART_buffer;
static volatile uint8_t  UART_buffer_data_ready = 0;


/**
 * Initialize UART at 9600 BAUD on USCI_B0.
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

	// Enable USCI_A0
	UCA0CTL1 &= ~UCSWRST;

	// Enable interrupts
	IE2 |= UCA0RXIE;
}


/**
 * Send an array of bytes over UART.
 *
 * @param data Pointer to array of bytes to send.
 * @param len Length of data array.
 */
void UART_send (const uint8_t *data, size_t len )
{
	while (len-- != 0) __UART_write_byte(*data++);
}


/**
 * Read from UART.
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
ssize_t UART_recv ( uint8_t *buffer, size_t n, uint8_t delim, uint8_t options )
{
	bool is_blocking, break_on_delim;
	ssize_t index;

	// get options
	is_blocking = options & UART_BLOCKING;
	break_on_delim = options & UART_DELIM;

	index = 0;

	while (index < n) {

		// don't block if there isn't any data and we're non-blocking
		if (!is_blocking && !UART_buffer_data_ready) {
			break;
		}

		// otherwise, block while waiting for data
		while (!UART_buffer_data_ready);

		/*
		 * This is a fairly unsophisticated attempt at mutual exclusion.  We
		 * need to be sure that the buffer isn't being modified by an IRQ while
		 * we're reading from it.  Because there's only one thread of execution
		 * the best way to do this is to disable interrupts.
		 *
		 * FIXME: only disable interrupts for USCI_A0
		 */
		disable_interrupts();

		// read the byte at the head of the buffer
		buffer[index] = *UART_buffer_head;

		// incrememnt the head
		UART_buffer_incr_head();

		// clear SPI_read_ready if there is no data in the buffer
		if (UART_buffer_head == UART_buffer_tail) {
			UART_buffer_data_ready = 0;
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


__attribute__((interrupt(USCIAB0RX_VECTOR)))
void USCI0RX_ISR(void)
{
	// FIXME: needs to work for both SPI and UART

	// write the data into the buffer
	*UART_buffer_tail = UCA0RXBUF;

	// Increment the tail
	UART_buffer_incr_tail();

	// Data is available to read
	UART_buffer_data_ready = 1;
}
