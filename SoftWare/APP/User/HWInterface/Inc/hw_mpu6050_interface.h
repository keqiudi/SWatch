#ifndef __HW_MPU6050_INTERFACE_H
#define __HW_MPU6050_INTERFACE_H

#include "device.h"

// 手腕状态定义
#define WRIST_IP 1
#define WRIST_DOWN 0

typedef struct{
 
	device_status_t state;
	uint16_t steps;
	uint8_t wrist_state;
    uint8_t wrist_is_enabled;

	uint8_t (*init)(void);
	void (*wrist_enable)(void);
    void (*wrist_disable)(void);
    uint16_t (*get_steps)(void);
    int (*set_steps)(unsigned long count);
}hw_mpu6050_interface_t;

extern hw_mpu6050_interface_t hw_mpu6050_interface;

#endif