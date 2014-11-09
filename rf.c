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

	// initialization
	RF0_send_cmd(RF_RECEIVER_ON);
	RF0_send_cmd(RF_CONFIG_915MHZ);
	RF0_send_cmd(0xA640);
	RF0_send_cmd(0xC647);
	RF0_send_cmd(0x94A2);
	RF0_send_cmd(0xC2AC);

	// FIFO mode, 2 byte sync
	RF0_send_cmd(0xCA837);
	RF0_send_cmd(0xCE00 | RF_SYNC_BYTE);

	RF0_send_cmd(0xA640);
	RF0_send_cmd(0x9850);
	RF0_send_cmd(0xCC77);
	RF0_send_cmd(0xE000);
	RF0_send_cmd(0xC800);
	RF0_send_cmd(0xC049);

	// read status
	RF0_send_cmd(0x0100);
}

void RF1_init ( )
{
	// initialize SPI
	SPI2_init(SPI_NONE);

	// TODO: config nIRQ

	// initialization
	RF1_send_cmd(RF_RECEIVER_ON);
	RF1_send_cmd(RF_CONFIG_915MHZ);
	RF1_send_cmd(0xA640);
	RF1_send_cmd(0xC647);
	RF1_send_cmd(0x94A2);
	RF1_send_cmd(0xC2AC);

	// FIFO mode, 2 byte sync
	RF1_send_cmd(0xCA83);
	RF1_send_cmd(0xCE00 | RF_SYNC_BYTE);

	RF1_send_cmd(0xA640);
	RF1_send_cmd(0x9850);
	RF1_send_cmd(0xCC77);
	RF1_send_cmd(0xE000);
	RF1_send_cmd(0xC800);
	RF1_send_cmd(0xC049);

	// read status
	RF1_send_cmd(0x0100);
}

void RF0_tx ( uint8_t *data, size_t len )
{
	// enable tx
	RF0_send_cmd(0x1000);
	RF0_send_cmd(RF_CONFIG_915MHZ_TX);
	RF0_send_cmd(RF_XMITTER_ON);

	P2OUT &= ~BIT0;
	__spi_send(USCI_B0, 0xB8);

	size_t index;
	for (index = 0; index < len; ++index) {
		while (P1IN&BIT6);
		while (USCI_B0->UCxx->STAT&UCBUSY);
		__spi_send(USCI_B0, data[index]);
	}

	while (USCI_B0->UCxx->STAT&UCBUSY);
	P2OUT |= BIT0;

	// enable rx
	RF0_send_cmd(0x1000);
	RF0_send_cmd(0x8077);
	RF0_send_cmd(0x8281);
	RF0_send_cmd(0x1000);
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

void RF0_cmd ( uint16_t cmd )
{
	RF0_send_cmd(cmd);
}

__attribute__((interrupt()))
void RFIRQ  ( void )
{
}
