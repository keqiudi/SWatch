#ifndef HW_POWER_INTERFACE_H
#define HW_POWER_INTERFACE_H


#include "device.h"

typedef struct{
    
    uint8_t remain_power; // 当前剩余电量百分比，取值范围0-100

	void (*init)(void);
    void (*shutdown)(void);
    uint8_t (*bat_caluculate)(void); // 根据电压计算剩余电量百分比
}hw_power_interface_t; 

extern hw_power_interface_t hw_power_interface;

#endif /* HW_POWER_INTERFACE_H */