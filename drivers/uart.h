#ifndef _OUTLET_UART_H
#define _OUTLET_UART_H

#include "msp430g2553.h"
#include <stddef.h>
#include <stdint.h>

#define UART_TX BIT1
#define UART_RX BIT2

// The UART buffer is 16 bytes
#define UART_BUF_SIZE 16

/**
 * Macro to initialize UART at 9600 BAUD.
 */
#define UART_init_9600() \
    P1SEL = BIT1 | BIT2; \
    P1SEL2 = BIT1 | BIT2; \
    UCA0CTL1 = UCSWRST; \
    UCA0CTL1 |= UCSSEL_2; \
    UCA0BR0 = 104; \
    UCA0BR1 = 0; \
    UCA0MCTL = UCBRS0; \
    UCA0CTL1 &= ~UCSWRST; \
    IE2 |= UCA0RXIE;

/**
 * Send a byte over UART.
 *
 * @param data Byte to send.
 */
void UART_write_byte ( const uint8_t data );

/**
 * Send a string over UART.
 *
 * @param str NULL terminated string to send.
 */
void UART_write_str ( const char* str );

/**
 * Send an array of bytes over UART.
 *
 * @param data Pointer to array of bytes to send.
 * @param len Length of data array.
 */
void UART_write_bytes ( const uint8_t* data, size_t len );

/**
 * Read a byte from the UART buffer.
 *
 * @returns A byte.
 */
uint8_t UART_read_byte ( );

/**
 * Read from UART.
 *
 * Reads up to some number of bytes or until a delimiter is received.
 *
 * @param buffer Pointer to buffer to read bytes into.
 * @param n Number of bytes to read.
 * @param delim Delimiter to watch for.
 *
 * @returns Bytes read.
 */
size_t UART_read ( uint8_t *buffer, size_t n, uint8_t delim );
#endif
