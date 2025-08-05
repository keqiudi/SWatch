#include "lcd.h"
#include "lv_port_disp.h"

#define OFFSET_Y 20 //显示区域为x:0~239 y:20~299

extern lv_display_t * disp; // lvgl显示对象


void LCD_Fill(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 color)
{          
	u16 i,j; 
	LCD_Address_Set(x_start,y_start+OFFSET_Y,x_end-1,y_end+OFFSET_Y-1);//设置显示范围
	for(i=y_start;i<y_end;i++)
	{		
		for(j=x_start;j<x_end;j++)
		{
			LCD_Write_Data16(color);
		}
	} 					  	    
}

void LCD_Fill_DMA(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16* color)
{

	u16 width,height;
	u32 size;
	width = x_end-x_start+1;
	height = y_end-y_start+1;
	size = width * height; // 区域总像素个数
	
	LCD_Address_Set(x_start,y_start+OFFSET_Y,x_end,y_end+OFFSET_Y);
  LCD_SPI_SetBit(16);
	
	HAL_SPI_Transmit_DMA(&hspi1,(uint8_t*)color,size); // 每个像素16bit
}

/*设置LCD背光源亮度*/
void LCD_Set_Light(uint8_t duty)
{
	if(duty >=5 && duty <=100)
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,duty*320/100);
}

/*开启LCD背光*/
void LCD_Open_BackLight()
{
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
}

/*关闭LCD背光*/
void LCD_Close_BackLight()
{
	HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_3);
}


void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
		if(hspi == &hspi1)
		{
				LCD_SPI_SetBit(8);
			  lv_display_flush_ready(disp);
		}
}