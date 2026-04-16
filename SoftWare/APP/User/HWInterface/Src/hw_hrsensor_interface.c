

#include "hw_hrsensor_interface.h"
#include "bsp_em7028.h"

static uint8_t hw_hrsensor_init()
{
    int ret = em7028_hrs_init();
    if(ret == 1)
    {
        return ERR_FAILED;
    }

    return ERR_SUCCESS;
}

static uint8_t hw_hrsensor_disable()
{
    int ret = em7028_hrs_disEnable();
    if(ret == 1)
    {
        return ERR_FAILED;
    }

    return ERR_SUCCESS;
}



hw_hrsensor_interface_t hw_em7028_interface = {
    .state = DEVICE_STATUS_UNINIT,
    .hr_rate = 0,
    .spo2 = 0,
    .init = hw_hrsensor_init, 
    .diable = hw_hrsensor_disable, 
};