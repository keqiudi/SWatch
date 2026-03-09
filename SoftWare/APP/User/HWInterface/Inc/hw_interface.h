#ifndef __HW_INTERFACE_H
#define __HW_INTERFACE_H


#include "stdint.h"
#include "hw_aht20_interface.h"
#include "hw_mpu6050_interface.h"
#include "hw_barometer_interface.h"
#include "hw_ecompass_interface.h"
#include "device.h"


typedef struct{
	
	hw_aht20_interface_t* hw_aht20_interface;
	hw_mpu6050_interface_t* hw_mpu6050_interface;
	hw_barometer_interface_t* hw_barometer_interface;
	hw_ecompass_interface_t* hw_ecompass_interface;
	
}hw_interface_t;


extern hw_interface_t hw_interface;


#endif