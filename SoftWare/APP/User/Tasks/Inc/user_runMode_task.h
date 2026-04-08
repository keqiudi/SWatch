#ifndef USER_RUNMODE_TASK_H
#define USER_RUNMODE_TASK_H


#include "user_task_init.h"

void IdleEnterTask(void *argument);
void StopEnterTask(void *argument);
void IdleTimerCallback(void *argument);

extern uint16_t IdleTimerCounter; 

#endif /* USER_RUNMODE_TASK_H */