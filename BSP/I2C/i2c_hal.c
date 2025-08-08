#include "i2c_hal.h"
#include "delay.h"

#define I2C_DELAY_TIME 2

/**
static void SDA_Output(i2c_bus_t* sensor, uint16_t value)
{
	 HAL_GPIO_WritePin(sensor->I2C_SDA_PORT,sensor->I2C_SDA_PIN,value);
}

static void SCL_Output(i2c_bus_t* sensor, uint16_t value)
{
	 HAL_GPIO_WritePin(sensor->I2C_SCL_PORT,sensor->I2C_SCL_PIN,value);
}
 */


/*直接操作寄存器，更精准控制时序*/
static void SDA_Output(i2c_bus_t* sensor, uint16_t bit_value)
{
		if(bit_value)  
		{
			sensor->I2C_SDA_PORT->BSRR = sensor->I2C_SDA_PIN;  //BSRR后16bit为设置对应的ODRy位(IO电平)，1可以设置，0不会影响，具体见手册寄存器描述
		}
		else
		{
			sensor->I2C_SDA_PORT->BSRR = (uint32_t)sensor->I2C_SDA_PIN << 16U; //BSRR前16bit为清除对应的ODRy位(IO电平)，1可以清除，0不会影响
		}
}

static void SCL_Output(i2c_bus_t* sensor, uint16_t bit_value)
{
		if(bit_value)  
		{
			sensor->I2C_SCL_PORT->BSRR = sensor->I2C_SCL_PIN;  //BSRR后16bit为设置对应的ODRy位(IO电平)，1可以设置，0不会影响，具体见手册寄存器描述
		}
		else
		{
			sensor->I2C_SCL_PORT->BSRR = (uint32_t)sensor->I2C_SCL_PIN << 16U; //BSRR前16bit为清除对应的ODRy位(IO电平)，1可以清除，0不会影响
		}
}


static uint8_t SDA_Input(i2c_bus_t* sensor)
{
	uint8_t bit_value = 0;
	
	bit_value = HAL_GPIO_ReadPin(sensor->I2C_SDA_PORT,sensor->I2C_SDA_PIN);
	
	return bit_value;
}

static uint8_t SCL_Input(i2c_bus_t* sensor)
{
	uint8_t bit_value = 0;
	
	bit_value = HAL_GPIO_ReadPin(sensor->I2C_SCL_PORT,sensor->I2C_SCL_PIN);
	
	return bit_value;
}




void i2c_init(i2c_bus_t* sensor)
{
	
	 __HAL_RCC_GPIOB_CLK_ENABLE();//开启时钟
	
	//SCL、SDA引脚初始化
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = sensor->I2C_SCL_PIN | sensor->I2C_SDA_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; //开漏输出
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; //50Mhz左右即可
	HAL_GPIO_Init(sensor->I2C_SCL_PORT,&GPIO_InitStruct);
	
}


void i2c_soft_start(i2c_bus_t* sensor)
{
	/*保证scl和sda都是高电平，能够产生下降沿*/
	SDA_Output(sensor,1); //保险起见，应该先拉高SDA，能够兼容重复起始条件，避免SCL处于高电平时时，SDA为低被拉高产生上升沿，就被误判为停止信号
	SCL_Output(sensor,1);
	delay_us(I2C_DELAY_TIME);//保证SDA为高一段时间
	
	//起始信号，SCL为高电平，SDA产生下降沿
	SDA_Output(sensor,0);
	delay_us(I2C_DELAY_TIME);
	SCL_Output(sensor,0);
	delay_us(I2C_DELAY_TIME);
}

void i2c_soft_stop(i2c_bus_t* sensor)
{
	/*保证scl和sda都是低电平,能够产生上升沿*/
	SCL_Output(sensor,0);
	SDA_Output(sensor,0);
	delay_us(I2C_DELAY_TIME); // 稳定一段时间
	
	//结束信号，SCL先拉高，再将SDA拉高产生上升沿
	SCL_Output(sensor,1);
	delay_us(I2C_DELAY_TIME);
	SDA_Output(sensor,1);
	delay_us(I2C_DELAY_TIME);
}


