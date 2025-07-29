/* #include "asm/timer_cap.h" */
#include "app_config.h"
#include "typedef.h"
#include "asm/includes.h"
#include "user_mcu_timer_cap_433.h"

#if MCU_TIMER_CAP433_ENABLE


static const u32 TIMERx_table[6] = {
    (u32)JL_TIMER0,
    (u32)JL_TIMER1,
    (u32)JL_TIMER2,
    (u32)JL_TIMER3,
};

static const u8 TIMERx_IRQ_IDX[6] = {
    IRQ_TIME0_IDX,
    IRQ_TIME1_IDX,
    IRQ_TIME2_IDX,
    IRQ_TIME3_IDX,
};

static u8 tmrcap_edge[4] = {0};
static u8 allow_set_clock = ALLOW_SET_CLOCK_RATIO;
static void (*tmrcap_isr_cbfun[4])(u8 edge, u32 cnt) = {NULL};

u8 device_433_to_app_cnt = 0;
/*
 * @brief   timer中断服务函数
*/
___interrupt
void timer_cap_433_isr(void)
{

    u32 cur_cnt = 0;
    /* JL_USB_IO->CON0 |= BIT(1); */

    /* putchar('#'); */
    if ((JL_TIMER0->CON & BIT(1)) && (JL_TIMER0->CON & BIT(15))) {
        JL_TIMER0->CON |= BIT(14);
        cur_cnt = JL_TIMER0->PRD;
        JL_TIMER0->CNT = 0;
        /* putchar('0' + cur_cnt/400/10); */
        /* putchar('0' + cur_cnt/400%10); */
        /* putchar('\n'); */
        /* printf("%d", cur_cnt); */
        if (tmrcap_isr_cbfun[0]) {
            u8 cur_edge = !!(JL_TIMER0->CON & BIT(0));
            tmrcap_isr_cbfun[0](cur_edge, cur_cnt);
        }
        if (tmrcap_edge[0] > 1) {
            JL_TIMER0->CON ^= BIT(0);
        }
    }
}

/**
 * @param JL_TIMERx : JL_TIMER0/1/2/3
 * @param cap_io : JL_PORTA_01, JL_PORTB_02,,,等等，支持任意普通IO
 * @param edge : 捕捉的触发边沿，0：上升沿触发  1：下降沿触发  2：上下沿切换触发,第一个边沿是上升沿  3：上下沿切换触发,第一个边沿是下降沿
 * @param clk_div : 时钟源的分频选择，分频越小，输入捕获的计数值数得越快，变化量就越大。时钟源统一选择std_24M
 * @param cbfun : 捕获起中断的中断回调函数，传出边沿和计数值
 */
void timer_cap_433_init(JL_TIMER_TypeDef *JL_TIMERx, u32 cap_io, u8 edge, CLK_DIV_4bit clk_div, void (*cbfun)(u8, u32))
{
    u8 tmr_num;
    for (tmr_num = 0; tmr_num < 4; tmr_num ++) {
        if ((u32)JL_TIMERx == TIMERx_table[tmr_num]) {
            break;
        }
        if (tmr_num == 3) {
            return;
        }
    }
    if (allow_set_clock == 1) {
        u32 timer_clk = 24000000;
    } else {
        u32 timer_clk = __get_lrc_hz();
    }
    tmrcap_edge[tmr_num] = edge;
    tmrcap_isr_cbfun[tmr_num] = cbfun;
    request_irq(TIMERx_IRQ_IDX[tmr_num], 3, timer_cap_433_isr, 0);
    gpio_set_fun_input_port(cap_io, PFI_TMR0_CAP + 8 * tmr_num);
    //浮空输入
    gpio_set_die(cap_io, 1);
    gpio_set_direction(cap_io, 1);
    gpio_set_pull_up(cap_io, 0);
    gpio_set_pull_down(cap_io, 0);
    //初始化timer
    JL_TIMERx->CON = 0;
    if (allow_set_clock == 1) {
        JL_TIMERx->CON |= (6 << 10);                    //时钟源选择STD_24M
    } else {
        JL_TIMERx->CON |= (3 << 10);				    //时钟源选择lrc
    }
    JL_TIMERx->CON |= ((clk_div & 0xf) << 4);			//设置时钟源分频
    JL_TIMERx->CNT = 0;								    //清计数值
    JL_TIMERx->CON |= BIT(1);                           //输入捕获模式
    JL_TIMERx->CON += !!edge;
    if (edge == 3) {
        JL_TIMERx->CON &= ~BIT(0);
    }
}

