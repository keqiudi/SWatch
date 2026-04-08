#include "user_runMode_task.h"
#include "user_task_init.h"

#include "hw_interface.h"

#include "ui_SOffTimeSetPage.h"
#include "ui_LTSetPage.h"
#include "ui_HomePage.h"

#include "main.h"
#include "spi.h"
#include "dma.h"
#include "usart.h"
#include "stm32f4xx_it.h"

#include "bsp_lcd.h"
#include "bsp_touch_cst816t.h"
#include "bsp_mpu6050.h"
#include "bsp_key.h"
#include "bsp_power.h"
#include "bsp_wdog.h"

void SystemClock_Config(void);

uint16_t IdleTimerCounter = 0; // 进入Idle模式的时间计数，单位为100ms


void IdleEnterTask(void *argument)
{
    uint8_t msg_idle = 0;
    uint8_t msg_idle_break =0;
    while(1)
    {
        if(osMessageQueueGet(IdleModeMsgQueue,&msg_idle, NULL, 1) == osOK) // 收到进入Idle模式的消息
        {
            hw_interface.hw_lcd_interface->set_light(5); // 屏幕亮度调到最低
        }

        if(osMessageQueueGet(IdleModeBreakMsgQueue,&msg_idle_break, NULL, 1) == osOK) // 收到打断Idle模式的消息
        {
            IdleTimerCounter = 0; // 重置计数器
            hw_interface.hw_lcd_interface->set_light(ui_LightSliderValue); // 恢复屏幕亮度为设置亮度
        }

        osDelay(pdMS_TO_TICKS(10));
    }
}




void StopEnterTask(void *argument)
{
    uint8_t msg_stop = 0;
    uint8_t msg_update_home = 0;
    uint8_t wrist_flag = 0;

    while(1)
    {
        if(osMessageQueueGet(StopModeMsgQueue, &msg_stop, NULL, 0) == osOK) // 收到进入Stop模式的消息
        {
            while(1)
            {
                uint8_t should_wake = 0;
               
                /**************************** 进入stop模式前的操作 ****************************/

                IdleTimerCounter = 0; // 进入stop模式前先重置Idle模式计数器，防止误判为长时间未操作

                // sensors 
                //hw_interface.hw_hrsensor_interface->diable(); // 心率传感器关闭
                //hw_interface.hw_ecompass_interface->sleep(); //  电子罗盘关闭
                //hw_interface.hw_barometer_interface->sleep(); // 气压传感器关闭

                // usart 和 ble
                HAL_UART_MspDeInit(&huart1); // deinit usart1，关闭时钟和中断
                //hw_interface.hw_ble_interface->disable(); // 关闭蓝牙模块

                // lcd,st7789 sleep
                LCD_Close_BackLight(); // 关闭LCD背光
                // touch
                CST816T_Sleep(); // 触摸屏进入睡眠模式

                /****************************************************************************/
                /**************************** 准备进入stop模式 *******************************/

                // 挂起FreeRTOS所有任务，停止FreeRTOS调度，进入临界区，防止在stop模式期间有任务运行
                vTaskSuspendAll(); 
                // 关闭看门狗
                wdog_disable();
                // 关闭SysTick中断，防止在stop模式期间被SysTick唤醒
                CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk); 
                // 进入stop模式, 等待中断唤醒后从这里继续往下执行    
                HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
                
                /****************************************************************************/

                /**************************** 退出stop模式后的操作 ***************************/

                // 开启SysTick中断，恢复系统时钟，喂狗，恢复FreeRTOS调度
                SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
           
                SystemClock_Config(); // 恢复系统时钟为正常频率，进入stop模式后HSI时钟会变
                HAL_SYSTICK_Config(SystemCoreClock / (1000U / uwTickFreq)); // 恢复systick中断为1ms

                // 我这里不重新初始化唤醒后UI刷新不出来，我估计是stop后外设状态不确定导致的，
                MX_DMA_Init(); 
                MX_SPI1_Init(); 

                wdog_feed(); // 喂狗，防止复位

                xTaskResumeAll(); // 恢复FreeRTOS调度，允许任务运行

                /****************************************************************************/

                /****************************** 唤醒后的操作 *********************************/

                if(hw_interface.hw_mpu6050_interface->wrist_is_enabled)
                {
                    uint8_t hor = mpu6050_is_horizontal();
                    // 如果当前水平放置状态与之前的手腕状态不一致，说明发生了抬腕或放下事件，才更新手腕状态并触发相应的事件处理逻辑
                    if(hor && hw_interface.hw_mpu6050_interface->wrist_state == WRIST_DOWN)
                    {
                        hw_interface.hw_mpu6050_interface->wrist_state = WRIST_UP;
                        wrist_flag = 1;
                    }
                    else if(!hor && hw_interface.hw_mpu6050_interface->wrist_state == WRIST_UP)
                    {
                        hw_interface.hw_mpu6050_interface->wrist_state = WRIST_DOWN;
                        IdleTimerCounter = 0;
                        continue;
                    }
                }

                if(!KEY1 || KEY2 || hard_int_charge_flag || wrist_flag)
                {
                    wrist_flag = 0; //  重置抬腕事件标志位
                    should_wake = 1; // 唤醒标志位
                }

                if(!should_wake)
                {
                    IdleTimerCounter = 0; 
                    continue; // 如果唤醒后没有检测到有效的唤醒事件（按键、充电、抬腕），继续保持在stop模式
                }

                // usart
                HAL_UART_MspInit(&huart1);

                // lcd
                LCD_Init();
                //LCD_Open_BackLight(); 不用重新打开PWM, 因为没有关闭定时器
                LCD_Set_Light(ui_LightSliderValue);

                // touch    
                CST816T_Wakeup();

                // 可能不是充电中断唤醒，需要补偿一次，避免漏掉充电事件
                if(charge_check())
                {
                    hard_int_charge_flag = 1;
                }

                // send the Home update message
                osMessageQueuePut(HomeUpdataMsgQueue, &msg_update_home, 0, 1);

                break;
            }
        }
        osDelay(pdMS_TO_TICKS(100)); 
    }
}


void IdleTimerCallback(void *argument)
{
    IdleTimerCounter++; // 每100ms增加1，单位为100ms

    // 这里要确保常亮时间<息屏时间，以满足先进入Idle模式再进入Stop模式的逻辑
    if(IdleTimerCounter == light_time_value * 10) // 如果达到设置的常亮时间（转换为100ms单位）
    {
        uint8_t msg_idle = 1;
        osMessageQueuePut(IdleModeMsgQueue, &msg_idle, 0, 0); // 没有操作后时间到了进入idle模式
    }
    
    if(IdleTimerCounter == scrren_off_time_value * 10) // 如果达到设置的息屏时间（转换为100ms单位）
    {
        uint8_t msg_stop = 1;
        osMessageQueuePut(StopModeMsgQueue, &msg_stop, 0, 0); // 没有操作后时间到了进入stop模式
    }

}