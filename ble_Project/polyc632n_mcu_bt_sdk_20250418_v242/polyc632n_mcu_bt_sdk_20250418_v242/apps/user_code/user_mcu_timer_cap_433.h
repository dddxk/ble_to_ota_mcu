#ifndef __USER_MCU_TIMER_CAP433_H__
#define __USER_MCU_TIMER_CAP433_H__

#define   MCU_TIMER_CAP433_ENABLE         0


#if  MCU_TIMER_CAP433_ENABLE



#define TCFG_433_PORT                        IO_PORTB_05

#define RECEPTION_NUMBER                 25//需要接受多少数据bit
#define BUF_NUM                          (RECEPTION_NUMBER / 8) + ((RECEPTION_NUMBER % 8 == 0) ? 0 : 1)

//433数据捕获部分,433协议变动需修改这部分
#define CNT_READ_1MS_VALUE               610//unit:0.1ms
#define CNT_READ_INTERVAL1               107 * CNT_READ_1MS_VALUE//uint: 0.1ms;首次数据触发沿时长: 107 * 0.1ms = 10.7ms
#define ALLOW_ERROR_VALUE                CNT_READ_1MS_VALUE / 2//uint: 0.1ms;允许误差:0.1ms / 2 = 50ns

#define CNT_LOW_LEVEL                    2200//低电平触发时间:约0.33ms
#define CNT_HIGH_LEVEL                   3 * CNT_LOW_LEVEL//高电平触发时间:约3 * CNT_LOW_LEVEL = 约1ms

#define KEY_INTERVAL_SET                 100 //uint:1ms 设置长按每包间隔
#define ALLOW_SET_CLOCK_RATIO            1//191 //因为时钟改变调节倍率,std:1,rlc:191(可修改rlc倍率)

#define PAIR_CODE_NUMBER                 3//433配对码个数设置


//具体需要的时间以后直接改变宏的参数就ok了!
#define CAP433_KEY_ONE_PAG_NUM            4//433单包有4次
#define CAP433_KEY_SHORT_CNT              (CAP433_KEY_ONE_PAG_NUM-1)*1  //短按标准//50ms*3
#define CAP433_KEY_DOUBLE_TIMOUT_CNT      CAP433_KEY_ONE_PAG_NUM*1      //双击超时时间//50ms*5
#define CAP433_KEY_LONG_CNT               CAP433_KEY_ONE_PAG_NUM*6     //双击超时时间//50ms*40
#define CAP433_KEY_HOLD_CNT               CAP433_KEY_ONE_PAG_NUM*(6 + 2) //双击超时时间//50ms*(40 + 10)
#define CAP433_KEY_RELEASE_CNT            CAP433_KEY_ONE_PAG_NUM*2  //松手


//定义按键按下去的状态

#define CAP433_KEY_SHORT       0x01 //执行一次短按之后松开手之后不需要呼应
#define CAP433_KEY_SHORT_UP    0x02
#define CAP433_KEY_LONG        0x03 //长按
#define CAP433_KEY_LONG_UP     0x04//长按松手后执行消息
#define CAP433_KEY_HOLD        0x05//长按不放，需要持续性消息

#define CAP433_KEY_NO_STATE    0xff

#define   CAP433_KEY_NO_VALUE    0x00
#define   CAP433_KEY_1           0x01
#define   CAP433_KEY_2           0x02
#define   CAP433_KEY_3           0x03
#define   CAP433_KEY_4           0x04

void timer_cap_433_test(void);

#endif // MCU_TIMER_CAP433_ENABLE
#endif // __USER_MCU_TIMER_CAP433_H__
