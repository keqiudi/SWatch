

#include "delay.h"
#include "user_hw_init.h"
#include "user_task_init.h"

#include "bsp_lcd.h"
#include "bsp_touch_cst816t.h"
#include "bsp_aht20.h"

#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lvgl.h"
#include "ui.h"

#include "device.h"
#include "hw_interface.h"

#include "SEGGER_RTT.h"

/* 保证该任务执行优先级最高，上电最先执行，同时不能在这里面分配堆栈，因为会释放*/
void HwInitTask(void *argument)
{
		int ret = 0;
		
		/* Systick 初始化才能使用delay */
   		delay_init();
	  
		
		//mpu6050_init();
		
		//lsm303dlhc_init();

		/* 传感器相关初始化 */
		ret = hw_interface.hw_aht20_interface->init();
	  	if(ret == ERR_SUCCESS)
		{
			hw_interface.hw_aht20_interface->state = DEVICE_STATUS_INITED;
		}
		
		ret = hw_interface.hw_barometer_interface->init();
	  	if(ret == ERR_SUCCESS)
		{
			hw_interface.hw_barometer_interface->state = DEVICE_STATUS_INITED;
		}

		ret = hw_interface.hw_ecompass_interface->init();
	  	if(ret == ERR_SUCCESS)
		{
			hw_interface.hw_ecompass_interface->state = DEVICE_STATUS_INITED;
		}
		

		
	  /* LCD 显示ST7789初始化*/
		LCD_Init();
		LCD_Fill(0,0,240,280,BLACK);
		LCD_Open_BackLight();
		LCD_Set_Light(50);
		
	  /* LCD 触摸CST816T初始化*/
		CST816T_Init();
		CST816T_Reset();
		
		/*2. lvgl初始化*/
		lv_init();			  // lvgl系统初始化
		lv_port_disp_init();  // lvgl显示接口初始化
		lv_port_indev_init(); // lvgl输入接口初始化
		ui_init();
		
		vTaskDelete(NULL); // 初始化完成后直接删除任务
	
}	