void i2c_soft_send_byte(i2c_bus_t* sensor,uint8_t data)
{
	
//	  SCL_Output(sensor,0);
//	  delay_us(I2C_DELAY_TIME);  这里确保SCL为低，但实际上由于SCL是主机控制，每次结束后SCL都被拉低了，正常SCL都为低，所以可以省略，保险起见可以加上
	
	 for(uint8_t i=0;i<8;i++)
	 {
			SDA_Output(sensor, data & (0x80 >> i)); // 依次发送data的每一位
		  delay_us(I2C_DELAY_TIME);
		 
		  SCL_Output(sensor,1);
		 	delay_us(I2C_DELAY_TIME);
		 	SCL_Output(sensor,0); // 拉低准备下次发送
			delay_us(I2C_DELAY_TIME);
	 }
	 	
}


uint8_t i2c_soft_read_byte(i2c_bus_t* sensor)
{
		uint8_t byte = 0x00;
	
		SDA_Output(sensor,1); // 主机释放SDA,（或者更准确地说，设置SDA为输入/高阻），不再驱动SDA线。这样SDA线上由从机决定电平，主机才能正确采样从机发出的数据。
	                              // 对应切换为输入模式前需要先将IO输出高电平，避免IO锁死在低电平
	  for(uint8_t i=0;i<8;i++)
		{
			 SCL_Output(sensor,0);
			 delay_us(I2C_DELAY_TIME);
			
			 SCL_Output(sensor,1);//SCL高电平主机读取
			 if(SDA_Input(sensor))
			 {
				 byte |= (0x80 >> i);
			 }
			 
			 SCL_Output(sensor,0);
			 delay_us(I2C_DELAY_TIME);
		}
		
		return byte;
}

/*软件i2c读取多个字节*/

uint8_t i2c_soft_read_multi_byte(i2c_bus_t* sensor,uint8_t byte[],int size)
{
	
	for(int i=0;i<size;i++)
	{
		 byte[i] = i2c_soft_read_byte(sensor);
	}
	
}

void i2c_soft_send_ack(i2c_bus_t* sensor)
{
//	SCL_Output(sensor,0);
//	delay_us(I2C_DELAY_TIME); 这里确保SCL为低，但实际上由于SCL是主机控制，每次结束后SCL都被拉低了，正常SCL都为低，所以可以省略，保险起见可以加上
	
	SDA_Output(sensor,0);
	delay_us(I2C_DELAY_TIME);
	
	SCL_Output(sensor,1);
	delay_us(I2C_DELAY_TIME);
	SCL_Output(sensor,0);
	delay_us(I2C_DELAY_TIME);
	
}	

void i2c_soft_send_not_ack(i2c_bus_t* sensor)
{
	//	SCL_Output(sensor,0);
	//	delay_us(I2C_DELAY_TIME); 这里确保SCL为低，但实际上由于SCL是主机控制，每次结束后SCL都被拉低了，正常SCL都为低，所以可以省略，保险起见可以加上
	SDA_Output(sensor,1);
	delay_us(I2C_DELAY_TIME);
	
	SCL_Output(sensor,1);
	delay_us(I2C_DELAY_TIME);
	SCL_Output(sensor,0);
	delay_us(I2C_DELAY_TIME);
	
}	

uint8_t i2c_soft_wait_ack(i2c_bus_t* sensor)
{
		uint8_t ack = 0;
	
		SDA_Output(sensor,1); // 主机释放SDA,（或者更准确地说，设置SDA为输入/高阻），不再驱动SDA线。这样SDA线上由从机决定电平，主机才能正确采样从机发出的数据。
		delay_us(I2C_DELAY_TIME);
	
		SCL_Output(sensor,1);
		delay_us(I2C_DELAY_TIME);
	  
	  ack = SDA_Input(sensor);
	
		SCL_Output(sensor,0);
		delay_us(I2C_DELAY_TIME);
		
		return ack;
}
