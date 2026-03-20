#ifndef BSP_BL24C02_H
#define BSP_BL24C02_H

#include <stdint.h>

#define BL24C02_I2C_ADDRESS 0x50 // BL24C02(2k)的I2C地址

#define BL24C02_SDA_PORT GPIOA
#define BL24C02_SDA_PIN GPIO_PIN_11
#define BL24C02_SCL_PORT GPIOA
#define BL24C02_SCL_PIN GPIO_PIN_12

void bl24c02_init(void);
uint8_t bl24c02_write(uint8_t reg_addr, uint8_t len,uint8_t buff[]);
uint8_t bl24c02_read(uint8_t reg_addr, uint8_t len,uint8_t buff[]);


#endif // BSP_BL24C02_H