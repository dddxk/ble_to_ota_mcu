#ifndef __VIRTUAL_MCU_PWM_H__
#define __VIRTUAL_MCU_PWM_H__

#define MCU_PWM_CHANNEL0                   0
#define MCU_PWM_CHANNEL1                   1
#define MCU_PWM_CHANNEL2                   2
#define MCU_PWM_CHANNEL3                   3
/**--------------------------------------------------------------------------------------------------------
    函数名：polyc_pwm_init
    参数  ：   初始化PWM--IO，频率和占空比
    说明  ：
    * @param ch  : PWM 通道 0-3
    * @param fre : 频率，单位Hz，不小于400
    * @param duty : 初始占空比，0~10000对应0~100%
    * @param port : pwm脚，除USB口外，可选任意IO口，如IO_PORTB_07
    返回值：void
 ----------------------------------------------------------------------------------------------------------*/
void polyc_pwm_init(u8 ch,u32 fre,u32 duty,u32 port);
/**--------------------------------------------------------------------------------------------------------
    函数名：polyc_set_pwm_duty
    参数  ：   设置占空比
    说明  ：
    * @param ch   : PWM 通道 0-3
    * @param duty : 占空比，0~10000对应0~100%
    返回值：void
 ----------------------------------------------------------------------------------------------------------*/
void polyc_set_pwm_duty(u8 ch,u32 duty);



#endif
