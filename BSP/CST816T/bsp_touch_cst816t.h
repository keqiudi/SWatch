#ifndef __BSP_TOUCH_CST816T_H
#define __BSP_TOUCH_CST816T_H

#include "i2c_hal.h"
#include "delay.h"

/* CST816T触摸芯片引脚宏定义 */

#define TP_RST_PORT   GPIOA
#define TP_RST_PIN    GPIO_PIN_15 

#define TP_INT_PORT   GPIOB
#define TP_INT_PIN    GPIO_PIN_1

#define TP_SCL_PORT   GPIOB
#define TP_SCL_PIN    GPIO_PIN_6

#define TP_SDA_PORT   GPIOB
#define TP_SDA_PIN    GPIO_PIN_4

/* CST816T触摸芯片相关宏函数定义*/

#define TP_RST_LOW() 	HAL_GPIO_WritePin(TP_RST_PORT, TP_RST_PIN, GPIO_PIN_RESET)
#define TP_RST_HIGH()   HAL_GPIO_WritePin(TP_RST_PORT, TP_RST_PIN, GPIO_PIN_SET)

/* CST816T触摸芯片I2C地址定义 */
#define CST816T_I2C_ADDRESS        0x15    // 7位I2C地址
#define CST816T_I2C_WRITE_ADDRESS  0x2A    // 8位写地址
#define CST816T_I2C_READ_ADDRESS   0X2B    // 8位读地址


/* CST816T触摸芯片寄存器地址 */

#define GestureID 			0x01    // 手势ID寄存器，记录当前检测到的手势类型
#define FingerNum 			0x02    // 触摸点数寄存器，当前触摸点的数量
#define XposH 					0x03    // 第一个触摸点X坐标高8位
#define XposL 					0x04    // 第一个触摸点X坐标低8位
#define YposH 					0x05    // 第一个触摸点Y坐标高8位
#define YposL 					0x06    // 第一个触摸点Y坐标低8位
#define ChipID  				0xA7    // 芯片ID寄存器，用于芯片识别，固定值
#define ProjID        	0xA8    // 项目ID寄存器，区分不同项目
#define FwVersion     	0xA9    // 固件版本号寄存器
#define FactoryID 			0xAA    // 工厂ID寄存器
#define BPC0H						0xB0    // 备份坐标0高8位
#define BPC0L 					0xB1    // 备份坐标0低8位
#define BPC1H						0xB2    // 备份坐标1高8位
#define BPC1L						0xB3    // 备份坐标1低8位
#define CDCRes        	0xC0    // CDC电阻
#define CDCFreq					0xC1    // CDC频率
#define CDCIdac					0xC2    // CDC电流
#define SleepMode     	0xE5    // 进入低功耗模式控制
#define ErrResetCtl   	0xEA    // 错误复位控制
#define LongPressTick 	0xEB    // 长按定时时间
#define MotionMask			0xEC    // 对某些运动相关操作的屏蔽或启用设置
#define IrqPluseWidth 	0xED    // 中断脉冲宽度
#define NorScanPer    	0xEE    // 正常扫描周期参数
#define MotionSlAngle 	0xEF    // 运动滑动角度
#define LpScanRaw1H   	0xF0    // 低功耗扫描原始数据1高字节
#define LpScanRaw1L   	0xF1    // 低功耗扫描原始数据1低字节
#define LpScanRaw2H 		0xF2    // 低功耗扫描原始数据2高字节
#define LpScanRaw2L 		0xF3    // 低功耗扫描原始数据2低字节
#define LpAutoWakeTime 	0xF4    // 低功耗自动唤醒时间
#define LpScanTH 				0xF5    // 低功耗扫描阈值
#define LpScanWin 			0xF6    // 低功耗扫描窗口
#define LpScanFreq 			0xF7    // 低功耗扫描频率
#define LpScanIdac 			0xF8    // 低功耗扫描电流
#define AutoSleepTime 	0xF9    // 自动休眠时间
#define IrqCtl 					0xFA    // 中断控制
#define AutoReset 			0xFB    // 自动复位控制
#define LongPressTime 	0xFC    // 长按判定时间
#define IOCtl 					0xFD    // IO控制
#define DisAutoSleep 		0xFE    // 禁止自动休眠


/* 触摸屏坐标结构体 */
typedef struct
{
	unsigned int x_pos;
	unsigned int y_pos;
}CST816T_Info;

/* 用于上层使用 */
extern CST816T_Info CST816T_Position;


/* 手势ID识别选项*/
typedef enum
{
	NOGESTURE = 	0x00,//无操作
	DOWNGLIDE = 	0x01,//下滑
	UPGLIDE = 		0x02,//上滑
	LEFTGLIDE = 	0x03,//左滑
	RIGHTGLIDE = 	0x04,//右滑 
	CLICK = 			0x05, //点击
	DOUBLECLICK = 0x0B, //双击
	LONGPRESS = 	0x0C, //长按
} GestureID_TypeDef;



/* 连续动作配置选项 */
typedef enum
{
	M_DISABLE = 	0x00,// 不需要任何连续动作
	EnConLR = 		0x01,// 左右滑动
	EnConUD = 		0x02,//上下滑动
	EnDClick = 		0x03, //双击
	M_ALLENABLE = 0x07, //启用所有连续动作配置
} MotionMask_TypeDef;

/* 中断低脉冲发射方式选项 */
typedef enum
{
	OnceWLP = 		0x00,//单词脉冲发射
	EnMotion = 		0x10,//运动变化
	EnChange = 		0x20,//检测某些特定变化
	EnTouch = 		0x40,//触摸事件
	EnTest = 			0x80,//检测中断是否正常
} IrqCtl_TypeDef;

/* 屏幕初始化相关函数 */
void CST816T_GPIO_Init(void);
void CST816T_Reset(void);
void CST816T_Init(void);

/* 触摸屏读写相关函数*/
void CST816T_WriteReg(uint8_t reg_addr,uint8_t data);
uint8_t CST816T_ReadReg(uint8_t reg_addr);
void CST816T_WriteBytes(uint8_t reg_addr,uint8_t data[],int size);
void CST816T_ReadBytes(uint8_t reg_addr,uint8_t data[],int size);

/* 触摸屏操作相关函数*/
void CST816T_Get_Postiton(void);
uint8_t CST816T_Get_FingerNum(void);
uint8_t CST816T_Get_ChipID(void);

/* 触摸屏参数配置相关函数*/
void CST816T_Sleep(void);
void CST816T_Wakeup(void);
void CST816T_Config_MotionMask(uint8_t mode);
void CST816T_Config_AutoSleepTime(uint8_t time);
void CST816T_Config_MotionSlAngle(uint8_t x_right_y_up_angle);
void CST816T_Config_NorScanPer(uint8_t Period);
void CST816T_Config_IrqPluseWidth(uint8_t Width);
void CST816T_Config_LpScanTH(uint8_t TH);

#endif