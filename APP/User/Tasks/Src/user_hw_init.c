


#include "user_hw_init.h"
#include "user_task_init.h"

#include "delay.h"

#include "bsp_lcd.h"
#include "bsp_touch_cst816t.h"
#include "bsp_aht20.h"

#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lvgl.h"



void User_Hw_Init()	
{
		delay_init();
		
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
		lv_port_disp_init();  // lvgl��ʾ�ӿڳ�ʼ��,����lv_init()�ĺ���
		lv_port_indev_init(); // lvgl����ӿڳ�ʼ��,����lv_init()�ĺ���
		
		/*3. ���Դ��룬��һ����ť*/
		lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0xFFFFFF), LV_PART_MAIN); // ���ñ���Ϊ��ɫ
		lv_obj_t* switch_obj = lv_switch_create(lv_scr_act());
		lv_obj_set_size(switch_obj, 120, 50);
		lv_obj_align(switch_obj, LV_ALIGN_CENTER, 0, 0);
}

void HwInitTask(void *argument)
{

}	