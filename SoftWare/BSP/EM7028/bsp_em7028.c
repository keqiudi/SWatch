#include "bsp_em7028.h"
#include "delay.h"

i2c_bus_t em7028_bus = {
    .I2C_SCL_PORT = EM7028_SCL_PORT,
    .I2C_SDA_PORT = EM7028_SDA_PORT,
    .I2C_SDA_PIN = EM7028_SDA_PIN,
    .I2C_SCL_PIN = EM7028_SCL_PIN
};


static uint8_t em7028_write_reg(uint8_t reg_addr,uint8_t data)
{
    i2c_soft_start(&em7028_bus);
    i2c_soft_send_byte(&em7028_bus, EM7028_ADDR << 1); // 写入地址
    if(i2c_soft_wait_ack(&em7028_bus)) // 非应答处理
    {
         i2c_soft_stop(&em7028_bus);
         return 1;
    }

    i2c_soft_send_byte(&em7028_bus, reg_addr);
    if(i2c_soft_wait_ack(&em7028_bus)) // 非应答处理
    {
         i2c_soft_stop(&em7028_bus);
         return 1;
    }

    i2c_soft_send_byte(&em7028_bus, data);
    if(i2c_soft_wait_ack(&em7028_bus)) // 非应答处理
    {
         i2c_soft_stop(&em7028_bus);
         return 1;
    }

    i2c_soft_stop(&em7028_bus);

    return 0;
}

static uint8_t em7028_read_reg(uint8_t reg_addr)
{
    uint8_t data = 0;

    i2c_soft_start(&em7028_bus);
    i2c_soft_send_byte(&em7028_bus, EM7028_ADDR << 1); // 写入地址
    if(i2c_soft_wait_ack(&em7028_bus)) // 非应答处理
    {
         i2c_soft_stop(&em7028_bus);
         return 1;
    }

    i2c_soft_send_byte(&em7028_bus, reg_addr);
    if(i2c_soft_wait_ack(&em7028_bus)) // 非应答处理
    {
         i2c_soft_stop(&em7028_bus);
         return 1;
    }

    i2c_soft_start(&em7028_bus); // 重复起始条件
    i2c_soft_send_byte(&em7028_bus, (EM7028_ADDR << 1) | 0x01); // 读操作，地址最后一位为1
    if(i2c_soft_wait_ack(&em7028_bus)) // 非应答处理
    {
         i2c_soft_stop(&em7028_bus);
         return 1;
    }

    data = i2c_soft_read_byte(&em7028_bus);
    i2c_soft_send_not_ack(&em7028_bus);

    i2c_soft_stop(&em7028_bus);
    return data;

}
   

uint8_t em7028_get_id()
{
    return em7028_read_reg(ID_REG);
}

uint8_t em7028_hrs_init()
{
    uint8_t retry = 5;
    i2c_init(&em7028_bus);

    while(em7028_get_id()!= EM7028_ID && retry)
    {
        retry--;
        delay_ms(100);
    }

    if(retry == 0)
    {
        return 1; // 设备ID不匹配，初始化失败
    }

    em7028_write_reg(HRS_CFG, 0x00);
    // 关闭心率传感器，进入配置模式
    em7028_write_reg(HRS2_DATA_OFFSET, 0x00); 
    // 心率传感器2数据不偏移
    em7028_write_reg(HRS2_GAIN_CTRL, 0x7f);
    //HRS2 GAIN = 1
    em7028_write_reg(HRS1_CTRL, 0x47); 
    //HRS1 GAIN =1, HRS1 RANGE =8, HRS1 FREQ = 2.62144MHz (1.5625ms), HRS1 RES = 16 bits, HRS1 mode
    em7028_write_reg(INT_CTRL, 0x00); 
    // 关闭中断,  LED programmed current = 2.5mA
    return 0; // 初始化成功
}

uint8_t em7028_hrs_enable()
{
    uint8_t retry = 5;
    while(em7028_get_id() != EM7028_ID && retry)
    {
        retry--;
        delay_ms(100);
    }
    if (retry == 0)
    {   
        return 1; // 设备ID不匹配，无法启用心率传感器
    }

    em7028_write_reg(HRS_CFG, 0x08); 
    // 开启 HRS1心率测量， LED1亮起，仅红光传感器和红外传感器处于测量状态
    // 当LED1亮起时，检测结果存放在HRS_DATA0中

    return 0; // 启用成功
}

uint8_t em7028_hrs_disEnable()
{
    uint8_t retry = 5;
    while(em7028_get_id() != EM7028_ID && retry)
    {
        retry--;
        delay_ms(100);
    }
    if (retry == 0)
    {   
        return 1; // 设备ID不匹配，无法禁用心率传感器
    }

    em7028_write_reg(HRS_CFG, 0x00); // 关闭心率传感器
    
    return 0; // 禁用成功
}


uint16_t em7028_get_hrs1()
{
    uint16_t hr_data;
    
    uint8_t data0_h = em7028_read_reg(HRS1_DATA0_H);
    uint8_t data0_l = em7028_read_reg(HRS1_DATA0_L);

    hr_data = ((uint16_t)data0_h << 8) | data0_l; 

    return hr_data;
}