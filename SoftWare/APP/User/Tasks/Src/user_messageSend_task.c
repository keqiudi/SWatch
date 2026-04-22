
#include "string.h"
#include "stdio.h"

#include "main.h"
#include "stm32f4xx_it.h"
#include "rtc.h"
#include "usart.h"

#include "user_messageSend_task.h"
#include "user_task_init.h"

#include "hw_interface.h"
#include "ui.h"
#include "ui_DateTimeMenuPage.h"

struct
{
	RTC_DateTypeDef nowdate;
	RTC_TimeTypeDef nowtime;
	int8_t humi;
	int8_t temp;
	uint8_t HR;
	uint8_t SPO2;
	uint16_t stepNum;
}BLEMessage;

struct
{
	RTC_DateTypeDef nowdate;
	RTC_TimeTypeDef nowtime;
}TimeSetMessage;

void StrCMD_Get(uint8_t * str,uint8_t * cmd)
{
	uint8_t i=0;
  while(str[i]!='=')
  {
      cmd[i] = str[i];
      i++;
  }
}

//set time//OV+ST=20230629125555
uint8_t TimeFormat_Get(uint8_t * str)
{
	TimeSetMessage.nowdate.Year = (str[8]-'0')*10+str[9]-'0';
	TimeSetMessage.nowdate.Month = (str[10]-'0')*10+str[11]-'0';
	TimeSetMessage.nowdate.Date = (str[12]-'0')*10+str[13]-'0';
	TimeSetMessage.nowtime.Hours = (str[14]-'0')*10+str[15]-'0';
	TimeSetMessage.nowtime.Minutes = (str[16]-'0')*10+str[17]-'0';
	TimeSetMessage.nowtime.Seconds = (str[18]-'0')*10+str[19]-'0';
	if(TimeSetMessage.nowdate.Year>0 && TimeSetMessage.nowdate.Year<99
		&& TimeSetMessage.nowdate.Month>0 && TimeSetMessage.nowdate.Month<=12
		&& TimeSetMessage.nowdate.Date>0 && TimeSetMessage.nowdate.Date<=31
		&& TimeSetMessage.nowtime.Hours>=0 && TimeSetMessage.nowtime.Hours<=23
		&& TimeSetMessage.nowtime.Minutes>=0 && TimeSetMessage.nowtime.Minutes<=59
		&& TimeSetMessage.nowtime.Seconds>=0 && TimeSetMessage.nowtime.Seconds<=59)
	{
        my_rtc_set_date(TimeSetMessage.nowdate.Year, TimeSetMessage.nowdate.Month,TimeSetMessage.nowdate.Date);
        my_rtc_set_time(TimeSetMessage.nowtime.Hours, TimeSetMessage.nowtime.Minutes,TimeSetMessage.nowtime.Seconds);
		printf("TIMESETOK\r\n");
	}
}


/**
  * @brief  send the message via BLE, use uart
  * @param  argument: Not used
  * @retval None
  */
void MessageSendTask(void *argument) 
{
	while(1)
	{
		if(hard_int_uart_flag)
		{
			hard_int_uart_flag = 0;
			uint8_t msg_idle_break = 0;
			osMessageQueuePut(IdleModeBreakMsgQueue,&msg_idle_break,NULL,1);
            // 发送接收到的数据
			printf("data:%s\r\n",uart_int_receive_str);

			if(!strcmp(uart_int_receive_str,"SW"))
			{
				printf("OK\r\n");
			}
			else if(!strcmp(uart_int_receive_str,"SW+VERSION"))
			{
				printf("VERSION=V%d.%d.%d\r\n", 1, 0, 0);
			}
			else if(!strcmp(uart_int_receive_str,"SW+SEND"))
			{
				HAL_RTC_GetTime(&hrtc,&(BLEMessage.nowtime),RTC_FORMAT_BIN);
				HAL_RTC_GetDate(&hrtc,&BLEMessage.nowdate,RTC_FORMAT_BIN);
				BLEMessage.humi = hw_interface.hw_aht20_interface->humidity;
				BLEMessage.temp = hw_interface.hw_aht20_interface->temperature;
				BLEMessage.HR = hw_interface.hw_hrsensor_interface->hr_rate;
				BLEMessage.SPO2 = hw_interface.hw_hrsensor_interface->spo2;
				BLEMessage.stepNum = hw_interface.hw_mpu6050_interface->steps;

				printf("data:%2d-%02d\r\n",BLEMessage.nowdate.Month,BLEMessage.nowdate.Date);
				printf("time:%02d:%02d:%02d\r\n",BLEMessage.nowtime.Hours,BLEMessage.nowtime.Minutes,BLEMessage.nowtime.Seconds);
				printf("humidity:%d%%\r\n",BLEMessage.humi);
				printf("temperature:%d\r\n",BLEMessage.temp);
				printf("Heart Rate:%d%%\r\n",BLEMessage.HR);
				printf("SPO2:%d%%\r\n",BLEMessage.SPO2);
				printf("Step today:%d\r\n",BLEMessage.stepNum);
			}
			//set time//SWATCH+ST=20230629125555
			else if(strlen(uart_int_receive_str)==20)
			{
				uint8_t cmd[10];
				memset(cmd,0,sizeof(cmd));
				StrCMD_Get(uart_int_receive_str,cmd);
				if(app_sync_en && !strcmp(cmd,"SW+ST"))
				{
					TimeFormat_Get(uart_int_receive_str);
				}
			}
			memset(uart_int_receive_str,0,sizeof(uart_int_receive_str));
		}
		osDelay(pdMS_TO_TICKS(1000));
	}
}


