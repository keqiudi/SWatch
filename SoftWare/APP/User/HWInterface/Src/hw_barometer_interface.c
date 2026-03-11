#include "bsp_spl06.h"
#include "hw_barometer_interface.h"


static uint8_t hw_barometer_init()
{
    int ret = spl06_init();
    if(ret == 1)
    {
        return ERR_FAILED;
    }
    
    return ERR_SUCCESS;
}

hw_barometer_interface_t hw_spl06_interface = {
    .state = DEVICE_STATUS_UNINIT,
    .altitude = 0,
    .init = hw_barometer_init,
};