#ifndef __VIRTUAL_MCU_BLE_SERVER_H__
#define __VIRTUAL_MCU_BLE_SERVER_H__

#include "virtual_mcu_main.h"

//�㲥����
#define MCU_BLE_SERVER_ADV_INTERVAL_MIN        MCU_ADV_SCAN_MS(200)  /*���� ת���� 0.625ms ��λ*/


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

extern u8 curr_server_peer_addr_info[7];  //��ǰ���ӶԷ���ַ��Ϣ  1λaddr_type + 6λaddress

u8 mcu_get_ble_server_connect_flag(void); //��ȡble_server�ӻ��Ƿ��Ѿ�����


//ble_server�ӻ���ʼ������
void mcu_ble_server_init(void);

//BLE_SERVER�ӻ�Ӧ��ģʽ�����͹ر�  1:����  0:�ر�
void mcu_ble_server_enable(u8 enable);

//ble_server�ӻ���ȡ��·�Է����ź�ǿ��, ��Χ-127 ~ 127
s8 mcu_get_ble_server_curr_rssi(void);

//�Ͽ�ble_server�ӻ�����
void mcu_ble_server_disconnected(void);

//ble_server�ӻ��˷����ӿ�  ����0,���ͳɹ�
int mcu_ble_server_data_send(u8 *buff, u16 buff_size);

//ע��ble_server�ӻ��������ӿ�
void mcu_ble_server_data_receive_register_callback(void *cbk(u16 value_handle, u16 offset, u8 *buffer, u16 buffer_size));


#endif
