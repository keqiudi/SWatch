#include "bsp_touch_cst816t.h"

#define OFFSET_Y 0
#define OFFSET_X 0

i2c_bus_t CST816T_bus = 
{
	.I2C_SDA_PORT = TP_SDA_PORT,
	.I2C_SCL_PORT = TP_SCL_PORT,
	.I2C_SDA_PIN =  TP_SDA_PIN,
	.I2C_SCL_PIN =  TP_SCL_PIN
};

CST816T_Info CST816T_Position;

/*
*********************************************************************************************************
*	函 数 名: CST816_GPIO_Init
*	功能说明: CST816 GPIO口初始化
*	形    参：none
*	返 回 值: none
*********************************************************************************************************
*/
void CST816T_GPIO_Init(void)
{
	/*开启时钟*/
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* 初始化复位引脚 */
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = TP_RST_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; //50Mhz左右即可
	HAL_GPIO_Init(TP_RST_PORT,&GPIO_InitStruct);

	HAL_GPIO_WritePin(TP_RST_PORT,TP_RST_PIN,GPIO_PIN_SET);

	/* 初始化中断引脚,不用中断的话屏蔽掉 */

	/* I2C引脚初始化 */
	i2c_init(&CST816T_bus);
}

/*
*********************************************************************************************************
*	函 数 名: CST816T_Init
*	功能说明: CST816初始化
*	形    参：none
*	返 回 值: none
*********************************************************************************************************
*/
void CST816T_Init(void)
{
	CST816T_GPIO_Init();
	CST816T_Config_AutoSleepTime(5);
}

/*
*********************************************************************************************************
*	函 数 名: CST816T_Reset
*	功能说明: 触摸屏复位
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816T_Reset(void)
{
	TP_RST_LOW();
	delay_ms(10);
	TP_RST_HIGH();
	delay_ms(100); //手册上复位等待最小100ms
}

/*
*********************************************************************************************************
*	函 数 名: CST816T_WriteReg
*	功能说明: 向指定寄存器写入单个字节数据
*	形    参：reg_addr：寄存器地址 data：要写入的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816T_WriteReg(uint8_t reg_addr,uint8_t data)
{
	i2c_soft_start(&CST816T_bus);

	i2c_soft_send_byte(&CST816T_bus,CST816T_I2C_WRITE_ADDRESS);
	i2c_soft_wait_ack(&CST816T_bus);
	i2c_soft_send_byte(&CST816T_bus,reg_addr);
	i2c_soft_wait_ack(&CST816T_bus);
	i2c_soft_send_byte(&CST816T_bus,data);
	i2c_soft_wait_ack(&CST816T_bus);

	i2c_soft_stop(&CST816T_bus);
}

/*
*********************************************************************************************************
*	函 数 名: CST816T_WriteBytes
*	功能说明: 向指定寄存器开始地址连续写入多个字节数据
*	形    参：reg_addr：寄存器起始地址 data：数据指针 size：写入字节数
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816T_WriteBytes(uint8_t reg_addr,uint8_t data[],int size)
{
	i2c_soft_start(&CST816T_bus);

	i2c_soft_send_byte(&CST816T_bus,CST816T_I2C_WRITE_ADDRESS);
	i2c_soft_wait_ack(&CST816T_bus);
	i2c_soft_send_byte(&CST816T_bus,reg_addr);
	i2c_soft_wait_ack(&CST816T_bus);	

	for(int i=0;i<size;i++)
	{
		i2c_soft_send_byte(&CST816T_bus,data[i]);
		i2c_soft_wait_ack(&CST816T_bus);	
	}

	i2c_soft_stop(&CST816T_bus);
}

/*
*********************************************************************************************************
*	函 数 名: CST816T_ReadReg
*	功能说明: 读取指定寄存器的单个字节数据
*	形    参：reg_addr：寄存器地址
*	返 回 值: 读取到的数据
*********************************************************************************************************
*/
uint8_t CST816T_ReadReg(uint8_t reg_addr)
{
	uint8_t data;

	i2c_soft_start(&CST816T_bus);

	i2c_soft_send_byte(&CST816T_bus,CST816T_I2C_WRITE_ADDRESS);
	i2c_soft_wait_ack(&CST816T_bus);
	i2c_soft_send_byte(&CST816T_bus,reg_addr);
	i2c_soft_wait_ack(&CST816T_bus);

	i2c_soft_start(&CST816T_bus);
	i2c_soft_send_byte(&CST816T_bus,CST816T_I2C_READ_ADDRESS);
	i2c_soft_wait_ack(&CST816T_bus);
	data = i2c_soft_read_byte(&CST816T_bus);
	i2c_soft_send_not_ack(&CST816T_bus);

	i2c_soft_stop(&CST816T_bus);

	return data;
}

