
#ifndef __USER_MCU_ADC_H__
#define __USER_MCU_ADC_H__


/*----------------------------ADCʹ��ģ��---------------------------------
    ģ��ӿ�ADC��11�飬�ֱ�ΪIO_PORTA_01��IO_PORTA_03��IO_PORTA_05��
    IO_PORTA_07��IO_PORTA_08��IO_PORTA_09��IO_PORTB_01��IO_PORTB_04��
    IO_PORTB_06��IO_PORT_DP1��IO_PORT_DM1��
    ����ֻ�����5���������ɸ��������������Ӧ���
------------------------------------------------------------------------*/
#define MCU_ADC_SCAN_ENABLE             1         ///ADCʹ��


#if MCU_ADC_SCAN_ENABLE

    #define MCU_ADC_1_ENABLE               1      //ʹ��ADC1
    #define MCU_ADC1_IO      IO_PORTA_01          //����ADC�ɼ���1

    #define MCU_ADC_2_ENABLE               1      //ʹ��ADC2
    #define MCU_ADC2_IO      IO_PORT_DM1          //����ADC�ɼ���2

    #define MCU_ADC_3_ENABLE               1      //ʹ��ADC3
    #define MCU_ADC3_IO      IO_PORTB_01          //����ADC�ɼ���3

    #define MCU_ADC_4_ENABLE               1      //ʹ��ADC4
    #define MCU_ADC4_IO      IO_PORT_DP1          //����ADC�ɼ���4

    #define MCU_ADC_5_ENABLE               0      //ʹ��ADC5
    #define MCU_ADC5_IO      IO_PORT_DP          //����ADC�ɼ���5

    #define MCU_ADC_6_ENABLE               0      //ʹ��ADC6
    #define MCU_ADC6_IO      IO_PORT_DM          //����ADC�ɼ���6

    #define MCU_ADC_7_ENABLE               1      //ʹ��ADC7
    #define MCU_ADC7_IO      IO_PORTA_08          //����ADC�ɼ���7

    #define MCU_ADC_8_ENABLE               1      //ʹ��ADC8
    #define MCU_ADC8_IO      IO_PORTA_09          //����ADC�ɼ���8


    void mcu_adc_init(void);

    void mcu_adc_scan_deal(void);

#endif




#endif
