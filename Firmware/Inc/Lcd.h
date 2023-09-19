#ifndef __LCD_H
#define __LCD_H

#include "main.h"
#include "spi.h"

// 　FreeRTOS头文件以及定义
#include "FreeRTOS.h"    //使用task.h前，必须线包含FreeRTOS,h
#include "task.h"

extern TaskHandle_t  LCD_Show_handle;   //函数句柄，用于接收xTaskCreate的参数
void LCD_Show(void *pvParameters);


//  LCD 尺寸
#define    LCD_LONG            160                            //LCD长边
#define    LCD_SHORT           80                             //LCD短边
#define    LCD_TOTAL_PIXEL     LCD_LONG*LCD_SHORT           //一个像素16个bit
#define    LCD_BUF_SIZE        LCD_TOTAL_PIXEL                           //LCD的缓冲区大小


//  LCD横屏竖屏选择
#define    LCD_IS_HORIZONTAL   3                             //0或1为竖屏 2或3为横屏


#if LCD_IS_HORIZONTAL==0 || LCD_IS_HORIZONTAL==1
#define     LCD_WIDTH          LCD_SHORT                          //LCD宽
#define     LCD_HEIGHT         LCD_LONG                           //LCD高
#else
#define     LCD_WIDTH          LCD_LONG                           //LCD宽
#define     LCD_HEIGHT         LCD_SHORT                          //LCD高
#endif



#define     POINT_COLOR        BLACK                            //画笔颜色
#define   	BACK_COLOR 	       WHITE                           //背景颜色	






//  LCD颜色
//画笔颜色
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
#define BROWN 			     0XBC40 //棕色
#define BRRED 		    	 0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

//  设置引脚状态

//  PWR=1,则三极管导通，LEDA大约在3V左右，背光开启，否则电压几乎为0。
//  PWR=1，开启背光。 PWR=0，背光关闭
//  RST=1，正常工作   RST=0，复位
//  DC=1， 选择显示数据或参数寄存器   DC=0，选择指令寄存器
#define LCD_PWR_Set(n) (n?LL_GPIO_SetOutputPin(LCD_PWR_GPIO_Port,LCD_PWR_Pin):LL_GPIO_ResetOutputPin(LCD_PWR_GPIO_Port,LCD_PWR_Pin));
#define LCD_RST_Set(n) (n?LL_GPIO_SetOutputPin(LCD_RST_GPIO_Port,LCD_RST_Pin):LL_GPIO_ResetOutputPin(LCD_RST_GPIO_Port,LCD_RST_Pin));
#define LCD_DC_Set(n) (n?LL_GPIO_SetOutputPin(LCD_DC_GPIO_Port,LCD_DC_Pin):LL_GPIO_ResetOutputPin(LCD_DC_GPIO_Port,LCD_DC_Pin));





void LCD_Init(void);                                                            //LCD初始化
void LCD_DisplayOn(void);                                                       //打开LCD背光
void LCD_DisplayOff(void);                                                                            //关闭LCD背光
void LCD_Clear(uint16_t color);                                                                          //以某一种颜色清屏
void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color);        //LCD颜色填充某一区域
void LCD_ShowChar(uint16_t x_start, uint16_t y_start, char chr, uint8_t font_size);                                         //显示一个ascii字符
void LCD_ShowString(uint16_t x_start, uint16_t y_start, uint16_t string_width, uint16_t height, uint8_t font_size, char *p);       //显示字符串
void LCD_ShowChineseword(uint16_t x_start, uint16_t y_start, uint16_t wordoffset);  //显示一个汉字
// 从字模表里的offset_begin开始显示，知道offset_end
void LCD_ShowChineseString(uint16_t x_start, uint16_t y_start, uint16_t string_width, uint16_t height, uint16_t offset_begin, uint16_t offset_end);   //显示多个汉字
void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p);            //画图  16位全彩RGB565

//  下面都是底层的函数
void LCD_Write_Data_8bit(uint8_t data);                                                                         //写字节
void LCD_Write_Data_8bits(uint8_t *data, uint16_t size);                                                       //发送size个bit

void LCD_Write_Data_16bit(uint16_t data);                                       //发送16bit数据
void LCD_Write_Data_16bits(uint16_t *data, uint16_t size);                      //发送多个16bit数据

void LCD_Hardware_Init(void);                                                   //LCD硬件初始化，主要是SPI初始化
void LCD_Software_Init(void);                                                   //LCD软件初始化，主要是配置LCD内部寄存器

void LCD_Address_Set(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end);             //LCD显存地址设置                                  //设置LCD显存的起始坐标和终止坐标



void LCD_Write_Data(uint8_t data);                                              //向LCD发送命令或者命令寄存器
void LCD_Write_Cmd(uint8_t data);                                               //向LCD发送命令




#endif

