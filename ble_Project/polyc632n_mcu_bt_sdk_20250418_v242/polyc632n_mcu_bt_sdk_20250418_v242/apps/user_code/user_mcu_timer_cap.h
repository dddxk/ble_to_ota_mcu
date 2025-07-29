#ifndef __USER_MCU_TIMER_CAP_H__
#define __USER_MCU_TIMER_CAP_H__


/*--------------------中断定时器或中断捕捉功能--------------------------
------------------------------------------------------------------------
------------------------------------------------------------------------
------------------------------------------------------------------------*/
#define MCU_TIMER_CAP_ENABLE              0   ///中断定时器或中断捕捉功能使能

#if  MCU_TIMER_CAP_ENABLE

    void mcu_timer_cap_init(void);

#endif // MCU_TIMER_CAP_ENABLE


#endif//__USER_MCU_TIMER_H__
