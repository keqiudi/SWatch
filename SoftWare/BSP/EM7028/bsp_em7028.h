#ifndef __BSP_EM7028_H
#define __BSP_EM7028_H

#include "i2c_hal.h"


#define EM7028_SDA_PORT GPIOB
#define EM7028_SDA_PIN  GPIO_PIN_13
#define EM7028_SCL_PORT GPIOB
#define EM7028_SCL_PIN  GPIO_PIN_14

#define EM7028_ID					0x36
#define EM7028_ADDR				    0x24

#define ID_REG						0x00
#define HRS_CFG						0x01
#define HRS_INT_CTRL			0x02
#define HRS_LT_L					0X03
#define HRS_LT_H					0x04
#define HRS_HT_L					0x05
#define HRS_HT_H					0x06
#define LED_CRT						0x07
#define	HRS2_DATA_OFFSET	0x08
#define HRS2_CTRL					0x09
#define HRS2_GAIN_CTRL		0x0A
#define HRS1_CTRL					0x0D
#define INT_CTRL					0x0E
#define SOFT_RESET				0x0F

#define HRS2_DATA0_L		0x20
#define HRS2_DATA0_H		0x21
#define HRS2_DATA1_L		0x22
#define HRS2_DATA1_H		0x23
#define HRS2_DATA2_L		0x24
#define HRS2_DATA2_H		0x25
#define HRS2_DATA3_L		0x26
#define HRS2_DATA3_H		0x27

#define HRS1_DATA0_L		0x28
#define HRS1_DATA0_H		0x29
#define HRS1_DATA1_L		0x2A
#define HRS1_DATA1_H		0x2B
#define HRS1_DATA2_L		0x2C
#define HRS1_DATA2_H		0x2D
#define HRS1_DATA3_L		0x2E
#define HRS1_DATA3_H		0x2F

uint8_t em7028_read_reg(uint8_t reg_addr);
uint8_t em7028_write_reg(uint8_t reg_addr,uint8_t data);

uint8_t em7028_get_id(void);
uint8_t em7028_hrs_init(void);
uint8_t em7028_hrs_enable(void);
uint8_t em7028_hrs_disEnable(void);
uint16_t em7028_get_hrs1(void);




#endif