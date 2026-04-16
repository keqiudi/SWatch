#include "data_save.h"

#include "delay.h"
#include "string.h"
#include "SEGGER_RTT.h"

/******************************************
EEPROM data layout:
[0x00]: 0x55 check marker
[0x01]: 0xCC check marker

[0x10]: wrist detection enable
[0x11]: app sync enable

[0x20]: last saved day (1-31)
[0x21-0x22]: day steps (uint16, big-endian)
*******************************************/


void eeprom_init()
{
    bl24c02_init();
}

uint8_t eeprom_check()
{

    uint8_t check_buff[2] = {0};

    delay_ms(5); // BL24C02有个写周期5ms,在写入数据后需要等待写入完成才能进行下一次操作，否则可能会出现通信失败的情况。这里简单地延时5ms，确保BL24C02完成写入周期。
    bl24c02_read(0,2,check_buff);
    if(check_buff[0] == 0x55 && check_buff[1] == 0xCC) // 检查标志位
    {
        //SEGGER_RTT_printf(0,"eeprom used, flag: 0x55 0xCC\r\n");
        return 0; // EEPROM已使用过
    }
    else
    {
        check_buff[0] = 0x55;
        check_buff[1] = 0xCC;
        bl24c02_write(0,2,check_buff);
        delay_ms(5); // 等待写周期5ms
        memset(check_buff,0,2); 
        bl24c02_read(0,2,check_buff);
        if(check_buff[0] == 0x55 && check_buff[1] == 0xCC)
        {
            //SEGGER_RTT_printf(0,"eeprom not used, write flag success: 0x55 0xCC\r\n");
            return 0; // EEPROM第一次使用，写入成功
        }
    }

    return 1; // check error
}

uint8_t eeprom_save_settings(uint8_t *buffer,uint8_t addr,uint8_t len)
{
    if(addr > 0x01 && !eeprom_check())
    {
        delay_ms(5);
        bl24c02_write(addr, len, buffer);
        delay_ms(5); // 等待写周期5ms
        return 0;
    }

    return 1; // save error
   
}

uint8_t eeprom_read_settings(uint8_t *buffer,uint8_t addr,uint8_t len)
{
    if(addr > 0x01 && !eeprom_check())
    {
        delay_ms(5);
        bl24c02_read(addr, len, buffer);
        delay_ms(5); // 等待写周期5ms
        return 0;
    }

    return 1;
}