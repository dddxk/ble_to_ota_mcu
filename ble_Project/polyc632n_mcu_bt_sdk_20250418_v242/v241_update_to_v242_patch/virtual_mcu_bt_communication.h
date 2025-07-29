#ifndef _VIRTUAL_MCU_BT_COMMUNICATION_H_
#define _VIRTUAL_MCU_BT_COMMUNICATION_H_


#include "system/includes.h"
#include "app_config.h"


/*---------------------------------------------------------------------------------------------------*/
/*                                          系统相关配置变量                                         */
/*---------------------------------------------------------------------------------------------------*/
extern u8 virtual_mcu_debug_tx_pin;             //调试用的串口打印接口
extern u32 virtual_mcu_debug_baudrate;          //调试用的串口波特率



/*---------------------------------------------------------------------------------------------------*/
/*
	系统delay函数，纯等待，delay(1);小于1us，具体根据实际运行为准，因为高优先级中断会影响它的等待时间
*/
/*---------------------------------------------------------------------------------------------------*/
extern void delay(unsigned int);

/*---------------------------------------------------------------------------------------------------*/
/*                                      us级定时器                                                   */
/*---------------------------------------------------------------------------------------------------*/
void polyc_fast_period_timer_init(u32 period_us);
void polyc_fast_period_timer_register_callback( void *cbk(void) );

/*---------------------------------------------------------------------------------------------------*/
/*                                ms级定时器，用于main函数调用                                       */
/*---------------------------------------------------------------------------------------------------*/
void polyc_lib_timer_init(u32 period_ms);
void polyc_lib_timer_register_callback( void *cbk(void) );



/*---------------------------------------------------------------------------------------------------*/
/*                                          蓝牙BLE相关配置                                          */
/*---------------------------------------------------------------------------------------------------*/
#define VIRTUAL_MCU_BLE_APP_MODE_NULL              0x00   //BLE无应用
#define VIRTUAL_MCU_BLE_SERVER_APP_MODE            0x01   //BLE_SERVER从机数据透传应用模式
#define VIRTUAL_MCU_BLE_CLIENT_APP_MODE            0x02   //BLE_CLIENT主机数据透传应用模式
#define VIRTUAL_MCU_BLE_NONCONN_RX_TX_APP_MODE     0x03   //基于非连接BLE广播收发应用模式

u8 get_virtual_mcu_ble_app_mode(void); //获取当前BLE应用模式


extern char virtual_mcu_local_ble_name[29];   //设置本地BLE名字，最大为29个字节
extern u8 virtual_mcu_local_ble_mac_addr[6];  //获取本地BLE--mac地址

typedef enum {
    MCU_BLE_ATT_OP_AUTO_READ_CCC = 0,    //server端 检查对方使能通知CCC来控制 notify or indicate发送
    MCU_BLE_ATT_OP_NOTIFY = 1,           //server端 默认notify方式发送，不检查使能通知CCC   (不需要对方回应答包,没有流控)
    MCU_BLE_ATT_OP_INDICATE = 2,         //server端 默认INDICATE方式发送，不检查使能通知CCC (需要对方回应答包,有流控)
    MCU_BLE_ATT_OP_READ,                 //client端 单次读, 用于获取<=MTU 的数据包    (需要对方回应答包,有流控)
    MCU_BLE_ATT_OP_READ_LONG,            //client端 多次读，用于支持获取>MTU 的数据包 (需要对方回应答包,有流控)
    MCU_BLE_ATT_OP_WRITE,                //client端 写发送 (需要对方回应答包,有流控)
    MCU_BLE_ATT_OP_WRITE_WITHOUT_RESPOND,//client端 写发送 (不需要对方回应答包,没有流控)
    //add here

    MCU_BLE_ATT_OP_CMD_MAX = 15,
} mcu_ble_att_op_type_e;

typedef struct {
    //服务16bit uuid
    u16 services_uuid16;
    //服务16bit uuid
    u16 characteristic_uuid16;
    u8  services_uuid128[16];
    u8  characteristic_uuid128[16];
    u16 opt_type; //属性
    u8 read_long_enable: 1; //en
    u8 read_report_reference: 1; //en
} polyc_target_uuid_t;
extern polyc_target_uuid_t  client_search_uuid_table[3];  //BLE主机连接后搜索指定uuid列表

/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    注册ble_client主机状态变化函数
   @param	 *cbk  注册的状态变化回调函数
   @return   无
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
void polyc_ble_client_state_change_register_callback(void *cbk(u8 state, u8 *priv, u16 len));

