#include "user_wdog_task.h"
#include "bsp_wdog.h"
#include "user_task_init.h"

void WDOGFeedTask(void *argument)
{
    wdog_init(); // 初始化看门狗GPIO
    while(1)
    {
        wdog_feed(); // 喂狗
        wdog_enable(); // 使能看门狗，确保在第一次喂狗后就开始监视系统状态
        osDelay(pdMS_TO_TICKS(100)); // TPS3823的超时周期标准值是1.6s，最小值是0.9s
    }
}