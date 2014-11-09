/*
 * rf.h
 *
 *  Created on: Nov 4, 2014
 *      Author: Nick
 */

#ifndef RF_H_
#define RF_H_

// power modes
#define RF_RECEIVER_ON		0x8288
#define RF_XMITTER_ON		0x8229
#define RF_IDLE_MODE		0x8209

#define RF_CONFIG_915MHZ	0x8077
#define RF_CONFIG_915MHZ_TX	0x80F7
#define RF_CONFIG_FREQ		0xA640

#define RF_SYNC_BYTE		0x11

void RF0_init ( );
void RF1_init ( );
void RF0_cmd ( uint16_t );

#endif /* RF_H_ */
