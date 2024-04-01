#include "lcd.h"
#include "string.h"
#include "font.h"
#include "gui.h"

/*******************************************************************
 * @name       :void GUI_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
 * @date       :2018-08-09 
 * @function   :draw a point in LCD screen
 * @parameters :x:the x coordinate of the point
                y:the y coordinate of the point
                color:the color value of the point
 * @retvalue   :None
********************************************************************/
void GUI_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_SetCursor(x,y);//设置光标位置 
	Lcd_WriteData_16Bit(color); 
}

/*******************************************************************
 * @name       :void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
 * @date       :2018-08-09 
 * @function   :fill the specified area
 * @parameters :sx:the bebinning x coordinate of the specified area
                sy:the bebinning y coordinate of the specified area
                ex:the ending x coordinate of the specified area
                ey:the ending y coordinate of the specified area
                color:the filled color value
 * @retvalue   :None
********************************************************************/
void GUI_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{
    uint16_t i,j;
    uint16_t width=ex-sx+1; 		//得到填充的宽度
    uint16_t height=ey-sy+1;		//高度
    LCD_SetWindows(sx,sy,ex,ey);//设置显示窗口
    for(i=0;i<height;i++){
        for(j=0;j<width;j++)
        Lcd_WriteData_16Bit(color);	//写入数据
    }
    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口设置为全屏
}

/*******************************************************************
 * @name       :void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
 * @date       :2018-08-09 
 * @function   :Draw a line between two points
 * @parameters :x1:the bebinning x coordinate of the line
                y1:the bebinning y coordinate of the line
                x2:the ending x coordinate of the line
                y2:the ending y coordinate of the line
 * @retvalue   :None
********************************************************************/
void GUI_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;

    delta_x=x2-x1; //计算坐标增量
    delta_y=y2-y1;
    uRow=x1;
    uCol=y1;
    if(delta_x>0)incx=1; //设置单步方向
    else if(delta_x==0)incx=0;//垂直线
    else {incx=-1;delta_x=-delta_x;}
    if(delta_y>0)incy=1;
    else if(delta_y==0)incy=0;//水平线
    else{incy=-1;delta_y=-delta_y;}
    if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
    else distance=delta_y;
    for(uint16_t t=0; t <= distance+1; t++ )//画线输出
    {
        GUI_DrawPoint(uRow, uCol, color);//画点
        xerr+=delta_x ;
        yerr+=delta_y ;
        if(xerr>distance){
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance){
            yerr-=distance;
            uCol+=incy;
        }
    }
} 

/*****************************************************************************
 * @name       :GUI_DrawRectangle
 * @date       :2018-08-09 
 * @function   :Draw a rectangle
 * @parameters :x1:     the bebinning x coordinate of the rectangle
 * @parameters :y1:     the bebinning y coordinate of the rectangle
 * @parameters :x2:     the ending x coordinate of the rectangle
 * @parameters :y2:     the ending y coordinate of the rectangle
 * @parameters :fill:   fill the rectangle or not
 *                          1   filling
 *                          0   no filling
 * @parameters :color:  the color of the rectangle
 * @retvalue   :None
******************************************************************************/
void GUI_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t fill, uint16_t color)
{
    if (fill)
        GUI_Fill(x1,y1,x2,y2,POINT_COLOR);
    else {
        GUI_DrawLine(x1, y1, x2, y1, color);
        GUI_DrawLine(x1, y1, x1, y2, color);
        GUI_DrawLine(x1, y2, x2, y2, color);
        GUI_DrawLine(x2, y1, x2, y2, color);
    }

}

/*****************************************************************************
 * @name       :void _draw_circle_8(int xc, int yc, int x, int y, uint16_t c)
 * @date       :2018-08-09 
 * @function   :8 symmetry circle drawing algorithm (internal call)
 * @parameters :xc:the x coordinate of the Circular center 
                yc:the y coordinate of the Circular center
                x:the x coordinate relative to the Circular center
                y:the y coordinate relative to the Circular center
                c:the color value of the circle
 * @retvalue   :None
******************************************************************************/
void _draw_circle_8(int xc, int yc, int x, int y, uint16_t c)
{
    GUI_DrawPoint(xc + x, yc + y, c);

    GUI_DrawPoint(xc - x, yc + y, c);

    GUI_DrawPoint(xc + x, yc - y, c);

    GUI_DrawPoint(xc - x, yc - y, c);

    GUI_DrawPoint(xc + y, yc + x, c);

    GUI_DrawPoint(xc - y, yc + x, c);

    GUI_DrawPoint(xc + y, yc - x, c);

    GUI_DrawPoint(xc - y, yc - x, c);
}

