/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "cmsis_os.h"

#include "FreeRTOS.h"
#include "task.h"


#define DOT_PIN         GPIO_PIN_14
#define DASH_PIN        GPIO_PIN_15
#define DOT_LED_PIN     GPIO_PIN_8   // PA8
#define DASH_LED_PIN    GPIO_PIN_12  // PB12
#define SIGNAL_OUT_PIN  GPIO_PIN_6 // PB6
#define IR_IN_PIN       GPIO_PIN_5   // PB9
#define DOT_DURATION    300
#define DASH_DURATION   3000
#define DEBOUNCE_DELAY_MS 20

extern osThreadId senderTaskHandle;
extern osThreadId receiverTaskHandle;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

uint8_t read_button_debounced(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET) {
        HAL_Delay(DEBOUNCE_DELAY_MS);
        if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET) {
            while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET); // wait for release
            return 1; // Valid press
        }
    }
    return 0; // No press
}

void SenderTask(void *argument){
	 while (1)
	  {
	    if (read_button_debounced(GPIOB,DOT_PIN) == 1)
	    {
	      HAL_GPIO_WritePin(GPIOB, SIGNAL_OUT_PIN, GPIO_PIN_SET);
	      osDelay(DOT_DURATION);
	      HAL_GPIO_WritePin(GPIOB, SIGNAL_OUT_PIN, GPIO_PIN_RESET);
	    }
	    if (read_button_debounced(GPIOB, DASH_PIN) == 1)
	    {
	      HAL_GPIO_WritePin(GPIOB, SIGNAL_OUT_PIN, GPIO_PIN_SET);
	      osDelay(DASH_DURATION);
	      HAL_GPIO_WritePin(GPIOB, SIGNAL_OUT_PIN, GPIO_PIN_RESET);
	    }

	  }
}
void ReceiverTask(void *argument){
	while (1)
	 {
		    if (HAL_GPIO_ReadPin(GPIOB, IR_IN_PIN) == GPIO_PIN_RESET) // because RX give 0 when active
		    {
		      osDelay(400); // sample timing to distinguish dot/dash

		      if(HAL_GPIO_ReadPin(GPIOB, IR_IN_PIN) == GPIO_PIN_RESET)
		      {
		    	  // Still high after 400ms — assume it's a DASH
				HAL_GPIO_WritePin(GPIOB, DASH_LED_PIN, GPIO_PIN_SET);
				osDelay(200);
				HAL_GPIO_WritePin(GPIOB, DASH_LED_PIN, GPIO_PIN_RESET);

		      }

		      else if (HAL_GPIO_ReadPin(GPIOB, IR_IN_PIN) != GPIO_PIN_RESET)
		      		      {
		      		    	  // Went low quickly — DOT
		      				HAL_GPIO_WritePin(GPIOA, DOT_LED_PIN, GPIO_PIN_SET);
		      				osDelay(200);
		      				HAL_GPIO_WritePin(GPIOA, DOT_LED_PIN, GPIO_PIN_RESET);

		      		      }

		      else{
		    	  /*nothing */
		      }
		    }
		  }
}
/* Private variables ---------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId SenderTaskHandle;
osThreadId ReceiverTaskHandle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void StartTask03(void const * argument);

int main(void)
{
	HAL_Init();
	  SystemClock_Config();
	  MX_GPIO_Init();

	  osThreadDef(SenderTask, SenderTask, osPriorityNormal, 0, 256);
	  SenderTaskHandle = osThreadCreate(osThread(SenderTask), NULL);
	  osThreadDef(ReceiverTask, ReceiverTask, osPriorityAboveNormal, 0, 256);
	  ReceiverTaskHandle = osThreadCreate(osThread(ReceiverTask), NULL);
	  osKernelStart();
	  while (1) {}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB5 */ /* ir reciver */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/*Configure GPIO pin : PB14 */ /*button dot */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/*Configure GPIO pin : PB15 */ /*button dash */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/*Configure GPIO pin : PB6 */ /*ir tx  */
   GPIO_InitStruct.Pin = GPIO_PIN_6;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

   /*Configure GPIO pin : PA8 */ /*dot led   */
      GPIO_InitStruct.Pin = GPIO_PIN_8;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
/*Configure GPIO pin : PB12 */ /*dot led   */
	 GPIO_InitStruct.Pin = GPIO_PIN_12;
	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
	 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

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
