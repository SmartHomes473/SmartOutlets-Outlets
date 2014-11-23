/*************************************************************
 *
 * Control interface for power meter.
 *
 * Author: Nick Jurgens <njurgens@umich.edu>
 *
 *************************************************************/

#include "drivers/uart.h"

#define METER_STATUS_DRDY 0x70;

static const uint8_t METER_reset[] = {0xC1};
static const uint8_t METER_read_status[] = {0x17};
static const uint8_t METER_write_status[] = {0x57};
static const uint8_t METER_clear_drdy[] = {0x57, 0x00, 0x00, 0x80};
static const uint8_t METER_clear_crdy[] = {0x57, 0x00, 0x00, 0x40};
static const uint8_t METER_set_baud[] = {0x47, 0x00, 0x40, 0x02};
static const uint8_t METER_write_config0[] = {0x40, 0x00, 0x00, 0x00};
static const uint8_t METER_write_config1[] = {0x41, 0x0F, 0x00, 0x00};
static const uint8_t METER_begin_conv[] = {0xD5};
static const uint8_t METER_cal_DC_offset[] = {};
static const uint8_t METER_cal_AC_offset[] = {};
static const uint8_t METER_cal_gain[] = {};
static const uint8_t METER_read_power[] = {0x04};
static const uint8_t METER_set_page_16[] = {0x90};

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
	uint8_t buffer[4];
	volatile long long i;

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

	// TODO: enable highpass filer? (config2)

	// TODO: enable Integrator for Rogowski coil (config2)


	// Set register page to 16 for reading power measurements
	UART_send(METER_set_page_16, sizeof(METER_set_page_16));

	// Calibrate DC offset
//	UART_send(METER_cal_DC_offset, sizeof(METER_cal_DC_offset));
//	__METER_poll_drdy();
//
//	// Calibrate AC offset
//	UART_send(METER_cal_AC_offset, sizeof(METER_cal_AC_offset));
//	__METER_poll_drdy();
//
//	// Calibrate gain
//	UART_send(METER_cal_gain, sizeof(METER_cal_gain));
//	__METER_poll_drdy();
}

void METER_begin ( )
{
	// TODO: wake up

	// Begin continuous mode conversions
	UART_send(METER_begin_conv, sizeof(METER_begin_conv));
}

uint32_t METER_read ( )
{
//	uint64_t power;
	uint8_t data[3];

	// send read command
	UART_send(METER_read_power, sizeof(METER_read_power));
	UART_recv(data, sizeof(data), '\0', USCI_BLOCKING);

	/**
	 * Power is provided on a scale of 0 to 0xFFFFFF.  The maximum value
	 * is 140V * 50A = 7000W which is represented by 0x2E147B.  Thus, we
	 * calculate power to be:
	 *
	 *  Power = (REG_power * 7000) / 0x2E147B
	 *
	 *  XXX: I'm masking off the sign bit (msb of data[2]) to get the absolute
	 *       value.  Not sure if this is right...
	 *  FIXME: this is really slow. http://www.ti.com/lit/an/slaa329/slaa329.pdf
	 */
//	power = (uint64_t)(data[0] + ((uint32_t)data[1]<<8) + ((uint32_t)(data[2]&0x7F)<<16));
//	power *= 7000;
//	power /= 0x2E147B;
//	return (uint16_t)power;

	return data[0] + (uint32_t)data[1]<<8 + (uint32_t)data[2]<<16;
}
