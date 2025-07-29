#ifndef __VIRTUAL_MCU_BLE_CLIENT_H__
#define __VIRTUAL_MCU_BLE_CLIENT_H__

#include "virtual_mcu_main.h"

//��������
#define SET_TYPE_SCAN_PASSIVE   0
#define SET_TYPE_SCAN_ACTIVE    1

//���� ���ڴ�С
#define MCU_BLE_CLIENT_SET_SCAN_INTERVAL   MCU_ADV_SCAN_MS(24) // unit: 0.625ms
//���� ���ڴ�С
#define MCU_BLE_CLIENT_SET_SCAN_WINDOW     MCU_ADV_SCAN_MS(8)  // unit: 0.625ms, <= MCU_BLE_CLIENT_SET_SCAN_INTERVAL

//��������
#define MCU_BLE_CLIENT_BASE_INTERVAL_MIN   (6)//��С��interval
#define MCU_BLE_CLIENT_SET_CONN_INTERVAL   (MCU_BLE_CLIENT_BASE_INTERVAL_MIN*8) //(unit:1.25ms)
//����latency
#define MCU_BLE_CLIENT_SET_CONN_LATENCY    0  //(unit:conn_interval)
//���ӳ�ʱ
#define MCU_BLE_CLIENT_SET_CONN_TIMEOUT    400 //(unit:10ms)


#define POLYC_ATT_PROPERTY_BROADCAST                0x01
#define POLYC_ATT_PROPERTY_READ                     0x02
#define POLYC_ATT_PROPERTY_WRITE_WITHOUT_RESPONSE   0x04
#define POLYC_ATT_PROPERTY_WRITE                    0x08
#define POLYC_ATT_PROPERTY_NOTIFY                   0x10
#define POLYC_ATT_PROPERTY_INDICATE                 0x20


extern u8 curr_client_peer_addr_info[7];         //��ǰ���ӶԷ���ַ��Ϣ  1λaddr_type + 6λaddress
extern u8 curr_client_peer_device_name_info[29];  //��ǰ���ӶԷ��豸����Ϣ,���29���ֽ�

u8 mcu_get_ble_client_connect_flag(void); //��ȡble_client�����Ƿ��Ѿ�����


enum {
    POLYC_CLIENT_CLI_CREAT_BY_ADDRESS = 1, //ָ����ַ��������
    POLYC_CLIENT_CLI_CREAT_BY_NAME = 2, //ָ���豸���ƴ�������
    POLYC_CLIENT_CLI_CREAT_BY_TAG = 3, //ƥ�䳧�ұ�ʶ��������
};
//����BLE��������ָ���豸
void mcu_ble_client_set_device_match(u8 creat_mode, u8 *data, u16 data_len);

//ble_client������ʼ������
void mcu_ble_client_init(void);

//BLE_CLIENT����Ӧ��ģʽ�����͹ر�  1:����  0:�ر�
void mcu_ble_client_enable(u8 enable);

//ble_client������ȡ��·�Է����ź�ǿ��, ��Χ-127 ~ 127
s8 mcu_get_ble_client_curr_rssi(void);

//�Ͽ�ble_client��������
void mcu_ble_client_disconnected(void);

//ble_client�����˷����ӿ�  ����0,���ͳɹ�
int mcu_ble_client_data_send(u8 *buff, u16 buff_size);

////ע��ble_client�����������ӿ�
void mcu_ble_client_data_receive_register_callback(void *cbk(u16 value_handle, u16 offset, u8 *buffer, u16 buffer_size));

#endif
