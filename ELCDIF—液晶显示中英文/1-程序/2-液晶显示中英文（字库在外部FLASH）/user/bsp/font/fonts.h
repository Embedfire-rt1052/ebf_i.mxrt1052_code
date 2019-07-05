#ifndef __FONT_H
#define __FONT_H       


#include <stdint.h>
#include "./font/fonts.h"



/* 在设置显示的中文字符大小，（要与实际的字库对应）
*根据字库位置设置：
*1. 如果字库位于SD卡，则根据GBKCODE_FILE_NAME宏定义设置。
*2. 如果字库位于外部Flash，则根据GBKCODE_name宏定义设置。
*/
#define      WIDTH_CH_CHAR		              32	    //中文字符宽度 
#define      HEIGHT_CH_CHAR		              32            //中文字符高度 


/*--------------------------中文字体相关宏定义---------------------------*/
/*SD卡字模路径*/
#define GBKCODE_FILE_NAME			"/Font/GB2312_H3232.FON"



/*--------------------------英文字体相关----------------------------*/
/*定义计算坐标结构体*/
#define LINE(x) ((x) * (((sFONT *)LCD_GetFont())->Height))
#define LINEY(x) ((x) * (((sFONT *)LCD_GetFont())->Width))

/*定义初始英文字体*/
#define CHAR_font   Font16x32   


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


/*----------------------------外部flash相关------------------------*/

/*中文字库存储在FLASH的起始地址*/
/*FLASH*/
/* 资源烧录到的FLASH基地址（目录地址） */
#define RESOURCE_BASE_ADDR	(16*1024*1024)

/* 存储在FLASH中的资源目录大小 */
#define CATALOG_SIZE	   (8*1024)

/*中文字库名*/
#define GBKCODE_name      "GB2312_H3232.FON"
/* 字库目录信息类型 */
typedef struct 
{
	char 	      name[40];  /* 资源的名字 */
	uint32_t      size;      /* 资源的大小 */ 
	uint32_t      offset;    /* 资源相对于基地址的偏移 */
}CatalogTypeDef;





/*要支持中文需要实现本函数*/
#define GetGBKCode( ucBuffer, usChar )  GetGBKCode_from_EXFlash( ucBuffer, usChar )

int GetGBKCode_from_sd ( uint8_t * pBuffer, uint16_t c);//从SD卡获取中文字符
int GetGBKCode_from_EXFlash( uint8_t * pBuffer, uint16_t c);//从外部FLASH获取中文字符
int GetResOffset(const char *res_name);
#endif /*end of __FONT_H    */


