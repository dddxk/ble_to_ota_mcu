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
//	UART_DeInit(UART1);                            //复位UART1
//	NVIC_DisableIRQ(UART1_IRQn);                   //关闭UART1中断
//  NVIC_DisableIRQ(SysTick_IRQn);                 //关闭systick中断
	NVIC_DisableIRQ(TIM14_IRQn);
	__disable_irq();                               //跳转之前关闭总中断，防止意外中断导致死机
}

void Reset_MCU(void)
{
	NVIC_SystemReset(); //系统复位
}

/***************************************************************************************************************/
typedef struct
{
	uint32_t start_addr;    // 的起始地址
	uint32_t number;        // 扇区编号
	uint32_t size;          // 扇区大小

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
  * @brief  根据地址和长度擦除所需扇区
  * @param  start_address ：要写入的起始地址
	* @param  len ：长度
  * @retval 无
  */
int erasure_sector(uint32_t start_address, uint32_t len)
{
    page_t page_info;                       // 记录扇区的信息
    uint32_t page_size  = 0;                // 记录所需扇区的大小
    uint32_t address = start_address;
    
    page_info = GetPage(start_address);     // 获取第一个扇区的信息
    
    //printf("page_info.number = %d\r\n",page_info.number);
    //printf("page_info.start_addr = 0x%X\r\n",page_info.start_addr);
    //printf("page_info.size = %d\r\n",page_info.size);
	
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

	if ((page_info.start_addr + page_info.size) - (start_address + len) > 0) 	// 第一个扇区就足够存下数据
	{
		//printf("[Erase][%d] Enough space!\r\n",page_info.number);
		FLASH_ErasePage(page_info.start_addr);
		#if 0
		{
			/* 给FLASH上锁，防止内容被篡改*/
			FLASH_Lock();
			/*擦除出错，返回，实际应用中可加入处理 */
			return -1;
		}
		#endif
	}
	else{
			page_size += (page_info.start_addr + page_info.size) - start_address;    // 记录第一个扇区的大小
			address += page_size;                // 偏移至下一个扇区的起始地址
		
			do
			{
				page_info = GetPage(address);    // 获取一个扇区的信息

				//printf("[Erase][%d] Not enough space!\r\n",page_info.number);

				FLASH_ErasePage(page_info.start_addr);
				#if 0
				{
					/* 给FLASH上锁，防止内容被篡改*/
					FLASH_Lock();
					/*擦除出错，返回，实际应用中可加入处理 */
					return -1;
				}
				#endif

				address += page_info.size;    // 偏移至下一个扇区的起始地址
				page_size += page_info.size;  // 记录擦除扇区的大小
			}
			while (page_size >= len);        // 擦除的扇区可以存下所需的数据
		}

		/* 给FLASH上锁，防止内容被篡改*/
		FLASH_Lock(); 
		
		return 0;
}

/**
  * @brief  根据地址和长度擦除所需扇区
  * @param  start_address ：要写入的起始地址
  * @param  *data : 需要保存的数据
	* @param  len ：长度
  * @retval 无
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
 * @brief   Xmodem 擦除要保存接收数据的扇区.
 * @param   address ：根据地址来擦除扇区
 * @return  返回当前扇区剩余的大小
 */
uint32_t x_receive_flash_erasure(uint32_t address)
{
  page_t page_info;
  
  if(erasure_sector(address, 1))   // 擦除当前地址所在扇区
  {
	  return 0;    // 擦除失败
  }

  page_info = GetPage(address);     // 得到当前扇区的信息

  return (page_info.size + page_info.start_addr - address); // 返回当前扇区剩余大小
}

/**
  * @brief  Xmodem 将接受到的数据保存到flash.
  * @param  start_address ：要写入的起始地址
  * @param  *data : 需要保存的数据
	* @param  len ：长度
  * @return 写入状态
 */
int x_receive_flash_writea(uint32_t start_address, const void *data, uint32_t len)
{
  if (flash_write_data(start_address, (uint8_t *)data, len) == 0)    // 擦除当前地址所在扇区
  {
		return 0;    // 写入成功
  }
  else
  {
		return -1;    // 写入失败
  }
}

/**
 * @brief   文件数据接收完成回调.
 * @param   *ptr: 控制句柄.
 * @param   *file_name: 文件名字.
 * @param   file_size: 文件大小，若为0xFFFFFFFF，则说明大小无效.
 * @return  返回写入的结果，0：成功，-1：失败.
 */
int receive_file_data_callback(void *ptr, char *file_data, uint32_t w_size)
{
    static uint32_t page_remain_size = 0;                  /* 扇区剩余大小. */

    /* 当前扇区不够了擦除下一个. */
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
 * @brief   文件接收完成回调.
 * @param   *ptr: 控制句柄.
 * @return  返回写入的结果，0：成功，-1：失败.
 */
int receive_file_callback(void *ptr)
{
    return 0;
}

// 检查APP是否有效 验证APP栈顶地址和复位向量是否在合法范围
bool is_app_valid(void) {
    // APP起始地址（需与你的ApplicationAddress一致）
    u32 app_stack_top = *(u32*)ApplicationAddress;
    u32 app_reset_vec = *(u32*)(ApplicationAddress + 4);
    
    // 检查栈顶是否在RAM范围
    // 检查复位向量是否在Flash代码区
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
    bool app_valid = is_app_valid(); // 先判断APP是否有效
    
    LED_SCAN_MODE = LED_SCAN_ALL_OFF;
    DELAY_Init();
    PLATFORM_Init();

    // 读取升级标志
    FLASH_Read(flag, BootJumpFlagAddress, 4);
    
    printf("\r\n[Bootloader] Flash Read:\r\n");
    for(i = 0; i < 4; i++) { printf("[%d] = 0x%02X ",i,flag[i]); }

    // 检查是否有升级请求（优先处理升级）
    if(strncmp((char *)flag,(char *)Valid_buf,4) == 0)
    {
        printf("\r\n有升级请求（Valid），准备升级...\r\n");
        LED_SCAN_MODE = LED_G_SCAN_SLOW;
        upgrade_requested = true;
    }
    else if(strncmp((char *)flag,(char *)Blank_buf,4) == 0)
    {
        printf("\r\n有升级请求（Blank），准备升级...\r\n");
        LED_SCAN_MODE = LED_R_SCAN_SLOW;
        upgrade_requested = true;
    }
    else
    {
        printf("\r\n无升级请求，检查APP状态...\r\n");
    }

    // 执行升级流程（仅当有升级请求时）
    if(upgrade_requested)
    {
        __asm("CPSID I"); // 禁用中断保护标志
        
        FLASH_Read(flag, BootJumpFlagAddress, 4);
        if(!(strncmp((char *)flag,(char *)Valid_buf,4) == 0 || 
             strncmp((char *)flag,(char *)Blank_buf,4) == 0))
        {
            printf("\r\n升级标志异常，取消升级\r\n");
            __asm("CPSIE I");
            goto jump_to_app;
        }
        
        __asm("CPSIE I"); // 恢复中断
        
        // 执行升级流程
        if(xmodem_receive() == 0)
        {
            // 升级成功后，验证APP是否有效
            if (is_app_valid()) {
                LED_SCAN_MODE = LED_G_ALWAYS_ON;
                DELAY_Ms(800);
                upgrade_success = true;
                printf("\r\n升级成功，APP验证有效\r\n");
            } else {
                LED_SCAN_MODE = LED_R_ALWAYS_ON;
                DELAY_Ms(800);
                printf("\r\n升级失败：APP写入无效\r\n");
            }
        }
        else
        {
            LED_SCAN_MODE = LED_R_ALWAYS_ON;
            DELAY_Ms(800);
            printf("\r\n升级失败：Xmodem传输失败（超时或错误）\r\n");
        }
    }

jump_to_app:
    // 升级成功：主动复位
    if(upgrade_success)
    {
        __asm("CPSID I");
        
        // 清除升级标志
        printf("\r\n清除升级标志...\r\n");
        Flash_Erase_Page(BootJumpFlagAddress);
        FLASH_Write(Claer_Flag_buf, BootJumpFlagAddress, sizeof(Claer_Flag_buf));
        
        __asm("CPSIE I");
        
        printf("\r\n2秒后自动复位...\r\n");
        DELAY_Ms(2000);
        
        Reset_Periph();
        __DSB();
        // 触发系统复位
        SCB->AIRCR = (0x5FA << SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk;
        while(1);
    }
    
    // 跳转前必须验证APP有效性
    if (is_app_valid()) {
        printf("\r\nAPP有效，跳转至APP\r\n");
        Reset_Periph();
        __set_MSP(*(u32*)ApplicationAddress);
        ((void(*)(void))*(u32*)(ApplicationAddress + 4))();
    } else {
        // 无有效APP：停在Boot等待下次升级
        printf("\r\n无有效APP，等待升级...\r\n");
        LED_SCAN_MODE = LED_R_SCAN_FAST; // 快速闪烁表示等待升级
        while(1); // 不跳转，持续等待升级
    }
    
    while (1);
}
/// @}

/// @}

/// @}

