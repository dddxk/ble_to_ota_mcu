#define _MM32SPIN0280_IT_C_

#include "platform.h"
#include <stdio.h>
#include "string.h"
#include "mm32spin0280_it.h"
#include "uart_interrupt.h"

extern uint8_t Need_Jump_Flag;
extern uint8_t Jump_Valid_Bytes[4];
static const uint8_t Jump_ACK_buff[] = { 0xF1,0x55, 0xAA, 0xF4 };

static const uint16_t CRC_16_VALUE = 0x93E1;

static uint16_t calculate_xmodem_crc(uint8_t *data, uint16_t length)
{
  uint16_t crc = 0u;
  while (length)
  {
      length--;
      crc = crc ^ ((uint16_t)*data++ << 8u);
      for (uint8_t i = 0u; i < 8u; i++)
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
  return crc;
}
/***********************************************************************************************************************
  * @brief  This function handles NMI exception
  * @note   none
  * @param  none
  * @retval none
  *********************************************************************************************************************/
void NMI_Handler(void)
{
}

/***********************************************************************************************************************
  * @brief  This function handles Hard Fault exception
  * @note   none
  * @param  none
  * @retval none
  *********************************************************************************************************************/
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/***********************************************************************************************************************
  * @brief  This function handles SVCall exception
  * @note   none
  * @param  none
  * @retval none
  *********************************************************************************************************************/
void SVC_Handler(void)
{
}

/***********************************************************************************************************************
  * @brief  This function handles PendSVC exception
  * @note   none
  * @param  none
  * @retval none
  *********************************************************************************************************************/
void PendSV_Handler(void)
{
}

/***********************************************************************************************************************
  * @brief  This function handles SysTick Handler
  * @note   none
  * @param  none
  * @retval none
  *********************************************************************************************************************/
//void SysTick_Handler(void)
//{
//    if (0 != PLATFORM_DelayTick)
//    {
//        PLATFORM_DelayTick--;
//    }
//}

/***********************************************************************************************************************
  * @brief  This function handles UART2 Handler
  * @note   none
  * @param  none
  * @retval none
  *********************************************************************************************************************/
void UART2_IRQHandler(void)
{
	uint16_t i = 0;
	
    if (RESET != UART_GetITStatus(UART2, UART_IT_RXIEN))
    {
        UART_RxBuffer[UART_RxLength++] = UART_ReceiveData(UART2);

        UART_ClearITPendingBit(UART2, UART_IT_RXIEN);
    }

    if (RESET != (UART2->ISR & UART_ISR_RXIDLE))
    {
        UART2->ICR = UART_ICR_RXIDLE;
	
		if((Need_Jump_Flag == 0)&&(UART_RxLength == 6))
		{
			uint16_t calc_crc = calculate_xmodem_crc((uint8_t *)&UART_RxBuffer[0],UART_RxLength - 2);
			uint16_t trans_crc = (((uint16_t)UART_RxBuffer[UART_RxLength - 2] & 0x00FF) << 8)
													| ((uint16_t)UART_RxBuffer[UART_RxLength - 1] & 0x00FF);
			//printf("\r\ncalc_crc  = 0x%02X",calc_crc);
			//printf("\r\ntrans_crc = 0x%02X",trans_crc);
			
			if((CRC_16_VALUE == calc_crc)&&(CRC_16_VALUE == trans_crc))
			{
				for (i = 0; i < sizeof(Jump_ACK_buff); i++)
				{
					UART_SendData(UART2, Jump_ACK_buff[i]);

					while (RESET == UART_GetFlagStatus(UART2, UART_FLAG_TXEPT))
					{
					}
				}
				memcpy(Jump_Valid_Bytes,(uint8_t *)UART_RxBuffer,UART_RxLength-2);
				Need_Jump_Flag = 1;
			}
		}
        UART_RxLength = 0;
    }
}

