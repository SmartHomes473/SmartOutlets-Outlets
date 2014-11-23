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
	// Initialize MSP430
	MSP430_init();
	enable_interrupts();

	// Initialize power meter
	METER_init();

//	// Initialize relay
//	RELAY_init();
//	RELAY_on();
//
//	// Initialize RFM12B
//	RF0_init();
//
//	// Run outlet's program loop
//	OUTLET_run();

	// trap
	while (1);
}

void OUTLET_run ( )
{
	uint8_t *head, *tail;
	uint8_t packet[9];

	// initialize buffer pointers
	head = packet;
	tail = packet+sizeof(packet);

	// begin metering power
	METER_begin();

	while (1) {
		uint8_t buffer[2];

		// TODO: convert the polling implementation to an interrupt
		//       driven one

		// poll the status pin
		if (!(P1IN&BIT4)) {

			// reset SPI buffer and read status
			SPI_reset();
			RF0_send_cmd(RF_STATUS_READ);
			SPI_recv(buffer, sizeof(buffer), '\0', USCI_BLOCKING);

			// check status for ready data
			if (!(buffer[0]&0x80)) {
				continue;
			}

			// reset SPI buffer and read byte
			SPI_reset();
			RF0_send_cmd(RF_FIFO_READ);
			SPI_recv(buffer, sizeof(buffer), '\0', USCI_BLOCKING);

			// store byte into packet
			*head = buffer[1];
			++head;

			// reset packet
			if (head == tail) {

				// verify packet identifier
				if (packet[0] == (uint8_t)0xDC && packet[1] == (uint8_t)0xDC){

					// TODO: packet verification, like checksum verification

					// decode opcode
					switch (packet[5]) {

					// TODO: add a SOPS module for packet decoding, etc

					// OUTLET_ON command
					case 0xAA:

						// power on SSR
						RELAY_on();
						RF0_send_ack(packet[4]);

						break;

					// OUTLET_OFF command
					case 0x55:

						// power on SSR
						RELAY_off();
						RF0_send_ack(packet[4]);

						break;

					// OUTLET_POWER command
					case 0x99:
						RF0_send_power(METER_read(), packet[4]);
						break;

					default:
						break;
					}
				}

				// reset to receive next packet
				head = packet;
				RF0_send_cmd(RF_FIFO_RESET);
				RF0_send_cmd(RF_FIFO_SYNC);
			}
		}
	}
}

