//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  J. Stevenson
//  Texas Instruments, Inc
//  July 2011
//  Built with Code Composer Studio v5
//***************************************************************************************

#include <msp430g2553.h>
#include "io.h"

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

	// Initialize GPIO pins
	init_IO();

	// Enable pin 1.0 and set it as an output
	enable_P1_0();
	dir_P1_0(1);

	// Program loop
	for (;;)
	{
		// Toggle the LED connected to pin 1.0
		toggle_P1_0();
		for (volatile int i = 1000; i != 0; --i);
	}
}
