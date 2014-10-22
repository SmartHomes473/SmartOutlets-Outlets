/*
 * init.h
 *
 *  Created on: Oct 21, 2014
 *      Author: Nick
 */

#ifndef INIT_H_
#define INIT_H_

#include <msp430g2553.h>

// include IO module headers
#include "_io.h"
#include "_uart.h"

// IO pins
#define PIN0 BIT0
#define PIN1 BIT1
#define PIN2 BIT2
#define PIN3 BIT3
#define PIN4 BIT4
#define PIN5 BIT5
#define PIN6 BIT6
#define PIN7 BIT7

/**
 * Macro to initialize GPIO.
 *
 * Initializes all pins to IO mode and as output pins.
 * Should be called before manipulating any IO pins.
 */
#define init_IO() \
	P1SEL2 = P1SEL = 0x0; \
	P2SEL2 = P2SEL = 0x0; \
	P3SEL2 = P3SEL = 0x0; \
	P1DIR = P2DIR = P3DIR = 0xFF; \


#endif /* INIT_H_ */
