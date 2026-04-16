#include "bsp_lsm303dlhc.h"
#include "math.h"
#include "delay.h"

#define PI 3.1415926


i2c_bus_t lsm303dlhc_bus = {
    .I2C_SCL_PORT = LSM303DLHC_SCL_PORT,
    .I2C_SDA_PORT = LSM303DLHC_SDA_PORT,
  	.I2C_SDA_PIN  = LSM303DLHC_SDA_PIN,
    .I2C_SCL_PIN  = LSM303DLHC_SCL_PIN
};

static uint8_t lsm303dlhc_write_reg(uint8_t reg_addr,uint8_t data)
{
    uint8_t slave_addr = (reg_addr > 0x19) ? LSM303_ACCEL_ADDRESS : LSM303_MAG_ADDRESS; // 根据寄存器地址选择加速度计或磁力计的I2C地址

    i2c_soft_start(&lsm303dlhc_bus);
    i2c_soft_send_byte(&lsm303dlhc_bus,slave_addr << 1); //写入地址
    if(i2c_soft_wait_ack(&lsm303dlhc_bus)) // 非应答处理
    {
         i2c_soft_stop(&lsm303dlhc_bus);
         return 1;
    }

    i2c_soft_send_byte(&lsm303dlhc_bus,reg_addr);
    if(i2c_soft_wait_ack(&lsm303dlhc_bus)) // 非应答处理
    {
         i2c_soft_stop(&lsm303dlhc_bus);
         return 1;
    }

    i2c_soft_send_byte(&lsm303dlhc_bus,data);
    if(i2c_soft_wait_ack(&lsm303dlhc_bus)) // 非应答处理
    {
         i2c_soft_stop(&lsm303dlhc_bus);
         return 1;
    }

    i2c_soft_stop(&lsm303dlhc_bus);

    return 0;
}


static uint8_t lsm303dlhc_read_reg(uint8_t reg_addr)
{
    uint8_t slave_addr = (reg_addr > 0x19) ? LSM303_ACCEL_ADDRESS : LSM303_MAG_ADDRESS; // 根据寄存器地址选择加速度计或磁力计的I2C地址
    uint8_t data = 0;

    i2c_soft_start(&lsm303dlhc_bus);
    i2c_soft_send_byte(&lsm303dlhc_bus,slave_addr << 1); //写入地址
    if(i2c_soft_wait_ack(&lsm303dlhc_bus)) // 非应答处理
    {
         i2c_soft_stop(&lsm303dlhc_bus);
         return 1;
    }

    i2c_soft_send_byte(&lsm303dlhc_bus,reg_addr);
    if(i2c_soft_wait_ack(&lsm303dlhc_bus)) // 非应答处理
    {
         i2c_soft_stop(&lsm303dlhc_bus);
         return 1;
    }

    i2c_soft_start(&lsm303dlhc_bus); // 重复起始条件
    i2c_soft_send_byte(&lsm303dlhc_bus,(slave_addr << 1) | 0x01); // 读操作，地址最后一位为1
    if(i2c_soft_wait_ack(&lsm303dlhc_bus)) // 非应答处理
    {
         i2c_soft_stop(&lsm303dlhc_bus);
         return 1;
    }
    
    data = i2c_soft_read_byte(&lsm303dlhc_bus);
    i2c_soft_send_not_ack(&lsm303dlhc_bus);

    i2c_soft_stop(&lsm303dlhc_bus);

    return data;
}

static uint8_t lsm303dlhc_read_multireg(uint8_t reg_addr,uint8_t buffer[],uint8_t len)
{
     uint8_t slave_addr = (reg_addr > 0x19) ? LSM303_ACCEL_ADDRESS : LSM303_MAG_ADDRESS; // 根据寄存器地址选择加速度计或磁力计的I2C地址
     
     i2c_soft_start(&lsm303dlhc_bus);
     i2c_soft_send_byte(&lsm303dlhc_bus,slave_addr << 1); //写入地址
     if(i2c_soft_wait_ack(&lsm303dlhc_bus)) // 非应答处理
     {
           i2c_soft_stop(&lsm303dlhc_bus);
           return 1;
     }
     
     i2c_soft_send_byte(&lsm303dlhc_bus,reg_addr);
     if(i2c_soft_wait_ack(&lsm303dlhc_bus)) // 非应答处理
     {
           i2c_soft_stop(&lsm303dlhc_bus);
           return 1;
     }
     
     i2c_soft_start(&lsm303dlhc_bus); // 重复起始条件
     i2c_soft_send_byte(&lsm303dlhc_bus,(slave_addr << 1) | 0x01); // 读操作，地址最后一位为1
     if(i2c_soft_wait_ack(&lsm303dlhc_bus)) // 非应答处理
     {
           i2c_soft_stop(&lsm303dlhc_bus);
           return 1;
     }
     
     for(uint8_t i=0;i<len;i++)
     {
          buffer[i] = i2c_soft_read_byte(&lsm303dlhc_bus);
          if(i < len - 1)
          {
               i2c_soft_send_ack(&lsm303dlhc_bus);
          }
          else
          {
               i2c_soft_send_not_ack(&lsm303dlhc_bus);
          }
     }
     
     i2c_soft_stop(&lsm303dlhc_bus);
     
     return 0;
}


