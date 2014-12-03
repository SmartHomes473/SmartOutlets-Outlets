/*************************************************************
 *
 * SmartOutlets outlet firmware.
 *
 * Author: Nick Jurgens <njurgens@umich.edu>
 *
 *************************************************************/

// C headers
#include <stdint.h>

// MSP430 header
#include "msp430g2553.h"

// system headers
#include "sys/init.h"
#include "sys/interrupts.h"

// drivers
#include "drivers/uart.h"
#include "drivers/spi.h"

// peripherals
#include "rf.h"
#include "meter.h"
#include "relay.h"

void OUTLET_run ( );

int main(void)
{
	// power-on delay
	__delay_cycles(10000);

	// Initialize MSP430
	MSP430_init();
	enable_interrupts();

	// Initialize relay
	RELAY_init();

	P1SEL &= ~BIT3;
	P1SEL2 &= ~BIT3;
	P1DIR &= ~BIT3;
	P1IE |= BIT3;

	RELAY_on();


	// Initialize power meter
	if (*(uint8_t*)0x1040 == 0xFF) {
		RELAY_on();
		METER_calibrate();
	}

	// Turn off relay to signal we've finished
	RELAY_off();

	// trap
	while (1);
}

