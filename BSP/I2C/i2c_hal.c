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


/*ֱ�Ӳ����Ĵ���������׼����ʱ��*/
static void SDA_Output(i2c_bus_t* sensor, uint16_t bit_value)
{
		if(bit_value)  
		{
			sensor->I2C_SDA_PORT->BSRR = sensor->I2C_SDA_PIN;  //BSRR��16bitΪ���ö�Ӧ��ODRyλ(IO��ƽ)��1�������ã�0����Ӱ�죬������ֲ�Ĵ�������
		}
		else
		{
			sensor->I2C_SDA_PORT->BSRR = (uint32_t)sensor->I2C_SDA_PIN << 16U; //BSRRǰ16bitΪ�����Ӧ��ODRyλ(IO��ƽ)��1���������0����Ӱ��
		}
}

static void SCL_Output(i2c_bus_t* sensor, uint16_t bit_value)
{
		if(bit_value)  
		{
			sensor->I2C_SCL_PORT->BSRR = sensor->I2C_SCL_PIN;  //BSRR��16bitΪ���ö�Ӧ��ODRyλ(IO��ƽ)��1�������ã�0����Ӱ�죬������ֲ�Ĵ�������
		}
		else
		{
			sensor->I2C_SCL_PORT->BSRR = (uint32_t)sensor->I2C_SCL_PIN << 16U; //BSRRǰ16bitΪ�����Ӧ��ODRyλ(IO��ƽ)��1���������0����Ӱ��
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
	
	 __HAL_RCC_GPIOB_CLK_ENABLE();//����ʱ��
	
	//SCL��SDA���ų�ʼ��
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = sensor->I2C_SCL_PIN | sensor->I2C_SDA_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; //��©���
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; //50Mhz���Ҽ���
	HAL_GPIO_Init(sensor->I2C_SCL_PORT,&GPIO_InitStruct);
	
}


void i2c_soft_start(i2c_bus_t* sensor)
{
	/*��֤scl��sda���Ǹߵ�ƽ���ܹ������½���*/
	SDA_Output(sensor,1); //���������Ӧ��������SDA���ܹ������ظ���ʼ����������SCL���ڸߵ�ƽʱʱ��SDAΪ�ͱ����߲��������أ��ͱ�����Ϊֹͣ�ź�
	SCL_Output(sensor,1);
	delay_us(I2C_DELAY_TIME);//��֤SDAΪ��һ��ʱ��
	
	//��ʼ�źţ�SCLΪ�ߵ�ƽ��SDA�����½���
	SDA_Output(sensor,0);
	delay_us(I2C_DELAY_TIME);
	SCL_Output(sensor,0);
	delay_us(I2C_DELAY_TIME);
}

void i2c_soft_stop(i2c_bus_t* sensor)
{
	/*��֤scl��sda���ǵ͵�ƽ,�ܹ�����������*/
	SCL_Output(sensor,0);
	SDA_Output(sensor,0);
	delay_us(I2C_DELAY_TIME); // �ȶ�һ��ʱ��
	
	//�����źţ�SCL�����ߣ��ٽ�SDA���߲���������
	SCL_Output(sensor,1);
	delay_us(I2C_DELAY_TIME);
	SDA_Output(sensor,1);
	delay_us(I2C_DELAY_TIME);
}


void i2c_soft_send_byte(i2c_bus_t* sensor,uint8_t data)
{
	
//	  SCL_Output(sensor,0);
//	  delay_us(I2C_DELAY_TIME);  ����ȷ��SCLΪ�ͣ���ʵ��������SCL���������ƣ�ÿ�ν�����SCL���������ˣ�����SCL��Ϊ�ͣ����Կ���ʡ�ԣ�����������Լ���
	
	 for(uint8_t i=0;i<8;i++)
	 {
			SDA_Output(sensor, data & (0x80 >> i)); // ���η���data��ÿһλ
		  delay_us(I2C_DELAY_TIME);
		 
		  SCL_Output(sensor,1);
		 	delay_us(I2C_DELAY_TIME);
		 	SCL_Output(sensor,0); // ����׼���´η���
			delay_us(I2C_DELAY_TIME);
	 }
	 	
}


uint8_t i2c_soft_read_byte(i2c_bus_t* sensor)
{
		uint8_t byte = 0x00;
	
		SDA_Output(sensor,1); // �����ͷ�SDA,�����߸�׼ȷ��˵������SDAΪ����/���裩����������SDA�ߡ�����SDA�����ɴӻ�������ƽ������������ȷ�����ӻ����������ݡ�
	                              // ��Ӧ�л�Ϊ����ģʽǰ��Ҫ�Ƚ�IO����ߵ�ƽ������IO�����ڵ͵�ƽ
	  for(uint8_t i=0;i<8;i++)
		{
			 SCL_Output(sensor,0);
			 delay_us(I2C_DELAY_TIME);
			
			 SCL_Output(sensor,1);//SCL�ߵ�ƽ������ȡ
			 if(SDA_Input(sensor))
			 {
				 byte |= (0x80 >> i);
			 }
			 
			 SCL_Output(sensor,0);
			 delay_us(I2C_DELAY_TIME);
		}
		
		return byte;
}

/*���i2c��ȡ����ֽ�*/

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
//	delay_us(I2C_DELAY_TIME); ����ȷ��SCLΪ�ͣ���ʵ��������SCL���������ƣ�ÿ�ν�����SCL���������ˣ�����SCL��Ϊ�ͣ����Կ���ʡ�ԣ�����������Լ���
	
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
	//	delay_us(I2C_DELAY_TIME); ����ȷ��SCLΪ�ͣ���ʵ��������SCL���������ƣ�ÿ�ν�����SCL���������ˣ�����SCL��Ϊ�ͣ����Կ���ʡ�ԣ�����������Լ���
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
	
		SDA_Output(sensor,1); // �����ͷ�SDA,�����߸�׼ȷ��˵������SDAΪ����/���裩����������SDA�ߡ�����SDA�����ɴӻ�������ƽ������������ȷ�����ӻ����������ݡ�
		delay_us(I2C_DELAY_TIME);
	
		SCL_Output(sensor,1);
		delay_us(I2C_DELAY_TIME);
	  
	  ack = SDA_Input(sensor);
	
		SCL_Output(sensor,0);
		delay_us(I2C_DELAY_TIME);
		
		return ack;
}
