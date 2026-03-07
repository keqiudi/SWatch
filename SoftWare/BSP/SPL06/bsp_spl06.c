#include <math.h>
#include "bsp_spl06.h"
#include "SEGGER_RTT.h"

int16_t c0,c1,c01,c11,c20,c21,c30;
int32_t c00,c10;  


i2c_bus_t spl06_bus = {
	
	.I2C_SCL_PORT = SPL06_SCL_PORT,
	.I2C_SDA_PORT = SPL06_SDA_PORT,
  	.I2C_SDA_PIN  = SPL06_SDA_PIN,
	.I2C_SCL_PIN  = SPL06_SCL_PIN
};


static uint8_t spl06_write_reg(uint8_t reg_addr,uint8_t data)
{
  i2c_soft_start(&spl06_bus);
	
	i2c_soft_send_byte(&spl06_bus,SPL06_CHIP_ADDRESS << 1); //写入地址
	if(i2c_soft_wait_ack(&spl06_bus)) // 非应答处理
	{
		 i2c_soft_stop(&spl06_bus);
		 return 1;
	}

	
	i2c_soft_send_byte(&spl06_bus,reg_addr);
	if(i2c_soft_wait_ack(&spl06_bus)) // 非应答处理
	{
		 i2c_soft_stop(&spl06_bus);
		 return 1;
	}
	
	i2c_soft_send_byte(&spl06_bus,data);
	if(i2c_soft_wait_ack(&spl06_bus)) // 非应答处理
	{
		 i2c_soft_stop(&spl06_bus);
		 return 1;
	}
	
	i2c_soft_stop(&spl06_bus);
	
	return 0;
}

static uint8_t spl06_read_reg(uint8_t reg_addr)
{

	i2c_soft_start(&spl06_bus);
	
	i2c_soft_send_byte(&spl06_bus,SPL06_CHIP_ADDRESS << 1);
	if(i2c_soft_wait_ack(&spl06_bus)) // 非应答处理
	{
		 i2c_soft_stop(&spl06_bus);
		 return 1;
	}

	i2c_soft_send_byte(&spl06_bus,reg_addr);
	if(i2c_soft_wait_ack(&spl06_bus)) // 非应答处理
	{
		 i2c_soft_stop(&spl06_bus);
		
		 return 1;
	}

	i2c_soft_start(&spl06_bus); // 重复起始条件
	i2c_soft_send_byte(&spl06_bus,(SPL06_CHIP_ADDRESS << 1) | 0x01); // 读操作，地址最后一位为1
	if(i2c_soft_wait_ack(&spl06_bus)) // 非应答处理
	{
		 i2c_soft_stop(&spl06_bus);
		 
		 return 1;
	}

	uint8_t data = i2c_soft_read_byte(&spl06_bus);
	i2c_soft_send_not_ack(&spl06_bus); // 读完最后一个字节后发送非应答

	i2c_soft_stop(&spl06_bus);
	return data;
}


static int16_t spl06_get_coef_c0()
{
	int16_t c0 = 0;

	uint8_t c0_h = spl06_read_reg(SPL_COEF_C0);
	uint8_t c0_c1 = spl06_read_reg(SPL_COEF_C0_C1);

	c0  = (int16_t)((c0_h << 4) | (c0_c1 >> 4)); // c0的高8位在c0_h，低4位在c0_l的高4位
	if(c0 & 0x0800) // 判断c0的第11位（符号位）是否为1
	{
		c0 |= 0xF000; // 补码有符号位，如果是负数，进行符号扩展，将高4位全部置为1
	}

	return c0;
}

static int16_t spl06_get_coef_c1()
{
	int16_t c1 = 0;

	uint8_t c1_l = spl06_read_reg(SPL_COEF_C1);
	uint8_t c0_c1 = spl06_read_reg(SPL_COEF_C0_C1);

	c1  = (int16_t)(((c0_c1 & 0x0F) << 8) | c1_l); // c1的高4位在c0_c1的低4位，低8位在c1_l
	if(c1 & 0x0800) // 判断c1的第11位（符号位）是否为1
	{
		c1 |= 0xF000; // 补码有符号位，如果是负数，进行符号扩展，将高4位全部置为1
	}

	return c1;
}


