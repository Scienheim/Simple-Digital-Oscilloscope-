//
// Created by 1 on 2024/3/29.
//

#include "KEY_Task.h"
#include "tim.h"
#include "RELAY_Task.h"
#include "adc.h"

uint8_t KEY_MODE, PWM_CTRL_MODE;

KeyState_Type KeyStatus[KEY_NUM];
uint16_t SIQ_Encoder[2];

uint8_t RELAY_STATE[2][2];

static void ButtonState_Judge(KeyState_Type *keyStatus){
	for (uint8_t i = 0; i < KEY_NUM; i++ ){
		uint32_t currentTime = HAL_GetTick();
		switch (keyStatus[i].current_status) {
			case IDLE_STATE:
				if ( keyStatus[i].key_detect == 0 )   //如果按键按下
				{
					keyStatus[i].current_status = PRESS_DETECTED_STATE; //切换到按键按下状态
					keyStatus[i].pressStartTime = currentTime;       //记录按下时间
				}
				break;

			case PRESS_DETECTED_STATE:    //按键 按下状态
				if ( keyStatus[i].key_detect == 1 )       //检测按键松开时间
				{
					keyStatus[i].ReleaseTime = currentTime;  //记录按键放松时间
					keyStatus[i].current_status = RELEASE_DETECTED_STATE;    //如果按下后释放则进入这里
				} else if ( currentTime - keyStatus[i].pressStartTime > LONG_PRESS_THRESHOLD ) //判断按键长按的时长如果符合则跳转到这里。
				{
					keyStatus[i].current_status = LONG_PRESS_STATE;
				} else if ( currentTime - keyStatus[i].pressStartTime > BUTTON_ERROR_Time ) //防止按键卡死
				{
					keyStatus[i].current_status = IDLE_STATE;
				}
				break;
			case LONG_PRESS_STATE_END:    //长按结束后的状态跳转这里等待按键松开 防止一直处于长按状态   /
				if ( keyStatus[i].key_detect == 1 ) {
					keyStatus[i].current_status = IDLE_STATE;
				}
				break;

			case RELEASE_DETECTED_STATE:    //按键释放过后的状态
				if (( keyStatus[i].key_detect == 0 ) &&
					( currentTime - keyStatus[i].ReleaseTime < SHORT_CLICK_THRESHOLD ))        //如果按键再次按下并且第二次时长是在500ms以内按下的
				{
					keyStatus[i].current_status = DOUBLE_PRESS_STATE;
				} else if ( currentTime - keyStatus[i].ReleaseTime > SHORT_CLICK_THRESHOLD ) //判断当前是否在双击范围内
				{
					keyStatus[i].current_status = SHORT_PRESS_STATE;
				} else if ( currentTime - keyStatus[i].ReleaseTime > BUTTON_ERROR_Time ) //按键出现无法判断情况回到初态
				{
					keyStatus[i].current_status = IDLE_STATE;
				}
				break;

			case SHORT_PRESS_STATE:
			case LONG_PRESS_STATE:
			case DOUBLE_PRESS_STATE:
				if (( keyStatus[i].ReleaseTime - currentTime ) > 500 )
					keyStatus[i].current_status = LONG_PRESS_STATE_END;
				break;
			}
	}
}

static void Get_KeyValue(){
	KeyStatus[0].key_detect = KEY1_READ;
	KeyStatus[1].key_detect = KEY2_READ;
	KeyStatus[2].key_detect = KEY3_READ;
	KeyStatus[3].key_detect = KEY4_READ;
	KeyStatus[4].key_detect = KEY5_READ;

	ButtonState_Judge(KeyStatus);


	if (Oscillograph.ec_data == 0){
		Oscillograph.ec_data = (int32_t)__HAL_TIM_GET_COUNTER(&htim4) - 32767;
		__HAL_TIM_SetCounter(&htim4, 0x7fff);
	} else{

	}

}

