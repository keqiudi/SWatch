

#include "hw_interface.h"


extern hw_aht20_interface_t hw_aht20_interface;
extern hw_mpu6050_interface_t hw_mpu6050_interface;
extern hw_barometer_interface_t hw_spl06_interface;
extern hw_ecompass_interface_t hw_lsm303dlhc_interface;

hw_interface_t hw_interface = {
	
	.hw_aht20_interface = &hw_aht20_interface,
	.hw_mpu6050_interface = &hw_mpu6050_interface,
	.hw_barometer_interface = &hw_spl06_interface,
	.hw_ecompass_interface = &hw_lsm303dlhc_interface,
};