/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    ble_client主机扫描参数配置
   @param    scan_type:      搜索类型
   @param	 scan_interval:  搜索周期大小(unit: 0.625ms)
   @param	 scan_window:    搜索窗口大小(unit: 0.625ms) <= scan_interval
   @return   无
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
void polyc_ble_client_scan_config_set(u8 scan_type, u16 scan_interval, u16 scan_window);

/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    ble_client主机连接参数配置
   @param    conn_interval:  连接周期(unit:1.25ms), 最小的conn_interval为6
   @param	 conn_latency:   连接latency(unit:1.25ms)
   @param	 supervision_timeout:  连接超时(unit:10ms)
   @return   无
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
void polyc_ble_client_conn_config_set(u16 conn_interval, u16 conn_latency, u16 supervision_timeout);

typedef struct {
    u8   event_type;    //对方广播包类型: 0--ADV_IND,1--ADV_DIRECT_IND,2--ADV_SCAN_IND,3--ADV_NONCONN_IND,4--SCAN_RSP
    u8   address_type;  //对方地址类型：0--public address,1--random address
    u8   address[6];    //peer_address
    s8   rssi;          //range:-127 ~128 dbm
    u8   length;        //广播包长度
    u8   data[0];       //广播包内容
} polyc_adv_report_t;
/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    注册ble_client主机广播包数据回调函数
   @param	 *cbk 注册的回调函数
   @return   无
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
void polyc_ble_client_report_adv_data_register_callback(void *cbk(polyc_adv_report_t *report_pt, u16 len));

typedef struct {
    u16  packet_type;  //数据包类型(notify,indicate,read_respone,...)
    u16  value_handle; //属性操作handle
    u16  value_offset; //包偏移
    u16  blob_length;  //包长度
    u8  *blob;         //包内容
    u16  conn_handle;  //连接handle
} polyc_att_data_report_t;
/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    注册ble_client主机接收到从机server端的数据回调函数
   @param	 *cbk 注册的回调函数
   @return   无
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
void polyc_ble_client_report_gatt_data_register_callback(void *cbk(polyc_att_data_report_t *report_data));

/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    BLE主机发数接口
   @param    *buff 发送数据的指针
   @param	 buff_size 发送数据的长度
   @return   返回值: 0      表示成功
			 其他值         错误代码
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
int polyc_ble_client_data_send(u8 *buff, u16 buff_size, mcu_ble_att_op_type_e handle_type); //BLE主机发数接口



void set_polyc_ble_profile_data(u8 *data, u16 size); //设置ble_profile_data数据

/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    设置ble_server从机广播包adv数据
   @param    *adv 广播包adv数据指针
   @param	 adv_len 广播包adv数据长度，最大长度31
   @return   返回值: 广播包adv数据长度
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
u8 set_polyc_ble_server_adv_data(u8 *adv, u8 adv_len);

/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    设置ble_server从机广播包rsp数据
   @param    *adv 广播包rsp数据指针
   @param	 adv_len 广播包rsp数据长度，最大长度31
   @return   返回值: 广播包rsp数据长度
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
u8 set_polyc_ble_server_rsp_data(u8 *rsp, u8 rsp_len);

/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    注册ble_server从机状态变化函数
   @param	 *cbk  注册的状态变化回调函数
   @return   无
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
void polyc_ble_server_state_change_register_callback(void *cbk(u8 state, u8 *priv, u16 len));

/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    ble_server从机广播参数设置
   @param    adv_interval: 广播周期 (unit:0.625ms)
   @param	 adv_type:     广播类型
   @param	 adv_channel:  广播通道
   @return   无
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
void polyc_ble_server_adv_param_set(u16 adv_interval, u8 adv_type, u8 adv_channel);

/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    ble_server从机端发数接口 (上行数据:ble_server从机发送数据到BLE主机如手机APP)
   @param    *buff 发送数据的指针
   @param	 buff_size 发送数据的长度
   @return   返回值: 0      表示成功
			 其他值         错误代码
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
int polyc_ble_server_data_send(u16 handle, u8 *buff, u16 buff_size, mcu_ble_att_op_type_e handle_type);

/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    ble_server从机端ble_att_read回调注册函数
   @param	 *cbk  注册的回调函数
   @return   无
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
void polyc_ble_att_read_register_callback(u16 *cbk(u16 connection_handle, u16 att_handle, u16 offset, u8 *buffer, u16 buffer_size));