void Key_Handle(){
	Get_KeyValue();

	if (KeyStatus[4].current_status == SHORT_PRESS_STATE ) {
		KEY_MODE++;
		if (KEY_MODE >= 3 )	KEY_MODE = 0;

		switch ( KEY_MODE ) {
			case 0:
				LED_R_ON;LED_G_OFF;LED_B_OFF;
				break;
			case 1:
				LED_R_OFF;LED_G_ON;LED_B_OFF;
				break;
			case 2:
				LED_R_OFF;LED_G_OFF;LED_B_ON;
				break;
			default:
				LED_R_OFF;LED_G_OFF;LED_B_OFF;
				break;
		}
	}

	switch ( KEY_MODE ) {
		case 0:
		case 1:
			if (Oscillograph.ec_data ){
				Oscillograph.adcSamplingTime[KEY_MODE] += Oscillograph.ec_data / 2;
				Oscillograph.ec_data = 0;
				Oscillograph.adcSamplingTime[KEY_MODE] = clamp(Oscillograph.adcSamplingTime[KEY_MODE], 0, 7);
				ADC_RegularChannelConfig(ADC1, KEY_MODE, KEY_MODE, Oscillograph.adcSamplingTime[KEY_MODE] );
			}
			if (KeyStatus[3].current_status == SHORT_PRESS_STATE ) {
				Oscillograph.displayPause ^= 1;
			}
			if (KeyStatus[0].current_status == SHORT_PRESS_STATE ){
				RELAY_STATE[KEY_MODE][0] ^= 1;
				RELAY_Set(KEY_MODE, 0, RELAY_STATE[KEY_MODE][0]);
			}
			if (KeyStatus[1].current_status == SHORT_PRESS_STATE ){
				RELAY_STATE[KEY_MODE][1] ^= 1;
				RELAY_Set(KEY_MODE, 0, RELAY_STATE[KEY_MODE][1]);
			}
			break;
		case 2:
			if (KeyStatus[0].current_status == SHORT_PRESS_STATE )
				PWM_CTRL_MODE = 0;
			else if (KeyStatus[1].current_status == SHORT_PRESS_STATE )
				PWM_CTRL_MODE = 1;

			if (KeyStatus[2].current_status == SHORT_PRESS_STATE )
				Oscillograph.pwmPolarity ^= 1;

			if (KeyStatus[3].current_status == LONG_PRESS_STATE )
				Oscillograph.pwmState ^= 1;

			if (Oscillograph.ec_data ){
				if (PWM_CTRL_MODE ){
					Oscillograph.pwmDuty += Oscillograph.ec_data / 2;
					Oscillograph.pwmDuty = clamp(Oscillograph.pwmDuty, 0, 100);
				} else{
					Oscillograph.pwmSetFreq += Oscillograph.ec_data * 500 / 2;
					Oscillograph.pwmSetFreq = clamp(Oscillograph.pwmSetFreq, 0, 100000);
					for ( int i = 72; i > 0 ; ++i ) {
						uint8_t temp;
						for ( int j = 0; j < 0xffff; ++j ) {
							if ( Oscillograph.pwmSetFreq == (72000000 / (i * j)) ){
								Oscillograph.pwmPSC = i;
								Oscillograph.pwmARR = j;
								Oscillograph.pwmFreq = Oscillograph.pwmSetFreq;
								temp = 1;
								break;
							}
						}
						if (temp )
							break;
					}
				}
				Oscillograph.ec_data = 0;
			}

			if (Oscillograph.pwmState ) {
				__HAL_TIM_PRESCALER(&htim3, Oscillograph.pwmPSC - 1);
				__HAL_TIM_SetAutoreload(&htim3, Oscillograph.pwmARR - 1);
//				__HAL_TIM_SetCounter(&htim3, Oscillograph.pwmARR - 1);
				__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, (Oscillograph.pwmDuty * Oscillograph.pwmARR) / 100);

				htim3.Instance->CCER &= 0xfffd;
				htim3.Instance->CCER |= Oscillograph.pwmPolarity << 1;

				HAL_TIM_PWM_Start( &htim3, TIM_CHANNEL_1 );
			} else {
				HAL_TIM_PWM_Stop( &htim3, TIM_CHANNEL_1 );
			}

			break;
	}


}
