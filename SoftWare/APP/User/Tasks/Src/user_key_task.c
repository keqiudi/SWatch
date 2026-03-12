
#include "user_task_init.h"
#include "user_key_task.h"
#include "bsp_key.h"
#include "SEGGER_RTT.h"


void KeyTask(void *argument)
{
    while(1)
    {
        switch(key_scan(0))
        {
            case 1: // key1按键事件
                SEGGER_RTT_printf(0,"Key1 Pressed\n");
      
                break;
            case 2: // 这里得有电池才会触发key2按键事件，实际为wake引脚被电源拉高的事件
                SEGGER_RTT_printf(0,"Key2 Pressed\n");
        
                break;
            default:
                break;
        }

        osDelay(pdMS_TO_TICKS(1)); 
    }
}