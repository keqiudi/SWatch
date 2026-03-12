#ifndef __KEY_H
#define __KEY_H

#include "main.h"

/* 按键电路按键*/
#define KEY1_PORT GPIOA
#define KEY1_PIN  GPIO_PIN_5
#define KEY1 HAL_GPIO_ReadPin(KEY1_PORT,KEY1_PIN)

/* 供电按键，实际为原理图wake引脚*/
#define KEY2_PORT GPIOA
#define KEY2_PIN  GPIO_PIN_4
#define KEY2 HAL_GPIO_ReadPin(KEY2_PORT,KEY2_PIN) 

void key_gpio_init();
void key_interrupt_callback();
uint8_t key_scan(uint8_t mode);


#endif