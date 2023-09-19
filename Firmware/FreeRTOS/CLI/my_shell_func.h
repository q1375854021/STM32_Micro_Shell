#ifndef __MY_SHELL_FUN_H
#define __MY_SHELL_FUN_H

#include "nr_micro_shell.h"
#include "Usart1.h"
#include "FreeRTOS.h"    //ʹ��task.hǰ�������߰���FreeRTOS,h
#include "task.h"
#include "semphr.h"

extern TaskHandle_t Shell_Init_handle;   //shell�ĺ������
extern BaseType_t xHigherPriorityTaskWoken;    //�ͷŶ������ź����õ�


void Shell_Init(void *pvParameters);
uint8_t Get_Usart_Char(uint8_t *ch);   //�Ӵ��ڻ������ڶ�һ������


void Led_Shell_Cmd(char argc, char *argv);  //����led���������

#endif
