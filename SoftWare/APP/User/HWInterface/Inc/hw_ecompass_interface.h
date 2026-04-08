#ifndef __HW_ECOMPASS_INTERFACE_H
#define __HW_ECOMPASS_INTERFACE_H

#include "device.h"

typedef struct{
 
	device_status_t state;
	uint16_t direction;
	uint8_t (*init)(void);
  	void (*sleep)(void);
	void (*wakeup)(void);
}hw_ecompass_interface_t;

extern hw_ecompass_interface_t hw_lsm303dlhc_interface;

#endif // __HW_ECOMPASS_INTERFACE_H