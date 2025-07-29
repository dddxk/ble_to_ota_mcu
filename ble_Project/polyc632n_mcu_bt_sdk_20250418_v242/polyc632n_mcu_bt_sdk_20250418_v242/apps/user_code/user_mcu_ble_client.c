#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_main.h"
#include "virtual_mcu_ble_client.h"
#include "user_mcu_ble_client.h"


/*---------------------------------------BLE_CLIENT����Ӧ��ʾ��-----------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------*/

//ble_client��������
int app_user_ble_client_data_send(u8 *data, u16 len)
{
    int ret;

    printf("client_data_send:%d\n", len);
    printf_buf(data, len);

    ret = mcu_ble_client_data_send(data, len);
    return ret;
}

//ble_client�����������ص�����
static void app_user_ble_client_data_receive_handle(u16 value_handle, u16 offset, u8 *buffer, u16 buffer_size)
{
    printf("client_data_receive:%d\n", buffer_size);
    printf_buf(buffer, buffer_size);

}

//����BLE��������ָ���豸
void app_user_set_ble_client_connected(u8 creat_mode, u8 *data, u16 data_len)
{
    //if(get_virtual_mcu_ble_app_mode() == VIRTUAL_MCU_BLE_CLIENT_APP_MODE) //��ǰΪBLE_CLIENT����Ӧ��ģʽ
    {
        mcu_ble_client_set_device_match(creat_mode, data, data_len);
    }
}

//�Ͽ�ble_client��������
void app_user_set_ble_client_disconnected(void)
{
    if(mcu_get_ble_client_connect_flag()){
        mcu_ble_client_set_device_match(POLYC_CLIENT_CLI_CREAT_BY_ADDRESS, NULL, 0); //�Ͽ�BLE����ʱ,��ָ����ַ����
        mcu_ble_client_disconnected();
    }
}

//Ӧ�ò����BLE_CLIENT����Ӧ��ģʽ�����͹ر�   1:����  0:�ر�
void app_user_ble_client_enable(u8 enable)
{
    if(enable){
        #if 1//ָ���豸������
        mcu_ble_client_set_device_match(POLYC_CLIENT_CLI_CREAT_BY_NAME, virtual_mcu_local_ble_name, strlen(virtual_mcu_local_ble_name));
        #endif
        mcu_ble_client_data_receive_register_callback(app_user_ble_client_data_receive_handle); //ע��ble_client�����������ӿ�
        mcu_ble_client_enable(1);
    }else{
        mcu_ble_client_set_device_match(POLYC_CLIENT_CLI_CREAT_BY_ADDRESS, NULL, 0); //�ر�BLE_CLIENT����Ӧ��ģʽʱ,��ָ����ַ����
        mcu_ble_client_enable(0);
    }
}
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
