#include "my_shell_func.h"
#include "stdlib.h"
#include "Led.h"


TaskHandle_t Shell_Init_handle;   //shell�ľ��
BaseType_t xHigherPriorityTaskWoken;    //�ͷŶ������ź����õ�
void Shell_Init(void *pvParameters)
{
	uint8_t *ch = (uint8_t *)pvPortMalloc(sizeof(uint8_t));   //������һ������ָ��ҲҪ����malloc
	for(;;)
	{
		if(Get_Usart_Char(ch) != 0)    //��ȡ����ʧ�ܣ�һ����û������
		{
			vTaskDelay(pdMS_TO_TICKS(50));
		}
		else
		{
			shell(*ch);
		}
	}
}

void Led_Shell_Cmd(char argc, char *argv)
{
	if(argc > 1)
	{
		if( !strcmp("on", &argv[argv[1]]) )  //���led���һ������Ϊon
		{
			Led_On();
			shell_printf("Led on!\r\n");
		}
		else if(!strcmp("off", &argv[argv[1]]) )
		{
			Led_Off();
			shell_printf("Led off!\r\n");
		}
		else
		{
			shell_printf("cmd wrong!\r\n");
		}
	}
	else
		shell_printf("cmd must follow an option!\r\n");
}



//  ��������            
//  ��ȡ״̬  ����ֵ��0�����ȡ�ɹ��������1 �򻺳����ǿյ�
#pragma push
#pragma O0    //�Ż��ȼ�ΪO0����ֹ�����⡣
uint8_t Get_Usart_Char(uint8_t *ch)   //�Ӵ��ڻ������ڶ�һ������
{
	if(USART_Data_Array_Struc_GetDataArrayLength(USART1_Data_Array)==0)
	{
		return 1;
	}
	else
	{
		*ch = USART_Readonebyte(USART1_Data_Array);   //�����˳��ȵģ�ָ���Զ���һ
		return 0;
	}
}
#pragma pop

