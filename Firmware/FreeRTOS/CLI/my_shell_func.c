#include "my_shell_func.h"
#include "stdlib.h"
#include "Led.h"


TaskHandle_t Shell_Init_handle;   //shell的句柄
BaseType_t xHigherPriorityTaskWoken;    //释放二进制信号量用到
void Shell_Init(void *pvParameters)
{
	uint8_t *ch = (uint8_t *)pvPortMalloc(sizeof(uint8_t));   //就算是一个这种指针也要进行malloc
	for(;;)
	{
		if(Get_Usart_Char(ch) != 0)    //读取数据失败，一般是没有数据
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
		if( !strcmp("on", &argv[argv[1]]) )  //如果led后第一个参数为on
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



//  参数定义            
//  读取状态  返回值是0，则读取成功，如果是1 则缓冲区是空的
#pragma push
#pragma O0    //优化等级为O0，防止出问题。
uint8_t Get_Usart_Char(uint8_t *ch)   //从串口缓冲区内读一个数据
{
	if(USART_Data_Array_Struc_GetDataArrayLength(USART1_Data_Array)==0)
	{
		return 1;
	}
	else
	{
		*ch = USART_Readonebyte(USART1_Data_Array);   //这个是顺序度的，指针自动加一
		return 0;
	}
}
#pragma pop

