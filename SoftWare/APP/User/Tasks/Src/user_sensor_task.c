


#include "hw_interface.h"
#include "user_sensor_task.h"

#include "page_manager.h"
#include "ui_EnvironmentPage.h"
#include "SEGGER_RTT.h"
#include "lvgl.h"
#include "bsp_aht20.h"

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
	