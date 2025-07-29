#include "app_config.h"
#include "asm/cpu.h"
#include "asm/wdt.h"
#include "polyc_uart_hw.h"
#include "polyc_app_uart_ctrl.h"

/*--------------------------------------------------------------------------------------------------------------------------*/

u8 uart_polyc_handle;

block_buf_t uart2ble_block_buf_handle;
block_ctrl uart2ble_block_ctrl[UART_RX_BLOCK_NUM];

circular_buffer_t ble2uart_cbuf_handle;

static u8 uart_rx_buff[UART_RX_BUFF_SIZE] __attribute__((aligned(4)));
static u8 uart_tx_buff[UART_TX_BUFF_SIZE] __attribute__((aligned(4)));

/*--------------------------------------------------------------------------------------------------------------------------*/
/* 避免出现长时间收发数据频繁产生中断，导致系统WDT复位 */

#define WDT_TX_TYPE   			 (0)
#define WDT_RX_TYPE				 (1)

#define WDT_UART_TX_CLEAR_CNT    (30)
#define WDT_UART_RX_CLEAR_CNT    (30)

void wdt_auto_clear(u8 is_tx_rx)
{
	static u8 tx_auto_cnt = 0;
	static u8 rx_auto_cnt = 0;

	if(is_tx_rx == WDT_RX_TYPE) {
		rx_auto_cnt++;
		if(rx_auto_cnt >= WDT_UART_RX_CLEAR_CNT){
			rx_auto_cnt = 0;
			wdt_clear();
		}
	}
	else {
		tx_auto_cnt++;
		if(tx_auto_cnt >= WDT_UART_TX_CLEAR_CNT){
			tx_auto_cnt = 0;
			wdt_clear();
		}
	}
}

/*--------------------------------------------------------------------------------------------------------------------------*/

void uart_polyc_send_packet(u8 *buf, u32 buf_len)
{
    //printf("BLE --> MCU_UART\n");
    //printf_buf(buf,buf_len);
    uart_polyc_hw_send_data(uart_polyc_handle, buf, buf_len);
}

/*--------------------------------------------------------------------------------------------------------------------------*/

void polyc_uart_init(void)
{
    printf("\n ----- polyc_uart_init ----- \n\n");

    uart_polyc_hw_open(uart_polyc_handle, USED_UART_GROUP, UART_BAUD_115200);

    uart_polyc_hw_set_recieve_data_buffer(uart_polyc_handle, uart_rx_buff, UART_RX_BLOCK_SIZE);

    uart_polyc_hw_register_recieved_data_callback(uart_polyc_handle,uart2ble_isr_cbk);

	uart_polyc_hw_register_send_finished_data_callback(uart_polyc_handle,ble2uart_isr_cbk);
}


void polyc_uart_colse(void)
{
	uart_polyc_hw_close(uart_polyc_handle);
}

/*--------------------------------------------------------------------------------------------------------------------------*/

void CBUFF_ENTER_CRITICAL(void)
{
	//printf("# %s \n",__func__);
	uart_polyc_hw_disable_irq(uart_polyc_handle);
}

void CBUFF_EXIT_CRITICAL(void)
{
	//printf("# %s \n",__func__);
	uart_polyc_hw_enable_irq(uart_polyc_handle);
}

void ble2uart_cbuf_manage_init(void)
{
	memset(uart_tx_buff,0x00,UART_TX_BUFF_SIZE);
	/* 初始化ble->uart的cbuf */
	circular_buffer_init(&ble2uart_cbuf_handle, (u32)uart_tx_buff, UART_TX_BUFF_SIZE);
	register_cbuf_critical_callback(&ble2uart_cbuf_handle,CBUFF_ENTER_CRITICAL,CBUFF_EXIT_CRITICAL);
	register_cbuf_operate_callback(&ble2uart_cbuf_handle,ble2uart_trans_process,NULL);
}

/*--------------------------------------------------------------------------------------------------------------------------*/

void BLOCK_BUFF_ENTER_CRITICAL(void)
{
	//printf("# %s \n",__func__);
	uart_polyc_hw_disable_irq(uart_polyc_handle);
}

void BLOCK_BUFF_EXIT_CRITICAL(void)
{
	//printf("# %s \n",__func__);
	uart_polyc_hw_enable_irq(uart_polyc_handle);
}

void uart2ble_block_buf_manage_init(void)
{
	memset(uart_rx_buff, 0x00, UART_RX_BUFF_SIZE);

	//printf("uart2ble_block_ctrl 0x%x\n",uart2ble_block_ctrl);
	block_buf_init(&uart2ble_block_buf_handle,uart_rx_buff,uart2ble_block_ctrl,UART_RX_BLOCK_NUM,UART_RX_BLOCK_SIZE);
	register_block_buf_critical_callback(&uart2ble_block_buf_handle,BLOCK_BUFF_ENTER_CRITICAL,BLOCK_BUFF_EXIT_CRITICAL);
	register_block_buf_operate_callback(&uart2ble_block_buf_handle,uart2ble_block_buf_dma_receive,NULL);
}

/*--------------------------------------------------------------------------------------------------------------------------*/

void polyc_app_uart_ctrl_init(void)
{
	uart_polyc_handle = USED_UART_INDEX; /* 使用的UART_INDEX必需先初始化 */

	ble2uart_cbuf_manage_init();
	uart2ble_block_buf_manage_init();

	polyc_uart_init();

	#if (BLE2UART_TRANS_DEAL_MODE == 1)
	/* 注册软中断 */
	//void request_irq(u8 index, u8 priority, void (*handler)(void), u8 cpu_id);
	//priority的取值范围(0~3) 0为最高优先级 3为最低优先级
	printf("# register soft irq!\n");
	request_irq(USER_SOFT1, 0, IRQ_SOFT1_IDX_Handle, 0);
	#endif
}
/*--------------------------------------------------------------------------------------------------------------------------*/
