/*************************************************************
 *
 * SPI device driver for MSP430G2553.
 *
 * Author: Nick Jurgens <njurgens@umich.edu>
 *
 *************************************************************/
#ifndef __OUTLETS_SPI_H
#define __OUTLETS_SPI_H


#include <unistd.h>
#include <stdint.h>

#include "drivers/usci.h"

#define SPI_NONE 0x0


/**
 * Initialize SPI on USCI_B0.
 */
void SPI_init ( uint8_t options );


/**
 * Send an array of bytes over SPI.
 *
 * @param data Pointer to array of bytes to send.
 * @param len Length of data array.
 */
#define SPI_send(_DATA, _LEN)\
{\
	P2OUT &= ~BIT0;\
	send(USCI_B0, _DATA, _LEN);\
	flush(USCI_B0);\
	P2OUT |= BIT0;\
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
#define SPI_recv(_BUFFER, _N, _DELIM, _OPTIONS)\
	recv(USCI_B0, _BUFFER, _N, _DELIM, _OPTIONS)

#define SPI_reset()\
{\
	disable_interrupts();\
	USCI_B0->buffer_head = USCI_B0->buffer;\
	USCI_B0->buffer_tail = USCI_B0->buffer;\
	USCI_B0->data_ready = 0;\
	enable_interrupts();\
}
#endif
