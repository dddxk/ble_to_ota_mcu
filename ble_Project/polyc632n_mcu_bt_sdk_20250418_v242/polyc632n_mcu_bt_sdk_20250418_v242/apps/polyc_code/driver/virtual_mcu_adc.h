#ifndef __VIRTUAL_MCU_ADC_H__
#define __VIRTUAL_MCU_ADC_H__



/**--------------------------------------------------------------------------------------------------------
    函数名： polyc_get_power_on_adc  开机可直接获取该通道的ADC值
    参数  ：u32 ch
    说明  ：
    * @param ch  : ADC通道
    返回值：u16  该ADC通道的AD值（0-1023）
 ----------------------------------------------------------------------------------------------------------*/
u16 polyc_get_power_on_adc(u32 ch);


/**--------------------------------------------------------------------------------------------------------
    polyc_adc_remove_sample_ch
    参数  ：   u32 ch 设置采样通道
    说明  ：   ADC通道采样通道移除，需要移除多少通道就移除多少个
    返回值：void
----------------------------------------------------------------------------------------------------------*/
void polyc_adc_remove_sample_ch(u32 ch);
/**--------------------------------------------------------------------------------------------------------
    函数名：polyc_adc_set_init
    参数  ：   u32 ch 设置采样通道
    说明  ：   ADC通道初始化，需要多少路就设置多少个
    返回值：void
----------------------------------------------------------------------------------------------------------*/
extern void polyc_adc_set_init(u32 ch);

/**--------------------------------------------------------------------------------------------------------
    函数名： polyc_get_adc_value
    参数  ： u32 ch 设置采样通道
    说明  ：
    返回值：u16反馈该通道的ADC采样值
----------------------------------------------------------------------------------------------------------*/
extern u16 polyc_get_adc_value(u32 ch);

/**--------------------------------------------------------------------------------------------------------
    函数名： polyc_get_adc_voltage
    参数  ： u32 ch 设置采样通道
    说明  ：
    返回值：u32反馈该通道的ADC采样值转换而成的电压值
----------------------------------------------------------------------------------------------------------*/
extern u32 polyc_get_adc_voltage(u32 ch);

#endif
