#ifndef __BLE2UART_H__
#define __BLE2UART_H__

#include "asm/cpu.h"

#define BLE2UART_TRANS_DEAL_MODE    (0)
#define BLE2UART_LOG_INFO			(0)

void ble2uart_trans_process(void);
void ble2uart_trans_update(u8 *buf,u32 buf_len);
void ble2uart_send(void);
void ble2uart_send_end(void);
void ble2uart_cbuff_clear(void);
void ble2uart_isr_cbk(void);

#endif
