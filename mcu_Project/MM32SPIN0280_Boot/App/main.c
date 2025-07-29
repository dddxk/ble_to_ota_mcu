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
#include <stdio.h>
#include "string.h"
#include "main.h"
#include "delay.h"
#include "platform.h"
#include "xmodem.h"
#include "gpio_led_toggle.h"

#define ApplicationAddress   (16*1024 + 0x8000000)
#define BootJumpFlagAddress  (ApplicationAddress - 1024)

const u8 Valid_buf[4]={0x55,0xAA,0x11,0x88};
const u8 Blank_buf[4]={0xFF,0xFF,0xFF,0xFF};
const u8 Claer_Flag_buf[4]={0x00,00,0x00,0x00};

uint16_t user_timeout_exit_cnt = 0; 

LED_SCAN_MODE_TypeDef LED_SCAN_MODE = LED_SCAN_ALL_OFF;

void LED_Scan_Func(void)
{
	static uint16_t led_scan_cnt = 0;
	
	led_scan_cnt++;

	switch(LED_SCAN_MODE)
	{
		case LED_G_SCAN_FAST:
			if((led_scan_cnt % 15) == 0 ) { GPIO_LED_Show_Update(); }
			break;
		case LED_G_SCAN_SLOW:
			if((led_scan_cnt % 50) == 0 ) { GPIO_LED_Show_Update(); }
			break;
		case LED_G_ALWAYS_ON:
			GPIO_LED_Show_Update_Succeeded();
			break;
		case LED_R_ALWAYS_ON:
			GPIO_LED_Show_Update_Failed();
			break;
		case LED_R_SCAN_FAST:
			if((led_scan_cnt % 15) == 0 ) { GPIO_LED_Show_Error(); }
			break;
		case LED_R_SCAN_SLOW:
			if((led_scan_cnt % 50) == 0 ) { GPIO_LED_Show_Error(); }
			break;
		case LED_SCAN_ALL_OFF:
			GPIO_LED_Show_All_OFF();
			break;
		default:
			break;
	}
	
	if(led_scan_cnt > 1000) { led_scan_cnt = 0; }	
}


