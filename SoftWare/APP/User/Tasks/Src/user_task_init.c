

#include "user_task_init.h"
#include "user_hw_init.h"
#include "user_lvgl_handler.h"
#include "user_sensor_task.h"
#include "SEGGER_RTT.h"
#include "FreeRTOS.h"
#include "task.h"

osThreadId_t HwInitTaskHandle;
const osThreadAttr_t HwInitTask_attributes = {
  .name = "HwInitTask",
  .stack_size = 1024 * 2,
  .priority = (osPriority_t) osPriorityHigh,
};


osThreadId_t LvglHandlerTaskHandle;
const osThreadAttr_t LvglHandlerTask_attributes = {
  .name = "LvglHandlerTaskHandle",
  .stack_size = 1024 * 4, // 官方处理任务栈空间至少2K，推荐>8KB。 这里3K会溢出
  .priority = (osPriority_t) osPriorityLow1,
};


osThreadId_t SensorDataUpdateTaskHandle;
const osThreadAttr_t SensorDataUpdateTask_attributes = {
  .name = "SensorDataUpdateTask",
  .stack_size = 128 * 10,
  .priority = (osPriority_t) osPriorityLow,
};



osMessageQueueId_t SensorMsgQueue;


void user_tasks_init()
{
	

	SensorMsgQueue = osMessageQueueNew(1, 1, NULL);
	
	HwInitTaskHandle      = osThreadNew(HwInitTask, NULL, &HwInitTask_attributes); // 硬件初始化任务
	if(HwInitTaskHandle == NULL)
		SEGGER_RTT_printf(0,"HwInitTask Create Failed");
	  
	LvglHandlerTaskHandle = osThreadNew(LvglHandlerTask, NULL, &LvglHandlerTask_attributes); // lvgl任务处理任务
	if(LvglHandlerTaskHandle == NULL)
		SEGGER_RTT_printf(0,"LvglHandlerTask Create Failed");
	
	SensorDataUpdateTaskHandle			= osThreadNew(SensorDataUpdateTask,NULL,&SensorDataUpdateTask_attributes); //传感器数据更新任务
	if(SensorDataUpdateTaskHandle == NULL)
		SEGGER_RTT_printf(0,"SensorDataUpdateTask Create Failed");
	
   // 打印剩余 heap
   SEGGER_RTT_printf(0, "Free heap: %uByte\n", xPortGetFreeHeapSize());
	
}	




/* FreeRTOS堆栈溢出钩子，堆栈溢出检查会增加上下文切换的开销，因此建议只在开发或测试阶段使用此检查 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    SEGGER_RTT_printf(0, RTT_CTRL_TEXT_BRIGHT_RED"Stack overflow in task: %s\r\n", pcTaskName);
    // 其他如复位、闪灯、死循环等
    while(1)
		{
			
		}
}







