#include "Lcd.h"
#include "Lcd_font.h"
#include "stdio.h"

static uint16_t lcd_buf[LCD_BUF_SIZE];        //LCD缓冲取

TaskHandle_t  LCD_Show_handle;   //函数句柄，用于接收xTaskCreate的参数

//LCD的Task
void LCD_Show(void *pvParameters)
{
	char show_buffer[15];
	TickType_t xLastWakeTime;
	uint8_t count=0;
  const TickType_t xDelay1000ms = pdMS_TO_TICKS( 1000 );     //一秒执行一次
  xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		sprintf((char *)show_buffer,"this is %u",count++);
		LCD_Clear(WHITE);
		LCD_ShowString(0,0,LCD_LONG,LCD_SHORT,12,show_buffer);
		vTaskDelayUntil(&xLastWakeTime,xDelay1000ms);         //十分准确的1s定时， 因为vTaskDelay是从当前时刻延时1S，而这个是任务延时1S。
	}
}



//   LCD软件初始化
void LCD_Software_Init(void)
{
	LCD_PWR_Set(0);         //背光也设为0
	LCD_RST_Set(0);         //软件复位
	LL_mDelay(200);          //延时200ms
	
	LCD_RST_Set(1);         //使能
	LL_mDelay(20);           //延时20ms
	LCD_PWR_Set(1);         //开背光
	
	LCD_Write_Cmd(0x11);    //退出睡眠状态   需要120ms
	LL_mDelay(100);
	
	
	LCD_Write_Cmd(0xB1);    //帧率控制，需要写入数值
	LCD_Write_Data(0x05);   //上电的顺序就是这样的
	LCD_Write_Data(0x10); 
	LCD_Write_Data(0x10); 
	
	LCD_Write_Cmd(0xB2);    //帧率控制，也是帧率控制，IDLE模式
	LCD_Write_Data(0x05);   //上电的顺序就是这样的
	LCD_Write_Data(0x10); 
	LCD_Write_Data(0x10); 
	
	LCD_Write_Cmd(0xB3);    //帧率控制，也是帧率控制.....
	LCD_Write_Data(0x05);   //上电的顺序就是这样的
	LCD_Write_Data(0x10); 
	LCD_Write_Data(0x10); 
	LCD_Write_Data(0x05);   
	LCD_Write_Data(0x10); 
	LCD_Write_Data(0x10); 
	//  具体看手册吧，手册就是这样的。。。。参考ST7736S芯片手册
	
	LCD_Write_Cmd(0xB4);     //显示反转控制
	LCD_Write_Data(0x03);    //NLB=1,NLC=1   空闲模式列反转，部分全彩模式列反转
	
	LCD_Write_Cmd(0xC0);     //供电控制
	LCD_Write_Data(0x62);    //AVDD=4.8  GVDD=4.6
	LCD_Write_Data(0x02);    //GVCL=-4.6
	LCD_Write_Data(0x04);    //VRHP=0   VRHN=0
	
	LCD_Write_Cmd(0xC1);      //供电控制2
	LCD_Write_Data(0xC0);     //V25=2.4  VGH=2*AVDD+VGH25-0.5
	
	LCD_Write_Cmd(0xC2);      //供电控制3
	LCD_Write_Data(0x0D);     //AP=101, SAP=001  运算放大器可以使用大电流
	LCD_Write_Data(0x00);
	
	LCD_Write_Cmd(0xC3);      //供电控制4
	LCD_Write_Data(0x8D);
	LCD_Write_Data(0x6A);   

	LCD_Write_Cmd(0xC4);      //供电控制5
	LCD_Write_Data(0x8D); 
	LCD_Write_Data(0xEE); 
		
	LCD_Write_Cmd(0xC5);      //VCOM 控制
	LCD_Write_Data(0x0E);   
		
		
	LCD_Write_Cmd(0xE0);      //伽马正极性校正参数设置
	LCD_Write_Data(0x10);
	LCD_Write_Data(0x0E);
	LCD_Write_Data(0x02);
	LCD_Write_Data(0x03);
	LCD_Write_Data(0x0E);
	LCD_Write_Data(0x07);
	LCD_Write_Data(0x02);
	LCD_Write_Data(0x07);
	LCD_Write_Data(0x0A);
	LCD_Write_Data(0x12);
	LCD_Write_Data(0x27);
	LCD_Write_Data(0x37);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x0D);
	LCD_Write_Data(0x0E);
	LCD_Write_Data(0x10);
	
	LCD_Write_Cmd(0xE1);      //伽马负极性参数校正
	LCD_Write_Data(0x10);
	LCD_Write_Data(0x0E);
	LCD_Write_Data(0x03);
	LCD_Write_Data(0x03);
	LCD_Write_Data(0x0F);
	LCD_Write_Data(0x06);
	LCD_Write_Data(0x02);
	LCD_Write_Data(0x08);
	LCD_Write_Data(0x0A);
	LCD_Write_Data(0x13);
	LCD_Write_Data(0x26);
	LCD_Write_Data(0x36);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x0D);
	LCD_Write_Data(0x0E);
	LCD_Write_Data(0x10);
	

	LCD_Write_Cmd(0x3A);    //界面像素格式
	LCD_Write_Data(0x05);   //每个像素16bit
	
	
	LCD_Write_Cmd(0x36);    //内存数据访问控制器
	if(LCD_IS_HORIZONTAL==0) LCD_Write_Data(0x08);      //从上往下刷新  从左往右刷新  BRG格式
	else if(LCD_IS_HORIZONTAL==1) LCD_Write_Data(0xC8); //列行地址顺序反向，从上往下刷新  BRG格式   从左往右刷新
	else if(LCD_IS_HORIZONTAL==2) LCD_Write_Data(0x78); //列地址顺序反向，列行交换，从下往上刷新，BRG格式，从左往右刷新
	else LCD_Write_Data(0xA8);            //行地址反向，列行交换，BGR格式，从左往右刷新
	
	LCD_Write_Cmd(0x29);      //开启显示
	
}

