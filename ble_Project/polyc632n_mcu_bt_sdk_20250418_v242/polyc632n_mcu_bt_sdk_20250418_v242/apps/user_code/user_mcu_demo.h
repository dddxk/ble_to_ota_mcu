#ifndef _MCU_USER_DEMO_H_
#define _MCU_USER_DEMO_H_

void mcu_user_system_init(void); //需要提前设置的系统初始化函数
void mcu_user_init(void);  //应用程序初始化函数
void mcu_user_timer_scan(void); //应用程序1ms中断扫描
void mcu_user_main(void); //应用程序主函数运行功能，20ms跑一次


///BLE数据传输应用模式切换
void app_user_ble_data_transfer_mode_select(u8 mode_select, u8 enable);


void set_led_disp_frequency(u16 fre);





#endif
