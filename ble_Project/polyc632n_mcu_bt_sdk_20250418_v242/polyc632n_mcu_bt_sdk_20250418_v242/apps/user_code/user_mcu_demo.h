#ifndef _MCU_USER_DEMO_H_
#define _MCU_USER_DEMO_H_

void mcu_user_system_init(void); //��Ҫ��ǰ���õ�ϵͳ��ʼ������
void mcu_user_init(void);  //Ӧ�ó����ʼ������
void mcu_user_timer_scan(void); //Ӧ�ó���1ms�ж�ɨ��
void mcu_user_main(void); //Ӧ�ó������������й��ܣ�20ms��һ��


///BLE���ݴ���Ӧ��ģʽ�л�
void app_user_ble_data_transfer_mode_select(u8 mode_select, u8 enable);


void set_led_disp_frequency(u16 fre);





#endif
