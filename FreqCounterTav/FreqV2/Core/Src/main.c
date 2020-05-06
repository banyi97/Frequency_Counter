/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h"
#include "arm_common_tables.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Ts 1/10000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t START_FRAME = 0xAA;
float32_t PI_2 = 2 * PI;
uint32_t freq = 2000;
uint32_t calcFreq = 0;
uint32_t fallCntr = 0;
float32_t phase = 0;

uint8_t rxBuffer[4];
uint8_t *p = rxBuffer;
uint8_t count = 0;

uint16_t sendFreq;
 uint8_t txBuff[3];
 uint8_t* tx = txBuff;

TIM_HandleTypeDef tim;
TIM_HandleTypeDef dac_tim;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_DAC_Init(void);
/* USER CODE BEGIN PFP */
static void TimerInit(void);
static void DAC_Tim(void);
uint8_t gencrc(uint8_t *data, size_t len);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART2){
		if(rxBuffer[count++] == START_FRAME){
			rxBuffer[0] = START_FRAME;
			count = 1;
		}
		if(count > 3){
			count = 0;
			if(rxBuffer[3] == gencrc(p, 3)){
				uint16_t f = 0;
				f |= rxBuffer[1];
				f |= rxBuffer[2] << 8;
				freq = f;
			}
		}
		HAL_UART_Receive_IT(&huart2, (p+count), 1);
	}
}
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
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_DAC_Init();
  /* USER CODE BEGIN 2 */
  DAC_Tim();
  TimerInit();
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {

	sendFreq = calcFreq; // local copy
	if(sendFreq > 9999){
		sendFreq = 9999;
	}

	// SEG 1
	uint8_t thouse = sendFreq / 1000;
	txBuff[0] = START_FRAME;
	txBuff[1] = thouse | 0x80;
	txBuff[2] = gencrc(tx, 2);
	HAL_UART_Transmit(&huart2, tx, 3, 1000);
	// SEG 2
	sendFreq -= thouse * 1000;
	uint8_t hundr = sendFreq / 100;
	txBuff[1] = hundr | 0x40;
	txBuff[2] = gencrc(tx, 2);
	HAL_UART_Transmit(&huart2, tx, 3, 1000);
	sendFreq -= hundr * 100;
	// SEG 3
	uint8_t ten = sendFreq / 10;
	txBuff[1] = ten | 0x20;
	txBuff[2] = gencrc(tx, 2);
	HAL_UART_Transmit(&huart2, tx, 3, 1000);
	sendFreq -= ten * 10;
	// SEG 4
	txBuff[1] = sendFreq | 0x10;
	txBuff[2] = gencrc(tx, 2);
	HAL_UART_Transmit(&huart2, tx, 3, 1000);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */
  /** DAC Initialization 
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT1 config 
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  HAL_UART_Receive_IT(&huart2, p, 1);
  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
static void DAC_Tim(){
	__TIM2_CLK_ENABLE(); //84Mhz - 10kHz 7,0x20C
	dac_tim.Instance = TIM2;
	dac_tim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	dac_tim.Init.CounterMode = TIM_COUNTERMODE_UP;
	dac_tim.Init.Prescaler = 7; // 7 + 1
	dac_tim.Init.Period = 0x419; // 524 + 1
	dac_tim.State = HAL_TIM_STATE_RESET;

	HAL_TIM_Base_Init(&dac_tim);

	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 2);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	HAL_TIM_Base_Start_IT(&dac_tim);
}
static void TimerInit(){
	__TIM4_CLK_ENABLE(); // 84Mhz - 1Hz 0xC7F,0x6689
	tim.Instance = TIM4;
	tim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim.Init.Prescaler = 0xC7F; // 3199 + 1
	tim.Init.Period = 0x6689; // 26249 + 1
	tim.State = HAL_TIM_STATE_RESET;

	HAL_TIM_Base_Init(&tim);

	HAL_NVIC_SetPriority(TIM4_IRQn, 0, 3);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);

	HAL_TIM_Base_Start_IT(&tim);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{
	if (htim->Instance == TIM4)
	{
		calcFreq = fallCntr;
		fallCntr = 0;
	}
	if (htim->Instance == TIM2)
	{
		phase += (float32_t)(PI_2 * (float32_t)freq * Ts);
		if(phase > PI_2){
			phase -= PI_2;
		}
		if(phase < PI){
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0x00000000);
		}
		else{
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0xFFFFFFFF);
		}
		//float32_t out_temp = 1 + 1 * arm_sin_f32(phase) + 1/3 * arm_sin_f32(phase * 3) + 1/5 * arm_sin_f32(phase * 5) + 1/7 * arm_sin_f32(phase * 7) + 1/9 * arm_sin_f32(phase * 9);
		//uint32_t out = (uint32_t)(out_temp * 4096);
		//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, out);
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t gpio){
	if(gpio == GPIO_PIN_10){
		fallCntr++;
	}
}
uint8_t gencrc(uint8_t *data, size_t len)
{
    uint8_t crc = 0xff;
    size_t i, j;
    for (i = 0; i < len; i++) {
        crc ^= data[i];
        for (j = 0; j < 8; j++) {
            if ((crc & 0x80) != 0)
                crc = (uint8_t)((crc << 1) ^ 0x31);
            else
                crc <<= 1;
        }
    }
    if(crc == 0xAA)
    	crc ^= 0xFF;
    return crc;
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
