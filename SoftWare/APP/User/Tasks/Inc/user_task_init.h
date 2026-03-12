#ifndef	__USER_TASK_INIT_H
#define __USER_TASK_INIT_H


#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"




void user_tasks_init();


extern osMessageQueueId_t SensorMsgQueue;

#endif