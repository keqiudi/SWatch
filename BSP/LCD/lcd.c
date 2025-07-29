#include "lcd.h"


#define OFFSET_Y 20 //��ʾ����Ϊx:0~240 y:20~300



void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	LCD_Address_Set(xsta,ysta+OFFSET_Y,xend-1,yend+OFFSET_Y-1);//������ʾ��Χ
	for(i=ysta;i<yend;i++)
	{		
		for(j=xsta;j<xend;j++)
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
	size = width * height;
	
	LCD_Address_Set(x_start,y_start+OFFSET_Y,x_end-1,y_end+OFFSET_Y-1);
	LCD_SPI_SetBit(16);
	
	HAL_SPI_Transmit_DMA(&hspi1,(uint8_t*)color,size); // һ������16bit

  LCD_SPI_SetBit(8);	
	
}

/*����LCD����Դ����*/
void LCD_Set_Light(uint8_t duty)
{
	if(duty >=5 && duty <=100)
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,duty*320/100);
}

/*����LCD����*/
void LCD_Open_BackLight()
{
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
}

/*�ر�LCD����*/
void LCD_Close_BackLight()
{
	HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_3);
}
