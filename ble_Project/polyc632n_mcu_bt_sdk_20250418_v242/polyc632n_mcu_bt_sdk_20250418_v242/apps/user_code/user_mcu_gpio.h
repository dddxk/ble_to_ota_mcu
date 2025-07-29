#ifndef __USER_MCU_GPIO_H__
#define __USER_MCU_GPIO_H__


void user_set_io_output_high_or_low(u8 gpio, u8 high_or_low); //设置gpio输出高或低电平

void user_set_io_input(u8 gpio); //设置gpio为无上下拉输入
void user_set_io_pull_up_input(u8 gpio); //设置gpio为上拉输入
void user_set_io_pull_down_input(u8 gpio); //设置gpio为下拉输入
u8 user_read_io_value(u8 gpio);  //读取gpio电平状态

void user_set_usb_to_gpio_init(void); //关闭U盘功能,初始化DM和DP口为普通IO口




#endif
