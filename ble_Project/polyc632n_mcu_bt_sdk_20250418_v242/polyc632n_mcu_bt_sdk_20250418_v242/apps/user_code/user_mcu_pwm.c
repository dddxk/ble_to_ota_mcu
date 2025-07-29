#include "virtual_mcu_main.h"
#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_pwm.h"
#include "user_mcu_pwm.h"

#if MCU_PWM_OUT_ENABLE

u16 ledr_pwm;
u16 ledg_pwm;
u16 ledb_pwm;
u8 led_change_mode;


u8 buzzing_num = 0,buzzing_cnt = 0;
u8 buzzing_is_on = 0;
u8 buzzing_is_idle = 1;
static u8 buzzing_scan_en = 0;
static u16 mcu_pwm_scan_max =10;

void mcu_pwm_init(void)
{
    ledr_pwm = 0;
    ledg_pwm = 0;
    ledb_pwm = 0;
    led_change_mode = 0;

    polyc_pwm_init(MCU_PWM_CHANNEL0,20000,0,MCU_PWM_CH0_GPIO);     ///初始化PWM，频率为20K，0%占空比
	polyc_set_pwm_duty(MCU_PWM_CHANNEL0,5000);                     ///设置50%占空比

	polyc_pwm_init(MCU_PWM_CHANNEL1,16000,0,MCU_PWM_CH1_GPIO);     ///初始化PWM，频率为16K，0%占空比
	polyc_set_pwm_duty(MCU_PWM_CHANNEL1,5000);                     ///设置50%占空比

	polyc_pwm_init(MCU_PWM_CHANNEL2,16000,0,MCU_PWM_CH2_GPIO);     ///初始化PWM，频率为16K，0%占空比
	polyc_set_pwm_duty(MCU_PWM_CHANNEL2,5000);                     ///设置50%占空比

    polyc_pwm_init(MCU_PWM_CHANNEL3,16000,0,MCU_PWM_CH3_GPIO);     ///初始化PWM，频率为16K，0%占空比
	polyc_set_pwm_duty(MCU_PWM_CHANNEL3,5000);                     ///设置50%占空比

}


void pwm_buzzer_en(u8 en)
{
    if(en)
    {
        polyc_set_pwm_duty(MCU_PWM_CHANNEL0,8000);
    }else
    {
        printf("polyc_set_pwm_duty 00000 \n");
        polyc_set_pwm_duty(MCU_PWM_CHANNEL0,0);
    }
}



void mcu_pwm_scan(void)        //20ms扫描一次
{
    static u16 mcu_pwm_scan_cnt = 0;

    if(buzzing_scan_en == 0)return;

    mcu_pwm_scan_cnt++;

    if(mcu_pwm_scan_cnt < mcu_pwm_scan_max)return;

    mcu_pwm_scan_cnt = 0;

    if(buzzing_is_on)
    {
        printf("buzzing->disen\n");
        pwm_buzzer_en(0);
        buzzing_is_on = 0;
        if(buzzing_cnt == buzzing_num)
        {
            mcu_pwm_scan_max = 1;
        }else
        {
            mcu_pwm_scan_max = 5;
        }

    }else
    {
        printf("buzzing->en\n");

        buzzing_is_on = 1;
        buzzing_cnt++;

        if(buzzing_cnt > buzzing_num )
        {
            pwm_buzzer_en(0);
            buzzing_scan_en = 0;

        }else
        {
            pwm_buzzer_en(1);
            mcu_pwm_scan_max = 5;
        }
    }

}

void set_buzzing_num(u8 _buzzing_num)
{
    buzzing_cnt = 0;
    buzzing_num = _buzzing_num;
    buzzing_is_on = 0;
    buzzing_is_idle = 0;
    buzzing_scan_en = 1;
    mcu_pwm_scan_max = 1;
}

#endif
