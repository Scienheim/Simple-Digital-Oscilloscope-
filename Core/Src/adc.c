/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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
/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    PA1     ------> ADC1_IN1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_NORMAL;
    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    PA1     ------> ADC1_IN1
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1);

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime){
	uint32_t tmpreg1 = 0, tmpreg2 = 0;
	/* Check the parameters */
	assert_param(IS_ADC_ALL_PERIPH(ADCx));
	assert_param(IS_ADC_CHANNEL(ADC_Channel));
	assert_param(IS_ADC_REGULAR_RANK(Rank));
	assert_param(IS_ADC_SAMPLE_TIME(ADC_SampleTime));
	/* if ADC_Channel_10 ... ADC_Channel_17 is selected */
	if (ADC_Channel > ADC_CHANNEL_9)
	{
		/* Get the old register value */
		tmpreg1 = ADCx->SMPR1;
		/* Calculate the mask to clear */
		tmpreg2 = ADC_SMPR1_SMP10_Msk << (3 * (ADC_Channel - 10));
		/* Clear the old channel sample time */
		tmpreg1 &= ~tmpreg2;
		/* Calculate the mask to set */
		tmpreg2 = (uint32_t)ADC_SampleTime << (3 * (ADC_Channel - 10));
		/* Set the new channel sample time */
		tmpreg1 |= tmpreg2;
		/* Store the new register value */
		ADCx->SMPR1 = tmpreg1;
	}
	else /* ADC_Channel include in ADC_Channel_[0..9] */
	{
		/* Get the old register value */
		tmpreg1 = ADCx->SMPR2;
		/* Calculate the mask to clear */
		tmpreg2 = ADC_SMPR2_SMP0_Msk << (3 * ADC_Channel);
		/* Clear the old channel sample time */
		tmpreg1 &= ~tmpreg2;
		/* Calculate the mask to set */
		tmpreg2 = (uint32_t)ADC_SampleTime << (3 * ADC_Channel);
		/* Set the new channel sample time */
		tmpreg1 |= tmpreg2;
		/* Store the new register value */
		ADCx->SMPR2 = tmpreg1;
	}
	/* For Rank 1 to 6 */
	if (Rank < 7)
	{
		/* Get the old register value */
		tmpreg1 = ADCx->SQR3;
		/* Calculate the mask to clear */
		tmpreg2 = ADC_SQR3_SQ1_Msk << (5 * (Rank - 1));
		/* Clear the old SQx bits for the selected rank */
		tmpreg1 &= ~tmpreg2;
		/* Calculate the mask to set */
		tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 1));
		/* Set the SQx bits for the selected rank */
		tmpreg1 |= tmpreg2;
		/* Store the new register value */
		ADCx->SQR3 = tmpreg1;
	}
		/* For Rank 7 to 12 */
	else if (Rank < 13)
	{
		/* Get the old register value */
		tmpreg1 = ADCx->SQR2;
		/* Calculate the mask to clear */
		tmpreg2 = ADC_SQR2_SQ7_Msk << (5 * (Rank - 7));
		/* Clear the old SQx bits for the selected rank */
		tmpreg1 &= ~tmpreg2;
		/* Calculate the mask to set */
		tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 7));
		/* Set the SQx bits for the selected rank */
		tmpreg1 |= tmpreg2;
		/* Store the new register value */
		ADCx->SQR2 = tmpreg1;
	}
		/* For Rank 13 to 16 */
	else
	{
		/* Get the old register value */
		tmpreg1 = ADCx->SQR1;
		/* Calculate the mask to clear */
		tmpreg2 = ADC_SQR1_SQ13_Msk << (5 * (Rank - 13));
		/* Clear the old SQx bits for the selected rank */
		tmpreg1 &= ~tmpreg2;
		/* Calculate the mask to set */
		tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 13));
		/* Set the SQx bits for the selected rank */
		tmpreg1 |= tmpreg2;
		/* Store the new register value */
		ADCx->SQR1 = tmpreg1;
	}
}

/* USER CODE END 1 */
