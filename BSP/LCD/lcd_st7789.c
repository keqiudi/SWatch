#include "sys.h"
#include "lcd_st7789.h"


/* 我们这里使用硬件SPI，其他的CS/DC/RST引脚使用软件模拟*/
static void LCD_GPIO_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure = {0};
		
		__HAL_RCC_GPIOB_CLK_ENABLE();
		
		GPIO_InitStructure.Pin = LCD_CS_PIN | LCD_DC_PIN | LCD_RST_PIN;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; // 50MHz即可
		HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);
		
		HAL_GPIO_WritePin(GPIOB, LCD_CS_PIN | LCD_DC_PIN | LCD_RST_PIN, GPIO_PIN_SET); //默认拉高
}


void LCD_SPI_SetBit(u8 bit)
{
	if(bit == 8)
		hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	else if(bit == 16)
		hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
	//HAL_SPI_DeInit(&hspi1);      
	HAL_SPI_Init(&hspi1);
}


/* 硬件SPI向ST7789发送一个字节*/
static void LCD_Write_Byte(u8 data)
{
		//hardware spi
		HAL_SPI_Transmit(&hspi1,&data,1,HAL_MAX_DELAY);
}

/* 软件spi*/
static void LCD_Write_Byte_Soft(u8 data)
{
		//software spi
	  for(uint8_t i=0;i<8;i++)
		{
				LCD_SCLK_LOW();
			  if( data & (0x80 >> i))
					LCD_MOSI_HIGH();
				else
					LCD_MOSI_LOW();
				
				LCD_SCLK_HIGH();
		}
}

/* LCD向ST7789写命令*/
void LCD_Write_Cmd(u8 data)
{
	LCD_DC_LOW();
	LCD_Write_Byte(data);
}

/*LCD向ST7789写8bit数据*/
void LCD_Write_Data8(u8 data)
{
	LCD_DC_HIGH();
	LCD_Write_Byte(data);
}
/*LCD向ST7789写16bit数据*/
void LCD_Write_Data16(u16 data)
{
	LCD_DC_HIGH();
	LCD_Write_Byte(data >> 8);
	LCD_Write_Byte(data & 0xFF);
}

/**
 * @brief   设置LCD的显示窗口（起始和结束地址）
 * @param   x1 显示区域起始列地址
 * @param   y1 显示区域起始行地址
 * @param   x2 显示区域结束列地址
 * @param   y2 显示区域结束行地址
 *
 * @note    本函数通过向LCD控制芯片发送命令和参数，设置后续像素数据写入的目标区域。
 *          只有设置好显示窗口后，才能高效地在指定区域进行图像、文字等内容的刷新操作。
 *          常用于绘制图形、局部刷新等场景，提升LCD操作效率。
 */
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	
	LCD_Write_Cmd(0x2A); // CASET命令
	LCD_Write_Data8(x1 >> 8); // XS高字节
	LCD_Write_Data8(x1 & 0xFF); // XS低字节
	LCD_Write_Data8(x2 >> 8); // XE高字节
	LCD_Write_Data8(x2 & 0xFF); // XE低字节
	
	LCD_Write_Cmd(0x2B); // RASET命令
	LCD_Write_Data8(y1 >> 8); // YS高字节
	LCD_Write_Data8(y1 & 0xFF); // YS低字节
	LCD_Write_Data8(y2 >> 8); // YE高字节
	LCD_Write_Data8(y2 & 0xFF); // YE低字节
	
	LCD_Write_Cmd(0x2C); // 发送该命令，LCD开始等待接收显存数据
}


/* LCD退出睡眠  */
void LCD_ST7789_SleepOut(void)
{
	
}

/* LCD进入睡眠 */
void LCD_ST7789_SleepIn(void)
{
	
}

