#include "system/includes.h"
#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_spi.h"
#include "virtual_mcu_main.h"
#include "user_mcu_iic_soft.h"




const struct soft_iic_config soft_iic_cfg[] = {
    //iic0 data
    {
        .scl = TCFG_SW_I2C0_CLK_PORT,                   //IIC CLK��
        .sda = TCFG_SW_I2C0_DAT_PORT,                   //IIC DAT��
        .delay = 8,                //���IIC��ʱ������Ӱ��ͨѶʱ��Ƶ��
        .io_pu = 1,                                     //�Ƿ���������裬����ⲿ��·û�к�������������Ҫ��1
    },
};


