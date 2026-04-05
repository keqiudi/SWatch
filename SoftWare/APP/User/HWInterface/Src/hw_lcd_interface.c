#include "hw_lcd_interface.h" 
#include "bsp_lcd.h"

static void hw_lcd_set_light(uint8_t duty)
{
    LCD_Set_Light(duty);
}

hw_lcd_interface_t hw_lcd_interface = {
    .set_light = hw_lcd_set_light, // 这里需要在具体的硬件实现文件中定义这个函数
};


