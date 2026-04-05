#ifndef HW_LCD_INTERFACE_H
#define HW_LCD_INTERFACE_H

#include "device.h"

typedef struct{
 
    void (*set_light)(uint8_t duty); // 设置屏幕亮度，duty取值范围0-100，表示亮度百分比
    
}hw_lcd_interface_t;

extern hw_lcd_interface_t hw_lcd_interface;

#endif /* HW_LCD_INTERFACE_H */