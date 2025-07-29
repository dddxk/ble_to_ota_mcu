#ifndef _POLYC_TIMER_H_
#define _POLYC_TIMER_H_



#define   TIMER_MODE                1   //定时器模式
#define   TIMER_RISING_EDGE         2   //上升沿捕捉
#define   TIMER_FALLING_EDGE        3   //下降沿捕捉
/*---------------------------------------------------------------------------------------------------
                               定时器timer0对应的捕捉中断口固定为 PA6
	参数  ：u8 time_mode,u32 period_us
	说明  ：
	* @param time_mode  : 可选择定时器模式，上升沿捕捉模式，下降沿捕捉模式
	* @param period_us  : 定时器时间，单位us，最低不能低于200us，
	返回值：void
---------------------------------------------------------------------------------------------------*/
void polyc_timer0_init(u8 time_mode,u32 period_us);
void polyc_timer0_register_callback( void *cbk(void));



#endif
