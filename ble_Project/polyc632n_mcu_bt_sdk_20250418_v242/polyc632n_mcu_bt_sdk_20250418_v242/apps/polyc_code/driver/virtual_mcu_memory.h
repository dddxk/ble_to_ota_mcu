#ifndef _VIRTUAL_MCU_MEMORY_H_
#define _VIRTUAL_MCU_MEMORY_H_



/*---------------------------------------------------------------------------------------------------*/
/*                                      MCU内部存储,掉电不丢失                                       */
/*---------------------------------------------------------------------------------------------------*/
#define VM_MCU_MEM_ITEM_ID_MIN			            (0)
#define VM_MCU_MEM_ITEM_ID_MAX			            (30)
#define VM_MCU_MEM_ITEM_DATA_LEN_MAX	            (64)
#define VM_ERR_MEM_ITEM_ID_OUT_OF_RANGE             (0xFFFF)
#define VM_ERR_MEM_ITEM_ID_DATA_LEN_OUT_OF_RANGE    (0xFFFE)
/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    vm_mcu_mem写入数据
   @param    item_id 取值范围 0~30
   @param    *buff 写入数据指针
   @param    buff_size 取值范围 0~64
   @return   返回值:0xFFFF,表示item_id取值超出范围;
             返回值:0xFFFE,表示buff_size取值超出范围;
             返回值:0~64,表示成功写入数据的长度;
             返回值为其他值,则表示错误代码;
   @note     函数不要在中断中调用
*/
/*---------------------------------------------------------------------------------------------------*/
int polyc_write_memory(u8 item_id,u8 *buff,u16 buff_size);
/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    vm_mcu_mem写入数据
   @param    item_id 取值范围 0~30
   @param    *buff 读取数据指针
   @param    buff_size 取值范围 0~64
   @return   返回值:0xFFFF,表示item_id取值超出范围;
             返回值:0xFFFE,表示buff_size取值超出范围;
             返回值:0~64,表示成功读取数据的长度;
             返回值为其他值,则表示错误代码;
   @note     函数不要在中断中调用
*/
/*---------------------------------------------------------------------------------------------------*/
int polyc_read_memory(u8 item_id,u8 *buff,u16 buff_size);



///VIRTUAL_MCU_MEM_ID内部存储范围0~30，每个ID最多能存64个字节
enum
{
    VIRTUAL_MCU_MEM_ID_1      = VM_MCU_MEM_ITEM_ID_MIN,
    VIRTUAL_MCU_MEM_ID_2,
    VIRTUAL_MCU_MEM_ID_3,
    VIRTUAL_MCU_MEM_ID_4,
    VIRTUAL_MCU_MEM_ID_5,
    VIRTUAL_MCU_MEM_ID_6,
    VIRTUAL_MCU_MEM_ID_7,
    VIRTUAL_MCU_MEM_ID_8,
    VIRTUAL_MCU_MEM_ID_9,
    VIRTUAL_MCU_MEM_ID_10,
    VIRTUAL_MCU_MEM_ID_11,
    VIRTUAL_MCU_MEM_ID_12,
    VIRTUAL_MCU_MEM_ID_13,
    VIRTUAL_MCU_MEM_ID_14,
    VIRTUAL_MCU_MEM_ID_15,
    VIRTUAL_MCU_MEM_ID_16,
    VIRTUAL_MCU_MEM_ID_17,
    VIRTUAL_MCU_MEM_ID_18,
    VIRTUAL_MCU_MEM_ID_19,
    VIRTUAL_MCU_MEM_ID_20,
    VIRTUAL_MCU_MEM_ID_21,
    VIRTUAL_MCU_MEM_ID_22,
    VIRTUAL_MCU_MEM_ID_23,
    VIRTUAL_MCU_MEM_ID_24,
    VIRTUAL_MCU_MEM_ID_25,
    VIRTUAL_MCU_MEM_ID_26,
    VIRTUAL_MCU_MEM_ID_27,
    VIRTUAL_MCU_MEM_ID_28,
    VIRTUAL_MCU_MEM_ID_29,
    VIRTUAL_MCU_MEM_ID_30,

    VIRTUAL_MCU_MEM_ID_MAX    = VM_MCU_MEM_ITEM_ID_MAX,
};

#endif

