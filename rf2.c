/*************************************************************
 *
 * Control interface for RFM12-B
 *
 * Author: Nick Jurgens <njurgens@umich.edu>
 *
 *************************************************************/

#include <stddef.h>

#include "msp430g2553.h"
#include "drivers/spi2.h"
#include "rf.h"

#define nIRQ BIT2

#define __wait_TX_ready()\
	while (P1IN & BIT6);

void RF2_init ( )
{
	// Initialize SPI
	SPI2_init(SPI_NONE);

	/*
	 * Initialize FFIT and nIRQ pins
	 *
	 * FFIT goes high when the FIFO buffer has data to read.
	 * nIRQ goes high when 
	 *
	 */
	P2SEL &= ~nIRQ;
	P2SEL2 &= ~nIRQ;
	P2DIR &= ~(nIRQ);

	/*
	 * Configure the RF module for transmit and receive
	 * on the 915MHz ISM band.
	 *
	 * 902.4 MHz, 4.8kbps
	 */
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
//	SPI2_send(config, sizeof(config));
//	SPI2_send(afc, sizeof(afc));
//	SPI2_send(bw, sizeof(bw));
//	SPI2_send(band, sizeof(band));
//	SPI2_send(fifo, sizeof(fifo));
//	SPI2_send(synchron, sizeof(synchron));
//	SPI2_send(rate, sizeof(rate));
//	SPI2_send(power, sizeof(power));
//	SPI2_send(pll, sizeof(pll));


	SPI2_send(RF_config_rx, sizeof(RF_config_rx));
	SPI2_send(RF_power_rx, sizeof(RF_power_rx));
	SPI2_send(RF_freq, sizeof(RF_freq));
	SPI2_send(RF_speed, sizeof(RF_speed));
	SPI2_send(RF_rctl, sizeof(RF_rctl));
	SPI2_send(RF_dfltr, sizeof(RF_dfltr));
	SPI2_send(RF_afc, sizeof(RF_afc));
	SPI2_send(RF_tx_config, sizeof(RF_tx_config));
	SPI2_send(RF_mcu, sizeof(RF_mcu));
	SPI2_send(RF_status_read, sizeof(RF_status_read));
}

ssize_t RF2_recv ( uint8_t *data, size_t len )
{
	if (len != 2) {
		return -1;
	}

	SPI2_send(RF_rx_read, sizeof(RF_rx_read));
	return SPI2_recv(data, len, '\0', USCI_BLOCKING);
} 


ssize_t RF2_status ( uint8_t *data, size_t len )
{
	if (len != 2) {
		return -1;
	}

	SPI2_send(RF_status_read, sizeof(RF_status_read));
	return SPI2_recv(data, len, '\0', USCI_BLOCKING);
}

ssize_t RF2_send ( uint8_t *data, size_t len )
{
	size_t index;
	uint8_t tx_data[2];

	// TODO: enable TX and send pramble
	SPI2_send(RF_config_tx, sizeof(RF_config_tx));
	SPI2_send(RF_power_tx, sizeof(RF_config_tx));
	SPI2_send(RF_status_read, sizeof(RF_status_read));

	index = 0;
	tx_data[0] = 0xB8;

	while (index < len) {
		// when MISO/SDO is low, TX buffer is ready for more data
		__wait_TX_ready();


		// transmit next byte
		tx_data[1] = data[index];
		SPI2_send(tx_data, sizeof(tx_data));

		++index;
	}

	// Write a dummy byte to make sure the entire message was sent
	__wait_TX_ready();
	SPI2_send(tx_data, sizeof(tx_data));
	__wait_TX_ready();

	// Turn off TX
	SPI2_send(RF_power_rx, sizeof(RF_config_rx));
	SPI2_send(RF_config_rx, sizeof(RF_config_rx));
}



//__attribute__((interrupt()))
//void RFIRQ  ( void )
//{
//	uint8_t status[2];
//
//	// read status
//	RF_status(status, sizeof(status));
//}