/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    ble_server从机端ble_att_write回调注册函数
   @param	 *cbk  注册的回调函数
   @return   无
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
void polyc_ble_att_write_register_callback(int *cbk(u16 connection_handle, u16 att_handle, u16 transaction_mode, u16 offset, u8 *buffer, u16 buffer_size));


u16 polyc_ble_gatt_server_characteristic_ccc_get(u16 conn_handle, u16 att_ccc_handle); //获取 notify or indicate 通知使能值

void polyc_ble_gatt_server_characteristic_ccc_set(u16 conn_handle, u16 att_ccc_handle, u16 ccc_config, u8 update_request);  //更新 notify or indicate 通知功能使能

//原厂微信小程序OAT升级使用接口
void polyc_ble_gatt_server_receive_update_characteristic_ccc_set(u16 conn_handle, u16 att_ccc_handle, u16 set_update_handle, u8 *buffer, u16 buffer_size);
void polyc_ble_gatt_server_receive_update_data(void *priv, void *buf, u16 len);

/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    基于蓝牙无连接BLE广播发数接口
   @param    *buff 发送数据的指针
   @param	 buff_size 发送数据的长度,最大长度31
   @param	 tx_interval 广播该数据持续的时间(单位:10ms)
   @return   返回值: 0   表示失败
			 非0         表示成功,当前发送数据的长度
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
u8 polyc_set_ble_nonconn_adv_tx(u8 *buff, u8 buff_size, u8 tx_interval);

/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    获取BLE广播发数状态
   @return   返回值 1			BLE广播发数在忙
   			 返回值 0			BLE广播发数结束
   @note
*/
/*---------------------------------------------------------------------------------------------------*/
u8 get_polyc_ble_nonconn_adv_tx_busy(void);


//ble_state状态
enum
{
    POLYC_BLE_ST_NULL = 0,
    POLYC_BLE_ST_INIT_OK,             //协议栈初始化ok
    POLYC_BLE_ST_IDLE,                //关闭广播或扫描状态
    POLYC_BLE_ST_CONNECT,             //链路刚连上
    POLYC_BLE_ST_SEND_DISCONN,        //发送断开命令，等待链路断开
    POLYC_BLE_ST_DISCONN,             //链路断开状态

    POLYC_BLE_ST_NOTIFY_IDICATE =0x21,  //设备已连上,允许发数(已被主机使能通知)

    POLYC_BLE_MTU_EXCHANGE_COMPLETE =0x30,  //ATT的MTU交换完成
    POLYC_BLE_RESUME_ALL_CCC_ENABLE,
};


//cmd_id
enum
{
    MCU_SET_BLE_APP_MODE_NULL=0,   ///关闭所有BLE功能


/*VIRTUAL_MCU_BLE_SERVER_APP_MODE*/
    MCU_SET_BLE_SERVER_ON,          ///打开BLE_SERVER从机功能
	MCU_SET_BLE_SERVER_OFF,         ///关闭BLE_SERVER从机功能
	MCU_SET_BLE_SERVER_DISCONN,     ///断开BLE_SERVER从机连接
    MCU_GET_BLE_SERVER_PEER_RSSI,   ///BLE_CLIENT从机获取链路对方的信号强度


/*VIRTUAL_MCU_BLE_CLIENT_APP_MODE*/
	MCU_SET_BLE_CLIENT_ON,          ///打开BLE_CLIENT主机功能
	MCU_SET_BLE_CLIENT_OFF,         ///关闭BLE_CLIENT主机功能
	MCU_SET_BLE_CLIENT_DISCONN,     ///断开BLE_CLIENT主机连接
    MCU_SET_BLE_CLIENT_CONNECT,     ///BLE_CLIENT主机发起连接
    MCU_GET_BLE_CLIENT_PEER_RSSI,   ///BLE_CLIENT主机获取链路对方的信号强度


/*VIRTUAL_MCU_BLE_NONCONN_RX_TX_APP_MODE*/
    MCU_SET_BLE_NONCONN_RX_TX_ON,    ///打开BLE非连接广播收发功能
	MCU_SET_BLE_NONCONN_RX_TX_OFF,   ///关闭BLE非连接广播收发功能

};

/*---------------------------------------------------------------------------------------------------*/
/*    函数名：polyc_set_ble_cmd
    参数  ：  u8 cmd_id,u8 *buf,u8 len
    说明  ：
    * @param cmd_id  : 设置命令参数
    * @param buf     : 设置相应功能数组
    * @param len     : 相应功能数组长度
   @return   返回值: 0      表示成功
			 其他值         错误代码
*/
/*---------------------------------------------------------------------------------------------------*/
int polyc_set_ble_cmd(u8 cmd_id, u8 *buf, u16 len);

