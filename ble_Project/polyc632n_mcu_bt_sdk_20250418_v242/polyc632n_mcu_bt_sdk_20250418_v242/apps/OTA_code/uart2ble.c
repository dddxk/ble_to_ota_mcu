#include "polyc_uart_hw.h"
#include "polyc_buffer_manage.h"
#include "polyc_app_uart_ctrl.h"
#include "uart2ble.h"
#include "system/event.h"
#include "polyc_cmd_process_common.h"


//函数app_send_user_data()为带缓存拷贝,非阻塞类型
//根据实际情况修改BLE_CBUF[ATT_SEND_CBUF_SIZE]
static void post_uart2ble_event(void)
{
	struct sys_event e;
	e.type = SYS_DEVICE_EVENT;
	e.arg  = (void *)DEVICE_EVENT_FROM_LOCAL_UART;
	e.u.dev.event = 0;
	e.u.dev.value = 0;
	sys_event_notify(&e);
}

void uart2ble_block_buf_dma_receive(void *buf,u16 buf_len)
{
	//printf("# dma_addr 0x%X,dma_len %d\n",buf,buf_len);
	uart_polyc_hw_set_recieve_data_buffer(uart_polyc_handle,buf,buf_len);
	post_uart2ble_event();
}

void uart2ble_send_handler(void)
{
	u16 datalen = 0;

    u16 datalen_send;
    u16 datalen_remain;
    u32 rxbuf_addr_read = 0;

	u16 ble_buff_remain = 0;

    datalen = block_buf_valid_data_len(&uart2ble_block_buf_handle);//获取block_buff有效数据
    if(datalen == 0){ return ; }

	/*if(bt_ble_is_connected()){
		ble_buff_remain = get_ble_buffer_remain_len();//获取ble缓存剩余空间
		//printf("#datalen %d , ble_remain %d\n",datalen,ble_buff_remain);

		if(ble_buff_remain == 0){
			//printf("# ble_buff_remain = 0 !!\n");
			return;
		}

		if(ble_buff_remain < datalen){
			printf("uart2ble input data to fast!\n");
			datalen_remain = ble_buff_remain;
		}
		else{
			if(datalen >= UART_RX_FIFO_SIZE){
		        datalen_remain = UART_RX_FIFO_SIZE;
		    }else{
		        datalen_remain = datalen;
		    }
		}
	}
	else*/{
		datalen_remain = datalen;
	}

	//printf("# block_read %d\n",uart2ble_block_buf_handle.read);
    datalen_send = block_buf_data_read(&uart2ble_block_buf_handle,datalen_remain, &rxbuf_addr_read);

	//printf("datalen_send %d\n",datalen_send);

	extern int xmodem_comm(u8 *buffer,u16 buffer_size);
	int ret = xmodem_comm((u8*)rxbuf_addr_read, datalen_send);

	if(ret < 0) { printf("error_result = %d\n",ret); }
	/* else{ printf("trans_len = %d\n",ret);} */

	/*if((bt_ble_is_connected()== 0)||(gpio_read(MODULE_SELECT_TRANS_IO) == 1)){
		ployc_private_cmd_process((u8*)rxbuf_addr_read, datalen_send);
	}
	else{
		uart2ble_send_data((u8*)rxbuf_addr_read, datalen_send);
	}*/
}

void uart2ble_send_last_deal(void)
{
	u16 datalen = 0;

	u16 datalen_send;
    u16 datalen_remain;
    u32 rxbuf_addr_read = 0;

	datalen = block_buf_valid_data_len(&uart2ble_block_buf_handle);
    if(datalen == 0){ return ; }

	//printf("# uart2ble send end [%d]!\n",uart2ble_block_buf_handle.read);

	datalen_send = block_buf_data_read(&uart2ble_block_buf_handle,datalen, &rxbuf_addr_read);

    uart2ble_send_data((u8*)rxbuf_addr_read, datalen_send);
}


void uart2ble_isr_cbk(u32 *buf,u16 buf_len)
{
	//printf("uart2ble_isr: buf_len %d\n",buf_len);

	if(buf_len > 0){
		block_buf_data_write(&uart2ble_block_buf_handle,buf,buf_len);
	}
}
