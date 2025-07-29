#include "system/timer.h"
#include "virtual_mcu_main.h"
#include "user_mcu_demo.h"
#include "virtual_mcu_bt_communication.h"
#include "user_mcu_gpio.h"
#include "user_mcu_adc.h"
#include "user_mcu_pwm.h"
#include "user_mcu_uart.h"
#include "user_mcu_memory.h"
#include "user_mcu_timer_cap.h"
#include "user_mcu_wakeup.h"
#include "user_mcu_ble_client.h"
#include "user_mcu_ble_server.h"
#include "user_mcu_ble_nonconn_rx_tx.h"
#include "user_mcu_charge.h"
#include "user_mcu_spi.h"
#include "driver_ic_1628.h"
#include "user_mcu_timer_cap_433.h"
#include "AHT20_commu.h"
#include "user_mcu_pulse_cnt.h"
static u16 led_disp_frequency = 0;

/*---------------------------------------------------------------------------------------------------------*/
/*--------------------------------------BLE数据传输应用模式切换---------------------------------------------*/

void app_user_ble_data_transfer_mode_select(u8 mode_select, u8 enable)
{
    if(enable){
        if(get_virtual_mcu_ble_app_mode() == mode_select){ //当前模式已使能
            return; //不执行
        }
    }

    switch(mode_select)
    {
        case VIRTUAL_MCU_BLE_APP_MODE_NULL:
            polyc_set_ble_cmd(MCU_SET_BLE_APP_MODE_NULL, NULL, 0);
            break;

        case VIRTUAL_MCU_BLE_SERVER_APP_MODE:
            app_user_ble_server_enable(enable);
            break;

        case VIRTUAL_MCU_BLE_CLIENT_APP_MODE:
            app_user_ble_client_enable(enable);
            break;

        case VIRTUAL_MCU_BLE_NONCONN_RX_TX_APP_MODE:
            app_user_ble_nonconn_rx_tx_enable(enable);
            break;
    }
}


