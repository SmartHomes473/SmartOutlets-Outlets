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



// SPI recv options
#define SPI_BLOCKING 0x01;
#define SPI_DELIM 0x02;


/**
 * Initialize SPI on USCI_B0.
 */
void SPI_init ( );


/**
 * Send an array of bytes over SPI.
 *
 * @param data Pointer to array of bytes to send.
 * @param len Length of data array.
 */
void SPI_send ( const uint8_t* data, size_t len );


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
ssize_t SPI_recv ( uint8_t *buffer, size_t n, uint8_t delim, uint8_t options );

#endif
