
#include "hw_power_interface.h"
#include "bsp_power.h"

static void hw_power_init(void)
{
    power_init();
}

static void hw_power_shutdown(void)
{
    power_disEnable();
}

static uint8_t hw_power_bat_calculate(void)
{
    return power_calculate();
}


hw_power_interface_t hw_power_interface = {
    .remain_power = 0,
    .init = hw_power_init, 
    .shutdown = hw_power_shutdown, 
    .bat_caluculate = hw_power_bat_calculate
};