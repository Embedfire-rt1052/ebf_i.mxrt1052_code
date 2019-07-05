#ifndef __FONT_H
#define __FONT_H       


#include <stdint.h>
#include "./font/fonts.h"

/*--------------------------����������غ궨��---------------------------*/

/* ����ʾ������ʾ���ַ���С��Ҫ��GBKCODE_FILE_NAME�궨�����Ӧ*/
#define      WIDTH_CH_CHAR		              24	    //�����ַ���� 
#define      HEIGHT_CH_CHAR		              24            //�����ַ��߶� 

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

/*----------------------------�ⲿflash���------------------------*/

/*�����ֿ�洢��FLASH����ʼ��ַ*/
/*FLASH*/
/* ��Դ��¼����FLASH����ַ��Ŀ¼��ַ�� */
#define RESOURCE_BASE_ADDR	(16*1024*1024)

/* �洢��FLASH�е���ԴĿ¼��С */
#define CATALOG_SIZE				(8*1024)

/* �ֿ�Ŀ¼��Ϣ���� */
typedef struct 
{
	char 	      name[40];  /* ��Դ������ */
	uint32_t  	size;      /* ��Դ�Ĵ�С */ 
	uint32_t 	  offset;    /* ��Դ����ڻ���ַ��ƫ�� */
}CatalogTypeDef;





/*�����ѡ��Ӣ�������ʽ��������fonts.c�ļ�*/
extern sFONT Font24x48;
extern sFONT Font16x32;
extern sFONT Font8x16;


/*Ҫ֧��������Ҫʵ�ֱ�����*/
#define GetGBKCode( ucBuffer, usChar )  GetGBKCode_from_EXFlash( ucBuffer, usChar )

int GetGBKCode_from_sd ( uint8_t * pBuffer, uint16_t c);//��SD����ȡ�����ַ�
int GetGBKCode_from_EXFlash( uint8_t * pBuffer, uint16_t c);//���ⲿFLASH��ȡ�����ַ�
int GetResOffset(const char *res_name);
#endif /*end of __FONT_H    */


