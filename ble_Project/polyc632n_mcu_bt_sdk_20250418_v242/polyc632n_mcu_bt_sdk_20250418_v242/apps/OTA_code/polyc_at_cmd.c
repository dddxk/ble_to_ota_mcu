#include "polyc_cmd_process_common.h"
#include "polyc_buffer_manage.h"
#include "polyc_at_cmd.h"
#include "system/timer.h"
#include "syscfg_id.h"
#include "le/le_user.h"

/*--------------------------------------------------------------------------------------------------------------*/
// --- 查询指令列表
//1.版本号					AT+VER

// --- 执行指令列表
//1.恢复出厂设置			AT+FCTRST
//2.软件复位				AT+RESET

// --- 查询/设置指令列表
//1.串口波特率				AT+BAUD
//2.蓝牙设备名				AT+NAME
//3.Mac_Address				AT+ADDR
//4.透传服务uuid			AT+SERVUUID
//5.透传发送uuid			AT+TXUUID
//6.透传接收uuid			AT+RXUUID
//7.广播间隔				AT+ADVINV
//8.发射功率				AT+TXPWR
//9.BT连接状态/断开连接		AT+CONSTA

// --- 暂不支持
//10.连接间隔				AT+CONINV
//11.MTU字节				AT+MTU
//12.配对PIN				AT+PIN
//13.串口流控制				AT+FLWCTL

// --- 工厂测试指令列表
//1.FCT进入模式				AT+FCT
//2.FCT指定IO输出			AT+PINW
//3.FCT指定IO读取			AT+PINR
/*--------------------------------------------------------------------------------------------------------------*/

static u8 trans_data_parse_cnt;
static u8 trans_data_parse_step;
static u8 trans_data_parse_timeout_cnt;
static u8 trans_cmd_buf[AT_CMD_SIZE_MAX];

/*--------------------------------------------------------------------------------------------------------------*/

//static const char specialchar[]        = {'\r','\n', '?', '=', '<', '>'};//separate

//通用指令列表
static const char at_str_ver[]      	= "AT+VER";			// size:6
static const char at_str_fctrst[]		= "AT+FCTRST";		// size:9
static const char at_str_reset[]		= "AT+RESET";		// size:8
static const char at_str_baud[]			= "AT+BAUD";		// size:7
static const char at_str_name[]			= "AT+NAME";		// size:7
static const char at_str_addr[]			= "AT+ADDR";		// size:7
static const char at_str_servuuid[]		= "AT+SERVUUID";	// size:11
static const char at_str_txuuid[]		= "AT+TXUUID";		// size:9
static const char at_str_rxuuid[]		= "AT+RXUUID";		// size:9
static const char at_str_advinv[]		= "AT+ADVINV";		// size:9
static const char at_str_txpwr[]		= "AT+TXPWR";		// size:8
static const char at_str_consta[]		= "AT+CONSTA";		// size:9


static const char at_str_test[]      	= "AT+TEST";		// size:7
static const char at_str_serv[]			= "AT+SERV";		// size:7
static const char at_str_notify[]		= "AT+NOTIFY";		// size:9
static const char at_str_write[]		= "AT+WRITE";		// size:8

//工厂测试指令列表
const char at_str_fct[]					= "AT+FCT";			// size:6
const char at_str_pinw[]				= "AT+PINW";		// size:7
const char at_str_pinr[]				= "AT+PINR";		// size:7

//Response指令列表
const char at_str_ok[]					= "\r\nOK\r\n";
const char at_str_error[]				= "\r\nERROR\r\n";

/*--------------------------------------------------------------------------------------------------------------*/

const str_info_t at_cmd_str_table[] =
{
	//通用指令列表
	INPUT_STR_INFO(CMD_ID_VER,at_str_ver),
	INPUT_STR_INFO(CMD_ID_FCTRST,at_str_fctrst),
	INPUT_STR_INFO(CMD_ID_RESET,at_str_reset),
	INPUT_STR_INFO(CMD_ID_BAUD,at_str_baud),
	INPUT_STR_INFO(CMD_ID_NAME,at_str_name),
	INPUT_STR_INFO(CMD_ID_ADDR,at_str_addr),
	INPUT_STR_INFO(CMD_ID_SERVUUID,at_str_servuuid),
	INPUT_STR_INFO(CMD_ID_TXUUID,at_str_txuuid),
	INPUT_STR_INFO(CMD_ID_RXUUID,at_str_rxuuid),
	INPUT_STR_INFO(CMD_ID_ADVINV,at_str_advinv),
	INPUT_STR_INFO(CMD_ID_TXPWR,at_str_txpwr),
	INPUT_STR_INFO(CMD_ID_CONSTA,at_str_consta),


	INPUT_STR_INFO(CMD_ID_TEST,at_str_test),
	INPUT_STR_INFO(CMD_ID_SERV,at_str_serv),
	INPUT_STR_INFO(CMD_ID_NOTIFY,at_str_notify),
	INPUT_STR_INFO(CMD_ID_WRITE,at_str_write),

	//工厂测试指令列表
	INPUT_STR_INFO(CMD_ID_FCT,at_str_fct),
	INPUT_STR_INFO(CMD_ID_FCT_PINW,at_str_pinw),
	INPUT_STR_INFO(CMD_ID_FCT_PINR,at_str_pinr),
};

/*--------------------------------------------------------------------------------------------------------------*/

