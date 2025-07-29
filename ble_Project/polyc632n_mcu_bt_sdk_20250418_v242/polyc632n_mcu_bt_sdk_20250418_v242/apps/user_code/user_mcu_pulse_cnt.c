
#include "system/timer.h"
#include "virtual_mcu_main.h"
#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_adc.h"
#include "user_mcu_pulse_cnt.h"
#include "user_mcu_gpio.h"


#if   PULSE_CNT_EN


void pulse_cnt_io_init()
{
    user_set_io_pull_down_input(IO_PORTA_06);
    user_set_io_pull_down_input(IO_PORTB_03);
}


void pulse_cnt_filter(u8 *pulse_io_val)
{
    int i = 0;
	static u16 high_cnt[2];//高电平检测计数
	static u8  low_cnt[2]; //低电平检测计数
	static u8  sec_pulse_cnt[2];//脉冲计数
	static u8  check_state[2] = {PULSE_DETECT_STOP,PULSE_DETECT_STOP};//初始化检测的状态
    for(i = 0; i < 2; i++)
    {
        switch(check_state[i])
        {
            case PULSE_DETECT_START:
                //putchar('a');
                if(pulse_io_val[i])
                {
                    high_cnt[i] = 1;
                    low_cnt[i] = 0;
                }else
                {
                    low_cnt[i] = 1;
                    high_cnt[i] = 0;
                }

                check_state[i] = PULSE_DETECT_UNDERWAY;
                break;
            case PULSE_DETECT_UNDERWAY:
                //putchar('u');
                if(pulse_io_val[i])
                {
                    high_cnt[i]++;
                    low_cnt[i] = 0;
                }else
                {
                    high_cnt[i] = 0;
                    low_cnt[i]++;
                }
                if(high_cnt[i] == PULSE_HIGH_CNT)//这里只判断=是为了防止持续高电平导致重复计数
                {
                    sec_pulse_cnt[i]++;
                    //putchar('q');
                }

                if(low_cnt[i] > PULSE_CHECK_STOP_CNT)
                {
                    check_state[i] = PULSE_DETECT_STOP;
                    printf("stop sec_pulse_cnt[%d]=[%d]",i,sec_pulse_cnt[i]);
                }
                break;
            case PULSE_DETECT_STOP:
                //putchar('s');
                sec_pulse_cnt[i] = 0;
                high_cnt[i] = 0;
                low_cnt[i] = 0;
                if(pulse_io_val[i])
                {
                    check_state[i] = PULSE_DETECT_START;
                }
                break;
        }

    }
}
static u8 io_value[2] = {0x00};
void pulse_scan()//20ms
{
    memset(io_value,0x00,sizeof(io_value));
    //printf("[%d][%d]",user_read_io_value(IO_PORTA_06),user_read_io_value(IO_PORTB_03));
    if(0 == user_read_io_value(IO_PORTA_06))
    {
        io_value[0] = 0;
    }else
    {
        io_value[0] = 1;
    }
    if(0 == user_read_io_value(IO_PORTB_03))
    {
        io_value[1] = 0;
    }else
    {
        io_value[1] = 1;
    }
   // put_buf(io_value,sizeof(io_value));
    pulse_cnt_filter(io_value);
}









#endif // PULSE_CNT_EN