void LCD_Init(void)
{
	LCD_Software_Init();
}

//  写一个字节
void LCD_Write_Data_8bit(uint8_t data)
{
	LL_SPI_SetTransferSize(LCD_SPI_Port,1);
	while(LL_SPI_IsActiveMasterTransfer(LCD_SPI_Port)==1);        //等待其空闲变为0  或者如果是0， 则目前没有数据在传输  CR2的TSIZE变为0时他会自动变为1
	while(LL_SPI_IsActiveFlag_TXP(LCD_SPI_Port)==0);                  //如果发送FIFO中有空间容纳一个数据包)   1有空间
	LL_SPI_TransmitData8(LCD_SPI_Port,data);                      //把数据传送到TXDR寄存器
	LL_SPI_StartMasterTransfer(LCD_SPI_Port);                    //开始传输
	while(LL_SPI_IsActiveFlag_EOT(LCD_SPI_Port)==0);             //等待EOT变成0，传输完成
	LL_SPI_ClearFlag_EOT(LCD_SPI_Port);                          //清除EOT
	LL_SPI_ClearFlag_TXTF(LCD_SPI_Port);                         //清除TXTF
}

//  发送命令
void LCD_Write_Cmd(uint8_t data)
{
	LCD_DC_Set(0);               //0代表cmd
	LCD_Write_Data_8bit(data);        //发送字节 data
}


//  发送数据
void LCD_Write_Data(uint8_t data)
{
	LCD_DC_Set(1);               //1代表数据
	LCD_Write_Data_8bit(data);        //发送字节 data
}

//   发送16bit数据
void LCD_Write_Data_16bit(uint16_t data)
{
	uint8_t temp_array[2];
	temp_array[0] = data >>8;
	temp_array[1] = data;
	LCD_DC_Set(1);
	LCD_Write_Data(temp_array[0]);
	LCD_Write_Data(temp_array[1]);
}


void LCD_DisplayOff(void)    //LCD关背光
{ 
	LCD_PWR_Set(0);
}

void LCD_DisplayOn(void)    //LCD开背光
{ 
	LCD_PWR_Set(1);
}

void LCD_Clear(uint16_t color)
{
	LCD_Fill(0,0,LCD_WIDTH,LCD_HEIGHT,color);
}

void LCD_Write_Data_8bits(uint8_t *data, uint16_t size)
{
	for(int i=0;i<size;i++)
	{
		LCD_Write_Data(*(data+i));    //发送数据
	}
}

void LCD_Write_Data_16bits(uint16_t *data, uint16_t size)
{
	for(int i=0;i<size;i++)
	{
		LCD_Write_Data_16bit(*(data+i));    //发送数据
	}
}



void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color)
{
    uint16_t i = 0;
    uint32_t size = 0, size_remain = 0;

    size = (x_end - x_start + 1) * (y_end - y_start + 1) * 2;

    if(size > LCD_TOTAL_PIXEL)
    {
        size_remain = size - LCD_TOTAL_PIXEL;
        size = LCD_TOTAL_PIXEL;
    }

    LCD_Address_Set(x_start, y_start, x_end, y_end);

    while(1)
    {
        for(i = 0; i < size; i++)
        {
            lcd_buf[i] = color;
        }
        LCD_DC_Set(1);
        LCD_Write_Data_16bits(lcd_buf, size);

        if(size_remain == 0)
            break;

        if(size_remain > LCD_BUF_SIZE)
        {
            size_remain = size_remain - LCD_BUF_SIZE;
        }

        else
        {
            size = size_remain;
            size_remain = 0;
        }
    }
}


