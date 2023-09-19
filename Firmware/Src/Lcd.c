#include "Lcd.h"
#include "Lcd_font.h"
#include "stdio.h"

static uint16_t lcd_buf[LCD_BUF_SIZE];        //LCD����ȡ

TaskHandle_t  LCD_Show_handle;   //������������ڽ���xTaskCreate�Ĳ���

//LCD��Task
void LCD_Show(void *pvParameters)
{
	char show_buffer[15];
	TickType_t xLastWakeTime;
	uint8_t count=0;
  const TickType_t xDelay1000ms = pdMS_TO_TICKS( 1000 );     //һ��ִ��һ��
  xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		sprintf((char *)show_buffer,"this is %u",count++);
		LCD_Clear(WHITE);
		LCD_ShowString(0,0,LCD_LONG,LCD_SHORT,12,show_buffer);
		vTaskDelayUntil(&xLastWakeTime,xDelay1000ms);         //ʮ��׼ȷ��1s��ʱ�� ��ΪvTaskDelay�Ǵӵ�ǰʱ����ʱ1S���������������ʱ1S��
	}
}



//   LCD�����ʼ��
void LCD_Software_Init(void)
{
	LCD_PWR_Set(0);         //����Ҳ��Ϊ0
	LCD_RST_Set(0);         //�����λ
	LL_mDelay(200);          //��ʱ200ms
	
	LCD_RST_Set(1);         //ʹ��
	LL_mDelay(20);           //��ʱ20ms
	LCD_PWR_Set(1);         //������
	
	LCD_Write_Cmd(0x11);    //�˳�˯��״̬   ��Ҫ120ms
	LL_mDelay(100);
	
	
	LCD_Write_Cmd(0xB1);    //֡�ʿ��ƣ���Ҫд����ֵ
	LCD_Write_Data(0x05);   //�ϵ��˳�����������
	LCD_Write_Data(0x10); 
	LCD_Write_Data(0x10); 
	
	LCD_Write_Cmd(0xB2);    //֡�ʿ��ƣ�Ҳ��֡�ʿ��ƣ�IDLEģʽ
	LCD_Write_Data(0x05);   //�ϵ��˳�����������
	LCD_Write_Data(0x10); 
	LCD_Write_Data(0x10); 
	
	LCD_Write_Cmd(0xB3);    //֡�ʿ��ƣ�Ҳ��֡�ʿ���.....
	LCD_Write_Data(0x05);   //�ϵ��˳�����������
	LCD_Write_Data(0x10); 
	LCD_Write_Data(0x10); 
	LCD_Write_Data(0x05);   
	LCD_Write_Data(0x10); 
	LCD_Write_Data(0x10); 
	//  ���忴�ֲ�ɣ��ֲ���������ġ��������ο�ST7736SоƬ�ֲ�
	
	LCD_Write_Cmd(0xB4);     //��ʾ��ת����
	LCD_Write_Data(0x03);    //NLB=1,NLC=1   ����ģʽ�з�ת������ȫ��ģʽ�з�ת
	
	LCD_Write_Cmd(0xC0);     //�������
	LCD_Write_Data(0x62);    //AVDD=4.8  GVDD=4.6
	LCD_Write_Data(0x02);    //GVCL=-4.6
	LCD_Write_Data(0x04);    //VRHP=0   VRHN=0
	
	LCD_Write_Cmd(0xC1);      //�������2
	LCD_Write_Data(0xC0);     //V25=2.4  VGH=2*AVDD+VGH25-0.5
	
	LCD_Write_Cmd(0xC2);      //�������3
	LCD_Write_Data(0x0D);     //AP=101, SAP=001  ����Ŵ�������ʹ�ô����
	LCD_Write_Data(0x00);
	
	LCD_Write_Cmd(0xC3);      //�������4
	LCD_Write_Data(0x8D);
	LCD_Write_Data(0x6A);   

	LCD_Write_Cmd(0xC4);      //�������5
	LCD_Write_Data(0x8D); 
	LCD_Write_Data(0xEE); 
		
	LCD_Write_Cmd(0xC5);      //VCOM ����
	LCD_Write_Data(0x0E);   
		
		
	LCD_Write_Cmd(0xE0);      //٤��������У����������
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
	
	LCD_Write_Cmd(0xE1);      //٤�����Բ���У��
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
	

	LCD_Write_Cmd(0x3A);    //�������ظ�ʽ
	LCD_Write_Data(0x05);   //ÿ������16bit
	
	
	LCD_Write_Cmd(0x36);    //�ڴ����ݷ��ʿ�����
	if(LCD_IS_HORIZONTAL==0) LCD_Write_Data(0x08);      //��������ˢ��  ��������ˢ��  BRG��ʽ
	else if(LCD_IS_HORIZONTAL==1) LCD_Write_Data(0xC8); //���е�ַ˳���򣬴�������ˢ��  BRG��ʽ   ��������ˢ��
	else if(LCD_IS_HORIZONTAL==2) LCD_Write_Data(0x78); //�е�ַ˳�������н�������������ˢ�£�BRG��ʽ����������ˢ��
	else LCD_Write_Data(0xA8);            //�е�ַ�������н�����BGR��ʽ����������ˢ��
	
	LCD_Write_Cmd(0x29);      //������ʾ
	
}

