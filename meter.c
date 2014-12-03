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
static const uint8_t METER_write_config2[] = {0x90, 0x40, 0x0A, 0x06, 0x50};
static const uint8_t METER_write_interrupt_mask[] = {0x80, 0x43, 0x00, 0x00, 0x80};

static const uint8_t METER_read_power[] = {0x90, 0x05};
static const uint8_t METER_integrator_gain[] = {0x92, 0x6B, 0x00, 0x00, 0x0C};

// Calibration gains
static const uint8_t METER_voltage_gain[] = {0x90, 0x63, 0x00, 0x00, 0x50};
static const uint8_t METER_current_gain[] = {0x90, 0x61, 0x00, 0x00, 0x50};

static const uint8_t METER_begin_conv[] = {0xD5};
static const uint8_t METER_stop_conv[] = {0xD8};
static const uint8_t METER_ac_calib[] = {0xFE};
static const uint8_t METER_ac_offset[] = {0xF1};

static const uint8_t METER_read_irms[] = {0x90, 0x05};
static const uint8_t METER_read_vrms[] = {0x90, 0x06};
static const uint8_t METER_read_pavg[] = {0x90, 0x07};
static const uint8_t METER_read_qavg[] = {0x90, 0x0E};
static const uint8_t METER_read_pf[] = {0x90, 0x15};

static const uint8_t METER_set_16k_samples[] = {0x90, 0x73, 0x80, 0x3E, 0x00};

static const uint8_t METER_read_igain[] = {0x90, 0x21};
static const uint8_t METER_read_vgain[] = {0x90, 0x23};
static const uint8_t METER_read_poff[] = {0x90, 0x24};
static const uint8_t METER_read_qoff[] = {0x90, 0x26};
static const uint8_t METER_read_iacoff[] = {0x90, 0x25};
static const uint8_t METER_read_pc[] = {0x80, 0x05};

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

typedef struct {
	uint8_t i_gain[3];
	uint8_t v_gain[3];
	uint8_t i_ac_off[3];
	uint8_t p_off[3];
	uint8_t q_off[3];
	uint8_t pc[3];
} CalibParams;


void METER_calibrate ( )
{
	CalibParams *params = (CalibParams*)((uint8_t*)0x1040);

	// *** RESET

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

	// *** END RESET


	// *** SINGLE CONVERSION
//	UART_send(METER_single_conv, sizeof(METER_single_conv));
	// *** END CONVERSION


	// *** CONFIGURATION
	UART_send(METER_write_config2, sizeof(METER_write_config2));
	// *** END CONFIGURATION


	// *** SET GAINS
	UART_send(METER_voltage_gain, sizeof(METER_voltage_gain));
	UART_send(METER_voltage_gain, sizeof(METER_voltage_gain));
	// *** END

	// *** START CONVERSION
	UART_send(METER_begin_conv, sizeof(METER_begin_conv));
	// *** END


	// *** READ VALUES
	__METER_poll_drdy();

	UART_send(METER_read_irms, sizeof(METER_read_irms));
	UART_send(METER_read_vrms, sizeof(METER_read_vrms));
	UART_send(METER_read_pavg, sizeof(METER_read_pavg));
	UART_send(METER_read_qavg, sizeof(METER_read_qavg));
	UART_send(METER_read_pf, sizeof(METER_read_pf));

	UART_send(METER_stop_conv, sizeof(METER_stop_conv));
	// *** END

	// *** WAIT TWO SECONDS to settle
	__delay_cycles(2*16000000);
	// *** END

	// *** SAMPLE COUNT to 16,000
	UART_send(METER_set_16k_samples, sizeof(METER_set_16k_samples));
	// ***

	// *** CLEAR DRDY
	UART_send(METER_clear_drdy, sizeof(METER_clear_drdy));
	// *** END

	// *** START AC GAIN CALIBRATION
	UART_send(METER_ac_calib, sizeof(METER_ac_calib));
	__METER_poll_drdy();
	// *** END AC CALIBRATION

	// *** WAIT TWO SECONDS to settle
	__delay_cycles(2*16000000);
	// *** END

	// *** CLEAR DRDY
	UART_send(METER_clear_drdy, sizeof(METER_clear_drdy));
	// *** END

	// *** START AC OFFSET CALIBRATION
	UART_send(METER_ac_offset, sizeof(METER_ac_offset));
	__METER_poll_drdy();
	// *** END AC CALIBRATION

	// erase flash
	FCTL2 = FWKEY + FSSEL0 + FN4;
	FCTL1 = FWKEY | ERASE;
	FCTL3 = FWKEY;
	*(uint8_t*)params = 0;

	// enable write to flash
	FCTL1 = FWKEY | WRT;

	UART_clear();

	UART_send(METER_read_vgain, sizeof(METER_read_vgain));
	UART_recv(&params->v_gain, sizeof(params->v_gain), '\0', USCI_BLOCKING);

	UART_send(METER_read_igain, sizeof(METER_read_igain));
	UART_recv(&params->i_gain, sizeof(params->i_gain), '\0', USCI_BLOCKING);

	UART_send(METER_read_iacoff, sizeof(METER_read_iacoff));
	UART_recv(&params->i_ac_off, sizeof(params->i_ac_off), '\0', USCI_BLOCKING);

	UART_send(METER_read_poff, sizeof(METER_read_poff));
	UART_recv(&params->p_off, sizeof(params->p_off), '\0', USCI_BLOCKING);

	UART_send(METER_read_qoff, sizeof(METER_read_qoff));
	UART_recv(&params->q_off, sizeof(params->q_off), '\0', USCI_BLOCKING);

	UART_send(METER_read_pc, sizeof(METER_read_pc));
	UART_recv(&params->pc, sizeof(params->pc), '\0', USCI_BLOCKING);

	FCTL1 = FWKEY;

	__delay_cycles(2*16000000);
	__delay_cycles(2*16000000);
	__delay_cycles(2*16000000);
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

	if (P1IFG & BIT3) {
		// erase flash
		FCTL2 = FWKEY + FSSEL0 + FN4;
		FCTL1 = FWKEY | ERASE;
		FCTL3 = FWKEY;
		*(uint8_t*)0x1040 = 0;
		P1IFG &= ~BIT3;
	}

	// Interrupt DO fired
	if (DOIFG & DO_PIN) {
		// we need access to the UART interrupts
		DOIE &= ~DO_PIN;
		enable_interrupts();

		// clear the interrupt from the power meter
		UART_send(METER_clear_drdy, sizeof(METER_clear_drdy));

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
	}
}

