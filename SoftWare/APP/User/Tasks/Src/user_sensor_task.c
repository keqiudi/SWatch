


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

#include "lvgl.h"
#include "ui_EnvironmentPage.h"
#include "ui_HeartRatePage.h"


uint32_t user_HR_timecount=0;


void HRDataTask(void *argument)
{
	uint8_t hr_rate,spo2;
	while(1)
	{
		
		if(get_top_page()->page_obj == &ui_HeartRatePage)
		{

			em7028_hrs_enable(); // 启动心率传感器测量

			if(hw_interface.hw_hrsensor_interface->state == DEVICE_STATUS_INITED)
			{
				//vTaskSuspendAll(); 可以不用暂停任务调度保护数据
				hr_rate = HR_Calculate(em7028_get_hrs1(), user_HR_timecount);
				//xTaskResumeAll();
				if(hr_rate > 0 && hr_rate < 220) // 心率合理范围
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

  sensor_msg_t sensor_msg;	
	while(1)
	{	
		if(osMessageQueueGet(SensorMsgQueue,&sensor_msg,NULL,0) == osOK)
		{
				switch(sensor_msg)
				{
					case MSG_AHT20_MEASURE:
					{
							if(hw_interface.hw_aht20_interface->state == DEVICE_STATUS_INITED)
							{
								float temp,hum;
								hw_interface.hw_aht20_interface->read(&temp,&hum);
								if((temp >= TEMP_RANGE_LOW && temp <=TEMP_RANGE_HIGH) && (hum >= HUM_RANGE_LOW && hum<= HUM_RANGE_HIGH)){
										hw_interface.hw_aht20_interface->temperature = temp;
										hw_interface.hw_aht20_interface->humidity = hum;
								}
							}
							
							break;
					}
					
					case MSG_COMPASS_MEASURE:
					{
							if(hw_interface.hw_ecompass_interface->state == DEVICE_STATUS_INITED)
							{
								int16_t ax,ay,az,mx,my,mz;
								lsm303dlhc_read_accel(&ax,&ay,&az);
								lsm303dlhc_read_magnetic(&mx,&my,&mz);
								float direction = lsm303dlhc_calc_azimuth_angle(ax,ay,az,mx,my,mz);
								if(direction<0)
								{
									direction+=360; // 确保方向在0-360度之间
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
							
							break;
					}

					case MSG_MPU6050_MEASURE:
					{
						
									break;
					}
						
					case MSG_EM7028_MEASURE:
						
					{
									break;
					}
				}
		 }

		osDelay(pdMS_TO_TICKS(100));
	}
}
	