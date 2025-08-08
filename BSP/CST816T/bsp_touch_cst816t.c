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
*	�� �� ��: CST816_GPIO_Init
*	����˵��: CST816 GPIO�ڳ�ʼ��
*	��    �Σ�none
*	�� �� ֵ: none
*********************************************************************************************************
*/
void CST816T_GPIO_Init(void)
{
	/*����ʱ��*/
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* ��ʼ����λ���� */
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = TP_RST_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; //�������
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; //50Mhz���Ҽ���
	HAL_GPIO_Init(TP_RST_PORT,&GPIO_InitStruct);

	HAL_GPIO_WritePin(TP_RST_PORT,TP_RST_PIN,GPIO_PIN_SET);

	/* ��ʼ���ж�����,�����жϵĻ����ε� */

	/* I2C���ų�ʼ�� */
	i2c_init(&CST816T_bus);
}

/*
*********************************************************************************************************
*	�� �� ��: CST816T_Init
*	����˵��: CST816��ʼ��
*	��    �Σ�none
*	�� �� ֵ: none
*********************************************************************************************************
*/
void CST816T_Init(void)
{
	CST816T_GPIO_Init();
	CST816T_Config_AutoSleepTime(5);
}

/*
*********************************************************************************************************
*	�� �� ��: CST816T_Reset
*	����˵��: ��������λ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CST816T_Reset(void)
{
	TP_RST_LOW();
	delay_ms(10);
	TP_RST_HIGH();
	delay_ms(100); //�ֲ��ϸ�λ�ȴ���С100ms
}

/*
*********************************************************************************************************
*	�� �� ��: CST816T_WriteReg
*	����˵��: ��ָ���Ĵ���д�뵥���ֽ�����
*	��    �Σ�reg_addr���Ĵ�����ַ data��Ҫд�������
*	�� �� ֵ: ��
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
*	�� �� ��: CST816T_WriteBytes
*	����˵��: ��ָ���Ĵ�����ʼ��ַ����д�����ֽ�����
*	��    �Σ�reg_addr���Ĵ�����ʼ��ַ data������ָ�� size��д���ֽ���
*	�� �� ֵ: ��
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
*	�� �� ��: CST816T_ReadReg
*	����˵��: ��ȡָ���Ĵ����ĵ����ֽ�����
*	��    �Σ�reg_addr���Ĵ�����ַ
*	�� �� ֵ: ��ȡ��������
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
*	�� �� ��: CST816T_ReadBytes
*	����˵��: ��ָ���Ĵ�����ַ��ʼ������ȡ����ֽ�����
*	��    �Σ�reg_addr���Ĵ�����ʼ��ַ data�����ݻ���ָ�� size����ȡ�ֽ���
*	�� �� ֵ: ��
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
*	�� �� ��: CST816T_Get_Postiton
*	����˵��: ��ȡ��ǰ�������X��Y����
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: CST816T_Get_FingerNum
*	����˵��: ��ȡ��ǰ��⵽����ָ����
*	��    �Σ���
*	�� �� ֵ: ��⵽����ָ����
*********************************************************************************************************
*/
uint8_t CST816T_Get_FingerNum(void)
{
	return CST816T_ReadReg(FingerNum);
}

/*
*********************************************************************************************************
*	�� �� ��: CST816T_Get_ChipID
*	����˵��: ��ȡCST816TоƬID
*	��    �Σ���
*	�� �� ֵ: оƬID
*********************************************************************************************************
*/
uint8_t CST816T_Get_ChipID(void)
{
	return CST816T_ReadReg(ChipID);
}

/*
*********************************************************************************************************
*	�� �� ��: CST816T_Config_MotionMask
*	����˵��: �������Ƽ��ʹ�����루Motion Mask��
*	��    �Σ�mode������ʹ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CST816T_Config_MotionMask(uint8_t mode)
{
	CST816T_WriteReg(MotionMask,mode);
}

/*
*********************************************************************************************************
*	�� �� ��: CST816T_Config_AutoSleepTime
*	����˵��: �����Զ�����˯��ǰ�ĵȴ�ʱ��
*	��    �Σ�time���Զ�˯�ߵȴ�ʱ�䣬��λ:s
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CST816T_Config_AutoSleepTime(uint8_t time)
{
	CST816T_WriteReg(AutoSleepTime,time);
}

/*
*********************************************************************************************************
*	�� �� ��: CST816T_Sleep
*	����˵��: ʹCST816T��������ģʽ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CST816T_Sleep(void)
{
	CST816T_WriteReg(SleepMode,0x03);
}

/*
*********************************************************************************************************
*	�� �� ��: CST816T_Wakeup
*	����˵��: ����CST816T��ͨ����λ��ʽ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CST816T_Wakeup(void)
{
	CST816T_Reset();
}

/*
*********************************************************************************************************
*	�� �� ��: CST816T_Config_MotionSlAngle
*	����˵��: ���û������Ƽ��Ƕ�
*	��    �Σ�x_right_y_up_angle���ǶȲ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CST816T_Config_MotionSlAngle(uint8_t x_right_y_up_angle)
{
	CST816T_WriteReg(MotionSlAngle,x_right_y_up_angle);
}

/*
*********************************************************************************************************
*	�� �� ��: CST816T_Config_NorScanPer
*	����˵��: ��������ɨ������
*	��    �Σ�Period��ɨ�����ڣ����30
*	�� �� ֵ: ��
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
*	�� �� ��: CST816T_Config_IrqPluseWidth
*	����˵��: ����IRQ�ж�������
*	��    �Σ�Width���ж��������200
*	�� �� ֵ: ��
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
*	�� �� ��: CST816t_Config_LpScanTH
*	����˵��: ���õ͹���ɨ����ֵ
*	��    �Σ�TH���͹���ɨ����ֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CST816T_Config_LpScanTH(uint8_t TH)
{
	CST816T_WriteReg(LpScanTH,TH);
}