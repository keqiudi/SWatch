
#include "bsp_mpu6050.h"                // Device header
#include "delay.h"
#include "i2c_hal.h"
#include "math.h"

i2c_bus_t mpu6050_bus = {
	
	.I2C_SCL_PORT = MPU6050_SCL_PORT,
	.I2C_SDA_PORT = MPU6050_SDA_PORT,
  	.I2C_SDA_PIN =  MPU6050_SDA_PIN,
	.I2C_SCL_PIN =  MPU6050_SCL_PIN
};

uint8_t mpu6050_write_len(uint8_t addr,uint8_t reg_addr,uint8_t len,uint8_t* buf)
{
	uint8_t i;
	i2c_soft_start(&mpu6050_bus);
	i2c_soft_send_byte(&mpu6050_bus,(addr << 1)|0); //鍐欏叆鍦板潃
	if(i2c_soft_wait_ack(&mpu6050_bus)) // 闈炲簲绛斿�勭悊
	{
		 i2c_soft_stop(&mpu6050_bus);
		 return 1;
	}
	i2c_soft_send_byte(&mpu6050_bus,reg_addr);
	if(i2c_soft_wait_ack(&mpu6050_bus)) // 闈炲簲绛斿�勭悊
	{
		 i2c_soft_stop(&mpu6050_bus);
		 return 1;
	}
	for(i=0;i<len;i++)
	{
		i2c_soft_send_byte(&mpu6050_bus,buf[i]);
		if(i2c_soft_wait_ack(&mpu6050_bus)) // 闈炲簲绛斿�勭悊
		{
			 i2c_soft_stop(&mpu6050_bus);
			 return 1;
		}
	}
	i2c_soft_stop(&mpu6050_bus);
	return 0;
}

uint8_t mpu6050_read_len(uint8_t addr,uint8_t reg_addr,uint8_t len,uint8_t* buf)
{
	i2c_soft_start(&mpu6050_bus);
	i2c_soft_send_byte(&mpu6050_bus,(addr << 1)|0); //鍐欏叆鍦板潃
	if(i2c_soft_wait_ack(&mpu6050_bus)) // 闈炲簲绛斿�勭悊
	{
		 i2c_soft_stop(&mpu6050_bus);
		 return 1;
	}
	i2c_soft_send_byte(&mpu6050_bus,reg_addr);
	if(i2c_soft_wait_ack(&mpu6050_bus)) // 闈炲簲绛斿�勭悊
	{
		 i2c_soft_stop(&mpu6050_bus);
		 return 1;
	}    
	
	i2c_soft_start(&mpu6050_bus);
	i2c_soft_send_byte(&mpu6050_bus,(addr << 1)|1); //璇绘搷浣滐紝鍦板潃鏈€鍚庝竴浣嶄负1
	if(i2c_soft_wait_ack(&mpu6050_bus)) // 闈炲簲绛斿�勭悊
	{
		 i2c_soft_stop(&mpu6050_bus);
		 return 1;
	}
	while(len)
	{
		
		*buf = i2c_soft_read_byte(&mpu6050_bus);
		if(len > 1)
		{
			i2c_soft_send_ack(&mpu6050_bus);
		}
		else
		{
			i2c_soft_send_not_ack(&mpu6050_bus);
		}
			
		len--;
		buf++;
	}

	i2c_soft_stop(&mpu6050_bus);
	return 0;
}




static uint8_t mpu6050_write_reg(uint8_t reg_addr,uint8_t data)
{
	
	i2c_soft_start(&mpu6050_bus);
	
	i2c_soft_send_byte(&mpu6050_bus,MPU6050_I2C_ADDRESS_WRITE);
	if(i2c_soft_wait_ack(&mpu6050_bus)) // 锟斤拷应锟斤拷锟斤拷
	{
		 i2c_soft_stop(&mpu6050_bus);
		 return 1;
	}
	
	i2c_soft_send_byte(&mpu6050_bus,reg_addr);
	if(i2c_soft_wait_ack(&mpu6050_bus)) // 锟斤拷应锟斤拷锟斤拷
	{
		 i2c_soft_stop(&mpu6050_bus);
		 return 1;
	}
	
	i2c_soft_send_byte(&mpu6050_bus,data);
	if(i2c_soft_wait_ack(&mpu6050_bus)) // 锟斤拷应锟斤拷锟斤拷
	{
		 i2c_soft_stop(&mpu6050_bus);
		 return 1;
	}
	
	i2c_soft_stop(&mpu6050_bus);
	return 0;
}