uint8_t lsm303dlhc_init(void)
{
     uint8_t retry = 0;
     uint8_t default_value = 0;

     i2c_init(&lsm303dlhc_bus);

     // 加速度计初始化
	for(retry = 0;retry < 3;retry ++)
	{
          lsm303dlhc_write_reg(LSM303_CTRL_REG1_A,0x2F); // 10hZ,低功耗模式，xyz三轴使能
          delay_ms(1); 
          lsm303dlhc_write_reg(LSM303_CTRL_REG4_A,0x10); // 满量程 +-4g
          delay_ms(1);
          
          default_value = lsm303dlhc_read_reg(LSM303_CTRL_REG1_A); // 读取0x20寄存器，默认值为0x07
		if(default_value != 0x2F) // 配置失败,进入重试
          {
               delay_ms(10);
          }
          else
          {
               break; // 配置成功，退出重试
          }
     }

     if(default_value != 0x2F)
     {
          return 1; // 加速度计初始化失败
     }

     delay_ms(1);

     // 磁力计初始化
     for(retry = 0;retry < 3;retry ++)
	{
          lsm303dlhc_write_reg(LSM303_CRA_REG_M,0x10); // 磁场传感器15Hz速率，关闭温度传感器
          delay_ms(1);
          lsm303dlhc_write_reg(LSM303_CRB_REG_M,0x80); // 磁场传感器gain设置: +-4.0Gauss, 450LBS/Gauss for XY, 400LBS/Gauss for Z
          delay_ms(1);
          lsm303dlhc_write_reg(LSM303_MR_REG_M, 0x00);		//磁场传感器连续转换模式
          
          default_value = lsm303dlhc_read_reg(LSM303_MR_REG_M);// 读取0x02寄存器，默认值为0x03
          if(default_value != 0x00) // 配置失败,进入重试
          {
               delay_ms(10);
          }
          else
          {
               break; // 配置成功，退出重试
          }
     }

     if(default_value != 0x00)
     {
          return 1; // 磁场传感器初始化失败
     }

    return 0;
}

void lsm303dlhc_sleep(void)
{
     lsm303dlhc_write_reg(LSM303_CTRL_REG1_A,0x0f);// 加速度传感器休眠模式
     delay_ms(1);
     lsm303dlhc_write_reg(LSM303_MR_REG_M,0x03); // 磁场传感器休眠模式
}

void lsm303dlhc_wakeup(void)
{
     lsm303dlhc_write_reg(LSM303_CTRL_REG1_A,0x2F); // 加速度传感器正常模式
     delay_ms(1);
     lsm303dlhc_write_reg(LSM303_MR_REG_M,0x00); // 磁场传感器连续转换模式
}

void lsm303dlhc_read_accel(int16_t* ax, int16_t* ay, int16_t* az)
{
     uint8_t raw_data[6] = {0};
     //lsm303dlhc_read_multireg(LSM303_OUT_X_L_A, raw_data,6);

     raw_data[0] = lsm303dlhc_read_reg(LSM303_OUT_X_L_A);
     raw_data[1] = lsm303dlhc_read_reg(LSM303_OUT_X_H_A);
     raw_data[2] = lsm303dlhc_read_reg(LSM303_OUT_Y_L_A);
     raw_data[3] = lsm303dlhc_read_reg(LSM303_OUT_Y_H_A);
     raw_data[4] = lsm303dlhc_read_reg(LSM303_OUT_Z_L_A);
     raw_data[5] = lsm303dlhc_read_reg(LSM303_OUT_Z_H_A);

     *ax = (int16_t)(((int16_t)raw_data[1] << 8) | raw_data[0]);
     *ay = (int16_t)(((int16_t)raw_data[3] << 8) | raw_data[2]);
     *az = (int16_t)(((int16_t)raw_data[5] << 8) | raw_data[4]);
}

