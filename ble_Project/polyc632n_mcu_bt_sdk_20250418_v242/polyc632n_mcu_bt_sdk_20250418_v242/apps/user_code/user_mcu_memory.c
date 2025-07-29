#include "virtual_mcu_main.h"
#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_memory.h"
#include "user_mcu_memory.h"



#if MCU_MEMORY_ENABLE

u8 mcu_mem_id2_buf[5] = {1,2,3,4,0};

///-------------------------测试内部存储器的读写----------------------------------
void mcu_memory_init(void)
{
    int ret;
    int len;

    len = sizeof(mcu_mem_id2_buf);

    printf("--mcu_memory_test--\n");
    ret = polyc_read_memory(VIRTUAL_MCU_MEM_ID_2, &mcu_mem_id2_buf, len);  ///读取ID2存储的有效数据和有效数据长度
    printf("mem_ret = %d\n",ret);
    if(ret < 0){///读取错误,ID2数据为空，没有写入过有效数据
        printf("mem_null_init!!!\n");
        polyc_write_memory(VIRTUAL_MCU_MEM_ID_2, &mcu_mem_id2_buf, len);  ///因没有写如果数据，开始初始化写入数据
    }else if(ret == len){///读取正确有效数据和有效长度，ret反馈的值就是有效存储长度的值。
        ///读取正确有效数据和有效长度，ret反馈的值就是有效存储长度的值。
        printf("read_mcu_mem_memory_ok:\n");
        put_buf(mcu_mem_id2_buf, len);    ///此语句只是打印读取到的有效数据
    }else{///读取错误
        printf("read_mcu_mem_memory_err!!!\n");
        #if 0 //根据实际应用场景,决定是否清0,重新写入数据
        memset(&mcu_mem_id2_buf, 0, len); ///清0
        polyc_write_memory(VIRTUAL_MCU_MEM_ID_2, &mcu_mem_id2_buf, len);  ///写入数据
        #endif
    }
}


#endif


