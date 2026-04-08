#include "bsp_lcd.h"
#include "lv_port_disp.h"
#include "lcdfont.h"

#define DISPLAY_OFFSET_X 0 //显示区域为x:0~239 y:20~299
#define DISPLAY_OFFSET_Y 20

extern lv_display_t * disp; // lvgl显示对象


void LCD_Fill(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 color)
{          
	u16 i,j; 
	LCD_Address_Set(x_start,y_start+DISPLAY_OFFSET_Y,x_end-1,y_end+DISPLAY_OFFSET_Y-1);//设置显示范围
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
	
	LCD_Address_Set(x_start,y_start+DISPLAY_OFFSET_Y,x_end,y_end+DISPLAY_OFFSET_Y);
  LCD_SPI_SetBit(16);
	
	HAL_SPI_Transmit_DMA(&hspi1,(uint8_t*)color,size); // 每个像素16bit
}

/*设置LCD背光源亮度*/
void LCD_Set_Light(uint8_t duty)
{
	if(duty >=5 && duty <=100)
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,duty*(99+1)/100); // 自动重装载寄存器ARR = 99+1，比较寄存器CCR取值范围0~99，对应0%~100%占空比
}

/*开启LCD背光*/
void LCD_Open_BackLight()
{
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
}

/*关闭LCD背光*/
void LCD_Close_BackLight()
{
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,0); 
	// HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_3); 如果想要更低功耗可以采取这个
}


void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
		if(hspi == &hspi1)
		{
				LCD_SPI_SetBit(8);
			  lv_display_flush_ready(disp);
		}
}

/* 其余部分是厂家给的画点、画线等等LCD的驱动，我这里不使用就不移植了*/


void LCD_WR_DATA(u16 dat)
{
	uint8_t temp[2];
	temp[0]=(dat>>8)&0xff;
	temp[1]=dat&0xff;
	HAL_SPI_Transmit(&hspi1,temp,2,1);
	
}
/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 temp,sizex,t,m=0;
	u16 i,TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    //得到偏移后的值
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //设置光标位置 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12)temp=ascii_1206[num][i];		       //调用6x12字体
		else if(sizey==16)temp=ascii_1608[num][i];		 //调用8x16字体
		else if(sizey==24)temp=ascii_2412[num][i];		 //调用12x24字体
		else if(sizey==32)temp=ascii_3216[num][i];		 //调用16x32字体
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)//非叠加模式
			{
				if(temp&(0x01<<t))LCD_WR_DATA(fc);
				else LCD_WR_DATA(bc);
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else//叠加模式
			{
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//画一个点
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}   	 	  
}

/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode)
{         
	while(*p!='\0')
	{       
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_Address_Set(x,y,x,y);//设置光标位置 
	LCD_WR_DATA(color);
} 
