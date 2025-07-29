#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_main.h"
#include "virtual_mcu_ble_client.h"

static bool ble_client_connect_flag = 0;      ///ble_client主机是否已连接状态
u8 curr_client_peer_addr_info[7];         //当前连接对方地址信息  1位addr_type + 6位address
u8 curr_client_peer_device_name_info[29];  //当前连接对方设备名信息,最大29个字节

static u8 set_device_match_creat_by_address[7]={0}; //指定地址创建连接  1位addr_type + 6位address
static u8 set_device_match_creat_by_address_flag =0;

static u8 set_device_match_creat_by_name[29]={0};  //指定设备名称创建连接
static u8 set_device_match_creat_by_name_valid_len =0; //指定设备名称有效长度


//*蓝牙BLE主机状态变化函数   注意:函数由系统底层回调，请不要在其他方调用*/
static void mcu_ble_client_state_change_callback(u8 state, u8 *priv, u16 len)
{
    printf("-->client_state_change:0x%02x\n", state);
	/*
    if(priv && len){
        printf("client--->priv:\n");
        printf_buf(priv,len);
    }
	*/
    switch(state)
    {
        case POLYC_BLE_ST_INIT_OK:  //BLE主机协议栈初始化ok
            ble_client_connect_flag =0;  //蓝牙BLE主机已断开
            break;

        case POLYC_BLE_ST_DISCONN:
            printf("disconn_reason:0x%02x\n", priv[0]);
			memset(curr_client_peer_device_name_info, 0, sizeof(curr_client_peer_device_name_info)); //清当前连接对方设备名信息
            ble_client_connect_flag =0;  //蓝牙BLE主机已断开
            break;

        case POLYC_BLE_ST_CONNECT:
            memcpy(curr_client_peer_addr_info, priv, 7); //存当前连接对方地址信息
            ble_client_connect_flag =1;  //蓝牙BLE主机已连接
            printf("curr_client_peer_addr_info:\n");
            printf_buf(curr_client_peer_addr_info, 7);
            break;

        case POLYC_BLE_MTU_EXCHANGE_COMPLETE:
            break;
    }
}

//检查是否有匹配的设备
static u8 mcu_ble_client_check_device_is_match(u8 type, u8 mode, u8 *data, u16 data_len)
{
    u8 check_ok = 0;

    if( (data == NULL) || (data_len == 0) ){ //长度为0
        return 0;
    }

    switch(mode)
    {
    case POLYC_CLIENT_CLI_CREAT_BY_ADDRESS: //指定地址创建连接
        if(set_device_match_creat_by_address_flag){
            if(set_device_match_creat_by_address[0] == type){ //地址类型一致
                if (0 == memcmp(data, &set_device_match_creat_by_address[1], 6)) { //地址一致
                    check_ok =1;
                }
            }
        }
        break;

    case POLYC_CLIENT_CLI_CREAT_BY_NAME: //指定设备名称创建连接
        if(set_device_match_creat_by_name_valid_len){
            if (0 == memcmp(data, set_device_match_creat_by_name, set_device_match_creat_by_name_valid_len)) {
                check_ok =1;
            }
        }
        break;

    case POLYC_CLIENT_CLI_CREAT_BY_TAG: //匹配厂家标识创建连接
        break;
    }

    return check_ok;
}

/*************************************************************************************************/
/*!
 *  \brief      ble_client主机解析scan到的adv&rsp包数据
 *  \param      [in]
 *  \return     是否有匹配的设备, true or false
 *  \note
 */
