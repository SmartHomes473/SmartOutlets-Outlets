#ifndef __OUTLETS_INIT_H
#define __OUTLETS_INIT_H

#include <msp430.h>

#define MSP430_init()\
	__MSP430_init_watchdog();\
	__MSP430_init_clk();

#define __MSP430_init_watchdog()\
	WDTCTL = WDTPW + WDTHOLD;

#define __MSP430_init_clk()\
	DCOCTL = 0;\
	BCSCTL1 = CALBC1_1MHZ;\
	DCOCTL = CALDCO_1MHZ;




#endif
