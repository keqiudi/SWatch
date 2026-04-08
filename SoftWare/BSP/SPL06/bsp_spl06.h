#ifndef __BSP_SPL06_H
#define __BSP_SPL06_H

#include "i2c_hal.h"

#define SPL06_SDA_PORT GPIOB
#define SPL06_SDA_PIN  GPIO_PIN_13

#define SPL06_SCL_PORT GPIOB
#define SPL06_SCL_PIN  GPIO_PIN_14

#define SPL06_CHIP_ID		    0X10
#define SPL06_CHIP_ADDRESS		0x76 //SDIO引脚被下拉了

/* ==================== SPL06-001 寄存器地址定义 ==================== */

/* 压力数据寄存器 (24位) */
#define SPL_PRS_B2                  0x00    // 压力数据 [23:16]
#define SPL_PRS_B1                  0x01    // 压力数据 [15:8]
#define SPL_PRS_B0                  0x02    // 压力数据 [7:0]

/* 温度数据寄存器 (24位) */
#define SPL_TMP_B2                  0x03    // 温度数据 [23:16]
#define SPL_TMP_B1                  0x04    // 温度数据 [15:8]
#define SPL_TMP_B0                  0x05    // 温度数据 [7:0]

/* 配置寄存器 */
#define SPL_PRS_CFG                 0x06    // 压力测量配置
#define SPL_TMP_CFG                 0x07    // 温度测量配置
#define SPL_MEAS_CFG                0x08    // 测量控制和模式配置
#define SPL_CFG_REG                 0x09    // 中断和FIFO配置

/* 状态寄存器 */
#define SPL_INT_STS                 0x0A    // 中断状态
#define SPL_FIFO_STS                0x0B    // FIFO状态

/* 复位和ID寄存器 */
#define SPL_RESET_REG               0x0C    // 软复位寄存器
#define SPL_ID_REG                  0x0D    // 芯片ID (0x10)

/* 校准系数寄存器(补码) */
#define SPL_COEF_C0                 0x10    // C0 系数 [11:4]
#define SPL_COEF_C0_C1              0x11    // C0[3:0] | C1[11:8]
#define SPL_COEF_C1                 0x12    // C1 系数 [7:0]
#define SPL_COEF_C00_H              0x13    // C00 系数 [19:12]
#define SPL_COEF_C00_M              0x14    // C00 系数 [11:4]
#define SPL_COEF_C00_C10            0x15    // C00[3:0] | C10[19:16]
#define SPL_COEF_C10_M              0x16    // C10 系数 [15:8]
#define SPL_COEF_C10_L              0x17    // C10 系数 [7:0]
#define SPL_COEF_C01_H              0x18    // C01 系数 [15:8]
#define SPL_COEF_C01_L              0x19    // C01 系数 [7:0]
#define SPL_COEF_C11_H              0x1A    // C11 系数 [15:8]
#define SPL_COEF_C11_L              0x1B    // C11 系数 [7:0]
#define SPL_COEF_C20_H              0x1C    // C20 系数 [15:8]
#define SPL_COEF_C20_L              0x1D    // C20 系数 [7:0]
#define SPL_COEF_C21_H              0x1E    // C21 系数 [15:8]
#define SPL_COEF_C21_L              0x1F    // C21 系数 [7:0]
#define SPL_COEF_C30_H              0x20    // C30 系数 [15:8]
#define SPL_COEF_C30_L              0x21    // C30 系数 [7:0]


/* ==================== 压力测量配置 ==================== */

/* 测量速率 (Measurement Rate) */
#define SPL_PM_RATE1                0x00       // 1 次/秒
#define SPL_PM_RATE2                0x01       // 2 次/秒
#define SPL_PM_RATE3                0x02       // 4 次/秒
#define SPL_PM_RATE4                0x03       // 8 次/秒
#define SPL_PM_RATE5                0x04       // 16 次/秒
#define SPL_PM_RATE6                0x05       // 32 次/秒
#define SPL_PM_RATE7                0x06       // 64 次/秒
#define SPL_PM_RATE8                0x07       // 128 次/秒

/* 过采样率 (Oversampling Rate) */
#define SPL_PM_PRC1                 0x00       // 单次测量
#define SPL_PM_PRC2                 0x01       // 2 倍过采样 (低功耗)
#define SPL_PM_PRC3                 0x02       // 4 倍过采样
#define SPL_PM_PRC4                 0x03       // 8 倍过采样 (标准)
#define SPL_PM_PRC5                 0x04       // 16 倍过采样 (需在CFG_REG配置移位)
#define SPL_PM_PRC6                 0x05       // 32 倍过采样 (需在CFG_REG配置移位)
#define SPL_PM_PRC7                 0x06       // 64 倍过采样 (高精度, 需在CFG_REG配置移位)
#define SPL_PM_PRC8                 0x07       // 128 倍过采样 (需在CFG_REG配置移位)


/* ==================== 温度测量配置 ==================== */

/* 温度传感器选择 */
#define SPL_TMP_INT                 0       // Internal sensor (in ASIC) 
#define SPL_TMP_EXT                 1       //  External sensor (in pressure sensor MEMS element) 

/* 测量速率 (Measurement Rate) */
#define SPL_TMP_RATE1               0x00    // 1 次/秒
#define SPL_TMP_RATE2               0x01    // 2 次/秒
#define SPL_TMP_RATE3               0x02    // 4 次/秒
#define SPL_TMP_RATE4               0x03    // 8 次/秒
#define SPL_TMP_RATE5               0x04    // 16 次/秒
#define SPL_TMP_RATE6               0x05    // 32 次/秒
#define SPL_TMP_RATE7               0x06    // 64 次/秒
#define SPL_TMP_RATE8               0x07    // 128 次/秒

/* 过采样率 (Oversampling Rate) */
#define SPL_TMP_PRC1                0x00    // 单次测量
#define SPL_TMP_PRC2                0x01    // 2 倍过采样
#define SPL_TMP_PRC3                0x02    // 4 倍过采样
#define SPL_TMP_PRC4                0x03    // 8 倍过采样
#define SPL_TMP_PRC5                0x04    // 16 倍过采样
#define SPL_TMP_PRC6                0x05    // 32 倍过采样
#define SPL_TMP_PRC7                0x06    // 64 倍过采样
#define SPL_TMP_PRC8                0x07    // 128 倍过采样

#define KT 524288 // 温度补偿系数，与温度采样率有关，固定值详见数据手册
#define KP 1572864 // 压力补偿系数，与压力采样率有关，固定值详见数据手册



uint8_t spl06_init(void);
void spl06_sleep();
void spl06_wakeup();
uint8_t spl006_get_id();

float spl06_calculate_pressure();
float spl06_calculate_temp();
float spl06_calculate_altitude();

#endif


