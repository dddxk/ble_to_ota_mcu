#ifndef __UART2BLE_H__
#define __UART2BLE_H__

#include "app_config.h"
#include "asm/cpu.h"

extern u16 bt_ble_is_connected(void);

void uart2ble_block_buf_dma_receive(void *buf,u16 buf_len);
void uart2ble_trans_update(void *buf,u16 buf_len);
void uart2ble_send_handler(void);
void uart2ble_send_last_deal(void);
void uart2ble_isr_cbk(u32 *buf,u16 buf_len);

#endif
