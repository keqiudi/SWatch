#ifndef __HW_AHT20_INTERFACE_H
#define __HW_AHT20_INTERFACE_H

#include "device.h"


typedef struct{
 
	device_status_t state;
	float temperature;
	float humidity;
	uint8_t (*init)(void);
	uint8_t (*read)(float* temperature,float* humidity);
	
}hw_aht20_interface_t;

extern hw_aht20_interface_t hw_aht20_interface;

#endif