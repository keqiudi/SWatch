#ifndef DATA_SAVE_H
#define DATA_SAVE_H

#include "bsp_bl24c02.h"


void eeprom_init();

uint8_t eeprom_check();

uint8_t eeprom_save_settings(uint8_t *buffer,uint8_t addr,uint8_t len);

uint8_t eeprom_read_settings(uint8_t *buffer,uint8_t addr,uint8_t len);

#endif // DATA_SAVE_H