void lsm303dlhc_read_magnetic(int16_t* mx, int16_t* my, int16_t* mz)
{
     uint8_t raw_data[6] = {0};
     //lsm303dlhc_read_multireg(LSM303_OUT_X_H_M, raw_data,6);

     raw_data[0] = lsm303dlhc_read_reg(LSM303_OUT_X_H_M);
     raw_data[1] = lsm303dlhc_read_reg(LSM303_OUT_X_L_M);
     raw_data[2] = lsm303dlhc_read_reg(LSM303_OUT_Z_H_M);
     raw_data[3] = lsm303dlhc_read_reg(LSM303_OUT_Z_L_M);
     raw_data[4] = lsm303dlhc_read_reg(LSM303_OUT_Y_H_M);
     raw_data[5] = lsm303dlhc_read_reg(LSM303_OUT_Y_L_M);

     *mx = (int16_t)(((int16_t)raw_data[0] << 8) | raw_data[1]);
     *mz = (int16_t)(((int16_t)raw_data[2] << 8) | raw_data[3]); // 这里有个坑，磁力计的X轴数据之后是Z轴数据，最后才是Y轴数据
     *my = (int16_t)(((int16_t)raw_data[4] << 8) | raw_data[5]);

}

void lsm303dlhc_read_temperature(float* temperature)
{
     uint8_t raw_data[2] = {0};
     lsm303dlhc_read_multireg(LSM303_TEMP_OUT_H_M , raw_data,2);

     int16_t raw_temp = (int16_t)(((int16_t)raw_data[0] << 8) | raw_data[1]);
     raw_temp >>= 4; // 温度数据是12位的，右移4位得到正确的温度值

     *temperature = raw_temp / 8.0f + 25.0f; // 根据LSM303DLHC的温度计算公式进行转换
}

int lsm303dlhc_calc_z_axis_angle(int16_t ax, int16_t ay, int16_t az)
{
    double A;
    float fx,fy,fz;
	
	A = sqrt((int)ax*ax + (int)ay*ay + (int)az*az);	//计算角加速度的矢量模长 |A|=根号下(X*X+Y*Y+Z*Z)
	fx = ax/A;
	fy = ay/A;
	fz = az/A;
	
	//Z方向
	A = fx*fx+fy*fy;
	A = sqrt(A);
	A = (double)A/fz;
	A = atan(A); 
	A = A*180/PI;
	if(A < 0)
	{
		A += 90;
		A = 0-A;
	}
	else
	{
		A = 90-A;
	}
	return A*100;
}

int lsm303dlhc_calc_x_axis_angle(int16_t ax, int16_t ay, int16_t az)
{
     double A;
	float fx,fy,fz;
	
	A = sqrt((int)ax*ax + (int)ay*ay + (int)az*az);	//计算角加速度的矢量模长 |A|=根号下(X*X+Y*Y+Z*Z)
	fx = ax/A;
	fy = ay/A;
	fz = az/A;
	
	
	//X方向
	A = fz*fz+fy*fy;
	A = sqrt(A);
	
	A = (double)A/fx;
	A = atan(A); 
	A = A*180/PI;
	if(A < 0)
	{
		A += 90; //向上为正
	}
	else
	{
		A = 90-A;
		A = 0-A; //向下为负
	}
	return A*100;
}

// 用加速度计算俯仰/横滚做倾斜补偿，再用磁力计水平分量算方位角
float lsm303dlhc_calc_azimuth_angle(int ax,int ay,int az,int mx,int my,int mz)
{
	float pitch, roll, Hy, Hx, Azimuth; 
	pitch   = atan2f(ax, sqrtf(ay * ay + az * az));
	roll    = atan2f(ay, sqrtf(ax * ax + az * az));
	Hy      = my * cosf(roll) + mx * sinf(roll) * sinf(pitch) - mz * cosf(pitch) * sinf(roll);
	Hx      = mx * cosf(pitch) + mz * sinf(pitch);
	Azimuth = atan2f(Hy,Hx)*180.0/PI;
	return Azimuth;
}

