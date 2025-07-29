#include "system/includes.h"
#include "driver/app_config.h"
#include "virtual_mcu_main.h"
#include "virtual_mcu_bt_communication.h"
#include "user_mcu_demo.h"

static u16 mcu_fast_isr_time;
static u16 mcu_slow_isr_time;

//---------注意：注意：注意：此函数只能做底层功能模块变量的使能，只能添加变量初始化，硬件模块还没初始化，不能添加硬件相关函数调用，否则会死机-----------
void polyc_config_bt_system(void)                             //底层调用
{
    //-----------------设置蓝牙和BLE的名字-----------------
    char local_ble_name[29] = "POL632n";                            //更新蓝牙名字，就改此字符串

	memset(virtual_mcu_local_ble_name,0x00,sizeof(virtual_mcu_local_ble_name));
	memcpy(virtual_mcu_local_ble_name,local_ble_name,strlen(local_ble_name)); //设置本地ble名字，最大为29个字节


#ifdef CONFIG_DEBUG_ENABLE
	//-----------------使能调试打印口和打印波特率-----------------
	virtual_mcu_debug_tx_pin = IO_PORT_DP;             //调试用的串口UART0打印接口
	virtual_mcu_debug_baudrate = 460800;                //调试用的串口UART0波特率
#else
    virtual_mcu_debug_tx_pin = 0xf8;                    //底层关闭打印，切记不要删除更改此配置，生产一定要关闭打印，节省资源
	virtual_mcu_debug_baudrate = 0xfff8;                //底层关闭打印，切记不要删除更改此配置，生产一定要关闭打印，节省资源
#endif

	mcu_fast_isr_time = 1000;                           //1ms快速中断
    mcu_slow_isr_time = 20;                             //20ms慢速中断，用于跑main.c运用函数

    mcu_user_system_init();
}


///-----------主要用于跑mcu的快速中断定时器函数，1ms可根据需求做调整-----------
///-----------注意：1、此函数不能跑while函数或者for无限循环函数----------------
///-----------注意：2、此函数中不能跑delay程序----------------------------------
AT_VOLATILE_RAM_CODE
void fast_period_timer_callback(void)      ///1ms
{
    mcu_user_timer_scan();
}



///------------------主要用于跑mcu的main函数，20ms可根据需求做调整-------------
///-----------注意：1、此函数不能跑while函数或者for无限循环函数----------------
///-----------注意：2、此函数中delay的时间不能超过此中断的时间-----------------*/
AT_VOLATILE_RAM_CODE
void slow_period_timer_callback(void)     ///20ms
{
    static u8 cnt = 0;
    cnt++;
    if(cnt >= 10)     //开机200ms后再调用,等待系统各功能稳定再执行
    {
        cnt = 30;
        mcu_user_main();
    }
}




///----------------MCU初始化接口和系统运行函数------------------
void polyc_init(void)   //底层调用
{
    printf("************************Hello Virtual MCU 01 -> init \n");

    mcu_user_init();

    polyc_fast_period_timer_init(mcu_fast_isr_time);                          ///初始化1ms定时中断   //最低不能低于200us
    polyc_fast_period_timer_register_callback(fast_period_timer_callback);    ///注册中断中调用的应用层函数

    polyc_lib_timer_init(mcu_slow_isr_time);                                  ///初始化20ms定时中断
    polyc_lib_timer_register_callback(slow_period_timer_callback);            ///注册中断中调用的应用层函数
}

