#ifndef __POLYC_AT_CMD_H__
#define __POLYC_AT_CMD_H__

#include "asm/cpu.h"
#include "app_config.h"

#define AT_CMD_SIZE_MAX		(64) // 支持最大AT指令长度

enum{
	FIND_CMD_HEAD = 0, /* AT+ */
	FIND_CMD_END1,	   /* \r */
	FIND_CMD_END2,	   /* \n */
};

typedef struct {
    u16 str_id;
    u16 str_len;
    const char *str;
} str_info_t;

#define INPUT_STR_INFO(id,string)  {.str_id = id, .str = string, .str_len = sizeof(string)-1,}

extern u8 Find_at_cmd_process(u8 *recv_buf,u16 recv_buf_size,parse_packet *packet);
extern u8 General_at_cmd_process(u8 cmd_id,parse_packet *packet);

#endif
