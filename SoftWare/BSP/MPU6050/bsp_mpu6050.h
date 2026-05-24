#ifndef __BSP_MPU6050_H
#define __BSP_MPU6050_H

#include "stdint.h"   

/*-----------------------------------寄存器定义-----------------------------------*/

// 自检相关寄存器（仅在调试/校准时使用，选用）
#define MPU6050_REG_SELF_TEST_X         0x0D  // X轴自检
#define MPU6050_REG_SELF_TEST_Y         0x0E  // Y轴自检
#define MPU6050_REG_SELF_TEST_Z         0x0F  // Z轴自检
#define MPU6050_REG_SELF_TEST_A         0x10  // XYZ自检结果


// 采样率/滤波/量程相关寄存器：通常在初始化时配置
#define MPU6050_REG_SMPLRT_DIV          0x19  // 采样率分频
#define MPU6050_REG_CONFIG              0x1A  // 配置：DLPF/外部同步
#define MPU6050_REG_GYRO_CONFIG         0x1B  // 陀螺仪配置：量程等
#define MPU6050_REG_ACCEL_CONFIG        0x1C  // 加速度计配置：量程等

// 运动检测/自由落体/零运动相关寄存器
#define MPU6050_REG_FF_THR              0x1D  // 自由落体阈值
#define MPU6050_REG_FF_DUR              0x1E  // 自由落体持续时间
#define MPU6050_REG_MOT_THR             0x1F  // 运动阈值
#define MPU6050_REG_MOT_DUR             0x20  // 运动持续时间
#define MPU6050_REG_ZRMOT_THR           0x21  // 零运动阈值
#define MPU6050_REG_ZRMOT_DUR           0x22  // 零运动持续时间

// FIFO功能寄存器（使用FIFO时配置，选用）
#define MPU6050_REG_FIFO_EN             0x23  // FIFO使能
#define MPU6050_REG_FIFO_COUNTH         0x72  // FIFO计数高字节
#define MPU6050_REG_FIFO_COUNTL         0x73  // FIFO计数低字节
#define MPU6050_REG_FIFO_R_W            0x74  // FIFO读写


// I2C主机/外部传感器扩展：主机模式或外部扩展使用
#define MPU6050_REG_I2C_MST_CTRL        0x24  // I2C主机控制
#define MPU6050_REG_I2C_SLV0_ADDR       0x25  // I2C从机0地址
#define MPU6050_REG_I2C_SLV0_REG        0x26  // I2C从机0寄存器
#define MPU6050_REG_I2C_SLV0_CTRL       0x27  // I2C从机0控制
#define MPU6050_REG_I2C_SLV1_ADDR       0x28  // I2C从机1地址
#define MPU6050_REG_I2C_SLV1_REG        0x29  // I2C从机1寄存器
#define MPU6050_REG_I2C_SLV1_CTRL       0x2A  // I2C从机1控制
#define MPU6050_REG_I2C_SLV2_ADDR       0x2B  // I2C从机2地址
#define MPU6050_REG_I2C_SLV2_REG        0x2C  // I2C从机2寄存器
#define MPU6050_REG_I2C_SLV2_CTRL       0x2D  // I2C从机2控制
#define MPU6050_REG_I2C_SLV3_ADDR       0x2E  // I2C从机3地址
#define MPU6050_REG_I2C_SLV3_REG        0x2F  // I2C从机3寄存器
#define MPU6050_REG_I2C_SLV3_CTRL       0x30  // I2C从机3控制
#define MPU6050_REG_I2C_SLV4_ADDR       0x31  // I2C从机4地址
#define MPU6050_REG_I2C_SLV4_REG        0x32  // I2C从机4寄存器
#define MPU6050_REG_I2C_SLV4_DO         0x33  // I2C从机4数据输出
#define MPU6050_REG_I2C_SLV4_CTRL       0x34  // I2C从机4控制
#define MPU6050_REG_I2C_SLV4_DI         0x35  // I2C从机4数据输入
#define MPU6050_REG_I2C_MST_STATUS      0x36  // I2C主机状态

