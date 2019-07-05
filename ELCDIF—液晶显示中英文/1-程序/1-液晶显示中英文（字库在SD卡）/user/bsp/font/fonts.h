#ifndef __FONT_H
#define __FONT_H       


#include <stdint.h>
#include "./font/fonts.h"

/*--------------------------����������غ궨��---------------------------*/

/* ����ʾ������ʾ���ַ���С��Ҫ��GBKCODE_FILE_NAME�궨�����Ӧ*/
#define      WIDTH_CH_CHAR		              32	    //�����ַ���� 
#define      HEIGHT_CH_CHAR		              32            //�����ַ��߶� 

/*SD����ģ·��*/
#define GBKCODE_FILE_NAME			"/Font/GB2312_H3232.FON"


/*--------------------------Ӣ���������----------------------------*/
/*�����������ṹ��*/
#define LINE(x) ((x) * (((sFONT *)LCD_GetFont())->Height))
#define LINEY(x) ((x) * (((sFONT *)LCD_GetFont())->Width))
   

/*����Ӣ������ṹ��*/
typedef struct _tFont
{    
  const uint8_t *table; /*ָ����ģ���ݵ�ָ��*/
  uint16_t Width;       /*��ģ�����ؿ��*/
  uint16_t Height;      /*��ģ�����ظ߶�*/  
} sFONT;
 

/*�����ѡ��Ӣ�������ʽ��������fonts.c�ļ�*/
extern sFONT Font24x48;
extern sFONT Font16x32;
extern sFONT Font8x16;


/*Ҫ֧��������Ҫʵ�ֱ�����*/
#define GetGBKCode( ucBuffer, usChar )  GetGBKCode_from_sd( ucBuffer, usChar )
int GetGBKCode_from_sd ( uint8_t * pBuffer, uint16_t c);//��SD����ȡ�����ַ�

#endif /*end of __FONT_H    */