static u8 get_vaild_at_cmd_len(u8 *buf,u16 buf_size)
{
	u8 vaild_len = 0,i;

	for(i = 0;i<buf_size;i++)
	{
		 if(buf[i] == '?'){
			vaild_len = i;
			break;
		 }
		 else if(buf[i] == '='){
		 	vaild_len = i;
			break;
		 }
		 else if((buf[i] == '\r')&&(buf[i+1] == '\n')){
		 	vaild_len = i;
			break;
		 }
	}

	return vaild_len;
}

static u8 get_vaild_para_len(u8 *buf,u16 buf_size)
{
	u8 vaild_len = 0,i;

	//如果是<设置命令>或者<查询命令> ,则跳过'='/'?',计算参数长度
	if((buf[0] == '=') ||(buf[0] == '?')){
		buf++;
	}

	for(i=0;i<buf_size;i++)
	{
		 if((buf[i] == '\r')&&(buf[i+1] == '\n')){
		 	vaild_len = i;
			break;
		 }
	}

	return vaild_len;
}

/*--------------------------------------------------------------------------------------------------------------*/

static u16 at_cmd_timeout_process_timer_id = NULL;

static void at_cmd_timeout_process_timer_stop(void)
{
	if( at_cmd_timeout_process_timer_id ){
		sys_s_hi_timer_del(at_cmd_timeout_process_timer_id);
		at_cmd_timeout_process_timer_id = NULL;
	}
}

static void at_cmd_timeout_process_timer_callback(void)
{
	if(++trans_data_parse_timeout_cnt >= 10){
		trans_data_parse_cnt = 0;
		trans_data_parse_timeout_cnt = 0;
		trans_data_parse_step = FIND_CMD_HEAD;
		//printf("Find At_cmd timeout!\n");
		at_cmd_timeout_process_timer_stop();
	}
}

static void at_cmd_timeout_process_timer_start(void)
{
	/* Clear 指令超时的timeout_cnt */
	trans_data_parse_timeout_cnt = 0;

	if( at_cmd_timeout_process_timer_id == NULL ){
		at_cmd_timeout_process_timer_id = sys_s_hi_timer_add(NULL, at_cmd_timeout_process_timer_callback, 100);
	}
}

/*--------------------------------------------------------------------------------------------------------------*/

static str_info_t *at_check_match_string(u8 *packet, u16 size, char *str_table, int table_size)
{
    int max_count = table_size / sizeof(str_info_t);
    str_info_t *str_p = str_table;
    u16 compara_len = 0;

	/* 通过指定的分隔符，获取AT字符串指令长度 */
	compara_len = get_vaild_at_cmd_len(packet,size);

	printf("Get AT_CMD's compara_len %d\n",compara_len);

	if(compara_len == 0) return NULL;

    while (max_count--) {
        if (str_p->str_len <= size) {
            if (str_p->str_len == compara_len) {
                if (0 == memcmp(packet, str_p->str, str_p->str_len)) {
                    return str_p;
                }
            }
        }
        //printf("# str_id = %d,str_len = %d #\n",str_p->str_id,str_p->str_len);
        str_p++;
    }

    return NULL;
}

static u8 at_get_cmd_para_packet(u8 *parse_buff,u8 parse_size,parse_packet *packet)
{
	str_info_t *str_p;

	//printf(" --> %s\n",__func__);

	packet->cmd_type = CMD_TYPE_NULL;
	packet->buff_size = 0;

	//匹配AT指令,获取对应的CMD_ID与AT指令字符串
	str_p = at_check_match_string(parse_buff, parse_size, at_cmd_str_table, sizeof(at_cmd_str_table));
	if (!str_p) {
		printf("Not match string!!\n");
		return CMD_ID_NO_MATCH;
	}

	//跳过匹配的指令头
	parse_buff += str_p->str_len;
	parse_size -= str_p->str_len;

	//数据域以'='开始
	packet->buff_size = get_vaild_para_len(parse_buff,parse_size);

	if(parse_buff[0] == '?'){
		packet->cmd_type = CMD_TYPE_INQUIRE;
		if(packet->buff_size > 0){
			parse_buff++;//跳过'?'
		}
	}
	else if(parse_buff[0] == '='){
		packet->cmd_type = CMD_TYPE_SET;
		if(packet->buff_size > 0){
			parse_buff++;//跳过'=',输出解析的数据
			memcpy(packet->buff,parse_buff,packet->buff_size);
		}
	}
	else if((parse_buff[0] == '\r')&&(parse_buff[1] == '\n')){
		packet->cmd_type = CMD_TYPE_EXECUTE;
	}

	printf("packet->buff_size[%d]:",packet->buff_size);
	if(packet->buff_size > 0){
		printf_buf(parse_buff,packet->buff_size);
	}

	printf("Find AT_CMD -> cmd_id[%d],type[%d]",str_p->str_id,packet->cmd_type);

	return str_p->str_id;
}

/*--------------------------------------------------------------------------------------------------------------*/

