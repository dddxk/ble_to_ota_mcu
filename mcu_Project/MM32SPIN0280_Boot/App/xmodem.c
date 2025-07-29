/**
  ******************************************************************************
  * @file    xmodem.c
  * @author  long
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   xmodem-1k Э��
  ******************************************************************************
***/

#include "platform.h"
#include "xmodem.h"
#include <string.h>
#include "hal_conf.h"

/* ȫ�ֱ���. */
static x_uint8_t recv_buf[X_PROT_FRAME_LEN_RECV];                     /* �������ݻ����� */
static x_uint32_t recv_len;                                           /* ���յ����ݵĳ��� */
static x_uint8_t xmodem_packet_number = 1u;                           /* ������. */
static x_uint8_t x_first_packet_received = X_IS_PACKET;               /* �ǲ��ǰ�ͷ. */
static void *file_ptr = 0;

/* �ֲ�����. */
static x_uint16_t xmodem_calc_crc(x_uint8_t *data, x_uint16_t length);
static xmodem_status xmodem_handle_packet(x_uint8_t *header);
static xmodem_status xmodem_error_handler(x_uint8_t *error_number, x_uint8_t max_error_number);
static x_uint16_t get_active_length(x_uint8_t *data, x_uint16_t len);
static int get_receive_data(x_uint8_t **data, x_uint32_t len);
static void reset_recv_len(void);
static x_uint32_t get_recv_len(void);

int x_transmit_ch(uint8_t ch)
{
	UART_SendData(UART2, ch);

	while (RESET == UART_GetFlagStatus(UART2, UART_FLAG_TXEPT)) {}

	return 0;
}


/**
 * @brief   ���������XmodemЭ��Ļ���.
 *          �������ݲ���������.
 * @param   rec_num:��Ҫ���յ��ļ�����
 * @return  0:�ļ����ճɹ� -1:�ļ�����ʧ��
 */
int xmodem_receive(void)
{
  volatile xmodem_status status = X_OK;
  x_uint8_t error_number = 0u;

  x_first_packet_received = X_NO_PACKET;
  xmodem_packet_number = 1u;
  
  (void)x_transmit_ch(X_C);    // ����λ������ ACSII "C" ��������λ����ʹ�� CRC-16 

  /* ѭ����ֱ��û���κδ���(���߻��������ļ��������). */
  while (X_OK == status)
  {
    x_uint8_t *header = 0x00u;

    /* ��ȡ����ͷ. */
    int receive_status = get_receive_data(&header, 1u);

    /* ��ACSII "C"���͸���λ��(ֱ�������յ�һЩ����), ������λ������Ҫʹ�� CRC-16 . */
    if ((0 != receive_status) && (X_NO_PACKET == x_first_packet_received))
    {
      (void)x_transmit_ch(X_C);    // ����λ������ ACSII "C" ��������λ����ʹ�� CRC-16 
			
			user_timeout_exit_cnt++;
			if(user_timeout_exit_cnt >= USER_AUTO_EXIT_CNT)
			{
				user_timeout_exit_cnt = 0;
				status = X_ERROR;
				printf("\r\nTimeout Exit!");
			}
    }
    /* ��ʱ����������. */
    else if ((0 != receive_status) && (X_IS_PACKET == x_first_packet_received))
    {
      status = xmodem_error_handler(&error_number, X_MAX_ERRORS);
    }
    else
    {
      /* û�д���. */
      //header = data_rx_buff;
    }

    /* ��ͷ����ʹ: SOH, STX, EOT and CAN. */
		xmodem_status packet_status = X_ERROR;
    
    switch(header[0])
    {
      /* 128��1024�ֽڵ�����. */
      case X_SOH:
      case X_STX:
        /* ���ݴ��� */
        packet_status = xmodem_handle_packet(header);
				/* �������ɹ�������һ�� ACK. */
        if (X_OK == packet_status)
        {
          (void)x_transmit_ch(X_ACK);
        }
        /* ���������flash��أ����������������������Ϊ���ֵ (������ֹ����). */
        else if (X_ERROR_FLASH == packet_status)
        {
          printf("X_ERROR_FLASH\r\n");
          error_number = X_MAX_ERRORS;
          status = xmodem_error_handler(&error_number, X_MAX_ERRORS);
        }
        /* �������ݰ�ʱ����Ҫô����һ�� NAK��Ҫôִ�д�����ֹ. */
        else
        {
          status = xmodem_error_handler(&error_number, X_MAX_ERRORS);
        }
        break;
        
      /* �������. */
      case X_EOT:
        /* ACK����������λ��(���ı���ʽ) */
        (void)x_transmit_ch(X_ACK);
        /* һ���ļ�������� */
        x_first_packet_received = X_NO_PACKET;
        xmodem_packet_number = 1;
        receive_file_callback(file_ptr);
        file_ptr = 0;
        return 0;
      
      /* ���Ͷ˷���ȡ��. */
      case X_CAN:
        status = X_ERROR;
        break;
      
      default:
        /* Wrong header. */
        if (0 == receive_status)
        {
          status = xmodem_error_handler(&error_number, X_MAX_ERRORS);
        }
        break;
    }
  }
  
  return -1;
}

