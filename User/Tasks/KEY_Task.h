//
// Created by 1 on 2024/3/29.
//

#ifndef DIGTAL_OSCILLOGRAPH_KEY_TASK_H
#define DIGTAL_OSCILLOGRAPH_KEY_TASK_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"


typedef enum{
	IDLE_STATE,             //空闲
	PRESS_DETECTED_STATE,   //按键按下
	RELEASE_DETECTED_STATE, //按键释放
	SHORT_PRESS_STATE,      //短按
	LONG_PRESS_STATE,       //长按
	DOUBLE_PRESS_STATE,     //双击
	LONG_PRESS_STATE_END    //长按结束状态
} ButtonState_Type;

typedef struct {
	uint8_t key_detect;
	ButtonState_Type current_status;
	uint32_t pressStartTime;
	uint32_t ReleaseTime;
	uint32_t lastReleaseTime;
}KeyState_Type;

#define KEY_NUM	5

#define KEY1_READ    HAL_GPIO_ReadPin(Key_1_GPIO_Port, Key_1_Pin)
#define KEY2_READ    HAL_GPIO_ReadPin(Key_2_GPIO_Port, Key_2_Pin)
#define KEY3_READ    HAL_GPIO_ReadPin(Key_3_GPIO_Port, Key_3_Pin)
#define KEY4_READ    HAL_GPIO_ReadPin(Key_4_GPIO_Port, Key_4_Pin)
#define KEY5_READ    HAL_GPIO_ReadPin(Key_EC_GPIO_Port, Key_EC_Pin)

#define SHORT_CLICK_THRESHOLD   400    // 这个是第一次松开时间和第二次按下时间的判断时长
#define LONG_PRESS_THRESHOLD    1000   // 定义长按的时间阈值（以HAL_GetTick()时基为单位）
#define BUTTON_ERROR_Time   1500       // 按键长久状态卡死阈值


/* Exported types ------------------------------------------------------------*/
extern KeyState_Type KeyStatus[KEY_NUM];

void Key_Handle();

#endif //DIGTAL_OSCILLOGRAPH_KEY_TASK_H
