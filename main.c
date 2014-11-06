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
#include "rf.h"
#include "meter.h"
#include "relay.h"

int main(void)
{
	// Initialize MSP430
	MSP430_init();
	enable_interrupts();

	// Initialize hardware
	//RELAY_init();
	//METER_init();
	//RF_init();

	uint8_t buf[] = {0x01, 0x02};
	//SPI_init(SPI_NONE);
	RF_init();
//	P2SEL &= ~BIT0;
//	P2SEL2 &= ~BIT0;
//	P2OUT &= ~BIT0;
//	P2DIR |= BIT0;

	while (1)
	{
		//uint8_t lowfq[] = {0xCC, 0x77};
		//uint8_t highfq[] = {0xCC, 0x17};

		//volatile unsigned int i;
		//SPI_send(lowfq, 2);
		//for (i=1000; i>0; --i);
		//SPI_send(lowfq, 2);
		//for (i=1000; i>0; --i);

//		P2OUT ^= BIT0;
//
	};
}

