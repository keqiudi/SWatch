#include "hw_ble_interface.h"
#include "bsp_kt6368.h"


static uint8_t hw_ble_init()
{
    ble_kt6368_init();
    return ERR_SUCCESS;
}

static uint8_t hw_ble_enable()
{
    ble_kt6368_enable();
    return ERR_SUCCESS;
}

static uint8_t hw_ble_disable()
{
    ble_kt6368_disable();
    return ERR_SUCCESS;
}

hw_ble_interface_t hw_kt6368_interface = {
    .init = hw_ble_init, 
    .enable = hw_ble_enable, 
    .disable = hw_ble_disable, 
};