/*************************************************************************************************/
static bool ble_client_resolve_adv_report(polyc_adv_report_t *report_pt, u16 len)
{
    u8 i, length, ad_type;
    u8 *adv_data_pt;
    u8 find_remoter = 0;
    u32 tmp32;

    //printf(">>>event_type, addr_type: %x,%x; ",report_pt->event_type,report_pt->address_type);
	//printf_buf(report_pt->address,6);

	//printf("adv_data_display:%d", report_pt->length);
	//printf_buf(report_pt->data,report_pt->length);

	//printf("rssi:%d\n", report_pt->rssi);

    if (mcu_ble_client_check_device_is_match(report_pt->address_type, POLYC_CLIENT_CLI_CREAT_BY_ADDRESS, report_pt->address, 6)) {//匹配到指定地址
        find_remoter = 1;
        printf("catch adderss ok!\n");
    }

    adv_data_pt = report_pt->data;
    for (i = 0; i < report_pt->length;) {
        if (*adv_data_pt == 0) {
            //printf("analyze end\n");
            break;
        }

        length = *adv_data_pt++;

        if (length >= report_pt->length || (length + i) >= report_pt->length) {
            /*过滤非标准包格式*/
            //printf("!!!error_adv_packet:");
            //printf_buf(report_pt->data, report_pt->length);
            break;
        }

        ad_type = *adv_data_pt++;
        i += (length + 1);

        switch (ad_type) {
        case BLE_HCI_EIR_DATATYPE_FLAGS:
            /* printf("flags:%02x\n",adv_data_pt[0]); */
            break;

        case BLE_HCI_EIR_DATATYPE_MORE_16BIT_SERVICE_UUIDS:
        case BLE_HCI_EIR_DATATYPE_COMPLETE_16BIT_SERVICE_UUIDS:
        case BLE_HCI_EIR_DATATYPE_MORE_32BIT_SERVICE_UUIDS:
        case BLE_HCI_EIR_DATATYPE_COMPLETE_32BIT_SERVICE_UUIDS:
        case BLE_HCI_EIR_DATATYPE_MORE_128BIT_SERVICE_UUIDS:
        case BLE_HCI_EIR_DATATYPE_COMPLETE_128BIT_SERVICE_UUIDS:
            //printf("service uuid:");
            //printf_buf(adv_data_pt, length - 1);
            break;

        case BLE_HCI_EIR_DATATYPE_COMPLETE_LOCAL_NAME:
        case BLE_HCI_EIR_DATATYPE_SHORTENED_LOCAL_NAME:
            tmp32 = adv_data_pt[length - 1];
            adv_data_pt[length - 1] = 0;;
            printf("remoter_name:%s ,rssi:%d\n", adv_data_pt, report_pt->rssi);
            printf_buf(report_pt->address, 6);
            adv_data_pt[length - 1] = tmp32;

            if (mcu_ble_client_check_device_is_match(report_pt->event_type, POLYC_CLIENT_CLI_CREAT_BY_NAME, adv_data_pt, length - 1)) { //匹配到指定设备名称
                find_remoter = 1;
                printf("catch name ok!\n");
            }
            break;

        case BLE_HCI_EIR_DATATYPE_MANUFACTURER_SPECIFIC_DATA:
            if (mcu_ble_client_check_device_is_match(report_pt->event_type, POLYC_CLIENT_CLI_CREAT_BY_TAG, adv_data_pt, length - 1)) {
                find_remoter = 1;
                printf("catch tag ok!\n");
            }
            break;

        case BLE_HCI_EIR_DATATYPE_APPEARANCE_DATA:
            /* printf("get_class_type:%04x\n",little_endian_read_16(adv_data_pt,0)); */
            break;

        default:
            /* printf("unknow ad_type:"); */
            break;
        }

        adv_data_pt += (length - 1);
    }

    return find_remoter;
}


//*主机client解析协议栈回调的scan到的adv&rsp 包   注意:函数由系统底层回调，请不要其他方调用*/
static void mcu_ble_client_report_adv_data_callback(polyc_adv_report_t *report_pt, u16 len)
{
    bool find_tag = 0;

    if(get_virtual_mcu_ble_app_mode() == VIRTUAL_MCU_BLE_CLIENT_APP_MODE) //在BLE_CLIENT主机应用模式下
    {
        //putchar('c');

        find_tag = ble_client_resolve_adv_report(report_pt, len); //ble_client主机解析scan到的adv&rsp包数据

        if(find_tag) //扫描到匹配的设备
        {
            u8 address_info[7];
            address_info[0] =report_pt->address_type; //对方地址类型
            memcpy(&address_info[1], report_pt->address, 6); //对方地址
            polyc_set_ble_cmd(MCU_SET_BLE_CLIENT_CONNECT, address_info, sizeof(address_info));  //BLE主机发起连接监听
        }
    }
}

