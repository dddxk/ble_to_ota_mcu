#include "polyc_cmd_process_common.h"
#include "polyc_buffer_manage.h"
#include "polyc_fct_cmd.h"
#include "le_common.h"

/*--------------------------------------------------------------------------------------------------------------*/

/*	AC632n | GPIOA:0 | GPIOB:1 | GPIOD:2 ,对应芯片gpio.h 修改以下函数 */

/**@brief    检测设置AC632n的GPIO是否有效
   @param    port	设置GPIO所属的分组
   @param    pin	设置GPIO的第几个引脚
   @return	 返回0 表示没有错误
   			 返回1 表示错误
   @note
*/
u8 check_ac632n_gpio_valid(u8 port,u8 pin)
{
	u8 ret_error = 0;

	switch(port)
	{
		/* GPIOA */
		case FCT_GPIOA_GROUP:
		/* GPIOB */
		case FCT_GPIOB_GROUP:
			if(pin > 9) ret_error = 1;
			break;

		/* GPIOD */
		case FCT_GPIOD_GROUP:
			if(pin > 4) ret_error = 1;
			break;

		/* GPIOC */
		case FCT_GPIOC_GROUP:
		/* AC6932N 没有GPIOC */
			ret_error = 1;
			break;

		/* USBDM */
		case FCT_USB_DM:
			break;
		/* USBDP */
		case FCT_USB_DP:
			break;

		/* 其他GPIO 暂不支持 */
		default:
			ret_error = 1;
			break;
	}

	return ret_error;
}


/**@brief    FCT设置GPIO,输出高低电平
   @param    port	设置GPIO所属的分组
   @param    pin	设置GPIO的第几个引脚
   @param    value	设置GPIO的输出电平
   @return   返回0  表示没有错误
   			 返回1  表示错误
   @note
*/
u8 fct_set_gpio_output(u8 port,u8 pin,u8 value)
{
	u8 ret_error = 0;
	u16 config_gpio;

	ret_error = check_ac632n_gpio_valid(port,pin);

	if(value > 1) ret_error = 1;

	if(ret_error == 0)
	{
		if(port < FCT_USB_DM){
			if(port == FCT_GPIOD_GROUP) port = 2;//根据实际芯片平台修改
			config_gpio = IO_GROUP_NUM * port + pin;
		    gpio_set_pull_down(config_gpio, 0);
		    gpio_set_pull_up(config_gpio, 0);
		    gpio_set_die(config_gpio, 1);
			gpio_direction_output(config_gpio,value);
		}
	}
	return ret_error;
}

/**@brief    FCT设置GPIO为输入
   @param    port	设置GPIO所属的分组
   @param    pin	设置GPIO的第几个引脚
   @return   返回0  表示没有错误
   			 返回1  表示错误
   @note
*/
u8 fct_set_gpio_input(u8 port,u8 pin)
{
	u8 ret_error = 0;
	u16 config_gpio;

	ret_error = check_ac632n_gpio_valid(port,pin);

	if(ret_error == 0)
	{
		if(port < FCT_USB_DM){
			if(port == FCT_GPIOD_GROUP) port = 2;//根据实际芯片平台修改
			config_gpio = IO_GROUP_NUM * port + pin;
			gpio_direction_input(config_gpio);
			gpio_set_pull_down(config_gpio, 0);
			gpio_set_pull_up(config_gpio, 0);
			gpio_set_die(config_gpio, 1);
		}
	}

	return ret_error;
}


/**@brief    FCT读取GPIO的值
   @param    port	设置GPIO所属的分组
   @param    pin	设置GPIO的第几个引脚
   @return   返回0xFF 表示配置IO错误;
   			 返回0x00 表示读取电平为低;
   			 返回0x01 表示读取电平为高;
   @note
*/
u8 fct_gpio_read(u8 port,u8 pin)
{
	u16 config_gpio;
	u8 value = 0;

	if(check_ac632n_gpio_valid(port,pin) != 0) return 0xFF;

	if(port < FCT_USB_DM) {
		if(port == FCT_GPIOD_GROUP) port = 2;//根据实际芯片平台修改
		config_gpio = IO_GROUP_NUM * port + pin;
		value = gpio_read(config_gpio);
	}

	return value;
}