static int32_t spl06_get_coef_c00()
{
	int32_t c00 = 0;

	uint8_t c00_h = spl06_read_reg(SPL_COEF_C00_H );
	uint8_t c00_m = spl06_read_reg(SPL_COEF_C00_M );
	uint8_t c00_l = spl06_read_reg(SPL_COEF_C00_C10);

	c00 = ((int32_t)c00_h << 12) | ((int32_t)c00_m << 4) | (c00_l >> 4); // c00的高20位在c00_h和c00_m，低4位在c00_l的高4位
	if(c00 & 0x80000) // 判断c00的第19位（符号位）是否为1
	{
		c00 |= 0xFFF00000; // 补码有符号位，如果是负数，进行符号扩展，将高12位全部置为1
	}

	return c00;
}

static int32_t spl06_get_coef_c10()
{
	int32_t c10 = 0;

	uint8_t c10_h = spl06_read_reg(SPL_COEF_C00_C10);
	uint8_t c10_m = spl06_read_reg(SPL_COEF_C10_M);
	uint8_t c10_l = spl06_read_reg(SPL_COEF_C10_L);

	c10 = ((int32_t)(c10_h & 0x0F) << 16) | ((int32_t)c10_m << 8) | c10_l; // c10的高20位在c10_h的低4位和c10_m，低8位在c10_l
	if(c10 & 0x80000) // 判断c10的第19位（符号位）是否为1
	{
		c10 |= 0xFFF00000; // 补码有符号位，如果是负数，进行符号扩展，将高12位全部置为1
	}

	return c10;
}

static int16_t spl06_get_coef_c01()
{
	int16_t c01 = 0;

	uint8_t c01_h = spl06_read_reg(SPL_COEF_C01_H);
	uint8_t c01_l = spl06_read_reg(SPL_COEF_C01_L);

	c01 = (int16_t)((c01_h << 8) | c01_l); // c01的高8位在c01_h，低8位在c01_l
	
	return c01;
}

static int16_t spl06_get_coef_c11()
{
	int16_t c11 = 0;

	uint8_t c11_h = spl06_read_reg(SPL_COEF_C11_H);
	uint8_t c11_l = spl06_read_reg(SPL_COEF_C11_L);

	c11 = (int16_t)((c11_h << 8) | c11_l); // c11的高8位在c11_h，低8位在c11_l
	
	return c11;
}

static int16_t spl06_get_coef_c20()
{
	int16_t c20 = 0;

	uint8_t c20_h = spl06_read_reg(SPL_COEF_C20_H);
	uint8_t c20_l = spl06_read_reg(SPL_COEF_C20_L);

	c20 = (int16_t)((c20_h << 8) | c20_l); // c20的高8位在c20_h，低8位在c20_l

	return c20;
}

static int16_t spl06_get_coef_c21()
{
	int16_t c21 = 0;

	uint8_t c21_h = spl06_read_reg(SPL_COEF_C21_H);
	uint8_t c21_l = spl06_read_reg(SPL_COEF_C21_L);

	c21 = (int16_t)((c21_h << 8) | c21_l); // c21的高8位在c21_h，低8位在c21_l

	return c21;
}

static int16_t spl06_get_coef_c30()
{
	int16_t c30 = 0;

	uint8_t c30_h = spl06_read_reg(SPL_COEF_C30_H);
	uint8_t c30_l = spl06_read_reg(SPL_COEF_C30_L);

	c30 = (int16_t)((c30_h << 8) | c30_l); // c30的高8位在c30_h，低8位在c30_l

	return c30;
}


static int32_t spl06_get_raw_temp()
{
	int32_t raw_temp = 0;

	uint8_t temp_h = spl06_read_reg(SPL_TMP_B2);
	uint8_t temp_m = spl06_read_reg(SPL_TMP_B1);
	uint8_t temp_l = spl06_read_reg(SPL_TMP_B0);

	raw_temp = ((int32_t)temp_h << 16) | ((int32_t)temp_m << 8) | temp_l; // 原始温度数据的高8位在temp_h，中间8位在temp_m，低8位在temp_l
	if(raw_temp & 0x800000) // 判断原始温度数据的第23位（符号位）是否为1
	{
		raw_temp |= 0xFF000000; // 补码有符号位，如果是负数，进行符号扩展，将高8位全部置为1
	}


	return raw_temp;
}

