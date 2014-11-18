/*************************************************************
 *
 * Control interface for power relay.
 *
 * Author: Nick Jurgens <njurgens@umich.edu>
 *
 *************************************************************/

#include "msp430g2553.h"


// The relay is controlled by pin 1.0
#define relayPin BIT0


/**
 * Initialize relay control pin.
 */
void RELAY_init ( )
{
	// Set relay control pin to IO mode
	P1SEL &= ~relayPin;
	P1SEL2 &= ~relayPin;

	// Default to off
	P1OUT &= ~relayPin;

	// Set pin as output
	P1DIR |= relayPin;
}


/**
 * Turn on the relay.
 */
void RELAY_on ( )
{
	P1OUT |= relayPin;
}


/**
 * Turn off the relay.
 */
void RELAY_off ( )
{
	P1OUT &= ~relayPin;
}
