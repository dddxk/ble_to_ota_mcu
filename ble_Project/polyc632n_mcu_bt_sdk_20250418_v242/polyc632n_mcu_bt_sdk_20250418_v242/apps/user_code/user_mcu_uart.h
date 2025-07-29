#ifndef __USER_MCU_UART_H__
#define __USER_MCU_UART_H__



/*----------------------------����ʹ��ģ��---------------------------------
    ����ģ��Ĭ������ģ������ϵ�TX��RX�ӿڣ������ʿɸ����������
    ֧�ֵĲ�����������
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
///-----UART0--GROUP1��Ӧ�ӿ�AC632N_UART0_GROUP_TXPA05_RXPA06-------
///-----UART0--GROUP2��Ӧ�ӿ�AC632N_UART0_GROUP_TXPB02_RXPB03-------
///-----UART0--GROUP3��Ӧ�ӿ�AC632N_UART0_GROUP_TXPA01_RXPA02-------
///-----ע�⣺UART0Ϊ�ײ���Դ�ӡ���ڣ�������ѡ��UART1��UART2��Ϊ����ͨѶ
///-----��ʹ��UART0������ͨѶʱ����عر�ϵͳ��ӡ�ܿ���CONFIG_DEBUG_ENABLE
#define MCU_UART0_ENABLE                    0      ///����0ͨѶʹ��

#if  MCU_UART0_ENABLE

    #define MCU_UART0_BAUD                      BAUD_115200
    #define MCU_UART0_GPIO                      GROUP3
    void mcu_uart0_init(void);
    void mcu_uart0_send_data(void);

#endif // MCU_UART0_ENABLE


///-----UART1--GROUP1��Ӧ�ӿ�632N_UART1_GROUP_TXPB04_RXPB05-------
///-----UART1--GROUP2��Ӧ�ӿ�632N_UART1_GROUP_TXPB00_RXPB01-------
///-----UART1--GROUP3��Ӧ�ӿ�632N_UART1_GROUP_TXPA07_RXPA08-------
///-----UART1--GROUP4��Ӧ�ӿ�632N_UART1_GROUP_USB_TXDP_RXDM-------

#define MCU_UART1_ENABLE                    1      ///����1ͨѶʹ��


#if  MCU_UART1_ENABLE
    #define MCU_UART1_BAUD                      BAUD_115200
    #define MCU_UART1_GPIO                      GROUP1
    void mcu_uart1_init(void);
    void mcu_uart1_send_data(void);
#endif // MCU_UART1_ENABLE
///-----UART2--GROUP1��Ӧ�ӿ�632N_UART2_GROUP_TXPA03_RXPA04-------
///-----UART2--GROUP3��Ӧ�ӿ�632N_UART2_GROUP_TXPB06_RXPB07-------
///-----UART2--GROUP4��Ӧ�ӿ�632N_UART2_GROUP_TXPDP1_RXPDM1-------

#define MCU_UART2_ENABLE                    0      ///����2ͨѶʹ��


#if  MCU_UART2_ENABLE
    #define MCU_UART2_BAUD                      BAUD_115200
    #define MCU_UART2_GPIO                      GROUP1
    void mcu_uart2_init(void);
    void mcu_uart2_send_data(void);
#endif // MCU_UART2_ENABLE


#endif
