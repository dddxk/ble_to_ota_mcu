#ifndef _USER_MCU_WAKEUP_H_
#define _USER_MCU_WAKEUP_H_


/*-------------------------低功耗唤醒功能-------------------------------
------------------------------------------------------------------------
------------------------------------------------------------------------
------------------------------------------------------------------------*/
#define MCU_WAKEUP_ENABLE                 0   ///低功耗唤醒功能使能

#define  WAKEUP_GPIO   IO_PORTA_09

#if  MCU_WAKEUP_ENABLE

    void mcu_wakeup_init(void);
    void mcu_power_off(void);
    void mcu_poweroff_det_scan(void);

#endif // MCU_WAKEUP_ENABLE





#endif // _USER_MCU_WAKEUP_H_
