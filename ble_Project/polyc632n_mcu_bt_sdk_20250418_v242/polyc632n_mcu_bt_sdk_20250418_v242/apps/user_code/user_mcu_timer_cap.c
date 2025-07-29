#include "system/includes.h"
#include "virtual_mcu_main.h"
#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_timer.h"
#include "user_mcu_timer_cap.h"


#if MCU_TIMER_CAP_ENABLE

bool timer_cap_gpio_on_flag = 0;



/**--------------------------------------------------------------------------------------------------------
    函数名：mcu_timer_cap_callback 定时器中断输入捕获回调函数
    参数  ：无
    说明  ：回调函数的段定义 AT_VOLATILE_RAM_CODE

    返回值：void
 ----------------------------------------------------------------------------------------------------------*/
AT_VOLATILE_RAM_CODE
void mcu_timer_cap_callback(void)
{
    ///printf("mcu_timer_cap_callback\n");
    if(timer_cap_gpio_on_flag)
    {
        printf("cap1");
        timer_cap_gpio_on_flag = 0;
        polyc_gpio_set_out(IO_PORTB_09,0);         ///输出低
    }
    else
    {
        printf("cap0");
        timer_cap_gpio_on_flag = 1;
        polyc_gpio_set_out(IO_PORTB_09,1);         ///输出高
    }
}

void mcu_timer_cap_init(void)
{
    ///printf("mcu_timer_cap_init\n");
    polyc_timer0_init(TIMER_FALLING_EDGE,1000);                           ///下降沿捕捉
    polyc_timer0_register_callback(mcu_timer_cap_callback);               ///注册中断中调用的应用层函数

    timer_cap_gpio_on_flag = 1;
    polyc_gpio_set_pull_down(IO_PORTB_09,0);   ///关闭下拉
    polyc_gpio_set_pull_up(IO_PORTB_09,0);     ///关闭上拉
    polyc_gpio_set_dir(IO_PORTB_09,0);         ///设置输出
    polyc_gpio_set_out(IO_PORTB_09,1);         ///输出高
}

#endif

