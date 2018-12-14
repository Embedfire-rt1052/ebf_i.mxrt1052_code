#ifndef __QR_DECODER_USER_H
#define	__QR_DECODER_USER_H

/*ɨ�贰�ڲ���*/
#define  Frame_width           320u//ɨ�贰�ڱ߳��������Σ�

/*ɨ��������֧���Զ��壬ע��ɨ���Ŀ�Ȳ��ܳ���ͼ��ĸ߶� */
#define  qr_img_w   Frame_width
#define  qr_img_h   Frame_width

/*ɨ�����������*/
#define  Frame_line_length     30u //ɨ�����������
#define  Frame_line_size       3u  //ɨ����������

#define  QR_SYMBOL_NUM  5u    //ʶ���ά���������
#define  QR_SYMBOL_SIZE 512u  //ÿ���ά��ĵ��������

//���ݻ�������
//1M�ֽ�
extern unsigned short  dcmi_buf[1024*1024];
//1M�ֽ�
extern unsigned short  qr_img_buf[1024*1024];
//512k�ֽ�
extern unsigned char   img_now[512*1024];

extern char decoded_buf[QR_SYMBOL_NUM][QR_SYMBOL_SIZE];

//���뺯��������ֵΪʶ������ĸ���
char QR_decoder(void);

//��ȡһ֡ͼ��
extern void get_image(unsigned int src_addr,unsigned short img_width,unsigned short img_height);

#endif /* __QR_DECODER_USER_H */
