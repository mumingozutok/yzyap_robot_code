/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPIO9_Pin GPIO_PIN_13
#define GPIO9_GPIO_Port GPIOC
#define GPIO17_Pin GPIO_PIN_14
#define GPIO17_GPIO_Port GPIOC
#define GPIO3_Pin GPIO_PIN_0
#define GPIO3_GPIO_Port GPIOH
#define GPIO4_Pin GPIO_PIN_1
#define GPIO4_GPIO_Port GPIOH
#define GPIO5_Pin GPIO_PIN_0
#define GPIO5_GPIO_Port GPIOC
#define GPIO6_Pin GPIO_PIN_1
#define GPIO6_GPIO_Port GPIOC
#define GPIO7_Pin GPIO_PIN_2
#define GPIO7_GPIO_Port GPIOC
#define GPIO8_Pin GPIO_PIN_3
#define GPIO8_GPIO_Port GPIOC
#define GPIO18_Pin GPIO_PIN_4
#define GPIO18_GPIO_Port GPIOA
#define GPIO15_Pin GPIO_PIN_13
#define GPIO15_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
