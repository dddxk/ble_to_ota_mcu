////////////////////////////////////////////////////////////////////////////////
/// @file    main.c
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion
#define _MAIN_C_

// Files includes
#include "platform.h"
#include "gpio_led_toggle.h"
#include "uart_interrupt.h"
#include "main.h"
#include "delay.h"

#define BOOTLOADER_ADDRESS		(0x8000000)
#define APPLICATION_ADDRESS		(16*1024 + 0x8000000)  
#define JUMP_FLAG_ADDRESS  		(APPLICATION_ADDRESS - 1024)

__IO uint32_t VectorTable[48] __attribute__((at(0x20000000)));

uint8_t Need_Jump_Flag = 0;
uint8_t Jump_Valid_Bytes[4] = { 0x00 };

void setNVIC(void)
{
	u8 i;
	
	//��APP��������ת�Ƶ�SRAM
	for(i = 0; i < 48; i++)
	{
		VectorTable[i] = *(__IO uint32_t*)(APPLICATION_ADDRESS + (i<<2));
	}
	//Enable the SYSCFG Peripheral Clock
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_SYSCFG,ENABLE);
    
	//Remap SRAM at 0x00000000 ��SRAM�е�������ӳ�䵽0x0000000
	SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
	__enable_irq();
}

void Reset_Periph(void)
{
	UART_DeInit(UART2);                            //��λUART2
	NVIC_DisableIRQ(UART2_IRQn);                   //�ر�UART2�ж�
	NVIC_DisableIRQ(SysTick_IRQn);                 //�ر�systick�ж�
	__disable_irq();                               //��ת֮ǰ�ر����жϣ���ֹ�����жϵ�������
}

void Reset_MCU(void)
{
	NVIC_SystemReset(); //ϵͳ��λ
}

static void FLASH_Write(const u8 *buff, u32 addr, u32 writeNumber)
{
	u32 temp;
	u32 WriteAddress;
	u16 HalfWord;

	WriteAddress = addr;

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	for (temp = 0; temp < writeNumber; temp+=2)
	{
		HalfWord = buff[temp];
		HalfWord = HalfWord | (buff[temp + 1] << 8);
		FLASH_ProgramHalfWord(WriteAddress, HalfWord);
		WriteAddress = WriteAddress + 2;
	}

	FLASH_Lock();
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval  0.
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
	setNVIC();

	DELAY_Init();
	
	PLATFORM_Init();

	UART_Configure(115200);

	while (1)
	{
		if(Need_Jump_Flag == 1)
		{
			FLASH_Unlock();
			FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
			FLASH_ErasePage(JUMP_FLAG_ADDRESS);
			FLASH_Lock();

			FLASH_Write(Jump_Valid_Bytes,JUMP_FLAG_ADDRESS,sizeof(Jump_Valid_Bytes));

			Reset_Periph();
			Reset_MCU();

			//__set_MSP(*(u32*)BOOTLOADER_ADDRESS);//����SP.����ջջ����ַ
			//((void(*)(void))*(u32*)(0x04+BOOTLOADER_ADDRESS))();//������ת����.����λ�ж�������ַ��Ϊ����ָ�� 
		}
		else
		{
			GPIO_LED_Toggle_Sample();
		}
	}
}

/// @}

/// @}

/// @}