void LCD_Init(void)
{
	LCD_Software_Init();
}

//  дһ���ֽ�
void LCD_Write_Data_8bit(uint8_t data)
{
	LL_SPI_SetTransferSize(LCD_SPI_Port,1);
	while(LL_SPI_IsActiveMasterTransfer(LCD_SPI_Port)==1);        //�ȴ�����б�Ϊ0  ���������0�� ��Ŀǰû�������ڴ���  CR2��TSIZE��Ϊ0ʱ�����Զ���Ϊ1
	while(LL_SPI_IsActiveFlag_TXP(LCD_SPI_Port)==0);                  //�������FIFO���пռ�����һ�����ݰ�)   1�пռ�
	LL_SPI_TransmitData8(LCD_SPI_Port,data);                      //�����ݴ��͵�TXDR�Ĵ���
	LL_SPI_StartMasterTransfer(LCD_SPI_Port);                    //��ʼ����
	while(LL_SPI_IsActiveFlag_EOT(LCD_SPI_Port)==0);             //�ȴ�EOT���0���������
	LL_SPI_ClearFlag_EOT(LCD_SPI_Port);                          //���EOT
	LL_SPI_ClearFlag_TXTF(LCD_SPI_Port);                         //���TXTF
}

//  ��������
void LCD_Write_Cmd(uint8_t data)
{
	LCD_DC_Set(0);               //0����cmd
	LCD_Write_Data_8bit(data);        //�����ֽ� data
}


//  ��������
void LCD_Write_Data(uint8_t data)
{
	LCD_DC_Set(1);               //1��������
	LCD_Write_Data_8bit(data);        //�����ֽ� data
}

//   ����16bit����
void LCD_Write_Data_16bit(uint16_t data)
{
	uint8_t temp_array[2];
	temp_array[0] = data >>8;
	temp_array[1] = data;
	LCD_DC_Set(1);
	LCD_Write_Data(temp_array[0]);
	LCD_Write_Data(temp_array[1]);
}


void LCD_DisplayOff(void)    //LCD�ر���
{ 
	LCD_PWR_Set(0);
}

void LCD_DisplayOn(void)    //LCD������
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
		LCD_Write_Data(*(data+i));    //��������
	}
}