// 中断相关寄存器：配置中断与读取状态
#define MPU6050_REG_INT_PIN_CFG         0x37  // 中断引脚配置
#define MPU6050_REG_INT_ENABLE          0x38  // 中断使能
#define MPU6050_REG_INT_STATUS          0x3A  // 中断状态（事件/数据就绪）


// 传感器数据寄存器：加速度/陀螺仪/温度
#define MPU6050_REG_ACCEL_XOUT_H        0x3B  // X轴加速度高字节
#define MPU6050_REG_ACCEL_XOUT_L        0x3C  // X轴加速度低字节
#define MPU6050_REG_ACCEL_YOUT_H        0x3D  // Y轴加速度高字节
#define MPU6050_REG_ACCEL_YOUT_L        0x3E  // Y轴加速度低字节
#define MPU6050_REG_ACCEL_ZOUT_H        0x3F  // Z轴加速度高字节
#define MPU6050_REG_ACCEL_ZOUT_L        0x40  // Z轴加速度低字节

#define MPU6050_REG_TEMP_OUT_H          0x41  // 温度高字节
#define MPU6050_REG_TEMP_OUT_L          0x42  // 温度低字节

#define MPU6050_REG_GYRO_XOUT_H         0x43  // X轴陀螺仪高字节
#define MPU6050_REG_GYRO_XOUT_L         0x44  // X轴陀螺仪低字节
#define MPU6050_REG_GYRO_YOUT_H         0x45  // Y轴陀螺仪高字节
#define MPU6050_REG_GYRO_YOUT_L         0x46  // Y轴陀螺仪低字节
#define MPU6050_REG_GYRO_ZOUT_H         0x47  // Z轴陀螺仪高字节
#define MPU6050_REG_GYRO_ZOUT_L         0x48  // Z轴陀螺仪低字节

// 外部传感器数据（I2C主机扩展）
#define MPU6050_REG_EXT_SENS_DATA_00    0x49  // 外部传感器数据00
#define MPU6050_REG_EXT_SENS_DATA_01    0x4A  // 外部传感器数据01
#define MPU6050_REG_EXT_SENS_DATA_02    0x4B  // 外部传感器数据02
#define MPU6050_REG_EXT_SENS_DATA_03    0x4C
#define MPU6050_REG_EXT_SENS_DATA_04    0x4D
#define MPU6050_REG_EXT_SENS_DATA_05    0x4E
#define MPU6050_REG_EXT_SENS_DATA_06    0x4F
#define MPU6050_REG_EXT_SENS_DATA_07    0x50
#define MPU6050_REG_EXT_SENS_DATA_08    0x51
#define MPU6050_REG_EXT_SENS_DATA_09    0x52
#define MPU6050_REG_EXT_SENS_DATA_10    0x53
#define MPU6050_REG_EXT_SENS_DATA_11    0x54
#define MPU6050_REG_EXT_SENS_DATA_12    0x55
#define MPU6050_REG_EXT_SENS_DATA_13    0x56
#define MPU6050_REG_EXT_SENS_DATA_14    0x57
#define MPU6050_REG_EXT_SENS_DATA_15    0x58
#define MPU6050_REG_EXT_SENS_DATA_16    0x59
#define MPU6050_REG_EXT_SENS_DATA_17    0x5A
#define MPU6050_REG_EXT_SENS_DATA_18    0x5B
#define MPU6050_REG_EXT_SENS_DATA_19    0x5C
#define MPU6050_REG_EXT_SENS_DATA_20    0x5D
#define MPU6050_REG_EXT_SENS_DATA_21    0x5E
#define MPU6050_REG_EXT_SENS_DATA_22    0x5F
#define MPU6050_REG_EXT_SENS_DATA_23    0x60

// 电源管理/复位寄存器：初始化与低功耗相关
#define MPU6050_REG_PWR_MGMT_2          0x6C  // 电源管理2：低功耗/通道选择
#define MPU6050_REG_PWR_MGMT_1          0x6B  // 电源管理1：复位/时钟/睡眠

