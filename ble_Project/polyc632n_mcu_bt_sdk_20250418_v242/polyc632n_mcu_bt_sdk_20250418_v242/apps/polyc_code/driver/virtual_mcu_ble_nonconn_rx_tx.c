#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_main.h"
#include "virtual_mcu_ble_nonconn_rx_tx.h"

//注册BLE非连接广播数据收数接口
static void (*mcu_ble_nonconn_adv_rx_func_callback)(polyc_adv_report_t *rx_report_pt, u16 len) =NULL;
void mcu_ble_nonconn_adv_rx_register_callback(void *cbk(polyc_adv_report_t *rx_report_pt, u16 len))
{
    mcu_ble_nonconn_adv_rx_func_callback =cbk;
}

//*BLE主机协议栈回调scan到的adv包   注意:函数由系统底层回调，请不要其他方调用*/
static void mcu_ble_nonconn_rx_report_callback(polyc_adv_report_t *report_pt, u16 len)
{
	if( (report_pt->event_type == BLE_NONCONN_ADV_TYPE) && (report_pt->address_type == 0) )
    {
        //putchar('n');
        //printf("rx_report_handle--->event_type, addr_type: %x,%x; ",report_pt->event_type,report_pt->address_type);
        //printf_buf(report_pt->address,6);
        //printf("adv_data_display:%d", report_pt->length);
        //printf_buf(report_pt->data,report_pt->length);
        //printf("len:%d, rssi:%d\n", len, report_pt->rssi);

        if(mcu_ble_nonconn_adv_rx_func_callback){
            mcu_ble_nonconn_adv_rx_func_callback(report_pt, len);
        }
    }
}

static void mcu_ble_nonconn_rx_tx_init(void)
{
    printf("mcu_ble_nonconn_rx_tx_init--->\n");

    polyc_ble_client_report_adv_data_register_callback(mcu_ble_nonconn_rx_report_callback); //注册主机广播包数据接收回调函数

    polyc_ble_client_scan_config_set(BLE_NONCONN_RX_SCAN_PASSIVE, BLE_NONCONN_RX_SCAN_INTERVAL, BLE_NONCONN_RX_SCAN_WINDOW);  //BLE主机接收扫描参数配置

    polyc_ble_server_adv_param_set(BLE_NONCONN_TX_ADV_INTERVAL_MIN, BLE_NONCONN_ADV_TYPE, BIT(0)|BIT(1)|BIT(2)); //BLE从机广播参数设置
}

/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    BLE非连接广播数据发送接口
   @param    *adv:  发送广播数据的指针
   @param	 adv_size:   发送广播数据的长度,最大长度为31
   @param	 tx_interval:  广播该数据持续的时间,(单位:10ms)
   @return   返回值: 0   表示失败
			 非0         表示成功,当前发送数据的长度
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
u8 mcu_ble_nonconn_adv_data_tx(u8 *buff, u8 buff_size, u8 tx_interval)
{
    if(get_virtual_mcu_ble_app_mode() != VIRTUAL_MCU_BLE_NONCONN_RX_TX_APP_MODE){ //当前不在BLE非连接广播收发应用模式下时
        return 0;
    }

    if(buff_size > 31){
        buff_size =31;
    }
    return polyc_set_ble_nonconn_adv_tx(buff, buff_size, tx_interval);
}

///BLE非连接广播收发功能开启和关闭
void mcu_ble_nonconn_rx_tx_enable(u8 enable)
{
    if(enable){
        if(get_virtual_mcu_ble_app_mode() != VIRTUAL_MCU_BLE_NONCONN_RX_TX_APP_MODE){ //当前不在BLE非连接广播收发应用模式
            mcu_ble_nonconn_rx_tx_init();
            polyc_set_ble_cmd(MCU_SET_BLE_NONCONN_RX_TX_ON, NULL, 0);
        }
    }else{
        polyc_set_ble_cmd(MCU_SET_BLE_NONCONN_RX_TX_OFF, NULL, 0);
    }
}


