#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_main.h"
#include "virtual_mcu_ble_server.h"
#include "user_mcu_spi.h"

static bool ble_server_connect_flag = 0;      ///ble_server从机是否已连接状态
u8 curr_server_peer_addr_info[7];  //当前连接对方地址信息  1位addr_type + 6位address

static const u8 mcu_ble_profile_data[] = {
       //////////////////////////////////////////////////////
    //
    // 0x0001 PRIMARY_SERVICE  1800
    //
    //////////////////////////////////////////////////////
    0x0a, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x28, 0x00, 0x18,

     /* CHARACTERISTIC,  2a00, READ | DYNAMIC, */
    // 0x0002 CHARACTERISTIC 2a00 READ | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x28, 0x02, 0x03, 0x00, 0x00, 0x2a,
    // 0x0003 VALUE 2a00 READ | DYNAMIC
    0x08, 0x00, 0x02, 0x01, 0x03, 0x00, 0x00, 0x2a,

     /* CHARACTERISTIC,  2a01, READ | DYNAMIC, */
    // 0x0004 CHARACTERISTIC 2a01 READ | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x04, 0x00, 0x03, 0x28, 0x02, 0x05, 0x00, 0x01, 0x2a,
    // 0x0005 VALUE 2a01 READ | DYNAMIC
    0x08, 0x00, 0x02, 0x01, 0x05, 0x00, 0x01, 0x2a,
#if   0
    //////////////////////////////////////////////////////
    //
    // 0x0006 PRIMARY_SERVICE  1801
    //
    //////////////////////////////////////////////////////
    0x0a, 0x00, 0x02, 0x00, 0x06, 0x00, 0x00, 0x28, 0x01, 0x18,

     /* CHARACTERISTIC,  2A05, INDICATE | DYNAMIC, */
    // 0x0007 CHARACTERISTIC 2A05 INDICATE | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x07, 0x00, 0x03, 0x28, 0x20, 0x08, 0x00, 0x05, 0x2a,
    // 0x0008 VALUE 2A05 INDICATE | DYNAMIC
    0x08, 0x00, 0x20, 0x01, 0x08, 0x00, 0x05, 0x2a,
    // 0x0009 CLIENT_CHARACTERISTIC_CONFIGURATION
    0x0a, 0x00, 0x0a, 0x01, 0x09, 0x00, 0x02, 0x29, 0x00, 0x00,

     /* CHARACTERISTIC,  2B29, READ |  WRITE | DYNAMIC, */
    // 0x000a CHARACTERISTIC 2B29 READ |  WRITE | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x0a, 0x00, 0x03, 0x28, 0x0a, 0x0b, 0x00, 0x29, 0x2b,
    // 0x000b VALUE 2B29 READ |  WRITE | DYNAMIC
    0x08, 0x00, 0x0a, 0x01, 0x0b, 0x00, 0x29, 0x2b,

     /* CHARACTERISTIC,  2B2A, READ | DYNAMIC, */
    // 0x000c CHARACTERISTIC 2B2A READ | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x0c, 0x00, 0x03, 0x28, 0x02, 0x0d, 0x00, 0x2a, 0x2b,
    // 0x000d VALUE 2B2A READ | DYNAMIC
    0x08, 0x00, 0x02, 0x01, 0x0d, 0x00, 0x2a, 0x2b,
#endif // 0
    //////////////////////////////////////////////////////
    //
    // 0x000e PRIMARY_SERVICE  FFE0
    //
    //////////////////////////////////////////////////////
    0x0a, 0x00, 0x02, 0x00, 0x0e, 0x00, 0x00, 0x28, 0xe0, 0xff,

     /* CHARACTERISTIC,   FFE2, WRITE | WRITE_WITHOUT_RESPONSE | DYNAMIC, */
    // 0x000f CHARACTERISTIC FFE2 WRITE | WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x0f, 0x00, 0x03, 0x28, 0x0c, 0x10, 0x00, 0xe2, 0xff,
    // 0x0010 VALUE FFE2 WRITE | WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x08, 0x00, 0x0c, 0x01, 0x10, 0x00, 0xe2, 0xff,

     /* CHARACTERISTIC,   FFE1, NOTIFY  | DYNAMIC, */
    // 0x0011 CHARACTERISTIC FFE1 NOTIFY  | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x11, 0x00, 0x03, 0x28, 0x10, 0x12, 0x00, 0xe1, 0xff,
    // 0x0012 VALUE FFE1 NOTIFY  | DYNAMIC
    0x08, 0x00, 0x10, 0x01, 0x12, 0x00, 0xe1, 0xff,
    // 0x0013 CLIENT_CHARACTERISTIC_CONFIGURATION
    0x0a, 0x00, 0x0a, 0x01, 0x13, 0x00, 0x02, 0x29, 0x00, 0x00,


#if 1 //**************************注意:原厂微信小程序OAT升级使用,不要随意修改***************//
    //////////////////////////////////////////////////////
    //
    // 0x0004 PRIMARY_SERVICE  ae00
    //
    //////////////////////////////////////////////////////
    0x0a, 0x00, 0x02, 0x00, 0x80, 0x00, 0x00, 0x28, 0x00, 0xae,

    /* CHARACTERISTIC,  ae01, WRITE_WITHOUT_RESPONSE | DYNAMIC, */
    // 0x0040 CHARACTERISTIC ae01 WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x81, 0x00, 0x03, 0x28, 0x04, 0x82, 0x00, 0x01, 0xae,
    // 0x0041 VALUE ae01 WRITE_WITHOUT_RESPONSE | DYNAMIC
    0x08, 0x00, 0x04, 0x01, 0x82, 0x00, 0x01, 0xae,

    /* CHARACTERISTIC,  ae02, NOTIFY, */
    // 0x0042 CHARACTERISTIC ae02 NOTIFY
    0x0d, 0x00, 0x02, 0x00, 0x83, 0x00, 0x03, 0x28, 0x10, 0x84, 0x00, 0x02, 0xae,
    // 0x0043 VALUE ae02 NOTIFY
    0x08, 0x00, 0x10, 0x00, 0x84, 0x00, 0x02, 0xae,
    // 0x0044 CLIENT_CHARACTERISTIC_CONFIGURATION
    0x0a, 0x00, 0x0a, 0x01, 0x85, 0x00, 0x02, 0x29, 0x00, 0x00,
#endif

    // END
    0x00, 0x00,
};
//
// characteristics <--> handles
//
//**************************注意:原厂微信小程序OAT升级,HANDLE值不要修改***************//
#define MCU_BLE_ATT_CHARACTERISTIC_ae01_02_VALUE_HANDLE  0x0082
#define MCU_BLE_ATT_CHARACTERISTIC_ae02_02_VALUE_HANDLE  0x0084
#define MCU_BLE_ATT_CHARACTERISTIC_ae02_02_CLIENT_CONFIGURATION_HANDLE  0x0085
//***********************************************************************//

