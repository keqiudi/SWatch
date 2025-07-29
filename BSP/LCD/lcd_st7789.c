#include "sys.h"
#include "lcd_st7789.h"


/* ��������ʹ��Ӳ��SPI��������CS/DC/RST����ʹ�����ģ��*/
static void LCD_GPIO_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure = {0};
		
		__HAL_RCC_GPIOB_CLK_ENABLE();
		
		GPIO_InitStructure.Pin = LCD_CS_PIN | LCD_DC_PIN | LCD_RST_PIN;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; // 50MHz����
		HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);
		
		HAL_GPIO_WritePin(GPIOB, LCD_CS_PIN | LCD_DC_PIN | LCD_RST_PIN, GPIO_PIN_SET); //Ĭ������
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


/* Ӳ��SPI��ST7789����һ���ֽ�*/
static void LCD_Write_Byte(u8 data)
{
		//hardware spi
		HAL_SPI_Transmit(&hspi1,&data,1,HAL_MAX_DELAY);
}

/* ���spi*/
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

/* LCD��ST7789д����*/
void LCD_Write_Cmd(u8 data)
{
	LCD_DC_LOW();
	LCD_Write_Byte(data);
}

/*LCD��ST7789д8bit����*/
void LCD_Write_Data8(u8 data)
{
	LCD_DC_HIGH();
	LCD_Write_Byte(data);
}
/*LCD��ST7789д16bit����*/
void LCD_Write_Data16(u16 data)
{
	LCD_DC_HIGH();
	LCD_Write_Byte(data >> 8);
	LCD_Write_Byte(data & 0xFF);
}

/**
 * @brief   ����LCD����ʾ���ڣ���ʼ�ͽ�����ַ��
 * @param   x1 ��ʾ������ʼ�е�ַ
 * @param   y1 ��ʾ������ʼ�е�ַ
 * @param   x2 ��ʾ��������е�ַ
 * @param   y2 ��ʾ��������е�ַ
 *
 * @note    ������ͨ����LCD����оƬ��������Ͳ��������ú�����������д���Ŀ������
 *          ֻ�����ú���ʾ���ں󣬲��ܸ�Ч����ָ���������ͼ�����ֵ����ݵ�ˢ�²�����
 *          �����ڻ���ͼ�Ρ��ֲ�ˢ�µȳ���������LCD����Ч�ʡ�
 */
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	
	LCD_Write_Cmd(0x2A); // CASET����
	LCD_Write_Data8(x1 >> 8); // XS���ֽ�
	LCD_Write_Data8(x1 & 0xFF); // XS���ֽ�
	LCD_Write_Data8(x2 >> 8); // XE���ֽ�
	LCD_Write_Data8(x2 & 0xFF); // XE���ֽ�
	
	LCD_Write_Cmd(0x2B); // RASET����
	LCD_Write_Data8(y1 >> 8); // YS���ֽ�
	LCD_Write_Data8(y1 & 0xFF); // YS���ֽ�
	LCD_Write_Data8(y2 >> 8); // YE���ֽ�
	LCD_Write_Data8(y2 & 0xFF); // YE���ֽ�
	
	LCD_Write_Cmd(0x2C); // ���͸����LCD��ʼ�ȴ������Դ�����
}


/* LCD�˳�˯��  */
void LCD_ST7789_SleepOut(void)
{
	
}

/* LCD����˯�� */
void LCD_ST7789_SleepIn(void)
{
	
}

void LCD_Init()
{
	LCD_GPIO_Init();
	LCD_CS_LOW();		//chip select
	
	LCD_RST_LOW(); //��λоƬ
	delay_ms(100);
	LCD_RST_HIGH();
	delay_ms(100);
	
	/*��ʼ��ʼ������*/
 //************* Start Initial Sequence **********//

    // 3. �˳�˯��ģʽ������LCD
    LCD_Write_Cmd(0x11);   // Sleep Out���������
    delay_ms(120);      // �ȴ�120ms��ȷ��оƬ��ȫ���ѣ��ֲ�Ҫ��

    // 4. ���� Porch ��������ʾ����ʱ��֡�����ͬ���ȣ�Ӱ���ȶ��ԣ�
    LCD_Write_Cmd(0xB2);   // Porch Setting
    LCD_Write_Data8(0x0C); // ǰ Porch
    LCD_Write_Data8(0x0C); // �� Porch
    LCD_Write_Data8(0x00); // ���ʱ��
    LCD_Write_Data8(0x33); // ֡���/ʱ��
    LCD_Write_Data8(0x33); // ֡���/ʱ��

    // 5. ����˺���źţ�Tearing Effect Line����ͨ������ͬ����ʾ��0��ʾ�ر�
    LCD_Write_Cmd(0x35);   
    LCD_Write_Data8(0x00);

    // 6. �����ڴ���ʿ��ƣ�MADCTL����������Ļ��ɨ�跽���RGB����
    LCD_Write_Cmd(0x36);   
    if (USE_HORIZONTAL == 0)
        LCD_Write_Data8(0x00); // ��������������
    else if (USE_HORIZONTAL == 1)
        LCD_Write_Data8(0xC0); // ������ˮƽ��ת
    else if (USE_HORIZONTAL == 2)
        LCD_Write_Data8(0x70); // ��������ֱ��ת
    else
        LCD_Write_Data8(0xA0); // ��������������

    // 7. �������ظ�ʽ��COLMOD����0x05��ʾ16λɫ(RGB565)
    LCD_Write_Cmd(0x3A);   
    LCD_Write_Data8(0x05);

    // 8. �����ż��������ƣ�Gate Control����Ӱ��ɨ��������ѹ
    LCD_Write_Cmd(0xB7);   
    LCD_Write_Data8(0x35);

    // 9. ����VCOM��ѹ��Ӱ��ԱȶȺ���ʾ����
    LCD_Write_Cmd(0xBB);   
    LCD_Write_Data8(0x2D);

    // 10. ���õ�Դ���ƣ�Power Control 1����������ʾ������ѹ
    LCD_Write_Cmd(0xC0);   
    LCD_Write_Data8(0x2C);

    // 11. ���õ�Դ���ƣ�Power Control 2���������ڲ�ƫ��
    LCD_Write_Cmd(0xC2);   
    LCD_Write_Data8(0x01);

    // 12. ���õ�Դ���ƣ�Power Control 3������һ�����ڵ�ѹ����
    LCD_Write_Cmd(0xC3);   
    LCD_Write_Data8(0x15);

    // 13. ���õ�Դ���ƣ�Power Control 4������һ�����ڵ�ѹ����
    LCD_Write_Cmd(0xC4);   
    LCD_Write_Data8(0x20);

    // 14. �����������ƣ�VDV and VRH Command Enable����Ӱ��֡���ʺ͵�ѹ
    LCD_Write_Cmd(0xC6);   
    LCD_Write_Data8(0x0F);

    // 15. ���������ѹ���ƣ�Positive Voltage Gamma Control��
    LCD_Write_Cmd(0xD0);   
    LCD_Write_Data8(0xA4);
    LCD_Write_Data8(0xA1);

    // 16. ���ø����ѹ���ƣ�Negative Voltage Gamma Control��
    LCD_Write_Cmd(0xD6);   
    LCD_Write_Data8(0xA1);

    // 17. ����GammaУ׼������Ӱ��ɫ�ʱ��ֺͻҽ���ʾ��
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

    // 18. ��ʾ���ࣨDisplay Inversion On��������ɫ����������ֹ��Ӱ
    LCD_Write_Cmd(0x21);

    // 19. ������ʾ��Display On��
    LCD_Write_Cmd(0x29);
}
