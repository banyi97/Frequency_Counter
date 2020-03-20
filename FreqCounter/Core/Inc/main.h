/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f4xx.h"
#include "stm32f4xx_hal_cortex.h"
#include "stm32f4xx_hal_tim.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define LE_1 GPIO_PIN_15
#define LE_2 GPIO_PIN_7
#define LE_3 GPIO_PIN_0
#define LE_4 GPIO_PIN_5

#define COUNTER_TIMER_FREQ 84000000 // 84Mhz
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define signal_Pin GPIO_PIN_0
#define signal_GPIO_Port GPIOA
#define signal_EXTI_IRQn EXTI0_IRQn
#define signalGen_Pin GPIO_PIN_1
#define signalGen_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define LE_3_Pin GPIO_PIN_0
#define LE_3_GPIO_Port GPIOB
#define seg_D_Pin GPIO_PIN_1
#define seg_D_GPIO_Port GPIOB
#define seg_C_Pin GPIO_PIN_2
#define seg_C_GPIO_Port GPIOB
#define seg_E_Pin GPIO_PIN_10
#define seg_E_GPIO_Port GPIOB
#define seg_A_Pin GPIO_PIN_12
#define seg_A_GPIO_Port GPIOB
#define seg_DP_Pin GPIO_PIN_13
#define seg_DP_GPIO_Port GPIOB
#define seg_F_Pin GPIO_PIN_14
#define seg_F_GPIO_Port GPIOB
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define LE_1_Pin GPIO_PIN_15
#define LE_1_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define seg_G_Pin GPIO_PIN_4
#define seg_G_GPIO_Port GPIOB
#define LE_4_Pin GPIO_PIN_5
#define LE_4_GPIO_Port GPIOB
#define seg_B_Pin GPIO_PIN_6
#define seg_B_GPIO_Port GPIOB
#define LE_2_Pin GPIO_PIN_7
#define LE_2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
