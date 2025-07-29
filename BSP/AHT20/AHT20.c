#include "AHT20.h"
#include "i2c_hal.h"
#include "delay.h"


#define AHT20_CMD_READ 0x71
#define AHT20_CMD_WRITE 0x70
#define AHT20_CMD_INIT 0xBE
#define AHT20_CMD_MEASURE 0xAC
#define AHT20_CMD_SOFT_RESET 0xBA


i2c_bus_t AHT20_bus = 
{
	.I2C_SDA_PORT = GPIOB,
	.I2C_SCL_PORT = GPIOB,
  .I2C_SDA_PIN =  GPIO_PIN_13,
	.I2C_SCL_PIN =  GPIO_PIN_14
};

static uint8_t AHT20_read_status(void)
{
	uint8_t status;
	
	i2c_soft_start(&AHT20_bus);
	
	i2c_soft_send_byte(&AHT20_bus,AHT20_CMD_READ);
	i2c_soft_wait_ack(&AHT20_bus);
	status = i2c_soft_read_byte(&AHT20_bus);
  i2c_soft_send_not_ack(&AHT20_bus);
	
	i2c_soft_stop(&AHT20_bus);
	return status;
}

void AHT20_soft_reset(void)
{
	i2c_soft_start(&AHT20_bus);
	i2c_soft_send_byte(&AHT20_bus,AHT20_CMD_WRITE);
	i2c_soft_wait_ack(&AHT20_bus);
	i2c_soft_send_byte(&AHT20_bus,AHT20_CMD_SOFT_RESET);
	i2c_soft_wait_ack(&AHT20_bus);
	i2c_soft_stop(&AHT20_bus);
}

uint8_t AHT20_init(void)
{
	i2c_init(&AHT20_bus);
	
	delay_ms(40);
	
	if((AHT20_read_status() & 0x08) != 1) //status的bit[3]不为1
	{
			
		i2c_soft_start(&AHT20_bus);
		i2c_soft_send_byte(&AHT20_bus,AHT20_CMD_WRITE);
		i2c_soft_wait_ack(&AHT20_bus);
		i2c_soft_send_byte(&AHT20_bus,AHT20_CMD_INIT);
		i2c_soft_wait_ack(&AHT20_bus);
		i2c_soft_send_byte(&AHT20_bus,0x08);
		i2c_soft_wait_ack(&AHT20_bus);
		i2c_soft_send_byte(&AHT20_bus,0x00);
		i2c_soft_wait_ack(&AHT20_bus);
		i2c_soft_stop(&AHT20_bus);
	}
	delay_ms(10);
	
	return 0;
}

uint8_t AHT20_read(float* temperature,float* humidity)
{
    uint8_t timeout_cnt = 10;
		uint8_t sensor_data[6];
		uint32_t humidity_raw_data;
	  uint32_t temperature_raw_data;
	
		i2c_soft_start(&AHT20_bus);
		i2c_soft_send_byte(&AHT20_bus,AHT20_CMD_WRITE);
		i2c_soft_wait_ack(&AHT20_bus);
		i2c_soft_send_byte(&AHT20_bus,AHT20_CMD_MEASURE);
		i2c_soft_wait_ack(&AHT20_bus);
		i2c_soft_send_byte(&AHT20_bus,0x33);
		i2c_soft_wait_ack(&AHT20_bus);
		i2c_soft_send_byte(&AHT20_bus,0x00);
		i2c_soft_wait_ack(&AHT20_bus);
	  delay_ms(80);
	
		while((AHT20_read_status() & 0x80) && timeout_cnt) //等待设备测量完成
		{
			delay_ms(5);
			timeout_cnt--;
			if(!timeout_cnt)// 超时等待
					return 1;
		}
		
		i2c_soft_start(&AHT20_bus);
		i2c_soft_send_byte(&AHT20_bus,AHT20_CMD_READ);
		i2c_soft_wait_ack(&AHT20_bus);
		
		sensor_data[0] = i2c_soft_read_byte(&AHT20_bus);
		i2c_soft_send_ack(&AHT20_bus);
		sensor_data[1] = i2c_soft_read_byte(&AHT20_bus);
		i2c_soft_send_ack(&AHT20_bus);
		sensor_data[2] = i2c_soft_read_byte(&AHT20_bus);
		i2c_soft_send_ack(&AHT20_bus);
		sensor_data[3] = i2c_soft_read_byte(&AHT20_bus);
		i2c_soft_send_ack(&AHT20_bus);
		sensor_data[4] = i2c_soft_read_byte(&AHT20_bus);
		i2c_soft_send_ack(&AHT20_bus);
		sensor_data[5] = i2c_soft_read_byte(&AHT20_bus);
		i2c_soft_send_not_ack(&AHT20_bus); // 我这里不需要CRC校验，所以直接发送NACK
		i2c_soft_stop(&AHT20_bus);
		 
		humidity_raw_data = ((uint32_t)sensor_data[1] << 12) + ((uint32_t)sensor_data[2] << 4) + ((uint32_t)sensor_data[3] >> 4);
		*humidity =  (humidity_raw_data * 100.0) / (1 << 20) ;
		
		temperature_raw_data =  (((uint32_t)sensor_data[3] & 0x0F) << 16 ) + ((uint32_t)sensor_data[4] << 8) + (uint32_t)sensor_data[5];
		*temperature = (temperature_raw_data * 200.0) / (1 << 20) -50;
		
		return 0;
}



	

