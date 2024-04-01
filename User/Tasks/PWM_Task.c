//
// Created by 1 on 2024/3/30.
//
#include "PWM_Task.h"
#include "tim.h"
#include "KEY_Task.h"

uint16_t PSC=0, ARR=0;

static HAL_StatusTypeDef PWM_Self_Cal(){
	for (PSC = 0; PSC < (TCK-1); ++PSC ) {
		for ( ARR = 0; ARR < 0xffff; ++ARR ) {
			if ((TCK/((PSC+1)*(ARR+1))) == Oscillograph.pwmFreq )
				return HAL_OK;
		}
	}
	return HAL_ERROR;
}

void PWM_Ctrl(){
	if (Oscillograph.pwmState )
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	else
		HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);

	htim3.Instance->CCR1 = (ARR * Oscillograph.pwmDuty) / 100;

	if (Oscillograph.pwmPolarity )
		htim3.Instance->CR1 |= 0x0010;
	else
		htim3.Instance->CR1 &= 0xffef;

	if (PWM_Self_Cal() == HAL_OK ){
		htim3.Instance->PSC = PSC;
		htim3.Instance->ARR = ARR;
	}


}