/**
 * @brief   ������յ����� CRC-16.
 * @param   *data:  Ҫ��������ݵ�����.
 * @param   length: ���ݵĴ�С��128�ֽڻ�1024�ֽ�.
 * @return  status: ����CRC.
 */
static x_uint16_t xmodem_calc_crc(x_uint8_t *data, x_uint16_t length)
{
  x_uint16_t crc = 0u;
  while (length)
  {
      length--;
      crc = crc ^ ((x_uint16_t)*data++ << 8u);
      for (x_uint8_t i = 0u; i < 8u; i++)
      {
          if (crc & 0x8000u)
          {
              crc = (crc << 1u) ^ 0x1021u;
          }
          else
          {
              crc = crc << 1u;
          }
      }
  }
  //printf("xmodem_calc_crc = 0x%04X\r\n",crc);
  return crc;
}

/**
 * @brief   ��������������Ǵ�xmodemЭ���л�õ����ݰ�.
 * @param   header: SOH ���� STX.
 * @return  status: ������.
 */
static xmodem_status xmodem_handle_packet(x_uint8_t *header)
{
  xmodem_status status = X_OK;
  x_uint16_t size = 0u;
  
  if (X_SOH == header[0])
  {
    size = X_PACKET_128_SIZE;
    //printf("X_PACKET_128_SIZE\r\n");
  }
  else if (X_STX == header[0])
  {
    size = X_PACKET_1024_SIZE;
    //printf("X_PACKET_1024_SIZE\r\n");
  }
  else
  {
    /* ���������. */
    status = X_ERROR;
  }
  x_uint16_t length = size + X_PACKET_DATA_INDEX + X_PACKET_CRC_SIZE;

#if 1    // 0:���������Լӿ촫���ٶȣ�����������������
  x_uint8_t received_data[X_PACKET_1024_SIZE + X_PACKET_DATA_INDEX + X_PACKET_CRC_SIZE];
  memcpy(&received_data[0u], header, length);
#else
  x_uint8_t *received_data = header;
#endif
  /* ��������. */
  int receive_status = 0;

  /* ���������д�� flash. */
  if (X_OK == status)
  {
    #if 1   // 0:��У����ԼӼӿ촫���ٶȣ�����������������
    /* ��������ֽ�������������CRC. */
    x_uint16_t crc_received = ((x_uint16_t)received_data[length-2u] << 8u) | ((x_uint16_t)received_data[length-1u]);
    //printf("crc_received = 0x%04X\r\n",crc_received);
    
    /* У��. */
    x_uint16_t crc_calculated = xmodem_calc_crc(&received_data[X_PACKET_DATA_INDEX], size);
    if (crc_calculated != crc_received)
    {
      /* �����CRC�ͽ��յ�CRC��ͬ. */
      status |= X_ERROR_CRC;
    }
    #endif
    
    if (0 != receive_status)
    {
      /* �������. */
      status |= X_ERROR_UART;
    }
    
    if (xmodem_packet_number != received_data[X_PACKET_NUMBER_INDEX])
    {
      /* ���������������ƥ��. */
      status |= X_ERROR_NUMBER;
    }
    
    if (255u != (received_data[X_PACKET_NUMBER_INDEX] +  received_data[X_PACKET_NUMBER_COMPLEMENT_INDEX]))
    {
      /* ���źͰ��Ų���֮�Ͳ���255. */
      /* �ܺ�Ӧ������255. */
      status |= X_ERROR_NUMBER;
    }
    
    if (status == X_OK)
    {
      #if 0
      /* �ж������� 0x1A ��ô�Ͷ������� */
      size = get_active_length((x_uint8_t *)&received_data[X_PACKET_DATA_INDEX], size);        // ��ȡ��Ч�ĳ���
      #endif
      if (receive_file_data_callback(file_ptr, (char *)&received_data[X_PACKET_DATA_INDEX], size) != 0)
      {
        /* Ӳ������. */
        status |= X_ERROR_FLASH;
      }
    }
    /* ��ǽ��յ�һ����. */
    x_first_packet_received = X_IS_PACKET;
  }

  /* ���Ӱ������͵�ַ�����ٵ�ǰ������ʣ������ (���û���κδ���Ļ�). */
  if (X_OK == status)
  { 
    xmodem_packet_number++;
  }

  return status;
}

