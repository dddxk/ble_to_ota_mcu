#include "virtual_mcu_main.h"
#include "virtual_mcu_charge.h"
#include "user_mcu_charge.h"



void mcu_charge_init(void)
{
    #if MCU_CHARGE_EN
    polyc_charge_en(1);
    polyc_set_charge_full_v(CHARGE_FULL_V_4222);//默认值CHARGE_FULL_V_4222
    polyc_set_charge_full_mA(CHARGE_FULL_mA_10);//默认值CHARGE_FULL_mA_10
    polyc_set_charge_mA(CHARGE_mA_60);//默认值CHARGE_mA_50

    #endif // MCU_CHARGE_EN

}


void mcu_charge_check_test()
{
    #if MCU_CHARGE_EN

    static u8 cnt = 0;
    if(++cnt == 50)
    {
        printf("online_flag[%d]\n",polyc_get_charge_online_flag());
        printf("full_flag[%d]\n",polyc_get_charge_full_flag());
        printf("vbat_value[%d]\n",polyc_get_vbat_value());
        cnt = 0;
    }

    #endif // MCU_CHARGE_EN
}