void FLASH_Read(u8 *buff, u32 addr, u32 readNumber)
{
	u32 temp;
	u32 Address;
	u16 halfword;

	Address = addr;

	for (temp = 0; temp < readNumber; temp+=2)
	{
		halfword = *((vu16*)Address) & 0xFFFF;
		buff[temp] = halfword & 0xFF;
		buff[temp+1] = (halfword>>8) & 0xFF;
		Address+=2;
	}
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

void Flash_Erase_Page(uint32_t Address)
{
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(Address);
	FLASH_Lock();	
}

void Reset_Periph(void)
{
//	UART_DeInit(UART1);                            //��λUART1
//	NVIC_DisableIRQ(UART1_IRQn);                   //�ر�UART1�ж�
//  NVIC_DisableIRQ(SysTick_IRQn);                 //�ر�systick�ж�
	NVIC_DisableIRQ(TIM14_IRQn);
	__disable_irq();                               //��ת֮ǰ�ر����жϣ���ֹ�����жϵ�������
}

void Reset_MCU(void)
{
	NVIC_SystemReset(); //ϵͳ��λ
}

/***************************************************************************************************************/
typedef struct
{
	uint32_t start_addr;    // ����ʼ��ַ
	uint32_t number;        // �������
	uint32_t size;          // ������С

} page_t;

#define KB     (1024)

static uint32_t xmodem_actual_flash_address = ApplicationAddress; 

page_t GetPage(uint32_t Address)
{
	page_t page;

	if((Address & 0x8FF0000) == FLASH_BASE)
	{
		page.number = ((Address & 0x0000FF00) >> 10);
		page.start_addr = FLASH_BASE + page.number * KB;
		page.size = 1 * KB;
	}

	return page;
}

/**
  * @brief  ���ݵ�ַ�ͳ��Ȳ�����������
  * @param  start_address ��Ҫд�����ʼ��ַ
	* @param  len ������
  * @retval ��
  */
int erasure_sector(uint32_t start_address, uint32_t len)
{
    page_t page_info;                       // ��¼��������Ϣ
    uint32_t page_size  = 0;                // ��¼���������Ĵ�С
    uint32_t address = start_address;
    
    page_info = GetPage(start_address);     // ��ȡ��һ����������Ϣ
    
    //printf("page_info.number = %d\r\n",page_info.number);
    //printf("page_info.start_addr = 0x%X\r\n",page_info.start_addr);
    //printf("page_info.size = %d\r\n",page_info.size);
	
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

	if ((page_info.start_addr + page_info.size) - (start_address + len) > 0) 	// ��һ���������㹻��������
	{
		//printf("[Erase][%d] Enough space!\r\n",page_info.number);
		FLASH_ErasePage(page_info.start_addr);
		#if 0
		{
			/* ��FLASH��������ֹ���ݱ��۸�*/
			FLASH_Lock();
			/*�����������أ�ʵ��Ӧ���пɼ��봦�� */
			return -1;
		}
		#endif
	}
	else{
			page_size += (page_info.start_addr + page_info.size) - start_address;    // ��¼��һ�������Ĵ�С
			address += page_size;                // ƫ������һ����������ʼ��ַ
		
			do
			{
				page_info = GetPage(address);    // ��ȡһ����������Ϣ

				//printf("[Erase][%d] Not enough space!\r\n",page_info.number);

				FLASH_ErasePage(page_info.start_addr);
				#if 0
				{
					/* ��FLASH��������ֹ���ݱ��۸�*/
					FLASH_Lock();
					/*�����������أ�ʵ��Ӧ���пɼ��봦�� */
					return -1;
				}
				#endif

				address += page_info.size;    // ƫ������һ����������ʼ��ַ
				page_size += page_info.size;  // ��¼���������Ĵ�С
			}
			while (page_size >= len);        // �������������Դ������������
		}

		/* ��FLASH��������ֹ���ݱ��۸�*/
		FLASH_Lock(); 
		
		return 0;
}

/**
  * @brief  ���ݵ�ַ�ͳ��Ȳ�����������
  * @param  start_address ��Ҫд�����ʼ��ַ
  * @param  *data : ��Ҫ���������
	* @param  len ������
  * @retval ��
  */
int flash_write_data(uint32_t start_address, const uint8_t *data, uint32_t len)
{
	u32 temp;
	u32 WriteAddress;
	u32 writeNumber;
	u16 HalfWord;

	WriteAddress = start_address;

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	if(len % 2 == 0)
	{
		writeNumber = len;
		for (temp = 0; temp < writeNumber; temp+=2)
		{
			HalfWord = data[temp];
			HalfWord = HalfWord | ((u16)data[temp + 1] << 8);
			FLASH_ProgramHalfWord(WriteAddress, HalfWord);
			WriteAddress = WriteAddress + 2;
		}
	}
	else{
		printf("input data is odd!\n");
	}
	FLASH_Lock();

	return 0;
}

/***************************************************************************************************************/
/**
 * @brief   Xmodem ����Ҫ����������ݵ�����.
 * @param   address �����ݵ�ַ����������
 * @return  ���ص�ǰ����ʣ��Ĵ�С
 */
uint32_t x_receive_flash_erasure(uint32_t address)
{
  page_t page_info;
  
  if(erasure_sector(address, 1))   // ������ǰ��ַ��������
  {
	  return 0;    // ����ʧ��
  }

  page_info = GetPage(address);     // �õ���ǰ��������Ϣ

  return (page_info.size + page_info.start_addr - address); // ���ص�ǰ����ʣ���С
}

/**
  * @brief  Xmodem �����ܵ������ݱ��浽flash.
  * @param  start_address ��Ҫд�����ʼ��ַ
  * @param  *data : ��Ҫ���������
	* @param  len ������
  * @return д��״̬
 */
int x_receive_flash_writea(uint32_t start_address, const void *data, uint32_t len)
{
  if (flash_write_data(start_address, (uint8_t *)data, len) == 0)    // ������ǰ��ַ��������
  {
		return 0;    // д��ɹ�
  }
  else
  {
		return -1;    // д��ʧ��
  }
}

/**
 * @brief   �ļ����ݽ�����ɻص�.
 * @param   *ptr: ���ƾ��.
 * @param   *file_name: �ļ�����.
 * @param   file_size: �ļ���С����Ϊ0xFFFFFFFF����˵����С��Ч.
 * @return  ����д��Ľ����0���ɹ���-1��ʧ��.
 */
int receive_file_data_callback(void *ptr, char *file_data, uint32_t w_size)
{
    static uint32_t page_remain_size = 0;                  /* ����ʣ���С. */

    /* ��ǰ���������˲�����һ��. */
    if (page_remain_size <= w_size)
    {
		page_remain_size += x_receive_flash_erasure(xmodem_actual_flash_address + page_remain_size);

		if (0 == page_remain_size)
		{
			return -1;
		}
		//printf("[FLASH Erase] page_remain_size = %d\r\n",page_remain_size);
    }

    if (x_receive_flash_writea(xmodem_actual_flash_address, file_data, w_size) == 0)
    {
		xmodem_actual_flash_address += w_size;
		if(page_remain_size < w_size) { return - 1; }
		page_remain_size -= w_size;
		//printf("[FLASH Write] page_remain_size = %d\r\n",page_remain_size);
		return 0;
    }
    else 
    {
		return -1;
    }
}

/**
 * @brief   �ļ�������ɻص�.
 * @param   *ptr: ���ƾ��.
 * @return  ����д��Ľ����0���ɹ���-1��ʧ��.
 */
int receive_file_callback(void *ptr)
{
    return 0;
}

// ���APP�Ƿ���Ч ��֤APPջ����ַ�͸�λ�����Ƿ��ںϷ���Χ
bool is_app_valid(void) {
    // APP��ʼ��ַ���������ApplicationAddressһ�£�
    u32 app_stack_top = *(u32*)ApplicationAddress;
    u32 app_reset_vec = *(u32*)(ApplicationAddress + 4);
    
    // ���ջ���Ƿ���RAM��Χ
    // ��鸴λ�����Ƿ���Flash������
    if ((app_stack_top >= 0x20000000 && app_stack_top <= 0x20040000) &&
        (app_reset_vec >= 0x08000000 && app_reset_vec <= 0x08100000)) {
        return true;
    }
    return false;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval  0.
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    u8 flag[4], i;
    bool upgrade_requested = false;
    bool upgrade_success = false;
    bool app_valid = is_app_valid(); // ���ж�APP�Ƿ���Ч
    
    LED_SCAN_MODE = LED_SCAN_ALL_OFF;
    DELAY_Init();
    PLATFORM_Init();

    // ��ȡ������־
    FLASH_Read(flag, BootJumpFlagAddress, 4);
    
    printf("\r\n[Bootloader] Flash Read:\r\n");
    for(i = 0; i < 4; i++) { printf("[%d] = 0x%02X ",i,flag[i]); }

    // ����Ƿ��������������ȴ���������
    if(strncmp((char *)flag,(char *)Valid_buf,4) == 0)
    {
        printf("\r\n����������Valid����׼������...\r\n");
        LED_SCAN_MODE = LED_G_SCAN_SLOW;
        upgrade_requested = true;
    }
    else if(strncmp((char *)flag,(char *)Blank_buf,4) == 0)
    {
        printf("\r\n����������Blank����׼������...\r\n");
        LED_SCAN_MODE = LED_R_SCAN_SLOW;
        upgrade_requested = true;
    }
    else
    {
        printf("\r\n���������󣬼��APP״̬...\r\n");
    }

    // ִ���������̣���������������ʱ��
    if(upgrade_requested)
    {
        __asm("CPSID I"); // �����жϱ�����־
        
        FLASH_Read(flag, BootJumpFlagAddress, 4);
        if(!(strncmp((char *)flag,(char *)Valid_buf,4) == 0 || 
             strncmp((char *)flag,(char *)Blank_buf,4) == 0))
        {
            printf("\r\n������־�쳣��ȡ������\r\n");
            __asm("CPSIE I");
            goto jump_to_app;
        }
        
        __asm("CPSIE I"); // �ָ��ж�
        
        // ִ����������
        if(xmodem_receive() == 0)
        {
            // �����ɹ�����֤APP�Ƿ���Ч
            if (is_app_valid()) {
                LED_SCAN_MODE = LED_G_ALWAYS_ON;
                DELAY_Ms(800);
                upgrade_success = true;
                printf("\r\n�����ɹ���APP��֤��Ч\r\n");
            } else {
                LED_SCAN_MODE = LED_R_ALWAYS_ON;
                DELAY_Ms(800);
                printf("\r\n����ʧ�ܣ�APPд����Ч\r\n");
            }
        }
        else
        {
            LED_SCAN_MODE = LED_R_ALWAYS_ON;
            DELAY_Ms(800);
            printf("\r\n����ʧ�ܣ�Xmodem����ʧ�ܣ���ʱ�����\r\n");
        }
    }

jump_to_app:
    // �����ɹ���������λ
    if(upgrade_success)
    {
        __asm("CPSID I");
        
        // ���������־
        printf("\r\n���������־...\r\n");
        Flash_Erase_Page(BootJumpFlagAddress);
        FLASH_Write(Claer_Flag_buf, BootJumpFlagAddress, sizeof(Claer_Flag_buf));
        
        __asm("CPSIE I");
        
        printf("\r\n2����Զ���λ...\r\n");
        DELAY_Ms(2000);
        
        Reset_Periph();
        __DSB();
        // ����ϵͳ��λ
        SCB->AIRCR = (0x5FA << SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk;
        while(1);
    }
    
    // ��תǰ������֤APP��Ч��
    if (is_app_valid()) {
        printf("\r\nAPP��Ч����ת��APP\r\n");
        Reset_Periph();
        __set_MSP(*(u32*)ApplicationAddress);
        ((void(*)(void))*(u32*)(ApplicationAddress + 4))();
    } else {
        // ����ЧAPP��ͣ��Boot�ȴ��´�����
        printf("\r\n����ЧAPP���ȴ�����...\r\n");
        LED_SCAN_MODE = LED_R_SCAN_FAST; // ������˸��ʾ�ȴ�����
        while(1); // ����ת�������ȴ�����
    }
    
    while (1);
}
/// @}

/// @}

/// @}

