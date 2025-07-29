#ifndef __USER_MCU_PWM_H__
#define __USER_MCU_PWM_H__


/*----------------------------PWMʹ��ģ��---------------------------------
    PWMģ��ӿ���4·�����������ʼ������·������Ӧ�ӿڣ�PWM-IO��������
    �ӿڣ�������ӦӲ��ѡ���ӦIO�ڡ�
------------------------------------------------------------------------*/
#define MCU_PWM_OUT_ENABLE                 1    ///PWM���IOʹ��


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