//ble_client主机端数据接收回调函数
static void (*mcu_ble_client_data_receive_func_callback)(u16 value_handle, u16 offset, u8 *buffer, u16 buffer_size) =NULL;
void mcu_ble_client_data_receive_register_callback(void *cbk(u16 value_handle, u16 offset, u8 *buffer, u16 buffer_size))
{
    mcu_ble_client_data_receive_func_callback =cbk;
}

//*ble_client主机接收到从机server端的数据回调函数   注意:函数由系统底层回调，请不要其他方调用*/
static void mcu_ble_client_report_gatt_data_callback(polyc_att_data_report_t *report_data)
{
    //printf("packet_type:0x%04x, conn_handle:0x%04x, value_handle:0x%04x\n", report_data->packet_type, report_data->conn_handle, report_data->value_handle);
    //printf("value_offset:%d, blob_length=%d\n", report_data->value_offset, report_data->blob_length);
    //printf_buf(report_data->blob, report_data->blob_length);

    switch (report_data->packet_type)
    {
    case 0xA7://notify
        if(mcu_ble_client_data_receive_func_callback){
            mcu_ble_client_data_receive_func_callback(report_data->value_handle, report_data->value_offset, report_data->blob, report_data->blob_length);
        }
        break;

    case 0xA8://indicate
        break;

    case 0xA5://read
        if(report_data->blob_length > sizeof(curr_client_peer_device_name_info) ){
            report_data->blob_length =sizeof(curr_client_peer_device_name_info);
        }
        memcpy(curr_client_peer_device_name_info, report_data->blob, report_data->blob_length); //存当前连接对方设备名信息
        printf("peer_device_name:%s\n", curr_client_peer_device_name_info);
        break;

    case 0xA6://read long
        break;

    default:
        break;
    }
}

// ble_client主机端发数接口  返回0,发送成功
int mcu_ble_client_data_send(u8 *buff, u16 buff_size)
{
    return polyc_ble_client_data_send(buff, buff_size, MCU_BLE_ATT_OP_WRITE_WITHOUT_RESPOND);
}


 //设置BLE主机连接搜索指定uuid
static void mcu_ble_client_set_search_uuid_table(void)
{
    //读取远端server设备名device_name
    client_search_uuid_table[0].services_uuid16 =0x1800;
    client_search_uuid_table[0].characteristic_uuid16 =0x2a00;
    client_search_uuid_table[0].opt_type =POLYC_ATT_PROPERTY_READ;

    //UUID16_NOTIFY    下行数据通道:ble_client主机接收远端BLE从机的数据通道
    client_search_uuid_table[1].services_uuid16 =0xFFF0;
    client_search_uuid_table[1].characteristic_uuid16 =0xFFF1;
    client_search_uuid_table[1].opt_type =POLYC_ATT_PROPERTY_NOTIFY;

    //UUID16_WRITE    上行数据通道:ble_client主机发送给远端BLE从机的数据通道
    client_search_uuid_table[2].services_uuid16 =0xFFF0;
    client_search_uuid_table[2].characteristic_uuid16 =0xFFF2;
    client_search_uuid_table[2].opt_type =POLYC_ATT_PROPERTY_WRITE_WITHOUT_RESPONSE;
}

//ble_client主机初始化函数
void mcu_ble_client_init(void)
{
	printf("--> mcu_ble_client_init\n");

    polyc_ble_client_state_change_register_callback(mcu_ble_client_state_change_callback); //注册ble_client主机状态变化函数

    polyc_ble_client_report_adv_data_register_callback(mcu_ble_client_report_adv_data_callback); //注册ble_client主机广播包数据接收回调函数
    polyc_ble_client_report_gatt_data_register_callback(mcu_ble_client_report_gatt_data_callback); //注册主机client接收到从机server端的数据回调函数

    polyc_ble_client_scan_config_set(SET_TYPE_SCAN_ACTIVE, MCU_BLE_CLIENT_SET_SCAN_INTERVAL, MCU_BLE_CLIENT_SET_SCAN_WINDOW);  //BLE主机扫描参数配置
    polyc_ble_client_conn_config_set(MCU_BLE_CLIENT_SET_CONN_INTERVAL, MCU_BLE_CLIENT_SET_CONN_LATENCY, MCU_BLE_CLIENT_SET_CONN_TIMEOUT);  //BLE主机连接参数配置

    mcu_ble_client_set_search_uuid_table(); //设置BLE主机连接指定搜索uuid
}

