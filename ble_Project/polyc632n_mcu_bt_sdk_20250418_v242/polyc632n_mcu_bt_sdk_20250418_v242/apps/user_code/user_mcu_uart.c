#include "system/includes.h"
#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_uart.h"
#include "virtual_mcu_main.h"
#include "user_mcu_uart.h"
#include "polyc_buffer_manage.h"


#if MCU_UART0_ENABLE
///-----UART0--GROUP1对应接口AC632N_UART0_GROUP_TXPA05_RXPA06-------
///-----UART0--GROUP2对应接口AC632N_UART0_GROUP_TXPB02_RXPB03-------
///-----UART0--GROUP3对应接口AC632N_UART0_GROUP_TXPA01_RXPA02-------
///-----注意：UART0为底层调试打印串口，请优先选用UART1或UART2做为串口通讯
///-----若使用UART0做串口通讯时，务必关闭系统打印总开关CONFIG_DEBUG_ENABLE

static u8 uart0_tx_data_buff[64];  //最大64个字节
static u8 uart0_tx_len;
static u8 uart0_recieved_flag = 0;

void mcu_uart0_init(void)
{
    polyc_set_BT_System_uart_open(UART0, MCU_UART0_GPIO, MCU_UART0_BAUD);
}


/*函数由系统底层回调，请不要其他方调用*/
///--------------------MCU接收到的串口数据反馈-------------------------
void polyc_uart0_recieved_data_func_callback(u8 *buff,u16 buff_size)
{
    printf(" --> polyc_uart0_recieved_data_func_callback:");
    printf_buf(buff,buff_size);

	uart0_tx_len = buff_size;
	if(uart0_tx_len > sizeof(uart0_tx_data_buff)){ //判断限制最大长度
        uart0_tx_len =sizeof(uart0_tx_data_buff);
	}
	memcpy(uart0_tx_data_buff, buff, uart0_tx_len);
	uart0_recieved_flag = 1;
}
///--------放在20ms，mcu_main中测试MCU每隔1s串口发送的数据包----------
void mcu_uart0_send_data(void)
{
    if(uart0_recieved_flag)
    {
    	uart0_recieved_flag = 0;
		polyc_uart_send_data(UART0, uart0_tx_data_buff, uart0_tx_len, 0);
	}
}

#endif


#if MCU_UART1_ENABLE
///-------------------串口初始化，可设置接口------------------------
///-----UART1--GROUP1对应接口632N_UART1_GROUP_TXPB04_RXPB05-------
///-----UART1--GROUP2对应接口632N_UART1_GROUP_TXPB00_RXPB01-------
///-----UART1--GROUP3对应接口632N_UART1_GROUP_TXPA07_RXPA08-------
///-----UART1--GROUP4对应接口632N_UART1_GROUP_USB_TXDP_RXDM-------

static u8 uart1_tx_data_buff[64];  //最大64个字节
static u8 uart1_tx_len;
static u8 uart1_recieved_flag = 0;

void mcu_uart1_init(void)
{
    uart2ble_block_buf_manage_init();
    polyc_set_BT_System_uart_open(UART1, MCU_UART1_GPIO, MCU_UART1_BAUD);
    /**************初始化xmodem相关函数**********/
    xmodem_comm_init();
}
u8 polyc_uart1_get_send_busy(void)
{
    return polyc_uart_get_send_busy(UART1);
}

/*函数由系统底层回调，请不要其他方调用*/
///--------------------MCU接收到的串口数据反馈-------------------------
void polyc_uart1_recieved_data_func_callback(u8 *buff,u16 buff_size)
{
    printf(" --> polyc_uart1_recieved_data_func_callback:");
    printf_buf(buff,buff_size);
    uart1_recieved_flag = 0;
    #if  0
	uart1_tx_len = buff_size;
	if(uart1_tx_len > sizeof(uart1_tx_data_buff)){ //判断限制最大长度
        uart1_tx_len =sizeof(uart1_tx_data_buff);
	}
	memcpy(uart1_tx_data_buff, buff, uart1_tx_len);
	#endif // 0
	uart_isr_cbk(buff,buff_size);
	uart1_recieved_flag = 1;
}
///--------放在20ms，mcu_main中测试MCU每隔1s串口发送的数据包----------
void mcu_uart1_send_data(void)
{
    #if  0
    if(uart1_recieved_flag)
    {
    	uart1_recieved_flag = 0;
		polyc_uart_send_data(UART1,uart1_tx_data_buff,uart1_tx_len,0);
	}
	#endif // 0
	/*********这里是uart发数目前用于mcu OTA**********/
	uart_send_handler();
}
u8 get_uart1_recieved_flag()
{
    return uart1_recieved_flag;
}

#endif


#if MCU_UART2_ENABLE
///-------------------串口初始化，可设置接口------------------------
///-----UART2--GROUP1对应接口632N_UART2_GROUP_TXPA03_RXPA04-------
///-----UART2--GROUP3对应接口632N_UART2_GROUP_TXPB06_RXPB07-------
///-----UART2--GROUP4对应接口632N_UART2_GROUP_TXPDP1_RXPDM1-------


static u8 uart2_tx_data_buff[64]; //最大64个字节
static u8 uart2_tx_len;
static u8 uart2_recieved_flag = 0;

void mcu_uart2_init(void)
{
    polyc_set_BT_System_uart_open(UART2, MCU_UART2_GPIO, MCU_UART2_BAUD);
}

/*函数由系统底层回调，请不要其他方调用*/
///--------------------MCU接收到的串口数据反馈-------------------------
void polyc_uart2_recieved_data_func_callback(u8 *buff,u16 buff_size)
{
    printf(" --> polyc_uart2_recieved_data_func_callback:");
    printf_buf(buff,buff_size);

	uart2_tx_len = buff_size;
	if(uart2_tx_len > sizeof(uart2_tx_data_buff)){ //判断限制最大长度
        uart2_tx_len =sizeof(uart2_tx_data_buff);
	}
	memcpy(uart2_tx_data_buff, buff, uart2_tx_len);
	uart2_recieved_flag = 1;
}
///--------放在20ms，mcu_main中测试MCU每隔1s串口发送的数据包----------
void mcu_uart2_send_data(void)
{
    if(uart2_recieved_flag)
    {
    	uart2_recieved_flag = 0;
		polyc_uart_send_data(UART2,uart2_tx_data_buff,uart2_tx_len,0);
	}
}

#endif
