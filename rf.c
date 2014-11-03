/*************************************************************
 *
 * Control interface for RFM12-B
 *
 * Author: Nick Jurgens <njurgens@umich.edu>
 *
 *************************************************************/

#include <stddef.h>

#include "msp430g2553.h"
#include "drivers/spi.h"

#define nIRQ BIT2
#define FFS BIT1

#define __wait_TX_ready()\
	while (P1IN & BIT6);

static const uint8_t RF_config_tx_on[] = {0x80, 0x7D};
static const uint8_t RF_config_tx_off[] = {0x80, 0xFD};
static const uint8_t RF_power_tx_on[] = {0x82, 0xD9};
static const uint8_t RF_power_tx_off[] = {0x82, 0xF9};
static const uint8_t RF_freq[] = {0xA1, 0x40};
static const uint8_t RF_speed[] = {0xC6, 0x47};
static const uint8_t RF_rctl[] = {0x90, 0x80};
static const uint8_t RF_dfltr[] = {0xC2, 0xAC};
static const uint8_t RF_fifo[] = {0xCA, 0x81};
static const uint8_t RF_sync[] = {0xCE, 0xD4};
static const uint8_t RF_afc[] = {0xC4, 0x43};
static const uint8_t RF_tx_config[] = {0x98, 0x50};
static const uint8_t RF_pll[] = {0xCC, 0x77};
static const uint8_t RF_mcu[] = {0xC0, 0x00};

static const uint8_t RF_rx_read[] = {0xB0, 0x00};
static const uint8_t RF_status_read[] = {0x10, 0x00};

void RF_init ( )
{
	// Initialize SPI
	SPI_init(SPI_NONE);

	/*
	 * Initialize FFIT and nIRQ pins
	 *
	 * FFIT goes high when the FIFO buffer has data to read.
	 * nIRQ goes high when 
	 *
	 */
	P2SEL |= nIRQ | FFS;
	P2SEL2 |= nIRQ | FFS;
	P2DIR &= ~(nIRQ | FFS);

	/*
	 * Configure the RF module for transmit and receive
	 * on the 915MHz ISM band.
	 *
	 * 902.4 MHz, 4.8kbps
	 */
	SPI_send(RF_config_tx_off, sizeof(RF_config_tx_off));
	SPI_send(RF_power_tx_off, sizeof(RF_power_tx_off));
	SPI_send(RF_freq, sizeof(RF_freq));
	SPI_send(RF_speed, sizeof(RF_speed));
	SPI_send(RF_rctl, sizeof(RF_rctl));
	SPI_send(RF_dfltr, sizeof(RF_dfltr));
	SPI_send(RF_sync, sizeof(RF_sync));
	SPI_send(RF_afc, sizeof(RF_afc));
	SPI_send(RF_tx_config, sizeof(RF_tx_config));
	SPI_send(RF_mcu, sizeof(RF_mcu));
}

ssize_t RF_recv ( uint8_t *data, size_t len )
{
	if (len != 2) {
		return -1;
	}

	SPI_send(RF_rx_read, sizeof(RF_rx_read));
	return SPI_recv(data, len, '\0', USCI_BLOCKING);
} 


ssize_t RF_status ( uint8_t *data, size_t len )
{
	if (len != 2) {
		return -1;
	}

	SPI_send(RF_status_read, sizeof(RF_status_read));
	return SPI_recv(data, len, '\0', USCI_BLOCKING);
}

ssize_t RF_send ( uint8_t *data, size_t len ) 

{
	size_t index;
	uint8_t tx_data[2];

	// TODO: enable TX and send pramble
	SPI_send(RF_config_tx_on, sizeof(RF_config_tx_on));
	SPI_send(RF_power_tx_on, sizeof(RF_config_tx_on));

	index = 0;
	tx_data[0] = 0xB8;

	while (index < len) {
		// when MISO/SDO is low, TX buffer is ready for more data
		__wait_TX_ready();


		// transmit next byte
		tx_data[1] = data[index];
		SPI_send(tx_data, sizeof(tx_data));

		++index;
	}

	// Write a dummy byte to make sure the entire message was sent
	__wait_TX_ready();
	SPI_send(tx_data, sizeof(tx_data));
	__wait_TX_ready();

	// Turn off TX
	SPI_send(RF_power_tx_off, sizeof(RF_config_tx_off));
	SPI_send(RF_config_tx_off, sizeof(RF_config_tx_off));
}



__attribute__((interrupt()))
void RFIRQ  ( void )
{
	uint8_t status[2];

	// read status
	RF_status(status, sizeof(status));
}
