#ifndef __USER_MCU_UART_H__
#define __USER_MCU_UART_H__



/*----------------------------串口使能模块---------------------------------
    串口模块默认蓝牙模块板子上的TX，RX接口，波特率可根据需求更改
    支持的波特率索引号
	BAUD_2400,
	BAUD_4800,
	BAUD_9600,
	BAUD_19200,
	BAUD_38400,
	BAUD_57600,
	BAUD_115200,
	BAUD_230400,
	BAUD_460800,
------------------------------------------------------------------------*/
///-----UART0--GROUP1对应接口AC632N_UART0_GROUP_TXPA05_RXPA06-------
///-----UART0--GROUP2对应接口AC632N_UART0_GROUP_TXPB02_RXPB03-------
///-----UART0--GROUP3对应接口AC632N_UART0_GROUP_TXPA01_RXPA02-------
///-----注意：UART0为底层调试打印串口，请优先选用UART1或UART2做为串口通讯
///-----若使用UART0做串口通讯时，务必关闭系统打印总开关CONFIG_DEBUG_ENABLE
#define MCU_UART0_ENABLE                    0      ///串口0通讯使能

#if  MCU_UART0_ENABLE

    #define MCU_UART0_BAUD                      BAUD_115200
    #define MCU_UART0_GPIO                      GROUP3
    void mcu_uart0_init(void);
    void mcu_uart0_send_data(void);

#endif // MCU_UART0_ENABLE


///-----UART1--GROUP1对应接口632N_UART1_GROUP_TXPB04_RXPB05-------
///-----UART1--GROUP2对应接口632N_UART1_GROUP_TXPB00_RXPB01-------
///-----UART1--GROUP3对应接口632N_UART1_GROUP_TXPA07_RXPA08-------
///-----UART1--GROUP4对应接口632N_UART1_GROUP_USB_TXDP_RXDM-------

#define MCU_UART1_ENABLE                    1      ///串口1通讯使能


#if  MCU_UART1_ENABLE
    #define MCU_UART1_BAUD                      BAUD_115200
    #define MCU_UART1_GPIO                      GROUP1
    void mcu_uart1_init(void);
    void mcu_uart1_send_data(void);
#endif // MCU_UART1_ENABLE
///-----UART2--GROUP1对应接口632N_UART2_GROUP_TXPA03_RXPA04-------
///-----UART2--GROUP3对应接口632N_UART2_GROUP_TXPB06_RXPB07-------
///-----UART2--GROUP4对应接口632N_UART2_GROUP_TXPDP1_RXPDM1-------

#define MCU_UART2_ENABLE                    0      ///串口2通讯使能


#if  MCU_UART2_ENABLE
    #define MCU_UART2_BAUD                      BAUD_115200
    #define MCU_UART2_GPIO                      GROUP1
    void mcu_uart2_init(void);
    void mcu_uart2_send_data(void);
#endif // MCU_UART2_ENABLE


#endif
