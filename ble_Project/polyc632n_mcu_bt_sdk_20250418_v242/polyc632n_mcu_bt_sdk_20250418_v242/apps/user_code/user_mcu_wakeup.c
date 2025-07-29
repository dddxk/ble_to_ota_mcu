#include "system/includes.h"
#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_main.h"
#include "virtual_mcu_wakeup.h"
#include "user_mcu_wakeup.h"


#if MCU_WAKEUP_ENABLE



void mcu_wakeup_init(void)
{
    polyc_set_wakeup_io(1,WAKEUP_FALLING_EDGE,WAKEUP_GPIO);  //下降沿唤醒，唤醒口必须在系统初始化前配置，不然不认
}

void mcu_power_off(void)
{
    polyc_power_off_io_keep(WAKEUP_GPIO);
    polyc_power_off_to_sleep();
}



void mcu_poweroff_det_scan(void)
{
    static u8 cnt = 0;

    polyc_gpio_set_pull_down(IO_PORTA_02,0);   ///关闭下拉
    polyc_gpio_set_pull_up(IO_PORTA_02,0);     ///关闭上拉
    polyc_gpio_set_dir(IO_PORTA_02,0);         ///设置输出
    polyc_gpio_set_out(IO_PORTA_02,1);         ///输出高

    polyc_gpio_set_pull_down(IO_PORTA_01,0);   ///关闭下拉
    polyc_gpio_set_pull_up(IO_PORTA_01,1);     ///打开上拉
    polyc_gpio_set_dir(IO_PORTA_01,1);         ///设置输入

    delay(10);

    if(polyc_gpio_read(IO_PORTA_01) == 0)
    {
        cnt++;
        if(cnt == 50)
        {
            mcu_power_off();
        }

        if(cnt >= 100)
        {
            cnt = 100;
        }
    }
    else
    {
        cnt = 0;
    }
}

#endif


