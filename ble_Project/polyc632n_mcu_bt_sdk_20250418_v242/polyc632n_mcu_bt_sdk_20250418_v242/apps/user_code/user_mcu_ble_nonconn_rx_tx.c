#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_main.h"
#include "virtual_mcu_ble_nonconn_rx_tx.h"
#include "user_mcu_ble_nonconn_rx_tx.h"

//crc8_MAXIM�㷨
static u8 user_crc8_MAXIM(u8 *data, u8 len)
{
    u8 crc, i;
    crc = 0x00;

    while(len--)
    {
        crc ^= *data++;
        for(i = 0;i < 8;i++)
        {
            if(crc & 0x01){
                crc = (crc >> 1) ^ 0x8c;
            }
            else crc >>= 1;
        }
    }
    //printf("crc8_MAXIM = 0x%02x\n", crc);
    return crc;
}



/*------------------------------------------BLE�����ӹ㲥�շ�Ӧ��ʾ��------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
//����BLE�����Ӵӻ��㲥����
u8 app_user_make_ble_nonconn_adv_tx_data(u8 *data, u8 len)
{
    static u8 packet_id =0;
    u8 ble_adv_tx_data[31]; //max is 31
    u8 ret_len =0;
    u8 tx_adv_data_len;

    if(get_virtual_mcu_ble_app_mode() == VIRTUAL_MCU_BLE_NONCONN_RX_TX_APP_MODE) //BLE�����ӹ㲥�շ�Ӧ��ģʽ
    {
        tx_adv_data_len = len;
        if(tx_adv_data_len > 29) //�ж�������ݳ���
        {
            tx_adv_data_len =29; //ȡ������ݳ���
        }

        ble_adv_tx_data[0] =packet_id; //��1���ֽ�:�浱ǰ���ݰ�id��

        memcpy(&ble_adv_tx_data[1], data, tx_adv_data_len); //�ӵ�2���ֽڿ�ʼ������
        tx_adv_data_len++; //��id����+1

        #if 1 //ʹ��crc8_MAXIM�㷨
        u8 rec_crc8_maxim = user_crc8_MAXIM(ble_adv_tx_data, tx_adv_data_len); //����crc8_MAXIMֵ
        ble_adv_tx_data[tx_adv_data_len] =rec_crc8_maxim; //���һ���ֽ�:��crc8_MAXIMֵ
        tx_adv_data_len++; //crc8_MAXIMֵ����+1
        #endif

        printf("tx_adv_data:%d", tx_adv_data_len);
        printf_buf(ble_adv_tx_data, tx_adv_data_len);

        ret_len =mcu_ble_nonconn_adv_data_tx(ble_adv_tx_data, tx_adv_data_len, 10); //��BLE�����ӹ㲥���ݷ��ͽӿ�, �㲥���ݳ���ʱ��Ϊ10*10ms

        if(ret_len== tx_adv_data_len){
            packet_id++; //��ӷ��ͳɹ�,��ǰ���ݰ�id����;
            printf("add_ok\n"); //��ӷ��ͳɹ�
        }else{
            printf("add_err\n"); //��ӷ���ʧ��
            return 0;
        }
    }

    return ret_len;
}

//BLE�����ӹ㲥���������ӿڻص�����
static void app_user_ble_nonconn_adv_rx_data_handle(polyc_adv_report_t *rx_report_pt, u16 len)
{
    #if 1 //ʹ��crc8_MAXIM�㷨
    u8 crc_len;
    u8 rec_crc8_maxim;
    crc_len = rx_report_pt->length -1;
	rec_crc8_maxim = rx_report_pt->data[crc_len]; //����յ���crc8_MAXIMֵ
	if(user_crc8_MAXIM(rx_report_pt->data, crc_len) ==rec_crc8_maxim) //�ж�crc8_MAXIMֵ�Ƿ�һ��
    #endif
    {
        //Զ�˴ӻ�6λ��ַ��Ϣ
        printf("rx_address:");
        printf_buf(rx_report_pt->address,6);

        //���յ�Զ�˴ӻ��㲥����������
        printf("rx_adv_data:%d", rx_report_pt->length);
        printf_buf(rx_report_pt->data, rx_report_pt->length);
	}
}

//Ӧ�ò����BLE�����ӹ㲥�շ����ܿ����͹ر�   1:����  0:�ر�
void app_user_ble_nonconn_rx_tx_enable(u8 enable)
{
    if(enable){
        mcu_ble_nonconn_adv_rx_register_callback(app_user_ble_nonconn_adv_rx_data_handle); //ע��BLE�����ӹ㲥���������ӿ�
        mcu_ble_nonconn_rx_tx_enable(1);
    }else{
        mcu_ble_nonconn_rx_tx_enable(0);
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
