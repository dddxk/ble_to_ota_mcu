#ifndef _POLYC_TIMER_H_
#define _POLYC_TIMER_H_



#define   TIMER_MODE                1   //��ʱ��ģʽ
#define   TIMER_RISING_EDGE         2   //�����ز�׽
#define   TIMER_FALLING_EDGE        3   //�½��ز�׽
/*---------------------------------------------------------------------------------------------------
                               ��ʱ��timer0��Ӧ�Ĳ�׽�жϿڹ̶�Ϊ PA6
	����  ��u8 time_mode,u32 period_us
	˵��  ��
	* @param time_mode  : ��ѡ��ʱ��ģʽ�������ز�׽ģʽ���½��ز�׽ģʽ
	* @param period_us  : ��ʱ��ʱ�䣬��λus����Ͳ��ܵ���200us��
	����ֵ��void
---------------------------------------------------------------------------------------------------*/
void polyc_timer0_init(u8 time_mode,u32 period_us);
void polyc_timer0_register_callback( void *cbk(void));



#endif
