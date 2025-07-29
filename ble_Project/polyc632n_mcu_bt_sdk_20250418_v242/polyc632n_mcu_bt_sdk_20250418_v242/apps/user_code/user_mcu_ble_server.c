#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_main.h"
#include "virtual_mcu_ble_server.h"
#include "user_mcu_ble_server.h"


/*---------------------------------------BLE_SERVER从机应用示例------------------------------------------*/
/*---------------------------------------------------------------------------------------------------*/

//ble_server从机发数
int app_user_ble_server_data_send(u8 *data, u16 len)
{
    int ret;

    printf("server_data_send:%d\n", len);
    printf_buf(data, len);

    ret = mcu_ble_server_data_send(data, len);
    return ret;
}

//ble_server从机端收数回调函数
static void app_user_ble_server_data_receive_handle(u16 value_handle, u16 offset, u8 *buffer, u16 buffer_size)
{
    printf("server_data_receive:%d\n", buffer_size);
    printf_buf(buffer, buffer_size);

    app_user_ble_server_data_send(buffer, buffer_size);    //测试ble_server从机发数
}

//断开ble_server从机连接
void app_user_set_ble_server_disconnected(void)
{
    if(mcu_get_ble_server_connect_flag()){
        mcu_ble_server_disconnected();
    }
}

//应用层控制BLE_SERVER从机应用模式开启和关闭   1:开启  0:关闭
void app_user_ble_server_enable(u8 enable)
{
    if(enable){
        mcu_ble_server_data_receive_register_callback(app_user_ble_server_data_receive_handle); //注册ble_server从机端收数接口
        mcu_ble_server_enable(1);
    }else{
        mcu_ble_server_enable(0);
    }
}
