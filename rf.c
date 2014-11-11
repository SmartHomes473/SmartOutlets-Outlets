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
#include "drivers/spi2.h"
#include "rf.h"

#define nIRQ BIT2

typedef usci_t rfm12_t;

#define __wait_TX_ready()\
	while (P1IN & BIT6);

#define __spi_send(USCI, BYTE)\
{\
	while (!(IFG2&USCI->TXIFG));\
	USCI->UCxx->TXBUF = BYTE;\
}

void __rfm12_send_cmd ( rfm12_t *usci, uint16_t cmd )
{
	__spi_send(usci, (uint8_t)(cmd>>8));
	__spi_send(usci, (uint8_t)(cmd&0xFF));
}

#define RF0_send_cmd(CMD)\
{\
	P2OUT &= ~BIT0;\
	__rfm12_send_cmd(USCI_B0, CMD);\
	while (USCI_B0->UCxx->STAT&UCBUSY);\
	P2OUT |= BIT0;\
}
#define RF1_send_cmd(CMD)\
{\
	P2OUT &= ~BIT1;\
	 __rfm12_send_cmd(USCI_A0, CMD);\
	 while (USCI_A0->UCxx->STAT&UCBUSY);\
	 P2OUT |= BIT1;\
}


void RF0_init ( )
{
	// initialize SPI
	SPI_init(SPI_NONE);

	// TODO: config nIRQ

	// Initialize hardware
	SPI_init(SPI_NONE);

	// nIRQ
	P1SEL &= ~BIT4;
	P1SEL2 &= ~BIT4;
	P1DIR &= ~BIT4;

	// initialization
	RF0_send_cmd(RF_CONFIG_RX);
	RF0_send_cmd(RF_POWER_RX);
	RF0_send_cmd(RF_CENTER_FREQ);
	RF0_send_cmd(RF_DATA_RATE);
	RF0_send_cmd(RF_RECV_CTL);
	RF0_send_cmd(RF_DATA_FILTER);

	// FIFO mode, 2 byte sync
	RF0_send_cmd(RF_FIFO_SYNC);
	RF0_send_cmd(RF_SYNC_MODE);

	RF0_send_cmd(RF_AFC_CMD);
	RF0_send_cmd(RF_TX_CTL);
	RF0_send_cmd(RF_PLL_CFG);
	RF0_send_cmd(RF_WAKEUP);
	RF0_send_cmd(RF_DUTY_CYCLE);
	RF0_send_cmd(RF_LOW_BATTERY);
}

void RF1_init ( )
{
	// initialize SPI
	SPI2_init(SPI_NONE);

	// TODO: config nIRQ

	// initialization
//	RF1_send_cmd(RF_RECEIVER_ON);
//	RF1_send_cmd(RF_CONFIG_915MHZ);
//	RF1_send_cmd(0xA640);
//	RF1_send_cmd(0xC647);
//	RF1_send_cmd(0x94A2);
//	RF1_send_cmd(0xC2AC);
//
//	// FIFO mode, 2 byte sync
//	RF1_send_cmd(0xCA83);
//	RF1_send_cmd(0xCE00 | RF_SYNC_BYTE);
//
//	RF1_send_cmd(0xA640);
//	RF1_send_cmd(0x9850);
//	RF1_send_cmd(0xCC77);
//	RF1_send_cmd(0xE000);
//	RF1_send_cmd(0xC800);
//	RF1_send_cmd(0xC049);
//
//	// read status
//	RF1_send_cmd(0x0100);
}


////	while (!(P1IN&BIT6));
//#define __generate_preamble()\
//	__rf_tx_byte(0xAA);

#define __rf_tx_byte(BYTE)\
{\
	while (USCI_B0->UCxx->STAT&UCBUSY);\
	while (!(P1IN&BIT6));\
	__spi_send(USCI_B0, BYTE);\
}

void RF0_tx ( uint8_t *data, size_t len )
{
	size_t index;

	// enable tx
	RF0_send_cmd(RF_STATUS_READ);
	RF0_send_cmd(RF_CONFIG_TX);

	// power on transmitter
	RF0_send_cmd(RF_POWER_TX);

	// generate preamble (0xAA)
//	__generate_preamble();
//	__generate_preamble();

	// start transmit
	P2OUT &= ~BIT0;
	while (!(P1IN&BIT6));
	__spi_send(USCI_B0, 0xB8);
	__rf_tx_byte(0xAA);
	__rf_tx_byte(0xAA);
	__rf_tx_byte(RF_SYNC_BYTE1);
	__rf_tx_byte(RF_SYNC_BYTE0);

	for (index = 0; index < len; ++index) {
		__rf_tx_byte(data[index]);
	}

	while (USCI_B0->UCxx->STAT&UCBUSY);
	P2OUT |= BIT0;


	// enable rx
	RF0_send_cmd(RF_CONFIG_RX);
	RF0_send_cmd(RF_POWER_RX);
}

ssize_t RF1_recv ( uint8_t *data, size_t len )
{
} 


ssize_t RF_status ( uint8_t *data, size_t len )
{
}

ssize_t RF_send ( uint8_t *data, size_t len ) 
{
}

void send_cmd ( uint16_t cmd )
{
	RF0_send_cmd(cmd);
}

__attribute__((interrupt()))
void RFIRQ  ( void )
{
}
