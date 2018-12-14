/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   二维码解码API
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
#include "qr_decoder_user.h"

//数据缓冲数组
//1M字节
unsigned short  dcmi_buf[1024*1024]    __attribute((section("NonCacheable")));
//1M字节
unsigned short  qr_img_buf[1024*1024]  __attribute((section("NonCacheable")));
/*512k字节*/
unsigned char   img_now[512*1024]     __attribute((section("NonCacheable")));

//解码数据封装为二维数组decoded_buf，格式为：
// （第一组：解码类型长度(8bit)+解码类型名称+解码数据长度(16bit,高位在前低位在后)+解码数据）
// （第二组：解码类型长度(8bit)+解码类型名称+解码数据长度(16bit,高位在前低位在后)+解码数据）
//  。。。
//以此类推
char decoded_buf[QR_SYMBOL_NUM][QR_SYMBOL_SIZE];

/**
  * @brief  获取一帧图像
  * @param  src_addr:图像源地址
  * @param  img_width：图像宽度
  * @param  img_height：图像高度
  * @retval 无		
  */
extern void get_image(unsigned int src_addr,unsigned short img_width,unsigned short img_height);

/**
  * @brief  二维码解码函数
  * @param  无
  * @retval 识别条码个数		
  */
extern char QR_decoder(void);