/**
 * @param JL_TIMERx : JL_TIMER0/1/2/3/
 */
void close_433_timer_cap(JL_TIMER_TypeDef *JL_TIMERx)
{
    JL_TIMERx->CON &= ~BIT(0);
}

/************************************ 以下SDK的参考示例 ****************************************/

static u8 reject_data_flag, record_edge, allow_return, event_type;
static u8 buffer_433[BUF_NUM], buffer_433_send[BUF_NUM], buffer_433_last[BUF_NUM];
static u16 wait_to_reject_data;
static u8 return_433_is_allow_pair = 1;//0---不允许配对,1---允许配对

u8  pair_code_433[1 + PAIR_CODE_NUMBER];

static u8 cur_pair[PAIR_CODE_NUMBER + 1];

void set_433_pair_code(u8 *pair_code)
{
    printf("set 433 pair code is");

    pair_code_433[0] = 0xfe;
    cur_pair[0] = 0xfe;
    memcpy(&pair_code_433[1], pair_code, PAIR_CODE_NUMBER);
    memcpy(&cur_pair[1], pair_code, PAIR_CODE_NUMBER);
    return_433_is_allow_pair = 0;
}

u8 *get_433_pair_code(void)
{

    return cur_pair;
}

static u8  key_state = CAP433_KEY_NO_STATE;//初始化键值的状态
static u16 key_press_cnt;//按下去的检测
static u8  key_release_cnt;//松手的检测
static u8  back_key;//存储按下去的键值


#if  0
AT_VOLATILE_RAM_CODE
static void wait_return(u8 key_value)
{
    printf("wait_return");
    wait_to_reject_data = 0;
    allow_return = 0;
}
#endif // 0

