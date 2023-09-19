#ifndef __LED_H
#define __LED_H

#include "main.h"
#include "FreeRTOS.h"    //ʹ��task.hǰ�������߰���FreeRTOS,h
#include "task.h"


extern TaskHandle_t  task_Led_Toggle_handle;



void Led_On(void);
void Led_Off(void);

void task_Led_Toggle(void *pvParameters);    //FreeRTOS��Task��������������� 
#endif
