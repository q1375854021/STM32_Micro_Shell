#ifndef __USART1_H
#define __USART1_H


// ע��
// ʹ�����º���ʱ��Ӧ��malloc�ṹ��󣬵���USART_Data_Array_Struc_Initialize�������г�ʼ����
#include "main.h"
#include "usart.h"
#include "String.h"
#include "FreeRTOS.h"
#include "semphr.h"


// ���ڻ������Ĵ�С
#define USART1_DATA_Array_Length 64  

// �õ��Ĵ���
#define USART_Port USART1


extern struct USART_Data_Array_Struc *USART1_Data_Array;      //����һ������




//  ���ڻ�������Ū��ѭ�����е���ʽ����ʡ�ռ��Ҳ������ϴ��ڵ���Ϊ��ʽ 
//  ʵ��ֻ���� USART1_DATA_Array_Length-1���ռ� 
#pragma pack(2)
struct USART_Data_Array_Struc {  
	uint16_t front;     // ��ͷ
	uint16_t rear;      // ��β 
	uint16_t Data_Array_Length;     //ѭ�����еĳ��� 
	uint8_t Data_Array[USART1_DATA_Array_Length];    //ѭ�����У�������
}; 



void USART_SendByte( USART_TypeDef *USART, uint8_t data);           //����һ���ַ�
void USART_Send_Nbit_Data(USART_TypeDef *USART,void *data,int Nbit);  //���ڷ���λ������λ������  int    long int     float ��Ϊ��λ   double 8λ   ˵ʵ�������Ǻܷ��㣬��Ϊ�����ж����ݵ���ʼ����ֹ����֪����ʼ����ֹ���Ǻܺõģ���֪���Ļ��������ַ���
void USART_SendBytes(USART_TypeDef *USART,uint8_t *data, int Send_Byte_Num);          //���Ͷ���ַ�

uint8_t USART_Readonebyte(struct USART_Data_Array_Struc *Struc);     //�ӻ�����˳���ȡһ���ֽ�
void USART_Array_Clear(struct USART_Data_Array_Struc *Struc);        //������ڻ�����
void USART_Writeonebyte(struct USART_Data_Array_Struc *Struc, uint8_t ch);  //�򴮿ڻ�����дһ���ֽ�



// �������ײ㺯��
uint8_t USART_Data_Array_Struc_Initialize(struct USART_Data_Array_Struc *Struc, uint16_t DATA_Array_Length);      //��������ʼ��
uint8_t USART_Data_Array_Struc_Push(struct USART_Data_Array_Struc *Struc, uint8_t ch);    //�򻺳���дһ���ֽ�
uint8_t USART_Data_Array_Struc_Pop(struct USART_Data_Array_Struc *Struc, uint8_t *ch);    //�ӻ�������ȡһ���ֽ�
int USART_Data_Array_Struc_GetDataArrayLength(struct USART_Data_Array_Struc *Struc);      //��ȡ������������������
#endif
