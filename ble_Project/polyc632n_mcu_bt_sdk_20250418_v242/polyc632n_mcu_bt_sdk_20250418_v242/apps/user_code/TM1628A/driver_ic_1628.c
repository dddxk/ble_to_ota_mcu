#include "driver_ic_1628.h"

#if   IC_1628_EN


extern void delay(unsigned int);
static u8 ic_1628_io_port[IC_1628_PORT_NUM];
u8 displaybuf[IC_1628_DISP_GROUP*ONE_GROUP_BYTE] = {0x00};
//=================================================================================//
//                        		1628驱动                    			        	   //
//=================================================================================//
void init_1628_pin(u8 port,  u8 number_index)
{
    if(number_index > IC_1628_PORT_NUM)
    {
        number_index = IC_1628_PORT_NUM;
    }

    ic_1628_io_port[number_index] =port;
    printf("ic_1628_io_port=%d\n",ic_1628_io_port[number_index]);
}

void SET_1628_CLK_PIN(u8 high)
{
    gpio_set_pull_up(ic_1628_io_port[0], 0);
    gpio_set_pull_down(ic_1628_io_port[0], 0);
    gpio_set_direction(ic_1628_io_port[0], 0);
    if(high){
        gpio_set_output_value(ic_1628_io_port[0],1);
    }else{
        gpio_set_output_value(ic_1628_io_port[0],0);
    }
}

void SET_1628_DATA_PIN(u8 high)
{
    gpio_set_pull_up(ic_1628_io_port[1], 0);
    gpio_set_pull_down(ic_1628_io_port[1], 0);
    gpio_set_direction(ic_1628_io_port[1], 0);
    if(high){
        gpio_set_output_value(ic_1628_io_port[1],1);
    }else{
        gpio_set_output_value(ic_1628_io_port[1],0);
    }
}

int GET_1628_DATA_PIN()
{
    gpio_set_pull_up(ic_1628_io_port[1], 1);
    gpio_set_pull_down(ic_1628_io_port[1], 0);
    gpio_set_direction(ic_1628_io_port[1], 1);
    gpio_set_die(ic_1628_io_port[1], 1);

    return gpio_read(ic_1628_io_port[1]);
}

void SET_1628_STB_PIN(u8 high)
{
    gpio_set_pull_up(ic_1628_io_port[2], 0);
    gpio_set_pull_down(ic_1628_io_port[2], 0);
    gpio_set_direction(ic_1628_io_port[2], 0);
    if(high){
        gpio_set_output_value(ic_1628_io_port[2],1);
    }else{
        gpio_set_output_value(ic_1628_io_port[2],0);
    }
}

void TM1628WriteData(u8 Dat)
{
    u8 i;
    for (i=0;i<8;i++)
	{
        delay(1);
        if(Dat & 0x01){
            SET_1628_DATA_PIN(1);
	  	}else{
	   		SET_1628_DATA_PIN(0);
	   	}
        delay(1);
        SET_1628_CLK_PIN(0);
        delay(1);
        Dat>>=1;
        SET_1628_CLK_PIN(1);
    }
    delay(1);
}

void TM1628readData(u8 *Dat)
{
    u8 i,j;
    //u8 Dat[5];

    for(j=0;j<5;j++)
    {
        for (i=0;i<8;i++)
        {
            SET_1628_CLK_PIN(0);
            delay(1);
            Dat[j]>>=1;
            delay(1);
            if(GET_1628_DATA_PIN())//if(gpio_read(IO_PORTB_06))//
            {
                Dat[j] |= 0x80;
            }
            delay(1);
            SET_1628_CLK_PIN(1);
            delay(1);
        }
        delay(1);
    }
    //return Dat;
}

void TM1628WriteCmd(u8 Cmd)
{
	SET_1628_STB_PIN(0);
	delay(5);
	TM1628WriteData(Cmd);
    SET_1628_STB_PIN(1);
    delay(5);
}

void SendBit_1628(u8 sdata, u8 cnt)
{
    unsigned char i;
    for(i=0;i<cnt;i++)
    {
        SET_1628_CLK_PIN(0);
        if(sdata&0x01){
            SET_1628_DATA_PIN(1);
        }else{
            SET_1628_DATA_PIN(0);
        }
        delay(1);
        SET_1628_CLK_PIN(1);
        delay(1);

        sdata>>=1;
    }
}
void SendCmd_1628(u8 command)
{
	SET_1628_STB_PIN(0);
	delay(5);
	SendBit_1628(command,8);
    SET_1628_STB_PIN(1);
	delay(5);
}

