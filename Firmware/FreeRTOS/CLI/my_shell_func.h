#ifndef __MY_SHELL_FUN_H
#define __MY_SHELL_FUN_H

#include "nr_micro_shell.h"
#include "Usart1.h"
#include "FreeRTOS.h"    //使用task.h前，必须线包含FreeRTOS,h
#include "task.h"
#include "semphr.h"

extern TaskHandle_t Shell_Init_handle;   //shell的函数句柄
extern BaseType_t xHigherPriorityTaskWoken;    //释放二进制信号量用到


void Shell_Init(void *pvParameters);
uint8_t Get_Usart_Char(uint8_t *ch);   //从串口缓冲区内读一个数据


void Led_Shell_Cmd(char argc, char *argv);  //控制led亮灭的命令

#endif
