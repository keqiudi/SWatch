#ifndef __BSP_MPU6050_H
#define __BSP_MPU6050_H

#include "stdint.h"   

/*-----------------------------------寄存器定义-----------------------------------*/

// 自检相关寄存器： 通常只在产测或传感器校准时使用(选用)
#define MPU6050_REG_SELF_TEST_X         0x0D  // X轴自检
#define MPU6050_REG_SELF_TEST_Y         0x0E  // Y轴自检
#define MPU6050_REG_SELF_TEST_Z         0x0F  // Z轴自检
#define MPU6050_REG_SELF_TEST_A         0x10  // XYZ自检低位


// 采样率/滤波/量程相关寄存器: 通常初始化时配置(常用)
#define MPU6050_REG_SMPLRT_DIV          0x19  // 采样率分频器（常用，决定输出速率）
#define MPU6050_REG_CONFIG              0x1A  // 配置（DLPF/外部同步，常用）
#define MPU6050_REG_GYRO_CONFIG         0x1B  // 陀螺仪量程配置（常用，满量程选择）
#define MPU6050_REG_ACCEL_CONFIG        0x1C  // 加速度计量程配置（常用，满量程选择）

// 自由落体/运动检测/零运动检测相关寄存器
#define MPU6050_REG_FF_THR              0x1D  // 自由落体加速度阈值
#define MPU6050_REG_FF_DUR              0x1E  // 自由落体持续时间
#define MPU6050_REG_MOT_THR             0x1F  // 运动检测阈值
#define MPU6050_REG_MOT_DUR             0x20  // 运动检测持续时间
#define MPU6050_REG_ZRMOT_THR           0x21  // 零运动阈值
#define MPU6050_REG_ZRMOT_DUR           0x22  // 零运动持续时间

//  FIFO功能寄存器： 仅在使用FIFO缓存数据时需配置(选用)
#define MPU6050_REG_FIFO_EN             0x23  // FIFO存储使能
#define MPU6050_REG_FIFO_COUNTH         0x72  // FIFO数据计数高字节
#define MPU6050_REG_FIFO_COUNTL         0x73  // FIFO数据计数低字节
#define MPU6050_REG_FIFO_R_W            0x74  // FIFO读写数据


// I2C主机/外部传感器扩展: 仅主机模式/扩展用到【选用】
#define MPU6050_REG_I2C_MST_CTRL        0x24  // I2C主机控制
#define MPU6050_REG_I2C_SLV0_ADDR       0x25  // I2C从机0地址
#define MPU6050_REG_I2C_SLV0_REG        0x26  // I2C从机0寄存器地址
#define MPU6050_REG_I2C_SLV0_CTRL       0x27  // I2C从机0控制
#define MPU6050_REG_I2C_SLV1_ADDR       0x28  // I2C从机1地址
#define MPU6050_REG_I2C_SLV1_REG        0x29  // I2C从机1寄存器地址
#define MPU6050_REG_I2C_SLV1_CTRL       0x2A  // I2C从机1控制
#define MPU6050_REG_I2C_SLV2_ADDR       0x2B  // I2C从机2地址
#define MPU6050_REG_I2C_SLV2_REG        0x2C  // I2C从机2寄存器地址
#define MPU6050_REG_I2C_SLV2_CTRL       0x2D  // I2C从机2控制
#define MPU6050_REG_I2C_SLV3_ADDR       0x2E  // I2C从机3地址
#define MPU6050_REG_I2C_SLV3_REG        0x2F  // I2C从机3寄存器地址
#define MPU6050_REG_I2C_SLV3_CTRL       0x30  // I2C从机3控制
#define MPU6050_REG_I2C_SLV4_ADDR       0x31  // I2C从机4地址
#define MPU6050_REG_I2C_SLV4_REG        0x32  // I2C从机4寄存器地址
#define MPU6050_REG_I2C_SLV4_DO         0x33  // I2C从机4数据输出
#define MPU6050_REG_I2C_SLV4_CTRL       0x34  // I2C从机4控制
#define MPU6050_REG_I2C_SLV4_DI         0x35  // I2C从机4数据输入
#define MPU6050_REG_I2C_MST_STATUS      0x36  // I2C主机状态

