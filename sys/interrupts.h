#ifndef __OUTLETS_INTERRUPTS_H
#define __OUTLETS_INTERRUPTS_H

#include "msp430g2553.h"

/**
 * Macros to enable/dissable interrupts.
 */
#define enable_interrupts() __bis_SR_register(GIE);
#define disable_interrupts() __bic_SR_register(GIE);

#endif