/*
*********************************************************************************************************
*	函 数 名: CST816T_ReadBytes
*	功能说明: 从指定寄存器地址开始连续读取多个字节数据
*	形    参：reg_addr：寄存器起始地址 data：数据缓存指针 size：读取字节数
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816T_ReadBytes(uint8_t reg_addr,uint8_t data[],int size)
{
	i2c_soft_start(&CST816T_bus);

	i2c_soft_send_byte(&CST816T_bus,CST816T_I2C_WRITE_ADDRESS);
	i2c_soft_wait_ack(&CST816T_bus);
	i2c_soft_send_byte(&CST816T_bus,reg_addr);
	i2c_soft_wait_ack(&CST816T_bus);

	i2c_soft_start(&CST816T_bus);
	i2c_soft_send_byte(&CST816T_bus,CST816T_I2C_READ_ADDRESS);
	i2c_soft_wait_ack(&CST816T_bus);
	for(int i=0;i<size;i++)
	{
		data[i] = i2c_soft_read_byte(&CST816T_bus);

		if(i < size-1)
		{
			i2c_soft_send_ack(&CST816T_bus);
		}			
	}

	i2c_soft_send_not_ack(&CST816T_bus);
	i2c_soft_stop(&CST816T_bus);
}

/*
*********************************************************************************************************
*	函 数 名: CST816T_Get_Postiton
*	功能说明: 获取当前触摸点的X、Y坐标
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816T_Get_Postiton(void)
{
	uint8_t data[4] ;
	CST816T_ReadBytes(XposH,data,4);
	CST816T_Position.x_pos = ((data[0] & 0x0F) << 8) | data[1];
	CST816T_Position.y_pos = ((data[2] & 0x0F) << 8) | data[3];
}

/*
*********************************************************************************************************
*	函 数 名: CST816T_Get_FingerNum
*	功能说明: 获取当前检测到的手指数量
*	形    参：无
*	返 回 值: 检测到的手指数量
*********************************************************************************************************
*/
uint8_t CST816T_Get_FingerNum(void)
{
	return CST816T_ReadReg(FingerNum);
}

/*
*********************************************************************************************************
*	函 数 名: CST816T_Get_ChipID
*	功能说明: 获取CST816T芯片ID
*	形    参：无
*	返 回 值: 芯片ID
*********************************************************************************************************
*/
uint8_t CST816T_Get_ChipID(void)
{
	return CST816T_ReadReg(ChipID);
}

/*
*********************************************************************************************************
*	函 数 名: CST816T_Config_MotionMask
*	功能说明: 配置手势检测使能掩码（Motion Mask）
*	形    参：mode：手势使能掩码
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816T_Config_MotionMask(uint8_t mode)
{
	CST816T_WriteReg(MotionMask,mode);
}

/*
*********************************************************************************************************
*	函 数 名: CST816T_Config_AutoSleepTime
*	功能说明: 配置自动进入睡眠前的等待时间
*	形    参：time：自动睡眠等待时间，单位:s
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816T_Config_AutoSleepTime(uint8_t time)
{
	CST816T_WriteReg(AutoSleepTime,time);
}

/*
*********************************************************************************************************
*	函 数 名: CST816T_Sleep
*	功能说明: 使CST816T进入休眠模式
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816T_Sleep(void)
{
	CST816T_WriteReg(SleepMode,0x03);
}

/*
*********************************************************************************************************
*	函 数 名: CST816T_Wakeup
*	功能说明: 唤醒CST816T（通过复位方式）
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816T_Wakeup(void)
{
	CST816T_Reset();
}

/*
*********************************************************************************************************
*	函 数 名: CST816T_Config_MotionSlAngle
*	功能说明: 配置滑动手势检测角度
*	形    参：x_right_y_up_angle：角度参数
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816T_Config_MotionSlAngle(uint8_t x_right_y_up_angle)
{
	CST816T_WriteReg(MotionSlAngle,x_right_y_up_angle);
}

/*
*********************************************************************************************************
*	函 数 名: CST816T_Config_NorScanPer
*	功能说明: 配置正常扫描周期
*	形    参：Period：扫描周期，最大30
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816T_Config_NorScanPer(uint8_t Period)
{
	if(Period >= 30)
		Period = 30;
	CST816T_WriteReg(NorScanPer,Period);
}

/*
*********************************************************************************************************
*	函 数 名: CST816T_Config_IrqPluseWidth
*	功能说明: 配置IRQ中断脉冲宽度
*	形    参：Width：中断脉宽，最大200
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816T_Config_IrqPluseWidth(uint8_t Width)
{
	if(Width >= 200)
		Width = 200;
	CST816T_WriteReg(IrqPluseWidth,Width);
}

/*
*********************************************************************************************************
*	函 数 名: CST816t_Config_LpScanTH
*	功能说明: 配置低功耗扫描阈值
*	形    参：TH：低功耗扫描阈值
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816T_Config_LpScanTH(uint8_t TH)
{
	CST816T_WriteReg(LpScanTH,TH);
}