#include "drivers/uart.h"
#include "sys/interrupts.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/*
 * UART uses a circular buffer to store incomming bytes.
 */
static volatile uint8_t UART_buffer[UART_BUF_SIZE];
static volatile uint8_t *UART_buffer_head = UART_buffer;
static volatile uint8_t *UART_buffer_tail = UART_buffer;
static volatile uint8_t UART_buffer_data_ready = 0;

/**
 * Macro to increment a pointer to the UART's circular buffer.
 */
#define __UART_buffer_incr_ptr(PTR)\
	++PTR;\
	if (PTR == UART_buffer + UART_BUF_SIZE) {\
		PTR = UART_buffer;\
	}

#define UART_buffer_incr_head() __UART_buffer_incr_ptr(UART_buffer_head)
#define UART_buffer_incr_tail() __UART_buffer_incr_ptr(UART_buffer_tail)


void UART_write_byte ( const uint8_t data )
{
	while (!(IFG2&UCA0TXIFG));
	UCA0TXBUF = data;
}

void UART_write_str ( const char *str )
{
	while (*str) UART_write_byte(*str++);
}

void UART_write_bytes (const uint8_t *data, size_t len )
{
	while (len-- != 0) UART_write_byte(*data++);
}

uint8_t UART_read_byte ( )
{
	uint8_t data;

	// wait for data to be read into the buffer
	while (!UART_buffer_data_ready);

	// disable interrupts so we can read from the UART buffer
	disable_interrupts();

	// read the byte at the head of the buffer
	data = *UART_buffer_head;

	// incrememnt the head
	UART_buffer_incr_head();

	// clear UART_read_ready if there is no data in the buffer
	if (UART_buffer_head == UART_buffer_tail) {
		UART_buffer_data_ready = 0;
	}

	// re-enable interrupts
	enable_interrupts();

	return data;
}

size_t UART_read ( uint8_t *buffer, size_t n, uint8_t delim )
{
	size_t index = 0;

	while (index < n) {
		buffer[index] = UART_read_byte();

		if (buffer[index] == delim) {
			break;
		}

		++index;
	}

	return index;
}

__attribute__((interrupt(USCIAB0RX_VECTOR)))
void USCI0RX_ISR(void)
{
	// write the data into the buffer
	*UART_buffer_tail = UCA0RXBUF;

	// Increment the tail
	UART_buffer_incr_tail();

	// Data is available to read
	UART_buffer_data_ready = 1;
}