static uint8_t mpu6050_read_reg(uint8_t reg_addr)
{
	uint8_t data = 0;
	
	i2c_soft_start(&mpu6050_bus);
	
	i2c_soft_send_byte(&mpu6050_bus,MPU6050_I2C_ADDRESS_WRITE);
	i2c_soft_wait_ack(&mpu6050_bus);
	i2c_soft_send_byte(&mpu6050_bus,reg_addr);
	i2c_soft_wait_ack(&mpu6050_bus);
	
	i2c_soft_start(&mpu6050_bus);
	i2c_soft_send_byte(&mpu6050_bus,MPU6050_I2C_ADDRESS_READ);
	i2c_soft_wait_ack(&mpu6050_bus);
	data = i2c_soft_read_byte(&mpu6050_bus);
	i2c_soft_send_not_ack(&mpu6050_bus);
	
	i2c_soft_stop(&mpu6050_bus);
	
	return data;
}

static uint8_t mpu6050_read(uint8_t reg_addr,uint8_t* buffer,uint8_t len)
{
	i2c_soft_start(&mpu6050_bus);
	
	i2c_soft_send_byte(&mpu6050_bus,MPU6050_I2C_ADDRESS_WRITE);
	i2c_soft_wait_ack(&mpu6050_bus);
	i2c_soft_send_byte(&mpu6050_bus,reg_addr);
	i2c_soft_wait_ack(&mpu6050_bus);
	
	i2c_soft_start(&mpu6050_bus);
	i2c_soft_send_byte(&mpu6050_bus,MPU6050_I2C_ADDRESS_READ);
	i2c_soft_wait_ack(&mpu6050_bus);
	
	while(len)
	{
		*buffer = i2c_soft_read_byte(&mpu6050_bus);
		
		if(len > 1)
		{
			i2c_soft_send_ack(&mpu6050_bus);
		}
		else
		{
			i2c_soft_send_not_ack(&mpu6050_bus);
		}
			
		len--;
		buffer++;
	}
	
	i2c_soft_stop(&mpu6050_bus);
	return 0;
}
	


void mpu6050_i2c_pin_init()
{
	i2c_init(&mpu6050_bus);
}


static void mpu6050_int_pin_init()
{
	// 时锟斤拷使锟斤拷
  __HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	// 锟斤拷锟斤拷锟斤拷锟斤拷
	GPIO_InitStruct.Pin = MPU6050_INT_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; //锟斤拷锟斤拷锟斤拷锟斤拷为锟斤拷锟解部锟叫讹拷模式锟斤拷锟铰斤拷锟截达拷锟斤拷
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	// GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // 锟斤拷锟诫不锟斤拷Speed锟街讹拷锟斤拷效
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); //HAL锟斤拷锟斤拷锟节诧拷锟皆讹拷锟斤拷锟紼XTI锟斤拷锟斤拷锟斤拷茫锟斤拷锟斤拷锟斤拷侄锟斤拷锟斤拷锟紼XTI
	
	// NVIC锟斤拷锟斤拷
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,0); //锟叫讹拷锟斤拷锟饺硷拷
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); //PB12, 锟斤拷应锟叫讹拷锟斤拷锟斤拷EXTI15_10
}

static void mpu6050_motion_init()
{
	mpu6050_write_reg(MPU6050_REG_MOT_THR,0x01); // 锟斤拷锟矫硷拷锟斤拷锟街�, 锟斤拷锟斤拷1LSB锟侥硷拷锟劫度变化锟斤拷锟斤拷锟剿讹拷
	mpu6050_write_reg(MPU6050_REG_MOT_DUR,0x01); // 锟斤拷锟斤拷锟剿讹拷锟斤拷锟斤拷锟斤拷时锟斤拷为1ms, 锟斤拷锟斤拷1ms锟斤拷为锟剿讹拷锟斤拷
}


uint8_t mpu6050_reset()
{
	uint8_t res = 0;
	res = mpu6050_write_reg(MPU6050_REG_PWR_MGMT_1,0x80);// bit7锟斤拷1锟斤拷锟斤拷位MPU6050,锟斤拷锟斤拷锟斤拷锟叫寄达拷锟斤拷为默锟斤拷值
	if(res == 1)
	{
		return res;
	}
	delay_ms(100); // 锟街诧拷要锟斤拷锟斤拷时100ms,锟饺达拷锟饺讹拷
	
	return res;
}

