/*************************************************************
 *
 * UART device driver for MSP430G2553.
 *
 * Author: Nick Jurgens <njurgens@umich.edu>
 *
 *************************************************************/
#ifndef __OUTLET_UART_H
#define __OUTLET_UART_H


#include <unistd.h>
#include <stdint.h>


// The UART buffer is 16 bytes
#define UART_BUF_SIZE 16

// UART init options
#define UART_9600_BAUD 0x01

// UART recv options
#define UART_BLOCKING 0x01
#define UART_DELIM 0x02


/**
 * Initialize UART at 9600 BAUD on USCI_B0.
 *
 * TODO: document options
 *
 * @param options UART options.
 */
void UART_init ( uint8_t options );


/**
 * Send an array of bytes over UART.
 *
 * @param data Pointer to array of bytes to send.
 * @param len Length of data array.
 */
void UART_send ( const uint8_t* data, size_t len );


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
ssize_t UART_recv ( uint8_t *buffer, size_t n, uint8_t delim, uint8_t options );
#endif
