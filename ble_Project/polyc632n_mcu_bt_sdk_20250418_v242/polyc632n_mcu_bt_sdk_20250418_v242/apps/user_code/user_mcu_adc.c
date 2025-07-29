#include "system/timer.h"
#include "virtual_mcu_main.h"
#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_adc.h"
#include "user_mcu_adc.h"

#if MCU_ADC_SCAN_ENABLE
static u16 adc_value_buf[8] = {0x00};
u8 printf_adc_value()
{
    printf("printf_adc_value:[0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x]",adc_value_buf[0],adc_value_buf[1],adc_value_buf[2],adc_value_buf[3],adc_value_buf[4],adc_value_buf[5],adc_value_buf[6],adc_value_buf[7]);
}
//---------------------------ADC1扫描处理-------------------------
void mcu_adc_deal_1(u16 value)    //10位ADC  0-1023
{
	///ADC数据处理函数
    ///printf("adc value1 = %d\n",value);
    adc_value_buf[0] = value;
}
//---------------------------ADC2扫描处理-------------------------
void mcu_adc_deal_2(u16 value)   //10位ADC  0-1023
{
    ///ADC数据处理函数
	///printf("adc value2 = %d\n",value);
	adc_value_buf[1] = value;
}
//---------------------------ADC3扫描处理-------------------------
void mcu_adc_deal_3(u16 value)   //10位ADC  0-1023
{
    ///ADC数据处理函数
	///printf("adc value3 = %d\n",value);
	adc_value_buf[2] = value;
}
//---------------------------ADC4扫描处理-------------------------
void mcu_adc_deal_4(u16 value)   //10位ADC  0-1023
{
    ///ADC数据处理函数
	///printf("adc value4 = %d\n",value);
	adc_value_buf[3] = value;
}
//---------------------------ADC5扫描处理-------------------------
void mcu_adc_deal_5(u16 value)   //10位ADC  0-1023
{
    ///ADC数据处理函数
	///printf("adc value5 = %d\n",value);
	adc_value_buf[4] = value;
}

//---------------------------ADC6扫描处理-------------------------
void mcu_adc_deal_6(u16 value)   //10位ADC  0-1023
{
    ///ADC数据处理函数
	///printf("adc value6 = %d\n",value);
	adc_value_buf[5] = value;
}

//---------------------------ADC7扫描处理-------------------------
void mcu_adc_deal_7(u16 value)   //10位ADC  0-1023
{
    ///ADC数据处理函数
	///printf("adc value7 = %d\n",value);
	adc_value_buf[6] = value;
}

//---------------------------ADC85扫描处理-------------------------
void mcu_adc_deal_8(u16 value)   //10位ADC  0-1023
{
    ///ADC数据处理函数
	///printf("adc value8 = %d\n",value);
	adc_value_buf[7] = value;
}


/*----------------------------------------------------------------------------*/
/**@brief  ADC初始化接口
   @return  void
   @note    void mcu_adc_init(void)
*/
/*----------------------------------------------------------------------------*/
void mcu_adc_init(void)
{
#if MCU_ADC_1_ENABLE
	polyc_adc_set_init(MCU_ADC1_IO);    //设置ADC采样接口
#endif

#if	MCU_ADC_2_ENABLE
	polyc_adc_set_init(MCU_ADC2_IO);    //设置ADC采样接口
#endif

#if	MCU_ADC_3_ENABLE
	polyc_adc_set_init(MCU_ADC3_IO);    //设置ADC采样接口
#endif

#if	MCU_ADC_4_ENABLE
	polyc_adc_set_init(MCU_ADC4_IO);    //设置ADC采样接口
#endif

#if	MCU_ADC_5_ENABLE
	polyc_adc_set_init(MCU_ADC5_IO);    //设置ADC采样接口
#endif

#if	MCU_ADC_6_ENABLE
	polyc_adc_set_init(MCU_ADC6_IO);    //设置ADC采样接口
#endif

#if	MCU_ADC_7_ENABLE
	polyc_adc_set_init(MCU_ADC7_IO);    //设置ADC采样接口
#endif

#if	MCU_ADC_8_ENABLE
	polyc_adc_set_init(MCU_ADC8_IO);    //设置ADC采样接口
#endif

}

/*----------------------------------------------------------------------------*/
/**@brief  ADC扫描
   @return  void
   @note    void mcu_adc_scan_deal(void)
*/
/*----------------------------------------------------------------------------*/
AT_VOLATILE_RAM_CODE
void mcu_adc_scan_deal(void)   //20ms扫描一次ADC，1个通道ADC转换完成时间5ms
{
	static u16 ad_value1,ad_value2,ad_value3,ad_value4,ad_value5,ad_value6,ad_value7,ad_value8;
    static u8 cnt;

    cnt++;
    if(cnt >= 20)
    {
        cnt = 0;
#if MCU_ADC_1_ENABLE
        ad_value1 = polyc_get_adc_value(MCU_ADC1_IO);   //ADC采集的值(10位ADC采样0-1023)
        mcu_adc_deal_1(ad_value1);
#endif

#if	MCU_ADC_2_ENABLE
        ad_value2 = polyc_get_adc_value(MCU_ADC2_IO);   //ADC采集的值(10位ADC采样0-1023)
        mcu_adc_deal_2(ad_value2);
#endif

#if	MCU_ADC_3_ENABLE
        ad_value3 = polyc_get_adc_value(MCU_ADC3_IO);   //ADC采集的值(10位ADC采样0-1023)
        mcu_adc_deal_3(ad_value3);
#endif

#if	MCU_ADC_4_ENABLE
        ad_value4 = polyc_get_adc_value(MCU_ADC4_IO);   //ADC采集的值(10位ADC采样0-1023)
        mcu_adc_deal_4(ad_value4);
#endif

#if	MCU_ADC_5_ENABLE
        ad_value5 = polyc_get_adc_value(MCU_ADC5_IO);   //ADC采集的值(10位ADC采样0-1023)
        mcu_adc_deal_5(ad_value5);
#endif

#if	MCU_ADC_6_ENABLE
        ad_value6 = polyc_get_adc_value(MCU_ADC6_IO);   //ADC采集的值(10位ADC采样0-1023)
        mcu_adc_deal_6(ad_value6);
#endif

#if	MCU_ADC_7_ENABLE
        ad_value7 = polyc_get_adc_value(MCU_ADC7_IO);   //ADC采集的值(10位ADC采样0-1023)
        mcu_adc_deal_7(ad_value7);
#endif

#if	MCU_ADC_8_ENABLE
        ad_value8 = polyc_get_adc_value(MCU_ADC8_IO);   //ADC采集的值(10位ADC采样0-1023)
        mcu_adc_deal_8(ad_value8);
#endif


    }
}

#endif




