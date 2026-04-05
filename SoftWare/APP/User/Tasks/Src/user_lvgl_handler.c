
#include "user_task_init.h"
#include "user_sensor_task.h"
#include "user_lvgl_handler.h"
#include "SEGGER_RTT.h"
#include "lvgl.h"
#include "ui_TimerPage.h"


void lvgl_log_cb(lv_log_level_t level, const char * buf) {
    const char * level_str = "";
    switch(level)
		{
        case LV_LOG_LEVEL_ERROR: level_str = "[ERROR]"; break;
        case LV_LOG_LEVEL_WARN:  level_str = "[WARN ]"; break;
        case LV_LOG_LEVEL_INFO:  level_str = "[INFO ]"; break;
        case LV_LOG_LEVEL_TRACE: level_str = "[TRACE]"; break;
        case LV_LOG_LEVEL_USER:  level_str = "[USER ]"; break;
        default: level_str = "[UNKWN]"; break;
    }
    SEGGER_RTT_printf(0, "%s %s\n", level_str, buf);
	  
}

void lv_mem_monitor_task(lv_timer_t * timer)
{
	lv_mem_monitor_t mon;
  lv_mem_monitor(&mon); 

  SEGGER_RTT_printf(0,
        "LVGL MEM: total=%lu Bytes, free=%lu Bytes, used=%u%%, max_used=%lu Bytes\n",
        (unsigned long)mon.total_size,
        (unsigned long)mon.free_size,
        mon.used_pct,
        (unsigned long)mon.max_used
    );
}


void LvglHandlerTask(void *argument)
{
	
	lv_log_register_print_cb(lvgl_log_cb);  
  lv_timer_create(lv_mem_monitor_task, 1000, NULL); // 输出lvgl内存池占用情况，便于调试

	while(1)
	{
		 lv_task_handler(); // lvgl任务处理
		 osDelay(pdMS_TO_TICKS(1));
	}
	
}


static void lvgl_tick_provide(void)
{
		lv_tick_inc(1);
}

/* FreeRTOS Tick钩子函数，用于提供lvgl时基*/
void vApplicationTickHook( void )
{
   /* This function will be called by each tick interrupt if
   configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
   added here, but the tick hook is called from an interrupt context, so
   code must not attempt to block, and only the interrupt safe FreeRTOS API
   functions can be used (those that end in FromISR()). */
	
	lvgl_tick_provide();

  user_HR_timecount += 1; // 心率算法时间计数


  /* 定时器页面计数 */
  if(timer_running_flag)
  {
     timer_ms += 1;
     if(timer_ms >= 10)
     {
          timer_ms = 0;
          timer_10ms += 1;
     }

     if(timer_10ms >= 100)
     {
          timer_10ms = 0;
          timer_sec += 1;

     }
     if(timer_sec >= 60)
     {
          timer_sec = 0;
          timer_min += 1;
     }
     if(timer_min >= 60)
     {
          timer_min = 0;
     }
  }

}
