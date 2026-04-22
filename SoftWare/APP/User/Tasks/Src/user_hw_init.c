#include "rtc.h"
#include "delay.h"
#include "usart.h"
#include "stm32f4xx_it.h"

#include "bsp_lcd.h"
#include "bsp_touch_cst816t.h"
#include "bsp_aht20.h"
#include "bsp_key.h"
#include "data_save.h"
#include "bsp_power.h"

#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lvgl.h"
#include "ui.h"
#include "ui_DateTimeMenuPage.h"

#include "device.h"
#include "hw_interface.h"
#include "user_hw_init.h"
#include "user_task_init.h"

#include "SEGGER_RTT.h"


/* 保证该任务执行优先级最高，上电最先执行，同时不能在这里面分配堆栈，因为会释放*/
void HwInitTask(void *argument)
{
		
		int ret = 0;
		
		// 如果备份寄存器被写过，就不会执行配置RTC唤醒的代码，就需要我们手动配置RTC唤醒
		if(HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 2000, RTC_WAKEUPCLOCK_RTCCLK_DIV16) != HAL_OK)
		{
		Error_Handler();
		}

		HAL_UART_Receive_DMA(&huart1,(uint8_t*)uart_int_receive_str,25); // 开启UART DMA接收，接收数据存储在hard_int_receive_str缓冲区，长度为25字节
    	__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE); // 使能UART空闲中断，空闲中断表示接收完成

 		/* Systick 初始化才能使用delay */
   		delay_init();
		
		// /* 启动电源 */
		hw_interface.hw_power_interface->init(); // 电源管理初始化，配置电池检测引脚和充电状态检测引脚

		/* 按键GPIO初始化，配置为外部中断模式 */
		key_gpio_init(); 
		
		/* 传感器相关初始化 */ 
		ret = hw_interface.hw_aht20_interface->init(); // AHT20温湿度传感器初始化
	  	if(ret == ERR_SUCCESS)
		{
			hw_interface.hw_aht20_interface->state = DEVICE_STATUS_INITED;
		}
		
		ret = hw_interface.hw_mpu6050_interface->init(); // MPU6050 DMP库初始化初始化
	  	if(ret == ERR_SUCCESS)
		{
			hw_interface.hw_mpu6050_interface->state = DEVICE_STATUS_INITED;
		}

		ret = hw_interface.hw_barometer_interface->init(); // SPL06气压计初始化
	  	if(ret == ERR_SUCCESS)
		{
			hw_interface.hw_barometer_interface->state = DEVICE_STATUS_INITED;
		}

		ret = hw_interface.hw_ecompass_interface->init(); // LSM303DLHC电子罗盘初始化
	  	if(ret == ERR_SUCCESS)
		{
			hw_interface.hw_ecompass_interface->state = DEVICE_STATUS_INITED;
		}

		ret = hw_interface.hw_hrsensor_interface->init(); // EM7028心率传感器初始化
	  	if(ret == ERR_SUCCESS)
		{
			hw_interface.hw_hrsensor_interface->state = DEVICE_STATUS_INITED;
			hw_interface.hw_hrsensor_interface->diable();
		}
		

		/* eeprom 初始化*/
		eeprom_init();
		if(!eeprom_check())
		{
			uint8_t rBuffer[3];
	
			eeprom_read_settings(rBuffer, 0x10, 2); // 从地址0x10读取2字节数据, 0x10:手腕检测设置，0x11:同步APP设置
			
			RTC_DateTypeDef nowdate;
			HAL_RTC_GetDate(&hrtc, &nowdate, RTC_FORMAT_BIN);
			if((rBuffer[0] !=0 && rBuffer[0] != 1) || (rBuffer[1] != 0 && rBuffer[1] != 1)) // 读取的数据不合法时，使用默认设置
			{
				hw_interface.hw_mpu6050_interface->wrist_is_enabled = 0; 
				app_sync_en = 0; 
			}
			else
			{
				hw_interface.hw_mpu6050_interface->wrist_is_enabled = rBuffer[0]; // 恢复手腕检测设置
				app_sync_en = rBuffer[1]; // 恢复同步APP设置
			}

			eeprom_read_settings(rBuffer, 0x20, 3);
			if(rBuffer[0] == nowdate.Date) // 如果保存的日期与当前日期一致，说明是同一天，恢复当天的步数数据
			{
				uint16_t steps = ((uint16_t)rBuffer[1] << 8) | rBuffer[2];
				if(hw_interface.hw_mpu6050_interface->state == DEVICE_STATUS_INITED)
				{
					hw_interface.hw_mpu6050_interface->set_steps(steps); // 恢复步数数据
				}
			}
		}

		/* BLE KT6328 初始化 */
		hw_interface.hw_ble_interface->init();
		hw_interface.hw_ble_interface->disable();
		
		
		/* LCD 显示ST7789初始化 */
		LCD_Init();
		LCD_Fill(0,0,240,280,BLACK);
		LCD_Open_BackLight();
		LCD_Set_Light(50);
		LCD_ShowString(34, 280/2, (uint8_t*)"SWatch Welcome!", WHITE, BLACK, 24, 0);
		delay_ms(1000);
		LCD_Fill(0,0,240,280,BLACK); 


		/* LCD 触摸CST816T初始化 */
		CST816T_Init();
		CST816T_Reset();
			
		/*2. lvgl初始化*/
		lv_init();			  // lvgl初始化
		lv_port_disp_init();  // lvgl显示初始化
		lv_port_indev_init(); // lvgl触摸初始化
		ui_init();            // UI界面初始化
			
		vTaskDelete(NULL); // 删除当前任务，释放资源
}	


