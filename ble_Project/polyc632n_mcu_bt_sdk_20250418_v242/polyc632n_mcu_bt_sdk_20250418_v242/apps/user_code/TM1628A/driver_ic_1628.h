#ifndef _DRIVER_IC_1628_H_
#define _DRIVER_IC_1628_H_

#include "includes.h"
#include "app_config.h"
#include "polyc_iokey.h"

#define IC_1628_EN                      1


#if  IC_1628_EN


#define  IC_1628_DISP_GROUP                  7//一共有6组显示
#define  ONE_GROUP_BYTE                      2//单组显示一共有2个字节表示




#define  IC_1628_PORT_NUM  3

#define  IO_1628_CLK_PIN            IO_PORTA_03
#define  IO_1628_DATA_PIN           IO_PORTA_04
#define  IO_1628_STB_PIN            IO_PORTA_05


//显示模式命令设置：
#define	TM1628_SEL_4_13	  (u8)0x00    //4位13段
#define	TM1628_SEL_5_12	  (u8)0x01    //5位12段
#define	TM1628_SEL_6_11	  (u8)0x02    //6位11段
#define	TM1628_SEL_7_10	  (u8)0x03    //7位10段

//数据命令设置：
#define	TM1628_CMD_AUTO_WRITE_MODE    (u8)0x40  //自动地址写模式,写数据到显示寄存器
#define	TM1628_CMD_FIXED_WRITE_MODE   (u8)0x44  //固定地址写模式
#define	TM1628_CMD_AUTO_READ_MODE	  (u8)0x42  //自动地址增加,读键扫数据
#define	TM1628_CMD_FIXED_READ_MODE	  (u8)0x46  //固定地址,读键扫数据

//显示控制命令设置：
#define TM1628_BRIGHTNESS_SEL_1_16    (u8)0x88  //亮度选择1/16
#define TM1628_BRIGHTNESS_SEL_2_16    (u8)0x81  //亮度选择2/16
#define TM1628_BRIGHTNESS_SEL_4_16    (u8)0x82  //亮度选择4/16
#define TM1628_BRIGHTNESS_SEL_10_16   (u8)0x83  //亮度选择10/16
#define TM1628_BRIGHTNESS_SEL_11_16   (u8)0x8C  //亮度选择11/16
#define TM1628_BRIGHTNESS_SEL_12_16   (u8)0x8D  //亮度选择12/16
#define TM1628_BRIGHTNESS_SEL_13_16   (u8)0x8E  //亮度选择13/16
#define TM1628_BRIGHTNESS_SEL_14_16   (u8)0x8F  //亮度选择14/16
#define	TM1628_CMD_DISP_OFF           (u8)0x80  //显示关

//地址命令设置：
#define	TM1628_CMD_DISP_ADDR_0         (u8)0xC0 //用来设置显示寄存器的起始地址
#define	TM1628_CMD_DISP_ADDR_LED       (u8)0xC0+(3*2) //用来设置显示寄存器的起始地址 3表示 G4  *2是因为每个G组有高低2个字节
#define	TM1628_CMD_DISP_ADDR_D         (u8)0xCD //用来设置显示寄存器   共14字节


/*********G1 G2 G3*****************/
#define NUM0                BIT(0) | BIT(1) | BIT(2) | BIT(3) | BIT(4) | BIT(5)
#define NUM1                BIT(1) | BIT(2)
#define NUM2                BIT(0) | BIT(1) | BIT(3) | BIT(4) | BIT(6)
#define NUM3                BIT(0) | BIT(1) | BIT(2) | BIT(3) | BIT(6)
#define NUM4                BIT(1) | BIT(2) | BIT(5) | BIT(6)
#define NUM5                BIT(0) | BIT(2) | BIT(3) | BIT(5) | BIT(6)
#define NUM6                BIT(0) | BIT(2) | BIT(3) | BIT(4) | BIT(5) | BIT(6)
#define NUM7                BIT(0) | BIT(1) | BIT(2)
#define NUM8                BIT(0) | BIT(1) | BIT(2) | BIT(3) | BIT(4) | BIT(5) | BIT(6)
#define NUM9                BIT(0) | BIT(1) | BIT(2) | BIT(5) | BIT(6)

#define PIONT                           BIT(7)//G1  小数点
#define PLUS_SIGN                       BIT(7)//G2  加号
#define TEMPERATURE_SYMBOL              BIT(7)//G3  温度符号

/*********G4***********/
#define LED1        BIT(0)
#define LED2        BIT(1)
#define LED3        BIT(2)
#define LED4        BIT(3)
#define LED5        BIT(4)
#define LED6        BIT(5)
#define BELL        BIT(6)//响铃
#define PLUG_IN     BIT(7)//插电


/*********G5***********/
#define CHARGE                  BIT(0) //充电
#define ALARM_CLOCK             BIT(1) //闹钟
#define WATER_CIRCULATION       BIT(2)//水循环
#define DRAIN_AWAY_WATER        BIT(3)//排水
#define ANTENNAE                BIT(4)//天线
#define GREEN_LEAF              BIT(5)//绿叶
#define OXYGEN2                 BIT(6)//氧气
#define ROOM                    BIT(7)//屋子

/*********G6***********/
#define MAX                  BIT(0) //MAX
#define AUTO                 BIT(1) //AUTO
#define ECO                  BIT(2)//ECO
#define ZZZZ                 BIT(3)//ZZZZ
#define SUN                  BIT(4)//太阳
#define BLOWING_IN           BIT(5)//送风
#define WATER_DROP           BIT(6)//水滴
#define SNOWFLAKE            BIT(7)//雪花

/*********G7***********/
#define BUTTON_LED1     BIT(0)
#define BUTTON_LED2     BIT(1)
#define BUTTON_LED3     BIT(2)
#define BUTTON_LED4     BIT(3)



#define KEY_1628_NO_STATE    0xff

#define   KEY_1628_NO_VALUE         0x00
#define   KEY_1628_VALUE0           0x01
#define   KEY_1628_VALUE1           0x02
#define   KEY_1628_VALUE2           0x04
#define   KEY_1628_VALUE3           0x08

//每个组的显示对应的displaybuf数组下标
enum
{
    DISP_GROUP1 = 0,
    DISP_GROUP2 = 2,
    DISP_GROUP3 = 4,
    DISP_GROUP4 = 6,
    DISP_GROUP5 = 8,
    DISP_GROUP6 = 10,
    DISP_GROUP7 = 12,

};
extern u8 displaybuf[IC_1628_DISP_GROUP*ONE_GROUP_BYTE];

void init_1628_pin(u8 port,  u8 number_index);
void SET_1628_STB_PIN(u8 high);
void TM1628WriteData(u8 Dat);
void TM1628WriteCmd(u8 Cmd);
void SendBit_1628(u8 sdata, u8 cnt);
void SendCmd_1628(u8 command);
void Init_1628(void);

void TM1628_disp_num(u16 num);
void TM1628_disp_led(u8 led_index);
void TM1628_disp_icon(u8 group, u8 icon,u8 disp_e);
void TM1628_ALL_display_test(void);
u8 polyc_TM1628_disp_test(void);


u8 key_1628_scan(void);
void key_1628_value_filter(u8 key_value);
#endif /* IC_1628_EN*/
#endif /* #ifndef _DRIVER_IC_1628_H_ */

