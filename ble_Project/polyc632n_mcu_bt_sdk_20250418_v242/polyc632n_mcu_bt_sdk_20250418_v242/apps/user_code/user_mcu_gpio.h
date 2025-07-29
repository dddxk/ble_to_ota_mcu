#ifndef __USER_MCU_GPIO_H__
#define __USER_MCU_GPIO_H__


void user_set_io_output_high_or_low(u8 gpio, u8 high_or_low); //����gpio����߻�͵�ƽ

void user_set_io_input(u8 gpio); //����gpioΪ������������
void user_set_io_pull_up_input(u8 gpio); //����gpioΪ��������
void user_set_io_pull_down_input(u8 gpio); //����gpioΪ��������
u8 user_read_io_value(u8 gpio);  //��ȡgpio��ƽ״̬

void user_set_usb_to_gpio_init(void); //�ر�U�̹���,��ʼ��DM��DP��Ϊ��ͨIO��




#endif