uint8_t mpu6050_init()
{
	// mpu6050锟斤拷I2C锟斤拷锟斤拷锟斤拷懦锟绞硷拷锟�
	mpu6050_i2c_pin_init(); 
	
	//bsp_mpu6050_reset();
	
	// 默锟斤拷锟较碉拷睡锟斤拷模式锟斤拷锟饺伙拷锟斤拷锟借备
	mpu6050_write_reg(MPU6050_REG_PWR_MGMT_1,0x00);
	
	// 锟斤拷锟斤拷锟饺讹拷时锟斤拷源, 选锟斤拷锟街册建锟斤拷锟絰锟斤拷锟斤拷锟斤拷锟角参匡拷锟斤拷锟脚猴拷通锟斤拷PLL锟斤拷频锟结供时锟斤拷
	mpu6050_write_reg(MPU6050_REG_PWR_MGMT_1,0x01);
	
	// 锟斤拷锟矫诧拷锟斤拷锟斤拷50Hz
	mpu6050_set_sample_rate(50);
	// CONFIG锟侥达拷锟斤拷锟斤拷锟矫ｏ拷
	// 锟斤拷锟斤拷锟解部同锟斤拷锟斤拷锟杰ｏ拷锟斤拷锟街碉拷通锟剿诧拷锟斤拷锟斤拷锟矫ｏ拷DLPF_CFG = bits 2:0 = 100
  // 锟斤拷应锟斤拷锟劫度硷拷(Accelerometer)锟斤拷锟斤拷锟斤拷锟斤拷(Gyroscope)锟侥达拷锟斤拷锟街憋拷为21Hz锟斤拷20Hz锟斤拷频锟绞讹拷为1Khz, 锟斤拷锟斤拷锟绞�8.5ms
	mpu6050_set_dlpf(DLPF_CFG_4); 
	
	//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷为锟斤拷锟斤拷2000 锟斤拷/s
	mpu6050_set_gyro_full_scale(FS_SEL_2000);
	//锟斤拷锟矫硷拷锟劫度硷拷锟斤拷锟斤拷锟教ｏ拷锟斤拷8g
	mpu6050_set_accel_full_scale(AFS_SEL_8G);
	
	uint8_t res = mpu6050_get_id();
	if(res == MPU6050_I2C_ADDRESS)
	{
		 
	}
	
	//锟斤拷锟斤拷锟叫讹拷锟斤拷锟�
	mpu6050_write_reg(MPU6050_REG_INT_ENABLE,0x40); //锟剿讹拷锟叫断硷拷锟绞癸拷锟�
	mpu6050_write_reg(MPU6050_REG_INT_PIN_CFG,0x90); // 锟斤拷锟斤拷INT锟斤拷锟脚低碉拷平锟斤拷效锟斤拷锟轿何讹拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟絀NT_STATUS位
	mpu6050_int_pin_init(); // mpu6050锟斤拷INT锟斤拷锟脚筹拷始锟斤拷
	
	// mpu6050锟剿讹拷锟斤拷锟斤拷锟截筹拷始锟斤拷
	mpu6050_motion_init(); 
	
	return 0;
}


uint8_t mpu6050_set_sample_rate(uint16_t rate)
{
	uint8_t res;
	uint8_t div_value = 0; 
	
	if(rate > 1000)
	{
			rate = 1000;
	}
	else if(rate < 4)
	{
			rate = 4;
	}
	div_value = 1000 / (1 + div_value);
	
	res = mpu6050_write_reg(MPU6050_REG_SMPLRT_DIV,div_value);
	if(res == 1)
	{
		 return res;
	}
	
	return res;
}

uint8_t mpu6050_set_dlpf(uint16_t dlpf_config)
{
	 uint8_t res = 0;
	
	 if(dlpf_config < 0)
	 {
		  dlpf_config = 0;
	 }
	 else if(dlpf_config > 6)
	 {
		  dlpf_config = 6;
	 }
		 
	 res = mpu6050_write_reg(MPU6050_REG_CONFIG,dlpf_config);
	 if(res == 1)
	 {
		 return res;
	 }
	 
	 return res;
}

uint8_t mpu6050_set_gyro_full_scale(uint8_t fs_sel)
{
		uint8_t res = 0;
	
	  if(fs_sel < 0 || fs_sel > 3)
		{
			fs_sel = 0;
		}
		
		res = mpu6050_write_reg(MPU6050_REG_GYRO_CONFIG,(fs_sel<<3));
		if(res == 1)
		{
			return res;
		}
		
		return res;
			
}

uint8_t mpu6050_set_accel_full_scale(uint8_t afs_sel)
{
		uint8_t res = 0;
	
	  if(afs_sel< 0 || afs_sel > 3)
		{
			afs_sel = 0;
		}
		
		res = mpu6050_write_reg(MPU6050_REG_ACCEL_CONFIG,(afs_sel << 3));
		if(res == 1)
		{
			return res;
		}
		
		return res;
			
}

uint8_t mpu6050_read_gyro(int16_t* gx,int16_t* gy,int16_t* gz)
{
	 uint8_t res = 1;
	 uint8_t raw_data[6];
	 res = mpu6050_read(MPU6050_REG_GYRO_XOUT_H,raw_data,6);
	 if(res == 0)
	 {
		 *gx = (int16_t)(((int16_t)raw_data[0] << 8) | raw_data[1]);
		 *gy = (int16_t)(((int16_t)raw_data[2] << 8) | raw_data[3]);
		 *gz = (int16_t)(((int16_t)raw_data[4] << 8) | raw_data[5]);
	 }
	
	 return res;
}

