#ifndef __AHT20_H
#define __AHT20_H

#include "stm32f4xx.h"                  // Device header


void AHT20_soft_reset(void);
uint8_t AHT20_init(void);
uint8_t AHT20_read(float* temperature,float* humidity);


#endif 

