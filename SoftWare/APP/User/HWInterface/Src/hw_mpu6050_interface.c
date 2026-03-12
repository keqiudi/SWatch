
#include "hw_mpu6050_interface.h"
#include "bsp_mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "SEGGER_RTT.h"

static uint8_t hw_mpu6050_init()
{
	// int ret = mpu6050_init();
	int ret = mpu_dmp_init(); // 使用DMP硬件解算
	if(ret == 1)
	{
		//SEGGER_RTT_printf(0,"mpu6050 init fail!");
		return ERR_FAILED;
	}
	
	return ERR_SUCCESS;
}

static void hw_mpu6050_wrist_enable()
{
	hw_mpu6050_interface.wrist_is_enabled = 1;
}

static void hw_mpu6050_wrist_disable()
{
	hw_mpu6050_interface.wrist_is_enabled = 0;
}

static uint16_t hw_mpu6050_get_steps()
{
	unsigned long STEPS = 0;
	if(hw_mpu6050_interface.state == DEVICE_STATUS_INITED )
	{
		dmp_get_pedometer_step_count(&STEPS);
		return (uint16_t)STEPS;
	}
}

static int hw_mpu6050_set_steps(unsigned long count)
{
	if(hw_mpu6050_interface.state == DEVICE_STATUS_INITED)
	{
		return dmp_set_pedometer_step_count(count);
	}
	return -1; // 设置失败
}

hw_mpu6050_interface_t hw_mpu6050_interface = {
	.state = DEVICE_STATUS_UNINIT,
	.steps = 0,
	.wrist_state = WRIST_DOWN,
	.wrist_is_enabled = 0,
	.init = hw_mpu6050_init,
	.wrist_enable = hw_mpu6050_wrist_enable,
	.wrist_disable = hw_mpu6050_wrist_disable, 
	.get_steps = hw_mpu6050_get_steps, 
	.set_steps = hw_mpu6050_set_steps, 
};