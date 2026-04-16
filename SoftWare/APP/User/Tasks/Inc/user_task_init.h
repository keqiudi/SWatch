#ifndef	__USER_TASK_INIT_H
#define __USER_TASK_INIT_H


#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"


extern osMessageQueueId_t HomeUpdataMsgQueue;
extern osMessageQueueId_t IdleModeMsgQueue;
extern osMessageQueueId_t IdleModeBreakMsgQueue;
extern osMessageQueueId_t StopModeMsgQueue;
extern osMessageQueueId_t KeyMsgQueue;
extern osMessageQueueId_t DataSaveMsgQueue; 

void user_tasks_init();

#endif