static void TM1628_ic_set_pin(void)
{
    init_1628_pin(IO_1628_CLK_PIN,0);
    init_1628_pin(IO_1628_DATA_PIN,1);
    init_1628_pin(IO_1628_STB_PIN,2);

}

void Init_1628(void)
{
    TM1628_ic_set_pin();

    SET_1628_CLK_PIN(0);
    SET_1628_DATA_PIN(0);
    SET_1628_STB_PIN(1);
    delay(50);
	SendCmd_1628(TM1628_SEL_7_10);
	SendCmd_1628(TM1628_CMD_FIXED_WRITE_MODE);
 	SendCmd_1628(TM1628_CMD_DISP_OFF);
 	sizeof(displaybuf,0x00,sizeof(displaybuf));
}


void TM1628_ALL_display_test(void)
{
    u8 i;
	memset(displaybuf, 0x00, sizeof(displaybuf));

    puts("\ndisplaybuf:");
    put_buf(displaybuf, sizeof(displaybuf));

    SendCmd_1628(TM1628_SEL_7_10); //设置显示模式:  7位10段
    TM1628WriteCmd(TM1628_CMD_AUTO_WRITE_MODE); //自动地址增加,写模式
    SET_1628_STB_PIN(0);
    TM1628WriteData(TM1628_CMD_DISP_ADDR_0);//设置显示寄存器的起始地址
    for(i=0; i<sizeof(displaybuf); i++){
        displaybuf[i] = 0xFF;
        TM1628WriteData(displaybuf[i]); //地址自增,连续写方式
    }
    SET_1628_STB_PIN(1);
    TM1628WriteCmd(TM1628_BRIGHTNESS_SEL_11_16); //传显示控制命令设置脉冲宽度
}

static const u8 disp_Num_check_buf[10] = {NUM0,NUM1,NUM2,NUM3,NUM4,NUM5,NUM6,NUM7,NUM8,NUM9};

u8 polyc_TM1628_disp_test(void)
{
    u8 i;
    u8 ret = 1;
    static u8 disp_index = 0;
    static u8 disp_bit_cnt = 0xff;
	memset(displaybuf, 0x00, sizeof(displaybuf));

    switch(disp_index)
    {
        case 0:
            if(disp_bit_cnt == 0xff)
            {
                disp_bit_cnt = 0;
            }else
            {
                disp_bit_cnt++;
            }
            displaybuf[0] |= BIT(disp_bit_cnt);
            if(disp_bit_cnt >= 7)
            {
                disp_bit_cnt = 0xff;
                disp_index++;
            }
            break;
        case 1:
            if(disp_bit_cnt == 0xff)
            {
                disp_bit_cnt = 0;
            }else
            {
                disp_bit_cnt++;
            }
            displaybuf[2] |= BIT(disp_bit_cnt);
            if(disp_bit_cnt >= 7)
            {
                disp_bit_cnt = 0xff;
                disp_index++;
            }
            break;
        case 2:
            if(disp_bit_cnt == 0xff)
            {
                disp_bit_cnt = 0;
            }else
            {
                disp_bit_cnt++;
            }
            displaybuf[4] |= BIT(disp_bit_cnt);
            if(disp_bit_cnt >= 7)
            {
                disp_bit_cnt = 0xff;
                disp_index++;
            }
            break;
        case 3:
            if(disp_bit_cnt == 0xff)
            {
                disp_bit_cnt = 0;
            }else
            {
                disp_bit_cnt++;
            }
            displaybuf[6] |= BIT(disp_bit_cnt);
            if(disp_bit_cnt >= 7)
            {
                disp_bit_cnt = 0xff;
                disp_index++;
            }
            break;
        case 4:
            if(disp_bit_cnt == 0xff)
            {
                disp_bit_cnt = 0;
            }else
            {
                disp_bit_cnt++;
            }
            displaybuf[8] |= BIT(disp_bit_cnt);
            if(disp_bit_cnt >= 7)
            {
                disp_bit_cnt = 0xff;
                disp_index++;
            }
            break;
        case 5:
            if(disp_bit_cnt == 0xff)
            {
                disp_bit_cnt = 0;
            }else
            {
                disp_bit_cnt++;
            }
            displaybuf[10] |= BIT(disp_bit_cnt);
            if(disp_bit_cnt >= 7)
            {
                disp_bit_cnt = 0xff;
                disp_index++;
            }
            break;
        case 6:
            if(disp_bit_cnt == 0xff)
            {
                disp_bit_cnt = 0;
            }else
            {
                disp_bit_cnt++;
            }
            displaybuf[12] |= BIT(disp_bit_cnt);
            if(disp_bit_cnt >= 7)
            {
                disp_bit_cnt = 0xff;
                disp_index++;
            }
            break;
        default:
            if(disp_bit_cnt == 0xff)
            {
                disp_bit_cnt = 0;
            }else
            {
                disp_bit_cnt++;

            }
            displaybuf[0] = 0xff;
            displaybuf[2] = 0xff;
            displaybuf[4] = 0xff;
            displaybuf[6] = 0xff;
            displaybuf[8] = 0xff;
            displaybuf[10] = 0xff;
            displaybuf[12] = 0xff;

            if(disp_bit_cnt >= 10)
            {
                memset(displaybuf, 0x00, sizeof(displaybuf));
                ret = 0;
            }

            break;

    }

    //put_buf(displaybuf, sizeof(displaybuf));

    TM1628WriteCmd(TM1628_CMD_AUTO_WRITE_MODE); //自动地址增加,写模式
    SET_1628_STB_PIN(0);
    TM1628WriteData(TM1628_CMD_DISP_ADDR_0);//设置显示寄存器的起始地址
    for(i=0; i<sizeof(displaybuf); i++){
        TM1628WriteData(displaybuf[i]); //地址自增,连续写方式
    }
    SET_1628_STB_PIN(1);
    TM1628WriteCmd(TM1628_BRIGHTNESS_SEL_11_16); //传显示控制命令设置脉冲宽度
    return ret;
}