AT_VOLATILE_RAM_CODE
void cap433_key_value_filter(u8 key_value)
{
    static u16 led_disp_fre = 1;

    if(key_value != CAP433_KEY_NO_VALUE)//此时的值已经从主循环扫描而得到了
    {
        if( back_key != key_value)//快速变换按键的情况下，以最新的按键为主
        {
            back_key = key_value;
            key_press_cnt = 0;
        }
        key_press_cnt++;
        key_release_cnt = 0;//松手计数清零
        if(key_press_cnt == CAP433_KEY_SHORT_CNT )//有些按键的作用只有短按，实际工作中具体分析
        {
            key_state = CAP433_KEY_SHORT;
            //printf("KEY_SHORT");
        }
        else if(key_press_cnt == CAP433_KEY_LONG_CNT)//到达长按的阈值，
        {
            key_state = CAP433_KEY_LONG;
            printf("KEY_LONG");
        }
        else if(key_press_cnt >= CAP433_KEY_HOLD_CNT)//到达了长按不放的阈值
        {
            key_press_cnt = CAP433_KEY_LONG_CNT;//防止超过参数的最大值，同时防止长按继续触发
            key_state = CAP433_KEY_HOLD;
        }
    }
    else
    {
        if(back_key == CAP433_KEY_NO_VALUE)return;
        key_release_cnt++;//检测到松手电平了，不要马上判断说是松手了
        if(key_release_cnt >= CAP433_KEY_RELEASE_CNT)//一段时间稳定后的松手电平之后就判断为松手，
        {
            key_release_cnt = 0;
            if(key_press_cnt >= CAP433_KEY_SHORT_CNT && key_press_cnt < CAP433_KEY_LONG_CNT)//大于CAP433_KEY_LONG_CNT小于CAP433_KEY_SHORT_CNT判断为短按
            {
                key_state = CAP433_KEY_SHORT_UP;
                //key_to_deal_work(back_key,key_state);
                printf("KEY_SHORT_UP");
                printf("key_value[%d]",back_key);
                if(back_key == 2)
                {
                    if(led_disp_fre<20)
                    {
                        led_disp_fre+=1;
                        set_led_disp_frequency(led_disp_fre);
                    }
                }else if(back_key == 8)
                {
                    if(led_disp_fre >= 1)
                    {
                        led_disp_fre-=1;
                        set_led_disp_frequency(led_disp_fre);
                    }
                }
            }
            else if(key_press_cnt >= CAP433_KEY_LONG_CNT)
            {
                key_state = CAP433_KEY_LONG_UP;
                printf("KEY_LONG_UP");
            }
            back_key = CAP433_KEY_NO_VALUE;
            key_press_cnt = 0;//只有真正松手了才会清掉这个计数
        }
    }
}
AT_VOLATILE_RAM_CODE
static void user_tmr3_cap_cbfun(u8 edge, u32 cnt)
{
    //[传参型处理]
    //[start reject data]
    //putchar('a');
    u8 key_value = 0;
    static u16 no_data_cnt = 3*CAP433_KEY_RELEASE_CNT;
    if (reject_data_flag == 1 && edge == 1) {
        //collect high level
        if ((cnt > (CNT_LOW_LEVEL / allow_set_clock - 3 * ALLOW_ERROR_VALUE / allow_set_clock)) && (cnt < (CNT_LOW_LEVEL / allow_set_clock + 3 * ALLOW_ERROR_VALUE / allow_set_clock))) {
            //按位存放数据,置0
            buffer_433[record_edge / 8] &= ~BIT(7 - record_edge % 8);
            record_edge++;
        } else if ((cnt > (CNT_HIGH_LEVEL / allow_set_clock - 6 * ALLOW_ERROR_VALUE / allow_set_clock)) && (cnt < (CNT_HIGH_LEVEL / allow_set_clock + 6 * ALLOW_ERROR_VALUE / allow_set_clock))) {
            //按位存放数据,置1
            buffer_433[record_edge / 8] |= BIT(7 - record_edge % 8);
            record_edge++;
        } else {
            //若接收过程数据错误,数据清空
            //putchar('X');
            //printf(":%d", cnt);
            reject_data_flag = 0;
            record_edge = 0;
            memcpy(buffer_433, 0, BUF_NUM);
            no_data_cnt = 20*CAP433_KEY_RELEASE_CNT;
        }

        if (record_edge == RECEPTION_NUMBER) {
            //put_buf(buffer_433,BUF_NUM);
            //校验前20位头码是否一致
            if (return_433_is_allow_pair && !((buffer_433[0] == 0 && buffer_433[1] == 0) || buffer_433[1] == 0)) {
                u8 pair_code_init[3];
                /*{buffer_433[0], buffer_433[1], (buffer_433[2] >> 4)}*/
                pair_code_init[0] = buffer_433[0];
                pair_code_init[1] = buffer_433[1];
                pair_code_init[2] = buffer_433[2] >> 4;
                set_433_pair_code(&pair_code_init);
            } else {
                u8 *pair_code = get_433_pair_code();
                if (pair_code[0] != 0xfe) {
                    /* printf("no save err"); */
                    putchar('R');
                    return;
                }

                if ((buffer_433[0] == pair_code[1]) && (buffer_433[1] == pair_code[2]) && ((buffer_433[2] >> 4) == pair_code[3])) {
                    //putchar('T');
                    if (RECEPTION_NUMBER % 8 != 0) {
                        //确保接收数据导致剩余位被填充数据，将这些剩余位清0
                        for (int i = 0; i < (8 - (RECEPTION_NUMBER % 8)); i++) {
                            buffer_433[BUF_NUM - 1] &= ~BIT(i);
                            /* put_buf(buffer_433, BUF_NUM); */
                        }
                    }
                    reject_data_flag = 0;
                    record_edge = 0;
                    memcpy(buffer_433_send, buffer_433, BUF_NUM);
                    key_value = (buffer_433[2] & 0x0f);
                    //printf("key_value[%d]",key_value);
                    cap433_key_value_filter(key_value);
                    no_data_cnt = 0;
                } else {
                    putchar('E');
                    //数据清空
                    reject_data_flag = 0;
                    record_edge = 0;
                    memcpy(buffer_433, 0, BUF_NUM);
                }
            }
        }else
        {
            no_data_cnt = 0;
        }
    }else
    {

        if(no_data_cnt < 3*CAP433_KEY_RELEASE_CNT)
        {
            no_data_cnt++;
            if(0 == no_data_cnt%3)
            {
                //putchar('n');
                cap433_key_value_filter(0x00);
            }

        }

    }

    //[接收到第二包数据时开始存储:read frist number ,edge == 0, cnt ~= 64000]
    /* if (reject_data_flag == 0 && (((cnt > (CNT_READ_INTERVAL1 - 3 * ALLOW_ERROR_VALUE)) && (cnt < (CNT_READ_INTERVAL1 + 3 * ALLOW_ERROR_VALUE))) || ((cnt > (CNT_READ_INTERVAL - 3 * ALLOW_ERROR_VALUE)) && (cnt < (CNT_READ_INTERVAL + 3 * ALLOW_ERROR_VALUE)))) && edge == 0) { */
    if ((((cnt > (CNT_READ_INTERVAL1 / allow_set_clock - 66 * ALLOW_ERROR_VALUE / allow_set_clock)) && (cnt < (CNT_READ_INTERVAL1 / allow_set_clock + 66 * ALLOW_ERROR_VALUE / allow_set_clock)))) && edge == 0) {
        //putchar('M');
        //接收完毕再接收新得数据包
        if (record_edge) {
            //[存了一半得数据才会清除record_edge]
            record_edge = 0;
            memcpy(buffer_433, 0, BUF_NUM);
        }
        reject_data_flag = 1;
    }

}