static int32_t spl06_get_raw_pressure()
{
	int32_t raw_pressure = 0;

	uint8_t prs_h = spl06_read_reg(SPL_PRS_B2);
	uint8_t prs_m = spl06_read_reg(SPL_PRS_B1);
	uint8_t prs_l = spl06_read_reg(SPL_PRS_B0);

	raw_pressure = ((int32_t)prs_h << 16) | ((int32_t)prs_m << 8) | prs_l; // 原始压力数据的高8位在prs_h，中间8位在prs_m，低8位在prs_l
	if(raw_pressure & 0x800000) // 判断原始压力数据的第23位（符号位）是否为1
	{
		raw_pressure |= 0xFF000000; // 补码有符号位，如果是负数，进行符号扩展，将高8位全部置为1
	}

	return raw_pressure;
}


uint8_t spl06_init(void)
{
	i2c_init(&spl06_bus);

	uint8_t id = spl06_read_reg(SPL_ID_REG);
	if(id != SPL06_CHIP_ID)
	{
		return 0; // 设备ID不匹配，初始化失败
	}

	spl06_write_reg(SPL_PRS_CFG, 0x01);//压力测量配置：测量速率1次/秒，过采样率2倍
	spl06_write_reg(SPL_TMP_CFG, 0x80); //温度测量配置：使用外部温度传感器，测量速率1次/秒，过采样率1倍
	spl06_write_reg(SPL_MEAS_CFG,0x07); //连续测量模式，压力和温度交替测量
	spl06_write_reg(SPL_CFG_REG, 0x00); // 该寄存器配置全关闭

	c0 = spl06_get_coef_c0();
	c1 = spl06_get_coef_c1();
	c00 = spl06_get_coef_c00();
	c10 = spl06_get_coef_c10();
	c01 = spl06_get_coef_c01();
	c11 = spl06_get_coef_c11();
	c20 = spl06_get_coef_c20();
	c21 = spl06_get_coef_c21();
	c30 = spl06_get_coef_c30();

	if(spl06_read_reg(SPL_PRS_CFG) != 0x01 || spl06_read_reg(SPL_CFG_REG) != 0x00)
	{
		return 1; // 配置寄存器值不正确，初始化失败
	}
	else
	{
		return 0; // 初始化成功
	}



}

void spl06_sleep()
{
	spl06_write_reg(SPL_MEAS_CFG,0x00); // 进入睡眠模式，停止测量
}

void spl06_wakeup()
{
	spl06_write_reg(SPL_MEAS_CFG,0x07); // 重新进入连续测量模式，压力和温度交替测量
}

uint8_t spl06_get_id()
{
	return spl06_read_reg(SPL_ID_REG);
}


float spl06_calculate_temp()
{	
	float Traw_sc=0.0,Tcomp=0.0;

	Traw_sc = (float)spl06_get_raw_temp() / KT; // 将原始温度数据转换为实际温度值，KT为温度补偿系数
	
	Tcomp = c0*0.5 + c1*Traw_sc; 

	return Tcomp;
}
float spl06_calculate_pressure()
{
	float Traw_sc=0.0,Praw_sc=0.0;
	float Pcomp;

	Traw_sc = (float)spl06_get_raw_temp() / KT; // 将原始温度数据转换为实际温度值，KT为温度补偿系数
	Praw_sc = (float)spl06_get_raw_pressure() / KP; // 将原始压力数据转换为实际压力值，KP为压力补偿系数
	Pcomp = c00 + Praw_sc*(c10 + Praw_sc*(c20 + Praw_sc*c30)) + Traw_sc*c01 + Traw_sc*Praw_sc*(c11 + Praw_sc*c21); // 根据补偿公式计算补偿后的压力值

	return Pcomp;
}

float spl06_calculate_altitude()
{
	float pressure = spl06_calculate_pressure();
	float altitude = 44330.0 * (1.0 - powf(pressure / 101325.0, 0.1903)); // 根据国际标准大气压公式计算海拔高度，假设海平面标准大气压为101325 Pa

	return altitude;
}

