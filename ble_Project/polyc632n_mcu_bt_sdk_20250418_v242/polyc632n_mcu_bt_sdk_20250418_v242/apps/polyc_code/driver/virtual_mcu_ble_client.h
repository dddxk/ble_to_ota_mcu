#ifndef __VIRTUAL_MCU_BLE_CLIENT_H__
#define __VIRTUAL_MCU_BLE_CLIENT_H__

#include "virtual_mcu_main.h"

//搜索类型
#define SET_TYPE_SCAN_PASSIVE   0
#define SET_TYPE_SCAN_ACTIVE    1

//搜索 周期大小
#define MCU_BLE_CLIENT_SET_SCAN_INTERVAL   MCU_ADV_SCAN_MS(24) // unit: 0.625ms
//搜索 窗口大小
#define MCU_BLE_CLIENT_SET_SCAN_WINDOW     MCU_ADV_SCAN_MS(8)  // unit: 0.625ms, <= MCU_BLE_CLIENT_SET_SCAN_INTERVAL

//连接周期
#define MCU_BLE_CLIENT_BASE_INTERVAL_MIN   (6)//最小的interval
#define MCU_BLE_CLIENT_SET_CONN_INTERVAL   (MCU_BLE_CLIENT_BASE_INTERVAL_MIN*8) //(unit:1.25ms)
//连接latency
#define MCU_BLE_CLIENT_SET_CONN_LATENCY    0  //(unit:conn_interval)
//连接超时
#define MCU_BLE_CLIENT_SET_CONN_TIMEOUT    400 //(unit:10ms)


#define POLYC_ATT_PROPERTY_BROADCAST                0x01
#define POLYC_ATT_PROPERTY_READ                     0x02
#define POLYC_ATT_PROPERTY_WRITE_WITHOUT_RESPONSE   0x04
#define POLYC_ATT_PROPERTY_WRITE                    0x08
#define POLYC_ATT_PROPERTY_NOTIFY                   0x10
#define POLYC_ATT_PROPERTY_INDICATE                 0x20


extern u8 curr_client_peer_addr_info[7];         //当前连接对方地址信息  1位addr_type + 6位address
extern u8 curr_client_peer_device_name_info[29];  //当前连接对方设备名信息,最大29个字节

u8 mcu_get_ble_client_connect_flag(void); //获取ble_client主机是否已经连接


enum {
    POLYC_CLIENT_CLI_CREAT_BY_ADDRESS = 1, //指定地址创建连接
    POLYC_CLIENT_CLI_CREAT_BY_NAME = 2, //指定设备名称创建连接
    POLYC_CLIENT_CLI_CREAT_BY_TAG = 3, //匹配厂家标识创建连接
};
//设置BLE主机连接指定设备
void mcu_ble_client_set_device_match(u8 creat_mode, u8 *data, u16 data_len);

//ble_client主机初始化函数
void mcu_ble_client_init(void);

//BLE_CLIENT主机应用模式开启和关闭  1:开启  0:关闭
void mcu_ble_client_enable(u8 enable);

//ble_client主机获取链路对方的信号强度, 范围-127 ~ 127
s8 mcu_get_ble_client_curr_rssi(void);

//断开ble_client主机连接
void mcu_ble_client_disconnected(void);

//ble_client主机端发数接口  返回0,发送成功
int mcu_ble_client_data_send(u8 *buff, u16 buff_size);

////注册ble_client主机端收数接口
void mcu_ble_client_data_receive_register_callback(void *cbk(u16 value_handle, u16 offset, u8 *buffer, u16 buffer_size));

#endif
