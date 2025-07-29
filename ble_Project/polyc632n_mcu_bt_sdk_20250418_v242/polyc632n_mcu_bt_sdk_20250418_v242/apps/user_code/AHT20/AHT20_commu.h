#ifndef __AHT20_COMMU_H__
#define __AHT20_COMMU_H__

#include <asm/cpu.h>
#include "user_mcu_iic_soft.h"



#define AHT20_CHECK_EN              0

#if AHT20_CHECK_EN


#define AHT20_ADDR_WRITE			((0x38 << 1) | 0x00)
#define AHT20_ADDR_READ				((0x38 << 1) | 0x01)

#define AHT20_INIT_CMD				0xBE
#define AHT20_START_MEASURE_CMD		0xAC
#define AHT20_SOFT_RESET_CMD		0xBA

void AHT20_Init(void);

//ÏòAHT20·¢ËÍ´¥·¢²âÁ¿ÃüÁî
void AHT20_SendAC(void);

//¶ÁÈ¡AHT20µÄ×´Ì¬¼Ä´æÆ÷
void AHT20_Read_CTdata(u32 *ct);


void AHT20_Start_Init(void);
void AHT20_Poweron_Self_Check(void);
void AHT20_CT_Measure_Callbackfunc(void);
#endif // AHT20_CHECK_EN

#endif
