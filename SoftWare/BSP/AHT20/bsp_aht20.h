#ifndef __BSP_AHT20_H
#define __BSP_AHT20_H

#include "stm32f4xx.h"                  // Device header





#define AHT20_I2C_ADDRESS 0x38
#define AHT20_CMD_READ 0x71
#define AHT20_CMD_WRITE 0x70
#define AHT20_CMD_INIT 0xBE
#define AHT20_CMD_MEASURE 0xAC
#define AHT20_CMD_SOFT_RESET 0xBA


#define AHT20_SDA_PORT GPIOB
#define AHT20_SDA_PIN  GPIO_PIN_13

#define AHT20_SCL_PORT GPIOB
#define AHT20_SCL_PIN  GPIO_PIN_14


void AHT20_soft_reset(void);

uint8_t AHT20_init(void);
uint8_t AHT20_read(float* temperature,float* humidity);


#endif 