/*---------------------------------------------------------------------------------------------------*/
/*
    函数名：polyc_get_ble_cmd
    参数  ：  u8 cmd_id,u8 *buf,u8 len
    说明  ：
    * @param cmd_id  : 获取命令参数
    * @param buf     : 获取有效数据数组
    * @param len     : 获取有效数据长度
    返回值：void
*/
/*---------------------------------------------------------------------------------------------------*/
void polyc_get_ble_cmd(u8 cmd_id, u8 *buf, u16 len);





/*毫秒 转换到 0.625ms 单位*/
#define MCU_ADV_SCAN_MS(_ms)                ((_ms) * 8 / 5)

/*eir packet_type*/
enum {
    BLE_HCI_EIR_DATATYPE_FLAGS =                                                 0x01,
    BLE_HCI_EIR_DATATYPE_MORE_16BIT_SERVICE_UUIDS =                              0x02,
    BLE_HCI_EIR_DATATYPE_COMPLETE_16BIT_SERVICE_UUIDS =                          0x03,
    BLE_HCI_EIR_DATATYPE_MORE_32BIT_SERVICE_UUIDS =                              0x04,
    BLE_HCI_EIR_DATATYPE_COMPLETE_32BIT_SERVICE_UUIDS =                          0x05,
    BLE_HCI_EIR_DATATYPE_MORE_128BIT_SERVICE_UUIDS =                             0x06,
    BLE_HCI_EIR_DATATYPE_COMPLETE_128BIT_SERVICE_UUIDS =                         0x07,
    BLE_HCI_EIR_DATATYPE_SHORTENED_LOCAL_NAME =                                  0x08,
    BLE_HCI_EIR_DATATYPE_COMPLETE_LOCAL_NAME =                                   0x09,
    BLE_HCI_EIR_DATATYPE_TX_POWER_LEVEL =                                        0x0A,
    BLE_HCI_EIR_DATATYPE_CLASS_OF_DEVICE =                                       0x0D,
    BLE_HCI_EIR_DATATYPE_SIMPLE_PAIRING_HASH_C =                                 0x0E,
    BLE_HCI_EIR_DATATYPE_SIMPLE_PAIRING_RANDOMIZER_R =                           0x0F,
    BLE_HCI_EIR_DATATYPE_SECURITY_MANAGER_TK_VALUE =                             0x10,
    BLE_HCI_EIR_DATATYPE_SECURITY_MANAGER_OOB_FLAGS =                            0x11,
    BLE_HCI_EIR_DATATYPE_SLAVE_CONNECTION_INTERVAL_RANGE =                       0x12,
    BLE_HCI_EIR_DATATYPE_16BIT_SERVICE_SOLICITATION_UUIDS =                      0x14,
    BLE_HCI_EIR_DATATYPE_128BIT_SERVICE_SOLICITATION_UUIDS =                     0x15,
    BLE_HCI_EIR_DATATYPE_SERVICE_DATA =                                          0x16,
    BLE_HCI_EIR_DATATYPE_APPEARANCE_DATA =                                       0x19,
    BLE_HCI_EIR_DATATYPE_MANUFACTURER_SPECIFIC_DATA =                            0xFF
};


/*---------------------------------------------------------------------------------------------------*/
/*
    函数名：polyc_rtc_set_time、polyc_rtc_get_time
    参数  ：current_time
    说明  ：
    * @param ：current_time  : 结构体变量

    struct sys_time {
    u16 year;
    u8 month;
    u8 day;
    u8 hour;
    u8 min;
    u8 sec;
};

    返回值：void
*/
/*---------------------------------------------------------------------------------------------------*/
void polyc_rtc_set_time(struct sys_time *current_time);


void polyc_rtc_get_time(struct sys_time *current_time);


u8 get_user_ota_update_flag();


/*---------------------------------------------------------------------------------------------------*/
/*
    函数名：irkey_init
    参数  ：u8  port: IO口
            u16 wUserCode: 用户码

    说明  ：注意!!! 红外使用的是定时器TIMER2  如果初始化了ir,别的地方请不要使用TIMER2以免冲突


    返回值：void
*/
/*---------------------------------------------------------------------------------------------------*/

int polyc_ir_init(u8 port,u16 wUserCode);

/*---------------------------------------------------------------------------------------------------*/
/*
    函数名：ir_get_key_value
    参数  ：null
    说明  ：

    返回值：value 红外码值
*/
/*---------------------------------------------------------------------------------------------------*/
u8 ir_get_value(void);
#endif
