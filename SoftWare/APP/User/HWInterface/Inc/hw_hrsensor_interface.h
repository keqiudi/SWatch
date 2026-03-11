#ifndef HW_HRSENSOR_INTERFACE_H
#define HW_HRSENSOR_INTERFACE_H

#include "device.h"

typedef struct{
 
	device_status_t state;
	uint8_t hr_rate;
    uint8_t spo2;
	uint8_t (*init)(void);
    uint8_t (*diable)(void);
    
}hw_hrsensor_interface_t;

#endif // HW_HRSENSOR_INTERFACE_