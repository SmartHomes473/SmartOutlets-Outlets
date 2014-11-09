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
	RF0_cmd(0x8077);
	RF0_cmd(0x8281);
	RF0_cmd(0xA680);
	RF0_cmd(0xC602);
	RF0_cmd(0x9482);
	RF0_cmd(0xC2AC);

	// FIFO mode, 2 byte sync
	RF0_cmd(0xCA80);
	RF0_cmd(0xCED4);

	RF0_cmd(0xC483);
	RF0_cmd(0x9870);
	RF0_cmd(0xCC77);
	RF0_cmd(0xE000);
	RF0_cmd(0xC800);
	RF0_cmd(0xC000);

	RF0_cmd(0x0100);

	uint8_t data[] = {0x2D, 0xD4, 0x11, 0x22, 0x33, 0x44, 0xAA, 0xAA, 0xAA};
	RF0_tx(data, sizeof(data));

	while (1) {
		if (P1IN & BIT4) {
			RF0_cmd(0x0100);
		}
	}
}

