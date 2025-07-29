#ifndef __USER_MCU_SPI_H__
#define __USER_MCU_SPI_H__




#define MCU_SPI1_EN          0

#define MCU_SPI2_EN          0


u8 get_spi_init_ok_flag();
void virtual_mcu_digital_rgb_led_disp_init(void);
void virtual_mcu_digital_rgb_led_disp_func_callback(void);
#endif
