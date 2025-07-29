#ifndef __USER_MCU_PWM_H__
#define __USER_MCU_PWM_H__


/*----------------------------PWM使能模块---------------------------------
    PWM模块接口有4路，根据需求初始化定义路数和相应接口，PWM-IO可以任意
    接口，根据相应硬件选择对应IO口。
------------------------------------------------------------------------*/
#define MCU_PWM_OUT_ENABLE                 1    ///PWM输出IO使能


#if MCU_PWM_OUT_ENABLE
    #define MCU_PWM_CH0_GPIO                   IO_PORTB_06
    #define MCU_PWM_CH1_GPIO                   IO_PORTA_02
    #define MCU_PWM_CH2_GPIO                   IO_PORTB_00
    #define MCU_PWM_CH3_GPIO                   IO_PORTA_07

    void mcu_pwm_init(void);
    void mcu_pwm_scan(void);
#endif


void set_buzzing_num(u8 _buzzing_num);



#endif
