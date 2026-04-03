
#include "user_task_init.h"
#include "user_chargeCheck_task.h"
#include "ui_ChargePage.h"
#include "stm32f4xx_it.h"
#include "bsp_power.h"



void ChargeCheckTask(void *argument)
{
    while(1)
    {
       
        if(hard_int_charge_flag) // 检测到充电状态变化
        {
            hard_int_charge_flag = 0; // 清除标志位

            if(charge_check() && get_top_page()->page_obj != &ui_ChargePage) 
            {
                page_load(&page_charge); // 如果检测到充电事件且当前页面不是充电页面，就跳转到充电页面
            }

            if(!charge_check() && get_top_page()->page_obj == &ui_ChargePage) 
            {
                page_back(); // 如果检测到充电事件结束且当前页面是充电页面，就返回上一页
            }
        }
        osDelay(pdMS_TO_TICKS(500)); // 每隔0.5秒检查一次充电状态
    }
}