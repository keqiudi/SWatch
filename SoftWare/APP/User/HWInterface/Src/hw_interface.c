

#include "hw_interface.h"


hw_interface_t hw_interface = {
	
	.hw_aht20_interface = &hw_aht20_interface,
	.hw_mpu6050_interface = &hw_mpu6050_interface,
	.hw_barometer_interface = &hw_spl06_interface,
	.hw_ecompass_interface = &hw_lsm303dlhc_interface,
	.hw_hrsensor_interface = &hw_em7028_interface,
	.hw_ble_interface = &hw_kt6368_interface,
};