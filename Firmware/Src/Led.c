#include "led.h"




TaskHandle_t  task_Led_Toggle_handle;   //������������ڽ���xTaskCreate�Ĳ���

void Led_On(void)
{
	LL_GPIO_ResetOutputPin(LED_GPIO_Port,LED_Pin);          //��0��ʱ�����
}

void Led_Off(void)
{
	LL_GPIO_SetOutputPin(LED_GPIO_Port,LED_Pin);        //���ߣ�����
}

void task_Led_Toggle(void *pvParameters)   //FreeRTOS��Task������Ļ���Ҫд��Whileѭ��
{
	for(;;)
	{
		vTaskDelay(pdMS_TO_TICKS(1000));    //��ʱ1000ms�� ������pdMS_TO_Ticks�궨���ms��תΪtick����Ȼ�����vTaskDelay��ʱ��
		Led_On();
		vTaskDelay(pdMS_TO_TICKS(1000));
		Led_Off();
	}
}