// 芯片ID寄存器：判断器件是否为MPU6050
#define MPU6050_REG_WHO_AM_I            0x75  // 设备ID


/*-----------------------------------I2C地址-----------------------------------*/


// MPU6050 I2C地址：由AD0引脚决定
#define PIN_AD0 0

#if (PIN_AD0 == 0)
#define MPU6050_I2C_ADDRESS 0x68
#else 
#define MPU6050_I2C_ADDRESS 0x69
#endif

// MPU6050读写地址
#define MPU6050_I2C_ADDRESS_WRITE 0xD0
#define MPU6050_I2C_ADDRESS_READ  0xD1

// MPU6050 INT/SCL/SDA引脚
#define MPU6050_INT_PORT	GPIOB
#define MPU6050_INT_PIN 	GPIO_PIN_12
#define MPU6050_SDA_PORT 	GPIOB
#define MPU6050_SDA_PIN 	GPIO_PIN_13
#define MPU6050_SCL_PORT	GPIOB
#define MPU6050_SCL_PIN     GPIO_PIN_14

// 低通滤波配置选项
#define DLPF_CFG_1 0x01
#define DLPF_CFG_2 0x02
#define DLPF_CFG_3 0x03
#define DLPF_CFG_4 0x04
#define DLPF_CFG_5 0x05
#define DLPF_CFG_6 0x06

// 陀螺仪量程配置
#define FS_SEL_250  0x00 
#define FS_SEL_500  0x01
#define FS_SEL_1000 0x02
#define FS_SEL_2000 0x03

// 加速度计量程配置
#define AFS_SEL_2G  0x00
#define AFS_SEL_4G  0x01
#define AFS_SEL_8G  0x02
#define AFS_SEL_16G 0x03

// MPU6050接口

void mpu6050_i2c_pin_init();
uint8_t mpu6050_init();
uint8_t mpu6050_reset();

uint8_t mpu6050_read_len(uint8_t addr,uint8_t reg_addr,uint8_t len,uint8_t* buf);
uint8_t mpu6050_write_len(uint8_t addr,uint8_t reg_addr,uint8_t len,uint8_t* buf);

/**
 * @brief  设置MPU6050采样率
 * @note   采样率 = 陀螺仪输出频率 / (1 + SMPLRT_DIV)，输出频率固定为1000Hz
 *         实际采样率范围为4Hz~1000Hz，超出范围将自动限制
 * @param  rate 目标采样率(Hz)，范围4~1000Hz
 * @retval 0 表示成功，1 表示失败
 */
uint8_t mpu6050_set_sample_rate(uint16_t rate);


/*
 * @brief  设置MPU6050数字低通滤波(DLPF)，降低加速度计和陀螺仪噪声
 *
 * DLPF_CFG的带宽/延迟对应关系如下（参考MPU6050数据手册）：
 * -----------------------------------------------------------------------------
 * | DLPF_CFG | ���ٶȼƴ���(Hz) | ���ٶȼ��ӳ�(ms) | �����Ǵ���(Hz) | �������ӳ�(ms) | �������(kHz) |
 * |----------|------------------|------------------|---------------|---------------|--------------|
 * |    0     |      260         |       0          |     256       |    0.98       |      8       |
 * |    1     |      184         |      2.0         |     188       |    1.9        |      1       |
 * |    2     |       94         |      3.0         |      98       |    2.8        |      1       |
 * |    3     |       44         |      4.9         |      42       |    4.8        |      1       |
 * |    4     |       21         |      8.5         |      20       |    8.3        |      1       |
 * |    5     |       10         |     13.8         |      10       |   13.4        |      1       |
 * |    6     |        5         |     19.0         |       5       |   18.6        |      1       |
 * |    7     |    RESERVED      |    RESERVED      |    RESERVED   |   RESERVED    |      8       |
 * -----------------------------------------------------------------------------
 *
 * DLPF滤波会降低高频噪声，不同配置对应不同的有效带宽和延迟。
 * 推荐根据实际运动频率选择合适带宽，避免过度滤波导致信号失真。
 *
 * @param  dlpf_config  DLPF配置值，范围0~6
 * @return 0 表示配置成功
 */

