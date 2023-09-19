#ifndef __LCD_H
#define __LCD_H

#include "main.h"
#include "spi.h"

// ��FreeRTOSͷ�ļ��Լ�����
#include "FreeRTOS.h"    //ʹ��task.hǰ�������߰���FreeRTOS,h
#include "task.h"

extern TaskHandle_t  LCD_Show_handle;   //������������ڽ���xTaskCreate�Ĳ���
void LCD_Show(void *pvParameters);


//  LCD �ߴ�
#define    LCD_LONG            160                            //LCD����
#define    LCD_SHORT           80                             //LCD�̱�
#define    LCD_TOTAL_PIXEL     LCD_LONG*LCD_SHORT           //һ������16��bit
#define    LCD_BUF_SIZE        LCD_TOTAL_PIXEL                           //LCD�Ļ�������С


//  LCD��������ѡ��
#define    LCD_IS_HORIZONTAL   3                             //0��1Ϊ���� 2��3Ϊ����


#if LCD_IS_HORIZONTAL==0 || LCD_IS_HORIZONTAL==1
#define     LCD_WIDTH          LCD_SHORT                          //LCD��
#define     LCD_HEIGHT         LCD_LONG                           //LCD��
#else
#define     LCD_WIDTH          LCD_LONG                           //LCD��
#define     LCD_HEIGHT         LCD_SHORT                          //LCD��
#endif



#define     POINT_COLOR        BLACK                            //������ɫ
#define   	BACK_COLOR 	       WHITE                           //������ɫ	






//  LCD��ɫ
//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE          	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //��ɫ
#define BRRED 		    	 0XFC07 //�غ�ɫ
#define GRAY  			     0X8430 //��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

//  ��������״̬

//  PWR=1,�������ܵ�ͨ��LEDA��Լ��3V���ң����⿪���������ѹ����Ϊ0��
//  PWR=1���������⡣ PWR=0������ر�
//  RST=1����������   RST=0����λ
//  DC=1�� ѡ����ʾ���ݻ�����Ĵ���   DC=0��ѡ��ָ��Ĵ���
#define LCD_PWR_Set(n) (n?LL_GPIO_SetOutputPin(LCD_PWR_GPIO_Port,LCD_PWR_Pin):LL_GPIO_ResetOutputPin(LCD_PWR_GPIO_Port,LCD_PWR_Pin));
#define LCD_RST_Set(n) (n?LL_GPIO_SetOutputPin(LCD_RST_GPIO_Port,LCD_RST_Pin):LL_GPIO_ResetOutputPin(LCD_RST_GPIO_Port,LCD_RST_Pin));
#define LCD_DC_Set(n) (n?LL_GPIO_SetOutputPin(LCD_DC_GPIO_Port,LCD_DC_Pin):LL_GPIO_ResetOutputPin(LCD_DC_GPIO_Port,LCD_DC_Pin));





void LCD_Init(void);                                                            //LCD��ʼ��
void LCD_DisplayOn(void);                                                       //��LCD����
void LCD_DisplayOff(void);                                                                            //�ر�LCD����
void LCD_Clear(uint16_t color);                                                                          //��ĳһ����ɫ����
void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color);        //LCD��ɫ���ĳһ����
void LCD_ShowChar(uint16_t x_start, uint16_t y_start, char chr, uint8_t font_size);                                         //��ʾһ��ascii�ַ�
void LCD_ShowString(uint16_t x_start, uint16_t y_start, uint16_t string_width, uint16_t height, uint8_t font_size, char *p);       //��ʾ�ַ���
void LCD_ShowChineseword(uint16_t x_start, uint16_t y_start, uint16_t wordoffset);  //��ʾһ������
// ����ģ�����offset_begin��ʼ��ʾ��֪��offset_end
void LCD_ShowChineseString(uint16_t x_start, uint16_t y_start, uint16_t string_width, uint16_t height, uint16_t offset_begin, uint16_t offset_end);   //��ʾ�������
void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p);            //��ͼ  16λȫ��RGB565

//  ���涼�ǵײ�ĺ���
void LCD_Write_Data_8bit(uint8_t data);                                                                         //д�ֽ�
void LCD_Write_Data_8bits(uint8_t *data, uint16_t size);                                                       //����size��bit

void LCD_Write_Data_16bit(uint16_t data);                                       //����16bit����
void LCD_Write_Data_16bits(uint16_t *data, uint16_t size);                      //���Ͷ��16bit����

void LCD_Hardware_Init(void);                                                   //LCDӲ����ʼ������Ҫ��SPI��ʼ��
void LCD_Software_Init(void);                                                   //LCD�����ʼ������Ҫ������LCD�ڲ��Ĵ���

void LCD_Address_Set(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end);             //LCD�Դ��ַ����                                  //����LCD�Դ����ʼ�������ֹ����



void LCD_Write_Data(uint8_t data);                                              //��LCD���������������Ĵ���
void LCD_Write_Cmd(uint8_t data);                                               //��LCD��������




#endif

