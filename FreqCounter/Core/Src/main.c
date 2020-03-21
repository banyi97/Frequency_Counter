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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
TIM_HandleTypeDef htim_gen;
TIM_HandleTypeDef htim_clockCntr;
TIM_HandleTypeDef htim_prell;

uint16_t sevenSegNumbers[] = {
		0x5446,
		0x0044,
		0x1452,
		0x1056,
		0x4054,
		0x5016,
		0x5416,
		0x1044,
		0x5456,
		0x5056
};
enum printMode {Freq, Time};
uint8_t mode = Freq;
uint32_t isPrint = 0;
uint32_t clockCntr = 0;
uint32_t printCntr = 0;
uint16_t sevenSegMask = 0x7456;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
static void DisplayOE_TIM_Config(void);
static void SeftSquareSignalGenerator(void);
static void ClockCounter(void);
static void ButtonPrellTimer(void);
static void PrintFreq(uint32_t, uint8_t);
static void CalcFreq(uint32_t);
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
  /* USER CODE BEGIN 2 */
  char* mess = "Start\r\n";
  HAL_UART_Transmit(&huart2, mess, 7, 5000);
   //DisplayOE_TIM_Config();
  ButtonPrellTimer();
  SeftSquareSignalGenerator();
  ClockCounter();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //PrintFreq(rand()%9999);
	  CalcFreq(printCntr);
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
  HAL_GPIO_WritePin(GPIOA, signalGen_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LE_3_Pin|LE_4_Pin|LE_2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, seg_D_Pin|seg_C_Pin|seg_E_Pin|seg_A_Pin 
                          |seg_DP_Pin|seg_F_Pin|seg_G_Pin|seg_B_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LE_1_GPIO_Port, LE_1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : signal_Pin */
  GPIO_InitStruct.Pin = signal_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(signal_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : signalGen_Pin */
  GPIO_InitStruct.Pin = signalGen_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(signalGen_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LE_3_Pin seg_D_Pin seg_C_Pin seg_E_Pin 
                           seg_A_Pin seg_DP_Pin seg_F_Pin seg_G_Pin 
                           LE_4_Pin seg_B_Pin LE_2_Pin */
  GPIO_InitStruct.Pin = LE_3_Pin|seg_D_Pin|seg_C_Pin|seg_E_Pin 
                          |seg_A_Pin|seg_DP_Pin|seg_F_Pin|seg_G_Pin 
                          |LE_4_Pin|seg_B_Pin|LE_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : LE_1_Pin */
  GPIO_InitStruct.Pin = LE_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LE_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : button_Pin */
  GPIO_InitStruct.Pin = button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(button_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t gpio){
	if(gpio == GPIO_PIN_0){
		clockCntr++;
	}
	if(gpio == GPIO_PIN_10){
		HAL_TIM_Base_Stop_IT(&htim_prell);
		TIM3->CNT &= 0;
		HAL_TIM_Base_Start_IT(&htim_prell);
	}
}

static void DisplayOE_TIM_Config(){

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{
	if (htim->Instance == TIM2)
	{
		isPrint = 1;
		//uint32_t res = (clockCntr + printCntr) / 2;
		//if(clockCntr != 0 && printCntr != 0){
		//	res += 1;
		//}
		printCntr = clockCntr;
		clockCntr = 0;
		char str[30];
		sprintf(str, "Freq: %d Hz\r\n", printCntr);
		HAL_UART_Transmit(&huart2, str, strlen(str), 5000);
	}
	if (htim->Instance == TIM3)
	{
		HAL_TIM_Base_Stop_IT(&htim_prell);
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == GPIO_PIN_RESET){
			char str[30] = "End of prell\r\n";
			HAL_UART_Transmit(&huart2, str, strlen(str), 5000);
			if(mode == Freq){
				mode = Time;
			}
			else{
				mode = Freq;
			}
		}
	}
	if (htim->Instance == TIM4)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
	}
}
static void SeftSquareSignalGenerator(){
	__TIM4_CLK_ENABLE(); // 42Mhz - 1Hz 661,0xFFFF - 2kHz 999,20
	htim_gen.Instance = TIM4;
	htim_gen.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim_gen.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim_gen.Init.Prescaler = 999;
	htim_gen.Init.Period = 20;
	htim_gen.State = HAL_TIM_STATE_RESET;

	HAL_TIM_Base_Init(&htim_gen);

	HAL_NVIC_SetPriority(TIM4_IRQn, 0, 3);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);

	HAL_TIM_Base_Start_IT(&htim_gen);
}
static void ButtonPrellTimer(){
	__TIM3_CLK_ENABLE(); // 42Mhz
	htim_prell.Instance = TIM3;
	htim_prell.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim_prell.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim_prell.Init.Prescaler = 120;
	htim_prell.Init.Period = 0xFFFF;
	htim_prell.State = HAL_TIM_STATE_RESET;

	HAL_TIM_Base_Init(&htim_prell);

	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 2);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
}
static void ClockCounter(){
	__TIM2_CLK_ENABLE(); //84Mhz
	htim_clockCntr.Instance = TIM2;
	htim_clockCntr.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim_clockCntr.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim_clockCntr.Init.Prescaler = 0;
	htim_clockCntr.Init.Period = 0x501BD00;
	htim_clockCntr.State = HAL_TIM_STATE_RESET;

	HAL_TIM_Base_Init(&htim_clockCntr);

	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	HAL_TIM_Base_Start_IT(&htim_clockCntr);
}
static void CalcFreq(uint32_t freq){
	if(freq > 9999)
			freq = 9999; // limit max freq - 4x7seg display
		if(mode == Freq){
			PrintFreq(freq, 0);
		}
		else{
			if(freq != 0){
				float periodeTime = ((float)1 / freq) * 1000; // ms
				if(periodeTime >= 1000){
					PrintFreq((uint32_t)periodeTime, 0);
				}
				else if(periodeTime >= 100){
					periodeTime *= 10;
					PrintFreq((uint32_t)periodeTime, 3);
				}
				else if(periodeTime >= 10){
					periodeTime *= 100;
					PrintFreq((uint32_t)periodeTime, 2);
				}
				else if(periodeTime >= 1){
					periodeTime *= 1000;

					PrintFreq((uint32_t)periodeTime, 1);
				}
				else{
					periodeTime *= 1000;
					PrintFreq((uint32_t)periodeTime, 1);
				}
			}
		}
}
static void PrintFreq(uint32_t freq, uint8_t dpIndex){
	if(freq > 9999)
		freq = 9999; // limit max freq - 4x7seg display

	// SEG1
	uint8_t thouse = freq / 1000;
	HAL_GPIO_WritePin(GPIOB, sevenSegMask, GPIO_PIN_RESET); // reset segment
	dpIndex == 1 ? HAL_GPIO_WritePin(GPIOB, sevenSegNumbers[thouse] | DP_PRINT, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOB, sevenSegNumbers[thouse], GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, LE_1, GPIO_PIN_RESET);
	HAL_Delay(2);
	HAL_GPIO_WritePin(GPIOA, LE_1, GPIO_PIN_SET);
	freq -= thouse * 1000;
	// SEG2
	uint8_t hundr = freq / 100;
	HAL_GPIO_WritePin(GPIOB, sevenSegMask, GPIO_PIN_RESET); // reset segment
	dpIndex == 2 ? HAL_GPIO_WritePin(GPIOB, sevenSegNumbers[hundr] | DP_PRINT, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOB, sevenSegNumbers[hundr], GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, LE_2, GPIO_PIN_RESET);
	HAL_Delay(2);
	HAL_GPIO_WritePin(GPIOB, LE_2, GPIO_PIN_SET);
	freq -= hundr * 100;
	// SEG3
	uint8_t ten = freq / 10;
	HAL_GPIO_WritePin(GPIOB, sevenSegMask, GPIO_PIN_RESET); // reset segment
	dpIndex == 3 ? HAL_GPIO_WritePin(GPIOB, sevenSegNumbers[ten] | DP_PRINT, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOB, sevenSegNumbers[ten], GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, LE_3, GPIO_PIN_RESET);
	HAL_Delay(2);
	HAL_GPIO_WritePin(GPIOB, LE_3, GPIO_PIN_SET);
	freq -= ten * 10;
	// SEG4
	HAL_GPIO_WritePin(GPIOB, sevenSegMask, GPIO_PIN_RESET); // reset segment
	HAL_GPIO_WritePin(GPIOB, sevenSegNumbers[freq], GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, LE_4, GPIO_PIN_RESET);
	HAL_Delay(2);
	HAL_GPIO_WritePin(GPIOB, LE_4, GPIO_PIN_SET);
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
