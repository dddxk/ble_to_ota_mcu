#include "polyc_buffer_manage.h"


/* block buffer */
void block_buf_init(block_buf_t * block_buf_handle,u32 *block_buff,block_ctrl *block_per_ctrl,u8 block_total_num,u16 per_block_size)
{
	u8 block_num;

	printf("\n --> block_buf_init! \n\n");

	block_buf_handle->read = 0;
	block_buf_handle->write = 0;
	block_buf_handle->used_sum = 0;

	block_buf_handle->buf_base_addr = (char *)block_buff;
	//printf(" Block_base_addr 0x%X\n",block_buf_handle->buf_base_addr);
	block_buf_handle->block_per_ctrl = block_per_ctrl;
	//printf(" Block_per_ctrl 0x%X ",block_buf_handle->block_per_ctrl);

	block_buf_handle->block_total_num = block_total_num;
	block_buf_handle->per_block_size = per_block_size;

	for(block_num = 0;block_num < block_buf_handle->block_total_num;block_num++)
	{
		block_buf_handle->block_per_ctrl[block_num].pos_read = 0;
		block_buf_handle->block_per_ctrl[block_num].valid_data_len = 0;
	}
}

void register_block_buf_critical_callback(block_buf_t *block_buf_handle,block_critical_cbk *enter_cbk,block_critical_cbk *exit_cbk)
{
	block_buf_handle->enter_critical = enter_cbk;
	block_buf_handle->exit_critical = exit_cbk;
}

void register_block_buf_operate_callback(block_buf_t *block_buf_handle,block_buf_cbk *write_cbk,block_buf_cbk *read_cbk)
{
	block_buf_handle->write_cbk = write_cbk;
	block_buf_handle->read_cbk = read_cbk;
}


u8 block_buf_used_sum(block_buf_t * block_buf)
{
    u8 temp = 0;

	if(block_buf->enter_critical){
		block_buf->enter_critical();
	}

    temp = block_buf->used_sum;

	if(block_buf->exit_critical){
		block_buf->exit_critical();
	}

    return temp;
}

u16 block_buf_valid_data_len(block_buf_t * block_buf)
{
    u8 i;
    u16 temp = 0;

	if(block_buf->enter_critical){
		block_buf->enter_critical();
	}

    for(i=0; i< block_buf->block_total_num; i++){
        temp += block_buf->block_per_ctrl[i].valid_data_len;
    }

	if(block_buf->exit_critical){
		block_buf->exit_critical();
	}

    return temp;
}


u16 block_buf_data_read(block_buf_t * block_buf,u16 need_read, u32 *addr_to_read)
{
    u16 data_len = 0;

	if(block_buf->enter_critical){
		block_buf->enter_critical();
	}

    if(block_buf->used_sum)
    {
    	//printf("#1 block_addr 0x%x\n",block_buf->buf_base_addr);
        *addr_to_read = (u32)(block_buf->buf_base_addr + block_buf->read * block_buf->per_block_size + block_buf->block_per_ctrl[block_buf->read].pos_read );
		//printf("#2 block_addr[%d] 0x%x\n",block_buf->read ,*addr_to_read);

        if( need_read > block_buf->block_per_ctrl[block_buf->read].valid_data_len ){
            need_read = block_buf->block_per_ctrl[block_buf->read].valid_data_len;
        }
        data_len = need_read;

        block_buf->block_per_ctrl[block_buf->read].valid_data_len -= data_len;
        block_buf->block_per_ctrl[block_buf->read].pos_read += data_len;

        if(block_buf->block_per_ctrl[block_buf->read].valid_data_len == 0)
        {
            block_buf->used_sum--;
            block_buf->block_per_ctrl[block_buf->read].pos_read = 0;
            if(++(block_buf->read) >= block_buf->block_total_num){
                block_buf->read = 0;
            }
        }
    }

	if(block_buf->exit_critical){
		block_buf->exit_critical();
	}

	if(block_buf->read_cbk){
		block_buf->read_cbk(NULL,block_buf->read);
	}

    return data_len;
}


void block_buf_data_write(block_buf_t *block_buf,u32 *buf,u16 buf_len)
{

	if(block_buf->used_sum < block_buf->block_total_num)
	{
		block_buf->used_sum++;
		block_buf->block_per_ctrl[block_buf->write].pos_read = 0;
		block_buf->block_per_ctrl[block_buf->write].valid_data_len = buf_len;
		memcpy(block_buf->buf_base_addr+(block_buf->write*block_buf->per_block_size),buf,buf_len);
		block_buf->write++;
		if( block_buf->write >= block_buf->block_total_num ){
			block_buf->write = 0;
		}
	}

	//printf("dma_addr 0x%X\n",block_buf->buf_base_addr+(block_buf->write*block_buf->per_block_size));
//	if(block_buf->write_cbk){
//		block_buf->write_cbk(block_buf->buf_base_addr+(block_buf->write*block_buf->per_block_size),block_buf->per_block_size);
//	}
}


block_buf_t uart2ble_block_buf_handle;
block_ctrl uart2ble_block_ctrl[UART_RX_BLOCK_NUM];

static u8 uart_rx_buff[UART_RX_BUFF_SIZE] __attribute__((aligned(4)));

void uart2ble_block_buf_manage_init(void)
{
	memset(uart_rx_buff, 0x00, UART_RX_BLOCK_SIZE);

	//printf("uart2ble_block_ctrl 0x%x\n",uart2ble_block_ctrl);
	block_buf_init(&uart2ble_block_buf_handle,uart_rx_buff,uart2ble_block_ctrl,UART_RX_BLOCK_NUM,UART_RX_BLOCK_SIZE);
	register_block_buf_critical_callback(&uart2ble_block_buf_handle,NULL,NULL);
	register_block_buf_operate_callback(&uart2ble_block_buf_handle,NULL,NULL);

}

/**************串口收到数之后写到block buff里面**************************/
void uart_isr_cbk(u32 *buf,u16 buf_len)
{
	printf("uart_isr: buf_len %d\n",buf_len);
	if(buf_len > 0){
		block_buf_data_write(&uart2ble_block_buf_handle,buf,buf_len);
	}
}

/**************查询block buff里面是否还有数据**************************/
extern u8 get_uart1_recieved_flag();
extern u8 polyc_uart1_get_send_busy(void);
void uart_send_handler(void)
{
    if(0==get_uart1_recieved_flag())return;
    if(polyc_uart1_get_send_busy())return;

	u16 datalen = 0;

    u16 datalen_send;
    u16 datalen_remain;
    u32 rxbuf_addr_read = 0;

	u16 ble_buff_remain = 0;

    datalen = block_buf_valid_data_len(&uart2ble_block_buf_handle);//获取block_buff有效数据
    if(datalen == 0){ return ; }

    datalen_remain = datalen;
	printf("# block_read %d\n",uart2ble_block_buf_handle.read);
    datalen_send = block_buf_data_read(&uart2ble_block_buf_handle,datalen_remain, &rxbuf_addr_read);
	printf("datalen_send %d\n",datalen_send);

	/******************在这里取数后处理的**************************/
	extern int xmodem_comm(u8 *buffer,u16 buffer_size);
	int ret = xmodem_comm((u8*)rxbuf_addr_read, datalen_send);

	if(ret < 0) { printf("error_result = %d\n",ret); }

}
