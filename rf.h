/*
 * rf.h
 *
 *  Created on: Nov 4, 2014
 *      Author: Nick
 */

#ifndef RF_H_
#define RF_H_

#define RF_SYNC_BYTE1	0x2D
#define RF_SYNC_BYTE0	0xD4

#define RF_CONFIG_RX	0x8078
#define RF_CONFIG_TX	0x80B8

#define RF_POWER_RX		0x8281
#define RF_POWER_TX		0x8221

#define RF_CENTER_FREQ	0xA680
#define RF_DATA_RATE	0xC602
#define RF_RECV_CTL		0x9465
#define RF_DATA_FILTER	0xC2EC
#define RF_FIFO_SYNC	0xCA8B
#define RF_SYNC_MODE	0xCE00 | RF_SYNC_BYTE0
#define RF_AFC_CMD		0xC483
#define RF_TX_CTL		0x9880
#define RF_PLL_CFG		0xCC77
#define RF_WAKEUP		0xE000
#define RF_DUTY_CYCLE	0xC800
#define RF_LOW_BATTERY	0xC000

#define RF_FIFO_READ	0xB000
#define RF_STATUS_READ	0x1000

void RF0_init ( );
void RF1_init ( );
void send_cmd ( uint16_t );

#endif /* RF_H_ */