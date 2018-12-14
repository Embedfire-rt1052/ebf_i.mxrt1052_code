#ifndef __QR_DECODER_USER_H
#define	__QR_DECODER_USER_H

/*扫描窗口参数*/
#define  Frame_width           320u//扫描窗口边长（正方形）

/*扫描框参数，支持自定义，注意扫描框的宽度不能超过图像的高度 */
#define  qr_img_w   Frame_width
#define  qr_img_h   Frame_width

/*扫描框线条参数*/
#define  Frame_line_length     30u //扫描框线条长度
#define  Frame_line_size       3u  //扫描框线条宽度

#define  QR_SYMBOL_NUM  5u    //识别二维码的最大个数
#define  QR_SYMBOL_SIZE 512u  //每组二维码的的最大容量

//数据缓冲数组
//1M字节
extern unsigned short  dcmi_buf[1024*1024];
//1M字节
extern unsigned short  qr_img_buf[1024*1024];
//512k字节
extern unsigned char   img_now[512*1024];

extern char decoded_buf[QR_SYMBOL_NUM][QR_SYMBOL_SIZE];

//解码函数，返回值为识别条码的个数
char QR_decoder(void);

//获取一帧图像
extern void get_image(unsigned int src_addr,unsigned short img_width,unsigned short img_height);

#endif /* __QR_DECODER_USER_H */
