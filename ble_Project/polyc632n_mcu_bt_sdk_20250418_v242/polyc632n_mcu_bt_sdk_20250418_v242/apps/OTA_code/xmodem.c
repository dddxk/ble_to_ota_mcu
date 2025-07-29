#include "app_config.h"
#include "asm/cpu.h"
#include "system/includes.h"
#include "xmodem.h"
#include "virtual_mcu_uart.h"
/*------------------------------------------------------------------------------------------------------------------*/

static u8 xm_tx_trans_buff[1024] __attribute__((aligned(4)));
static u16 xm_tx_trans_len = 0;

//extern void uart_polyc_hw_send_data(u8 *buf, u32 buf_len);
void uart1_send_data(u8 *buf, u32 buf_len)
{
    polyc_uart_send_data(UART1,buf,buf_len,0);
}

static const u16 xm_crc16tab[256]= {
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
	0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
	0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
	0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
	0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
	0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
	0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
	0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
	0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
	0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
	0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
	0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
	0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
	0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
	0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
	0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
	0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
	0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
	0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
	0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
	0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
	0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
	0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
	0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
	0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
	0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
	0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
	0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
	0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
	0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};


static u16 xm_crc16_ccitt(const u8 *buf, u16 len)
{
	u16 counter;
	u16 crc = 0;

	for( counter = 0; counter < len; counter++)
	{
		crc = (crc<<8) ^ xm_crc16tab[((crc>>8) ^ *(char *)buf++)&0x00FF];
	}
	return crc;
}

/*------------------------------------------------------------------------------------------------------------------*/

static u16 g_xm_timeout_handle = NULL;
static u16 xmodem_timeout_cnt = 0;
static u8 sync_retry = 0;

static u8 error_result = 0;
static u8 update_succ_flag = 0;

static FILE *fp = NULL;
static u32 f_size = 0;

static u8 f_auto_trans_flag = 0;

/*------------------------------------------------------------------------------------------------------------------*/

u8 xm_inbyte(u8 *buffer,u16 buffer_size)
{
	u8 ch = 0;

	error_result = 0;

	if(xmodem_timeout_cnt < XM_DLY_1S)
	{
	    /* 移植时，需要修改该函数 */
		if((buffer != NULL)&&(buffer_size == 1))
        {
            ch = buffer[0];
            /* putchar('['); */
            /* putchar(ch);  */
            /* putchar(']'); */
            return ch;
        }
	}

    //超时管理
	xmodem_timeout_cnt = 0;
	error_result = 1;
	return ch;
}

void xm_outbyte(u8 trans_byte)
{
    u8 tx_data[1] = { 0x00 };

    tx_data[0] =  trans_byte;

    /*printf("xm_outbyte = 0x%02x\n",tx_data[0]);*/

    /* 移植时，需要修改该函数 */
    uart1_send_data(tx_data,1);
}

void xm_out_nbyte(u8 *buff,u16 buff_len)
{
    /* 移植时，需要修改该函数 */
    uart1_send_data(buff,buff_len);
}

static void xm_flushinput(void)
{
    if(fp != NULL){
        fclose(fp);
        f_size = 0;
        fp = NULL;
    }
}

/*------------------------------------------------------------------------------------------------------------------*/

static void xm_comm_timeout_process(void)
{
    if(g_xm_timeout_handle == NULL) { return; }
    if(f_auto_trans_flag == 0) return;

    xmodem_timeout_cnt++;
    if(xmodem_timeout_cnt >= XM_DLY_1S)
    {
        xmodem_timeout_cnt = 0;
        if(sync_retry < XM_RETRY) { sync_retry++; }
        else{
            sys_timer_del(g_xm_timeout_handle);
            g_xm_timeout_handle = NULL;
            sync_retry = 0;
            f_auto_trans_flag = 2;
            printf("xm_comm_timeout_process f_auto_trans_flag=2");
        }
    }
}

static const u8 enter_bootloader_buf[] = { 0x55,0xAA,0x11,0x88,0x93,0xE1 };


