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
#include "rf.h"

#define nIRQ BIT2

#define __wait_TX_ready()\
	while (P1IN & BIT6);

const uint8_t RF_config_tx[] = {0x80, 0xF8};
const uint8_t RF_config_rx[] = {0x80, 0x78};
const uint8_t RF_power_tx[] = {0x82, 0x28};
const uint8_t RF_power_rx[] = {0x82, 0x89};
const uint8_t RF_freq[] = {0xA6, 0x80};
const uint8_t RF_speed[] = {0xC6, 0x02};
const uint8_t RF_rctl[] = {0x94, 0x80};
const uint8_t RF_dfltr[] = {0xC2, 0x2C};
const uint8_t RF_fifo[] = {0xCA, 0x85};
const uint8_t RF_afc[] = {0xC4, 0xF7};
const uint8_t RF_tx_config[] = {0x98, 0x70};
const uint8_t RF_pll[] = {0xCC, 0x77};
const uint8_t RF_mcu[] = {0xC0, 0x00};
const uint8_t RF_rx_read[] = {0xB0, 0x00};
const uint8_t RF_status_read[] = {0x10, 0x00};



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
	P2SEL &= ~nIRQ;
	P2SEL2 &= ~nIRQ;
	P2DIR &= ~nIRQ;

	/*
	 * Configure the RF module for transmit and receive
	 * on the 915MHz ISM band.
	 *
	 * 902.4 MHz, 4.8kbps
	 */
//	uint8_t config[] = {0x80, 0xE8};
//	uint8_t afc[] = {0xC4, 0x83};
//	uint8_t bw[] = {0x98, 0x24};
//	uint8_t band[] = {0xA6, 0x80};
//	uint8_t fifo[] = {0xCA, 0x83};
//	uint8_t synchron[] = {0xCE, 0xC2};
//	uint8_t rate[] = {0xC6, 0x26};
//	uint8_t power[] = {0x82, 0x19};
//	uint8_t pll[] = {0xCC, 0x16};
//
//	SPI_send(config, sizeof(config));
//	SPI_send(afc, sizeof(afc));
//	SPI_send(bw, sizeof(bw));
//	SPI_send(band, sizeof(band));
//	SPI_send(fifo, sizeof(fifo));
//	SPI_send(synchron, sizeof(synchron));
//	SPI_send(rate, sizeof(rate));
//	SPI_send(power, sizeof(power));
//	SPI_send(pll, sizeof(pll));


	SPI_send(RF_config_rx, sizeof(RF_config_rx));
	SPI_send(RF_power_rx, sizeof(RF_power_rx));
	SPI_send(RF_freq, sizeof(RF_freq));
	SPI_send(RF_speed, sizeof(RF_speed));
	SPI_send(RF_rctl, sizeof(RF_rctl));
	SPI_send(RF_dfltr, sizeof(RF_dfltr));
	SPI_send(RF_afc, sizeof(RF_afc));
	SPI_send(RF_tx_config, sizeof(RF_tx_config));
	SPI_send(RF_mcu, sizeof(RF_mcu));
	SPI_send(RF_status_read, sizeof(RF_status_read));
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
	uint8_t cmd[2];

	cmd[0] = 0x00;
	cmd[1] = 0x00;
	SPI_send(cmd, sizeof(cmd));

	cmd[0] = 0x82;
	cmd[1] = 0x39;
	SPI_send(cmd, sizeof(cmd));

	uint8_t tx_buf[] = { 0xb8, 0xaa, 0xaa, 0x2d, 0xc2, 0xaa, 0xaa};

	P2OUT &= ~BIT0;

	index = 0;
	while (index < sizeof(tx_buf)) {
		__wait_TX_ready();
		while (!(IFG2&USCI_B0->TXIFG));
		USCI_B0->UCxx->TXBUF = tx_buf[index];
		++index;
	}

	flush(USCI_B0);
	P2OUT |= BIT0;

//	SPI_send(RF_config_tx, sizeof(RF_config_tx));
//	SPI_send(RF_power_tx, sizeof(RF_power_tx));
//	SPI_send(RF_status_read, sizeof(RF_status_read));



//	index = 0;
//	tx_data[0] = 0xB8;

//	while (index < len) {
//		// when MISO/SDO is low, TX buffer is ready for more data
//		__wait_TX_ready();
//
//
//		// transmit next byte
//		tx_data[1] = data[index];
//		SPI_send(tx_data, sizeof(tx_data));
//
//		++index;
//	}

	// Write a dummy byte to make sure the entire message was sent
//	__wait_TX_ready();
//	SPI_send(tx_data, sizeof(tx_data));
//	__wait_TX_ready();

	// Turn off TX
	SPI_send(RF_power_rx, sizeof(RF_power_rx));
	SPI_send(RF_config_rx, sizeof(RF_config_rx));
}



__attribute__((interrupt()))
void RFIRQ  ( void )
{
	uint8_t status[2];

	// read status
	RF_status(status, sizeof(status));
}
