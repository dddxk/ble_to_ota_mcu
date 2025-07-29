#ifndef _USER_MCU_BLE_CLIENT_H_
#define _USER_MCU_BLE_CLIENT_H_



void app_user_set_ble_client_connected(u8 creat_mode, u8 *data, u16 data_len); //设置BLE主机连接指定设备
void app_user_set_ble_client_disconnected(void);//断开ble_client主机连接
int app_user_ble_client_data_send(u8 *data, u16 len);//ble_client主机发数


#endif//_USER_MCU_BLE_CLIENT_H_
