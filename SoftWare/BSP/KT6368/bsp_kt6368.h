#ifndef __BSP_KT6368_H__
#define __BSP_KT6368_H__

#include "main.h"

#define BLE_EN_PORT GPIOA
#define BLE_EN_PIN GPIO_PIN_8
#define UART1_TX_PORT GPIOA   
#define UART1_TX_PIN GPIO_PIN_9
#define UART1_RX_PORT GPIOA
#define UART1_RX_PIN GPIO_PIN_10

void ble_kt6368_init(void);

void ble_kt6368_enable(void);

void ble_kt6368_disable(void);

#endif // __BSP_KT6368_H__