/**--------------------------------------------------------------------------------------------------------
    函数名：mcu_user_system_init  需要提前设置的系统初始化参数
    返回值：void
----------------------------------------------------------------------------------------------------------*/
void mcu_user_system_init(void)
{
#if MCU_WAKEUP_ENABLE
    mcu_wakeup_init();
#endif
    mcu_charge_init();
}
/**--------------------------------------------------------------------------------------------------------
    函数名：mcu_user_init  应用程序初始化
    返回值：void
----------------------------------------------------------------------------------------------------------*/
void mcu_user_init(void)
{

    mcu_ble_server_init(); //需要先初始化
    mcu_ble_client_init(); //需要先初始化

     ///芯片上电开机选择的BLE应用
    app_user_ble_data_transfer_mode_select(VIRTUAL_MCU_BLE_SERVER_APP_MODE, 1);

    //user_set_io_output_high_or_low(IO_PORTA_02,1);//GPIO TEST

#if	MCU_ADC_SCAN_ENABLE
	mcu_adc_init();                 ///ADC接口和模块初始化
#endif

#if MCU_PWM_OUT_ENABLE
    mcu_pwm_init();
#endif

#if MCU_UART0_ENABLE
    mcu_uart0_init();                ///初始化串口0通讯
#endif

#if MCU_UART1_ENABLE
    mcu_uart1_init();                ///初始化串口1通讯
#endif

#if MCU_UART2_ENABLE
    mcu_uart2_init();                ///初始化串口2通讯
#endif

#if MCU_MEMORY_ENABLE
    mcu_memory_init();             ///测试内部存储初始化
#endif

#if MCU_TIMER_CAP_ENABLE
    mcu_timer_cap_init();
#endif


#if MCU_SPI1_EN || MCU_SPI2_EN
    virtual_mcu_digital_rgb_led_disp_init();
#endif // MCU_SPI_EN

#if  IC_1628_EN
    Init_1628();
#endif // IC_1628_EN

#if  MCU_TIMER_CAP433_ENABLE
    timer_cap_433_test();
#endif // IC_1628_EN

#if IO_KEY_EN
    key_io_init();
#endif // IO_KEY_EN

#if AHT20_CHECK_EN
    AHT20_Init();
#endif // AHT20_CHECK_EN

#if   PULSE_CNT_EN
    pulse_cnt_io_init();
#endif // PULSE_CNT_EN

    polyc_ir_init(IO_PORTB_02,0XFF00);
}
/**--------------------------------------------------------------------------------------------------------
    函数名：mcu_user_timer_scan 应用程序1ms中断扫描
    返回值：void
----------------------------------------------------------------------------------------------------------*/
void mcu_user_timer_scan(void)
{
#if	MCU_ADC_SCAN_ENABLE
	mcu_adc_scan_deal();
#endif
}
/**--------------------------------------------------------------------------------------------------------
    函数名：mcu_user_main 应用程序主函数运行功能，20ms跑一次
    返回值：void

    注意：请勿在此函数中任何死循环函数，如有添加循环一定要少于10ms内完成函数处理跳出
----------------------------------------------------------------------------------------------------------*/
void mcu_user_main(void)
{

#if IO_KEY_EN
    io_key_scan();
#endif // IO_KEY_EN

#if IC_1628_EN
    key_1628_scan();
#endif // IO_KEY_EN

#if MCU_PWM_OUT_ENABLE
    mcu_pwm_scan();
#endif

#if MCU_UART0_ENABLE
	mcu_uart0_send_data();
#endif

#if MCU_UART1_ENABLE
	mcu_uart1_send_data();
#endif

#if MCU_UART2_ENABLE
	mcu_uart2_send_data();
#endif

#if MCU_WAKEUP_ENABLE
    mcu_poweroff_det_scan();
#endif
#if MCU_CHARGE_EN
    mcu_charge_check_test();
#endif // MCU_CHARGE_EN
#if MCU_SPI1_EN || MCU_SPI2_EN
    static u16 led_disp_cnt = 0;
    if(led_disp_cnt >= led_disp_frequency)
    {
        led_disp_cnt = 0;
        virtual_mcu_digital_rgb_led_disp_func_callback();
    }else
    {
        led_disp_cnt++;
    }

#endif // MCU_SPI1_EN
#if  IC_1628_EN
    static u8 is_test_mode = 1;///开机时进入测试模式
    static u8 test_mode_ret = 0;
    static u8 disp_cnt = 0;
    #if AHT20_CHECK_EN
    extern volatile int AHT20_Temperature;
    #endif // AHT20_CHECK_EN
    if(is_test_mode)
    {
        if(disp_cnt >= 10)//200ms
        {
            disp_cnt = 0;
            //printf("curr is test mode!!!\n");
            test_mode_ret = polyc_TM1628_disp_test();
            if(test_mode_ret == 0)
            {
                is_test_mode = 0;//结束
                #if AHT20_CHECK_EN
                TM1628_disp_num((u16)AHT20_Temperature);
                #else
                TM1628_disp_num(888);
                #endif // AHT20_CHECK_EN

                TM1628_disp_icon(DISP_GROUP3,TEMPERATURE_SYMBOL,1);
            }
        }
        disp_cnt++;
    }else
    {

        if(disp_cnt >= 100)//200ms
        {
            disp_cnt = 0;
            #if AHT20_CHECK_EN
            TM1628_disp_num((u16)AHT20_Temperature);
            #else
            TM1628_disp_num(888);
            #endif // AHT20_CHECK_EN
            TM1628_disp_icon(DISP_GROUP3,TEMPERATURE_SYMBOL,1);
        }
        disp_cnt++;
    }
#endif // IC_1628_EN

#if AHT20_CHECK_EN//温湿度传感器
    static u16 AHT20_check_cnt = 0;///检测时间
    if(AHT20_check_cnt>= 100)
    {
        AHT20_CT_Measure_Callbackfunc();
        AHT20_check_cnt = 0;
    }else
    {
        AHT20_check_cnt++;
    }

#endif // AHT20_CHECK_EN

#if	MCU_ADC_SCAN_ENABLE  ///adc值debug - demo
    extern u8 printf_adc_value();
    static u16 printf_adc_cnt = 0 ,printf_adc_cnt1 = 0;///检测时间
    static u8 is_on = 0;
    if(printf_adc_cnt>= 10)
    {
        printf_adc_value();
        printf_adc_cnt = 0;

    }else
    {
        printf_adc_cnt++;
    }
#endif // MCU_ADC_SCAN_ENABLE

#if   PULSE_CNT_EN
    pulse_scan();
#endif // PULSE_CNT_EN

/***************红外按键扫描 demo********************/
    static u8 ir_key = 0;
    ir_key = ir_get_value();
    if(ir_key == 0xff)
    {
        putchar('f');
    }else
    {
        printf("ir_key[0x%x]",ir_key);
    }
/*************************************/


}



void set_led_disp_frequency(u16 fre)
{
    led_disp_frequency = fre;
    printf("led_disp_frequency[%d]",led_disp_frequency);
}
