
#ifndef __USER_MCU_ADC_H__
#define __USER_MCU_ADC_H__


/*----------------------------ADC使能模块---------------------------------
    模块接口ADC有11组，分别为IO_PORTA_01，IO_PORTA_03，IO_PORTA_05，
    IO_PORTA_07，IO_PORTA_08，IO_PORTA_09，IO_PORTB_01，IO_PORTB_04，
    IO_PORTB_06，IO_PORT_DP1，IO_PORT_DM1。
    下面只是添加5个样例，可根据需求再添加相应组别。
------------------------------------------------------------------------*/
#define MCU_ADC_SCAN_ENABLE             1         ///ADC使能


#if MCU_ADC_SCAN_ENABLE

    #define MCU_ADC_1_ENABLE               1      //使能ADC1
    #define MCU_ADC1_IO      IO_PORTA_01          //设置ADC采集口1

    #define MCU_ADC_2_ENABLE               1      //使能ADC2
    #define MCU_ADC2_IO      IO_PORT_DM1          //设置ADC采集口2

    #define MCU_ADC_3_ENABLE               1      //使能ADC3
    #define MCU_ADC3_IO      IO_PORTB_01          //设置ADC采集口3

    #define MCU_ADC_4_ENABLE               1      //使能ADC4
    #define MCU_ADC4_IO      IO_PORT_DP1          //设置ADC采集口4

    #define MCU_ADC_5_ENABLE               0      //使能ADC5
    #define MCU_ADC5_IO      IO_PORT_DP          //设置ADC采集口5

    #define MCU_ADC_6_ENABLE               0      //使能ADC6
    #define MCU_ADC6_IO      IO_PORT_DM          //设置ADC采集口6

    #define MCU_ADC_7_ENABLE               1      //使能ADC7
    #define MCU_ADC7_IO      IO_PORTA_08          //设置ADC采集口7

    #define MCU_ADC_8_ENABLE               1      //使能ADC8
    #define MCU_ADC8_IO      IO_PORTA_09          //设置ADC采集口8


    void mcu_adc_init(void);

    void mcu_adc_scan_deal(void);

#endif




#endif
