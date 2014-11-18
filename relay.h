/*************************************************************
 *
 * Control interface for power relay.
 *
 * Author: Nick Jurgens <njurgens@umich.edu>
 *
 *************************************************************/
#ifndef __OUTLETS_RELAY_H
#define __OUTLETS_RELAY_H


/**
 * Initialize relay control pin.
 */
void RELAY_init ( );


/**
 * Turn on the relay.
 */
void RELAY_on ( );


/**
 * Turn off the relay.
 */
void RELAY_off ( );


#endif