void xmodem_comm_init(void)
{
    if(g_xm_timeout_handle == NULL){
        xmodem_timeout_cnt = 0;
        g_xm_timeout_handle = sys_timer_add(NULL,xm_comm_timeout_process,10);
    }
    f_auto_trans_flag = 0;
    printf("get_user_ota_update_flag = [%d]!\n",get_user_ota_update_flag());
    if(get_user_ota_update_flag() == 1){
        uart1_send_data(enter_bootloader_buf,sizeof(enter_bootloader_buf));
    }
}

int xmodem_comm(u8 *buffer,u16 buffer_size)
{
    u8 xm_buf[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc */
    u8 f_read_buff[128] = { 0x00 };

    u8 Handshake_byte,i;
    int ret = 0;

    static int crc = -1;
    static u8 stage = 0;

    static u32 trans_len = 0;
    static u8 trans_retry = 0;
    static u8 end_trans_retry = 0;

    static u16 xm_buf_size;
    static s32 fill_size;

    static u8 xm_packet_num = 1;

    printf("--> xmodem_comm:\n");
    printf_buf(buffer,buffer_size);

    Handshake_byte = xm_inbyte(buffer,buffer_size);
    printf("Handshake_byte[%x][%x]",Handshake_byte,'C');
    /*----------------------------------------------------------------------------------*/

    if((f_auto_trans_flag == 0)&&(Handshake_byte != 0))
    {
        f_auto_trans_flag = 1;
        sync_retry = 0;
        printf("f_auto_trans_flag = 1\n");
    }
    else if(f_auto_trans_flag == 2){
        printf("f_auto_trans_flag = 2\n");
        return 0;
    }
    printf("SDFILE_RES_ROOT_PATH[%s]",SDFILE_RES_ROOT_PATH);
    printf("stage[%x]",stage);
    /*----------------------------------------------------------------------------------*/
    if(stage == 0)
    {
        if(sync_retry >= XM_RETRY){
            sync_retry = 0;
            trans_len = 0;
            xm_outbyte(XM_CAN);
            xm_outbyte(XM_CAN);
            xm_outbyte(XM_CAN);
            xm_flushinput();
            return -2; /* no sync */
        }

        if(error_result == 0)
        {
            switch (Handshake_byte)
            {
                case 'C':
                    crc = 1;
                    if((fp == NULL)&&(f_size == 0))
                    {
                        fp = fopen(SDFILE_RES_ROOT_PATH"user.bin", "r");
                        if(fp != NULL) {
                            printf("\n___fopen_successed!___\n");
                            f_size = flen(fp);
                            printf("file_length = %d\n",f_size);
                            stage = 1;
                            trans_len = 0;
                            trans_retry = 0;
                            end_trans_retry = 0;

                            goto start_trans;
                        }
                        else {
                            printf("\n___ccrc fopen_failed!___\n");
                            xm_outbyte(XM_CAN);
                            xm_outbyte(XM_CAN);
                            xm_flushinput();
                            return -3; /* fopen error */
                        }
                    }
                    break;

                case XM_NAK:
                    crc = 0;
                    if((fp == NULL)&&(f_size == 0))
                    {
                        fp = fopen(SDFILE_RES_ROOT_PATH"user.bin", "r");
                        if(fp != NULL) {
                            printf("\n___fopen_successed!___\n");
                            f_size = flen(fp);
                            printf("file_length = %d\n",f_size);
                            stage = 1;
                            trans_len = 0;
                            trans_retry = 0;
                            end_trans_retry = 0;
                            goto start_trans;
                        }
                        else {
                            printf("\n___ccks fopen_failed!___\n");
                            xm_outbyte(XM_CAN);
                            xm_outbyte(XM_CAN);
                            xm_flushinput();
                            return -3; /* fopen error */
                        }
                    }
                    break;
                case XM_CAN:
                    {
                        stage = 0;
                        trans_len = 0;
                        trans_retry = 0;
                        end_trans_retry = 0;
                        xm_outbyte(XM_ACK);
                        xm_flushinput();
                        return -1; /* canceled by remote */
                    }
                    break;
                default:
                    break;
            }
        }
    }
    else if(stage == 1)
    {
        if(error_result == 0)
        {
            switch (Handshake_byte)
            {
                case XM_ACK:
                    ++xm_packet_num;
                    trans_retry = 0;
                    goto start_trans;
                    break;
                case XM_CAN:
                    {
                        xm_outbyte(XM_ACK);
                        xm_flushinput();
                        return -1; /* canceled by remote */
                    }
                    break;
                case XM_NAK:
                default:
                    break;
            }
        }

        goto retrans;
    }
    else if(stage == 2){
        printf("stage2->Handshake_byte[%x]",Handshake_byte);
        switch (Handshake_byte)
        {
            case XM_ACK:
                xm_flushinput();
                /* Add your code here... */
                f_auto_trans_flag = 2;
                return trans_len;

                break;
            default :
                if(end_trans_retry < MAXRETRANS){
                    end_trans_retry++;
                }
                else{
                   xm_flushinput();
                   return -5; /* EOT error*/
                }
                break;
        }

        goto end_trans;
    }

    /*----------------------------------------------------------------------------------*/

start_trans:
    if(fp != NULL)
    {
        ///printf("--> start_trans!\n");
        xm_buf[0] = XM_SOH; xm_buf_size = 128;
        /* xm_buf[0] = XM_STX; xm_buf_size = 1024; */
        xm_buf[1] = xm_packet_num;
        xm_buf[2] = ~xm_packet_num;

        fill_size = f_size - trans_len;
        ///printf("remain_size = %d\n",fill_size);

        if (fill_size > xm_buf_size) { fill_size = xm_buf_size; }//取最大数据帧长度
        ///printf("fill_size = %d\n",fill_size);

        if (fill_size >= 0)
        {
            /*---------------------------------------------------------*/
            memset(&xm_buf[3], 0, xm_buf_size);

            memset(f_read_buff,0x00,sizeof(f_read_buff));
            ret = fread(fp, f_read_buff, fill_size);
            if (ret != fill_size) {
                printf(" file read buf err %d\n", ret);
                xm_flushinput();
                return -3; /* fopen error */;
            }
            else{
                memcpy(&xm_buf[3],f_read_buff,fill_size);
                /* Add your code here... */
                if (fill_size < xm_buf_size){
                    memset(&xm_buf[3+fill_size],XM_EOF,(xm_buf_size - fill_size));
                }
                trans_len += xm_buf_size;
            }
            /*---------------------------------------------------------*/
            if (crc){
                u16 ccrc = xm_crc16_ccitt(&xm_buf[3], xm_buf_size);
                xm_buf[xm_buf_size+3] = (ccrc>>8) & 0xFF;
                xm_buf[xm_buf_size+4] = ccrc & 0xFF;
            }
            else{
                u16 ccks = 0;
                for (i = 3; i < xm_buf_size+3; ++i)
                {
                    ccks += xm_buf[i];
                }
                xm_buf[xm_buf_size+3] = (u8)(ccks & 0x00FF);
            }
            /*---------------------------------------------------------*/
retrans:
            if(trans_retry < MAXRETRANS)
            {
                trans_retry++;
                xm_tx_trans_len = xm_buf_size+3+(crc ? 2 : 1);
                ///printf("xm_tx_trans_len = %d\n",xm_tx_trans_len);
                memcpy(xm_tx_trans_buff,xm_buf,xm_tx_trans_len);
                xm_out_nbyte(xm_tx_trans_buff,xm_tx_trans_len);
            }
            else{
                xm_outbyte(XM_CAN);
                xm_outbyte(XM_CAN);
                xm_outbyte(XM_CAN);
                xm_flushinput();
                return -4; /* xmit error */
            }
            /*---------------------------------------------------------*/
        }
        else{
            if(stage != 2) { stage = 2; }
end_trans:
            xm_outbyte(XM_EOT);
        }

        return trans_len;
    }

    return 0;
    /*----------------------------------------------------------------------------------*/

}



/*------------------------------------------------------------------------------------------------------------------*/
