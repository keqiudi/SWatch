#ifndef __HW_MPU6050_INTERFACE_H
#define __HW_MPU6050_INTERFACE_H

#include "device.h"


typedef struct{
 
	device_status_t state;

	uint8_t (*init)(void);

}hw_mpu6050_interface_t;


#endif