void TM1628_disp_num(u16 num)
{
    u8 i;
    u16 disp_Num = num;
    //if(disp_Num< 999 )disp_Num++;
	//memset(displaybuf, 0x00, sizeof(displaybuf));
	static u8 last_num1 = 0,last_num2 = 0,last_num3 = 0;

    displaybuf[0] &= ~disp_Num_check_buf[last_num1];
    displaybuf[2] &= ~disp_Num_check_buf[last_num2];
    displaybuf[4] &= ~disp_Num_check_buf[last_num3];

    displaybuf[0] |= disp_Num_check_buf[(disp_Num / 100)];
    displaybuf[2] |= disp_Num_check_buf[((disp_Num % 100)/10)];
    displaybuf[4] |= disp_Num_check_buf[(disp_Num %10)];
    //displaybuf[0] |= PIONT;
    //put_buf(displaybuf, sizeof(displaybuf));
    last_num1 = disp_Num / 100;
    last_num2 = (disp_Num % 100)/10;
    last_num3 = (disp_Num %10);

    TM1628WriteCmd(TM1628_CMD_AUTO_WRITE_MODE); //自动地址增加,写模式
    SET_1628_STB_PIN(0);
    TM1628WriteData(TM1628_CMD_DISP_ADDR_0);//设置显示寄存器的起始地址
    for(i=0; i<sizeof(displaybuf); i++){
        TM1628WriteData(displaybuf[i]); //地址自增,连续写方式
    }
    SET_1628_STB_PIN(1);
    TM1628WriteCmd(TM1628_BRIGHTNESS_SEL_11_16); //传显示控制命令设置脉冲宽度

}



void TM1628_disp_icon(u8 group, u8 icon,u8 disp_en)
{
    u8 i;
    if(disp_en)
    {
        displaybuf[group] |= icon;
    }else
    {
        displaybuf[group] &= ~icon;
    }


    TM1628WriteCmd(TM1628_CMD_AUTO_WRITE_MODE); //自动地址增加,写模式
    SET_1628_STB_PIN(0);
    TM1628WriteData(TM1628_CMD_DISP_ADDR_0+group);//设置显示寄存器的起始地址
    for(i=0; i<ONE_GROUP_BYTE; i++){
        TM1628WriteData(displaybuf[group+i]); //地址自增,连续写方式
    }
    SET_1628_STB_PIN(1);
    TM1628WriteCmd(TM1628_BRIGHTNESS_SEL_11_16); //传显示控制命令设置脉冲宽度

}

u8 key_1628_scan(void)
{
    u8 ret = 0;
    u8 led_disp = 0;
    u8 key_value_buf[5] = {0};
    u8 key_value = 0;

    SET_1628_STB_PIN(0);

    TM1628WriteData(TM1628_CMD_AUTO_READ_MODE);

    TM1628readData(key_value_buf);

    SET_1628_STB_PIN(1);
    SET_1628_CLK_PIN(0);
    SET_1628_DATA_PIN(0);
    if(key_value_buf[0] & BIT(0))
    {
        key_value |= BIT(0);
    }
    if(key_value_buf[0] & BIT(3))
    {
        key_value |= BIT(1);
    }
    if(key_value_buf[1] & BIT(4))
    {
        key_value |= BIT(2);
    }
    if(key_value_buf[1] & BIT(1))
    {
        key_value |= BIT(3);
    }
    //printf("key_scan:\n");
    //put_buf(key_value_buf,5);
    key_1628_value_filter(key_value);
    return ret;
}

