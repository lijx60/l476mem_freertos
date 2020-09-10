/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "stdio.h"
#include "gpio.h"
 extern  key_val key_sta;
 extern uint8_t  interrupt_flag;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint8_t sem1[] = " it is created a  semaphore after 100's";
uint8_t sem2[] = " wait a semaphore ";
uint8_t * pbuffer;

/* USER CODE END Variables */
osThreadId ledTask1Handle;
osThreadId mallocTask2Handle;
osThreadId keyTask3Handle;
osSemaphoreId myBinarySem1Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartledTask1(void const * argument);
void StartmallocTask2(void const * argument);
void StartkeyTask3(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of myBinarySem1 */
  osSemaphoreDef(myBinarySem1);
  myBinarySem1Handle = osSemaphoreCreate(osSemaphore(myBinarySem1), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of ledTask1 */
  osThreadDef(ledTask1, StartledTask1, osPriorityBelowNormal, 0, 128);
  ledTask1Handle = osThreadCreate(osThread(ledTask1), NULL);

  /* definition and creation of mallocTask2 */
  osThreadDef(mallocTask2, StartmallocTask2, osPriorityNormal, 0, 128);
  mallocTask2Handle = osThreadCreate(osThread(mallocTask2), NULL);

  /* definition and creation of keyTask3 */
  osThreadDef(keyTask3, StartkeyTask3, osPriorityNormal, 0, 128);
  keyTask3Handle = osThreadCreate(osThread(keyTask3), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartledTask1 */
/**
  * @brief  Function implementing the ledTask1 thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartledTask1 */
void StartledTask1(void const * argument)
{
  /* USER CODE BEGIN StartledTask1 */
	uint8_t i=0;
  /* Infinite loop */
  for(;;)
  {
		i++;
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    osDelay(500);
		 if(i==50)
		 {
			 printf(" Task1 to release a Semaphore \r\n");
		   osSemaphoreRelease(myBinarySem1Handle);  // 释放信号量
			 vTaskSuspend(NULL);
		    i=0;
			 //osThreadSuspend(StartledTask1);    
		 }
  }
  /* USER CODE END StartledTask1 */
}

/* USER CODE BEGIN Header_StartmallocTask2 */
/**
* @brief Function implementing the mallocTask2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartmallocTask2 */
void StartmallocTask2(void const * argument)
{
  /* USER CODE BEGIN StartmallocTask2 */
	 static uint8_t timer = 0;
  /* Infinite loop */
  for(;;)
  {
		if(osOK == osSemaphoreWait(myBinarySem1Handle,osWaitForever))
		{
			timer++;
		   pbuffer = pvPortMalloc(60);
			 if( pbuffer != NULL )
			 {
			  printf("the address of memory is = %#x \r\n",(int)pbuffer);
				sprintf((char * )pbuffer, "sem runs %d \r\n",timer ); 
				 printf(" pbuffer = %s \r\n",pbuffer);
			 }
		}
    osDelay(500);
  }
  /* USER CODE END StartmallocTask2 */
}

/* USER CODE BEGIN Header_StartkeyTask3 */
/**
* @brief Function implementing the keyTask3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartkeyTask3 */
void StartkeyTask3(void const * argument)
{
  /* USER CODE BEGIN StartkeyTask3 */
  /* Infinite loop */
  for(;;)
  {
		if((HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 0)&(interrupt_flag==1))
		{
		  osDelay(30);
			if((HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 0)&(interrupt_flag==1)) 
			{
				key_sta = key_on;
				interrupt_flag = 0;
			}	
		}
		if(key_sta == key_on)  
		{
			vPortFree(pbuffer);
			pbuffer = NULL;  //必须的，否则指针不干净
			printf("pbuffer's address after PortFree %#x\r\n",(int)pbuffer) ;
		  vTaskResume(ledTask1Handle); 		
		}			
		
    osDelay(500);
  }
  /* USER CODE END StartkeyTask3 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