void LCD_Write_Data_16bits(uint16_t *data, uint16_t size)
{
	for(int i=0;i<size;i++)
	{
		LCD_Write_Data_16bit(*(data+i));    //��������
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


//size=12ʱ  csize=12            ��Ҫ12���ֽ��ų�һ��   1206  �ܹ���СҲ��12*6/8=9���ֽڣ�������Ҫ16���ֽڣ���Ϊ6����1���ֽڣ���˺�����bitû���õ�����ʡ����
//size=16ʱ  csize=16            ��Ҫ16���ֽ��ų�һ��   1608
//size=24ʱ  csize=48            ��Ҫ48���ֽ��ų�һ��   2412
//size=32ʱ  csize=64            ��Ҫ64���ֽ��ų�һ��   3216
void LCD_ShowChar(uint16_t x_start, uint16_t y_start, char chr, uint8_t font_size)  //��ʾһ��ascii���ַ�
{
	uint8_t chr_bytewidth;                //��ͬ�ֺŵĵ����ַ���ռ�ֽڴ�С
	uint8_t chr_offset=chr-' ';           //����ʾ���ַ������' '�ı���
	uint8_t temp_byte;                       //����ȡ����
	uint8_t temp_i;                          //ѭ����
	
	if(font_size==12||font_size==16)
		chr_bytewidth=font_size;
	else if(font_size==24||font_size==32)
		chr_bytewidth=font_size*2;
	
	//ÿ��ascii�ַ��ĸ߶���font_size,����font_size/2
	if(x_start>(LCD_WIDTH-font_size/2) || (y_start>LCD_HEIGHT-font_size))       //��ʣ�ռ��޷�����һ���ַ�
		return ;   //ֱ�Ӿ���û�пռ����
	
	
	LCD_Address_Set(x_start,y_start,x_start+font_size/2-1,y_start+font_size-1);   //�����Դ��ַ
	
	//  ��ʼд�Դ�
	if(font_size==12)
	{
		for(temp_i=0;temp_i<chr_bytewidth;temp_i++)      //ѡ��ÿ���ֽ�
		{
			temp_byte = asc2_1206[chr_offset][temp_i];     //����
			for(uint8_t temp_j=0;temp_j<6;temp_j++)
			{
				if(temp_byte&0x80)  LCD_Write_Data_16bit(POINT_COLOR);  //������bit��1����ô�ͻ�  �ȴ��ϻ����£�Ȼ��������һ�
				else LCD_Write_Data_16bit(BACK_COLOR);                   //������bit��0���ͻ�����ɫ
				temp_byte <<= 1;   //����һλ   Ҳ����˵��λ��ǰ����λ��Ӧ�ϣ���λ��Ӧ��
			}
		}
	}
	else if(font_size==16)
	{
		for(temp_i=0;temp_i<chr_bytewidth;temp_i++)      //ѡ��ÿ���ֽ�
		{
			temp_byte = asc2_1608[chr_offset][temp_i];     //����
			for(uint8_t temp_j=0;temp_j<8;temp_j++)
			{
				if(temp_byte&0x80)  LCD_Write_Data_16bit(POINT_COLOR);  //������bit��1����ô�ͻ�  �ȴ��ϻ����£�Ȼ��������һ�
				else LCD_Write_Data_16bit(BACK_COLOR);                   //������bit��0���ͻ�����ɫ
				temp_byte <<= 1;   //����һλ   Ҳ����˵��λ��ǰ����λ��Ӧ�ϣ���λ��Ӧ��
			}
		}
	}
	else if(font_size==24)
	{
		for(temp_i=0;temp_i<chr_bytewidth;temp_i++)      //ѡ��ÿ���ֽ�
		{
			temp_byte = asc2_2412[chr_offset][temp_i];     //����
			for(uint8_t temp_j=0;temp_j<6;temp_j++)
			{
				if(temp_byte&0x80)  LCD_Write_Data_16bit(POINT_COLOR);  //������bit��1����ô�ͻ�  �ȴ��ϻ����£�Ȼ��������һ�
				else LCD_Write_Data_16bit(BACK_COLOR);                   //������bit��0���ͻ�����ɫ
				temp_byte <<= 1;                   //����һλ   Ҳ����˵��λ��ǰ����λ��Ӧ�ϣ���λ��Ӧ��
			}
		}
	}
	else if(font_size==32)
	{
		for(temp_i=0;temp_i<chr_bytewidth;temp_i++)      //ѡ��ÿ���ֽ�
		{
			temp_byte = asc2_3216[chr_offset][temp_i];     //����
			for(uint8_t temp_j=0;temp_j<8;temp_j++)
			{
				if(temp_byte&0x80)  LCD_Write_Data_16bit(POINT_COLOR);  //������bit��1����ô�ͻ�  �ȴ��ϻ����£�Ȼ��������һ�
				else LCD_Write_Data_16bit(BACK_COLOR);                   //������bit��0���ͻ�����ɫ
				temp_byte <<= 1;                   //����һλ   Ҳ����˵��λ��ǰ����λ��Ӧ�ϣ���λ��Ӧ��
			}
		}
	}
}

// ��ʾһ���ַ���  //����ַ������ȳ�����width��һ�����n����font_size/2����ô���Զ�����  ��ʾ������ַ��ܸ߶���Ϊheight
void LCD_ShowString(uint16_t x_start, uint16_t y_start, uint16_t string_width, uint16_t height, uint8_t font_size, char *p)
{
    uint8_t x0 = x_start;   //��ʼ��x����
		
    while((*p <= '~') && (*p >= ' ')) //�ж��ǲ��ǷǷ��ַ�!
    {
        if(x_start>(LCD_WIDTH-font_size/2))
        {
            x_start = x0;   //���˵�
            y_start = y_start + font_size;   //����һ��  �����12�Ļ���������12�����꣬��ʱ��ע��Ҫ�ֽڶ���
        }
        if(y_start>LCD_HEIGHT-font_size) break; //�˳�  �����ʼ�߶ȳ����˽��ޣ����˳�

        LCD_ShowChar(x_start, y_start, *p, font_size);
        x_start += font_size / 2;    //���ƣ�ͬʱҲҪע���ֺ�Ϊ12�����
        p++;
    }
}

void LCD_ShowChineseword(uint16_t x_start, uint16_t y_start, uint16_t wordoffset)  //��ʾһ������
{
	uint8_t temp_byte;                       //����ȡ����
	uint8_t temp_i;                          //ѭ����
	uint8_t chr_bytewidth = 32;              //16*16/8=32

	
	//ÿ��ascii�ַ��ĸ߶���font_size,����font_size/2
	if(x_start>(LCD_WIDTH-16) || (y_start>LCD_HEIGHT-16))       //��ʣ�ռ��޷�����һ���ַ�
		return ;   //ֱ�Ӿ���û�пռ����
	
	
	LCD_Address_Set(x_start,y_start,x_start+16-1,y_start+16-1);   //�����Դ��ַ
	
	//  ��ʼд�Դ�
	for(temp_i=0;temp_i<chr_bytewidth;temp_i++)      //ѡ��ÿ���ֽ�
	{
		temp_byte = chinese_1616[wordoffset][temp_i];     //����
		for(uint8_t temp_j=0;temp_j<8;temp_j++)
		{
			if(temp_byte&0x80)  LCD_Write_Data_16bit(POINT_COLOR);  //������bit��1����ô�ͻ�  �ȴ��ϻ����£�Ȼ��������һ�
			else LCD_Write_Data_16bit(BACK_COLOR);                   //������bit��0���ͻ�����ɫ
			temp_byte <<= 1;   //����һλ   Ҳ����˵��λ��ǰ����λ��Ӧ�ϣ���λ��Ӧ��
		}
	}
}

// offset_begin ����ȡ��������ģ����������ʼ
// offset_end   ����ȡ��������ģ����������ֹ
// ��������������ͬ
void LCD_ShowChineseString(uint16_t x_start, uint16_t y_start, uint16_t string_width, uint16_t height, uint16_t offset_begin, uint16_t offset_end)
{
    uint8_t x0 = x_start;   //��ʼ��x����
		uint8_t temp_i;
    for(temp_i=offset_begin;temp_i <= offset_end;temp_i++)
		{
        if(x_start>(LCD_WIDTH-16))
        {
            x_start = x0;   //���˵�
            y_start = y_start + 16;   //����һ��  �����12�Ļ���������12�����꣬��ʱ��ע��Ҫ�ֽڶ���
        }
        if(y_start>LCD_HEIGHT-16) break; //�˳�  �����ʼ�߶ȳ����˽��ޣ����˳�

        LCD_ShowChineseword(x_start, y_start, temp_i);
        x_start += 16;    //���ƣ�ͬʱҲҪע���ֺ�Ϊ12�����
    }
}

// ���������  x=0~79   y=0~159
// ����Ǻ���  x=0~159  y=0~79
// ������һ���������Խ��Ĺ��ܾͺ���
void LCD_Address_Set(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)   //����LCD�Դ����ʼ�������ֹ����   
{
	if(LCD_IS_HORIZONTAL==0||LCD_IS_HORIZONTAL==1)        //������ʾ
	{
		LCD_Write_Cmd(0x2a);//�е�ַ����
		LCD_Write_Data_16bit(x_start+24);
		LCD_Write_Data_16bit(x_end+24);
		LCD_Write_Cmd(0x2b);//�е�ַ����
		LCD_Write_Data_16bit(y_start);
		LCD_Write_Data_16bit(y_end);
		LCD_Write_Cmd(0x2c);//������д
	}
	else
	{
		LCD_Write_Cmd(0x2a);//�е�ַ����
		LCD_Write_Data_16bit(x_start);
		LCD_Write_Data_16bit(x_end);
		LCD_Write_Cmd(0x2b);//�е�ַ����
		LCD_Write_Data_16bit(y_start+24);
		LCD_Write_Data_16bit(y_end+24);
		LCD_Write_Cmd(0x2c);//������д
	}
}

//  LCD��ͼƬ
// Image2Lcdȡģ��ʽ��	C��������/ˮƽɨ��/16λ���ɫ(RGB565)/��λ��ǰ	
// ͼƬ�����ֽ�Ϊ��λ�����
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
