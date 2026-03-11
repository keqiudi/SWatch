#ifndef __USER_SENSOR_TASK_H
#define __USER_SENSOR_TASK_H


#include "user_task_init.h"

extern uint32_t user_HR_timecount;

void SensorDataUpdateTask(void *argument);

void HRDataTask(void *argument);


#endif