// 中断与配置相关寄存器: 推荐配置中断输出/读取状态【常用】
#define MPU6050_REG_INT_PIN_CFG         0x37  // 中断管脚配置
#define MPU6050_REG_INT_ENABLE          0x38  // 中断使能
#define MPU6050_REG_INT_STATUS          0x3A  // 中断状态（读取事件/数据到达）


// 传感器输出数据相关寄存器：实时读取加速度/陀螺仪/温度数据【常用】
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

// 外部传感器数据（仅I2C主机扩展用，绝大多数场合不用）【选用】
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

// 电源管理/复位寄存器： 通常初始化、低功耗、唤醒等场合需要配置【常用】
#define MPU6050_REG_PWR_MGMT_1          0x6B  // 电源管理1（常用，软复位/时钟源/睡眠）
#define MPU6050_REG_PWR_MGMT_2          0x6C  // 电源管理2（常用，低功耗/通道选择）

//芯片ID寄存器：读芯片ID检测是否为MPU6050【常用】
#define MPU6050_REG_WHO_AM_I            0x75  // 设备ID（常用）


/*-----------------------------------I2C地址定义-----------------------------------*/


// MPU6050的I2C地址: AD0引脚
#define PIN_AD0 0

#if (PIN_AD0 == 0)
#define MPU6050_I2C_ADDRESS 0x68
#else 
#define MPU6050_I2C_ADDRESS 0x69
#endif

// MPU6050读写地址
#define MPU6050_I2C_ADDRESS_WRITE 0xD0
#define MPU6050_I2C_ADDRESS_READ  0xD1

// MPU6050 INT SCL SDA引脚定义
#define MPU6050_INT_PORT	GPIOB
#define MPU6050_INT_PIN 	GPIO_PIN_12
#define MPU6050_SDA_PORT 	GPIOB
#define MPU6050_SDA_PIN 	GPIO_PIN_13
#define MPU6050_SCL_PORT	GPIOB
#define MPU6050_SCL_PIN   GPIO_PIN_14

// 低通滤波相关配置选项
#define DLPF_CFG_1 0x01
#define DLPF_CFG_2 0x02
#define DLPF_CFG_3 0x03
#define DLPF_CFG_4 0x04
#define DLPF_CFG_5 0x05
#define DLPF_CFG_6 0x06

// 陀螺仪满量程配置
#define FS_SEL_250  0x00 
#define FS_SEL_500  0x01
#define FS_SEL_1000 0x02
#define FS_SEL_2000 0x03

// 加速度计满量程配置
#define AFS_SEL_2G  0x00
#define AFS_SEL_4G  0x01
#define AFS_SEL_8G  0x02
#define AFS_SEL_16G 0x03

// MPU6050相关API

uint8_t mpu6050_init();
uint8_t mpu6050_reset();

/**
 * @brief  设置MPU6050的采样率
 * @note   采样率 = 陀螺仪输出频率 / (1 + SMPLRT_DIV)，陀螺仪输出频率固定为1000Hz
 *         实际采样率范围为4Hz~1000Hz，超过范围会自动限定
 * @param  rate 期望的采样率(Hz)，范围4~1000Hz
 * @retval 操作结果：0表示成功，1表示失败
 */
uint8_t mpu6050_set_sample_rate(uint16_t rate);


/*
 * @brief  配置MPU6050的数字低通滤波器（DLPF），设置加速度计和陀螺仪的数据带宽。
 *
 * DLPF_CFG参数与各带宽/延迟对应关系如下（参考MPU6050数据手册）：
 * -----------------------------------------------------------------------------
 * | DLPF_CFG | 加速度计带宽(Hz) | 加速度计延迟(ms) | 陀螺仪带宽(Hz) | 陀螺仪延迟(ms) | 输出速率(kHz) |
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
 * DLPF的作用是滤除高于指定带宽的高频噪声。不同配置下，加速度计和陀螺仪的有效带宽、延迟略有不同。
 * 推荐根据应用需求选择合适带宽, 根据奈奎斯特采样定理，采样率必须至少是信号最高频率的两倍，以避免混叠现象并确保信号的无失真还原
 * 建议采样率大于带宽的2倍。
 *
 * @param  dlpf_config  DLPF配置参数，具体值见上表（0~6）
 * @return 返回0表示配置成功
 */

uint8_t mpu6050_set_dlpf(uint16_t dlpf_config);

