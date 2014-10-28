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
#include <stdbool.h>

#include "drivers/usci.h"


// The UART buffer is 16 bytes
#define UART_BUF_SIZE 16


// UART init options
#define UART_9600_BAUD 0x01


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
#define UART_send(_DATA, _LEN)\
	send(USCI_A0, _DATA, _LEN);


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
#define UART_recv(_BUFFER, _N, _DELIM, _OPTIONS)\
	recv(USCI_A0, _BUFFER, _N, _DELIM, _OPTIONS);
#endif
