

#include "user_task_init.h"
#include "user_hw_init.h"
#include "user_lvgl_handler.h"


//osThreadId_t HwInitTaskHandle;
//const osThreadAttr_t HwInitTask_attributes = {
//  .name = "HwInitTask",
//  .stack_size = 128 * 4,
//  .priority = (osPriority_t)  osPriorityHigh,
//};


osThreadId_t LvglHandlerTaskHandle;
const osThreadAttr_t LvglHandlerTask_attributes = {
  .name = "LvglHandlerTaskHandle",
  .stack_size = 128 * 24, // lvgl官方推荐栈空间至少2K
  .priority = (osPriority_t) osPriorityNormal,
};



void User_Tasks_Init()
{
	
//	HwInitTaskHandle      = osThreadNew(HwInitTask, NULL, &HwInitTask_attributes); // 硬件初始化
	LvglHandlerTaskHandle = osThreadNew(LvglHandlerTask, NULL, &LvglHandlerTask_attributes);

}	











