/*************************************************************
 *
 * Control interface for power meter.
 *
 * Author: Nick Jurgens <njurgens@umich.edu>
 *
 *************************************************************/

// board headers
#include "msp430g2553.h"

// project headers
#include "sys/interrupts.h"
#include "drivers/uart.h"


// DO pin definitions
#define DO_PIN		BIT4
#define DOSEL		P1SEL
#define DOSEL2		P1SEL2
#define DODIR		P1DIR
#define DOIE		P1IE
#define DOIFG		P1IFG

#define METER_STATUS_DRDY 0x70;

static const uint8_t METER_reset[] = {0xC1};
static const uint8_t METER_read_status[] = {0x80, 0x17};
static const uint8_t METER_clear_drdy[] = {0x80, 0x57, 0x00, 0x00, 0x80};
static const uint8_t METER_clear_crdy[] = {0x80, 0x57, 0x00, 0x00, 0x40};
static const uint8_t METER_set_baud[] = {0x80, 0x47, 0x00, 0x40, 0x02};
static const uint8_t METER_write_config0[] = {0x80, 0x40, 0x00, 0x20, 0xC0};
static const uint8_t METER_write_config1[] = {0x80, 0x41, 0xEF, 0xEE, 0x00};
static const uint8_t METER_write_config2[] = {0x90, 0x40, 0x1A, 0x06, 0x50};
static const uint8_t METER_write_interrupt_mask[] = {0x80, 0x43, 0x00, 0x00, 0x80};
static const uint8_t METER_begin_conv[] = {0xD5};
static const uint8_t METER_read_power[] = {0x90, 0x04};

/**
 * Wait until the DRDY flag is set in the Status0 register.
 */
#define __METER_poll_drdy()\
	while(1) {\
		uint8_t data[3];\
		UART_send(METER_read_status, sizeof(METER_read_status));\
		UART_recv(data, sizeof(data), '\0', USCI_BLOCKING);\
		if (data[2] & 0x80) {\
			UART_send(METER_clear_drdy, sizeof(METER_clear_drdy));\
			break;\
		}\
	}

#define __METER_poll_crdy()\
	while(1) {\
		uint8_t data[3];\
		UART_send(METER_read_status, sizeof(METER_read_status));\
		UART_recv(data, sizeof(data), '\0', USCI_BLOCKING);\
		if (data[2] & 0x40) {\
			UART_send(METER_clear_crdy, sizeof(METER_clear_crdy));\
			break;\
		}\
	}


void METER_init ( )
{
	// Try resetting the meter @ 128,000 baud
	UART_init(UART_128000_BAUD);
	UART_send(METER_reset, sizeof(METER_reset));

	// Try resetting the meter @ 600 baud
	UART_flush();
	UART_init(UART_600_BAUD);
	UART_send(METER_reset, sizeof(METER_reset));

	// Wait for responses
	__METER_poll_drdy();

	// Increase the baud rate to 128000 baud
	UART_send(METER_set_baud, sizeof(METER_set_baud));
	UART_flush();
	UART_init(UART_128000_BAUD);

	// Write configuration
	UART_send(METER_write_config0, sizeof(METER_write_config0));
	UART_send(METER_write_config1, sizeof(METER_write_config1));
	UART_send(METER_write_interrupt_mask, sizeof(METER_write_interrupt_mask));

	// Enable integrator on current channel, HPF on voltage channel
	UART_send(METER_write_config2, sizeof(METER_write_config2));

	// TODO: remove after debugging
	UART_flush();

	// configure interrupt DO on p1.4
	DOSEL &= ~DO_PIN;
	DOSEL2 &= ~DO_PIN;
	DODIR &= ~DO_PIN;
	DOIFG &= ~DO_PIN;
	DOIE |= DO_PIN;
}

void METER_begin ( )
{
	// TODO: wake up

	// Begin continuous mode conversions
	UART_send(METER_begin_conv, sizeof(METER_begin_conv));
}

uint32_t METER_read ( )
{
	uint32_t power;
	uint8_t data[3];

	// send read command
//	__METER_poll_drdy();
//	UART_send(METER_clear_drdy, sizeof(METER_clear_drdy));
	UART_send(METER_read_power, sizeof(METER_read_power));
	UART_recv(data, sizeof(data), '\0', USCI_BLOCKING);

	power = (uint32_t)data[0] + ((uint32_t)data[1]<<8) + ((uint32_t)data[2]<<16);
	return power;
}

// TODO: figure out the byte order

void METER_debug ( )
{
	uint8_t dbg[] = {0x00};
	UART_send(METER_begin_conv, sizeof(METER_begin_conv));
}

static uint32_t metered_power;

__attribute__ ((interrupt(PORT1_VECTOR)))
void  METER_ISR ( void )
{
	// Interrupt DO fired
	if (DOIFG & DO_PIN) {
		// we need access to the UART interrupts

		// clear the interrupt from the power meter
		UART_send(METER_read_status, sizeof(METER_read_status));
		UART_send(METER_clear_drdy, sizeof(METER_clear_drdy));

		// read power
//		UART_clear();
//		UART_send(METER_read_power, sizeof(METER_read_power));
		metered_power = METER_read();

		// clear our pending interrupt
		DOIFG &= ~DO_PIN;
	}
}
