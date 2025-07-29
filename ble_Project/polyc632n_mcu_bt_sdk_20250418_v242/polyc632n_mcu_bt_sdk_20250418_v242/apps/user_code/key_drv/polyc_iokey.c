
#include "system/timer.h"
#include "virtual_mcu_main.h"
#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_adc.h"
#include "polyc_iokey.h"
#include "user_mcu_gpio.h"
#include "driver_ic_1628.h"

#if  IO_KEY_EN
void key_io_init()
{
    user_set_io_pull_up_input(IO_PORTA_00);
    user_set_io_pull_up_input(IO_PORTB_06);
    user_set_io_pull_up_input(IO_PORTB_07);
    user_set_io_pull_up_input(IO_PORTB_08);
    user_set_io_pull_up_input(IO_PORTB_00);
    user_set_io_pull_up_input(IO_PORTB_09);

}


static void key_to_deal_work(u8 key_value,u8 key_state)
{
    printf("key_value[%x]key_state[%d]\n",key_value,key_state);
    static u8 key_value0_led_en = 0;
    static u8 key_value1_led_en = 0;
    static u8 key_value2_led_en = 0;
    static u8 key_value3_led_en = 0;
    switch(key_state)
    {
        case KEY_SHORT_UP:
            if(key_value == IO_KEY_0)
            {
                set_buzzing_num(1);
                key_value0_led_en = !key_value0_led_en;
                #if   IC_1628_EN
                TM1628_disp_icon(DISP_GROUP7,BUTTON_LED1,key_value0_led_en);
                #endif // IC_1628_EN
            }else if(key_value == IO_KEY_1)
            {
                set_buzzing_num(1);
                key_value1_led_en = !key_value1_led_en;
                #if   IC_1628_EN
                TM1628_disp_icon(DISP_GROUP7,BUTTON_LED2,key_value1_led_en);
                #endif // IC_1628_EN
            }else if(key_value == IO_KEY_2)
            {
                set_buzzing_num(1);
                key_value2_led_en = !key_value2_led_en;
                #if   IC_1628_EN
                TM1628_disp_icon(DISP_GROUP7,BUTTON_LED3,key_value2_led_en);
                #endif // IC_1628_EN
            }else if(key_value == IO_KEY_3)
            {
                set_buzzing_num(1);
                key_value3_led_en = !key_value3_led_en;
                #if   IC_1628_EN
                TM1628_disp_icon(DISP_GROUP7,BUTTON_LED4,key_value3_led_en);
                #endif // IC_1628_EN
            }else if(key_value == IO_KEY_4)
            {

            }else if(key_value == IO_KEY_5)
            {

            }
            break;
        case KEY_LONG_UP:
            break;
        case KEY_HOLD:
            if(key_value == IO_KEY_0)
            {

            }else if(key_value == IO_KEY_1)
            {

            }else if(key_value == IO_KEY_2)
            {

            }else if(key_value == IO_KEY_3)
            {

            }else if(key_value == IO_KEY_4)
            {

            }else if(key_value == IO_KEY_5)
            {

            }
            break;
        case KEY_LONG:
            if(key_value == IO_KEY_0)
            {

            }else if(key_value == IO_KEY_1)
            {

            }else if(key_value == IO_KEY_2)
            {

            }else if(key_value == IO_KEY_3)
            {

            }else if(key_value == IO_KEY_4)
            {

            }else if(key_value == IO_KEY_5)
            {

            }
            break;
    }
}

void key_value_filter(u8 key_value)
{
	static u16 key_press_cnt;//按下去的检测
	static u8  key_release_cnt;//松手的检测
	static u8  back_key;//存储按下去的键值
	u8 key_state = KEY_NO_STATE;//初始化键值的状态

    if(key_value != IO_KEY_NO_VALUE)//此时的值已经从主循环扫描而得到了
    {
        if( back_key != key_value)//快速变换按键的情况下，以最新的按键为主
        {
            back_key = key_value;
            key_press_cnt = 0;
        }
        key_press_cnt++;
        key_release_cnt = 0;//松手计数清零
        if(key_press_cnt == KEY_SHORT_CNT )//有些按键的作用只有短按，实际工作中具体分析
        {
            key_state = KEY_SHORT;
        }
        else if(key_press_cnt == KEY_LONG_CNT)//到达长按的阈值，1s
        {
            key_state = KEY_LONG;
            key_to_deal_work(back_key,key_state);
        }
        else if(key_press_cnt >= KEY_HOLD_CNT)//到达了长按不放的阈值
        {
            key_press_cnt = KEY_LONG_CNT;//防止超过参数的最大值，同时防止长按继续触发
            key_state = KEY_HOLD;
            key_to_deal_work(back_key,key_state);
        }
    }
    else
    {
        key_release_cnt++;//检测到松手电平了，不要马上判断说是松手了
        if(key_release_cnt >= KEY_RELEASE_CNT)//约30ms稳定的松手电平之后就判断为松手，
        {
            key_release_cnt = 0;
            if(key_press_cnt >= KEY_SHORT_CNT && key_press_cnt < KEY_LONG_CNT)//大于15ms小于1s判断为短按
            {
                key_state = KEY_SHORT_UP;
                key_to_deal_work(back_key,key_state);
            }
            else if(key_press_cnt >= KEY_LONG_CNT)
            {
                key_state = KEY_LONG_UP;
                key_to_deal_work(back_key,key_state);
            }
            key_press_cnt=0;//只有真正松手了才会清掉这个计数
        }
    }
}

void io_key_scan()//20ms
{
    u8 key_value = IO_KEY_NO_VALUE;
    if(0 == user_read_io_value(IO_PORTA_00))
    {
        key_value |= BIT(0);
    }
    if(0 == user_read_io_value(IO_PORTB_06))
    {
        key_value |= BIT(1);
    }
    if(0 == user_read_io_value(IO_PORTB_07))
    {
        key_value |= BIT(2);
    }
    if(0 == user_read_io_value(IO_PORTB_08))
    {
        key_value |= BIT(3);
    }
    if(0 == user_read_io_value(IO_PORTB_00))
    {
        key_value |= BIT(4);
    }
    if(0 == user_read_io_value(IO_PORTB_09))
    {
        key_value |= BIT(5);
    }

    key_value_filter(key_value);
}
#endif
