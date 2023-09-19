/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Led.h"
#include "Usart1.h"
#include "Lcd.h"

//  shell文件
#include "nr_micro_shell.h"
#include "my_shell_func.h"

#include "stdlib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */



TaskHandle_t  task_initial_handle;   //定义函数句柄，用于接收参数
void task_initial(void *pvParameters);    //函数声明一下



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_SYSCFG);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3, 3));

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI6_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	USART1_Data_Array = (struct USART_Data_Array_Struc *)pvPortMalloc(sizeof(struct USART_Data_Array_Struc));      //必须要malloc一下 ;
	USART_Data_Array_Struc_Initialize(USART1_Data_Array,USART1_DATA_Array_Length);   //初始化USART1的缓冲区
	LCD_Init();
	LCD_Clear(WHITE); 		//清屏	
	shell_init();         //初始化shell
	//在这个地方创建FreeRTOS的Task
	
	xTaskCreate( (TaskFunction_t) task_initial,     //这里的函数名字其实就是函数指针 
								(const char *)"task_initial",     //任务的名字，主要是调试的时候用的
								(uint16_t)     128,       //任务堆栈的大小  一般就是128，然后FreeRTOSConfig.h默认的堆栈大小为130，这个是可以改的
								(void *) NULL,            //传给函数的参数
								(UBaseType_t)  1,        //优先级  这个函数优先级设的高一点，很重要的函数
								(TaskHandle_t *) &task_initial_handle
								);                        // 传出来的任务句柄，后续对该任务进行操作使用该句柄就可以


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


	vTaskStartScheduler();
	
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_PWR_ConfigSupply(LL_PWR_LDO_SUPPLY);
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSE);
  LL_RCC_PLL1P_Enable();
  LL_RCC_PLL1R_Enable();
  LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_4_8);
  LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
  LL_RCC_PLL1_SetM(5);
  LL_RCC_PLL1_SetN(160);
  LL_RCC_PLL1_SetP(2);
  LL_RCC_PLL1_SetQ(2);
  LL_RCC_PLL1_SetR(2);
  LL_RCC_PLL1_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL1_IsReady() != 1)
  {
  }

   /* Intermediate AHB prescaler 2 when target frequency clock is higher than 80 MHz */
   LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1)
  {

  }
  LL_RCC_SetSysPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_DIV_2);
  LL_RCC_SetAPB4Prescaler(LL_RCC_APB4_DIV_2);

  LL_Init1msTick(50000000);

  LL_SetSystemCoreClock(400000000);
}

/* USER CODE BEGIN 4 */
// FreeRTOS的任务初始化函数
void task_initial(void *pvParameters)
{
	taskENTER_CRITICAL();        //进入临界区，防止别的函数打断，因为创建函数任务很重要


	xTaskCreate( (TaskFunction_t) LCD_Show,     //这里的函数名字其实就是函数指针 
								(const char *)"LCD_Show",     //任务的名字，主要是调试的时候用的
								(uint16_t)     128,       //任务堆栈的大小  一般就是128，然后FreeRTOSConfig.h默认的堆栈大小为130，这个是可以改的
								(void *) NULL,           //传给函数的参数
								(UBaseType_t)  4,        //优先级  这个函数优先级设的高一点，很重要的函数
								(TaskHandle_t *) &LCD_Show_handle
								);  
	

	xTaskCreate( (TaskFunction_t) Shell_Init,     //这里的函数名字其实就是函数指针 
								(const char *)"Shell_Init",     //任务的名字，主要是调试的时候用的
								(uint16_t)     1024,       //任务堆栈的大小  一般就是128，然后FreeRTOSConfig.h默认的堆栈大小为130，这个是可以改的
								(void *) NULL,           //传给函数的参数
								(UBaseType_t)  2,        //优先级  这个函数优先级设的高一点，很重要的函数
								(TaskHandle_t *) &Shell_Init_handle
								);  


	vTaskDelete(NULL);       //创建完函数，这个函数就没有用了  删除自己传参为NULL

	taskEXIT_CRITICAL();         //退出临界区
	
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
