#ifndef __VIRTUAL_MCU_SPI_H__
#define __VIRTUAL_MCU_SPI_H__


/*
    ����2��spi���
*/
#define     SPI1_HDL        1
#define     SPI2_HDL        2

#define     POLYC_HW_SPI1_BAUD        8000000L  //8M
#define     POLYC_HW_SPI2_BAUD        8000000L  //8M


typedef const int spi_dev;


/********************
struct spi_platform_data {
    u8 port[3 + 2];//CLK, DO, DI D2(wp) D3(hold)
    u8 mode;  //ģʽ��ѡ��Ϊenum spi_mode�е�ö�ٳ���
    u8 role;  //��ɫ��ѡ��Ϊenum spi_role�е�ö�ٳ���
    u32 clk;  //������
};

*********************/
/*
 * @brief ö��spiģʽ
 */
enum polyc_spi_mode {
    POLYC_SPI_MODE_BIDIR_1BIT,    //֧��SPIx(x=0,1,2)��ȫ˫����di���գ�do����
    POLYC_SPI_MODE_UNIDIR_1BIT,   //֧��SPIx(x=0,1,2)����˫����do��ʱ����/����
};
/*
 * @brief ö��spi��ɫ-����
 */
enum polyc_spi_role {
    POLYC_SPI_ROLE_MASTER,
    POLYC_SPI_ROLE_SLAVE,
};
/*
 * @brief ��spi
 * @parm spi  spi���
 * @return null
ע�⣺��Ҫ��������ʼ�����֮�󣬲��ܴ�spi,��Ȼʱ�ӻ᲻׼
 */

void polyc_spi_open(spi_dev spi);



/*
 * @brief �ر�spi
 * @parm spi  spi���
 * @return null
 */
void polyc_spi_close(spi_dev spi);



/*
 * @brief ����1���ֽ�
 * @parm spi  spi���
 * @parm byte ���͵��ֽ�
 * @return 0 �ɹ���< 0 ʧ��
 */
int polyc_spi_send_byte(spi_dev spi, u8 byte);



/*
 * @brief spi dma����
 * @parm spi  spi���
 * @parm buf  ���ͻ���������ַ
 * @parm len  �������ͳ���
 * @return ʵ�ʷ��ͳ��ȣ�< 0��ʾʧ��
 */
int polyc_spi_dma_send(spi_dev spi, const void *buf, u32 len);


/*
 * @brief spi dma����
 * @parm spi  spi���
 * @parm buf  ���ջ���������ַ
 * @parm len  �������ճ���
 * @return ʵ�ʽ��ճ��ȣ�< 0��ʾʧ��
 */
int polyc_spi_dma_recv(spi_dev spi, void *buf, u32 len);



/*
 * @brief ���ò�����
 * @parm spi  spi���
 * @parm baud  ������
 * @return 0 �ɹ���< 0 ʧ��
 */
int polyc_spi_set_baud(spi_dev spi, u32 baud);

/*
 * @brief ��ȡ������
 * @parm spi  spi���
 * @return  ������
 */
u32 polyc_spi_get_baud(spi_dev spi);


#endif
