#ifndef __POLYC_FCT_CMD_H__
#define __POLYC_FCT_CMD_H__

#include "asm/cpu.h"
#include "asm/gpio.h"

#include "polyc_at_cmd.h"

enum{
	FCT_GPIOA_GROUP = 0,
	FCT_GPIOB_GROUP,
	FCT_GPIOC_GROUP,
	FCT_GPIOD_GROUP,
	FCT_GPIOE_GROUP,
	FCT_GPIOF_GROUP,
	FCT_GPIOG_GROUP,
	FCT_GPIOH_GROUP,
	FCT_GPIOI_GROUP,
	FCT_GPIOJ_GROUP,

	FCT_USB_DM = 30,
	FCT_USB_DP,
};

extern const str_info_t at_cmd_str_table[];

//工厂测试指令列表
extern const char at_str_fct[];				
extern const char at_str_pinw[];			
extern const char at_str_pinr[];			

//Response指令列表
extern const char at_str_ok[];
extern const char at_str_error[];

extern u8 FCT_at_cmd_process(u8 cmd_id,parse_packet *packet);

#endif