/*--------------------------------------------------------------------------------------------------------------*/
extern circular_buffer_t ble2uart_cbuf_handle;

static void FCT_at_cmd_response(u8 *buff,u8 buff_size)
{
	circular_buffer_write(&ble2uart_cbuf_handle, (u8*)buff, buff_size);
}

u8 FCT_at_cmd_process(u8 cmd_id,parse_packet *packet)
{
	//进入FCT需接收指定序列指令
	static u8 fct_sequence_cnt = 0;
	static u8 backup_module_sta;
	
	u8 last_cmd_id = CMD_ID_NULL;
	u8 temp_para[5];

	u8 *FCT_cmd_id_str = NULL;
	u8 FCT_cmd_id_len;
	u8 temp_str_len;
	u8 FCT_response_buff[64];
	u8 FCT_response_error;

	u8 config_gpio;
	u8 gpio_port,gpio_pin,gpio_value,i;
	
	FCT_response_error = 0xFF;
	FCT_cmd_id_len = 0;

	memset(FCT_response_buff,0x00,sizeof(FCT_response_buff));

	if(cmd_id != CMD_ID_NULL)
	{
		//printf("--> %s[0x%02x]\n",__func__,cmd_id);

		FCT_cmd_id_str = at_cmd_str_table[cmd_id-1].str+2;
		FCT_cmd_id_len = at_cmd_str_table[cmd_id-1].str_len-2;
		
		if((ble_module_sta != MODULE_RUN_FCT_MODE)&&(cmd_id != CMD_ID_FCT)){
			printf("ble_module not run FCT Mode!\n");
			return cmd_id;
		}
	}
	else{
		return cmd_id;
	}

	switch(cmd_id)
	{
		case CMD_ID_FCT:
			if(packet->cmd_type == CMD_TYPE_INQUIRE){
				if(packet->buff_size == 0){

					/*执行命令对应操作*/
					if(ble_module_sta == MODULE_RUN_FCT_MODE){
						temp_para[0] = '1';
					}
					else{
						temp_para[0] = '0';
					}
					
					/*添加对应指令字段*/
					memcpy(FCT_response_buff,FCT_cmd_id_str,FCT_cmd_id_len);
					FCT_response_buff[FCT_cmd_id_len] = '=';
					FCT_cmd_id_len += 1;
					
					/*添加返回信息字段*/
					FCT_response_buff[FCT_cmd_id_len] = temp_para[0];
					FCT_cmd_id_len += 1;

					/*添加返回结果*/
					temp_str_len = strlen(at_str_ok);
					memcpy(FCT_response_buff+FCT_cmd_id_len,at_str_ok,strlen(at_str_ok));
					FCT_cmd_id_len += temp_str_len;

					
					FCT_response_error = 0;
				}
			}
			else if(packet->cmd_type == CMD_TYPE_SET){
				if(packet->buff_size == 1){
					if(ble_module_sta != MODULE_RUN_FCT_MODE)
					{
						/* 检查数据合法性 */
						if(check_ascii_number_input_range_is_illegal(packet->buff,1,9)) break;
						
						/* 通过指定序列进入FCT模式 */
						if((fct_sequence_cnt == 0)&&(packet->buff[0] == '8')){
							fct_sequence_cnt++;
						}
						else if((fct_sequence_cnt == 1)&&(packet->buff[0] == '3')){
							fct_sequence_cnt++;
						}
						else if((fct_sequence_cnt == 2)&&(packet->buff[0] == '0')){
							fct_sequence_cnt = 0;
							
							backup_module_sta = ble_module_sta;
							ble_module_sta = MODULE_RUN_FCT_MODE;
							printf("# Enter FCT Mode!\n");
						}
						else {
							fct_sequence_cnt = 0;
						}
					}
					else{
						fct_sequence_cnt = 0;
						
						if(packet->buff[0] == 'Q')
						{
							ble_module_sta = backup_module_sta;
							printf("# Exit FCT Mode!\n");
						}
						else if(check_ascii_number_input_range_is_illegal(packet->buff,1,9)){
							break;
						}
					}

					/*添加返回结果*/
					FCT_cmd_id_len = strlen(at_str_ok);
					memcpy(FCT_response_buff,at_str_ok,FCT_cmd_id_len);

					FCT_response_error = 0;
				}
			}
			break;
			
		case CMD_ID_FCT_PINW:
			if(packet->cmd_type == CMD_TYPE_SET){
				if(packet->buff_size == 5){ 
				
					/* 检查数据合法性 */
					if(check_ascii_number_input_range_is_illegal(packet->buff,5,9)) break;

					for(i = 0;i< 5;i++)
					{
						temp_para[i] = ascii2hex(packet->buff[i]);
					}
					gpio_port = temp_para[0]*10+temp_para[1];
					gpio_pin = temp_para[2]*10+temp_para[3];
					gpio_value = temp_para[4];

					if(gpio_port < FCT_USB_DM){
						printf("FCT_PINW --> GPIO%c_%c%c\n",gpio_port+'A',packet->buff[2],packet->buff[3]);
					}
					
					if(fct_set_gpio_output(gpio_port,gpio_pin,gpio_value)) break;

					printf("FCT_PINW --> OUTPUT:%d",gpio_value);

					/*添加返回结果*/
					FCT_cmd_id_len = strlen(at_str_ok);
					memcpy(FCT_response_buff,at_str_ok,FCT_cmd_id_len);
					
					FCT_response_error = 0;
				}
			}
			break;

		case CMD_ID_FCT_PINR:
			if(packet->cmd_type == CMD_TYPE_SET){
				if(packet->buff_size == 4){
					/* 检查数据合法性 */
					if(check_ascii_number_input_range_is_illegal(packet->buff,4,9)) break;

					for(i = 0;i< 4;i++)
					{
						temp_para[i] = ascii2hex(packet->buff[i]);
					}
					gpio_port = temp_para[0]*10+temp_para[1];
					gpio_pin = temp_para[2]*10+temp_para[3];

					if(gpio_port < FCT_USB_DM)
					{
						printf("FCT_PINR --> GPIO%c_%c%c\n",gpio_port+'A',packet->buff[2],packet->buff[3]);
					}
					
					if(fct_set_gpio_input(gpio_port,gpio_pin)) break;

					/*执行命令对应操作*/
					if(fct_gpio_read(gpio_port,gpio_pin)){
						temp_para[0] = '1';
						printf("FCT_PINR --> OUTPUT:1");
					}
					else{
						temp_para[0] = '0';
						printf("FCT_PINR --> OUTPUT:0");
					}

					/*添加对应指令字段*/
					memcpy(FCT_response_buff,FCT_cmd_id_str,FCT_cmd_id_len);
					FCT_response_buff[FCT_cmd_id_len] = '=';
					FCT_cmd_id_len += 1;
					
					/*添加返回信息字段*/
					FCT_response_buff[FCT_cmd_id_len] = temp_para[0];
					FCT_cmd_id_len += 1;

					/*添加返回结果*/
					temp_str_len = strlen(at_str_ok);
					memcpy(FCT_response_buff+FCT_cmd_id_len,at_str_ok,strlen(at_str_ok));
					FCT_cmd_id_len += temp_str_len;
					
					FCT_response_error = 0;
				}
			}
			break;

		default:
			printf("^_^ FCT Running here [%d]\n",cmd_id);
			last_cmd_id = cmd_id;
			FCT_cmd_id_str = NULL;
			FCT_cmd_id_len = 0;
			FCT_response_error = 0;
			break;
	}


	if(FCT_response_error != 0){
		//printf("#FCT: Illegal Parameter!!\n");
		FCT_cmd_id_len = strlen(at_str_error);
		memcpy(FCT_response_buff,at_str_error,FCT_cmd_id_len);
	}

	if(FCT_cmd_id_len > 0){
		printf("FCT_CMD_ID --> Response[%d]",FCT_cmd_id_len);
		printf("AT_Response:\n%s",FCT_response_buff);
		/* printf_buf(FCT_response_buff,FCT_cmd_id_len); */

		FCT_at_cmd_response(FCT_response_buff,FCT_cmd_id_len);
	}
	
	return last_cmd_id;
}

/*--------------------------------------------------------------------------------------------------------------*/

