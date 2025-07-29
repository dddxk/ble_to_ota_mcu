#ifndef __USER_MCU_PULSE_CNT_H__
#define __USER_MCU_PULSE_CNT_H__


#define PULSE_CNT_EN            1///脉冲计数使能


#if   PULSE_CNT_EN

#define PULSE_HIGH_CNT     8  //高电平检测标准
#define PULSE_LOW_CNT      8  //低电平检测标准
#define PULSE_CHECK_STOP_CNT      PULSE_LOW_CNT*2  //

enum
{
    PULSE_DETECT_START = 1,    // 起始
    PULSE_DETECT_UNDERWAY = 2, // 进行中
    PULSE_DETECT_STOP = 3,     // 停止

};

void pulse_cnt_io_init();

void pulse_scan();

#endif // PULSE_CNT_EN

#endif // __USER_MCU_PULSE_CNT_H__