#define ATT_CHARACTERISTIC_2a00_01_VALUE_HANDLE 0x0003
#define ATT_CHARACTERISTIC_2a01_01_VALUE_HANDLE 0x0005

#if   0
#define ATT_CHARACTERISTIC_2A05_01_VALUE_HANDLE 0x0008
#define ATT_CHARACTERISTIC_2A05_01_CLIENT_CONFIGURATION_HANDLE 0x0009
#define ATT_CHARACTERISTIC_2B29_01_VALUE_HANDLE 0x000b
#define ATT_CHARACTERISTIC_2B2A_01_VALUE_HANDLE 0x000d
#endif // 0

#define ATT_CHARACTERISTIC_FFE2_01_VALUE_HANDLE 0x0010
#define ATT_CHARACTERISTIC_FFE1_01_VALUE_HANDLE 0x0012
#define ATT_CHARACTERISTIC_FFE1_01_CLIENT_CONFIGURATION_HANDLE 0x0013

const char BLE_READ_DATA[16] =
{
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
//*蓝牙BLE从机状态变化函数   注意:函数由系统底层回调，请不要在其他方调用*/
static void mcu_ble_server_state_change_callback(u8 state, u8 *priv, u16 len)
{
    printf("-->server_state_change:0x%02x\n", state);
	/*
    if(priv && len){
        printf("server--->priv:\n");
        printf_buf(priv,len);
    }
	*/
    switch(state)
    {
        case POLYC_BLE_ST_INIT_OK: //BLE从机协议栈初始化ok
            printf("virtual_mcu_local_ble_mac_addr:\n");
            printf_buf(virtual_mcu_local_ble_mac_addr,6); //本地BLE--mac地址
            ble_server_connect_flag =0;  //蓝牙BLE从机已断开

            break;

        case POLYC_BLE_ST_DISCONN:
            printf("disconn_reason:0x%02x\n", priv[0]);
            ble_server_connect_flag =0;  //蓝牙BLE从机已断开
            break;

        case POLYC_BLE_ST_CONNECT:
            memcpy(curr_server_peer_addr_info, priv, 7); //存当前连接对方地址信息
            ble_server_connect_flag =1;  //蓝牙BLE从机已连接
            printf("curr_server_peer_addr_info:\n");
            printf_buf(curr_server_peer_addr_info, 7);
            break;

        case POLYC_BLE_ST_NOTIFY_IDICATE:
            break;

        case POLYC_BLE_MTU_EXCHANGE_COMPLETE:
            u16 curr_ble_mtu =0;
            if(priv){
                memcpy(&curr_ble_mtu, priv, 2);
                //printf("curr_ble_mtu =%d\n",curr_ble_mtu);
            }
            break;

        case POLYC_BLE_RESUME_ALL_CCC_ENABLE: //主动使能全部通知
            u16 connection_handle =0;
            if(priv){
                memcpy(&connection_handle, priv, 2);
                //printf("connection_handle=0x%04x\n",connection_handle);
                if(connection_handle){
                    polyc_ble_gatt_server_characteristic_ccc_set(connection_handle, ATT_CHARACTERISTIC_FFE1_01_CLIENT_CONFIGURATION_HANDLE, MCU_BLE_ATT_OP_NOTIFY, 0);
                    polyc_ble_gatt_server_characteristic_ccc_set(connection_handle, MCU_BLE_ATT_CHARACTERISTIC_ae02_02_CLIENT_CONFIGURATION_HANDLE, MCU_BLE_ATT_OP_NOTIFY, 1);
                }
            }
            break;
    }
}

#define POLYC_ADV_RSP_PACKET_MAX   (31)  //最大31,不可修改

//按(长度 + 类型 + 内容)这样的格,组合填入广播包数据
static inline u8 polyc_make_eir_packet_data(u8 *buf, u16 offset, u8 data_type, u8 *data, u8 data_len)
{
    if (POLYC_ADV_RSP_PACKET_MAX - offset < data_len + 2) {
        return offset + data_len + 2;
    }

    buf[0] = data_len + 1;
    buf[1] = data_type;
    memcpy(buf + 2, data, data_len);
    return data_len + 2;
}

//按(长度 + 类型 + 内容)这样的格,组合填入广播包数据
static inline u8 polyc_make_eir_packet_val(u8 *buf, u16 offset, u8 data_type, u32 val, u8 val_size)
{
    if (POLYC_ADV_RSP_PACKET_MAX - offset < val_size + 2) {
        return offset + val_size + 2;
    }

    buf[0] = val_size + 1;
    buf[1] = data_type;
    memcpy(buf + 2, &val, val_size);
    return val_size + 2;
}


//生成adv包数据,设置BLE从机广播包数据
static u8  adv_name_ok = 0;//name 优先存放在ADV包
static int ble_server_make_set_adv_data(void)
{
    u8 adv_data[POLYC_ADV_RSP_PACKET_MAX] ={0}; //max is 31
    u8 offset = 0;

    offset += polyc_make_eir_packet_val(&adv_data[offset], offset, BLE_HCI_EIR_DATATYPE_FLAGS,  BIT(1)|BIT(2), 1);
    offset += polyc_make_eir_packet_val(&adv_data[offset], offset, BLE_HCI_EIR_DATATYPE_COMPLETE_16BIT_SERVICE_UUIDS, 0xFFF0, 2); //16BIT_SERVICE_UUIDS

    u8 name_len = strlen(virtual_mcu_local_ble_name);
    u8 vaild_len = POLYC_ADV_RSP_PACKET_MAX - (offset + 2);

    if (name_len < vaild_len) {
        offset += polyc_make_eir_packet_data(&adv_data[offset], offset, BLE_HCI_EIR_DATATYPE_COMPLETE_LOCAL_NAME, virtual_mcu_local_ble_name, name_len);
        adv_name_ok = 1;
    } else {
        adv_name_ok = 0;
    }

    if (offset > POLYC_ADV_RSP_PACKET_MAX) {
        printf("***adv_data overflow!!!!!!\n");
        return -1;
    }

    printf("adv_data(%d):", offset);
    printf_buf(adv_data, offset);

    set_polyc_ble_server_adv_data(adv_data, offset);

    return 0;
}

//构造rsp包数据,设置BLE从机广播包数据
static int ble_server_make_set_rsp_data(void)
{
    u8 rsp_data[POLYC_ADV_RSP_PACKET_MAX] ={0}; //max is 31
    u8 offset = 0;

    if (!adv_name_ok) {
        u8 name_len = strlen(virtual_mcu_local_ble_name);
        u8 vaild_len = POLYC_ADV_RSP_PACKET_MAX - (offset + 2);
        if (name_len > vaild_len) {
            name_len = vaild_len;
        }
        offset += polyc_make_eir_packet_data(&rsp_data[offset], offset, BLE_HCI_EIR_DATATYPE_COMPLETE_LOCAL_NAME, virtual_mcu_local_ble_name, name_len);
    }

    if (offset > POLYC_ADV_RSP_PACKET_MAX) {
        printf("***rsp_data overflow!!!!!!\n");
        return -1;
    }

    printf("rsp_data(%d):", offset);
    printf_buf(rsp_data, offset);

    set_polyc_ble_server_rsp_data(rsp_data, offset);

    return 0;
}

static void mcu_ble_server_set_adv_config(void) //ble从机server广播参数设置
{
    int ret = 0;
    ret |= ble_server_make_set_adv_data();
    ret |= ble_server_make_set_rsp_data();

    if (ret) {
        printf("server_set_adv_config fail!!!\n");
        return;
    }

    polyc_ble_server_adv_param_set(MCU_BLE_SERVER_ADV_INTERVAL_MIN, MCU_BLE_SERVER_ADV_IND, MCU_BLE_SERVER_ADV_CHANNEL_ALL);
}

// BLE从机server端发数接口  返回0,发送成功
int mcu_ble_server_data_send(u8 *buff, u16 buff_size)
{
    //printf("mcu_ble_server_data_send:%d\n", buff_size);
	//printf_buf(buff,buff_size);
    return polyc_ble_server_data_send(ATT_CHARACTERISTIC_FFE1_01_VALUE_HANDLE, buff, buff_size, MCU_BLE_ATT_OP_AUTO_READ_CCC);
}

//ble_att_read回调函数
static u16 mcu_ble_server_att_read_callback(u16 connection_handle, u16 att_handle, u16 offset, u8 *buffer, u16 buffer_size)
{
    u16  att_value_len = 0;
    u16 handle = att_handle;

    ///printf("mcu_ble_read_callback, handle= 0x%04x,buffer= %08x\n", handle, (u32)buffer);

    switch (handle) {
    case ATT_CHARACTERISTIC_2a00_01_VALUE_HANDLE:
        att_value_len = strlen(virtual_mcu_local_ble_name);  //BLE设备名长度,不包含结束符

        if ((offset >= att_value_len) || (offset + buffer_size) > att_value_len) {
            break;
        }

        if (buffer) {
            memcpy(buffer, &virtual_mcu_local_ble_name[offset], buffer_size);
            att_value_len = buffer_size;
            printf("\n------read local_ble_name:%s\n", virtual_mcu_local_ble_name);
        }
        break;
    #if  0
    case ATT_CHARACTERISTIC_2B29_01_VALUE_HANDLE:

    	break;
    case ATT_CHARACTERISTIC_2B2A_01_VALUE_HANDLE:
        {
	        att_value_len = sizeof(BLE_READ_DATA);
	        if ((offset >= att_value_len) || (offset + buffer_size) > att_value_len) {
	            break;
	        }

	        if (buffer) {
	            memcpy(buffer, &BLE_READ_DATA[offset] ,buffer_size);
	            att_value_len = buffer_size;
	            printf("\n------read DATA_STRING: %s\n", att_value_len);
	        }
        }
        break;
    #endif // 0
    case ATT_CHARACTERISTIC_FFE1_01_CLIENT_CONFIGURATION_HANDLE:
        if (buffer) {
            buffer[0] = ble_gatt_server_characteristic_ccc_get(connection_handle, handle);
            buffer[1] = 0;
        }
        att_value_len = 2;
        break;

    default:
        break;
    }

    return att_value_len;
}

//注册ble_server从机端收数接口
static void (*mcu_ble_server_data_receive_func_callback)(u16 value_handle, u16 offset, u8 *buffer, u16 buffer_size) =NULL;
void mcu_ble_server_data_receive_register_callback(void *cbk(u16 value_handle, u16 offset, u8 *buffer, u16 buffer_size))
{
    mcu_ble_server_data_receive_func_callback =cbk;
}

//ble_att_write回调函数
static int mcu_ble_server_att_write_callback(u16 connection_handle, u16 att_handle, u16 transaction_mode, u16 offset, u8 *buffer, u16 buffer_size)
{
    u16 handle = att_handle;

    ///printf("mcu_ble_write_callback, handle= 0x%04x,size = %d\n", handle, buffer_size);

    switch (handle) {

    case ATT_CHARACTERISTIC_2a00_01_VALUE_HANDLE:
		break;

	case ATT_CHARACTERISTIC_FFE1_01_CLIENT_CONFIGURATION_HANDLE:
        printf("\n------write ccc:%04x,%02x\n", handle, buffer[0]);
        polyc_ble_gatt_server_characteristic_ccc_set(connection_handle, handle, buffer[0], 1);
        break;

	case ATT_CHARACTERISTIC_FFE2_01_VALUE_HANDLE:
		if(buffer && buffer_size)
		{
		    if(mcu_ble_server_data_receive_func_callback){
                mcu_ble_server_data_receive_func_callback(handle, offset, buffer, buffer_size);
		    }
		}
		break;

    #if 1 //原厂微信小程序OAT升级
    case MCU_BLE_ATT_CHARACTERISTIC_ae02_02_CLIENT_CONFIGURATION_HANDLE:
        polyc_ble_gatt_server_receive_update_characteristic_ccc_set(connection_handle, att_handle, MCU_BLE_ATT_CHARACTERISTIC_ae02_02_VALUE_HANDLE, buffer, buffer_size);
        break;

    case MCU_BLE_ATT_CHARACTERISTIC_ae01_02_VALUE_HANDLE:
      	polyc_ble_gatt_server_receive_update_data(NULL, buffer, buffer_size);
        break;
    #endif

    default:
        break;
    }

    return 0;
}

static char mcu_Hex2Char(const unsigned char value) //十六进制转字符函数
{
    char dst;
    if (value < 10) {
        dst = value + '0';
    } else {
        dst = value - 10 + 'A';
    }
    return dst;
}

static void mcu_ble_local_name_ext_mac_char(void)//本地BLE设备名增加地址后缀函数
{
    char ext_name[] = "-0000";
    int len =strlen(virtual_mcu_local_ble_name);
    if(len > (28 -5)) {
        len =(28 -5);
    }

    //取本地BLE两位地址转为4个字符作为BLE设备名后缀
    ext_name[0] = '-';
    ext_name[1] = mcu_Hex2Char((virtual_mcu_local_ble_mac_addr[1]>>4)&0x0F);
    ext_name[2] = mcu_Hex2Char((virtual_mcu_local_ble_mac_addr[1]>>0)&0x0F);
    ext_name[3] = mcu_Hex2Char((virtual_mcu_local_ble_mac_addr[0]>>4)&0x0F);
    ext_name[4] = mcu_Hex2Char((virtual_mcu_local_ble_mac_addr[0]>>0)&0x0F);

    virtual_mcu_local_ble_name[len++] = ext_name[0];
    virtual_mcu_local_ble_name[len++] = ext_name[1];
    virtual_mcu_local_ble_name[len++] = ext_name[2];
    virtual_mcu_local_ble_name[len++] = ext_name[3];
    virtual_mcu_local_ble_name[len++] = ext_name[4];
    virtual_mcu_local_ble_name[len++] =0; //结束符

	printf("modify--->mcu_local_ble_name[len:%d]:%s\n", strlen(virtual_mcu_local_ble_name), virtual_mcu_local_ble_name);
}


//ble_server从机初始化函数
void mcu_ble_server_init(void)
{
	printf("--> mcu_ble_server_init\n");

	#if 0//修改本地BLE地址。注意地址大小端问题，virtual_mcu_local_ble_mac_addr为小端格式
	virtual_mcu_local_ble_mac_addr[5] =0xAA; //固定前两位地址
    virtual_mcu_local_ble_mac_addr[4] =0xBB;
    #endif

    //mcu_ble_local_name_ext_mac_char();//BLE设备名增加地址后缀

    set_polyc_ble_profile_data(&mcu_ble_profile_data, sizeof(mcu_ble_profile_data)); //配置ble_profile_data

    polyc_ble_server_state_change_register_callback(mcu_ble_server_state_change_callback); //注册ble_server从状态变化函数
    polyc_ble_att_read_register_callback(mcu_ble_server_att_read_callback); //注册ble_att_read回调函数
    polyc_ble_att_write_register_callback(mcu_ble_server_att_write_callback); //注册ble_att_write回调函数

    mcu_ble_server_set_adv_config(); //ble_server从机广播包参数设置
}

//ble_server从机获取链路对方的信号强度, 范围-127 ~ 127
s8 mcu_get_ble_server_curr_rssi(void)
{
    u16 get_ble_cmd_len =0;
    s8 curr_rssi;
    polyc_get_ble_cmd(MCU_GET_BLE_SERVER_PEER_RSSI, &curr_rssi, get_ble_cmd_len);

    //printf("server_rssi:%d\n",curr_rssi);
    return curr_rssi;
}

///获取ble从机当前连接状态
u8 mcu_get_ble_server_connect_flag(void)
{
    return ble_server_connect_flag;
}

void mcu_ble_server_disconnected(void) ///设置ble_server从机断开连接
{
     polyc_set_ble_cmd(MCU_SET_BLE_SERVER_DISCONN, NULL, 0);  //断开BLE从机连接
}

void mcu_ble_server_enable(u8 enable)   ///BLE_SERVER从机应用模式开启和关闭
{
    if(enable){
        if(get_virtual_mcu_ble_app_mode() != VIRTUAL_MCU_BLE_SERVER_APP_MODE){ //当前不在BLE_SERVER从机应用模式
            mcu_ble_server_set_adv_config(); //ble_server从机广播包参数设置
            polyc_set_ble_cmd(MCU_SET_BLE_SERVER_ON, NULL, 0); //BLE_SERVER从机应用模式打开
        }
    }else{
        polyc_set_ble_cmd(MCU_SET_BLE_SERVER_OFF, NULL, 0); //BLE_SERVER从机应用模式关闭
    }
}

