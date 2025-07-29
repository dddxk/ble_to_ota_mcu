#include "virtual_mcu_main.h"
#include "virtual_mcu_gpio.h"
#include "user_mcu_gpio.h"



/*----------------------------IO口设置说明---------------------------------
注意：IO口的耐高压为3.3V，使用时，硬件上的电压不要超过3.3V，注意分压，防止IO口被损坏

polyc_gpio_set_dir(IO_PORTB_06,1);        //设置为输入
polyc_gpio_set_dir(IO_PORTB_06,0);        //设置为输出
polyc_gpio_set_out(IO_PORTB_06,1);        //设置输出高电平
polyc_gpio_set_out(IO_PORTB_06,0);        //设置输出低电平
polyc_gpio_set_pull_up(IO_PORTB_06,1);    //设置上拉打开
polyc_gpio_set_pull_up(IO_PORTB_06,0);    //设置上拉关闭
polyc_gpio_set_pull_down(IO_PORTB_06,1);  //设置下拉打开
polyc_gpio_set_pull_down(IO_PORTB_06,0);  //设置下拉关闭
polyc_gpio_set_hd(IO_PORTB_06,1);         //设置强驱打开
polyc_gpio_set_hd(IO_PORTB_06,0);         //设置强驱关闭
polyc_gpio_read(IO_PORTB_06)              //设置输入时读取IO口高低电平
------------------------------------------------------------------------*/

void user_set_io_output_high_or_low(u8 gpio, u8 high_or_low) //设置gpio输出高或低电平
{
    polyc_gpio_set_pull_up(gpio,0);    //关闭上拉
    polyc_gpio_set_pull_down(gpio,0);  //关闭下拉
    polyc_gpio_set_hd(gpio,0);         //设置强驱关闭
    polyc_gpio_set_dir(gpio,0);        //设置为输出
    if(high_or_low ==0){
        polyc_gpio_set_out(gpio,0);    //0:输出低
    }else{
        polyc_gpio_set_out(gpio,1);    //1:输出高
    }
}



void user_set_io_input(u8 gpio) //设置gpio为无上下拉输入
{
    polyc_gpio_set_pull_down(gpio,0);  //关闭下拉
    polyc_gpio_set_pull_up(gpio,0);    //关闭上拉
    polyc_gpio_set_dir(gpio,1);        //设置为输入
}

void user_set_io_pull_up_input(u8 gpio) //设置gpio为上拉输入
{
    polyc_gpio_set_pull_down(gpio,0);  //关闭下拉
    polyc_gpio_set_pull_up(gpio,1);    //打开上拉
    polyc_gpio_set_dir(gpio,1);        //设置为输入
}

void user_set_io_pull_down_input(u8 gpio) //设置gpio为下拉输入
{
    polyc_gpio_set_pull_down(gpio,1);  //打开下拉
    polyc_gpio_set_pull_up(gpio,0);    //关闭上拉
    polyc_gpio_set_dir(gpio,1);        //设置为输入
}

u8 user_read_io_value(u8 gpio)  //获取gpio电平状态
{
    return polyc_gpio_read(gpio);
}

