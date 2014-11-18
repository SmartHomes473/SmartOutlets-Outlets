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

	// Initialize RFM12B
	RF0_init();

	uint8_t tx = 0;
	while (1) {
		uint8_t data[2];

		if (tx) {
			uint8_t tx[] = "mello world :)";
			RF0_tx(0x00, tx, sizeof(tx));
		}

		else {
			if (!(P1IN&BIT4)) {
				SPI_reset();
				send_cmd(RF_STATUS_READ);
				SPI_recv(data, sizeof(data), '\0', USCI_BLOCKING);
				if (data[0]&0x80) {
					SPI_reset();
					send_cmd(RF_FIFO_READ);
				}
				else {
					//send_cmd(RF_FIFO_RESET);
					//send_cmd(RF_FIFO_SYNC);
				}
			}
		}
	}
}