uint8_t mpu6050_set_dlpf(uint16_t dlpf_config);

/**
 * @brief  设置MPU6050陀螺仪满量程范围(Full Scale Range)
 *
 * 量程选项通过fs_sel选择：
 *   FS_SEL_250  ����250 ��/s
 *   FS_SEL_500  ����500 ��/s
 *   FS_SEL_1000 ����1000 ��/s
 *   FS_SEL_2000 ����2000 ��/s
 *
 * @param  fs_sel  量程选择，取值0~3，对应FS_SEL_xxx
 * @retval 0       成功
 * @retval 1       失败
 */
uint8_t mpu6050_set_gyro_full_scale(uint8_t fs_sel);


/**
 * @brief  设置MPU6050加速度计满量程范围(Full Scale Range)
 *
 * 量程选项通过afs_sel选择：
 *   AFS_SEL_2G  ����2g
 *   AFS_SEL_4G  ����4g
 *   AFS_SEL_8G  ����8g
 *   AFS_SEL_16G ����16g
 *
 * @param  afs_sel 量程选择，取值0~3，对应AFS_SEL_xG
 * @retval 0       成功
 * @retval 1       失败
 */

uint8_t mpu6050_set_accel_full_scale(uint8_t afs_sel);


/**
 * @brief  读取MPU6050陀螺仪原始数据
 * @param  gx 指向X轴原始角速度数据(LSB)
 * @param  gy 指向Y轴原始角速度数据(LSB)
 * @param  gz 指向Z轴原始角速度数据(LSB)
 * @retval 0 读取成功
 * @retval 1 读取失败
 */
uint8_t mpu6050_read_gyro(int16_t* gx, int16_t* gy, int16_t* gz);


/**
 * @brief  读取MPU6050加速度计原始数据
 * @param  ax 指向X轴原始加速度数据(LSB)
 * @param  ay 指向Y轴原始加速度数据(LSB)
 * @param  az 指向Z轴原始加速度数据(LSB)
 * @retval 0 读取成功
 * @retval 1 读取失败
 */
uint8_t mpu6050_read_accel(int16_t* ax, int16_t* ay, int16_t* az);

/**
 * @brief  读取MPU6050温度原始数据并转换为摄氏温度
 * @note   温度公式：Temperature(℃) = TEMP_OUT / 340 + 36.53
 * @param  temperature 指向温度变量(单位℃)
 * @retval 0 读取成功
 * @retval 1 读取失败
 */
uint8_t mpu6050_read_temperature(float* temperature);


/**
 * @brief  读取MPU6050的WHO_AM_I寄存器
 * @note   WHO_AM_I通常为0x68，部分版本为0x69
 * @retval WHO_AM_I寄存器值
 */
uint8_t mpu6050_get_id();

/**
 * @brief  使用陀螺仪积分计算姿态角
 * @note   该方法依赖陀螺仪积分，存在累计漂移
 * @param  yaw   指向偏航角变量(度)
 * @param  pitch 指向俯仰角变量(度)
 * @param  roll  指向横滚角变量(度)
 * @retval 0 成功
 * @retval 1 失败
 */
uint8_t mpu6050_gyro_get_angles(float* yaw,float* pitch,float* roll);

/**
 * @brief  使用加速度计计算姿态角
 * @note   仅适用于静态或低动态，yaw无法由加速度计直接得到，通常置0
 * @param  yaw   指向偏航角变量(度)
 * @param  pitch 指向俯仰角变量(度)
 * @param  roll  指向横滚角变量(度)
 * @retval 0 成功
 * @retval 1 失败
 */
uint8_t mpu6050_accel_get_angles(float* yaw,float* pitch,float* roll);

/**
 * @brief  判断设备是否处于水平状态
 * @note   通过加速度计计算pitch/roll并与阈值比较，yaw不参与判断
 * @retval 1 水平
 * @retval 0 非水平
 */
uint8_t mpu6050_is_horizontal();

#endif