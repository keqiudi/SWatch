

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


static uint8_t mpu6050_write_reg(uint8_t reg_addr,uint8_t data)
{
	
	i2c_soft_start(&mpu6050_bus);
	
	i2c_soft_send_byte(&mpu6050_bus,MPU6050_I2C_ADDRESS_WRITE);
	if(i2c_soft_wait_ack(&mpu6050_bus)) // 非应答处理
	{
		 i2c_soft_stop(&mpu6050_bus);
		 return 1;
	}
	
	i2c_soft_send_byte(&mpu6050_bus,reg_addr);
	if(i2c_soft_wait_ack(&mpu6050_bus)) // 非应答处理
	{
		 i2c_soft_stop(&mpu6050_bus);
		 return 1;
	}
	
	i2c_soft_send_byte(&mpu6050_bus,data);
	if(i2c_soft_wait_ack(&mpu6050_bus)) // 非应答处理
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
	


static void mpu6050_i2c_pin_init()
{
	i2c_init(&mpu6050_bus);
}


static void mpu6050_int_pin_init()
{
	// 时钟使能
  __HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	// 引脚配置
	GPIO_InitStruct.Pin = MPU6050_INT_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; //配置引脚为，外部中断模式，下降沿触发
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	// GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // 输入不用Speed字段无效
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); //HAL库在内部自动完成EXTI相关配置，不用手动配置EXTI
	
	// NVIC配置
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,0); //中断优先级
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); //PB12, 对应中断线在EXTI15_10
}

static void mpu6050_motion_init()
{
	mpu6050_write_reg(MPU6050_REG_MOT_THR,0x01); // 设置检测阈值, 大于1LSB的加速度变化才算运动
	mpu6050_write_reg(MPU6050_REG_MOT_DUR,0x01); // 设置运动检测持续时间为1ms, 大于1ms认为运动了
}


uint8_t mpu6050_reset()
{
	uint8_t res = 0;
	res = mpu6050_write_reg(MPU6050_REG_PWR_MGMT_1,0x80);// bit7置1，复位MPU6050,重置所有寄存器为默认值
	if(res == 1)
	{
		return res;
	}
	delay_ms(100); // 手册要求延时100ms,等待稳定
	
	return res;
}

uint8_t mpu6050_init()
{
	// mpu6050的I2C相关引脚初始化
	mpu6050_i2c_pin_init(); 
	
	//bsp_mpu6050_reset();
	
	// 默认上电睡眠模式，先唤醒设备
	mpu6050_write_reg(MPU6050_REG_PWR_MGMT_1,0x00);
	
	// 设置稳定时钟源, 选择手册建议的x轴陀螺仪参考的信号通过PLL倍频提供时钟
	mpu6050_write_reg(MPU6050_REG_PWR_MGMT_1,0x01);
	
	// 配置采样率50Hz
	mpu6050_set_sample_rate(50);
	// CONFIG寄存器配置：
	// 禁用外部同步功能，数字低通滤波器配置：DLPF_CFG = bits 2:0 = 100
  // 对应加速度计(Accelerometer)和陀螺仪(Gyroscope)的带宽分别为21Hz和20Hz，频率都为1Khz, 最大延时8.5ms
	mpu6050_set_dlpf(DLPF_CFG_4); 
	
	//配置陀螺仪满量程为：±2000 °/s
	mpu6050_set_gyro_full_scale(FS_SEL_2000);
	//配置加速度计满量程：±8g
	mpu6050_set_accel_full_scale(AFS_SEL_8G);
	
//	uint8_t res = mpu6050_get_id();
//	if(res == MPU6050_I2C_ADDRESS)
//	{
//		 
//	}
	
	//配置中断相关
	//mpu6050_write_reg(MPU6050_REG_INT_ENABLE,0x40); //运动中断检测使能
	//mpu6050_write_reg(MPU6050_REG_INT_PIN_CFG,0x90); // 设置INT引脚低电平有效，任何读操作即可清楚中INT_STATUS位
	//mpu6050_int_pin_init(); // mpu6050的INT引脚初始化
	
	// mpu6050运动检测相关初始化
	//mpu6050_motion_init(); 
	
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

	// 我的mpu使用的是+-2000°/s满量程时，陀螺仪的灵敏度为16.4 LSB/(°/s)，将原始数据转换为实际的角速度（单位：°/s） 
	gyro_x = (float)gx / 16.4;
	gyro_y = (float)gy / 16.4;
	gyro_z = (float)gz / 16.4;

	// 通过陀螺仪的测量结果计算欧拉角, mpu6050最大采样率是200Hz, 也就是每5ms采样一次, 这里积分计算角度变化
	*yaw = *yaw + gyro_z * 0.005; 
	*pitch = *pitch + gyro_x * 0.005; 
	*roll = *roll + gyro_y * 0.005; 

	return 0;	
}

uint8_t mpu6050_accel_get_angles(float* yaw,float* pitch,float* roll)
{
	int16_t ax,ay,az;
	
	mpu6050_read_accel(&ax,&ay,&az);
	
	// 通过加速度计的测量结果计算欧拉角
	*yaw = 0.0f; // 由于加速度计无法测量偏航角，这里暂时设置为0
	*pitch  = atan2(ay,az) / 3.1415927f * 180; // 将弧度转换为角度
	*roll = atan2(ax,az) / 3.1415927f * 180; // 将弧度转换为角度
	
	return 0;
}


uint8_t mpu6050_is_horizontal()
{
	float yaw,pitch,roll;
	mpu6050_accel_get_angles(&yaw,&pitch,&roll);

	if(pitch<=3.0f && pitch >= -3.0f && roll <= 3.0f && roll >= -3.0f)
	{
		return 1; // 水平
	}
	else
	{
		return 0; // 非水平
	}
}

