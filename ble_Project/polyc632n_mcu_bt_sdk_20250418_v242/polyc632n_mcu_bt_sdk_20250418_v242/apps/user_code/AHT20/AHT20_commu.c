#include "AHT20_commu.h"
#include "virtual_mcu_bt_communication.h"
#include "driver_ic_1628.h"



#if AHT20_CHECK_EN

volatile int AHT20_Temperature;
volatile int AHT20_Humidness;

static int CT_data[2];
static u16 g_ct_measure_timer = NULL;

#define iic_dev   IIC0_DVE

//读取AHT20的状态寄存器
u8 AHT20_Read_Status(void)
{
	u8 status,ack;

	iic_start(iic_dev);

	ack = iic_tx_byte(iic_dev,AHT20_ADDR_READ);

	status = iic_rx_byte(iic_dev,0);

	iic_stop(iic_dev);

	//printf("--> AHT20_status : 0x%02X\n",status);

	return status;
}

//查询cal enable位有没有使能
u8 AHT20_Read_Cal_Enable(void)
{
	u8 val = 0;
	u8 ret = 0;

	val = AHT20_Read_Status();

	if((val & 0x68) == 0x08) { ret = 1; }
	else  { ret = 0; }

	printf("--> %s [%d]\n",__func__,ret);

	return ret;
}

//向AHT20发送触发测量命令
void AHT20_SendAC(void)
{
	iic_start(iic_dev);
	iic_tx_byte(iic_dev,AHT20_ADDR_WRITE);
	iic_tx_byte(iic_dev,AHT20_START_MEASURE_CMD);//0xAC采集命令
	iic_tx_byte(iic_dev,0x33);
	iic_tx_byte(iic_dev,0x00);
	iic_stop(iic_dev);
}

//直接读取AHT20的温度和湿度数据
void AHT20_Read_CTdata(u32 *ct)
{
	u8 Byte_1th = 0;
	u8 Byte_2th = 0;
	u8 Byte_3th = 0;
	u8 Byte_4th = 0;
	u8 Byte_5th = 0;
	u8 Byte_6th = 0;
	u32 RetuData = 0;

	iic_start(iic_dev);
	iic_tx_byte(iic_dev,AHT20_ADDR_READ);
	Byte_1th = iic_rx_byte(iic_dev,1);//状态字
	Byte_2th = iic_rx_byte(iic_dev,1);//湿度
	Byte_3th = iic_rx_byte(iic_dev,1);//湿度
	Byte_4th = iic_rx_byte(iic_dev,1);//湿度/温度
	Byte_5th = iic_rx_byte(iic_dev,1);//温度
	Byte_6th = iic_rx_byte(iic_dev,0);//温度
	iic_stop(iic_dev);

	RetuData = (RetuData|Byte_2th)<<8;
	RetuData = (RetuData|Byte_3th)<<8;
	RetuData = (RetuData|Byte_4th);
	RetuData = RetuData >> 4;
	ct[0] = RetuData;//湿度
	RetuData = 0;
	RetuData = (RetuData|Byte_4th)<<8;
	RetuData = (RetuData|Byte_5th)<<8;
	RetuData = (RetuData|Byte_6th);
	RetuData = RetuData& 0x000fffff;
	ct[1] = RetuData;//温度
}

void AHT20_CT_Measure_Callbackfunc(void)
{
	static u8 try_cnt = 0;
	u8 status;

	status = AHT20_Read_Status();

	status &= 0x80;

	if(status == 0x80){
		try_cnt++;
		if(try_cnt > 6) { printf("--> AHT20 Exist Abnormal!\n"); }
	}
	else{
		AHT20_Read_CTdata(CT_data);

		AHT20_Humidness = (CT_data[0]*100*10) >> 20;
		AHT20_Temperature = (CT_data[1]*200*10 >> 20) - 500;

		//printf("\n--> AHT20_Humidness %d%\n",AHT20_Humidness);
//		printf("--> AHT20_Temperature %d%\n\n",AHT20_Temperature);

		AHT20_SendAC();

		try_cnt = 0;
	}
}

void AHT20_Init(void)
{
    printf("--> %s\n",__func__);
	iic_init(iic_dev);

	iic_start(iic_dev);
	iic_tx_byte(iic_dev,AHT20_ADDR_WRITE);
	iic_tx_byte(iic_dev,0xa8);
	iic_tx_byte(iic_dev,0x00);
	iic_tx_byte(iic_dev,0x00);
	iic_stop(iic_dev);

	delay(1000);//延时1ms左右

	iic_start(iic_dev);
	iic_tx_byte(iic_dev,AHT20_ADDR_WRITE);
	iic_tx_byte(iic_dev,AHT20_INIT_CMD);//0xBE初始化命令
	iic_tx_byte(iic_dev,0x08);
	iic_tx_byte(iic_dev,0x00);
	iic_stop(iic_dev);

	delay(1000);
	AHT20_Poweron_Self_Check();
}

void JH_Reset_REG(u8 addr)
{
	uint8_t Byte_first,Byte_second,Byte_third;

	iic_start(iic_dev);
	iic_tx_byte(iic_dev,AHT20_ADDR_WRITE);
	iic_tx_byte(iic_dev,addr);
	iic_tx_byte(iic_dev,0x00);
	iic_tx_byte(iic_dev,0x00);
	iic_stop(iic_dev);

	delay(500);//延时0.5ms左右

	iic_start(iic_dev);
	iic_tx_byte(iic_dev,AHT20_ADDR_READ);
	Byte_first = iic_rx_byte(iic_dev,1);
	Byte_second = iic_rx_byte(iic_dev,1);
	Byte_third = iic_rx_byte(iic_dev,0);
	iic_stop(iic_dev);

	delay(1000);//延时1ms左右

	iic_tx_byte(iic_dev,AHT20_ADDR_WRITE);
	iic_tx_byte(iic_dev,0xB0|addr);//寄存器命令
	iic_tx_byte(iic_dev,Byte_second);
	iic_tx_byte(iic_dev,Byte_third);
	iic_stop(iic_dev);

	Byte_second = 0x00;
	Byte_third  = 0x00;
}

void AHT20_Start_Init(void)
{
    printf("--> %s\n",__func__);

	JH_Reset_REG(0x1b);
	JH_Reset_REG(0x1c);
	JH_Reset_REG(0x1e);
}

void AHT20_Poweron_Self_Check(void)
{
	printf("--> %s\n",__func__);

	if((AHT20_Read_Status()&0x18) != 0x18)
	{
		AHT20_Start_Init(); //重新初始化寄存器
		delay(500);
	}

    AHT20_SendAC();
}

#endif
