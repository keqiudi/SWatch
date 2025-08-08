#ifndef __BSP_TOUCH_CST816T_H
#define __BSP_TOUCH_CST816T_H

#include "i2c_hal.h"
#include "delay.h"

/* CST816T����оƬ���ź궨�� */

#define TP_RST_PORT   GPIOA
#define TP_RST_PIN    GPIO_PIN_15 

#define TP_INT_PORT   GPIOB
#define TP_INT_PIN    GPIO_PIN_1

#define TP_SCL_PORT   GPIOB
#define TP_SCL_PIN    GPIO_PIN_6

#define TP_SDA_PORT   GPIOB
#define TP_SDA_PIN    GPIO_PIN_4

/* CST816T����оƬ��غ꺯������*/

#define TP_RST_LOW() 	HAL_GPIO_WritePin(TP_RST_PORT, TP_RST_PIN, GPIO_PIN_RESET)
#define TP_RST_HIGH()   HAL_GPIO_WritePin(TP_RST_PORT, TP_RST_PIN, GPIO_PIN_SET)

/* CST816T����оƬI2C��ַ���� */
#define CST816T_I2C_ADDRESS        0x15    // 7λI2C��ַ
#define CST816T_I2C_WRITE_ADDRESS  0x2A    // 8λд��ַ
#define CST816T_I2C_READ_ADDRESS   0X2B    // 8λ����ַ


/* CST816T����оƬ�Ĵ�����ַ */

#define GestureID 			0x01    // ����ID�Ĵ�������¼��ǰ��⵽����������
#define FingerNum 			0x02    // ���������Ĵ�������ǰ�����������
#define XposH 					0x03    // ��һ��������X�����8λ
#define XposL 					0x04    // ��һ��������X�����8λ
#define YposH 					0x05    // ��һ��������Y�����8λ
#define YposL 					0x06    // ��һ��������Y�����8λ
#define ChipID  				0xA7    // оƬID�Ĵ���������оƬʶ�𣬹̶�ֵ
#define ProjID        	0xA8    // ��ĿID�Ĵ��������ֲ�ͬ��Ŀ
#define FwVersion     	0xA9    // �̼��汾�żĴ���
#define FactoryID 			0xAA    // ����ID�Ĵ���
#define BPC0H						0xB0    // ��������0��8λ
#define BPC0L 					0xB1    // ��������0��8λ
#define BPC1H						0xB2    // ��������1��8λ
#define BPC1L						0xB3    // ��������1��8λ
#define CDCRes        	0xC0    // CDC����
#define CDCFreq					0xC1    // CDCƵ��
#define CDCIdac					0xC2    // CDC����
#define SleepMode     	0xE5    // ����͹���ģʽ����
#define ErrResetCtl   	0xEA    // ����λ����
#define LongPressTick 	0xEB    // ������ʱʱ��
#define MotionMask			0xEC    // ��ĳЩ�˶���ز��������λ���������
#define IrqPluseWidth 	0xED    // �ж�������
#define NorScanPer    	0xEE    // ����ɨ�����ڲ���
#define MotionSlAngle 	0xEF    // �˶������Ƕ�
#define LpScanRaw1H   	0xF0    // �͹���ɨ��ԭʼ����1���ֽ�
#define LpScanRaw1L   	0xF1    // �͹���ɨ��ԭʼ����1���ֽ�
#define LpScanRaw2H 		0xF2    // �͹���ɨ��ԭʼ����2���ֽ�
#define LpScanRaw2L 		0xF3    // �͹���ɨ��ԭʼ����2���ֽ�
#define LpAutoWakeTime 	0xF4    // �͹����Զ�����ʱ��
#define LpScanTH 				0xF5    // �͹���ɨ����ֵ
#define LpScanWin 			0xF6    // �͹���ɨ�贰��
#define LpScanFreq 			0xF7    // �͹���ɨ��Ƶ��
#define LpScanIdac 			0xF8    // �͹���ɨ�����
#define AutoSleepTime 	0xF9    // �Զ�����ʱ��
#define IrqCtl 					0xFA    // �жϿ���
#define AutoReset 			0xFB    // �Զ���λ����
#define LongPressTime 	0xFC    // �����ж�ʱ��
#define IOCtl 					0xFD    // IO����
#define DisAutoSleep 		0xFE    // ��ֹ�Զ�����


/* ����������ṹ�� */
typedef struct
{
	unsigned int x_pos;
	unsigned int y_pos;
}CST816T_Info;

/* �����ϲ�ʹ�� */
extern CST816T_Info CST816T_Position;


/* ����IDʶ��ѡ��*/
typedef enum
{
	NOGESTURE = 	0x00,//�޲���
	DOWNGLIDE = 	0x01,//�»�
	UPGLIDE = 		0x02,//�ϻ�
	LEFTGLIDE = 	0x03,//��
	RIGHTGLIDE = 	0x04,//�һ� 
	CLICK = 			0x05, //���
	DOUBLECLICK = 0x0B, //˫��
	LONGPRESS = 	0x0C, //����
} GestureID_TypeDef;



/* ������������ѡ�� */
typedef enum
{
	M_DISABLE = 	0x00,// ����Ҫ�κ���������
	EnConLR = 		0x01,// ���һ���
	EnConUD = 		0x02,//���»���
	EnDClick = 		0x03, //˫��
	M_ALLENABLE = 0x07, //��������������������
} MotionMask_TypeDef;

/* �жϵ����巢�䷽ʽѡ�� */
typedef enum
{
	OnceWLP = 		0x00,//�������巢��
	EnMotion = 		0x10,//�˶��仯
	EnChange = 		0x20,//���ĳЩ�ض��仯
	EnTouch = 		0x40,//�����¼�
	EnTest = 			0x80,//����ж��Ƿ�����
} IrqCtl_TypeDef;

/* ��Ļ��ʼ����غ��� */
void CST816T_GPIO_Init(void);
void CST816T_Reset(void);
void CST816T_Init(void);

/* ��������д��غ���*/
void CST816T_WriteReg(uint8_t reg_addr,uint8_t data);
uint8_t CST816T_ReadReg(uint8_t reg_addr);
void CST816T_WriteBytes(uint8_t reg_addr,uint8_t data[],int size);
void CST816T_ReadBytes(uint8_t reg_addr,uint8_t data[],int size);

/* ������������غ���*/
void CST816T_Get_Postiton(void);
uint8_t CST816T_Get_FingerNum(void);
uint8_t CST816T_Get_ChipID(void);

/* ����������������غ���*/
void CST816T_Sleep(void);
void CST816T_Wakeup(void);
void CST816T_Config_MotionMask(uint8_t mode);
void CST816T_Config_AutoSleepTime(uint8_t time);
void CST816T_Config_MotionSlAngle(uint8_t x_right_y_up_angle);
void CST816T_Config_NorScanPer(uint8_t Period);
void CST816T_Config_IrqPluseWidth(uint8_t Width);
void CST816T_Config_LpScanTH(uint8_t TH);

#endif