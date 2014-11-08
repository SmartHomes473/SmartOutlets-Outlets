/*
 * rf.h
 *
 *  Created on: Nov 4, 2014
 *      Author: Nick
 */

#ifndef RF_H_
#define RF_H_

extern const uint8_t RF_config_tx[2];
extern const uint8_t RF_config_rx[2];
extern const uint8_t RF_power_tx[2];
extern const uint8_t RF_power_rx[2];
extern const uint8_t RF_freq[2];
extern const uint8_t RF_speed[2];
extern const uint8_t RF_rctl[2];
extern const uint8_t RF_dfltr[2];
extern const uint8_t RF_fifo[2];
extern const uint8_t RF_sync[2];
extern const uint8_t RF_afc[2];
extern const uint8_t RF_tx_config[2];
extern const uint8_t RF_pll[2];
extern const uint8_t RF_mcu[2];
extern const uint8_t RF_rx_read[2];
extern const uint8_t RF_status_read[2];

void RF_init ( );


#endif /* RF_H_ */
