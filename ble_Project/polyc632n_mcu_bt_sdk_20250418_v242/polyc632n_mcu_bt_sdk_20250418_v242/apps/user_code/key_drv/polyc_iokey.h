#ifndef __POLYC_IOKEY_H__
#define __POLYC_IOKEY_H__


#define IO_KEY_EN       0



//������Ҫ��ʱ���Ժ�ֱ�Ӹı��Ĳ�����ok��!
#define KEY_SHORT_CNT     3  //�̰���׼
#define KEY_LONG_CNT      100
#define KEY_HOLD_CNT      KEY_LONG_CNT + 25
#define KEY_RELEASE_CNT   5  //����


//���尴������ȥ��״̬

#define KEY_SHORT       0x01 //ִ��һ�ζ̰�֮���ɿ���֮����Ҫ��Ӧ
#define KEY_SHORT_UP    0x02
#define KEY_LONG        0x03 //����
#define KEY_LONG_UP     0x04//�������ֺ�ִ����Ϣ
#define KEY_HOLD        0x05//�������ţ���Ҫ��������Ϣ

#define KEY_NO_STATE    0xff

#define   IO_KEY_NO_VALUE    0x00
#define   IO_KEY_0           0x01
#define   IO_KEY_1           0x02
#define   IO_KEY_2           0x04
#define   IO_KEY_3           0x08
#define   IO_KEY_4           0x10
#define   IO_KEY_5           0x20


#if  IO_KEY_EN
void key_io_init();

void io_key_scan();//20ms

#endif // IO_KEY_EN

#endif // __POLYC_IOKEY_H__


