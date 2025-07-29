#include "polyc_cmd_process_common.h"
#include "polyc_at_cmd.h"
#include "polyc_fct_cmd.h"

/*---------------------------------------------------------------------------------------------------------------------*/

parse_packet para_packet;

/*---------------------------------------------------------------------------------------------------------------------*/

u8 ascii2hex(char acsii)
{
	u8 char_hex = 0xFF;

	if((acsii >= '0')&&(acsii <= '9')){
		char_hex = (acsii - '0');
	}
	else if((acsii >= 'A')&&(acsii <= 'F')){
		char_hex = (acsii - 'A') + 0x0A;
	}

	return char_hex;
}

char hex2ascii(u8 hex)
{
	char temp;

	if((hex >= 0x00)&&(hex <= 0x09)){
		temp = hex + '0';
	}
	else if((hex >= 0x0A)&&(hex <= 0x0F)){
		temp = (hex - 0x0A) + 'A';
	}

	return temp;
}

u8 check_ascii_input_is_illegal(u8 *str,u8 cnt)
{
	u8 ret = 0;
	u8 ascii,i;

	for(i=0;i<cnt;i++)
	{
		ascii = ascii2hex(str[i]);
		if(ascii == 0xFF){
			ret = 1;
			break;
		}
	}

	return ret;
}

u8 check_ascii_number_input_range_is_illegal(u8 *str,u8 cnt,u8 num_max)
{
	u8 ret = 0;
	u8 ascii,hex,i;

	for(i=0;i<cnt;i++)
	{
		if((str[i] >= '0')&&(str[i] <= '9')){
			hex = (str[i] - '0');
			if(hex > num_max){
				ret = 1;
				break;
			}
		}
		else{
			ret = 1;
			break;
		}
	}

	return ret;
}

/*---------------------------------------------------------------------------------------------------------------------*/

static u8 (*Parse_process_func_callback)(u8 *buff,u8 buff_size,parse_packet *packet) = NULL;
static u8 (*General_cmd_execute_func_callback)(u8 cmd_id,parse_packet *packet) = NULL;
static u8 (*FCT_cmd_execute_func_callback)(u8 cmd_id,parse_packet *packet) = NULL;

static void register_parse_process_cbk(u8 *cbk)
{
	if(Parse_process_func_callback == NULL){
		Parse_process_func_callback = cbk;
	}
}

static void register_general_cmd_execute_cbk(u8 *cbk)
{
	if(General_cmd_execute_func_callback == NULL){
		General_cmd_execute_func_callback = cbk;
	}
}

static void register_fct_cmd_execute_cbk(u8 *cbk)
{
	/* Factory Testing */
	if(FCT_cmd_execute_func_callback == NULL){
		FCT_cmd_execute_func_callback = cbk;
	}
}

/*---------------------------------------------------------------------------------------------------------------------*/

void ployc_private_cmd_init(void)
{
	para_packet.cmd_type = CMD_TYPE_NULL;
	memset(para_packet.buff,0x00,sizeof(para_packet.buff));
	para_packet.buff_size = 0;

	printf(" --> %s\n",__func__);

	register_parse_process_cbk(Find_at_cmd_process);
	register_general_cmd_execute_cbk(General_at_cmd_process);
	register_fct_cmd_execute_cbk(FCT_at_cmd_process);
}

void ployc_private_cmd_process(u8 *buff,u8 buff_size)
{
	u8 cmd_id = CMD_ID_NULL;
	parse_packet *packet = &para_packet;
	u8 ret;

	if(Parse_process_func_callback)
	{
		//1.解析接收数据,获取有效指令
		//2.返回指令ID,同时得到解析包
		cmd_id = Parse_process_func_callback(buff,buff_size,packet);
	}

	if(cmd_id != CMD_ID_NULL)
	{
		//上面得到的解析包，以下函数使用时只进行执行
		if(General_cmd_execute_func_callback){
			cmd_id = General_cmd_execute_func_callback(cmd_id,packet);
		}
		if(FCT_cmd_execute_func_callback){
			cmd_id = FCT_cmd_execute_func_callback(cmd_id,packet);
		}
	}
	return;
}

/*---------------------------------------------------------------------------------------------------------------------*/

