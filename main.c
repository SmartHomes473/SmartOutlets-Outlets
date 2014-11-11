/*************************************************************
 *
 * SmartOutlets outlet firmware.
 *
 * Author: Nick Jurgens <njurgens@umich.edu>
 *
 *************************************************************/
#include <stdint.h>

#include "msp430g2553.h"
#include "sys/init.h"
#include "sys/interrupts.h"
#include "drivers/uart.h"
#include "drivers/spi.h"
#include "drivers/spi2.h"
#include "rf.h"
#include "meter.h"
#include "relay.h"

int main(void)
{
	// Initialize MSP430
	MSP430_init();
	enable_interrupts();

	// Initialize hardware
	SPI_init(SPI_NONE);

	P1SEL &= ~BIT4;
	P1SEL2 &= ~BIT4;
	P1DIR &= ~BIT4;

	// initialization
	send_cmd(RF_CONFIG_RX);
	send_cmd(RF_POWER_RX);
	send_cmd(RF_CENTER_FREQ);
	send_cmd(RF_DATA_RATE);
	send_cmd(RF_RECV_CTL);
	send_cmd(RF_DATA_FILTER);

	// FIFO mode, 2 byte sync
	send_cmd(RF_FIFO_SYNC);
	send_cmd(RF_SYNC_MODE);

	send_cmd(RF_AFC_CMD);
	send_cmd(RF_TX_CTL);
	send_cmd(RF_PLL_CFG);
	send_cmd(RF_WAKEUP);
	send_cmd(RF_DUTY_CYCLE);
	send_cmd(RF_LOW_BATTERY);

	uint8_t tx = 1;
	while (1) {
		if (tx) {
			uint8_t tx[] = {0x11, 0x22, 0x33, 0x44};
			RF0_tx(tx, sizeof(tx));
		}

		else {
			if (!(P1IN&BIT4)) {
				send_cmd(RF_STATUS_READ);
			}
		}
	}
}

