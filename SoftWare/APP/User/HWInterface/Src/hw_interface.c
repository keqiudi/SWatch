

#include "hw_interface.h"


extern hw_aht20_interface_t hw_aht20_interface;
extern hw_mpu6050_interface_t hw_mpu6050_interface;


hw_interface_t hw_interface = {
	
	.hw_aht20_interface = &hw_aht20_interface,
	.hw_mpu6050_interface = &hw_mpu6050_interface,
};