#ifndef __USER_MCU_PULSE_CNT_H__
#define __USER_MCU_PULSE_CNT_H__


#define PULSE_CNT_EN            1///�������ʹ��


#if   PULSE_CNT_EN

#define PULSE_HIGH_CNT     8  //�ߵ�ƽ����׼
#define PULSE_LOW_CNT      8  //�͵�ƽ����׼
#define PULSE_CHECK_STOP_CNT      PULSE_LOW_CNT*2  //

enum
{
    PULSE_DETECT_START = 1,    // ��ʼ
    PULSE_DETECT_UNDERWAY = 2, // ������
    PULSE_DETECT_STOP = 3,     // ֹͣ

};

void pulse_cnt_io_init();

void pulse_scan();

#endif // PULSE_CNT_EN

#endif // __USER_MCU_PULSE_CNT_H__


