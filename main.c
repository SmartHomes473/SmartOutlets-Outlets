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
//            |             P1.2|-->LED
//
//  J. Stevenson
//  Texas Instruments, Inc
//  July 2011
//  Built with Code Composer Studio v5
//***************************************************************************************

#include "msp430g2553.h"
#include "sys/init.h"
#include "drivers/uart.h"

int main(void)
{
	// Initialize MSP430
	MSP430_init();

	// Start UART
	UART_init_9600();

//	const char *str = "foo\n";
//	UART_write_str(str);


	UART_write_byte('a');
	UART_write_str("hello!\n");

	// Enable interrupts
    __bis_SR_register(GIE);

	while(1);
}

__attribute__((interrupt(USCIAB0RX_VECTOR)))
void USCI0RX_ISR(void)
{
    while (!(IFG2&UCA0TXIFG));
    UCA0TXBUF = UCA0RXBUF;
}
