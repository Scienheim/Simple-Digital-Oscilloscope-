//
// Created by 1 on 2024/3/28.
//
#include "lcd.h"
#include "gui.h"
#include "LCD_Task.h"
#include "pic.h"
#include <stdio.h>
#include <string.h>

uint16_t ColorTab[5] = {RED, GREEN, BLUE, YELLOW, BRED};

void  Display_StaticUI(void){
	uint16_t i=0;

	GUI_Fill(0,0,160,128,BLACK);

	GUI_ShowString( 5, 0, ( uint8_t * ) "Oscillograph", GREEN, BLACK, 16, 0 );

	GUI_ShowString(110,0,(uint8_t *) "  PWM  ",BLACK,YELLOW,16,0);

	GUI_ShowChinese(110,18,(uint8_t *)"STATE ",WHITE,PURPLE,16,0);

	GUI_ShowString(110,36,(uint8_t *)"      ",BLACK,YELLOW,16,0);

	GUI_ShowChinese(110,54,(uint8_t *)"Freq  ",WHITE,PURPLE,16,0);

	GUI_ShowString(110,72,(uint8_t *)"      ",BLACK,YELLOW,16,0);

	GUI_ShowChinese(110,90,(uint8_t *)"Duty  ",WHITE,PURPLE,16,0);

	GUI_ShowString(110,108,(uint8_t *)"      ",BLACK,YELLOW,16,0);

	GUI_ShowChinese(5,92,(uint8_t *)"Vpp1:   ",BLACK,LIGHTYELLOW, 12,0);
	GUI_ShowChinese(5,106,(uint8_t *)"Vpp2:   ",BLACK,LIGHTBLUE,12,0);

	GUI_ShowChinese(55,92,(uint8_t *)"F1:     ",BLACK,LIGHTYELLOW,12,0);
	GUI_ShowChinese(55,106,(uint8_t *)"F2:     ",BLACK, LIGHTBLUE,12,0);

	for(i=0;i<=128;i=i+2)
	{
		GUI_DrawPoint(106,i,YELLOW);
	}

	for (i = 20; i< 81; ++i) {
		GUI_DrawPoint(0,i,GREEN);
	}

	for(i=0;i<100;i++)
	{
		GUI_DrawPoint(i,81,GREEN);
	}
	for(i=0;i<10;i++)
	{
		GUI_DrawPoint((i*10)+2,82,GREEN);
		GUI_DrawPoint((i*10)+3,82,GREEN);
	}
	for(i=0;i<10;i++)
	{
		GUI_DrawPoint((i*10)+2,83,GREEN);
		GUI_DrawPoint((i*10)+3,83,GREEN);
	}
}