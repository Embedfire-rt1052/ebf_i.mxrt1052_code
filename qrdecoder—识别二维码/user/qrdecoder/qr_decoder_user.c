/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ��ά�����API
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
#include "qr_decoder_user.h"

//���ݻ�������
//1M�ֽ�
unsigned short  dcmi_buf[1024*1024]    __attribute((section("NonCacheable")));
//1M�ֽ�
unsigned short  qr_img_buf[1024*1024]  __attribute((section("NonCacheable")));
/*512k�ֽ�*/
unsigned char   img_now[512*1024]     __attribute((section("NonCacheable")));

//�������ݷ�װΪ��ά����decoded_buf����ʽΪ��
// ����һ�飺�������ͳ���(8bit)+������������+�������ݳ���(16bit,��λ��ǰ��λ�ں�)+�������ݣ�
// ���ڶ��飺�������ͳ���(8bit)+������������+�������ݳ���(16bit,��λ��ǰ��λ�ں�)+�������ݣ�
//  ������
//�Դ�����
char decoded_buf[QR_SYMBOL_NUM][QR_SYMBOL_SIZE];

/**
  * @brief  ��ȡһ֡ͼ��
  * @param  src_addr:ͼ��Դ��ַ
  * @param  img_width��ͼ����
  * @param  img_height��ͼ��߶�
  * @retval ��		
  */
extern void get_image(unsigned int src_addr,unsigned short img_width,unsigned short img_height);

/**
  * @brief  ��ά����뺯��
  * @param  ��
  * @retval ʶ���������		
  */
extern char QR_decoder(void);
