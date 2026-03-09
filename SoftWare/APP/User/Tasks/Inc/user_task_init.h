#ifndef	__USER_TASK_INIT_H
#define __USER_TASK_INIT_H


#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"


typedef enum {
    MSG_AHT20_MEASURE=0,
    MSG_MPU6050_MEASURE,
    MSG_EM7028_MEASURE,
    MSG_COMPASS_MEASURE,
}sensor_msg_t;


void user_tasks_init();


extern osMessageQueueId_t SensorMsgQueue;

#endif