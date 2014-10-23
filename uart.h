#ifndef _OUTLET_UART_H
#define _OUTLET_UART_H

#include <msp430.h>


#define UART_TX BIT1
#define UART_RX BIT2

#define UART_init_9600() \
	P1SEL = UART_TX + UART_RX; \
	P1SEL2 = UART_TX + UART_RX; \
	UCA0CTL1 |= UCSSEL_2; \
	UCA0BR0 = 104; \
	UCA0BR1 = 0; \
	UCA0MCTL = UCBRS0; \
	UCA0CTL1 = ~UCSWRST; \
	IE2 |= UCA0RXIE;

#define UART_write(CHAR) \
	while (!(IFG2&UCA0TXIFG)); \
	UCA0TXBUF = CHAR;

#endif