//size=12时  csize=12            需要12个字节排成一排   1206  总共大小也就12*6/8=9个字节，但是需要16个字节，因为6少于1个字节，因此后两个bit没有用到，就省掉了
//size=16时  csize=16            需要16个字节排成一排   1608
//size=24时  csize=48            需要48个字节排成一排   2412
//size=32时  csize=64            需要64个字节排成一排   3216
void LCD_ShowChar(uint16_t x_start, uint16_t y_start, char chr, uint8_t font_size)  //显示一个ascii码字符
{
	uint8_t chr_bytewidth;                //不同字号的单个字符所占字节大小
	uint8_t chr_offset=chr-' ';           //所显示的字符相对于' '的便宜
	uint8_t temp_byte;                       //用于取点用
	uint8_t temp_i;                          //循环用
	
	if(font_size==12||font_size==16)
		chr_bytewidth=font_size;
	else if(font_size==24||font_size==32)
		chr_bytewidth=font_size*2;
	
	//每个ascii字符的高都是font_size,宽都是font_size/2
	if(x_start>(LCD_WIDTH-font_size/2) || (y_start>LCD_HEIGHT-font_size))       //所剩空间无法放下一个字符
		return ;   //直接就是没有空间放了
	
	
	LCD_Address_Set(x_start,y_start,x_start+font_size/2-1,y_start+font_size-1);   //设置显存地址
	
	//  开始写显存
	if(font_size==12)
	{
		for(temp_i=0;temp_i<chr_bytewidth;temp_i++)      //选中每个字节
		{
			temp_byte = asc2_1206[chr_offset][temp_i];     //比如
			for(uint8_t temp_j=0;temp_j<6;temp_j++)
			{
				if(temp_byte&0x80)  LCD_Write_Data_16bit(POINT_COLOR);  //如果这个bit是1，那么就画  先从上画到下，然后从左往右画
				else LCD_Write_Data_16bit(BACK_COLOR);                   //如果这个bit是0，就画背景色
				temp_byte <<= 1;   //左移一位   也就是说高位在前，高位对应上，地位对应下
			}
		}
	}
	else if(font_size==16)
	{
		for(temp_i=0;temp_i<chr_bytewidth;temp_i++)      //选中每个字节
		{
			temp_byte = asc2_1608[chr_offset][temp_i];     //比如
			for(uint8_t temp_j=0;temp_j<8;temp_j++)
			{
				if(temp_byte&0x80)  LCD_Write_Data_16bit(POINT_COLOR);  //如果这个bit是1，那么就画  先从上画到下，然后从左往右画
				else LCD_Write_Data_16bit(BACK_COLOR);                   //如果这个bit是0，就画背景色
				temp_byte <<= 1;   //左移一位   也就是说高位在前，高位对应上，地位对应下
			}
		}
	}
	else if(font_size==24)
	{
		for(temp_i=0;temp_i<chr_bytewidth;temp_i++)      //选中每个字节
		{
			temp_byte = asc2_2412[chr_offset][temp_i];     //比如
			for(uint8_t temp_j=0;temp_j<6;temp_j++)
			{
				if(temp_byte&0x80)  LCD_Write_Data_16bit(POINT_COLOR);  //如果这个bit是1，那么就画  先从上画到下，然后从左往右画
				else LCD_Write_Data_16bit(BACK_COLOR);                   //如果这个bit是0，就画背景色
				temp_byte <<= 1;                   //左移一位   也就是说高位在前，高位对应上，地位对应下
			}
		}
	}
	else if(font_size==32)
	{
		for(temp_i=0;temp_i<chr_bytewidth;temp_i++)      //选中每个字节
		{
			temp_byte = asc2_3216[chr_offset][temp_i];     //比如
			for(uint8_t temp_j=0;temp_j<8;temp_j++)
			{
				if(temp_byte&0x80)  LCD_Write_Data_16bit(POINT_COLOR);  //如果这个bit是1，那么就画  先从上画到下，然后从左往右画
				else LCD_Write_Data_16bit(BACK_COLOR);                   //如果这个bit是0，就画背景色
				temp_byte <<= 1;                   //左移一位   也就是说高位在前，高位对应上，地位对应下
			}
		}
	}
}

