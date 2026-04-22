

#include "user_task_init.h"
#include "user_key_task.h"
#include "user_wdog_task.h"
#include "user_chargeCheck_task.h"
#include "user_hw_init.h"
#include "user_lvgl_handler.h"
#include "user_sensor_task.h"
#include "user_runMode_task.h"
#include "user_dataSave_task.h"
#include "user_messageSend_task.h"
#include "SEGGER_RTT.h"
#include "FreeRTOS.h"
#include "task.h"


/* 定义任务句柄和属性 */
osThreadId_t HwInitTaskHandle;   
const osThreadAttr_t HwInitTask_attributes = {
  .name = "HwInitTask",
  .stack_size = 128 * 10,
  .priority = (osPriority_t) osPriorityHigh3, //这里优先级必须给所有任务最高, 之前我优先级小于看门狗任务，导致关机后无法正常开机
};


osThreadId_t LvglHandlerTaskHandle;
const osThreadAttr_t LvglHandlerTask_attributes = {
  .name = "LvglHandlerTaskHandle",
  .stack_size = 128 * 32, // 官方处理任务栈空间至少2K，推荐>8KB。 这里给128*30是我测试后极限的结果
  .priority = (osPriority_t) osPriorityLow,
};


osThreadId_t SensorDataUpdateTaskHandle;
const osThreadAttr_t SensorDataUpdateTask_attributes = {
  .name = "SensorDataUpdateTask",
  .stack_size = 128 * 5,
  .priority = (osPriority_t) osPriorityLow,
};


osThreadId_t HRDataTaskHandle;
const osThreadAttr_t HRDataTask_attributes = {
  .name = "HRDataTask",
  .stack_size = 128 * 5,
  .priority = (osPriority_t) osPriorityLow1,
};

osThreadId_t WristWakeCheckTaskHandle;
const osThreadAttr_t WristWakeCheckTask_attributes = {
  .name = "WristWakeCheckTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow2,
};

//WDOG Feed task
osThreadId_t WDOGFeedTaskHandle;
const osThreadAttr_t WDOGFeedTask_attributes = {
  .name = "WDOGFeedTask",
  .stack_size = 128 * 2,
  .priority = (osPriority_t) osPriorityHigh2,
};   

//Charge Check task
osThreadId_t ChargeCheckTaskHandle;
const osThreadAttr_t ChargeCheckTask_attributes = {
  .name = "ChargeCheckTask",
  .stack_size = 128 * 10,    // 给少了会任务会溢出
  .priority = (osPriority_t) osPriorityLow1,
};

