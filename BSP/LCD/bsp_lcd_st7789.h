#ifndef __BSP_LCD_ST7789_H
#define __BSP_LCD_ST7789_H

#include "sys.h"
#include "spi.h"
#include "delay.h"
#include "tim.h"

#define USE_HORIZONTAL 0

#define LCD_SCLK_PORT   GPIOB
#define LCD_SCLK_PIN    GPIO_PIN_3

#define LCD_MOSI_PORT   GPIOB
#define LCD_MOSI_PIN    GPIO_PIN_5

#define LCD_CS_PORT     GPIOB
#define LCD_CS_PIN      GPIO_PIN_8

#define LCD_RST_PORT    GPIOB
#define LCD_RST_PIN     GPIO_PIN_7

#define LCD_DC_PORT  	  GPIOB
#define LCD_DC_PIN 			GPIO_PIN_9

#define LCD_BLK_PORT    GPIOB
#define LCD_BLK_PIN     GPIO_PIN_0

#define LCD_SCLK_LOW()      HAL_GPIO_WritePin(LCD_SCLK_PORT, LCD_SCLK_PIN, GPIO_PIN_RESET)
#define LCD_SCLK_HIGH()     HAL_GPIO_WritePin(LCD_SCLK_PORT, LCD_SCLK_PIN, GPIO_PIN_SET)

#define LCD_MOSI_LOW()      HAL_GPIO_WritePin(LCD_MOSI_PORT, LCD_MOSI_PIN, GPIO_PIN_RESET)
#define LCD_MOSI_HIGH()     HAL_GPIO_WritePin(LCD_MOSI_PORT, LCD_MOSI_PIN, GPIO_PIN_SET)

#define LCD_RST_LOW()     	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET)
#define LCD_RST_HIGH()    	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET)

#define LCD_DC_LOW()        HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_RESET)
#define LCD_DC_HIGH()       HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET)

#define LCD_CS_LOW()        HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define LCD_CS_HIGH()       HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET)

#define LCD_BLK_LOW()        HAL_GPIO_WritePin(LCD_BLK_PORT, LCD_BLK_PIN, GPIO_PIN_RESET)
#define LCD_BLK_HIGH()       HAL_GPIO_WritePin(LCD_BLK_PORT, LCD_BLK_PIN, GPIO_PIN_SET)

void LCD_Init(void);

void LCD_SPI_SetBit(u8 bit);
	
void LCD_Write_Cmd(u8 data);

void LCD_Write_Data8(u8 data);

void LCD_Write_Data16(u16 data);

void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);


#endif