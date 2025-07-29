#include "system/includes.h"
#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_spi.h"
#include "virtual_mcu_main.h"
#include "user_mcu_spi.h"
#include "asm/spi.h"


const struct spi_platform_data spi1_p_data = {
	.port[0] = IO_PORTA_07,
	.port[1] = IO_PORTA_08,
	.port[2] = -1,
	.mode = POLYC_SPI_MODE_UNIDIR_1BIT,
	.clk  = POLYC_HW_SPI1_BAUD,
	.role = POLYC_SPI_ROLE_MASTER,
};

const struct spi_platform_data spi2_p_data = {
	.port[0] = IO_PORT_DP1,
	.port[1] = IO_PORT_DM1,
	.port[2] = -1,
	.mode = POLYC_SPI_MODE_UNIDIR_1BIT,
	.clk  = POLYC_HW_SPI2_BAUD,
	.role = POLYC_SPI_ROLE_MASTER,
};


static u8 spi_init_ok_flag = 0;
u8 get_spi_init_ok_flag()
{
    return spi_init_ok_flag;
}
void polyc_spi_test()
{
    printf("/**********polyc_spi_test**********/");
    int i;
    int err;

    #if MCU_SPI1_EN
    polyc_spi_open(SPI1_HDL);
    #endif // MCU_SPI1_EN
    #if MCU_SPI2_EN
    polyc_spi_open(SPI2_HDL);
    #endif // MCU_SPI1_EN
    spi_init_ok_flag = 1;
    printf("/**********spi_close**********/");
}


void led_spi_rgb_to_24byte(u8 r, u8 g, u8 b, u8 *buf, int idx)
{
    buf = buf + idx * 24;
    u32 dat = ((g << 16) | (r << 8) | b);
    for (u8 i = 0; i < 24; i ++) {
        if (dat & BIT(23 - i)) {
            *(buf + i) = 0x7c;
        } else {
            *(buf + i) = 0x60;
        }
    }
}
#define LED_NUM_MAX     30
#define LED_NUM_MARGIN  9//多9个余量 方便rgb颜色排列控制

static u8 spi_dat_buf[24 * (LED_NUM_MAX+LED_NUM_MARGIN)] __attribute__((aligned(4)));
#define BREATH_CYCLE_NUM       126
static u8 virtual_mcu_digital_rgb_led_stop_flag = 0;
static u8 virtual_mcu_waterfall_cnt = 0;
static u16 g_digital_rgb_led_disp_timer = NULL;
const u8 red_or_green_breath_cycle_buff[BREATH_CYCLE_NUM] =
{
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x02, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x0A, 0x0B, 0x0C, 0x0E,
    0x0F, 0x10, 0x12, 0x13, 0x14,
    0x15, 0x17, 0x19, 0x1A, 0x1C,
    0x1D, 0x1F, 0x20, 0x22, 0x24,
    0x26, 0x27, 0x29, 0x2B, 0x2D,
    0x2E, 0x30, 0x32, 0x34, 0x36,
    0x38, 0x3A, 0x3C, 0x3E, 0x40,
    0x42, 0x44, 0x46, 0x48, 0x4A,
    0x4D, 0x4F, 0x51, 0x53, 0x55,
    0x58, 0x5A, 0x5C, 0x5F, 0x61, 0x64,
    0x61, 0x5F, 0x5C, 0x5A, 0x58,
    0x55, 0x53, 0x51, 0x4F, 0x4D,
    0x4A, 0x48, 0x46, 0x44, 0x42,
    0x40, 0x3E, 0x3C, 0x3A, 0x38,
    0x36, 0x34, 0x32, 0x30, 0x2E,
    0x2D, 0x2B, 0x29, 0x27, 0x26,
    0x24, 0x22, 0x20, 0x1F, 0x1D,
    0x1C, 0x1A, 0x19, 0x17, 0x15,
    0x14, 0x13, 0x12, 0x10, 0x0F,
    0x0E, 0x0C, 0x0B, 0x0A, 0x09,
    0x08, 0x07, 0x06, 0x05, 0x04,
    0x03, 0x02, 0x02, 0x01, 0x01
};

