


#include "SEGGER_RTT.h"
#include "hw_interface.h"
#include "user_sensor_task.h"

#include "page_manager.h"
#include "hr_algorithm.h"

#include "bsp_aht20.h"
#include "bsp_spl06.h"
#include "bsp_mpu6050.h"
#include "bsp_lsm303dlhc.h"
#include "bsp_em7028.h"
#include "bsp_power.h"

#include "lvgl.h"
#include "ui_EnvironmentPage.h"
#include "ui_HeartRatePage.h"
#include "ui_CompassPage.h"
#include "ui_SpO2MeasurePage.h"


uint32_t user_HR_timecount=0; // 心率计算时间计数, 每1ms增加1，单位为ms



void WristWakeCheckTask(void *argument)
{
    while(1)
    {

        if(hw_interface.hw_mpu6050_interface->wrist_is_enabled)
        {

            if(mpu6050_is_horizontal()) // 判断当前是否水平放置，水平放置则认为是抬腕状态
            {
                hw_interface.hw_mpu6050_interface->wrist_state = WRIST_UP;
            }
            else  
            {
                if(hw_interface.hw_mpu6050_interface->wrist_state == WRIST_UP) // 只有从手腕抬起到放下的状态变化才触发
                {
                    hw_interface.hw_mpu6050_interface->wrist_state = WRIST_DOWN;

                    if(get_top_page()->page_obj == &ui_HomePage || 
                        get_top_page()->page_obj == &ui_MenuPage ) // 只有在主页才触发抬腕事件，其他页面不受影响
                    {
                        uint8_t msg_stop = 0;
                        osMessageQueuePut(StopModeMsgQueue, &msg_stop, 0, 1); // 通过消息队列通知进入Stop模式
                        
                    }
                }
            }

        }
            // float yaw,pitch, roll;
            // mpu6050_accel_get_angles(&yaw, &pitch, &roll); 
            // SEGGER_RTT_printf(0,"wrist_state: %s, yaw: %.2f, pitch: %.2f, roll: %.2f\n", 
            //     (hw_interface.hw_mpu6050_interface->wrist_state == WRIST_UP) ? "UP" : "DOWN", yaw, pitch, roll);

        osDelay(pdMS_TO_TICKS(200));
    }
}




void HRDataTask(void *argument)
{
	uint8_t hr_rate,spo2;
    uint8_t msg_idle_break = 0;
	while(1)
	{
		
		if(get_top_page()->page_obj == &ui_HeartRatePage)
		{

            osMessageQueuePut(IdleModeBreakMsgQueue, &msg_idle_break, 0, 1); // 避免进入idle状态

			em7028_hrs_enable(); // 启动心率传感器测量

			if(hw_interface.hw_hrsensor_interface->state == DEVICE_STATUS_INITED)
			{
				//vTaskSuspendAll();  可以不用暂停任务调度保护数据
				hr_rate = HR_Calculate(em7028_get_hrs1(), user_HR_timecount);
				//xTaskResumeAll();
				if(hr_rate > 0 && hr_rate < 220) 
				{
					hw_interface.hw_hrsensor_interface->hr_rate = hr_rate;
				}
			}

		}
		osDelay(pdMS_TO_TICKS(50));
	}
}


void SensorDataUpdateTask(void *argument)
{
    uint8_t msg_idle_break = 0;

	while(1)
	{	

		/*Home page：battery、steps、heartrate*/
		uint8_t msg_home_update;
		if(osMessageQueueGet(HomeUpdataMsgQueue,&msg_home_update,NULL,0) == osOK) // 更新HOME页面数据
        {
            uint8_t buffer[5];
            uint8_t power = 0;

            // 电量更新
            power = hw_interface.hw_power_interface->bat_caluculate();
            if(power > 0 && power <= 100)
            {
                hw_interface.hw_power_interface->remain_power = power;
            }
            else
            {
                hw_interface.hw_power_interface->remain_power = 0;
            }

			// 步数更新
            if(hw_interface.hw_mpu6050_interface->state == DEVICE_STATUS_INITED)
            {
                hw_interface.hw_mpu6050_interface->steps = hw_interface.hw_mpu6050_interface->get_steps();
            }
            
            // 心率更新

            // 保存数据到EEPROM
            uint8_t save_msg = 0;
            osMessageQueuePut(DataSaveMsgQueue, &save_msg, 0, 1); // 通过消息队列通知数据保存任务保存数据
		}

		 /* Environment page: AHT20 */
        if(get_top_page()->page_obj == &ui_EnvironmentPage)
        {
            osMessageQueuePut(IdleModeBreakMsgQueue, &msg_idle_break, 0, 1); // 避免进入idle状态

            if(hw_interface.hw_aht20_interface->state == DEVICE_STATUS_INITED)
            {
                float temp, hum;
                hw_interface.hw_aht20_interface->read(&temp, &hum);
                if((temp >= TEMP_RANGE_LOW && temp <= TEMP_RANGE_HIGH) &&
                   (hum >= HUM_RANGE_LOW && hum <= HUM_RANGE_HIGH))
                {
                    hw_interface.hw_aht20_interface->temperature = temp;
                    hw_interface.hw_aht20_interface->humidity = hum;
                }
            }
        }

        /* Compass page: Compass + barometer */
        else if(get_top_page()->page_obj == &ui_CompassPage)
        {
            osMessageQueuePut(IdleModeBreakMsgQueue, &msg_idle_break, 0, 1);

            hw_interface.hw_ecompass_interface->wakeup(); //唤醒指南针

            if(hw_interface.hw_ecompass_interface->state == DEVICE_STATUS_INITED)
            {
                int16_t ax, ay, az, mx, my, mz;
                lsm303dlhc_read_accel(&ax, &ay, &az); // 计算方位角需要加速度计数据做倾斜补偿
                lsm303dlhc_read_magnetic(&mx, &my, &mz); // 磁力计数据用于计算方位角

                float direction = lsm303dlhc_calc_azimuth_angle(ax, ay, az, mx, my, mz);
                if(direction < 0)
                {
                    direction += 360;
                }

                if(direction >= 0 && direction <= 360)
                {
                    hw_interface.hw_ecompass_interface->direction = (uint16_t)direction;
                }
            }

            if(hw_interface.hw_barometer_interface->state == DEVICE_STATUS_INITED)
            {
                float altitude = spl06_calculate_altitude();
                hw_interface.hw_barometer_interface->altitude = (int16_t)altitude;
            }
        }

		/*SPO2 page */
		 else if(get_top_page()->page_obj == &ui_SpO2MeasurePage)
		 {
            osMessageQueuePut(IdleModeBreakMsgQueue, &msg_idle_break, 0, 1);
			// 保留
		 }

		osDelay(pdMS_TO_TICKS(100));
	}
}
	