// 显示一个字符串  //如果字符串长度超过了width，一般就是n倍的font_size/2，那么会自动换行  显示区域的字符总高度设为height
void LCD_ShowString(uint16_t x_start, uint16_t y_start, uint16_t string_width, uint16_t height, uint8_t font_size, char *p)
{
    uint8_t x0 = x_start;   //起始点x坐标
		
    while((*p <= '~') && (*p >= ' ')) //判断是不是非法字符!
    {
        if(x_start>(LCD_WIDTH-font_size/2))
        {
            x_start = x0;   //回退到
            y_start = y_start + font_size;   //往下一行  如果是12的话，就往下12个坐标，则时候注意要字节对齐
        }
        if(y_start>LCD_HEIGHT-font_size) break; //退出  如果起始高度超出了界限，就退出

        LCD_ShowChar(x_start, y_start, *p, font_size);
        x_start += font_size / 2;    //右移，同时也要注意字号为12的情况
        p++;
    }
}

void LCD_ShowChineseword(uint16_t x_start, uint16_t y_start, uint16_t wordoffset)  //显示一个汉字
{
	uint8_t temp_byte;                       //用于取点用
	uint8_t temp_i;                          //循环用
	uint8_t chr_bytewidth = 32;              //16*16/8=32

	
	//每个ascii字符的高都是font_size,宽都是font_size/2
	if(x_start>(LCD_WIDTH-16) || (y_start>LCD_HEIGHT-16))       //所剩空间无法放下一个字符
		return ;   //直接就是没有空间放了
	
	
	LCD_Address_Set(x_start,y_start,x_start+16-1,y_start+16-1);   //设置显存地址
	
	//  开始写显存
	for(temp_i=0;temp_i<chr_bytewidth;temp_i++)      //选中每个字节
	{
		temp_byte = chinese_1616[wordoffset][temp_i];     //比如
		for(uint8_t temp_j=0;temp_j<8;temp_j++)
		{
			if(temp_byte&0x80)  LCD_Write_Data_16bit(POINT_COLOR);  //如果这个bit是1，那么就画  先从上画到下，然后从左往右画
			else LCD_Write_Data_16bit(BACK_COLOR);                   //如果这个bit是0，就画背景色
			temp_byte <<= 1;   //左移一位   也就是说高位在前，高位对应上，地位对应下
		}
	}
}

// offset_begin 是所取汉字在字模表里的序号起始
// offset_end   是所取汉字在字模表里的序号终止
// 其它参数大致相同
void LCD_ShowChineseString(uint16_t x_start, uint16_t y_start, uint16_t string_width, uint16_t height, uint16_t offset_begin, uint16_t offset_end)
{
    uint8_t x0 = x_start;   //起始点x坐标
		uint8_t temp_i;
    for(temp_i=offset_begin;temp_i <= offset_end;temp_i++)
		{
        if(x_start>(LCD_WIDTH-16))
        {
            x_start = x0;   //回退到
            y_start = y_start + 16;   //往下一行  如果是12的话，就往下12个坐标，则时候注意要字节对齐
        }
        if(y_start>LCD_HEIGHT-16) break; //退出  如果起始高度超出了界限，就退出

        LCD_ShowChineseword(x_start, y_start, temp_i);
        x_start += 16;    //右移，同时也要注意字号为12的情况
    }
}

// 如果是竖屏  x=0~79   y=0~159
// 如果是横屏  x=0~159  y=0~79
// 后续加一个检测数组越界的功能就好了
void LCD_Address_Set(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)   //设置LCD显存的起始坐标和终止坐标   
{
	if(LCD_IS_HORIZONTAL==0||LCD_IS_HORIZONTAL==1)        //竖屏显示
	{
		LCD_Write_Cmd(0x2a);//列地址设置
		LCD_Write_Data_16bit(x_start+24);
		LCD_Write_Data_16bit(x_end+24);
		LCD_Write_Cmd(0x2b);//行地址设置
		LCD_Write_Data_16bit(y_start);
		LCD_Write_Data_16bit(y_end);
		LCD_Write_Cmd(0x2c);//储存器写
	}
	else
	{
		LCD_Write_Cmd(0x2a);//列地址设置
		LCD_Write_Data_16bit(x_start);
		LCD_Write_Data_16bit(x_end);
		LCD_Write_Cmd(0x2b);//行地址设置
		LCD_Write_Data_16bit(y_start+24);
		LCD_Write_Data_16bit(y_end+24);
		LCD_Write_Cmd(0x2c);//储存器写
	}
}

//  LCD画图片
// Image2Lcd取模方式：	C语言数据/水平扫描/16位真彩色(RGB565)/高位在前	
// 图片是以字节为单位保存的
void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p)
{
    if(x + width > LCD_WIDTH || y + height > LCD_HEIGHT)
    {
        return;
    }

    LCD_Address_Set(x, y, x + width - 1, y + height - 1);
    LCD_DC_Set(1);

    LCD_Write_Data_8bits((uint8_t *)p, width * height * 2);   
}
