#ifndef	__USER_TASK_INIT_H
#define __USER_TASK_INIT_H


#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"


extern osMessageQueueId_t HomeUpdataMsgQueue;

void user_tasks_init();

#endif