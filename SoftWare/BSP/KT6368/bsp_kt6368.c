#include "bsp_kt6368.h"


void ble_kt6368_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = BLE_EN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BLE_EN_PORT, &GPIO_InitStruct);

    /*Configure GPIO pin Output Level: close kt6328 */
    HAL_GPIO_WritePin(BLE_EN_PORT, BLE_EN_PIN, GPIO_PIN_RESET);
}

void ble_kt6368_enable(void)
{
    HAL_GPIO_WritePin(BLE_EN_PORT, BLE_EN_PIN, GPIO_PIN_SET);
}

void ble_kt6368_disable(void)
{
    HAL_GPIO_WritePin(BLE_EN_PORT, BLE_EN_PIN, GPIO_PIN_RESET);
}