int read_433_data(u8 data_num)
{
    /* return buffer_433[data_num]; */
}


void timer_cap_433_test(void)
{
    printf("*********** timer cap 433 test *************\n");
    printf("buf number:");
    put_buf(buffer_433, BUF_NUM);
    /* u8 pair_code_init[] = {DATA0_NUMBER_1, DATA1_NUMBER_1, DATA2_NUMBER_1}; */
    /* set_433_pair_code(&pair_code_init); */

//    if (allow_set_clock == 1) {
//        timer_cap_433_init(JL_TIMER0, TCFG_433_PORT, 3, CLK_DIV_4, user_tmr3_cap_cbfun);
//    } else {
        timer_cap_433_init(JL_TIMER0, TCFG_433_PORT, 3, CLK_DIV_4, user_tmr3_cap_cbfun);
//    }
}

void timer_cap_433_change(void)
{
    if (ALLOW_SET_CLOCK_RATIO != 1) {
        if (allow_set_clock == 1) {
            allow_set_clock = ALLOW_SET_CLOCK_RATIO;
        } else {
            allow_set_clock = 1;
        }
        log_info("open low power to charge clock:%d", allow_set_clock);
        //close last irq
        u8 tmr_num;
        JL_TIMER_TypeDef *JL_TIMERx = NULL;
        for (tmr_num = 0; tmr_num < 4; tmr_num ++) {
            if ((u32)JL_TIMERx == TIMERx_table[tmr_num]) {
                break;
            }
            if (tmr_num == 3) {
                return;
            }
        }
        unrequest_irq(TIMERx_IRQ_IDX[tmr_num]);
        //init timer_cap
        timer_cap_433_test();
    } else {
        log_info("low power no to charge clock");
    }
}


#endif
