/*
 * meter.h
 *
 *  Created on: Nov 4, 2014
 *      Author: Nick
 */

#ifndef METER_H_
#define METER_H_

#include <stdint.h>

extern volatile uint32_t metered_power;

void METER_init ( );
void METER_begin ( );

#endif /* METER_H_ */
