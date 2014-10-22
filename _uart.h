/*
 * _uart.h
 *
 *  Created on: Oct 22, 2014
 *      Author: Nick
 */

#ifndef UART_H_
#define UART_H_

/**
 * Initialize USCI_A0 in UART mode.
 *
 * Initialization consists of:
 *   1. set UCSWRST bit in UCA0CTL1
 *   2. set UCSWRST in other registers
 *   3. configure IO pins
 *   4. clear UCSWRST in UCA0CTL1
 */
#define init_UART()\
	UCA0CTL1 |= UCSWRST;\
	UCA0CTL1 &= ~(UCSWRST);

#endif /* UART_H_ */