/*****************************************************************************
 * @name       :void gui_circle(int xc, int yc,uint16_t c,int r, int fill)
 * @date       :2018-08-09 
 * @function   :Draw a circle of specified size at a specified location
 * @parameters :xc:the x coordinate of the Circular center
 *              yc:the y coordinate of the Circular center
 *              r:Circular radius
 *              fill:   fill the circle or not
 *                          1   filling
 *                          0   no filling
 * @retvalue   :None
******************************************************************************/  
void Gui_DrawCircle(uint16_t xc, uint16_t yc,uint16_t c,uint16_t r, uint16_t fill, uint16_t color)
{
    int x = 0, y = r, yi, d;

    d = 3 - 2 * r;


    if (fill)
    {
        // 如果填充（画实心圆）
        while (x <= y) {
            for (yi = x; yi <= y; yi++)
                _draw_circle_8(xc, yc, x, yi, c);

            if (d < 0) {
                d = d + 4 * x + 6;
            } else {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    } else
    {
        // 如果不填充（画空心圆）
        while (x <= y) {
            _draw_circle_8(xc, yc, x, y, c);
            if (d < 0) {
                d = d + 4 * x + 6;
            } else {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    }
}



static void _swap(uint16_t *a, uint16_t *b)
{
    uint16_t tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

/*****************************************************************************
 * @name       :GUI_DrawTriangel
 * @date       :2018-08-09 
 * @function   :Draw a triangle at a specified position
 * @parameters :x0:     the bebinning x coordinate of the triangular edge
                y0:     the bebinning y coordinate of the triangular edge
                x1:     the vertex x coordinate of the triangular
                y1:     the vertex y coordinate of the triangular
                x2:     the ending x coordinate of the triangular edge
                y2:     the ending y coordinate of the triangular edge
                fill:   fill the triangel or not
                color:  the color of the triangel
 * @retvalue   :None
******************************************************************************/ 
void GUI_DrawTriangel(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, uint16_t fill,
                      uint16_t color)
{
    if (fill ){
        uint16_t a, b, y, last;
        int dx01, dy01, dx02, dy02, dx12, dy12;
        long sa = 0;
        long sb = 0;
        if (y0 > y1)
        {
            _swap(&y0,&y1);
            _swap(&x0,&x1);
        }
        if (y1 > y2)
        {
            _swap(&y2,&y1);
            _swap(&x2,&x1);
        }
        if (y0 > y1)
        {
            _swap(&y0,&y1);
            _swap(&x0,&x1);
        }
        if(y0 == y2)
        {
            a = b = x0;
            if(x1 < a)
            {
                a = x1;
            }
            else if(x1 > b)
            {
                b = x1;
            }
            if(x2 < a)
            {
                a = x2;
            }
            else if(x2 > b)
            {
                b = x2;
            }
            GUI_Fill(a,y0,b,y0,color);
            return;
        }
        dx01 = x1 - x0;
        dy01 = y1 - y0;
        dx02 = x2 - x0;
        dy02 = y2 - y0;
        dx12 = x2 - x1;
        dy12 = y2 - y1;

        if(y1 == y2)
        {
            last = y1;
        }
        else
        {
            last = y1-1;
        }
        for(y=y0; y<=last; y++)
        {
            a = x0 + sa / dy01;
            b = x0 + sb / dy02;
            sa += dx01;
            sb += dx02;
            if(a > b)
            {
                _swap(&a,&b);
            }
            GUI_Fill(a,y,b,y,color);
        }
        sa = dx12 * (y - y1);
        sb = dx02 * (y - y0);
        for(; y<=y2; y++)
        {
            a = x1 + sa / dy12;
            b = x0 + sb / dy02;
            sa += dx12;
            sb += dx02;
            if(a > b)
            {
                _swap(&a,&b);
            }
            GUI_Fill(a,y,b,y,color);
        }
    } else {
        GUI_DrawLine( x0, y0, x1, y1, color );
        GUI_DrawLine( x1, y1, x2, y2, color);
        GUI_DrawLine( x2, y2, x0, y0, color );
    }
}

static uint16_t lastX=0,lastY=0;
static uint8_t firstPoint = 1;
/**
 * @brief
 * @param yOffset:  For speed, it is appropriately scaled to 90 pixel points that can be used to represent the input values
 * @param rawValue: Corresponding converted voltage value
 */
void GUI_DrawCurve(uint8_t yOffset, int16_t rawValue){
    uint16_t i=0,j=0;
    uint16_t x=0,y=0;
    y = yOffset - rawValue;  	//data processing code
    if(firstPoint)//如果是第一次画点，则无需连线，直接描点即可
    {
        GUI_DrawPoint(0,y,GREEN);
        lastX=0;
        lastY=y;
        firstPoint=0;
    }
    else
    {
        x=lastX+1;

        if(x<100)  //不超过屏幕宽度
        {
            GUI_DrawLine(lastX,lastY,x,y,GREEN);
            for(i=20;i<81;i++)
            {
                GUI_DrawPoint(x+1,i,BLACK);//画点
            }
            lastX=x;
            lastY=y;
        }
        else  //超出屏幕宽度，清屏，从第一个点开始绘制，实现动态更新效果
        {
            GUI_DrawPoint(0,y,GREEN);
            lastX=0;
            lastY=y;
        }
    }
}


/*****************************************************************************
 * @name       :void LCD_ShowChar(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, uint8_t num,uint8_t size,uint8_t mode)
 * @date       :2018-08-09 
 * @function   :Display a single English character
 * @parameters :x:the bebinning x coordinate of the Character display position
                y:the bebinning y coordinate of the Character display position
                fc:the color value of display character
                bc:the background color of display character
                num:the ascii code of display character(0~94)
                size:the size of display character
                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/ 
void GUI_ShowChar( uint16_t x, uint16_t y, uint16_t w_c, uint16_t b_c, uint8_t num, uint8_t size, uint8_t mode)
{  
    uint8_t temp;
    uint8_t pos,t;
	uint16_t colortemp=POINT_COLOR;

	num=num-' ';//得到偏移后的值
	LCD_SetWindows(x,y,x+size/2-1,y+size-1);//设置单个文字显示窗口
	if(!mode) //非叠加方式
	{		
		for(pos=0;pos<size;pos++)
		{
			if (size==12)			temp=asc2_1206[num][pos];		//调用1206字体
			else if (size == 16)	temp=asc2_1608[num][pos];		//调用1608字体
			else 					temp=asc2_1005[num][pos];		//调用1005字体
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)Lcd_WriteData_16Bit( w_c);
				else Lcd_WriteData_16Bit( b_c);
				temp>>=1; 
				
		    }
			
		}	
	}else//叠加方式
	{
		for(pos=0;pos<size;pos++)
		{
			if (size==12)			temp=asc2_1206[num][pos];		//调用1206字体
			else if (size == 16)	temp=asc2_1608[num][pos];		//调用1608字体
			else 					temp=asc2_1005[num][pos];		//调用1005字体
			for(t=0;t<size/2;t++)
		    {   
				POINT_COLOR=w_c;
		        if(temp&0x01)LCD_DrawPoint( x + t, y + pos, 0 );//画一个点
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏    	   	 	  
}

/*****************************************************************************
 * @name       :void LCD_ShowString(uint16_t x,uint16_t y,uint8_t size,uint8_t *p,uint8_t mode)
 * @date       :2018-08-09 
 * @function   :Display English string
 * @parameters :x:the bebinning x coordinate of the English string
                y:the bebinning y coordinate of the English string
                p:the start address of the English string
                size:the size of display character
                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
void GUI_ShowString( uint16_t x, uint16_t y, uint8_t *p, uint16_t word_color, uint16_t back_color, uint8_t size,
					 uint8_t mode )
{         
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {
        if(x>(lcddev.width-1)||y>(lcddev.height-1))
            return;
        GUI_ShowChar(x,y,word_color,back_color,*p,size,mode);
        x+=size/2;
        p++;
    }  
} 

/*****************************************************************************
 * @name       :uint32_t mypow(uint8_t m,uint8_t n)
 * @date       :2018-08-09 
 * @function   :get the nth power of m (internal call)
 * @parameters :m:the multiplier
                n:the power
 * @retvalue   :the nth power of m
******************************************************************************/ 
uint32_t mypow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
    while(n--)result*=m;
    return result;
}

/*****************************************************************************
 * @name       :void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
 * @date       :2018-08-09 
 * @function   :Display number
 * @parameters :x:the bebinning x coordinate of the number
                y:the bebinning y coordinate of the number
                num:the number(0~4294967295)
                len:the length of the display number
                size:the size of display number
 * @retvalue   :None
******************************************************************************/
void
GUI_ShowNum( uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint16_t num_color, uint16_t back_color )
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				GUI_ShowChar(x+(size/2)*t,y,num_color,back_color,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	GUI_ShowChar(x+(size/2)*t,y,num_color,back_color,temp+'0',size,0);
	}
} 

/*****************************************************************************
 * @name       :void GUI_DrawFont16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode)
 * @date       :2018-08-09 
 * @function   :Display a single 16x16 Chinese character
 * @parameters :x:the bebinning x coordinate of the Chinese character
                y:the bebinning y coordinate of the Chinese character
                fc:the color value of Chinese character
                bc:the background color of Chinese character
                s:the start address of the Chinese character
                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/ 
void GUI_DrawFont16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode)
{
	uint8_t i,j;
	uint16_t k;
	uint16_t HZnum;
	uint16_t x0=x;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//自动统计汉字数目
	
			
	for (k=0;k<HZnum;k++) 
	{
	  if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
	  { 	LCD_SetWindows(x,y,x+16-1,y+16-1);
		    for(i=0;i<16*2;i++)
		    {
				for(j=0;j<8;j++)
		    	{	
					if(!mode) //非叠加方式
					{
						if(tfont16[k].Msk[i]&(0x80>>j))	Lcd_WriteData_16Bit(fc);
						else Lcd_WriteData_16Bit(bc);
					}
					else
					{
						POINT_COLOR=fc;
						if(tfont16[k].Msk[i]&(0x80>>j)) LCD_DrawPoint( x, y, 0 );//画一个点
						x++;
						if((x-x0)==16)
						{
							x=x0;
							y++;
							break;
						}
					}

				}
				
			}
			
			
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}

	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏  
} 

/*****************************************************************************
 * @name       :void GUI_DrawFont24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode)
 * @date       :2018-08-09 
 * @function   :Display a single 24x24 Chinese character
 * @parameters :x:the bebinning x coordinate of the Chinese character
                y:the bebinning y coordinate of the Chinese character
                fc:the color value of Chinese character
                bc:the background color of Chinese character
                s:the start address of the Chinese character
                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/ 
void GUI_DrawFont24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode)
{
	uint8_t i,j;
	uint16_t k;
	uint16_t HZnum;
	uint16_t x0=x;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//自动统计汉字数目
		
			for (k=0;k<HZnum;k++) 
			{
			  if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
			  { 	LCD_SetWindows(x,y,x+24-1,y+24-1);
				    for(i=0;i<24*3;i++)
				    {
							for(j=0;j<8;j++)
							{
								if(!mode) //非叠加方式
								{
									if(tfont24[k].Msk[i]&(0x80>>j))	Lcd_WriteData_16Bit(fc);
									else Lcd_WriteData_16Bit(bc);
								}
							else
							{
								POINT_COLOR=fc;
								if(tfont24[k].Msk[i]&(0x80>>j)) LCD_DrawPoint( x, y, 0 );//画一个点
								x++;
								if((x-x0)==24)
								{
									x=x0;
									y++;
									break;
								}
							}
						}
					}
					
					
				}				  	
				continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
			}

	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏  
}

/*****************************************************************************
 * @name       :void GUI_DrawFont32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode)
 * @date       :2018-08-09 
 * @function   :Display a single 32x32 Chinese character
 * @parameters :x:the bebinning x coordinate of the Chinese character
                y:the bebinning y coordinate of the Chinese character
                fc:the color value of Chinese character
                bc:the background color of Chinese character
                s:the start address of the Chinese character
                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/ 
void GUI_DrawFont32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode)
{
	uint8_t i,j;
	uint16_t k;
	uint16_t HZnum;
	uint16_t x0=x;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//自动统计汉字数目
	for (k=0;k<HZnum;k++) 
			{
			  if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
			  { 	LCD_SetWindows(x,y,x+32-1,y+32-1);
				    for(i=0;i<32*4;i++)
				    {
						for(j=0;j<8;j++)
				    	{
							if(!mode) //非叠加方式
							{
								if(tfont32[k].Msk[i]&(0x80>>j))	Lcd_WriteData_16Bit(fc);
								else Lcd_WriteData_16Bit(bc);
							}
							else
							{
								POINT_COLOR=fc;
								if(tfont32[k].Msk[i]&(0x80>>j)) LCD_DrawPoint( x, y, 0 );//画一个点
								x++;
								if((x-x0)==32)
								{
									x=x0;
									y++;
									break;
								}
							}
						}
					}
					
					
				}				  	
				continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
			}
	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏  
} 

/*****************************************************************************
 * @name       :void GUI_ShowChinese(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *str,uint8_t size,uint8_t mode)
 * @date       :2018-08-09 
 * @function   :Display Chinese and English strings
 * @parameters :x:the bebinning x coordinate of the Chinese and English strings
                y:the bebinning y coordinate of the Chinese and English strings
                fc:the color value of Chinese and English strings
                bc:the background color of Chinese and English strings
                str:the start address of the Chinese and English strings
                size:the size of Chinese and English strings
                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
void GUI_ShowChinese( uint16_t x, uint16_t y, uint8_t *str, uint16_t fc, uint16_t bc, uint8_t size, uint8_t mode )
{					
	uint16_t x0=x;							  	  
  	uint8_t bHz=0;     //字符或者中文 
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
			if(x>(lcddev.width-size/2)||y>(lcddev.height-size)) 
			return; 
	        if(*str>0x80)bHz=1;//中文 
	        else              //字符
	        {          
		        if(*str==0x0D)//换行符号
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }  
		        else
				{
					if(size>16)//字库中没有集成12X24 16X32的英文字体,用8X16代替
					{  
					GUI_ShowChar(x,y,fc,bc,*str,16,mode);
					x+=8; //字符,为全字的一半 
					}
					else
					{
					GUI_ShowChar(x,y,fc,bc,*str,size,mode);
					x+=size/2; //字符,为全字的一半 
					}
				} 
				str++; 
		        
	        }
        }else//中文 
        {   
			if(x>(lcddev.width-size)||y>(lcddev.height-size)) 
			return;  
            bHz=0;//有汉字库    
			if(size==32)
			GUI_DrawFont32(x,y,fc,bc,str,mode);	 	
			else if(size==24)
			GUI_DrawFont24(x,y,fc,bc,str,mode);	
			else
			GUI_DrawFont16(x,y,fc,bc,str,mode);
				
	        str+=2; 
	        x+=size;//下一个汉字偏移	    
        }						 
    }   
}

/*****************************************************************************
 * @name       :void Gui_StrCenter(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *str,uint8_t size,uint8_t mode)
 * @date       :2018-08-09 
 * @function   :Centered display of English and Chinese strings
 * @parameters :x:the bebinning x coordinate of the Chinese and English strings
                y:the bebinning y coordinate of the Chinese and English strings
                fc:the color value of Chinese and English strings
                bc:the background color of Chinese and English strings
                str:the start address of the Chinese and English strings
                size:the size of Chinese and English strings
                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/ 
void Gui_StrCenter(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *str,uint8_t size,uint8_t mode)
{
	uint16_t len=strlen((const char *)str);
	uint16_t x1=(lcddev.width-len*8)/2;
	GUI_ShowChinese( x1, y, str, fc, bc, size, mode );
} 
 
/*****************************************************************************
 * @name       :void Gui_Drawbmp16(uint16_t x,uint16_t y,const unsigned char *p)
 * @date       :2018-08-09 
 * @function   :Display a 16-bit BMP image
 * @parameters :x:the bebinning x coordinate of the BMP image
                y:the bebinning y coordinate of the BMP image
								p:the start address of image array
 * @retvalue   :None
******************************************************************************/
void Gui_Drawbmp16( uint16_t x, uint16_t y, uint16_t width, uint16_t height, const unsigned char *p )
{
  	int i; 
	unsigned char picH,picL; 
	LCD_SetWindows(x,y,x+width-1,y+height-1);//窗口设置
    for(i=0;i<width*height;i++)
	{	
	 	picL=*(p+i*2);	//数据低位在前
		picH=*(p+i*2+1);				
		Lcd_WriteData_16Bit(picH<<8|picL);  						
	}	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复显示窗口为全屏	
}
