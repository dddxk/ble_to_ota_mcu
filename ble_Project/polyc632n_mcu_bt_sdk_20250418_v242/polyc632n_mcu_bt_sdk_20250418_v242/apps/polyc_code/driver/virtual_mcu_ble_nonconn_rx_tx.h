#ifndef __VIRTUAL_MCU_BLE_NONCONN_RX_TX_H__
#define __VIRTUAL_MCU_BLE_NONCONN_RX_TX_H__

#include "virtual_mcu_main.h"


//��������
#define BLE_NONCONN_RX_SCAN_PASSIVE   0
#define BLE_NONCONN_RX_SCAN_ACTIVE    1

#define BLE_NONCONN_ADV_TYPE          3   /*Non connectable undirected advertising*/


//���� ���ڴ�С
#define BLE_NONCONN_RX_SCAN_INTERVAL   MCU_ADV_SCAN_MS(200) // unit: 0.625ms
//���� ���ڴ�С
#define BLE_NONCONN_RX_SCAN_WINDOW     MCU_ADV_SCAN_MS(200)  // unit: 0.625ms, <= BLE_NONCONN_RX_SCAN_INTERVAL


//�㲥����
#define BLE_NONCONN_TX_ADV_INTERVAL_MIN        MCU_ADV_SCAN_MS(20)  /*���� ת���� 0.625ms ��λ*/

//BLE�����ӹ㲥�շ����ܿ����͹ر�   1:����  0:�ر�
void virtual_mcu_ble_nonconn_rx_tx_enable(u8 enable);

/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    BLE�����ӹ㲥���ݷ��ͽӿ�
   @param    *adv:  ���͹㲥���ݵ�ָ��
   @param	 adv_size:   ���͹㲥���ݵĳ���,��󳤶�Ϊ31
   @param	 tx_interval:  �㲥�����ݳ�����ʱ��(��λ:10ms)
   @return   ����ֵ: 0   ��ʾʧ��
			 ��0         ��ʾ�ɹ�,��ǰ�������ݵĳ���
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
u8 mcu_ble_nonconn_adv_data_tx(u8 *adv, u8 adv_size, u8 tx_interval);


//ע��BLE�����ӹ㲥���������ӿ�
void mcu_ble_nonconn_adv_rx_register_callback(void *cbk(polyc_adv_report_t *rx_report_pt, u16 len));

#endif
