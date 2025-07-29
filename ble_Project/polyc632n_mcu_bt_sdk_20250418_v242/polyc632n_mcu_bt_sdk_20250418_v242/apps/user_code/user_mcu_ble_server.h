#ifndef _USER_MCU_BLE_SERVER_H_
#define _USER_MCU_BLE_SERVER_H_


void app_user_set_ble_server_disconnected(void);//断开ble_server从机连接
int app_user_ble_server_data_send(u8 *data, u16 len);//ble_server从机发数



#endif // _USER_MCU_BLE_SERVER_H_
