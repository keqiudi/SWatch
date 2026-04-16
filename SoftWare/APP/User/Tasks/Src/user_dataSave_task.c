#include "user_dataSave_task.h"
#include "user_task_init.h"

#include "data_save.h"
#include "rtc.h"

#include "ui_DateTimeMenuPage.h"

#include "hw_interface.h"
/******************************************
EEPROM data layout:
[0x00]: 0x55 check marker
[0x01]: 0xCC check marker

[0x10]: wrist detection enable (hw_interface.hw_mpu6050_interface->wrist_is_enabled)
[0x11]: app sync enable (app_sync_en)

[0x20]: last saved day (1-31)
[0x21-0x22]: day steps (uint16, big-endian)
*******************************************/

void DataSaveTask(void *argument)
{
  while(1)
  {
    uint8_t msg_save = 0;
    if(osMessageQueueGet(DataSaveMsgQueue, &msg_save, NULL, 1) == osOK) // 收到数据保存消息
    {
        
        uint8_t data_buffer[3];

        // 抬腕亮屏和同步app时间状态保存
        data_buffer[0] = hw_interface.hw_mpu6050_interface->wrist_is_enabled; // 保存手腕检测设置
        data_buffer[1] = app_sync_en; // 保存同步APP设置
        eeprom_save_settings(data_buffer, 0x10, 2); // 从地址0x10开始保存2字节数据, 0x10:手腕检测设置，0x11:同步APP设置

        RTC_DateTypeDef nowdate;
        HAL_RTC_GetDate(&hrtc, &nowdate, RTC_FORMAT_BIN);

        eeprom_read_settings(data_buffer, 0x20, 3); // 保存当前日期到地址0x20
        if(data_buffer[0] != nowdate.Date) // 如果日期发生变化，说明新的一天开始了，需要保存前一天的步数数据，并重置当天的步数数据
        {
            if(hw_interface.hw_mpu6050_interface->state == DEVICE_STATUS_INITED)
            {
               hw_interface.hw_mpu6050_interface->set_steps(0); // 步数重置为0
            }

            data_buffer[0] = nowdate.Date; // 更新保存日期
            data_buffer[1] = 0; // 清空步数
            data_buffer[2] = 0; // 清空步数
            eeprom_save_settings(data_buffer, 0x20, 3); // 从地址0x20开始保存3字节数据, 0x20:日期，0x21-0x22:步数
        }
        else
        {
            uint16_t steps = hw_interface.hw_mpu6050_interface->get_steps();
            data_buffer[0] = nowdate.Date; // 更新保存日期
            data_buffer[1] = (steps >> 8) & 0xFF; // 保存步数高8位
            data_buffer[2] = steps & 0xFF; // 保存步数低8位
            eeprom_save_settings(data_buffer, 0x20, 3); // 从地址
        }
    }

     osDelay(pdMS_TO_TICKS(100)); 
  }
}   