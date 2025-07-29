#ifndef __VIRTUAL_MCU_CHARGE_H__
#define __VIRTUAL_MCU_CHARGE_H__

/*------充满电电压选择 3.962V-4.611V-------*/
#define CHARGE_FULL_V_3962		0
#define CHARGE_FULL_V_4002		1
#define CHARGE_FULL_V_4044		2
#define CHARGE_FULL_V_4086		3
#define CHARGE_FULL_V_4130		4
#define CHARGE_FULL_V_4175		5
#define CHARGE_FULL_V_4222		6
#define CHARGE_FULL_V_4270		7
#define CHARGE_FULL_V_4308		8
#define CHARGE_FULL_V_4349		9
#define CHARGE_FULL_V_4391		10
#define CHARGE_FULL_V_4434		11
#define CHARGE_FULL_V_4472		12
#define CHARGE_FULL_V_4517		13
#define CHARGE_FULL_V_4564		14
#define CHARGE_FULL_V_4611		15
#define CHARGE_FULL_V_MAX       16
/*****************************************/

/*充满电电流选择 2mA-30mA*/
#define CHARGE_FULL_mA_2		0
#define CHARGE_FULL_mA_5		1
#define CHARGE_FULL_mA_7	 	2
#define CHARGE_FULL_mA_10		3
#define CHARGE_FULL_mA_15		4
#define CHARGE_FULL_mA_20		5
#define CHARGE_FULL_mA_25		6
#define CHARGE_FULL_mA_30		7

/*
 	充电电流选择
	恒流：20-220mA
*/
#define CHARGE_mA_20			0
#define CHARGE_mA_30			1
#define CHARGE_mA_40			2
#define CHARGE_mA_50			3
#define CHARGE_mA_60			4
#define CHARGE_mA_70			5
#define CHARGE_mA_80			6
#define CHARGE_mA_90			7
#define CHARGE_mA_100			8
#define CHARGE_mA_110			9
#define CHARGE_mA_120			10
#define CHARGE_mA_140			11
#define CHARGE_mA_160			12
#define CHARGE_mA_180			13
#define CHARGE_mA_200			14
#define CHARGE_mA_220			15

/**--------------------------------------------------------------------------------------------------------
    函数名：polyc_charge_en，使能内置充电
    参数  ：u8 enabled
    说明  ：
    * @param enabled  : 使能
    返回值：void
 ----------------------------------------------------------------------------------------------------------*/
extern void polyc_charge_en(u8 enabled);

/**--------------------------------------------------------------------------------------------------------
    函数名：polyc_get_charge_en_flag，获取内置充电标志
    参数  ：void
    说明  ：
    * @param
    返回值：0/1
 ----------------------------------------------------------------------------------------------------------*/
extern u8 polyc_get_charge_en_flag(void);


/**--------------------------------------------------------------------------------------------------------
    函数名：polyc_set_charge_full_v,设置充电满电电压
    参数  ：u8 full_v
    说明  ：
    * @param 电压 对应的索引号
    返回值：
 ----------------------------------------------------------------------------------------------------------*/
extern void polyc_set_charge_full_v(u8 full_v);

/**--------------------------------------------------------------------------------------------------------
    函数名：polyc_get_charge_full_v,获取充电满电电压
    参数  ：u
    说明  ：
    * @param
    返回值： charge_full_v
 ----------------------------------------------------------------------------------------------------------*/
extern u8 polyc_get_charge_full_v(void);


/**--------------------------------------------------------------------------------------------------------
    函数名：polyc_set_charge_full_ma,设置充电满电电流
    参数  ：u8 full_ma
    说明  ：
    * @param 电流 对应的索引号
    返回值：
 ----------------------------------------------------------------------------------------------------------*/
extern void polyc_set_charge_full_mA(u8 full_mA);
/**--------------------------------------------------------------------------------------------------------
    函数名：polyc_get_charge_full_ma,获取充电满电电流
    参数  ：u
    说明  ：
    * @param
    返回值： charge_full_ma
 ----------------------------------------------------------------------------------------------------------*/
extern u8 polyc_get_charge_full_mA(void);

/**--------------------------------------------------------------------------------------------------------
    函数名：polyc_set_charge_ma,设置充电电流
    参数  ：u8 ma
    说明  ：
    * @param 电流 对应的索引号
    返回值：
 ----------------------------------------------------------------------------------------------------------*/
extern void polyc_set_charge_mA(u8 mA);
/**--------------------------------------------------------------------------------------------------------
    函数名：polyc_get_charge_ma,获取充电电流
    参数  ：u
    说明  ：
    * @param
    返回值： charge_ma
 ----------------------------------------------------------------------------------------------------------*/
extern u8 polyc_get_charge_mA(void);


/**--------------------------------------------------------------------------------------------------------
    函数名：polyc_get_charge_online_flag,获取充电插入标志
    参数  ：
    说明  ：
    * @param
    返回值： get_charge_online_flag
 ----------------------------------------------------------------------------------------------------------*/
extern u8 polyc_get_charge_full_flag(void);

/**--------------------------------------------------------------------------------------------------------
    函数名：polyc_get_charge_online_flag,获取充电插入标志
    参数  ：
    说明  ：
    * @param
    返回值： get_charge_online_flag
 ----------------------------------------------------------------------------------------------------------*/
extern u8 polyc_get_charge_online_flag(void);


/**--------------------------------------------------------------------------------------------------------
    函数名：polyc_get_vbat_value,获取电池电压
    参数  ：
    说明  ：
    * @param
    返回值： get_vbat_value
 ----------------------------------------------------------------------------------------------------------*/
extern u16 polyc_get_vbat_value(void);


#endif