u8 Find_at_cmd_process(u8 *recv_buf,u16 recv_buf_size,parse_packet *packet)
{
	u16 i;

	u8 trans_byte,cmd_id;

	cmd_id = CMD_ID_NULL;

	//printf("-->%s[%d]",__func__,recv_buf_size);
	//printf_buf(recv_buf,recv_buf_size);

	at_cmd_timeout_process_timer_start();

	for(i=0; i<recv_buf_size; i++)
	{
		trans_byte = *recv_buf++;
        switch (trans_data_parse_step)
		{
			case FIND_CMD_HEAD:
			    if( trans_data_parse_cnt == 0 ){
                    if( trans_byte == 'A' ){
                        trans_cmd_buf[trans_data_parse_cnt++] = trans_byte;
                        break;
                    }
                }else if( trans_data_parse_cnt == 1 ){
                    if( trans_byte == 'T' ){
                        trans_cmd_buf[trans_data_parse_cnt++] = trans_byte;
                        break;
                    }
                }else if( trans_data_parse_cnt == 2 ){
                    if( trans_byte == '+' ){
                        trans_cmd_buf[trans_data_parse_cnt++] = trans_byte;
                        trans_data_parse_step = FIND_CMD_END1;
                        break;
                    }
                }
                trans_data_parse_cnt = 0;
                trans_data_parse_step = FIND_CMD_HEAD;
				break;

			case FIND_CMD_END1:
				/* 超出最大指令BUFF,重新查找 */
			    if( trans_data_parse_cnt > PARSE_PACKET_SIZE){
                    trans_data_parse_cnt = 0;
                    trans_data_parse_step = FIND_CMD_HEAD;
                    break;
			    }
				trans_cmd_buf[trans_data_parse_cnt++] = trans_byte;
				if( trans_byte == '\r'){
					trans_data_parse_step = FIND_CMD_END2;
				}
				break;

			case FIND_CMD_END2:
				/* 超出最大指令BUFF,重新查找 */
			    if( trans_data_parse_cnt > PARSE_PACKET_SIZE){
                    trans_data_parse_cnt = 0;
                    trans_data_parse_step = FIND_CMD_HEAD;
                    break;
			    }
			    trans_cmd_buf[trans_data_parse_cnt++] = trans_byte;
				if( trans_byte == '\n'){
					trans_cmd_buf[trans_data_parse_cnt] = 0;

					printf("--> Find AT_CMD[%d] :",trans_data_parse_cnt);
					printf_buf(trans_cmd_buf,trans_data_parse_cnt);

					cmd_id = at_get_cmd_para_packet(trans_cmd_buf,trans_data_parse_cnt,packet);

					trans_data_parse_cnt = 0;
                    trans_data_parse_step = FIND_CMD_HEAD;
				}
				break;

			default:
				trans_data_parse_cnt = 0;
				trans_data_parse_step = FIND_CMD_HEAD;
				break;
		}
	}

	return cmd_id;
}


/*--------------------------------------------------------------------------------------------------------------*/

extern char ble_device_name[BT_NAME_LEN_MAX];
extern u8 ble_device_name_len;

extern const char BLE_Module_Software_Revision_String[];
extern u8 uart_baud_index;
extern u8 user_ble_mac_addr[6];
extern void trans_disconnect(void);
extern u8 ble_adv_interval_index;

extern u16 user_services_uuid;
extern u16 user_tx_uuid;
extern u16 user_rx_uuid;

extern u8 ble_tx_power;
extern u8 Restore_FCT_Settings;

extern void bt_max_pwr_set(u8 pwr, u8 pg_pwr, u8 iq_pwr, u8 ble_pwr);
extern void ble_set_fix_pwr(u8 fix);

extern circular_buffer_t ble2uart_cbuf_handle;

static u8 chip_reset_flag = 0;

static void General_at_cmd_response(u8 *buff,u8 buff_size)
{
	circular_buffer_write(&ble2uart_cbuf_handle, (u8*)buff, buff_size);
}


