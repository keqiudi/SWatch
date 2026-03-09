
#include "hw_mpu6050_interface.h"
#include "bsp_mpu6050.h"
#include "SEGGER_RTT.h"

int hw_mpu6050_init()
{
	int ret = mpu6050_init();
	if(ret == 1)
	{
		//SEGGER_RTT_printf(0,"mpu6050 init fail!");
		return ERR_FAILED;
	}
	
	return ERR_SUCCESS;
}


hw_mpu6050_interface_t hw_mpu6050_interface = {
		.state = DEVICE_STATUS_UNINIT,
		.init = hw_mpu6050_init,
};