osThreadId_t KeyTaskHandle;
const osThreadAttr_t KeyTask_attributes = {
  .name = "KeyScanTask",
  .stack_size = 128 * 2,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t KeyEventTaskHandle;
const osThreadAttr_t KeyEventTask_attributes = {
  .name = "KeyEventTask",
  .stack_size = 128 * 8,
  .priority = (osPriority_t) osPriorityLow1,
};


// Idle模式判断任务
osThreadId_t IdleEnterTaskHandle;
const osThreadAttr_t IdleEnterTask_attributes = {
  .name = "IdleEnterTask",
  .stack_size = 128 * 2,
  .priority = (osPriority_t) osPriorityHigh,
};

// Stop模式判断任务
osThreadId_t StopEnterTaskHandle;
const osThreadAttr_t StopEnterTask_attributes = {
  .name = "StopEnterTask",
  .stack_size = 128 * 8,
  .priority = (osPriority_t) osPriorityHigh1,
};

osThreadId_t DataSaveTaskHandle;
const osThreadAttr_t DataSaveTask_attributes = {
  .name = "DataSaveTask",
  .stack_size = 128 * 3,
  .priority = (osPriority_t) osPriorityLow2,
};

// 
osThreadId_t MessageSendTaskHandle;
const osThreadAttr_t MessageSendTask_attributes = {
  .name = "MessageSendTask",
  .stack_size = 128 * 5,
  .priority = (osPriority_t) osPriorityLow1,
};

/* 定时器定义 */
osTimerId_t IdleTimerHandle;

/* 定义消息队列句柄 */  
osMessageQueueId_t KeyMsgQueue; // 用于按键事件通知
osMessageQueueId_t IdleModeMsgQueue; // 用于Idle模式判断
osMessageQueueId_t IdleModeBreakMsgQueue; // 用于刷新Idle判断
osMessageQueueId_t StopModeMsgQueue; // 用于Stop模式判断
osMessageQueueId_t HomeUpdataMsgQueue; // 用于HOME页面更新数据
osMessageQueueId_t DataSaveMsgQueue; // 用于数据保存通知

void user_tasks_init()
{
	
  IdleTimerHandle = osTimerNew(IdleTimerCallback, osTimerPeriodic, NULL, NULL); // 进入Idle模式定时器，周期性检测是否满足进入Idle模式的条件
  osTimerStart(IdleTimerHandle, pdMS_TO_TICKS(100)); // 启动定时器，定时器周期设置为100ms

  KeyMsgQueue = osMessageQueueNew(1, 1, NULL);
  IdleModeMsgQueue = osMessageQueueNew(1, 1, NULL);
  IdleModeBreakMsgQueue = osMessageQueueNew(1, 1, NULL);
  StopModeMsgQueue = osMessageQueueNew(1, 1, NULL);
  HomeUpdataMsgQueue  = osMessageQueueNew(1, 1, NULL);
  DataSaveMsgQueue = osMessageQueueNew(2, 1, NULL);
	

 /* 任务初始化 */

	HwInitTaskHandle      = osThreadNew(HwInitTask, NULL, &HwInitTask_attributes); // 硬件初始化任务，优先级较高，确保在其他任务之前完成硬件初始化
	LvglHandlerTaskHandle   = osThreadNew(LvglHandlerTask, NULL, &LvglHandlerTask_attributes); // lvgl处理任务，优先级较低，确保在硬件初始化完成后再运行
	SensorDataUpdateTaskHandle			= osThreadNew(SensorDataUpdateTask,NULL,&SensorDataUpdateTask_attributes); // 传感器数据更新任务，优先级较低，确保在硬件初始化完成后再运行
  HRDataTaskHandle      = osThreadNew(HRDataTask, NULL, &HRDataTask_attributes); // 心率数据处理任务，优先级较低，确保在硬件初始化完成后再运行
  WristWakeCheckTaskHandle =  osThreadNew(WristWakeCheckTask, NULL, &WristWakeCheckTask_attributes); // 手腕唤醒检测任务
	WDOGFeedTaskHandle = osThreadNew(WDOGFeedTask, NULL, &WDOGFeedTask_attributes); // 看门狗喂狗任务
  ChargeCheckTaskHandle = osThreadNew(ChargeCheckTask, NULL, &ChargeCheckTask_attributes); // 充电检测任务
  KeyTaskHandle = osThreadNew(KeyTask, NULL, &KeyTask_attributes); // 按键扫描任务
  KeyEventTaskHandle = osThreadNew(KeyEventTask, NULL, &KeyEventTask_attributes); // 按键事件处理任务
  IdleEnterTaskHandle = osThreadNew(IdleEnterTask, NULL, &IdleEnterTask_attributes); // 进入Idle模式空闲任务
  StopEnterTaskHandle = osThreadNew(StopEnterTask, NULL, &StopEnterTask_attributes); // 进入Stop模式任务
  DataSaveTaskHandle = osThreadNew(DataSaveTask, NULL, &DataSaveTask_attributes); // 数据保存任务
  MessageSendTaskHandle = osThreadNew(MessageSendTask, NULL, &MessageSendTask_attributes); // 消息发送任务
  
	uint8_t msg_update_home = 0;
  osMessageQueuePut(HomeUpdataMsgQueue, &msg_update_home, 0, 0); // 启动时先更新一次HOME页面数据
}	


/* FreeRTOS堆栈溢出钩子，堆栈溢出检查会增加上下文开销，建议只在开发阶段使用 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    SEGGER_RTT_printf(0, RTT_CTRL_TEXT_BRIGHT_RED"Stack overflow in task: %s\r\n", pcTaskName);
    
    while(1)
		{
			
		}
}