static void key_1628_to_deal_work(u8 key_value,u8 key_state)
{
    printf("key_1628_value[%x]key_state[%d]\n",key_value,key_state);
    static u8 key_value0_led_en = 0;
    static u8 key_value1_led_en = 0;
    static u8 key_value2_led_en = 0;
    static u8 key_value3_led_en = 0;
    switch(key_state)
    {
        case KEY_SHORT_UP:
            if(key_value == KEY_1628_VALUE0)
            {
                key_value0_led_en = !key_value0_led_en;
                TM1628_disp_icon(DISP_GROUP4,LED1,key_value0_led_en);
            }else if(key_value == KEY_1628_VALUE1)
            {
                key_value1_led_en = !key_value1_led_en;
                TM1628_disp_icon(DISP_GROUP4,LED2,key_value1_led_en);
            }else if(key_value == KEY_1628_VALUE2)
            {
                key_value2_led_en = !key_value2_led_en;
                TM1628_disp_icon(DISP_GROUP4,LED3,key_value2_led_en);
            }else if(key_value == KEY_1628_VALUE3)
            {
                key_value3_led_en = !key_value3_led_en;
                TM1628_disp_icon(DISP_GROUP4,LED4,key_value3_led_en);
            }
            break;
        case KEY_LONG_UP:
            break;
        case KEY_HOLD:
            if(key_value == KEY_1628_VALUE0)
            {

            }else if(key_value == KEY_1628_VALUE1)
            {

            }else if(key_value == KEY_1628_VALUE2)
            {

            }else if(key_value == KEY_1628_VALUE3)
            {

            }
            break;
        case KEY_LONG:
            if(key_value == KEY_1628_VALUE0)
            {

            }else if(key_value == KEY_1628_VALUE1)
            {

            }else if(key_value == KEY_1628_VALUE2)
            {

            }else if(key_value == KEY_1628_VALUE3)
            {

            }
            break;
    }
}
void key_1628_value_filter(u8 key_value)
{
	static u16 key_press_cnt;//按下去的检测
	static u8  key_release_cnt;//松手的检测
	static u8  back_key;//存储按下去的键值
	u8 key_state = KEY_1628_NO_STATE;//初始化键值的状态

    if(key_value != KEY_1628_NO_VALUE)//此时的值已经从主循环扫描而得到了
    {
        if( back_key != key_value)//快速变换按键的情况下，以最新的按键为主
        {
            back_key = key_value;
            key_press_cnt = 0;
        }
        key_press_cnt++;
        key_release_cnt = 0;//松手计数清零
        if(key_press_cnt == KEY_SHORT_CNT )//有些按键的作用只有短按，实际工作中具体分析
        {
            key_state = KEY_SHORT;
        }
        else if(key_press_cnt == KEY_LONG_CNT)//到达长按的阈值，1s
        {
            key_state = KEY_LONG;
            key_1628_to_deal_work(back_key,key_state);
        }
        else if(key_press_cnt >= KEY_HOLD_CNT)//到达了长按不放的阈值
        {
            key_press_cnt = KEY_LONG_CNT;//防止超过参数的最大值，同时防止长按继续触发
            key_state = KEY_HOLD;
            key_1628_to_deal_work(back_key,key_state);
        }
    }
    else
    {
        key_release_cnt++;//检测到松手电平了，不要马上判断说是松手了
        if(key_release_cnt >= KEY_RELEASE_CNT)//约30ms稳定的松手电平之后就判断为松手，
        {
            key_release_cnt = 0;
            if(key_press_cnt >= KEY_SHORT_CNT && key_press_cnt < KEY_LONG_CNT)//大于15ms小于1s判断为短按
            {
                key_state = KEY_SHORT_UP;
                key_1628_to_deal_work(back_key,key_state);
            }
            else if(key_press_cnt >= KEY_LONG_CNT)
            {
                key_state = KEY_LONG_UP;
                key_1628_to_deal_work(back_key,key_state);
            }
            key_press_cnt=0;//只有真正松手了才会清掉这个计数
        }
    }
}


#endif
