/*
 * clock.h
 *
 *  Created on: Nov 23, 2014
 *      Author: Nick
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "msp430g2553.h"

#define CLOCK_16MHZ() {\
	DCOCTL = 0;\
	BCSCTL1 = CALBC1_16MHZ;\
	DCOCTL = CALDCO_16MHZ;\
}

#define CLOCK_1MHZ() {\
	DCOCTL = 0;\
	BCSCTL1 = CALBC1_1MHZ;\
	DCOCTL = CALDCO_1MHZ;\
}

#endif /* CLOCK_H_ */
