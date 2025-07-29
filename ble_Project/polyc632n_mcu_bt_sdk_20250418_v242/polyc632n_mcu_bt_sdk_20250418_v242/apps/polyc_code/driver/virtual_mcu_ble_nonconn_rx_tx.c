#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_main.h"
#include "virtual_mcu_ble_nonconn_rx_tx.h"

//ע��BLE�����ӹ㲥���������ӿ�
static void (*mcu_ble_nonconn_adv_rx_func_callback)(polyc_adv_report_t *rx_report_pt, u16 len) =NULL;
void mcu_ble_nonconn_adv_rx_register_callback(void *cbk(polyc_adv_report_t *rx_report_pt, u16 len))
{
    mcu_ble_nonconn_adv_rx_func_callback =cbk;
}

//*BLE����Э��ջ�ص�scan����adv��   ע��:������ϵͳ�ײ�ص����벻Ҫ����������*/
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

    polyc_ble_client_report_adv_data_register_callback(mcu_ble_nonconn_rx_report_callback); //ע�������㲥�����ݽ��ջص�����

    polyc_ble_client_scan_config_set(BLE_NONCONN_RX_SCAN_PASSIVE, BLE_NONCONN_RX_SCAN_INTERVAL, BLE_NONCONN_RX_SCAN_WINDOW);  //BLE��������ɨ���������

    polyc_ble_server_adv_param_set(BLE_NONCONN_TX_ADV_INTERVAL_MIN, BLE_NONCONN_ADV_TYPE, BIT(0)|BIT(1)|BIT(2)); //BLE�ӻ��㲥��������
}

/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    BLE�����ӹ㲥���ݷ��ͽӿ�
   @param    *adv:  ���͹㲥���ݵ�ָ��
   @param	 adv_size:   ���͹㲥���ݵĳ���,��󳤶�Ϊ31
   @param	 tx_interval:  �㲥�����ݳ�����ʱ��,(��λ:10ms)
   @return   ����ֵ: 0   ��ʾʧ��
			 ��0         ��ʾ�ɹ�,��ǰ�������ݵĳ���
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
u8 mcu_ble_nonconn_adv_data_tx(u8 *buff, u8 buff_size, u8 tx_interval)
{
    if(get_virtual_mcu_ble_app_mode() != VIRTUAL_MCU_BLE_NONCONN_RX_TX_APP_MODE){ //��ǰ����BLE�����ӹ㲥�շ�Ӧ��ģʽ��ʱ
        return 0;
    }

    if(buff_size > 31){
        buff_size =31;
    }
    return polyc_set_ble_nonconn_adv_tx(buff, buff_size, tx_interval);
}

///BLE�����ӹ㲥�շ����ܿ����͹ر�
void mcu_ble_nonconn_rx_tx_enable(u8 enable)
{
    if(enable){
        if(get_virtual_mcu_ble_app_mode() != VIRTUAL_MCU_BLE_NONCONN_RX_TX_APP_MODE){ //��ǰ����BLE�����ӹ㲥�շ�Ӧ��ģʽ
            mcu_ble_nonconn_rx_tx_init();
            polyc_set_ble_cmd(MCU_SET_BLE_NONCONN_RX_TX_ON, NULL, 0);
        }
    }else{
        polyc_set_ble_cmd(MCU_SET_BLE_NONCONN_RX_TX_OFF, NULL, 0);
    }
}


