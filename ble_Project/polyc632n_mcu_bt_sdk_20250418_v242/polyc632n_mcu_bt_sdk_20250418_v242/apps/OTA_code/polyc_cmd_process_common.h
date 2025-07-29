#ifndef __POLYC_CMD_PROCESS_COMMON_H__
#define __POLYC_CMD_PROCESS_COMMON_H__

#include "asm/cpu.h"
#include "gpio.h"

#define PARSE_PACKET_SIZE	 	(64)

typedef struct _parse_packet
{
    u8 cmd_type;
	u8 buff[PARSE_PACKET_SIZE];
	u8 buff_size;
}parse_packet;

enum {
    CMD_ID_NULL = 0,

	/* --- ͨ��ָ���б� --- */
    CMD_ID_VER,
    CMD_ID_FCTRST,
    CMD_ID_RESET,
    CMD_ID_BAUD,
    CMD_ID_NAME,
    CMD_ID_ADDR,
    CMD_ID_SERVUUID,
    CMD_ID_TXUUID,
    CMD_ID_RXUUID,
    CMD_ID_ADVINV,
    CMD_ID_TXPWR,
    CMD_ID_CONSTA,
    
   
	CMD_ID_TEST,
	CMD_ID_SERV,
	CMD_ID_NOTIFY,
	CMD_ID_WRITE,

	/* -- ��������ָ���б� -- */

    CMD_ID_FCT,
    CMD_ID_FCT_PINW,
    CMD_ID_FCT_PINR,

    /* --- Add CMD_ID_XXX --- */
	//.....
	
	CMD_ID_MAX,
	CMD_ID_NO_MATCH, /* ռλ,����ָ�ƥ������,û�ж�Ӧָ��ƥ���ַ�����ָ��� */
};

enum{
	CMD_TYPE_NULL = 0,
	CMD_TYPE_INQUIRE,    //��ѯָ��:  AT+<CMD_ID_XX>?\r\n
	CMD_TYPE_SET,		 //����ָ��:  AT+<CMD_ID_XX>=<parameter>\r\n
	CMD_TYPE_EXECUTE,	 //ִ��ָ��:  AT+<CMD_ID_XX>\r\n
};

extern u8 ascii2hex(char acsii);
extern char hex2ascii(u8 hex);
extern u8 check_ascii_input_is_illegal(u8 *str,u8 cnt);
extern u8 check_ascii_number_input_range_is_illegal(u8 *str,u8 cnt,u8 num_max);

extern void ployc_private_cmd_init(void);
extern void ployc_private_cmd_process(u8 *buff,u8 buff_size);
extern void polyc_private_response(u8 *buff,u8 buff_size);

#endif
