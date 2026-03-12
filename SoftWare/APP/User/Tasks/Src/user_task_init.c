

#include "user_task_init.h"
#include "user_key_task.h"
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
  .stack_size = 1024 * 4, // �ٷ���������ջ�ռ�����2K���Ƽ�>8KB�� ����������3K����
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

osMessageQueueId_t SensorMsgQueue;


void user_tasks_init()
{
	
	SensorMsgQueue = osMessageQueueNew(1, 1, NULL);
	
	 HwInitTaskHandle      = osThreadNew(HwInitTask, NULL, &HwInitTask_attributes); // Ӳ����ʼ������
	 if(HwInitTaskHandle == NULL)
	 	SEGGER_RTT_printf(0,"HwInitTask Create Failed");
	  
	LvglHandlerTaskHandle = osThreadNew(LvglHandlerTask, NULL, &LvglHandlerTask_attributes); // lvgl����������
	 if(LvglHandlerTaskHandle == NULL)
	 	SEGGER_RTT_printf(0,"LvglHandlerTask Create Failed");
	
	SensorDataUpdateTaskHandle			= osThreadNew(SensorDataUpdateTask,NULL,&SensorDataUpdateTask_attributes); //���������ݸ�������
	 if(SensorDataUpdateTaskHandle == NULL)
	 	SEGGER_RTT_printf(0,"SensorDataUpdateTask Create Failed");
	
   HRDataTaskHandle      = osThreadNew(HRDataTask, NULL, &HRDataTask_attributes); // �������ݴ�������
     if(HRDataTaskHandle == NULL)
         SEGGER_RTT_printf(0,"HRDataTask Create Failed");

   KeyTaskHandle = osThreadNew(KeyTask, NULL, &KeyTask_attributes); // 按键扫描任务

   // ��ӡʣ�� heap
   SEGGER_RTT_printf(0, "Free heap: %uByte\n", xPortGetFreeHeapSize());
}	




/* FreeRTOS��ջ������ӣ���ջ������������������л��Ŀ�������˽���ֻ�ڿ�������Խ׶�ʹ�ô˼�� */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    SEGGER_RTT_printf(0, RTT_CTRL_TEXT_BRIGHT_RED"Stack overflow in task: %s\r\n", pcTaskName);
    // �����縴λ�����ơ���ѭ����
    while(1)
		{
			
		}
}







