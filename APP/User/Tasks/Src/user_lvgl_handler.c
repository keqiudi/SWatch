
#include "user_task_init.h"
#include "user_lvgl_handler.h"

void LvglHandlerTask(void *argument)
{
	
	while(1)
	{
		 lv_task_handler(); // 启动lvgl的事务处理
		 osDelay(5);
	}
	
}


void lvgl_tick_provide(void)
{
		lv_tick_inc(1);
}

/* FreeRTOS Tick钩子*/
void vApplicationTickHook( void )
{
   /* This function will be called by each tick interrupt if
   configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
   added here, but the tick hook is called from an interrupt context, so
   code must not attempt to block, and only the interrupt safe FreeRTOS API
   functions can be used (those that end in FromISR()). */
	
	lvgl_tick_provide();
}