const u8 yellow_breath_cycle_buff[BREATH_CYCLE_NUM] =
{
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01,
    0x01, 0x02, 0x02, 0x03, 0x03,
    0x04, 0x04, 0x05, 0x06, 0x07,
    0x07, 0x08, 0x09, 0x09, 0x0A,
    0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x10, 0x11, 0x12, 0x13,
    0x14, 0x15, 0x16, 0x17, 0x18,
    0x19, 0x1B, 0x1C, 0x1D, 0x1E,
    0x1F, 0x20, 0x21, 0x23, 0x24,
    0x25, 0x26, 0x28, 0x29, 0x2A,
    0x2B, 0x2C, 0x2E, 0x2F, 0x30,
    0x32, 0x33, 0x34, 0x36, 0x37,
    0x39, 0x3A, 0x3C, 0x3A, 0x39,
    0x37, 0x36, 0x34, 0x33, 0x32,
    0x30, 0x2F, 0x2E, 0x2C, 0x2B,
    0x2A, 0x29, 0x28, 0x26, 0x25,
    0x24, 0x23, 0x21, 0x20, 0x1F,
    0x1E, 0x1D, 0x1C, 0x1B, 0x19,
    0x18, 0x17, 0x16, 0x15, 0x14,
    0x13, 0x12, 0x11, 0x10, 0x10,
    0x0F, 0x0E, 0x0D, 0x0C, 0x0B,
    0x0A, 0x09, 0x09, 0x08, 0x07,
    0x07, 0x06, 0x05, 0x04, 0x04,
    0x03, 0x03, 0x02, 0x02, 0x01,
    0x01
};

void virtual_mcu_digital_rgb_led_disp_start_waterfall_reset(void)
{
    virtual_mcu_waterfall_cnt = 0;
    virtual_mcu_digital_rgb_led_stop_flag = 0;
    memset(spi_dat_buf,0x00,sizeof(spi_dat_buf));
}

