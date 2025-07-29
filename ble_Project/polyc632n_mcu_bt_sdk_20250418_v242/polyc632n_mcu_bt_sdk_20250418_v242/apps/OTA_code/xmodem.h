#ifndef __XMODEM_H__
#define __XMODEM_H__

#define XM_DLY_1S	100		//在1S内xm_timer被调用的次数，小于65536
#define XM_RETRY	10		//retry次数

#define XM_SOH  	0x01	//Xmodem数据头
#define XM_STX  	0x02	//1K-xmodem数据头
#define XM_EOT  	0x04	//发送结束
#define XM_ACK  	0x06	//认可应答
#define XM_NAK  	0x15	//不认可应答
#define XM_CAN  	0x18	//丛机撤销传输
#define XM_EOF 		0x1A	//数据包填充

#define XM_OK		0
#define XM_ERR		-1

#define MAXRETRANS 	10

void xmodem_comm_init(void);
int xmodem_comm(u8 *buffer,u16 buffer_size);

#endif
