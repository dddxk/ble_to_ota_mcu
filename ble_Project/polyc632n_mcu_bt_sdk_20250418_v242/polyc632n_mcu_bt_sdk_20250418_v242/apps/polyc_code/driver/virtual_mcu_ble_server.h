#ifndef __VIRTUAL_MCU_BLE_SERVER_H__
#define __VIRTUAL_MCU_BLE_SERVER_H__

#include "virtual_mcu_main.h"

//广播周期
#define MCU_BLE_SERVER_ADV_INTERVAL_MIN        MCU_ADV_SCAN_MS(200)  /*毫秒 转换到 0.625ms 单位*/


/*adv type*/
#define  MCU_BLE_SERVER_ADV_IND               0   /*Connectable and scannable undirected advertising*/
#define  MCU_BLE_SERVER_ADV_DIRECT_IND        1   /*Connectable high duty cycle directed advertising*/
#define  MCU_BLE_SERVER_ADV_SCAN_IND          2   /*Scannable undirected advertising*/
#define  MCU_BLE_SERVER_ADV_NONCONN_IND       3   /*Non connectable undirected advertising*/
#define  MCU_BLE_SERVER_ADV_DIRECT_IND_LOW    4   /*Connectable low duty cycle directed advertising*/


/*adv channel*/
#define  MCU_BLE_SERVER_ADV_CHANNEL_37    BIT(0)
#define  MCU_BLE_SERVER_ADV_CHANNEL_38    BIT(1)
#define  MCU_BLE_SERVER_ADV_CHANNEL_39    BIT(2)
#define  MCU_BLE_SERVER_ADV_CHANNEL_ALL  (MCU_BLE_SERVER_ADV_CHANNEL_37 | MCU_BLE_SERVER_ADV_CHANNEL_38 | MCU_BLE_SERVER_ADV_CHANNEL_39)

extern u8 curr_server_peer_addr_info[7];  //当前连接对方地址信息  1位addr_type + 6位address

u8 mcu_get_ble_server_connect_flag(void); //获取ble_server从机是否已经连接


//ble_server从机初始化函数
void mcu_ble_server_init(void);

//BLE_SERVER从机应用模式开启和关闭  1:开启  0:关闭
void mcu_ble_server_enable(u8 enable);

//ble_server从机获取链路对方的信号强度, 范围-127 ~ 127
s8 mcu_get_ble_server_curr_rssi(void);

//断开ble_server从机连接
void mcu_ble_server_disconnected(void);

//ble_server从机端发数接口  返回0,发送成功
int mcu_ble_server_data_send(u8 *buff, u16 buff_size);

//注册ble_server从机端收数接口
void mcu_ble_server_data_receive_register_callback(void *cbk(u16 value_handle, u16 offset, u8 *buffer, u16 buffer_size));


#endif
