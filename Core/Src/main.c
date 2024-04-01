/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"

#include "lcd.h"
#include "gui.h"
#include "LCD_Task.h"
#include "KEY_Task.h"
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

/* USER CODE BEGIN PV */

uint32_t capture_Buf[3] = {0};   //存放计数值
uint8_t capture_Cnt = 0;    //状态标志位
uint32_t high_time;   //高电平时间

uint8_t ADC_DMA_FLAG;

OscillographState_Type Oscillograph;

float max_data[2] = {1.0f, 1.0f};

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
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  LCD_Init();
  Display_StaticUI();

  Oscillograph.pwmDuty = 50;
  Oscillograph.pwmFreq = 2000;
  Oscillograph.pwmPolarity = 1;
  Oscillograph.pwmPSC = 72;
  Oscillograph.pwmARR = 500;

  __HAL_TIM_SetCounter(&htim4, 0x7fff);
  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1 | TIM_CHANNEL_2 );

	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)Oscillograph.adcAcquisition, SAMPLING_TIMES);

	LED_R_ON;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	Key_Handle();

	  switch (capture_Cnt){
		  case 0:
			  capture_Cnt++;
//			  __HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1 | TIM_CHANNEL_2);	//启动输入捕获       或者: __HAL_TIM_ENABLE(&htim5);
			  break;
		  case 3:
			  Oscillograph.freqGather[0] = 1000000.f / (capture_Buf[1]- capture_Buf[0]);    //高电平时间
			  capture_Cnt = 0;  //清空标志位
			  break;

	  }


//	  GUI_ShowNum(0, 0, Oscillograph.adcSamplingTime[0], 1, 12, LIGHTYELLOW, BLACK);
//	  GUI_ShowNum(12, 0, Oscillograph.adcSamplingTime[1], 1, 12, LIGHTGREEN, BLACK);
	  GUI_ShowNum(73, 92, Oscillograph.freqGather[0], 5, 12, BLACK, LIGHTYELLOW);
	  GUI_ShowNum(73, 106, Oscillograph.freqGather[1], 5, 12, BLACK, LIGHTBLUE);

	  if (Oscillograph.pwmState ){
		  GUI_ShowString(110,36,(uint8_t *)"    ON",BLACK,YELLOW,16,0);
	  } else{
		  GUI_ShowString(110,36,(uint8_t *)"   OFF",BLACK,YELLOW,16,0);
	  }
	  GUI_ShowNum(110, 72, Oscillograph.pwmFreq, 6, 16, BLACK, YELLOW);
	  GUI_ShowNum(110, 108, Oscillograph.pwmDuty, 6, 16, BLACK, YELLOW);

	  if (ADC_DMA_FLAG && (Oscillograph.displayPause == 0)) {
		  ADC_DMA_FLAG = 0;

		  for ( int i = 0; i < 2; ++i ) {
			  for ( int j = 0; j < SAMPLING_TIMES; ++j ) {
				  Oscillograph.voltage[j][i] = ( (float )Oscillograph.adcAcquisition[j][i] * 3.3f ) / 4096.0f;

				  if ( Oscillograph.vpp[i] < Oscillograph.voltage[j][i] )
					  Oscillograph.vpp[i] = Oscillograph.voltage[j][i];
				  if ( Oscillograph.vpp[i] <= 0.3 )
					  Oscillograph.freqGather[0] = 0;
			  }
		  }

		  for ( int i = 0; i < 2; ++i ) {
			  for ( int j = 0; j < 200; ++j ) {
				  if ( Oscillograph.voltage[j][i] < max_data[i] ) {
					  for ( ; j < 200; ++j ) {
						  if ( Oscillograph.voltage[j][i] > max_data[i] ) {
							  Oscillograph.triggerNumber[i] = j;
							  break;
						  }
					  }
				  }
				  break;
			  }
		  }

		  for ( int i = 0; i < 2; ++i ) {
			  if ( Oscillograph.vpp[i] > 0.3 ) {
				  Oscillograph.median[i] = Oscillograph.vpp[i] / 2.f;

				  Oscillograph.gainFactor[i] = 18.f / Oscillograph.median[i];

			  }
		  }

		  for ( uint16_t i = Oscillograph.triggerNumber[0]; i < Oscillograph.triggerNumber[0] + 100; ++i ) {
			  Oscillograph.displayVoltage[0] = Oscillograph.voltage[i][0];

			  if ( Oscillograph.displayVoltage[0] >= Oscillograph.median[0] )
				  Oscillograph.displayVoltage[0] =
						  40 - ( Oscillograph.displayVoltage[0] - Oscillograph.median[0] ) * Oscillograph.gainFactor[0];
			  else
				  Oscillograph.displayVoltage[0] =
						  40 + ( Oscillograph.displayVoltage[0] - Oscillograph.median[0] ) * Oscillograph.gainFactor[0];

			  GUI_DrawCurve( 80, Oscillograph.displayVoltage[0] );
			  Oscillograph.displayVoltage[1] = Oscillograph.voltage[i][1];

			  if ( Oscillograph.displayVoltage[1] >= Oscillograph.median[1] )
				  Oscillograph.displayVoltage[1] =
						  40 - ( Oscillograph.displayVoltage[1] - Oscillograph.median[1] ) * Oscillograph.gainFactor[1];
			  else
				  Oscillograph.displayVoltage[1] =
						  40 + ( Oscillograph.displayVoltage[1] - Oscillograph.median[1] ) * Oscillograph.gainFactor[1];

			  GUI_DrawCurve( 80, Oscillograph.displayVoltage[1] );
		  }

		  GUI_ShowNum(35, 92, Oscillograph.vpp[0], 3, 12, BLACK, LIGHTYELLOW);
		  GUI_ShowNum(35, 106, Oscillograph.vpp[1], 3, 12, BLACK, LIGHTBLUE);
	  }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {

	if ( TIM1 == htim->Instance ) {
		switch ( capture_Cnt ) {
			case 1:
				capture_Buf[0] = HAL_TIM_ReadCapturedValue( &htim1, TIM_CHANNEL_1 );//获取当前的捕获值.
				__HAL_TIM_SET_CAPTUREPOLARITY( &htim1, TIM_CHANNEL_1, TIM_ICPOLARITY_FALLING );  //设置为下降沿捕获
				capture_Cnt++;
				break;
			case 2:
				capture_Buf[1] = HAL_TIM_ReadCapturedValue( &htim1, TIM_CHANNEL_1 );//获取当前的捕获值.
				HAL_TIM_IC_Stop_IT( &htim1, TIM_CHANNEL_1 ); //停止捕获   或者: __HAL_TIM_DISABLE(&htim5);
				capture_Cnt++;
		}

	}
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
