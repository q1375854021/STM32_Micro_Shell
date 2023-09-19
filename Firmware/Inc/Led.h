#ifndef __LED_H
#define __LED_H

#include "main.h"
#include "FreeRTOS.h"    //使用task.h前，必须线包含FreeRTOS,h
#include "task.h"


extern TaskHandle_t  task_Led_Toggle_handle;



void Led_On(void);
void Led_Off(void);

void task_Led_Toggle(void *pvParameters);    //FreeRTOS的Task，在这里进行声明 
#endif