u8 General_at_cmd_process(u8 cmd_id,parse_packet *packet)
{
    u8 last_cmd_id = CMD_ID_NULL;
	int ret;
	u8 temp_para[33],i;

	u8 *cmd_id_str = NULL;
	u8 cmd_id_len;
	u8 temp_str_len;
	u8 response_buff[64];
	u8 response_error;

	response_error = 0xFF;
	cmd_id_len = 0;

	memset(response_buff,0x00,sizeof(response_buff));

	if(cmd_id != CMD_ID_NO_MATCH){
		cmd_id_str = at_cmd_str_table[cmd_id-1].str+2;
		cmd_id_len = at_cmd_str_table[cmd_id-1].str_len-2;
	}

    switch(cmd_id)
    {
		/* 查询版本号 */
        case CMD_ID_VER:
			if(packet->cmd_type == CMD_TYPE_INQUIRE)
			{
				if(packet->buff_size == 0){
					/*添加对应指令字段*/
					memcpy(response_buff,cmd_id_str,cmd_id_len);
					response_buff[cmd_id_len] = '=';
					cmd_id_len += 1;

					/*添加返回信息字段*/
					temp_str_len = strlen(BLE_Module_Software_Revision_String);
					memcpy(response_buff+cmd_id_len,BLE_Module_Software_Revision_String,temp_str_len);
					cmd_id_len += temp_str_len;

					/*添加返回结果*/
					temp_str_len = strlen(at_str_ok);
					memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
					cmd_id_len += temp_str_len;

					response_error = 0; /* 返回AT指令执行结果 */
				}
			}
            break;

		/* 恢复出厂设置 */
        case CMD_ID_FCTRST:
			if(packet->cmd_type == CMD_TYPE_EXECUTE){
				if(packet->buff_size == 0){

					/*执行命令对应操作*/
					if(Restore_FCT_Settings == 0){
						Restore_FCT_Settings = 1;
						ret = syscfg_write(CFG_RESTORE_FCT_SETTINGS, &Restore_FCT_Settings, 1);
						chip_reset_flag = 1;
					}

					/*添加返回结果*/
					cmd_id_len = strlen(at_str_ok);
					memcpy(response_buff,at_str_ok,cmd_id_len);

					response_error = 0;/* 返回AT指令执行结果 */
				}
			}
            break;

		/* 系统软复位 */
        case CMD_ID_RESET:
			if(packet->cmd_type == CMD_TYPE_EXECUTE){
				if(packet->buff_size == 0){

					/*执行命令对应操作*/
					chip_reset_flag = 1;

					/*添加返回结果*/
					cmd_id_len = strlen(at_str_ok);
					memcpy(response_buff,at_str_ok,cmd_id_len);

					response_error = 0;/* 返回AT指令执行结果 */
				}
			}
            break;

		/* 查询串口波特率/设置串口波特率 */
		case CMD_ID_BAUD:
			if(packet->cmd_type == CMD_TYPE_INQUIRE){
				if(packet->buff_size == 0){
					/*添加对应指令字段*/
					memcpy(response_buff,cmd_id_str,cmd_id_len);
					response_buff[cmd_id_len] = '=';
					cmd_id_len += 1;

					/*添加返回信息字段*/
					response_buff[cmd_id_len] = hex2ascii(uart_baud_index);
					cmd_id_len += 1;

					/*添加返回结果*/
					temp_str_len = strlen(at_str_ok);
					memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
					cmd_id_len += temp_str_len;

					response_error = 0; /* 返回AT指令执行结果 */
				}
			}
			else if(packet->cmd_type == CMD_TYPE_SET){
				if((packet->buff_size == 1))
				{
					if((packet->buff[0] >='0')&&(packet->buff[0] <='8'))
					{
						/*执行命令对应操作*/
						temp_para[0] = ascii2hex(packet->buff[0]);

						ret = syscfg_write(CFG_USER_UART_BAUD,temp_para, 1);
						if(ret == 1)
						{
							memcpy(response_buff,cmd_id_str,cmd_id_len);
							response_buff[cmd_id_len] = '=';
							cmd_id_len += 1;

							/*添加返回信息字段*/
							response_buff[cmd_id_len] = packet->buff[0];
							cmd_id_len += 1;

							/*添加返回结果*/
							temp_str_len = strlen(at_str_ok);
							memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
							cmd_id_len += temp_str_len;

							response_error = 0; /* 返回AT指令执行结果 */
						}
					}
				}
			}
            break;

		/* 查询设备名/设置设备名 */
		case CMD_ID_NAME:
			if(packet->cmd_type == CMD_TYPE_INQUIRE)
			{
				if(packet->buff_size == 0)
				{
					/*添加对应指令字段*/
					memcpy(response_buff,cmd_id_str,cmd_id_len);
					response_buff[cmd_id_len] = '=';
					cmd_id_len += 1;

					/*添加返回信息字段*/
					temp_str_len = ble_device_name_len;//strlen(ble_device_name);
					memcpy(response_buff+cmd_id_len,ble_device_name,temp_str_len);
					cmd_id_len += temp_str_len;

					/*添加返回结果*/
					temp_str_len = strlen(at_str_ok);
					memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
					cmd_id_len += temp_str_len;

					response_error = 0;/* 返回AT指令执行结果 */
				}
			}
			else if(packet->cmd_type == CMD_TYPE_SET)
			{
				if((packet->buff_size > 0)&&(packet->buff_size <= (BT_NAME_LEN_MAX-1)))
				{
					/*执行命令对应操作*/

					memset(temp_para,0x00,BT_NAME_LEN_MAX);
					memcpy(temp_para,packet->buff,packet->buff_size);

					ret = syscfg_write(CFG_BT_NAME, temp_para, BT_NAME_LEN_MAX);

					if(ret == BT_NAME_LEN_MAX){
						/*添加对应指令字段*/
						memcpy(response_buff,cmd_id_str,cmd_id_len);
						response_buff[cmd_id_len] = '=';
						cmd_id_len += 1;

						/*添加返回信息字段*/
						memcpy(response_buff+cmd_id_len,packet->buff,packet->buff_size);
						cmd_id_len += packet->buff_size;

						/*添加返回结果*/
						temp_str_len = strlen(at_str_ok);
						memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
						cmd_id_len += temp_str_len;

						response_error = 0;/* 返回AT指令执行结果 */
					}
				}
			}
            break;

		/* 查询蓝牙地址/设置蓝牙地址 */
        case CMD_ID_ADDR:
			if(packet->cmd_type == CMD_TYPE_INQUIRE){
				if(packet->buff_size == 0){
					/*添加对应指令字段*/
					memcpy(response_buff,cmd_id_str,cmd_id_len);
					response_buff[cmd_id_len] = '=';
					cmd_id_len += 1;

					for(i = 0;i<6;i++){
						temp_para[i*2] = hex2ascii((user_ble_mac_addr[5-i]&0xF0) >> 4);
						temp_para[i*2+1] = hex2ascii(user_ble_mac_addr[5-i]&0x0F);
					}

					/*添加返回信息字段*/
					temp_str_len = 12;
					memcpy(response_buff+cmd_id_len,temp_para,temp_str_len);
					cmd_id_len += temp_str_len;

					/*添加返回结果*/
					temp_str_len = strlen(at_str_ok);
					memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
					cmd_id_len += temp_str_len;

					response_error = 0;/* 返回AT指令执行结果 */
				}
			}
			else if(packet->cmd_type == CMD_TYPE_SET){
				if(packet->buff_size == 12){

					/*检查输入参数是否非法*/
					if(check_ascii_input_is_illegal(packet->buff,12)) break;

					/*执行命令对应操作*/
					for(i=0;i<6;i++)
					{
						temp_para[5-i] = (ascii2hex(packet->buff[i*2])& 0x0F)<<4;
						temp_para[5-i] |= (ascii2hex(packet->buff[i*2+1])& 0x0F);
					}

					ret = syscfg_write(CFG_USER_BLE_MAC_ADDR,(void *)temp_para, 6);
					if(ret == 6)
					{
						/*添加对应指令字段*/
						memcpy(response_buff,cmd_id_str,cmd_id_len);
						response_buff[cmd_id_len] = '=';
						cmd_id_len += 1;

						/*添加返回信息字段*/
						temp_str_len = 12;
						memcpy(response_buff+cmd_id_len,packet->buff,temp_str_len);
						cmd_id_len += temp_str_len;

						/*添加返回结果*/
						temp_str_len = strlen(at_str_ok);
						memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
						cmd_id_len += temp_str_len;

						response_error = 0;/* 返回AT指令执行结果 */
					}
				}
			}
            break;

		/* 查询透传服务UUID/设置透传服务UUID */
		case CMD_ID_SERVUUID:
		case CMD_ID_SERV:
			if(packet->cmd_type == CMD_TYPE_INQUIRE){
				if(packet->buff_size == 0){
					/*添加对应指令字段*/
					memcpy(response_buff,cmd_id_str,cmd_id_len);
					response_buff[cmd_id_len] = '=';
					cmd_id_len += 1;

					temp_para[0] = hex2ascii((user_services_uuid&0xF000) >> 12);
					temp_para[1] = hex2ascii((user_services_uuid&0x0F00) >> 8);
					temp_para[2] = hex2ascii((user_services_uuid&0x00F0) >> 4);
					temp_para[3] = hex2ascii((user_services_uuid&0x000F));

					/*添加返回信息字段*/
					temp_str_len = 4;
					memcpy(response_buff+cmd_id_len,temp_para,temp_str_len);
					cmd_id_len += temp_str_len;

					/*添加返回结果*/
					temp_str_len = strlen(at_str_ok);
					memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
					cmd_id_len += temp_str_len;

					response_error = 0;/* 返回AT指令执行结果 */
				}
			}
			else if(packet->cmd_type == CMD_TYPE_SET){
				if(packet->buff_size == 4){
					/*检查输入参数是否非法*/
					if(check_ascii_input_is_illegal(packet->buff,4)) break;

					temp_para[1]  = (ascii2hex(packet->buff[0])& 0x0F) << 4;
					temp_para[1] |= (ascii2hex(packet->buff[1])& 0x0F);
					temp_para[0]  = (ascii2hex(packet->buff[2])& 0x0F) << 4;
					temp_para[0] |= (ascii2hex(packet->buff[3])& 0x0F);


					ret = syscfg_write(CFG_USER_SERV_UUID,temp_para, 2);
					if(ret == 2)
					{
						/*添加对应指令字段*/
						memcpy(response_buff,cmd_id_str,cmd_id_len);
						response_buff[cmd_id_len] = '=';
						cmd_id_len += 1;

						/*添加返回信息字段*/
						temp_str_len = 4;
						memcpy(response_buff+cmd_id_len,packet->buff,temp_str_len);
						cmd_id_len += temp_str_len;

						/*添加返回结果*/
						temp_str_len = strlen(at_str_ok);
						memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
						cmd_id_len += temp_str_len;

						response_error = 0;/* 返回AT指令执行结果 */
					}
				}
			}
			break;

		/* 查询透传NOTIFY UUID/设置透传NOTIFY UUID */
		case CMD_ID_TXUUID:
		case CMD_ID_NOTIFY:
			if(packet->cmd_type == CMD_TYPE_INQUIRE){
				if(packet->buff_size == 0){
					/*添加对应指令字段*/
					memcpy(response_buff,cmd_id_str,cmd_id_len);
					response_buff[cmd_id_len] = '=';
					cmd_id_len += 1;

					temp_para[0] = hex2ascii((user_tx_uuid&0xF000) >> 12);
					temp_para[1] = hex2ascii((user_tx_uuid&0x0F00) >> 8);
					temp_para[2] = hex2ascii((user_tx_uuid&0x00F0) >> 4);
					temp_para[3] = hex2ascii((user_tx_uuid&0x000F));

					/*添加返回信息字段*/
					temp_str_len = 4;
					memcpy(response_buff+cmd_id_len,temp_para,temp_str_len);
					cmd_id_len += temp_str_len;

					/*添加返回结果*/
					temp_str_len = strlen(at_str_ok);
					memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
					cmd_id_len += temp_str_len;

					response_error = 0;/* 返回AT指令执行结果 */
				}
			}
			else if(packet->cmd_type == CMD_TYPE_SET){
				if(packet->buff_size == 4){

					/*检查输入参数是否非法*/
					if(check_ascii_input_is_illegal(packet->buff,4)) break;

					temp_para[1]  = (ascii2hex(packet->buff[0])& 0x0F) << 4;
					temp_para[1] |= (ascii2hex(packet->buff[1])& 0x0F);
					temp_para[0]  = (ascii2hex(packet->buff[2])& 0x0F) << 4;
					temp_para[0] |= (ascii2hex(packet->buff[3])& 0x0F);

					ret = syscfg_write(CFG_USER_TX_UUID,temp_para, 2);
					if(ret == 2)
					{
						/*添加对应指令字段*/
						memcpy(response_buff,cmd_id_str,cmd_id_len);
						response_buff[cmd_id_len] = '=';
						cmd_id_len += 1;

						/*添加返回信息字段*/
						temp_str_len = 4;
						memcpy(response_buff+cmd_id_len,packet->buff,temp_str_len);
						cmd_id_len += temp_str_len;

						/*添加返回结果*/
						temp_str_len = strlen(at_str_ok);
						memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
						cmd_id_len += temp_str_len;

						response_error = 0;/* 返回AT指令执行结果 */
					}
				}
			}
			break;

		/* 查询透传WRITE UUID/设置透传WRITE UUID */
		case CMD_ID_RXUUID:
		case CMD_ID_WRITE:
			if(packet->cmd_type == CMD_TYPE_INQUIRE){
				if(packet->buff_size == 0){
					/*添加对应指令字段*/
					memcpy(response_buff,cmd_id_str,cmd_id_len);
					response_buff[cmd_id_len] = '=';
					cmd_id_len += 1;

					temp_para[0] = hex2ascii((user_rx_uuid&0xF000) >> 12);
					temp_para[1] = hex2ascii((user_rx_uuid&0x0F00) >> 8);
					temp_para[2] = hex2ascii((user_rx_uuid&0x00F0) >> 4);
					temp_para[3] = hex2ascii((user_rx_uuid&0x000F));

					/*添加返回信息字段*/
					temp_str_len = 4;
					memcpy(response_buff+cmd_id_len,temp_para,temp_str_len);
					cmd_id_len += temp_str_len;

					/*添加返回结果*/
					temp_str_len = strlen(at_str_ok);
					memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
					cmd_id_len += temp_str_len;

					response_error = 0;/* 返回AT指令执行结果 */
				}
			}
			else if(packet->cmd_type == CMD_TYPE_SET){
				if(packet->buff_size == 4){

					/*检查输入参数是否非法*/
					if(check_ascii_input_is_illegal(packet->buff,4)) break;

					temp_para[1]  = (ascii2hex(packet->buff[0])& 0x0F) << 4;
					temp_para[1] |= (ascii2hex(packet->buff[1])& 0x0F);
					temp_para[0]  = (ascii2hex(packet->buff[2])& 0x0F) << 4;
					temp_para[0] |= (ascii2hex(packet->buff[3])& 0x0F);

					ret = syscfg_write(CFG_USER_RX_UUID,temp_para, 2);
					if(ret == 2)
					{
						/*添加对应指令字段*/
						memcpy(response_buff,cmd_id_str,cmd_id_len);
						response_buff[cmd_id_len] = '=';
						cmd_id_len += 1;

						/*添加返回信息字段*/
						temp_str_len = 4;
						memcpy(response_buff+cmd_id_len,packet->buff,temp_str_len);
						cmd_id_len += temp_str_len;

						/*添加返回结果*/
						temp_str_len = strlen(at_str_ok);
						memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
						cmd_id_len += temp_str_len;

						response_error = 0;/* 返回AT指令执行结果 */
					}
				}
			}
			break;

		/* 查询广播间隔/设置广播间隔 */
        case CMD_ID_ADVINV:
			if(packet->cmd_type == CMD_TYPE_INQUIRE){
				if(packet->buff_size == 0){
					/*添加对应指令字段*/
					memcpy(response_buff,cmd_id_str,cmd_id_len);
					response_buff[cmd_id_len] = '=';
					cmd_id_len += 1;

					/*添加返回信息字段*/
					response_buff[cmd_id_len] = hex2ascii(ble_adv_interval_index);
					cmd_id_len += 1;

					/*添加返回结果*/
					temp_str_len = strlen(at_str_ok);
					memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
					cmd_id_len += temp_str_len;

					response_error = 0; /* 返回AT指令执行结果 */
				}
			}
			else if(packet->cmd_type == CMD_TYPE_SET){
				if(packet->buff_size == 1)
				{
					/*检查输入参数是否非法*/
					if(check_ascii_input_is_illegal(packet->buff,1)) break;

					/*执行命令对应操作*/
					ble_gatt_server_adv_enable(0);
					ble_adv_interval_index = ascii2hex(packet->buff[0]);
					ble_gatt_server_adv_enable(1);

					ret = syscfg_write(CFG_USER_ADVINV,&ble_adv_interval_index, 1);
					if(ret == 1)
					{
						/*添加对应指令字段*/
						memcpy(response_buff,cmd_id_str,cmd_id_len);
						response_buff[cmd_id_len] = '=';
						cmd_id_len += 1;

						/*添加返回信息字段*/
						response_buff[cmd_id_len] = packet->buff[0];
						cmd_id_len += 1;

						/*添加返回结果*/
						temp_str_len = strlen(at_str_ok);
						memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
						cmd_id_len += temp_str_len;

						response_error = 0; /* 返回AT指令执行结果 */
					}
				}
			}
            break;


		/* 查询发射功率/设置发射功率 */
		case CMD_ID_TXPWR:
			if(packet->cmd_type == CMD_TYPE_INQUIRE){
				if(packet->buff_size == 0){
					/*添加对应指令字段*/
					memcpy(response_buff,cmd_id_str,cmd_id_len);
					response_buff[cmd_id_len] = '=';
					cmd_id_len += 1;

					/*添加返回信息字段*/
					response_buff[cmd_id_len] = hex2ascii(ble_tx_power);
					cmd_id_len += 1;

					/*添加返回结果*/
					temp_str_len = strlen(at_str_ok);
					memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
					cmd_id_len += temp_str_len;

					response_error = 0; /* 返回AT指令执行结果 */
				}
			}
			else if(packet->cmd_type == CMD_TYPE_SET){
				if(packet->buff_size == 1)
				{
					if((packet->buff[0] >= '0')&&(packet->buff[0] <= '9'))
					{
						/*执行命令对应操作*/
						ble_tx_power = ascii2hex(packet->buff[0]);
						ble_set_fix_pwr(ble_tx_power);

						ret = syscfg_write(CFG_USER_BLE_TXPWR,&ble_tx_power, 1);
						if(ret == 1)
						{
							memcpy(response_buff,cmd_id_str,cmd_id_len);
							response_buff[cmd_id_len] = '=';
							cmd_id_len += 1;

							/*添加返回信息字段*/
							response_buff[cmd_id_len] = packet->buff[0];
							cmd_id_len += 1;

							/*添加返回结果*/
							temp_str_len = strlen(at_str_ok);
							memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
							cmd_id_len += temp_str_len;

							response_error = 0; /* 返回AT指令执行结果 */
						}
					}
				}
			}
			break;

		/* 查询BLE连接状态/断开BLE连接 */
        case CMD_ID_CONSTA:
			if(packet->cmd_type == CMD_TYPE_INQUIRE){
				if(packet->buff_size == 0)
				{
					if(bt_ble_is_connected()){
						temp_para[0] = '1';
					}
					else{
						temp_para[0] = '0';
					}

					/*添加对应指令字段*/
					memcpy(response_buff,cmd_id_str,cmd_id_len);
					response_buff[cmd_id_len] = '=';
					cmd_id_len += 1;

					/*添加返回信息字段*/
					response_buff[cmd_id_len] = temp_para[0];
					cmd_id_len += 1;

					/*添加返回结果*/
					temp_str_len = strlen(at_str_ok);
					memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
					cmd_id_len += temp_str_len;

					response_error = 0; /* 返回AT指令执行结果 */
				}
			}
			else if(packet->cmd_type == CMD_TYPE_SET){
				if((packet->buff_size == 1)&&(packet->buff[0] == '0')){

					/*执行命令对应操作*/
					if(bt_ble_is_connected()){
						trans_disconnect();
						os_time_dly(3);
					}

					/*添加对应指令字段*/
					memcpy(response_buff,cmd_id_str,cmd_id_len);
					response_buff[cmd_id_len] = '=';
					cmd_id_len += 1;

					/*添加返回信息字段*/
					response_buff[cmd_id_len] = '0';
					cmd_id_len += 1;

					/*添加返回结果*/
					temp_str_len = strlen(at_str_ok);
					memcpy(response_buff+cmd_id_len,at_str_ok,strlen(at_str_ok));
					cmd_id_len += temp_str_len;

					response_error = 0; /* 返回AT指令执行结果 */
				}
			}
            break;

		case CMD_ID_TEST:
			if(packet->cmd_type == CMD_TYPE_EXECUTE){
				if(packet->buff_size == 0){
					/*添加返回结果*/
					cmd_id_len = strlen(at_str_ok);
					memcpy(response_buff,at_str_ok,cmd_id_len);

					response_error = 0; /* 返回AT指令执行结果 */
				}
			}
			break;

		case CMD_ID_NO_MATCH:
			last_cmd_id = CMD_ID_NULL;
			break;

        default:
			printf("^_^ Running here [%d]\n",cmd_id);
			last_cmd_id = cmd_id;
			cmd_id_str = NULL;
			cmd_id_len = 0;
			response_error = 0;
            break;
    }

	if(response_error != 0){
		cmd_id_len = strlen(at_str_error);
		memcpy(response_buff,at_str_error,cmd_id_len);
	}

	if(cmd_id_len > 0)
	{
		printf("General_CMD_ID --> Response[%d]",cmd_id_len);
		printf("AT_Response:\n%s",response_buff);
		/* printf_buf(response_buff,cmd_id_len); */

		General_at_cmd_response(response_buff,cmd_id_len);

		if(chip_reset_flag){
			chip_reset_flag = 0;
			os_time_dly(3);
			cpu_reset();
		}
	}

    return last_cmd_id;
}