static void virtual_mcu_digital_rgb_led_disp_all_off(void)
{
    for(u8 i= 0;i< LED_NUM_MAX;i++)
    {
        led_spi_rgb_to_24byte(0, 0, 0, spi_dat_buf, i);
    }

    polyc_spi_dma_send(SPI1_HDL,spi_dat_buf,LED_NUM_MAX);
    polyc_spi_dma_send(SPI2_HDL,spi_dat_buf,LED_NUM_MAX);

}
static u8 virtual_mcu_digital_rgb_led_disp_start_waterfall(void)
{
    static u8 virtual_mcu_waterfall_cnt = 0;
    u8 i = 0;

    if(virtual_mcu_waterfall_cnt > (LED_NUM_MAX+LED_NUM_MARGIN))
    {
        virtual_mcu_waterfall_cnt = 1;
//        virtual_mcu_waterfall_cnt++;
//        if(virtual_mcu_waterfall_cnt>133)
//        {
//            virtual_mcu_waterfall_cnt = 0;
//        }
//        return 0;
    }

    if(virtual_mcu_waterfall_cnt == 0)
    {
        virtual_mcu_digital_rgb_led_disp_all_off();
    }
    else if(virtual_mcu_waterfall_cnt <= 3)
    {
        for(i = 0;i < virtual_mcu_waterfall_cnt;i++)
        {
            led_spi_rgb_to_24byte(100, 0, 0, spi_dat_buf, i);
        }
    }
    else if(virtual_mcu_waterfall_cnt <= 6)
    {
        for(i = 0;i < virtual_mcu_waterfall_cnt-3;i++)
        {
            led_spi_rgb_to_24byte(0, 100, 0, spi_dat_buf, i);
        }

        for(i = virtual_mcu_waterfall_cnt - 3;i < virtual_mcu_waterfall_cnt;i++)
        {
            led_spi_rgb_to_24byte(100, 0, 0, spi_dat_buf, i);
        }
    }
    else if(virtual_mcu_waterfall_cnt <= 9)
    {
        for(i = 0;i < virtual_mcu_waterfall_cnt-6;i++)
        {
            led_spi_rgb_to_24byte(0, 0, 100, spi_dat_buf, i);
        }
        for(i = virtual_mcu_waterfall_cnt - 6;i < virtual_mcu_waterfall_cnt-3;i++)
        {
            led_spi_rgb_to_24byte(0, 100, 0, spi_dat_buf, i);
        }
        for(i = virtual_mcu_waterfall_cnt - 3;i < virtual_mcu_waterfall_cnt;i++)
        {
            led_spi_rgb_to_24byte(100, 0, 0, spi_dat_buf, i);
        }
    }
    else {
        for(i = 0;i < virtual_mcu_waterfall_cnt-9;i++)
        {
            wdt_clear();
            led_spi_rgb_to_24byte(0, 0, 0, spi_dat_buf, i);
        }

        led_spi_rgb_to_24byte(0, 0, 100, spi_dat_buf, virtual_mcu_waterfall_cnt-9);
        led_spi_rgb_to_24byte(0, 0, 100, spi_dat_buf, virtual_mcu_waterfall_cnt-8);
        led_spi_rgb_to_24byte(0, 0, 100, spi_dat_buf, virtual_mcu_waterfall_cnt-7);
        led_spi_rgb_to_24byte(0, 100, 0, spi_dat_buf, virtual_mcu_waterfall_cnt-6);
        led_spi_rgb_to_24byte(0, 100, 0, spi_dat_buf, virtual_mcu_waterfall_cnt-5);
        led_spi_rgb_to_24byte(0, 100, 0, spi_dat_buf, virtual_mcu_waterfall_cnt-4);
        led_spi_rgb_to_24byte(100, 0, 0, spi_dat_buf, virtual_mcu_waterfall_cnt-3);
        led_spi_rgb_to_24byte(100, 0, 0, spi_dat_buf, virtual_mcu_waterfall_cnt-2);
        led_spi_rgb_to_24byte(100, 0, 0, spi_dat_buf, virtual_mcu_waterfall_cnt-1);
    }

    virtual_mcu_waterfall_cnt++;

    //led_spi_send_rgbbuf_isr(spi_dat_buf, 33);
    //put_buf(spi_dat_buf,32);
    int err;
    #if MCU_SPI1_EN
    err = polyc_spi_dma_send(SPI1_HDL,spi_dat_buf,24*LED_NUM_MAX);
    //printf("spi1 send len[%d]",err);
    #endif // MCU_SPI1_EN
    #if MCU_SPI2_EN
    err = polyc_spi_dma_send(SPI2_HDL,spi_dat_buf,24*LED_NUM_MAX);
    //printf("spi1 send len[%d]",err);
    #endif // MCU_SPI2_EN
    return 1;
}
static void virtual_mcu_digital_rgb_led_disp_resistance_level(u8 level)
{
    static u8 cnt  = 0;
    u8 i = 0;

    if(level <= 25) {
        for(i = 0;i < LED_NUM_MAX;i++)
        {
            led_spi_rgb_to_24byte(0, red_or_green_breath_cycle_buff[cnt], 0, spi_dat_buf, i);
        }
    }
    else if(level <= 50) {
        for(u8 i = 0;i < LED_NUM_MAX;i++)
        {
            led_spi_rgb_to_24byte(yellow_breath_cycle_buff[cnt], yellow_breath_cycle_buff[cnt], 0, spi_dat_buf, i);
        }
    }
    else if(level <= 75){
        for(i = 0;i < LED_NUM_MAX;i++)
        {
            led_spi_rgb_to_24byte(red_or_green_breath_cycle_buff[cnt], 0, 0, spi_dat_buf, i);
        }
    }else {
        for(i = 0;i < LED_NUM_MAX;i++)
        {
            led_spi_rgb_to_24byte(red_or_green_breath_cycle_buff[cnt], 0, red_or_green_breath_cycle_buff[cnt], spi_dat_buf, i);
        }
    }
    #if MCU_SPI1_EN
    polyc_spi_dma_send(SPI1_HDL,spi_dat_buf,24*LED_NUM_MAX);
    #endif // MCU_SPI1_EN
    #if MCU_SPI2_EN
    polyc_spi_dma_send(SPI2_HDL,spi_dat_buf,24*LED_NUM_MAX);
    #endif // MCU_SPI2_EN
    cnt++;
    if(cnt >= BREATH_CYCLE_NUM) { cnt = 0; }
}
void virtual_mcu_digital_rgb_led_disp_func_callback(void)
{
    if(get_spi_init_ok_flag()==0)return;

    u8 waterfall_run_flag = 0;
    static u8 level = 0 ;
    if(virtual_mcu_digital_rgb_led_stop_flag == 0){
        waterfall_run_flag = virtual_mcu_digital_rgb_led_disp_start_waterfall();
        if(waterfall_run_flag == 0){
            level++;
            if(level >=100)
                level = 0;
            virtual_mcu_digital_rgb_led_disp_resistance_level(level);
        }
    }
    else if(virtual_mcu_digital_rgb_led_stop_flag == 1){
        virtual_mcu_digital_rgb_led_stop_flag = 2;
        virtual_mcu_digital_rgb_led_disp_all_off();
    }
}

void virtual_mcu_digital_rgb_led_disp_init(void)
{
    printf("\nvirtual_mcu_digital_rgb_led_disp_init\n");
    if(g_digital_rgb_led_disp_timer == NULL){

        polyc_spi_test();

        //g_digital_rgb_led_disp_timer = sys_s_hi_timer_add(NULL,virtual_mcu_digital_rgb_led_disp_func_callback,100);
    }
    virtual_mcu_digital_rgb_led_disp_start_waterfall_reset();
}
