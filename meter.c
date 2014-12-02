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

// Metered power
volatile uint32_t metered_power = 0;


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
static const uint8_t METER_read_power[] = {0x90, 0x07};
static const uint8_t METER_integrator_gain[] = {0x92, 0x6B, 0x00, 0x00, 0x0C};

//FIRST CALIBRATION
//static const uint8_t METER_write_i_gain[] = {0x90, 0x61, 0x00, 0x00, 0x40};
//static const uint8_t METER_write_v_gain[] = {0x90, 0x63, 0xD5, 0x94, 0x78};

// SECOND CALIBRATION
//static const uint8_t METER_write_i_gain[] = {0x90, 0x61, 0x00, 0x00, 0x4B};
//static const uint8_t METER_write_v_gain[] = {0x90, 0x63, 0xBF, 0xCD, 0x79};


// THIRD CALIBRATION
static const uint8_t METER_write_i_gain[] = {0x90, 0x61, 0x00, 0x00, 0x40};
static const uint8_t METER_write_v_gain[] = {0x90, 0x63, 0x00, 0x00, 0x90};


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

	// Enable integrator on current channel, HPF on voltage channel
	UART_send(METER_write_config2, sizeof(METER_write_config2));

	// Set gain
	UART_send(METER_write_i_gain, sizeof(METER_write_i_gain));
	UART_send(METER_write_v_gain, sizeof(METER_write_v_gain));

	// Set Rogowski coil integrator to 60Hz
	UART_send(METER_integrator_gain, sizeof(METER_integrator_gain));

	// Enable interrupt on DRDY
	UART_send(METER_write_interrupt_mask, sizeof(METER_write_interrupt_mask));
}



void METER_begin ( )
{
	// configure interrupt DO on p1.4
	DOSEL &= ~DO_PIN;
	DOSEL2 &= ~DO_PIN;
	DODIR &= ~DO_PIN;
	DOIFG &= ~DO_PIN;
	DOIE |= DO_PIN;

	// Begin continuous mode conversions
	UART_send(METER_begin_conv, sizeof(METER_begin_conv));
}


__attribute__ ((interrupt(PORT1_VECTOR)))
void METER_ISR ( void )
{
	uint8_t data[3];

	// Interrupt DO fired
	if (DOIFG & DO_PIN) {
		// we need access to the UART interrupts
		DOIE &= ~DO_PIN;
		enable_interrupts();

		// read power
		UART_clear();
		UART_send(METER_read_power, sizeof(METER_read_power));
		UART_recv(data, sizeof(data), '\0', USCI_BLOCKING);

		// convert the byte array to a 32bit value
		metered_power = (uint32_t)data[0];
		metered_power += ((uint32_t)data[1]<<8);
		metered_power += ((uint32_t)data[2]<<16);

		// clear our pending interrupt
		DOIFG &= ~DO_PIN;
		DOIE |= DO_PIN;

		// clear the interrupt from the power meter
		UART_send(METER_clear_drdy, sizeof(METER_clear_drdy));
	}
}
