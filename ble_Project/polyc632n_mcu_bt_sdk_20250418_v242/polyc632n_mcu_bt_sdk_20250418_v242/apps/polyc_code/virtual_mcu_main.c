#include "system/includes.h"
#include "driver/app_config.h"
#include "virtual_mcu_main.h"
#include "virtual_mcu_bt_communication.h"
#include "user_mcu_demo.h"

static u16 mcu_fast_isr_time;
static u16 mcu_slow_isr_time;

//---------ע�⣺ע�⣺ע�⣺�˺���ֻ�����ײ㹦��ģ�������ʹ�ܣ�ֻ����ӱ�����ʼ����Ӳ��ģ�黹û��ʼ�����������Ӳ����غ������ã����������-----------
void polyc_config_bt_system(void)                             //�ײ����
{
    //-----------------����������BLE������-----------------
    char local_ble_name[29] = "POL632n";                            //�����������֣��͸Ĵ��ַ���

	memset(virtual_mcu_local_ble_name,0x00,sizeof(virtual_mcu_local_ble_name));
	memcpy(virtual_mcu_local_ble_name,local_ble_name,strlen(local_ble_name)); //���ñ���ble���֣����Ϊ29���ֽ�


#ifdef CONFIG_DEBUG_ENABLE
	//-----------------ʹ�ܵ��Դ�ӡ�ںʹ�ӡ������-----------------
	virtual_mcu_debug_tx_pin = IO_PORT_DP;             //�����õĴ���UART0��ӡ�ӿ�
	virtual_mcu_debug_baudrate = 460800;                //�����õĴ���UART0������
#else
    virtual_mcu_debug_tx_pin = 0xf8;                    //�ײ�رմ�ӡ���мǲ�Ҫɾ�����Ĵ����ã�����һ��Ҫ�رմ�ӡ����ʡ��Դ
	virtual_mcu_debug_baudrate = 0xfff8;                //�ײ�رմ�ӡ���мǲ�Ҫɾ�����Ĵ����ã�����һ��Ҫ�رմ�ӡ����ʡ��Դ
#endif

	mcu_fast_isr_time = 1000;                           //1ms�����ж�
    mcu_slow_isr_time = 20;                             //20ms�����жϣ�������main.c���ú���

    mcu_user_system_init();
}


///-----------��Ҫ������mcu�Ŀ����ж϶�ʱ��������1ms�ɸ�������������-----------
///-----------ע�⣺1���˺���������while��������for����ѭ������----------------
///-----------ע�⣺2���˺����в�����delay����----------------------------------
AT_VOLATILE_RAM_CODE
void fast_period_timer_callback(void)      ///1ms
{
    mcu_user_timer_scan();
}



///------------------��Ҫ������mcu��main������20ms�ɸ�������������-------------
///-----------ע�⣺1���˺���������while��������for����ѭ������----------------
///-----------ע�⣺2���˺�����delay��ʱ�䲻�ܳ������жϵ�ʱ��-----------------*/
AT_VOLATILE_RAM_CODE
void slow_period_timer_callback(void)     ///20ms
{
    static u8 cnt = 0;
    cnt++;
    if(cnt >= 10)     //����200ms���ٵ���,�ȴ�ϵͳ�������ȶ���ִ��
    {
        cnt = 30;
        mcu_user_main();
    }
}




///----------------MCU��ʼ���ӿں�ϵͳ���к���------------------
void polyc_init(void)   //�ײ����
{
    printf("************************Hello Virtual MCU 01 -> init \n");

    mcu_user_init();

    polyc_fast_period_timer_init(mcu_fast_isr_time);                          ///��ʼ��1ms��ʱ�ж�   //��Ͳ��ܵ���200us
    polyc_fast_period_timer_register_callback(fast_period_timer_callback);    ///ע���ж��е��õ�Ӧ�ò㺯��

    polyc_lib_timer_init(mcu_slow_isr_time);                                  ///��ʼ��20ms��ʱ�ж�
    polyc_lib_timer_register_callback(slow_period_timer_callback);            ///ע���ж��е��õ�Ӧ�ò㺯��
}

