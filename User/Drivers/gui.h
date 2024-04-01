#ifndef __GUI_H__
#define __GUI_H__
#include "main.h"

void GUI_DrawPoint(uint16_t x,uint16_t y,uint16_t color);
void GUI_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);
void GUI_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

void GUI_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t fill, uint16_t color);
void Gui_DrawCircle(uint16_t xc, uint16_t yc,uint16_t c,uint16_t r, uint16_t fill, uint16_t color);
void GUI_DrawTriangel(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, uint16_t fill,
                      uint16_t color);

void GUI_ShowChar( uint16_t x, uint16_t y, uint16_t w_c, uint16_t b_c, uint8_t num, uint8_t size, uint8_t mode);
void GUI_ShowNum( uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint16_t num_color,
                  uint16_t back_color );

void GUI_ShowString( uint16_t x, uint16_t y, uint8_t *p, uint16_t word_color, uint16_t back_color, uint8_t size,
					 uint8_t mode );
void GUI_DrawFont16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode);
void GUI_DrawFont24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode);
void GUI_DrawFont32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode);
void GUI_ShowChinese( uint16_t x, uint16_t y, uint8_t *str, uint16_t fc, uint16_t bc, uint8_t size, uint8_t mode );

void Gui_Drawbmp16( uint16_t x, uint16_t y, uint16_t width, uint16_t height, const unsigned char *p ); //œ‘ æ40*40 QQÕº∆¨
void Gui_StrCenter(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *str,uint8_t size,uint8_t mode);

void GUI_DrawCurve(uint8_t yOffset, int16_t rawValue);

#endif
