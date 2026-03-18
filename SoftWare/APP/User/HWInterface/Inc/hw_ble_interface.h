#ifndef HW_BLE_INTERFACE_H
#define HW_BLE_INTERFACE_H

#include "device.h"

typedef struct{
 
	//device_status_t state;
		uint8_t (*init)(void);
    uint8_t (*enable)(void);
    uint8_t (*disable)(void);
    
}hw_ble_interface_t;

extern hw_ble_interface_t hw_kt6368_interface;



#endif /* HW_BLE_INTERFACE_H */