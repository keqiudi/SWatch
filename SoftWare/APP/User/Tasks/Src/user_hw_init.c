

#include "delay.h"
#include "user_hw_init.h"
#include "user_task_init.h"

#include "bsp_lcd.h"
#include "bsp_touch_cst816t.h"
#include "bsp_aht20.h"
#include "bsp_key.h"

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
		
		/* BLE模块初始化 */
		hw_interface.hw_ble_interface->init();
		hw_interface.hw_ble_interface->disable();
		
		
		/* LCD 显示ST7789初始化 */
			LCD_Init();
			LCD_Fill(0,0,240,280,BLACK);
			LCD_Open_BackLight();
			LCD_Set_Light(50);
			
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


