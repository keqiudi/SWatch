#include "hw_ecompass_interface.h"
#include "bsp_lsm303dlhc.h"

static int hw_ecompass_init()
{
    int ret = lsm303dlhc_init();
    if(ret == 1)
    {
        return ERR_FAILED;
    }

    return ERR_SUCCESS;
}

static void hw_ecompass_sleep()
{
    lsm303dlhc_sleep();
}




hw_ecompass_interface_t hw_lsm303dlhc_interface = {
    .state = DEVICE_STATUS_UNINIT,
    .direction = 0,
    .init = hw_ecompass_init, // 这里需要实现初始化函数
    .sleep = hw_ecompass_sleep, // 这里需要实现睡眠函数
};