#ifndef _VIRTUAL_MCU_MEMORY_H_
#define _VIRTUAL_MCU_MEMORY_H_



/*---------------------------------------------------------------------------------------------------*/
/*                                      MCU�ڲ��洢,���粻��ʧ                                       */
/*---------------------------------------------------------------------------------------------------*/
#define VM_MCU_MEM_ITEM_ID_MIN			            (0)
#define VM_MCU_MEM_ITEM_ID_MAX			            (30)
#define VM_MCU_MEM_ITEM_DATA_LEN_MAX	            (64)
#define VM_ERR_MEM_ITEM_ID_OUT_OF_RANGE             (0xFFFF)
#define VM_ERR_MEM_ITEM_ID_DATA_LEN_OUT_OF_RANGE    (0xFFFE)
/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    vm_mcu_memд������
   @param    item_id ȡֵ��Χ 0~30
   @param    *buff д������ָ��
   @param    buff_size ȡֵ��Χ 0~64
   @return   ����ֵ:0xFFFF,��ʾitem_idȡֵ������Χ;
             ����ֵ:0xFFFE,��ʾbuff_sizeȡֵ������Χ;
             ����ֵ:0~64,��ʾ�ɹ�д�����ݵĳ���;
             ����ֵΪ����ֵ,���ʾ�������;
   @note     ������Ҫ���ж��е���
*/
/*---------------------------------------------------------------------------------------------------*/
int polyc_write_memory(u8 item_id,u8 *buff,u16 buff_size);
/*---------------------------------------------------------------------------------------------------*/
/*
   @brief    vm_mcu_memд������
   @param    item_id ȡֵ��Χ 0~30
   @param    *buff ��ȡ����ָ��
   @param    buff_size ȡֵ��Χ 0~64
   @return   ����ֵ:0xFFFF,��ʾitem_idȡֵ������Χ;
             ����ֵ:0xFFFE,��ʾbuff_sizeȡֵ������Χ;
             ����ֵ:0~64,��ʾ�ɹ���ȡ���ݵĳ���;
             ����ֵΪ����ֵ,���ʾ�������;
   @note     ������Ҫ���ж��е���
*/
/*---------------------------------------------------------------------------------------------------*/
int polyc_read_memory(u8 item_id,u8 *buff,u16 buff_size);



///VIRTUAL_MCU_MEM_ID�ڲ��洢��Χ0~30��ÿ��ID����ܴ�64���ֽ�
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

