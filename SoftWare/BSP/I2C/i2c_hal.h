#ifndef __I2C_HAL_H
#define __I2C_HAL_H

#include "stm32f4xx.h"                  // Device header

typedef struct{
	GPIO_TypeDef* I2C_SDA_PORT;
	GPIO_TypeDef* I2C_SCL_PORT;
	uint32_t I2C_SDA_PIN;
	uint32_t I2C_SCL_PIN;
}i2c_bus_t;


void i2c_init(i2c_bus_t* sensor);


void i2c_soft_start(i2c_bus_t* sensor);
void i2c_soft_stop(i2c_bus_t* sensor);

void i2c_soft_send_byte(i2c_bus_t* sensor,uint8_t data);
uint8_t i2c_soft_read_byte(i2c_bus_t* sensor);

void i2c_soft_send_ack(i2c_bus_t* sensor);
void i2c_soft_send_not_ack(i2c_bus_t* sensor);
uint8_t i2c_soft_wait_ack(i2c_bus_t* sensor);

#endif
