
#include "user_task_init.h"
#include "user_lvgl_handler.h"
#include "SEGGER_RTT.h"
#include "lvgl.h"


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
    lv_mem_monitor(&mon); // 填充 mon 结构体

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
	
	lv_log_register_print_cb(lvgl_log_cb); // 注册日志输出回调
	lv_timer_create(lv_mem_monitor_task, 1000, NULL); // 注册定时器每秒输出内存占用 
	while(1)
	{
		 lv_task_handler(); // 启动lvgl的事务处理
		 osDelay(1);
	}
	
}


static void lvgl_tick_provide(void)
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
