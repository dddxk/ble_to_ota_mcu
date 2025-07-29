#include "virtual_mcu_main.h"
#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_memory.h"
#include "user_mcu_memory.h"



#if MCU_MEMORY_ENABLE

u8 mcu_mem_id2_buf[5] = {1,2,3,4,0};

///-------------------------�����ڲ��洢���Ķ�д----------------------------------
void mcu_memory_init(void)
{
    int ret;
    int len;

    len = sizeof(mcu_mem_id2_buf);

    printf("--mcu_memory_test--\n");
    ret = polyc_read_memory(VIRTUAL_MCU_MEM_ID_2, &mcu_mem_id2_buf, len);  ///��ȡID2�洢����Ч���ݺ���Ч���ݳ���
    printf("mem_ret = %d\n",ret);
    if(ret < 0){///��ȡ����,ID2����Ϊ�գ�û��д�����Ч����
        printf("mem_null_init!!!\n");
        polyc_write_memory(VIRTUAL_MCU_MEM_ID_2, &mcu_mem_id2_buf, len);  ///��û��д������ݣ���ʼ��ʼ��д������
    }else if(ret == len){///��ȡ��ȷ��Ч���ݺ���Ч���ȣ�ret������ֵ������Ч�洢���ȵ�ֵ��
        ///��ȡ��ȷ��Ч���ݺ���Ч���ȣ�ret������ֵ������Ч�洢���ȵ�ֵ��
        printf("read_mcu_mem_memory_ok:\n");
        put_buf(mcu_mem_id2_buf, len);    ///�����ֻ�Ǵ�ӡ��ȡ������Ч����
    }else{///��ȡ����
        printf("read_mcu_mem_memory_err!!!\n");
        #if 0 //����ʵ��Ӧ�ó���,�����Ƿ���0,����д������
        memset(&mcu_mem_id2_buf, 0, len); ///��0
        polyc_write_memory(VIRTUAL_MCU_MEM_ID_2, &mcu_mem_id2_buf, len);  ///д������
        #endif
    }
}


#endif


