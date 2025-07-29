#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_main.h"
#include "virtual_mcu_ble_nonconn_rx_tx.h"
#include "user_mcu_ble_nonconn_rx_tx.h"

//crc8_MAXIM算法
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



/*------------------------------------------BLE非连接广播收发应用示例------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
//构造BLE非连接从机广播数据
u8 app_user_make_ble_nonconn_adv_tx_data(u8 *data, u8 len)
{
    static u8 packet_id =0;
    u8 ble_adv_tx_data[31]; //max is 31
    u8 ret_len =0;
    u8 tx_adv_data_len;

    if(get_virtual_mcu_ble_app_mode() == VIRTUAL_MCU_BLE_NONCONN_RX_TX_APP_MODE) //BLE非连接广播收发应用模式
    {
        tx_adv_data_len = len;
        if(tx_adv_data_len > 29) //判断最大数据长度
        {
            tx_adv_data_len =29; //取最大数据长度
        }

        ble_adv_tx_data[0] =packet_id; //第1个字节:存当前数据包id号

        memcpy(&ble_adv_tx_data[1], data, tx_adv_data_len); //从第2个字节开始存数据
        tx_adv_data_len++; //包id长度+1

        #if 1 //使用crc8_MAXIM算法
        u8 rec_crc8_maxim = user_crc8_MAXIM(ble_adv_tx_data, tx_adv_data_len); //计数crc8_MAXIM值
        ble_adv_tx_data[tx_adv_data_len] =rec_crc8_maxim; //最后一个字节:存crc8_MAXIM值
        tx_adv_data_len++; //crc8_MAXIM值长度+1
        #endif

        printf("tx_adv_data:%d", tx_adv_data_len);
        printf_buf(ble_adv_tx_data, tx_adv_data_len);

        ret_len =mcu_ble_nonconn_adv_data_tx(ble_adv_tx_data, tx_adv_data_len, 10); //调BLE非连接广播数据发送接口, 广播数据持续时间为10*10ms

        if(ret_len== tx_adv_data_len){
            packet_id++; //添加发送成功,当前数据包id自增;
            printf("add_ok\n"); //添加发送成功
        }else{
            printf("add_err\n"); //添加发送失败
            return 0;
        }
    }

    return ret_len;
}

//BLE非连接广播数据收数接口回调函数
static void app_user_ble_nonconn_adv_rx_data_handle(polyc_adv_report_t *rx_report_pt, u16 len)
{
    #if 1 //使用crc8_MAXIM算法
    u8 crc_len;
    u8 rec_crc8_maxim;
    crc_len = rx_report_pt->length -1;
	rec_crc8_maxim = rx_report_pt->data[crc_len]; //存接收到的crc8_MAXIM值
	if(user_crc8_MAXIM(rx_report_pt->data, crc_len) ==rec_crc8_maxim) //判断crc8_MAXIM值是否一致
    #endif
    {
        //远端从机6位地址信息
        printf("rx_address:");
        printf_buf(rx_report_pt->address,6);

        //接收到远端从机广播的数据内容
        printf("rx_adv_data:%d", rx_report_pt->length);
        printf_buf(rx_report_pt->data, rx_report_pt->length);
	}
}

//应用层控制BLE非连接广播收发功能开启和关闭   1:开启  0:关闭
void app_user_ble_nonconn_rx_tx_enable(u8 enable)
{
    if(enable){
        mcu_ble_nonconn_adv_rx_register_callback(app_user_ble_nonconn_adv_rx_data_handle); //注册BLE非连接广播数据收数接口
        mcu_ble_nonconn_rx_tx_enable(1);
    }else{
        mcu_ble_nonconn_rx_tx_enable(0);
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
