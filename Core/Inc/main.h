/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

#define SAMPLING_TIMES	300

typedef struct {
	int32_t ec_data;
	uint8_t channel;
	uint8_t coupler[2];
	uint8_t attenuator[2];

	uint8_t pwmState;
	uint16_t pwmDuty;
	uint8_t pwmPolarity;
	uint32_t pwmSetFreq;
	uint32_t pwmFreq;
	uint16_t pwmPSC;
	uint16_t pwmARR;

	int8_t adcSamplingTime[2];

	uint16_t adcAcquisition[SAMPLING_TIMES][2];
	float voltage[SAMPLING_TIMES][2];
	float vpp[2];
	float freqGather[2];


	uint8_t displayPause;
	float displayVoltage[2];
	float median[2];
	uint16_t triggerNumber[2];
	float gainFactor[2];


}OscillographState_Type;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define clamp(val, min, max) ({			\
	typeof(val) __val = (val);		\
	typeof(min) __min = (min);		\
	typeof(max) __max = (max);		\
	(void) (&__val == &__min);		\
	(void) (&__val == &__max);		\
	__val = __val < __min ? __min: __val;	\
	__val > __max ? __max: __val; })


/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern OscillographState_Type Oscillograph;

extern uint8_t ADC_DMA_FLAG;

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Key_EC_Pin GPIO_PIN_13
#define Key_EC_GPIO_Port GPIOC
#define RELAY_A_1_Pin GPIO_PIN_2
#define RELAY_A_1_GPIO_Port GPIOA
#define RELAY_A_2_Pin GPIO_PIN_3
#define RELAY_A_2_GPIO_Port GPIOA
#define RELAY_B_1_Pin GPIO_PIN_4
#define RELAY_B_1_GPIO_Port GPIOA
#define RELAY_B_2_Pin GPIO_PIN_5
#define RELAY_B_2_GPIO_Port GPIOA
#define LED_B_Pin GPIO_PIN_7
#define LED_B_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_0
#define LED_G_GPIO_Port GPIOB
#define LED_R_Pin GPIO_PIN_1
#define LED_R_GPIO_Port GPIOB
#define Key_2_Pin GPIO_PIN_10
#define Key_2_GPIO_Port GPIOB
#define Key_1_Pin GPIO_PIN_11
#define Key_1_GPIO_Port GPIOB
#define Key_3_Pin GPIO_PIN_12
#define Key_3_GPIO_Port GPIOB
#define Key_4_Pin GPIO_PIN_13
#define Key_4_GPIO_Port GPIOB
#define BLK_Pin GPIO_PIN_15
#define BLK_GPIO_Port GPIOA
#define CS_Pin GPIO_PIN_4
#define CS_GPIO_Port GPIOB
#define DC_Pin GPIO_PIN_8
#define DC_GPIO_Port GPIOB
#define RES_Pin GPIO_PIN_9
#define RES_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define LED_R_ON	LED_R_GPIO_Port->BSRR = LED_R_Pin
#define LED_G_ON	LED_G_GPIO_Port->BSRR = LED_G_Pin
#define LED_B_ON	LED_B_GPIO_Port->BSRR = LED_B_Pin

#define LED_R_OFF	LED_R_GPIO_Port->BSRR = (uint32_t)LED_R_Pin << 16U
#define LED_G_OFF	LED_G_GPIO_Port->BSRR = (uint32_t)LED_G_Pin << 16U
#define LED_B_OFF	LED_B_GPIO_Port->BSRR = (uint32_t)LED_B_Pin << 16U

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