void mcu_ble_client_set_device_match(u8 creat_mode, u8 *data, u16 data_len) //设置BLE主机连接指定设备
{
    printf("client_set_device_match:%d\n", creat_mode);

    switch(creat_mode)
    {
    case POLYC_CLIENT_CLI_CREAT_BY_ADDRESS: //指定地址创建连接
        memset(&set_device_match_creat_by_address, 0, sizeof(set_device_match_creat_by_address));

        if( (data_len == 0) || (data == NULL) ){//清指定地址创建连接标志
            set_device_match_creat_by_address_flag =0;
            break;
        }

        if(data_len > sizeof(set_device_match_creat_by_address)){
           data_len =sizeof(set_device_match_creat_by_address);
        }
        memcpy(&set_device_match_creat_by_address, data, data_len); //1位地址类型 + 6位mac地址
        set_device_match_creat_by_address_flag =1; //置上指定地址创建连接标志
        break;

    case POLYC_CLIENT_CLI_CREAT_BY_NAME: //指定设备名称创建连接
        memset(&set_device_match_creat_by_name, 0, sizeof(set_device_match_creat_by_name));

        if( (data_len == 0) || (data == NULL) ){//清指定设备名称创建连接标志
            set_device_match_creat_by_name_valid_len =0;
            break;
        }

        if(data_len > (sizeof(set_device_match_creat_by_name)-1) ){
           data_len =sizeof(set_device_match_creat_by_name)-1; //去结束符
        }
        memcpy(&set_device_match_creat_by_name, data, data_len); //存指定设备名
        set_device_match_creat_by_name_valid_len =data_len; //存指定设备名字符长度
        break;

    case POLYC_CLIENT_CLI_CREAT_BY_TAG: //匹配厂家标识创建连接
        break;
    }
}

//ble_client主机获取链路对方的信号强度, 范围-127 ~ 127
s8 mcu_get_ble_client_curr_rssi(void)
{
    u16 get_ble_cmd_len =0;
    s8 curr_rssi =0;
    polyc_get_ble_cmd(MCU_GET_BLE_CLIENT_PEER_RSSI, &curr_rssi, get_ble_cmd_len);

    //printf("client_rssi:%d\n",curr_rssi);
    return curr_rssi;
}

//获取ble_client主机连接状态
u8 mcu_get_ble_client_connect_flag(void)
{
    return ble_client_connect_flag;
}

//断开ble_client主机连接
void mcu_ble_client_disconnected(void)
{
    polyc_set_ble_cmd(MCU_SET_BLE_CLIENT_DISCONN, NULL, 0);
}

///BLE_CLIENT主机应用模式开启和关闭  1:开启  0:关闭
void mcu_ble_client_enable(u8 enable)
{
    if(enable){
        if(get_virtual_mcu_ble_app_mode() != VIRTUAL_MCU_BLE_CLIENT_APP_MODE){ //当前不在BLE_CLIENT主机应用模式
            polyc_ble_client_report_adv_data_register_callback(mcu_ble_client_report_adv_data_callback); //注册ble_client主机广播包数据接收回调函数
            polyc_ble_client_scan_config_set(SET_TYPE_SCAN_ACTIVE, MCU_BLE_CLIENT_SET_SCAN_INTERVAL, MCU_BLE_CLIENT_SET_SCAN_WINDOW);  //BLE主机扫描参数配置
            polyc_ble_client_conn_config_set(MCU_BLE_CLIENT_SET_CONN_INTERVAL, MCU_BLE_CLIENT_SET_CONN_LATENCY, MCU_BLE_CLIENT_SET_CONN_TIMEOUT);  //BLE主机连接参数配置

            polyc_set_ble_cmd(MCU_SET_BLE_CLIENT_ON, NULL, 0); //BLE_CLIENT主机应用模式打开
        }
    }else{
        polyc_set_ble_cmd(MCU_SET_BLE_CLIENT_OFF, NULL, 0); //BLE_CLIENT主机应用模式关闭
    }
}


