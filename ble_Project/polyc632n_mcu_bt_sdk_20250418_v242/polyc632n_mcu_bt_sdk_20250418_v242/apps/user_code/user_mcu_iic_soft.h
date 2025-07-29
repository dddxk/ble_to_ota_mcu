#ifndef __USER_MCU_IIC_SOFT_H__
#define __USER_MCU_IIC_SOFT_H__
#include "asm/iic_soft.h"




#define IIC0_DVE                            0
#define TCFG_SW_I2C0_CLK_PORT               IO_PORTB_02 //软件IIC  CLK脚选择
#define TCFG_SW_I2C0_DAT_PORT               IO_PORTB_03 //软件IIC  DAT脚选择
#define TCFG_SW_I2C0_DELAY_CNT              10


#define IIC1_DVE                            1
#define TCFG_SW_I2C1_CLK_PORT               -1 //软件IIC  CLK脚选择
#define TCFG_SW_I2C1_DAT_PORT               -1 //软件IIC  DAT脚选择
#define TCFG_SW_I2C1_DELAY_CNT              2


#define iic_init(iic)                       soft_iic_init(iic)
#define iic_uninit(iic)                     soft_iic_uninit(iic)
#define iic_start(iic)                      soft_iic_start(iic)
#define iic_stop(iic)                       soft_iic_stop(iic)
#define iic_tx_byte(iic, byte)              soft_iic_tx_byte(iic, byte)
#define iic_rx_byte(iic, ack)               soft_iic_rx_byte(iic, ack)
#define iic_read_buf(iic, buf, len)         soft_iic_read_buf(iic, buf, len)
#define iic_write_buf(iic, buf, len)        soft_iic_write_buf(iic, buf, len)
#define iic_suspend(iic)                    soft_iic_suspend(iic)
#define iic_resume(iic)                     soft_iic_resume(iic)


#endif