uint8_t mpu6050_read_accel(int16_t* ax,int16_t* ay,int16_t* az)
{
	 uint8_t res = 1;
	 uint8_t raw_data[6];
	 res = mpu6050_read(MPU6050_REG_ACCEL_XOUT_H,raw_data,6);
	 if(res == 0)
	 {
		 *ax = (int16_t)(((int16_t)raw_data[0] << 8) | raw_data[1]);
		 *ay = (int16_t)(((int16_t)raw_data[2] << 8) | raw_data[3]);
		 *az = (int16_t)(((int16_t)raw_data[4] << 8) | raw_data[5]);
	 }
	
	 return res;
	
}


uint8_t mpu6050_read_temperature(float* temperature)
{
	uint8_t res = 1;
	uint8_t raw_data[2];
	int16_t raw_temp;
	res = mpu6050_read(MPU6050_REG_TEMP_OUT_H ,raw_data,2);
	if(res == 0)
	{
		raw_temp = (int16_t)(((int16_t)raw_data[0] << 8) | raw_data[1]);
		* temperature =  raw_temp / 340 + 36.53;
	}
}


uint8_t mpu6050_get_id()
{
	return mpu6050_read_reg(MPU6050_REG_WHO_AM_I);
}

uint8_t mpu6050_gyro_get_angles(float* yaw,float* pitch,float* roll)
{
	int16_t gx,gy,gz;
	float gyro_x, gyro_y, gyro_z;

	mpu6050_read_gyro(&gx,&gy,&gz);

	// 锟揭碉拷mpu使锟矫碉拷锟斤拷+-2000锟斤拷/s锟斤拷锟斤拷锟斤拷时锟斤拷锟斤拷锟斤拷锟角碉拷锟斤拷锟斤拷锟斤拷为16.4 LSB/(锟斤拷/s)锟斤拷锟斤拷原始锟斤拷锟斤拷转锟斤拷为实锟绞的斤拷锟劫度ｏ拷锟斤拷位锟斤拷锟斤拷/s锟斤拷 
	gyro_x = (float)gx / 16.4;
	gyro_y = (float)gy / 16.4;
	gyro_z = (float)gz / 16.4;

	// 通锟斤拷锟斤拷锟斤拷锟角的诧拷锟斤拷锟斤拷锟斤拷锟斤拷锟脚凤拷锟斤拷锟�, mpu6050锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷200Hz, 也锟斤拷锟斤拷每5ms锟斤拷锟斤拷一锟斤拷, 锟斤拷锟斤拷锟斤拷旨锟斤拷锟角度变化
	*yaw = *yaw + gyro_z * 0.005; 
	*pitch = *pitch + gyro_x * 0.005; 
	*roll = *roll + gyro_y * 0.005; 

	return 0;	
}

uint8_t mpu6050_accel_get_angles(float* yaw,float* pitch,float* roll)
{
	int16_t ax,ay,az;
	
	mpu6050_read_accel(&ax,&ay,&az);
	
	/* 区分正反面：角度在-180°~180间变化，水平时角度可能是0°也可能是-180°(正反面) */
	// *yaw = 0.0f; // 
	// *pitch  = atan2f(ay,az) / 3.1415927f * 180; // 
	// *roll = atan2f(ax,az) / 3.1415927f * 180; // 
    
	/*不区分正反面，限制在-90-90°间，无论正反面使水平时角度都在0°左右，而不是-180°左右*/
	*yaw = 0.0f;
	*pitch = atan2f((float)ay, sqrtf((float)ax * ax + (float)az * az)) * 57.29578f;
	*roll  = atan2f((float)ax, sqrtf((float)ay * ay + (float)az * az)) * 57.29578f;
	
	return 0;
}


uint8_t mpu6050_is_horizontal()
{
	float yaw,pitch,roll;
	mpu6050_accel_get_angles(&yaw,&pitch,&roll); //用加速度计计算角度判断水平状态，因为要低功耗考虑，而手册上显示陀螺仪更耗电
	
	// if((pitch<=3.0f && pitch >= -3.0f) && (roll <=3.0f && roll >= -3.0f))
	// {
	// 	return 1; // 水平
	// }
	// else
	// {
	// 	return 0; // 非水平
	// }

	if((pitch<=7.0f && pitch >= -7.0f) && (roll <=7.0f && roll >= -7.0f))
	{
		return 1; // 水平
	}
	else
	{
		return 0; // 非水平
	}
}

