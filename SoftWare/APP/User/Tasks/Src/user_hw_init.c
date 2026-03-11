

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

/* ��֤������ִ�����ȼ���ߣ��ϵ�����ִ�У�ͬʱ����������������ջ����Ϊ���ͷ�*/
void HwInitTask(void *argument)
{
		int ret = 0;
		
		/* Systick ��ʼ������ʹ��delay */
   		delay_init();
	  
		
		/* ��������س�ʼ�� */
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

		ret = hw_interface.hw_hrsensor_interface->init();
	  	if(ret == ERR_SUCCESS)
		{
			hw_interface.hw_hrsensor_interface->state = DEVICE_STATUS_INITED;
			hw_interface.hw_hrsensor_interface->diable();
		}
		

		
	  /* LCD ��ʾST7789��ʼ��*/
		LCD_Init();
		LCD_Fill(0,0,240,280,BLACK);
		LCD_Open_BackLight();
		LCD_Set_Light(50);
		
	  /* LCD ����CST816T��ʼ��*/
		CST816T_Init();
		CST816T_Reset();
		
		/*2. lvgl��ʼ��*/
		lv_init();			  // lvglϵͳ��ʼ��
		lv_port_disp_init();  // lvgl��ʾ�ӿڳ�ʼ��
		lv_port_indev_init(); // lvgl����ӿڳ�ʼ��
		ui_init();
		
		vTaskDelete(NULL); // ��ʼ����ɺ�ֱ��ɾ������
	
}	


