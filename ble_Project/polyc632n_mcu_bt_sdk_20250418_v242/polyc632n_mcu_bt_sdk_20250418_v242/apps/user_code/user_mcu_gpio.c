#include "virtual_mcu_main.h"
#include "virtual_mcu_gpio.h"
#include "user_mcu_gpio.h"



/*----------------------------IO������˵��---------------------------------
ע�⣺IO�ڵ��͸�ѹΪ3.3V��ʹ��ʱ��Ӳ���ϵĵ�ѹ��Ҫ����3.3V��ע���ѹ����ֹIO�ڱ���

polyc_gpio_set_dir(IO_PORTB_06,1);        //����Ϊ����
polyc_gpio_set_dir(IO_PORTB_06,0);        //����Ϊ���
polyc_gpio_set_out(IO_PORTB_06,1);        //��������ߵ�ƽ
polyc_gpio_set_out(IO_PORTB_06,0);        //��������͵�ƽ
polyc_gpio_set_pull_up(IO_PORTB_06,1);    //����������
polyc_gpio_set_pull_up(IO_PORTB_06,0);    //���������ر�
polyc_gpio_set_pull_down(IO_PORTB_06,1);  //����������
polyc_gpio_set_pull_down(IO_PORTB_06,0);  //���������ر�
polyc_gpio_set_hd(IO_PORTB_06,1);         //����ǿ����
polyc_gpio_set_hd(IO_PORTB_06,0);         //����ǿ���ر�
polyc_gpio_read(IO_PORTB_06)              //��������ʱ��ȡIO�ڸߵ͵�ƽ
------------------------------------------------------------------------*/

void user_set_io_output_high_or_low(u8 gpio, u8 high_or_low) //����gpio����߻�͵�ƽ
{
    polyc_gpio_set_pull_up(gpio,0);    //�ر�����
    polyc_gpio_set_pull_down(gpio,0);  //�ر�����
    polyc_gpio_set_hd(gpio,0);         //����ǿ���ر�
    polyc_gpio_set_dir(gpio,0);        //����Ϊ���
    if(high_or_low ==0){
        polyc_gpio_set_out(gpio,0);    //0:�����
    }else{
        polyc_gpio_set_out(gpio,1);    //1:�����
    }
}



void user_set_io_input(u8 gpio) //����gpioΪ������������
{
    polyc_gpio_set_pull_down(gpio,0);  //�ر�����
    polyc_gpio_set_pull_up(gpio,0);    //�ر�����
    polyc_gpio_set_dir(gpio,1);        //����Ϊ����
}

void user_set_io_pull_up_input(u8 gpio) //����gpioΪ��������
{
    polyc_gpio_set_pull_down(gpio,0);  //�ر�����
    polyc_gpio_set_pull_up(gpio,1);    //������
    polyc_gpio_set_dir(gpio,1);        //����Ϊ����
}

void user_set_io_pull_down_input(u8 gpio) //����gpioΪ��������
{
    polyc_gpio_set_pull_down(gpio,1);  //������
    polyc_gpio_set_pull_up(gpio,0);    //�ر�����
    polyc_gpio_set_dir(gpio,1);        //����Ϊ����
}

u8 user_read_io_value(u8 gpio)  //��ȡgpio��ƽ״̬
{
    return polyc_gpio_read(gpio);
}