/**
 * @brief   ����xmodem����.
 *          ���Ӵ����������Ȼ��������������ﵽ�ٽ磬������ֹ��������һ�� NAK.
 * @param   *error_number:    ��ǰ������(��Ϊָ�봫��).
 * @param   max_error_number: �������������.
 * @return  status: X_ERROR �ﵽ������������, X_OK ��������.
 */
static xmodem_status xmodem_error_handler(x_uint8_t *error_number, x_uint8_t max_error_number)
{
  xmodem_status status = X_OK;
  /* �������������. */
  (*error_number)++;
  /* ����������ﵽ���ֵ������ֹ. */
  if ((*error_number) >= max_error_number)
  {
    /* ��ֹ����. */
    (void)x_transmit_ch(X_CAN);
    (void)x_transmit_ch(X_CAN);
    status = X_ERROR;
  }
  /* ����һ��NAK�����ظ�. */
  else
  {
    (void)x_transmit_ch(X_NAK);
    status = X_OK;
  }
  return status;
}

/**
 * @brief   ��ȡ�ļ���Ч�ĳ��ȣ���ȥ����Ϊ0x1A���ֽڣ�.
 * @param   *data: ָ�����ݵ�ָ��.
 * @param   len: ���ݳ���.
 * @return  ��Ч�����ݳ���.
 */
static x_uint16_t get_active_length(x_uint8_t *data, x_uint16_t len)
{
  while(len)
  {
    if (*(data + len - 1) == 0x1A) { len--; }
    else { break; }
  }
  
  return len;
}

/**
 * @brief   Ymodem �������ݵĽӿ�.
 * @param   *data ����������
 * @param   *len ���������ݵĳ���
 * @return  �������ݵ�״̬
 */