/*--------------------------------------------------------------------------------------------------------------*/
extern void lib_make_ble_address(u8 *ble_address, u8 *edr_address);
extern void bt_set_local_name(char *name, u8 len);
extern const u8 *bt_get_mac_addr(void);

void Restore_FCT_CFG(void)
{
	u8 temp[33] = {0};
    int ret = 0;

	ret = syscfg_read(CFG_RESTORE_FCT_SETTINGS, temp, 1);

	if(ret == 1){
		Restore_FCT_Settings = temp[0];
		printf("\n# Restore_FCT_Settings [%d]\n",Restore_FCT_Settings);
	}
	else{
		printf("\n# Fisrt Power Settings!\n");
		Restore_FCT_Settings = 1;
	}

	/*--------------------------------------------------------------------------------------*/

	if(Restore_FCT_Settings)
	{
		/* Clear Restore_FCT_Settings Flag */
		Restore_FCT_Settings = 0;
		char temp_device_name[BT_NAME_LEN_MAX] = "POL830";

		memset(ble_device_name,0x00,BT_NAME_LEN_MAX);

		ble_device_name_len = strlen(temp_device_name);

		memcpy(ble_device_name,temp_device_name,ble_device_name_len);
		bt_set_local_name(ble_device_name,ble_device_name_len);

		ret = syscfg_write(CFG_BT_NAME, ble_device_name, ble_device_name_len);
		if(ret == ble_device_name_len){
			printf("--> Restore_FCT_Settings :[ble_name] %s\n",ble_device_name);
		}

		memcpy(user_ble_mac_addr,(void *)bt_get_mac_addr(),6);
		memcpy(temp,user_ble_mac_addr,6);

		ret = syscfg_write(CFG_USER_BLE_MAC_ADDR, temp, 6);
		if(ret == 6){
			printf("#1  Restore_FCT_Settings :[ble_mac_addr],OK!\n");
		}

		/*----------------------------------------------------------------------------------*/

		uart_baud_index = UART_BAUD_INDEX_DEFAULT;
		temp[0] = uart_baud_index;
		ret = syscfg_write(CFG_USER_UART_BAUD, temp, 1);
		if(ret == 1){
			printf("#2  Restore_FCT_Settings :[uart_baud],OK!\n");
		}

		/*----------------------------------------------------------------------------------*/

		user_services_uuid = CONFIG_USER_SERVICE_UUID;
		temp[0] = (u8)(user_services_uuid & 0x00FF);
		temp[1] = (u8)((user_services_uuid & 0xFF00)>>8);
		ret = syscfg_write(CFG_USER_SERV_UUID, temp, 2);
		if(ret == 2){
			printf("#3  Restore_FCT_Settings :[services_uuid],OK!\n");
		}

		/*----------------------------------------------------------------------------------*/

		user_tx_uuid = CONFIG_USER_TX_UUID;
		temp[0] = (u8)(user_tx_uuid & 0x00FF);
		temp[1] = (u8)((user_tx_uuid & 0xFF00)>>8);
		ret = syscfg_write(CFG_USER_TX_UUID, temp, 2);
		if(ret == 2){
			printf("#4 Restore_FCT_Settings :[notify_uuid],OK!\n");
		}

		/*----------------------------------------------------------------------------------*/

		user_rx_uuid = CONFIG_USER_RX_UUID;
		temp[0] = (u8)(user_rx_uuid & 0x00FF);
		temp[1] = (u8)((user_rx_uuid & 0xFF00)>>8);
		ret = syscfg_write(CFG_USER_RX_UUID, temp, 2);
		if(ret == 2){
			printf("#5 Restore_FCT_Settings :[write_uuid],OK!\n");
		}

		/*----------------------------------------------------------------------------------*/

		ble_adv_interval_index = CONFIG_BLE_ADV_INTERVAL;
		temp[0] = ble_adv_interval_index;
		ret = syscfg_write(CFG_USER_ADVINV, temp, 1);
		if(ret == 1){
			printf("#6 Restore_FCT_Settings :[adv_interval],OK!\n");
		}

		/*----------------------------------------------------------------------------------*/

		ble_tx_power = CONFIG_BLE_TX_POWER;
		temp[0] = ble_tx_power;
		ret = syscfg_write(CFG_USER_BLE_TXPWR, temp, 1);
		if(ret == 1){
			printf("#7 Restore_FCT_Settings :[ble_tx_power],OK!\n");
		}

		/*----------------------------------------------------------------------------------*/

		temp[0] = Restore_FCT_Settings;
		ret = syscfg_write(CFG_RESTORE_FCT_SETTINGS,temp, 1);
		if(ret == 1){
			printf("--> Restore_FCT_Settings : Succeed!\n\n");
		}

		/*----------------------------------------------------------------------------------*/
	}
	else{
		printf("--> Use User Settings!!\n");

		/*----------------------------------------------------------------------------------*/

		ret = syscfg_read(CFG_USER_BLE_MAC_ADDR, temp, 6);
		if(ret == 6){
			memcpy(user_ble_mac_addr,temp,6);
			printf("#1  User_Settings :[ble_mac_addr]");
			printf_buf(user_ble_mac_addr,6);
		}

		/*----------------------------------------------------------------------------------*/

		ret = syscfg_read(CFG_USER_UART_BAUD, temp, 1);
		if(ret == 1){
			uart_baud_index = temp[0];
			printf("#2  User_Settings :[uart_baud] = %d\n",uart_baud_index);
		}

		/*----------------------------------------------------------------------------------*/

		ret = syscfg_read(CFG_USER_SERV_UUID, temp, 2);
		if(ret == 2){
			user_services_uuid = (temp[0] & 0x00FF);
			user_services_uuid |= (temp[1] & 0x00FF) << 8;
			printf("#3  User_Settings :[services_uuid] = 0x%04X\n",user_services_uuid);
		}

		/*----------------------------------------------------------------------------------*/

		ret = syscfg_read(CFG_USER_TX_UUID, temp, 2);
		if(ret == 2){
			user_tx_uuid = (u16)(temp[0] & 0x00FF);
			user_tx_uuid |= (u16)((temp[1] & 0x00FF) << 8);
			printf("#4  User_Settings :[notify_uuid] = 0x%04X\n",user_tx_uuid);
		}

		/*----------------------------------------------------------------------------------*/

		ret = syscfg_read(CFG_USER_RX_UUID, temp, 2);
		if(ret == 2){
			user_rx_uuid = (u16)(temp[0] & 0x00FF);
			user_rx_uuid |= (u16)((temp[1] & 0x00FF) << 8);
			printf("#5  User_Settings :[write_uuid] = 0x%04X\n",user_rx_uuid);
		}

		/*----------------------------------------------------------------------------------*/

		ret = syscfg_read(CFG_USER_ADVINV, temp, 1);
		if(ret == 1){
			ble_adv_interval_index = temp[0];
			printf("#6  User_Settings :[adv_interval] = %d\n",ble_adv_interval_index);
		}

		/*----------------------------------------------------------------------------------*/

		ret = syscfg_read(CFG_USER_BLE_TXPWR, temp, 1);
		if(ret == 1){
			ble_tx_power = temp[0];
			printf("#7  User_Settings :[ble_tx_power] = %d\n",ble_tx_power);
		}

		/*----------------------------------------------------------------------------------*/

		printf("-->User_Settings Update End!!\n\n");
	}
}

/*--------------------------------------------------------------------------------------------------------------*/

