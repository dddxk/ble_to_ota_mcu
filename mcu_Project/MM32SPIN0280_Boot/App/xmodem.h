#ifndef _XMODEM_H_
#define _XMODEM_H_

#include <stdio.h>

/* XModem (128 bytes) packet format
 * Byte  0:         Header
 * Byte  1:         Packet number
 * Byte  2:         Packet number complement
 * Bytes 3-132:     Data
 * Bytes 132-133:   CRC
 */

/* XModem (1024 bytes) packet format
 * Byte  0:         Header
 * Byte  1:         Packet number
 * Byte  2:         Packet number complement
 * Bytes 3-1026:    Data
 * Bytes 1027-1028: CRC
 */
 
/* �������Ͷ��� */
typedef unsigned           char x_uint8_t;
typedef unsigned short     int  x_uint16_t;
typedef unsigned           int  x_uint32_t;
 
/* ���ݽ��ջ�������С(�û�����,����Ҫ���� 3 + 1024 + 2) */
#define X_PROT_FRAME_LEN_RECV  1200u

/* ����������(�û�����). */
#define X_MAX_ERRORS ((x_uint8_t)10u)

/* ��ⳬʱ�������� */
/* 0��ʹ�þ�ȷʱ����м�⣬��ʵ�� x_uint32_t x_get_tick(void) ������
      �ú���Ӧ���غ���ʱ�������Ȼ��Ҳ����������ֵ��������Ҫ���� X_RECEIVE_TIMEOUT ��ֵ
      
   1��ʹ��ѭ���� X_RECEIVE_TIMEOUT ����ֵ0�ķ�������ͬƵ�ʵĿ�������ܻ���Ҫ��ͬ����ֵ*/
#define TIMEOUT_CONFIG      1

/* �������ݳ�ʱ�����Ƽ�����Ϊ 3 �볬ʱ. */
#if TIMEOUT_CONFIG
#define X_RECEIVE_TIMEOUT    0x333333u//0xFFFFFFu
#else
#define X_RECEIVE_TIMEOUT    3000u
#endif

/* ��ͷ���ձ�־ */
#define X_IS_PACKET     ((x_uint8_t)1u)
#define X_NO_PACKET     ((x_uint8_t)0u)

/* ���ݰ���С. */
#define X_PACKET_128_SIZE   ((x_uint16_t)128u)
#define X_PACKET_1024_SIZE  ((x_uint16_t)1024u)
#define X_PACKET_CRC_SIZE   ((x_uint16_t)2u)

/* �������λ��(������ͷ). */
#define X_PACKET_NUMBER_INDEX             ((x_uint16_t)1u)
#define X_PACKET_NUMBER_COMPLEMENT_INDEX  ((x_uint16_t)2u)
#define X_PACKET_DATA_INDEX               ((x_uint16_t)3u)

/* Э�鶨����ֽ�. */
#define X_SOH ((x_uint8_t)0x01u)  /**< ��ͷ (128 bytes). */
#define X_STX ((x_uint8_t)0x02u)  /**< ��ͷ (1024 bytes). */
#define X_EOT ((x_uint8_t)0x04u)  /**< �������. */
#define X_ACK ((x_uint8_t)0x06u)  /**< Ӧ��. */
#define X_NAK ((x_uint8_t)0x15u)  /**< ��Ӧ��. */
#define X_CAN ((x_uint8_t)0x18u)  /**< ȡ��. */
#define X_C   ((x_uint8_t)0x43u)  /**< ASCII��C����Ҫ֪ͨ��λ��������Ҫ��CRC16. */

/* ���ܵ�״̬����. */
typedef enum {
  X_OK            = 0x00u, /**< ����ɹ�. */
  X_ERROR_CRC     = 0x01u, /**< CRC У�����. */
  X_ERROR_NUMBER  = 0x02u, /**< ��������ƥ�����. */
  X_ERROR_UART    = 0x04u, /**< �������. */
  X_ERROR_FLASH   = 0x06u, /**< Flash ����. */
  X_ERROR         = 0xFFu  /**< ��������. */
} xmodem_status;

#define X_UNUSED(Y) (void)Y      /* To avoid gcc/g++ warnings */
  
/***************************** ���⺯�� ***************************************/
/* �û����� */
int xmodem_receive(void);
void xmodem_data_recv(x_uint8_t *data, x_uint16_t data_len);

/* �û���ʵ�� */
x_uint32_t x_get_tick(void);
int x_transmit_ch(x_uint8_t ch);
int receive_file_data_callback(void *ptr, char *file_data, x_uint32_t w_size);
int receive_file_callback(void *ptr);
/***************************** ���⺯�� ***************************************/

#endif /* XMODEM_H_ */
