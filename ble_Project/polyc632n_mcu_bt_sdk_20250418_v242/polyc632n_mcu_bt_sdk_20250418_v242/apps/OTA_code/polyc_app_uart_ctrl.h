#ifndef __POLYC_APP_UART_CTRL_H__
#define __POLYC_APP_UART_CTRL_H__

#include "polyc_buffer_manage.h"
#include "ble_user.h"
#include "uart2ble.h"
#include "ble2uart.h"

/*--------------------------------------------------------------------------------------------------*/

#define UART_TX_FIFO_BUFF_SIZE		(64)
#define UART_TX_BUFF_SIZE			(4096)  		// AC632n串口发送缓存

#define UART_RX_BLOCK_NUM			(8) 			// AC632n串口接收缓存
#define UART_RX_BLOCK_SIZE			(512)

#define UART_RX_FIFO_SIZE			(UART_RX_BLOCK_SIZE)
#define UART_RX_BUFF_SIZE			(UART_RX_BLOCK_NUM*UART_RX_BLOCK_SIZE)

/*--------------------------------------------------------------------------------------------------*/

extern u8 uart_polyc_handle;
extern block_buf_t uart2ble_block_buf_handle;
extern circular_buffer_t ble2uart_cbuf_handle;

extern void polyc_app_uart_ctrl_init(void);

extern u32 get_ble_buffer_remain_len(void);
extern int uart2ble_send_data(u8 *buffer,u16 buffer_len);


#endif