/**
 * @brief  配置MPU6050陀螺仪的满量程范围（Full Scale Range）
 *
 * 可选范围（通过fs_sel参数选择）：
 *   FS_SEL_250  ：±250 °/s
 *   FS_SEL_500  ：±500 °/s
 *   FS_SEL_1000 ：±1000 °/s
 *   FS_SEL_2000 ：±2000 °/s
 *
 * @param  fs_sel  满量程范围选择（取值0~3，建议用宏FS_SEL_xxx）
 * @retval 0       配置成功
 * @retval 1       配置失败
 */
uint8_t mpu6050_set_gyro_full_scale(uint8_t fs_sel);


/**
 * @brief  配置MPU6050加速度计的满量程范围（Full Scale Range）
 *
 * 可选范围（通过afs_sel参数选择）：
 *   AFS_SEL_2G  ：±2g
 *   AFS_SEL_4G  ：±4g
 *   AFS_SEL_8G  ：±8g
 *   AFS_SEL_16G ：±16g
 *
 * @param  afs_sel 满量程范围选择（取值0~3，建议用宏AFS_SEL_xG）
 * @retval 0       配置成功
 * @retval 1       配置失败
 */

uint8_t mpu6050_set_accel_full_scale(uint8_t afs_sel);


/**
 * @brief  读取MPU6050的陀螺仪原始数据
 * @param  gx 指向存放X轴原始角速度数据的指针（单位：LSB，未缩放）
 * @param  gy 指向存放Y轴原始角速度数据的指针（单位：LSB，未缩放）
 * @param  gz 指向存放Z轴原始角速度数据的指针（单位：LSB，未缩放）
 * @retval 0   读取成功
 * @retval 1   读取失败
 */
uint8_t mpu6050_read_gyro(int16_t* gx, int16_t* gy, int16_t* gz);


/**
 * @brief  读取MPU6050的加速度计原始数据
 * @param  ax 指向存放X轴原始加速度数据的指针（单位：LSB，未缩放）
 * @param  ay 指向存放Y轴原始加速度数据的指针（单位：LSB，未缩放）
 * @param  az 指向存放Z轴原始加速度数据的指针（单位：LSB，未缩放）
 * @retval 0   读取成功
 * @retval 1   读取失败
 */
uint8_t mpu6050_read_accel(int16_t* ax, int16_t* ay, int16_t* az);

/**
 * @brief  读取MPU6050温度原始数据并转换为摄氏温度
 * @note   换算公式：Temperature(℃) = TEMP_OUT / 340 + 36.53
 * @param  temperature 指向温度变量的指针（单位：℃）
 * @retval 0   读取成功
 * @retval 1   读取失败
 */
uint8_t mpu6050_read_temperature(float* temperature);


/**
 * @brief  读取MPU6050芯片的WHO_AM_I寄存器，获取器件ID
 * @note   WHO_AM_I寄存器值通常为0x68（部分版本为0x69），用于检测芯片通信是否正常
 * @retval 返回WHO_AM_I寄存器的值，即MPU6050的ID
 */
uint8_t mpu6050_get_id();

/**
 * @brief  基于陀螺仪角速度积分更新欧拉角
 * @note   该方法仅依赖陀螺仪，长期运行会有累计漂移
 * @param  yaw   指向偏航角变量的指针（单位：°）
 * @param  pitch 指向俯仰角变量的指针（单位：°）
 * @param  roll  指向横滚角变量的指针（单位：°）
 * @retval 0 计算成功
 * @retval 1 读取陀螺仪失败
 */
uint8_t mpu6050_gyro_get_angles(float* yaw,float* pitch,float* roll);

/**
 * @brief  基于加速度计估算欧拉角
 * @note   该方法可用于静态姿态估计；yaw 无法由加速度计直接测得，函数内通常置为0
 * @param  yaw   指向偏航角变量的指针（单位：°）
 * @param  pitch 指向俯仰角变量的指针（单位：°）
 * @param  roll  指向横滚角变量的指针（单位：°）
 * @retval 0 计算成功
 * @retval 1 读取加速度计失败
 */
uint8_t mpu6050_accel_get_angles(float* yaw,float* pitch,float* roll);

/**
 * @brief  判断设备当前是否处于近似水平状态
 * @note   通常基于加速度计计算出的 pitch/roll 与阈值比较来判定，yaw 不参与判断
 * @retval 1 水平（在阈值范围内）
 * @retval 0 非水平（超出阈值范围）
 */
uint8_t mpu6050_is_horizontal();

#endif