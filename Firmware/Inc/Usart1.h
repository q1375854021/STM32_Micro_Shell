#ifndef __USART1_H
#define __USART1_H


// 注意
// 使用以下函数时，应当malloc结构体后，调用USART_Data_Array_Struc_Initialize函数进行初始化。
#include "main.h"
#include "usart.h"
#include "String.h"
#include "FreeRTOS.h"
#include "semphr.h"


// 串口缓冲区的大小
#define USART1_DATA_Array_Length 64  

// 用到的串口
#define USART_Port USART1


extern struct USART_Data_Array_Struc *USART1_Data_Array;      //串口一缓冲区




//  串口缓冲区，弄成循环队列的形式，节省空间且操作符合串口的行为方式 
//  实际只能用 USART1_DATA_Array_Length-1个空间 
#pragma pack(2)
struct USART_Data_Array_Struc {  
	uint16_t front;     // 队头
	uint16_t rear;      // 队尾 
	uint16_t Data_Array_Length;     //循环队列的长度 
	uint8_t Data_Array[USART1_DATA_Array_Length];    //循环队列，缓冲区
}; 



void USART_SendByte( USART_TypeDef *USART, uint8_t data);           //发送一个字符
void USART_Send_Nbit_Data(USART_TypeDef *USART,void *data,int Nbit);  //用于发送位宽超过两位的数据  int    long int     float 均为四位   double 8位   说实话并不是很方便，因为不好判断数据的起始和终止，能知道起始和终止还是很好的，不知道的话尽量用字符串
void USART_SendBytes(USART_TypeDef *USART,uint8_t *data, int Send_Byte_Num);          //发送多个字符

uint8_t USART_Readonebyte(struct USART_Data_Array_Struc *Struc);     //从缓冲区顺序读取一个字节
void USART_Array_Clear(struct USART_Data_Array_Struc *Struc);        //清除串口缓冲区
void USART_Writeonebyte(struct USART_Data_Array_Struc *Struc, uint8_t ch);  //向串口缓冲区写一个字节



// 缓冲区底层函数
uint8_t USART_Data_Array_Struc_Initialize(struct USART_Data_Array_Struc *Struc, uint16_t DATA_Array_Length);      //缓冲区初始化
uint8_t USART_Data_Array_Struc_Push(struct USART_Data_Array_Struc *Struc, uint8_t ch);    //向缓冲区写一个字节
uint8_t USART_Data_Array_Struc_Pop(struct USART_Data_Array_Struc *Struc, uint8_t *ch);    //从缓冲区读取一个字节
int USART_Data_Array_Struc_GetDataArrayLength(struct USART_Data_Array_Struc *Struc);      //读取缓冲区内数据量长度
#endif
