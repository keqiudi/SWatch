#ifndef BSP_POWER_H
#define BSP_POWER_H

#include "stm32f4xx_hal.h"

#define BAT_CHECK_PORT	    GPIOA
#define BAT_CHECK_PIN		GPIO_PIN_1

#define CHARGE_PORT			GPIOA
#define CHARGE_PIN			GPIO_PIN_2

#define POWER_PORT			GPIOA
#define POWER_PIN			GPIO_PIN_3

void power_init(void);
void power_enable(void);
void power_disEnable(void);
uint8_t power_calculate(void);

float bat_check(void);
float bat_check_8times(void);
uint8_t charge_check(void);




#endif // BSP_POWER_H