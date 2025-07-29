#include "polyc_uart_hw.h"
#include "polyc_buffer_manage.h"
#include "polyc_app_uart_ctrl.h"
#include "ble2uart.h"

/*--------------------------------------------------------------------------------------------------------------------------*/

/****************!!注意!!*********************************************************/
//BD19的软中断3用于操作系统,用户可以使用软中断0、1、2
//*********************************************************************************
//初始化函数加上
#define USER_SOFT0 IRQ_SOFT0_IDX
#define USER_SOFT1 IRQ_SOFT1_IDX
#define USER_SOFT2 IRQ_SOFT2_IDX

#define USER_SOFT0_BIT IRQ_SOFT0_IDX - 60
#define USER_SOFT1_BIT IRQ_SOFT1_IDX - 60
#define USER_SOFT2_BIT IRQ_SOFT2_IDX - 60

//*********************************************************************************

//定义中断函数
___interrupt
void IRQ_SOFT1_IDX_Handle(void)
{
    //printf("# RUN IRQ_SOFT1_IDX\n");
    //清空中断位
    j32CPU(0)->ILAT_CLR |= BIT(USER_SOFT1_BIT);
}

//*********************************************************************************

void TRIGGER_SOFT1_IRQ(void)
{
	//printf("# TRIGGER SOFT IRQ!\n");
	//需要触发中断处加上
	j32CPU(0)->ILAT_SET |= BIT(USER_SOFT1_BIT);
	ble2uart_send();
}

/*--------------------------------------------------------------------------------------------------------------------------*/

void ble2uart_trans_process(void)
{
	/* 选择BLE->UART,接收到数据的处理方法 */

	#if (BLE2UART_TRANS_DEAL_MODE == 1)

	/* 触发软中断处理 */
	TRIGGER_SOFT1_IRQ();

    #elif (BLE2UART_TRANS_DEAL_MODE == 2)

	/* 推事件消息处理 */
	struct sys_event e;
    e.type = SYS_BT_EVENT;
    e.arg  = (void *)SYS_BT_EVENT_FROM_USER_DEFINED_UUID;
    e.u.dev.event = 0;
    e.u.dev.value = 0;
    sys_event_notify(&e);

	printf("--> Updata ble2uart Event!\n");

	#else

	/* 直接处理 */
	ble2uart_send();

	#endif
}

#if BLE2UART_LOG_INFO

static u16 g_log_trans_bytes_timer = NULL;
static u32 trans_total_bytes = 0;
static u32 take_buff_bytes = 0;

void log_trans_total_bytes_process(void)
{
	static u32 log_trans_cnt = 0;

	//printf("--> log_trans_total_bytes_process\n");

	if(log_trans_cnt == 0){
		printf("[TRANS] %d,%d\n",trans_total_bytes,take_buff_bytes);
	}

	log_trans_cnt++;
	if(log_trans_cnt>= 30) log_trans_cnt = 0;
}

#endif

void ble2uart_trans_update(u8 *buf,u32 buf_len)
{
	/*ble2uart 传输数据有更新 */

	#if BLE2UART_LOG_INFO

	if(g_log_trans_bytes_timer == NULL){
		g_log_trans_bytes_timer = sys_timer_add(NULL,log_trans_total_bytes_process,1000);
		//printf("--> [ADD] g_log_trans_bytes_timer = 0x%04X!!\n",g_log_trans_bytes_timer);
	}

	if(buf_len > 0) { trans_total_bytes += buf_len; }

	#endif

    circular_buffer_write(&ble2uart_cbuf_handle, (u8*)buf, buf_len);	// 写入 ble2uart_cbuf
}

void ble2uart_send(void)
{
	u16 datalen = 0;

	static u32 ble2uart_fifo_addr = 0;

	if(uart_polyc_hw_get_tx_dma_busy(uart_polyc_handle))
	{
		//printf("[TX_BUSY]\n");
		return;
	}

	//printf(" data_len %d\n",circular_buffer_get_data_len(&ble2uart_cbuf_handle));
    if(circular_buffer_get_data_len(&ble2uart_cbuf_handle))
    {
        datalen = circular_buffer_preread(&ble2uart_cbuf_handle, UART_TX_FIFO_BUFF_SIZE, &ble2uart_fifo_addr);
		//printf("ble2uart_len %d\n",datalen);
		//printf("handle_r_addr 0x%x\n",ble2uart_cbuf_handle.read_ptr);
		#if (BLE2UART_LOG_INFO == 1)
		take_buff_bytes += datalen;
		#endif
        if(datalen > 0){
			uart_polyc_hw_send_data(uart_polyc_handle,(u8*)ble2uart_fifo_addr,datalen);
        }
    }
}

void ble2uart_send_end(void)
{
	circular_buffer_preread_end(&ble2uart_cbuf_handle);
}

void ble2uart_cbuff_clear(void)
{
	#if (BLE2UART_LOG_INFO == 1)
	trans_total_bytes = 0;
	take_buff_bytes = 0;
	#endif
	circular_buffer_clear(&ble2uart_cbuf_handle);
}

void ble2uart_isr_cbk(void)
{
	ble2uart_send_end();
	uart_polyc_hw_clear_tx_dma_busy(uart_polyc_handle);
	ble2uart_send();
}
