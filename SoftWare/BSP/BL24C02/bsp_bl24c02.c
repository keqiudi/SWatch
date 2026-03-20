#include "bsp_bl24c02.h"
#include "i2c_hal.h"

i2c_bus_t bl24c02_bus = {
    .I2C_SDA_PORT = BL24C02_SDA_PORT,
    .I2C_SCL_PORT = BL24C02_SCL_PORT,
    .I2C_SDA_PIN = BL24C02_SDA_PIN,
    .I2C_SCL_PIN = BL24C02_SCL_PIN
};



void bl24c02_init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    i2c_init(&bl24c02_bus);
}

uint8_t bl24c02_write(uint8_t reg_addr, uint8_t len,uint8_t buff[])
{
    uint8_t i;

	i2c_soft_start(&bl24c02_bus);
	i2c_soft_send_byte(&bl24c02_bus,BL24C02_I2C_ADDRESS << 1);
	if(i2c_soft_wait_ack(&bl24c02_bus))
    {
        i2c_soft_stop(&bl24c02_bus);
        return 1; // 发送设备地址失败
    }

    i2c_soft_send_byte(&bl24c02_bus,reg_addr);
    if(i2c_soft_wait_ack(&bl24c02_bus))
    {
        i2c_soft_stop(&bl24c02_bus);
        return 1;
    }

    for(i=0;i<len;i++)
    {
        i2c_soft_send_byte(&bl24c02_bus,buff[i]);
    }

    i2c_soft_stop(&bl24c02_bus);
    return 0; // 写入成功
}

uint8_t bl24c02_read(uint8_t reg_addr, uint8_t len,uint8_t buff[])
{
    i2c_soft_start(&bl24c02_bus);
    i2c_soft_send_byte(&bl24c02_bus,BL24C02_I2C_ADDRESS << 1);
    if(i2c_soft_wait_ack(&bl24c02_bus))
    {
        i2c_soft_stop(&bl24c02_bus);
        return 1; // 发送设备地址失败
    }

    i2c_soft_send_byte(&bl24c02_bus,reg_addr);
    if(i2c_soft_wait_ack(&bl24c02_bus))
    {
        i2c_soft_stop(&bl24c02_bus);
        return 1;
    }

    // 发送重复起始条件，准备读取数据
    i2c_soft_start(&bl24c02_bus);
    i2c_soft_send_byte(&bl24c02_bus,(BL24C02_I2C_ADDRESS << 1) | 0x01); // 读操作
    if(i2c_soft_wait_ack(&bl24c02_bus))
    {
        i2c_soft_stop(&bl24c02_bus);
        return 1; // 发送设备地址失败
    }

    for(uint8_t i=0;i<len;i++)
    {
         buff[i] = i2c_soft_read_byte(&bl24c02_bus);
         if(i<len-1)
         {
             i2c_soft_send_ack(&bl24c02_bus); // 发送ACK，表示继续读取
         }
         else
         {
             i2c_soft_send_not_ack(&bl24c02_bus); // 最后一个字节发送NACK，表示读取结束
         }
    }

    i2c_soft_stop(&bl24c02_bus);
    return 0; // 读取成功
}