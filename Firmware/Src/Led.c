#include "led.h"




TaskHandle_t  task_Led_Toggle_handle;   //函数句柄，用于接收xTaskCreate的参数

void Led_On(void)
{
	LL_GPIO_ResetOutputPin(LED_GPIO_Port,LED_Pin);          //置0的时候灯亮
}

void Led_Off(void)
{
	LL_GPIO_SetOutputPin(LED_GPIO_Port,LED_Pin);        //拉高，灯灭
}

void task_Led_Toggle(void *pvParameters)   //FreeRTOS的Task，定义的话需要写成While循环
{
	for(;;)
	{
		vTaskDelay(pdMS_TO_TICKS(1000));    //延时1000ms， 这里用pdMS_TO_Ticks宏定义把ms数转为tick数，然后调用vTaskDelay延时。
		Led_On();
		vTaskDelay(pdMS_TO_TICKS(1000));
		Led_Off();
	}
}
