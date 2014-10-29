/*************************************************************
 *
 * USCI device driver for MSP430G2553.
 *
 * Author: Nick Jurgens <njurgens@umich.edu>
 *
 *************************************************************/
#ifndef __OUTLETS_USCI_H
#define __OUTLETS_USCI_H


#include <stdint.h>

#include "msp430g2553.h"
#include "config.h"


// USCI recv options
#define USCI_BLOCKING 0x01
#define USCI_DELIM 0x02


typedef struct {
	volatile uint8_t CTL0;
	volatile uint8_t CTL1;
	volatile uint8_t BR0;
	volatile uint8_t BR1;
	volatile uint8_t MCTL;
	volatile uint8_t STAT;
	volatile uint8_t RXBUF;
	volatile uint8_t TXBUF;
} usci_mem_t;

typedef struct {
	volatile usci_mem_t *UCxx;
	volatile uint8_t *buffer_head;
	volatile uint8_t *buffer_tail;
	volatile uint16_t TXIFG;
	volatile uint8_t buffer[USCI_BUFFER_SIZE];
	volatile bool data_ready;
} usci_t;


// USCI instances
extern usci_t *USCI_A0;
extern usci_t *USCI_B0;


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
ssize_t recv ( usci_t *usci, uint8_t *buffer, size_t n, uint8_t delim, uint8_t options );


/**
 * Send an array of bytes over USCI.
 *
 * @param data Pointer to array of bytes to send.
 * @param len Length of data array.
 */
void send ( usci_t *usci, const uint8_t *data, size_t len );

#endif
