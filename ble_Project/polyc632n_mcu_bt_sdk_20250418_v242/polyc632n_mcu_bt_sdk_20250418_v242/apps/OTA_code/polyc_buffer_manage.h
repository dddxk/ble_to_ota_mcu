#ifndef __POLYC_BUFFER_MANAGE_H__
#define __POLYC_BUFFER_MANAGE_H__

#include "asm/cpu.h"

/*-------------------------------------------------------------------------------------------------------------*/
#define UART_RX_BLOCK_NUM			(8)
#define UART_RX_BLOCK_SIZE          64
#define UART_RX_BUFF_SIZE			(UART_RX_BLOCK_NUM*UART_RX_BLOCK_SIZE)

typedef void (*block_buf_cbk)(void *buf,u16 buf_len); //函数指针类型定义
typedef void (*block_critical_cbk)(void);

typedef struct _block_ctrl
{
	u16	pos_read;
    u16	valid_data_len;
}block_ctrl;

typedef struct _block_buf_t
{
	u8 read;
	u8 write;
	u8 used_sum;
	u8 *buf_base_addr;

	u16 per_block_size;
	u8 block_total_num;
	block_ctrl  *block_per_ctrl;

	block_buf_cbk write_cbk;
	block_buf_cbk read_cbk;
	block_critical_cbk enter_critical;
	block_critical_cbk exit_critical;
}block_buf_t;

/*-------------------------------------------------------------------------------------------------------------*/
void block_buf_init(block_buf_t * block_buf_handle,u32 *block_buff,block_ctrl *block_per_ctrl,u8 block_total_num,u16 per_block_size);
void register_block_buf_critical_callback(block_buf_t *block_buf_handle,block_critical_cbk *enter_cbk,block_critical_cbk *exit_cbk);
void register_block_buf_operate_callback(block_buf_t *block_buf_handle,block_buf_cbk *write_cbk,block_buf_cbk *read_cbk);
u8 block_buf_used_sum(block_buf_t *block_buf);
u16 block_buf_valid_data_len(block_buf_t *block_buf);
u16 block_buf_data_read(block_buf_t *block_buf,u16 need_read, u32 *addr_to_read);
void block_buf_data_write(block_buf_t *block_buf,u32 *buf,u16 buf_len);


void uart2ble_block_buf_manage_init(void);
void uart_isr_cbk(u32 *buf,u16 buf_len);
void uart_send_handler(void);
#endif
