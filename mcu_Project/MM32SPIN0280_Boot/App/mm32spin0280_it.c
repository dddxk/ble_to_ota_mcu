#define _MM32SPIN0280_IT_C_

#include "platform.h"
#include "mm32spin0280_it.h"
#include "uart_interrupt.h"
#include "gpio_led_toggle.h"
#include "xmodem.h"
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
  * @brief  This function handles TIM14 Handler
  * @note   none
  * @param  none
  * @retval none
  *********************************************************************************************************************/
extern void LED_Scan_Func(void);

void TIM14_IRQHandler(void)
{
    if (RESET != TIM_GetITStatus(TIM14, TIM_IT_Update))
    {
		LED_Scan_Func();
        TIM_ClearITPendingBit(TIM14,TIM_IT_Update);
    }
}

/***********************************************************************************************************************
  * @brief  This function handles UART2 Handler
  * @note   none
  * @param  none
  * @retval none
//  *********************************************************************************************************************/
void UART2_IRQHandler(void)
{
    if (RESET != UART_GetITStatus(UART2, UART_IT_RXIEN))
    {
		UART_RxBuffer[UART_RxLength++] = UART_ReceiveData(UART2);	

        UART_ClearITPendingBit(UART2, UART_IT_RXIEN);
    }

    if (RESET != (UART2->ISR & UART_ISR_RXIDLE))
    {
        UART2->ICR = UART_ICR_RXIDLE;
			
		/*
		printf("\r\nUART_RxLength = %d",UART_RxLength);

		for (uint8_t i = 0; i < UART_RxLength; i++)
		{
			printf("\r\n[%d] 0x%02X",i,UART_RxBuffer[i]);
		}
		*/
		
		xmodem_data_recv((uint8_t *)UART_RxBuffer, UART_RxLength);

        UART_RxLength = 0;
    }
}

