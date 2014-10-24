#include "drivers/uart.h"
#include "sys/interrupts.h"
#include <stddef.h>
#include <stdint.h>

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
	// TODO: implement me
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
