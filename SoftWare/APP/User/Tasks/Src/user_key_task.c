
#include "user_task_init.h"
#include "user_key_task.h"
#include "bsp_key.h"

#include "page_manager.h"
#include "ui_MenuPage.h"

#include "hw_interface.h"

void KeyTask(void *argument)
{
    uint8_t msg_key = 0;
    uint8_t msg_idle_break = 0;
    uint8_t msg_stop = 0;
    while(1)
    {
        switch(key_scan(0))
        {

            case 1: // key1按键事件
                msg_key = 1;  
                osMessageQueuePut(KeyMsgQueue, &msg_key, 0, 1); // 通过消息队列通知HOME页面更新数据
                osMessageQueuePut(IdleModeBreakMsgQueue, &msg_idle_break, 0, 1); // 通过消息队列通知Idle模式判断任务打断Idle模式
                break;

            case 2: // 这里得有电池才会触发key2按键事件，实际为wake引脚被电源拉高的事件
                if(get_top_page()->page_obj == &ui_HomePage) 
                {   
                    //主页按key2进入睡眠模式
                    osMessageQueuePut(StopModeMsgQueue, &msg_stop, 0, 1); 
                }
                else // 如果当前不在菜单页面，直接退回主页
                {
                    msg_key = 2;  
                    osMessageQueuePut(KeyMsgQueue, &msg_key, 0, 1);
                    osMessageQueuePut(IdleModeBreakMsgQueue, &msg_idle_break, 0, 1);
                }
                break;
        }

        osDelay(pdMS_TO_TICKS(1)); 
    }
}


void KeyEventTask(void *argument)
{
    uint8_t msg_key = 0;
    while(1)
    {
        if(osMessageQueueGet(KeyMsgQueue, &msg_key, NULL, 0) == osOK) // 收到按键事件消息
        {
            
            if(msg_key == 1)
            {
                page_back();
                if(get_top_page()->page_obj == &ui_MenuPage) // 退回到菜单页面时
                {
                    hw_interface.hw_hrsensor_interface->diable(); // 心率传感器关闭
                    hw_interface.hw_ecompass_interface->sleep(); //  电子罗盘关闭
                    hw_interface.hw_barometer_interface->sleep(); // 气压传感器关闭
                }
            }

            else if(msg_key == 2)
            {
                page_back_bottom();// 退回到主页
                hw_interface.hw_hrsensor_interface->diable(); // 心率传感器关闭
                hw_interface.hw_ecompass_interface->sleep(); //  电子罗盘关闭
                hw_interface.hw_barometer_interface->sleep(); // 气压传感器关闭
            }

        }

        osDelay(pdMS_TO_TICKS(10)); // 每隔10ms检查一次按键事件
    }
}