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
#include "rf2.h"
#include "meter.h"
#include "relay.h"

int main(void)
	{
	// Initialize MSP430
	MSP430_init();
	enable_interrupts();

	// Initialize hardware
//	RELAY_init();
//	METER_init();
	RF_init();
	RF2_init();

	while (1)
	{
		uint8_t message[] = {0x11, 0x22, 0x33};
		uint8_t readrq[] = {0xB0, 0x00, 0x00, 0x00};
		uint8_t status[] = {0x01, 0x00, 0x00, 0x00};
		RF_send(message, sizeof(message));
		SPI2_send(status, sizeof(status));
		SPI2_send(readrq, sizeof(readrq));
		SPI2_send(readrq, sizeof(readrq));
		SPI2_send(readrq, sizeof(readrq));
		SPI2_send(readrq, sizeof(readrq));
	};
}

