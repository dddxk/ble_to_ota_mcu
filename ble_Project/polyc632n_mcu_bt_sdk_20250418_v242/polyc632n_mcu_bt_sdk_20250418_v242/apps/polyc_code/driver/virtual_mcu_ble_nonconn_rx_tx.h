#ifndef __VIRTUAL_MCU_BLE_NONCONN_RX_TX_H__
#define __VIRTUAL_MCU_BLE_NONCONN_RX_TX_H__

#include "virtual_mcu_main.h"


//搜索类型
#define BLE_NONCONN_RX_SCAN_PASSIVE   0
#define BLE_NONCONN_RX_SCAN_ACTIVE    1

#define BLE_NONCONN_ADV_TYPE          3   /*Non connectable undirected advertising*/


//搜索 周期大小
#define BLE_NONCONN_RX_SCAN_INTERVAL   MCU_ADV_SCAN_MS(200) // unit: 0.625ms
//搜索 窗口大小
#define BLE_NONCONN_RX_SCAN_WINDOW     MCU_ADV_SCAN_MS(200)  // unit: 0.625ms, <= BLE_NONCONN_RX_SCAN_INTERVAL


//广播周期
#define BLE_NONCONN_TX_ADV_INTERVAL_MIN        MCU_ADV_SCAN_MS(20)  /*毫秒 转换到 0.625ms 单位*/

//BLE非连接广播收发功能开启和关闭   1:开启  0:关闭
void virtual_mcu_ble_nonconn_rx_tx_enable(u8 enable);

/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    BLE非连接广播数据发送接口
   @param    *adv:  发送广播数据的指针
   @param	 adv_size:   发送广播数据的长度,最大长度为31
   @param	 tx_interval:  广播该数据持续的时间(单位:10ms)
   @return   返回值: 0   表示失败
			 非0         表示成功,当前发送数据的长度
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
u8 mcu_ble_nonconn_adv_data_tx(u8 *adv, u8 adv_size, u8 tx_interval);


//注册BLE非连接广播数据收数接口
void mcu_ble_nonconn_adv_rx_register_callback(void *cbk(polyc_adv_report_t *rx_report_pt, u16 len));

#endif
