#include "system/includes.h"
#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_spi.h"
#include "virtual_mcu_main.h"
#include "user_mcu_iic_soft.h"




const struct soft_iic_config soft_iic_cfg[] = {
    //iic0 data
    {
        .scl = TCFG_SW_I2C0_CLK_PORT,                   //IIC CLK脚
        .sda = TCFG_SW_I2C0_DAT_PORT,                   //IIC DAT脚
        .delay = 8,                //软件IIC延时参数，影响通讯时钟频率
        .io_pu = 1,                                     //是否打开上拉电阻，如果外部电路没有焊接上拉电阻需要置1
    },
};


