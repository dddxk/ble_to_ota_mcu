#ifndef __USER_MCU_MEMORY_H__
#define __USER_MCU_MEMORY_H__

/*----------------------内部掉电记忆存储测试----------------------------
------------------------------------------------------------------------
------------------------------------------------------------------------
------------------------------------------------------------------------*/
#define MCU_MEMORY_ENABLE                 0   ///内部掉电记忆存储测试使能


#if  MCU_MEMORY_ENABLE

extern void mcu_memory_init(void);

#endif // MCU_MEMORY_ENABLE


#endif
