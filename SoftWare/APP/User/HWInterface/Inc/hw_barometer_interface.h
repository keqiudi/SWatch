#ifndef __HW_BAROMETER_INTERFACE_H
#define __HW_BAROMETER_INTERFACE_H

#include "device.h"

typedef struct{
 
	device_status_t state;
	int16_t altitude;
	int (*init)(void);
}hw_barometer_interface_t;



#endif 