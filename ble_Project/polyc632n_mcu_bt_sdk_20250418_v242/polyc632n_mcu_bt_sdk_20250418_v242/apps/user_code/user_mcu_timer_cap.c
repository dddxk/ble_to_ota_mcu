#include "system/includes.h"
#include "virtual_mcu_main.h"
#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_timer.h"
#include "user_mcu_timer_cap.h"


#if MCU_TIMER_CAP_ENABLE

bool timer_cap_gpio_on_flag = 0;



/**--------------------------------------------------------------------------------------------------------
    ��������mcu_timer_cap_callback ��ʱ���ж����벶��ص�����
    ����  ����
    ˵��  ���ص������Ķζ��� AT_VOLATILE_RAM_CODE

    ����ֵ��void
 ----------------------------------------------------------------------------------------------------------*/
AT_VOLATILE_RAM_CODE
void mcu_timer_cap_callback(void)
{
    ///printf("mcu_timer_cap_callback\n");
    if(timer_cap_gpio_on_flag)
    {
        printf("cap1");
        timer_cap_gpio_on_flag = 0;
        polyc_gpio_set_out(IO_PORTB_09,0);         ///�����
    }
    else
    {
        printf("cap0");
        timer_cap_gpio_on_flag = 1;
        polyc_gpio_set_out(IO_PORTB_09,1);         ///�����
    }
}

void mcu_timer_cap_init(void)
{
    ///printf("mcu_timer_cap_init\n");
    polyc_timer0_init(TIMER_FALLING_EDGE,1000);                           ///�½��ز�׽
    polyc_timer0_register_callback(mcu_timer_cap_callback);               ///ע���ж��е��õ�Ӧ�ò㺯��

    timer_cap_gpio_on_flag = 1;
    polyc_gpio_set_pull_down(IO_PORTB_09,0);   ///�ر�����
    polyc_gpio_set_pull_up(IO_PORTB_09,0);     ///�ر�����
    polyc_gpio_set_dir(IO_PORTB_09,0);         ///�������
    polyc_gpio_set_out(IO_PORTB_09,1);         ///�����
}

#endif

