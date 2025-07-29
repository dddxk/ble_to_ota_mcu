#ifndef __VIRTUAL_MCU_SPI_H__
#define __VIRTUAL_MCU_SPI_H__


/*
    定义2个spi句柄
*/
#define     SPI1_HDL        1
#define     SPI2_HDL        2

#define     POLYC_HW_SPI1_BAUD        8000000L  //8M
#define     POLYC_HW_SPI2_BAUD        8000000L  //8M


typedef const int spi_dev;


/********************
struct spi_platform_data {
    u8 port[3 + 2];//CLK, DO, DI D2(wp) D3(hold)
    u8 mode;  //模式，选项为enum spi_mode中的枚举常量
    u8 role;  //角色，选项为enum spi_role中的枚举常量
    u32 clk;  //波特率
};

*********************/
/*
 * @brief 枚举spi模式
 */
enum polyc_spi_mode {
    POLYC_SPI_MODE_BIDIR_1BIT,    //支持SPIx(x=0,1,2)，全双工，di接收，do发送
    POLYC_SPI_MODE_UNIDIR_1BIT,   //支持SPIx(x=0,1,2)，半双工，do分时发送/接收
};
/*
 * @brief 枚举spi角色-主从
 */
enum polyc_spi_role {
    POLYC_SPI_ROLE_MASTER,
    POLYC_SPI_ROLE_SLAVE,
};
/*
 * @brief 打开spi
 * @parm spi  spi句柄
 * @return null
注意：需要在蓝牙初始化完成之后，才能打开spi,不然时钟会不准
 */

void polyc_spi_open(spi_dev spi);



/*
 * @brief 关闭spi
 * @parm spi  spi句柄
 * @return null
 */
void polyc_spi_close(spi_dev spi);



/*
 * @brief 发送1个字节
 * @parm spi  spi句柄
 * @parm byte 发送的字节
 * @return 0 成功，< 0 失败
 */
int polyc_spi_send_byte(spi_dev spi, u8 byte);



/*
 * @brief spi dma发送
 * @parm spi  spi句柄
 * @parm buf  发送缓冲区基地址
 * @parm len  期望发送长度
 * @return 实际发送长度，< 0表示失败
 */
int polyc_spi_dma_send(spi_dev spi, const void *buf, u32 len);


/*
 * @brief spi dma接收
 * @parm spi  spi句柄
 * @parm buf  接收缓冲区基地址
 * @parm len  期望接收长度
 * @return 实际接收长度，< 0表示失败
 */
int polyc_spi_dma_recv(spi_dev spi, void *buf, u32 len);



/*
 * @brief 设置波特率
 * @parm spi  spi句柄
 * @parm baud  波特率
 * @return 0 成功，< 0 失败
 */
int polyc_spi_set_baud(spi_dev spi, u32 baud);

/*
 * @brief 获取波特率
 * @parm spi  spi句柄
 * @return  波特率
 */
u32 polyc_spi_get_baud(spi_dev spi);


#endif