void LCD_Init()
{
	LCD_GPIO_Init();
	LCD_CS_LOW();		//chip select
	
	LCD_RST_LOW(); //复位芯片
	delay_ms(100);
	LCD_RST_HIGH();
	delay_ms(100);
	
	/*开始初始化序列*/
 //************* Start Initial Sequence **********//

    // 3. 退出睡眠模式，唤醒LCD
    LCD_Write_Cmd(0x11);   // Sleep Out（唤醒命令）
    delay_ms(120);      // 等待120ms，确保芯片完全唤醒（手册要求）

    // 4. 设置 Porch 参数（显示驱动时的帧间隔、同步等，影响稳定性）
    LCD_Write_Cmd(0xB2);   // Porch Setting
    LCD_Write_Data8(0x0C); // 前 Porch
    LCD_Write_Data8(0x0C); // 后 Porch
    LCD_Write_Data8(0x00); // 间隔时间
    LCD_Write_Data8(0x33); // 帧间隔/时序
    LCD_Write_Data8(0x33); // 帧间隔/时序

    // 5. 开启撕裂信号（Tearing Effect Line），通常用于同步显示，0表示关闭
    LCD_Write_Cmd(0x35);   
    LCD_Write_Data8(0x00);

    // 6. 设置内存访问控制（MADCTL），决定屏幕的扫描方向和RGB排列
    LCD_Write_Cmd(0x36);   
    if (USE_HORIZONTAL == 0)
        LCD_Write_Data8(0x00); // 竖屏，正常方向
    else if (USE_HORIZONTAL == 1)
        LCD_Write_Data8(0xC0); // 横屏，水平翻转
    else if (USE_HORIZONTAL == 2)
        LCD_Write_Data8(0x70); // 竖屏，垂直翻转
    else
        LCD_Write_Data8(0xA0); // 横屏，其他方向

    // 7. 设置像素格式（COLMOD），0x05表示16位色(RGB565)
    LCD_Write_Cmd(0x3A);   
    LCD_Write_Data8(0x05);

    // 8. 设置门极驱动控制（Gate Control），影响扫描驱动电压
    LCD_Write_Cmd(0xB7);   
    LCD_Write_Data8(0x35);

    // 9. 设置VCOM电压，影响对比度和显示质量
    LCD_Write_Cmd(0xBB);   
    LCD_Write_Data8(0x2D);

    // 10. 设置电源控制（Power Control 1），调节显示驱动电压
    LCD_Write_Cmd(0xC0);   
    LCD_Write_Data8(0x2C);

    // 11. 设置电源控制（Power Control 2），调节内部偏置
    LCD_Write_Cmd(0xC2);   
    LCD_Write_Data8(0x01);

    // 12. 设置电源控制（Power Control 3），进一步调节电压参数
    LCD_Write_Cmd(0xC3);   
    LCD_Write_Data8(0x15);

    // 13. 设置电源控制（Power Control 4），进一步调节电压参数
    LCD_Write_Cmd(0xC4);   
    LCD_Write_Data8(0x20);

    // 14. 设置驱动控制（VDV and VRH Command Enable），影响帧速率和电压
    LCD_Write_Cmd(0xC6);   
    LCD_Write_Data8(0x0F);

    // 15. 设置正向电压控制（Positive Voltage Gamma Control）
    LCD_Write_Cmd(0xD0);   
    LCD_Write_Data8(0xA4);
    LCD_Write_Data8(0xA1);

    // 16. 设置负向电压控制（Negative Voltage Gamma Control）
    LCD_Write_Cmd(0xD6);   
    LCD_Write_Data8(0xA1);

    // 17. 设置Gamma校准参数（影响色彩表现和灰阶显示）
    LCD_Write_Cmd(0xE0);   
    LCD_Write_Data8(0x70);
    LCD_Write_Data8(0x05);
    LCD_Write_Data8(0x0A);
    LCD_Write_Data8(0x0B);
    LCD_Write_Data8(0x0A);
    LCD_Write_Data8(0x27);
    LCD_Write_Data8(0x2F);
    LCD_Write_Data8(0x44);
    LCD_Write_Data8(0x47);
    LCD_Write_Data8(0x37);
    LCD_Write_Data8(0x14);
    LCD_Write_Data8(0x14);
    LCD_Write_Data8(0x29);
    LCD_Write_Data8(0x2F);

    LCD_Write_Cmd(0xE1);   
    LCD_Write_Data8(0x70);
    LCD_Write_Data8(0x07);
    LCD_Write_Data8(0x0C);
    LCD_Write_Data8(0x08);
    LCD_Write_Data8(0x08);
    LCD_Write_Data8(0x04);
    LCD_Write_Data8(0x2F);
    LCD_Write_Data8(0x33);
    LCD_Write_Data8(0x46);
    LCD_Write_Data8(0x18);
    LCD_Write_Data8(0x15);
    LCD_Write_Data8(0x15);
    LCD_Write_Data8(0x2B);
    LCD_Write_Data8(0x2D);

    // 18. 显示反相（Display Inversion On），让颜色更鲜亮，防止残影
    LCD_Write_Cmd(0x21);

    // 19. 开启显示（Display On）
    LCD_Write_Cmd(0x29);
}