static int get_receive_data(x_uint8_t **data, x_uint32_t len)
{
    volatile x_uint32_t timeout = X_RECEIVE_TIMEOUT;
    x_uint8_t *data_temp = 0;
    x_uint16_t max_len = 1;
    x_uint16_t data_len[2] = {128, 1024};
  
#if TIMEOUT_CONFIG
	while (timeout--)   // �ȴ����ݽ������
	{
        if (get_recv_len() >= max_len)
        {
            if (max_len != 1) break;
            
            data_temp = recv_buf;                                 // ��ȡ���յ�������
            if (*data_temp == X_SOH || *data_temp == X_STX)       // ��һ����SOH��˵��������Ҫ����133���ֽ�
            {
                max_len = data_len[*data_temp - 1] + 3 + 2;       // ���ݲ�ͬ��ͷ��¼��ͬ�ĳ���
            }
            else
            {
                break;
            }
        }
        
        if (timeout == 0)
        {
            printf("\r\nxmodem rx timeout!");
            return -1;    // ��ʱ����
        }
	}
#else
    x_uint32_t tickstart = x_get_tick();
    while (1)   // �ȴ����ݽ������
    {
        if (x_get_tick() - tickstart > timeout)    // ����Ƿ�ʱ
        {
            return -1;    // ��ʱ����
        }

        if (get_recv_len() >= max_len)
        {
            if (max_len != 1) break;
            
            data_temp = recv_buf;                                 // ��ȡ���յ�������
            if (*data_temp == X_SOH || *data_temp == X_STX)       // ��һ����SOH��˵��������Ҫ����133���ֽ�
            {
                max_len = data_len[*data_temp] + 3 + 2;           // ���ݲ�ͬ��ͷ��¼��ͬ�ĳ���
            }
            else
            {
                break;
            }
        }
    }
#endif
	
	/* ��ȡ�������� */
    *data = recv_buf;
    reset_recv_len();

    return 0;
}

/**
 * @brief   ��λ���ݽ��ճ���
  * @param  void.
 * @return  void.
 */
static void reset_recv_len(void)
{
  recv_len = 0;
}

/**
 * @brief   ��ȡ���ݽ��ճ���
 * @param   void.
 * @return  ���յ����ݵĳ���.
 */
static x_uint32_t get_recv_len(void)
{
  return recv_len;
}

/**
 * @brief   �������ݴ���
 * @param   *data:  Ҫ��������ݵ�����.
 * @param   data_len: ���ݵĴ�С
 * @return  void.
 */
void xmodem_data_recv(x_uint8_t *data, x_uint16_t data_len)
{
  if(recv_len + data_len >= X_PROT_FRAME_LEN_RECV){
     recv_len = 0;
  }
  
  memcpy(recv_buf + recv_len, data, data_len); 
  recv_len += data_len;
}

/****************************************************************************************************/
/**
 * @brief   ��ȡ����ʱ���.
 * @param   void
 * @return  ʱ���
 */
__weak x_uint32_t x_get_tick(void)
{
	
	return 0;
}

/**
 * @brief   Ymodem ����һ���ַ��Ľӿ�.
 * @param   ch �����͵�����
 * @return  ���ط���״̬
 */
/*__weak int x_transmit_ch(x_uint8_t ch)
{
	X_UNUSED(ch);
	
	return -1;
}*/

/**
 * @brief   �ļ����ݽ�����ɻص�.
 * @param   *ptr: ���ƾ��.
 * @param   *file_name: �ļ�����.
 * @param   file_size: �ļ���С����Ϊ0xFFFFFFFF����˵����С��Ч.
 * @return  ����д��Ľ����0���ɹ���-1��ʧ��.
 */
__weak int receive_file_data_callback(void *ptr, char *file_data, x_uint32_t w_size)
{
  X_UNUSED(ptr);
  X_UNUSED(file_data);
  X_UNUSED(w_size);
  
  /* �û�Ӧ�����ⲿʵ��������� */
  return -1;
}

/**
 * @brief   һ���ļ�������ɻص�.
 * @param   *ptr: ���ƾ��.
 * @return  ����д��Ľ����0���ɹ���-1��ʧ��.
 */
__weak int receive_file_callback(void *ptr)
{
  X_UNUSED(ptr);
  
  /* �û�Ӧ�����ⲿʵ��������� */
  return -1;
}
/****************************************************************************************************/

