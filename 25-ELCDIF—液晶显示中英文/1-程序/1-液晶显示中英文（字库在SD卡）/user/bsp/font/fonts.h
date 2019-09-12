#ifndef __FONT_H
#define __FONT_H       


#include <stdint.h>
#include "./font/fonts.h"

/*--------------------------中文字体相关宏定义---------------------------*/

/* 在显示屏上显示的字符大小，要与GBKCODE_FILE_NAME宏定义相对应*/
#define      WIDTH_CH_CHAR		              32	    //中文字符宽度 
#define      HEIGHT_CH_CHAR		              32            //中文字符高度 

/*SD卡字模路径*/
#define GBKCODE_FILE_NAME			"/Font/GB2312_H3232.FON"


/*--------------------------英文字体相关----------------------------*/
/*定义计算坐标结构体*/
#define LINE(x) ((x) * (((sFONT *)LCD_GetFont())->Height))
#define LINEY(x) ((x) * (((sFONT *)LCD_GetFont())->Width))
   

/*定义英文字体结构体*/
typedef struct _tFont
{    
  const uint8_t *table; /*指向字模数据的指针*/
  uint16_t Width;       /*字模的像素宽度*/
  uint16_t Height;      /*字模的像素高度*/  
} sFONT;
 

/*定义可选的英文字体格式，定义在fonts.c文件*/
extern sFONT Font24x48;
extern sFONT Font16x32;
extern sFONT Font8x16;


/*要支持中文需要实现本函数*/
#define GetGBKCode( ucBuffer, usChar )  GetGBKCode_from_sd( ucBuffer, usChar )
int GetGBKCode_from_sd ( uint8_t * pBuffer, uint16_t c);//从SD卡获取中文字符

#endif /*end of __FONT_H    */


