#ifndef _USER_MCU_BLE_CLIENT_H_
#define _USER_MCU_BLE_CLIENT_H_



void app_user_set_ble_client_connected(u8 creat_mode, u8 *data, u16 data_len); //����BLE��������ָ���豸
void app_user_set_ble_client_disconnected(void);//�Ͽ�ble_client��������
int app_user_ble_client_data_send(u8 *data, u16 len);//ble_client��������


#endif//_USER_MCU_BLE_CLIENT_H_
