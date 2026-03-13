

#include "user_task_init.h"
#include "user_key_task.h"
#include "user_hw_init.h"
#include "user_lvgl_handler.h"
#include "user_sensor_task.h"
#include "SEGGER_RTT.h"
#include "FreeRTOS.h"
#include "task.h"


// 定义任务句柄和属性
osThreadId_t HwInitTaskHandle;   
const osThreadAttr_t HwInitTask_attributes = {
  .name = "HwInitTask",
  .stack_size = 1024 * 2,
  .priority = (osPriority_t) osPriorityHigh,
};


osThreadId_t LvglHandlerTaskHandle;
const osThreadAttr_t LvglHandlerTask_attributes = {
  .name = "LvglHandlerTaskHandle",
  .stack_size = 1024 * 4, // 官方处理任务栈空间至少2K，推荐>8KB。 我的项目中3K不够
  .priority = (osPriority_t) osPriorityLow1,
};


osThreadId_t SensorDataUpdateTaskHandle;
const osThreadAttr_t SensorDataUpdateTask_attributes = {
  .name = "SensorDataUpdateTask",
  .stack_size = 128 * 10,
  .priority = (osPriority_t) osPriorityLow,
};


osThreadId_t HRDataTaskHandle;
const osThreadAttr_t HRDataTask_attributes = {
  .name = "HRDataTask",
  .stack_size = 128 * 5,
  .priority = (osPriority_t) osPriorityLow1,
};

osThreadId_t KeyTaskHandle;
const osThreadAttr_t KeyTask_attributes = {
  .name = "KeyScanTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t WristWakeCheckTaskHandle;
const osThreadAttr_t WristWakeCheckTask_attributes = {
  .name = "WristWakeCheckTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow2,
};


/* 定义消息队列句柄 */
osMessageQueueId_t HomeUpdataMsgQueue; // 用于HOME页面更新数据

void user_tasks_init()
{
	
  HomeUpdataMsgQueue  = osMessageQueueNew(1, 1, NULL);
	
	HwInitTaskHandle      = osThreadNew(HwInitTask, NULL, &HwInitTask_attributes); // 硬件初始化任务，优先级较高，确保在其他任务之前完成硬件初始化
	//  if(HwInitTaskHandle == NULL)
	//  	SEGGER_RTT_printf(0,"HwInitTask Create Failed");
	  
	LvglHandlerTaskHandle   = osThreadNew(LvglHandlerTask, NULL, &LvglHandlerTask_attributes); // lvgl处理任务，优先级较低，确保在硬件初始化完成后再运行
	// if(LvglHandlerTaskHandle == NULL)
  // 	SEGGER_RTT_printf(0,"LvglHandlerTask Create Failed");
	
	SensorDataUpdateTaskHandle			= osThreadNew(SensorDataUpdateTask,NULL,&SensorDataUpdateTask_attributes); // 传感器数据更新任务，优先级较低，确保在硬件初始化完成后再运行
	//  if(SensorDataUpdateTaskHandle == NULL)
	//  	SEGGER_RTT_printf(0,"SensorDataUpdateTask Create Failed");
	
   HRDataTaskHandle      = osThreadNew(HRDataTask, NULL, &HRDataTask_attributes); // 心率数据处理任务，优先级较低，确保在硬件初始化完成后再运行
    //  if(HRDataTaskHandle == NULL)
    //      SEGGER_RTT_printf(0,"HRDataTask Create Failed");

   KeyTaskHandle = osThreadNew(KeyTask, NULL, &KeyTask_attributes); // 按键扫描任务
    // if(KeyTaskHandle == NULL)
    //     SEGGER_RTT_printf(0,"KeyTask Create Failed");

   WristWakeCheckTaskHandle =  osThreadNew(WristWakeCheckTask, NULL, &WristWakeCheckTask_attributes); // 手腕唤醒检测任务
    // if(WristWakeCheckTaskHandle == NULL)
    //     SEGGER_RTT_printf(0,"WristWakeCheckTask Create Failed");




   // 打印剩余FreeRTOS堆内存大小，调试用
   SEGGER_RTT_printf(0, "Free heap: %uByte\n", xPortGetFreeHeapSize());
}	




/* FreeRTOS堆栈溢出钩子，堆栈溢出检查会增加上下文开销，建议只在开发阶段使用 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    SEGGER_RTT_printf(0, RTT_CTRL_TEXT_BRIGHT_RED"Stack overflow in task: %s\r\n", pcTaskName);
    
    while(1)
		{
			
		}
}







