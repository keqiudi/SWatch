#ifndef __BSP_MPU6050_H
#define __BSP_MPU6050_H

#include "stdint.h"   

/*-----------------------------------�Ĵ�������-----------------------------------*/

// �Լ���ؼĴ����� ͨ��ֻ�ڲ���򴫸���У׼ʱʹ��(ѡ��)
#define MPU6050_REG_SELF_TEST_X         0x0D  // X���Լ�
#define MPU6050_REG_SELF_TEST_Y         0x0E  // Y���Լ�
#define MPU6050_REG_SELF_TEST_Z         0x0F  // Z���Լ�
#define MPU6050_REG_SELF_TEST_A         0x10  // XYZ�Լ��λ


// ������/�˲�/������ؼĴ���: ͨ����ʼ��ʱ����(����)
#define MPU6050_REG_SMPLRT_DIV          0x19  // �����ʷ�Ƶ�������ã�����������ʣ�
#define MPU6050_REG_CONFIG              0x1A  // ���ã�DLPF/�ⲿͬ�������ã�
#define MPU6050_REG_GYRO_CONFIG         0x1B  // �������������ã����ã�������ѡ��
#define MPU6050_REG_ACCEL_CONFIG        0x1C  // ���ٶȼ��������ã����ã�������ѡ��

// ��������/�˶����/���˶������ؼĴ���
#define MPU6050_REG_FF_THR              0x1D  // ����������ٶ���ֵ
#define MPU6050_REG_FF_DUR              0x1E  // �����������ʱ��
#define MPU6050_REG_MOT_THR             0x1F  // �˶������ֵ
#define MPU6050_REG_MOT_DUR             0x20  // �˶�������ʱ��
#define MPU6050_REG_ZRMOT_THR           0x21  // ���˶���ֵ
#define MPU6050_REG_ZRMOT_DUR           0x22  // ���˶�����ʱ��

//  FIFO���ܼĴ����� ����ʹ��FIFO��������ʱ������(ѡ��)
#define MPU6050_REG_FIFO_EN             0x23  // FIFO�洢ʹ��
#define MPU6050_REG_FIFO_COUNTH         0x72  // FIFO���ݼ������ֽ�
#define MPU6050_REG_FIFO_COUNTL         0x73  // FIFO���ݼ������ֽ�
#define MPU6050_REG_FIFO_R_W            0x74  // FIFO��д����


// I2C����/�ⲿ��������չ: ������ģʽ/��չ�õ���ѡ�á�
#define MPU6050_REG_I2C_MST_CTRL        0x24  // I2C��������
#define MPU6050_REG_I2C_SLV0_ADDR       0x25  // I2C�ӻ�0��ַ
#define MPU6050_REG_I2C_SLV0_REG        0x26  // I2C�ӻ�0�Ĵ�����ַ
#define MPU6050_REG_I2C_SLV0_CTRL       0x27  // I2C�ӻ�0����
#define MPU6050_REG_I2C_SLV1_ADDR       0x28  // I2C�ӻ�1��ַ
#define MPU6050_REG_I2C_SLV1_REG        0x29  // I2C�ӻ�1�Ĵ�����ַ
#define MPU6050_REG_I2C_SLV1_CTRL       0x2A  // I2C�ӻ�1����
#define MPU6050_REG_I2C_SLV2_ADDR       0x2B  // I2C�ӻ�2��ַ
#define MPU6050_REG_I2C_SLV2_REG        0x2C  // I2C�ӻ�2�Ĵ�����ַ
#define MPU6050_REG_I2C_SLV2_CTRL       0x2D  // I2C�ӻ�2����
#define MPU6050_REG_I2C_SLV3_ADDR       0x2E  // I2C�ӻ�3��ַ
#define MPU6050_REG_I2C_SLV3_REG        0x2F  // I2C�ӻ�3�Ĵ�����ַ
#define MPU6050_REG_I2C_SLV3_CTRL       0x30  // I2C�ӻ�3����
#define MPU6050_REG_I2C_SLV4_ADDR       0x31  // I2C�ӻ�4��ַ
#define MPU6050_REG_I2C_SLV4_REG        0x32  // I2C�ӻ�4�Ĵ�����ַ
#define MPU6050_REG_I2C_SLV4_DO         0x33  // I2C�ӻ�4�������
#define MPU6050_REG_I2C_SLV4_CTRL       0x34  // I2C�ӻ�4����
#define MPU6050_REG_I2C_SLV4_DI         0x35  // I2C�ӻ�4��������
#define MPU6050_REG_I2C_MST_STATUS      0x36  // I2C����״̬

// �ж���������ؼĴ���: �Ƽ������ж����/��ȡ״̬�����á�
#define MPU6050_REG_INT_PIN_CFG         0x37  // �жϹܽ�����
#define MPU6050_REG_INT_ENABLE          0x38  // �ж�ʹ��
#define MPU6050_REG_INT_STATUS          0x3A  // �ж�״̬����ȡ�¼�/���ݵ��


// ���������������ؼĴ�����ʵʱ��ȡ���ٶ�/������/�¶����ݡ����á�
#define MPU6050_REG_ACCEL_XOUT_H        0x3B  // X����ٶȸ��ֽ�
#define MPU6050_REG_ACCEL_XOUT_L        0x3C  // X����ٶȵ��ֽ�
#define MPU6050_REG_ACCEL_YOUT_H        0x3D  // Y����ٶȸ��ֽ�
#define MPU6050_REG_ACCEL_YOUT_L        0x3E  // Y����ٶȵ��ֽ�
#define MPU6050_REG_ACCEL_ZOUT_H        0x3F  // Z����ٶȸ��ֽ�
#define MPU6050_REG_ACCEL_ZOUT_L        0x40  // Z����ٶȵ��ֽ�

#define MPU6050_REG_TEMP_OUT_H          0x41  // �¶ȸ��ֽ�
#define MPU6050_REG_TEMP_OUT_L          0x42  // �¶ȵ��ֽ�

#define MPU6050_REG_GYRO_XOUT_H         0x43  // X�������Ǹ��ֽ�
#define MPU6050_REG_GYRO_XOUT_L         0x44  // X�������ǵ��ֽ�
#define MPU6050_REG_GYRO_YOUT_H         0x45  // Y�������Ǹ��ֽ�
#define MPU6050_REG_GYRO_YOUT_L         0x46  // Y�������ǵ��ֽ�
#define MPU6050_REG_GYRO_ZOUT_H         0x47  // Z�������Ǹ��ֽ�
#define MPU6050_REG_GYRO_ZOUT_L         0x48  // Z�������ǵ��ֽ�

// �ⲿ���������ݣ���I2C������չ�ã�����������ϲ��ã���ѡ�á�
#define MPU6050_REG_EXT_SENS_DATA_00    0x49  // �ⲿ����������00
#define MPU6050_REG_EXT_SENS_DATA_01    0x4A  // �ⲿ����������01
#define MPU6050_REG_EXT_SENS_DATA_02    0x4B  // �ⲿ����������02
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

// ��Դ����/��λ�Ĵ����� ͨ����ʼ�����͹��ġ����ѵȳ�����Ҫ���á����á�
#define MPU6050_REG_PWR_MGMT_1          0x6B  // ��Դ����1�����ã�����λ/ʱ��Դ/˯�ߣ�
#define MPU6050_REG_PWR_MGMT_2          0x6C  // ��Դ����2�����ã��͹���/ͨ��ѡ��

//оƬID�Ĵ�������оƬID����Ƿ�ΪMPU6050�����á�
#define MPU6050_REG_WHO_AM_I            0x75  // �豸ID�����ã�


/*-----------------------------------I2C��ַ����-----------------------------------*/


// MPU6050��I2C��ַ: AD0����
#define PIN_AD0 0

#if (PIN_AD0 == 0)
#define MPU6050_I2C_ADDRESS 0x68
#else 
#define MPU6050_I2C_ADDRESS 0x69
#endif

// MPU6050��д��ַ
#define MPU6050_I2C_ADDRESS_WRITE 0xD0
#define MPU6050_I2C_ADDRESS_READ  0xD1

// MPU6050 INT SCL SDA���Ŷ���
#define MPU6050_INT_PORT	GPIOB
#define MPU6050_INT_PIN 	GPIO_PIN_12
#define MPU6050_SDA_PORT 	GPIOB
#define MPU6050_SDA_PIN 	GPIO_PIN_13
#define MPU6050_SCL_PORT	GPIOB
#define MPU6050_SCL_PIN     GPIO_PIN_14

// ��ͨ�˲��������ѡ��
#define DLPF_CFG_1 0x01
#define DLPF_CFG_2 0x02
#define DLPF_CFG_3 0x03
#define DLPF_CFG_4 0x04
#define DLPF_CFG_5 0x05
#define DLPF_CFG_6 0x06

// ����������������
#define FS_SEL_250  0x00 
#define FS_SEL_500  0x01
#define FS_SEL_1000 0x02
#define FS_SEL_2000 0x03

// ���ٶȼ�����������
#define AFS_SEL_2G  0x00
#define AFS_SEL_4G  0x01
#define AFS_SEL_8G  0x02
#define AFS_SEL_16G 0x03

// MPU6050���API

void mpu6050_i2c_pin_init();
uint8_t mpu6050_init();
uint8_t mpu6050_reset();

uint8_t mpu6050_read_len(uint8_t addr,uint8_t reg_addr,uint8_t len,uint8_t* buf);
uint8_t mpu6050_write_len(uint8_t addr,uint8_t reg_addr,uint8_t len,uint8_t* buf);

/**
 * @brief  ����MPU6050�Ĳ�����
 * @note   ������ = ���������Ƶ�� / (1 + SMPLRT_DIV)�����������Ƶ�ʹ̶�Ϊ1000Hz
 *         ʵ�ʲ����ʷ�ΧΪ4Hz~1000Hz��������Χ���Զ��޶�
 * @param  rate �����Ĳ�����(Hz)����Χ4~1000Hz
 * @retval ���������0��ʾ�ɹ���1��ʾʧ��
 */
uint8_t mpu6050_set_sample_rate(uint16_t rate);


/*
 * @brief  ����MPU6050�����ֵ�ͨ�˲�����DLPF�������ü��ٶȼƺ������ǵ����ݴ�����
 *
 * DLPF_CFG�����������/�ӳٶ�Ӧ��ϵ���£��ο�MPU6050�����ֲᣩ��
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
 * DLPF���������˳�����ָ�������ĸ�Ƶ��������ͬ�����£����ٶȼƺ������ǵ���Ч�������ӳ����в�ͬ��
 * �Ƽ�����Ӧ������ѡ����ʴ���, �����ο�˹�ز��������������ʱ����������ź����Ƶ�ʵ��������Ա���������ȷ���źŵ���ʧ�滹ԭ
 * ��������ʴ��ڴ�����2����
 *
 * @param  dlpf_config  DLPF���ò���������ֵ���ϱ���0~6��
 * @return ����0��ʾ���óɹ�
 */

uint8_t mpu6050_set_dlpf(uint16_t dlpf_config);

/**
 * @brief  ����MPU6050�����ǵ������̷�Χ��Full Scale Range��
 *
 * ��ѡ��Χ��ͨ��fs_sel����ѡ�񣩣�
 *   FS_SEL_250  ����250 ��/s
 *   FS_SEL_500  ����500 ��/s
 *   FS_SEL_1000 ����1000 ��/s
 *   FS_SEL_2000 ����2000 ��/s
 *
 * @param  fs_sel  �����̷�Χѡ��ȡֵ0~3�������ú�FS_SEL_xxx��
 * @retval 0       ���óɹ�
 * @retval 1       ����ʧ��
 */
uint8_t mpu6050_set_gyro_full_scale(uint8_t fs_sel);


/**
 * @brief  ����MPU6050���ٶȼƵ������̷�Χ��Full Scale Range��
 *
 * ��ѡ��Χ��ͨ��afs_sel����ѡ�񣩣�
 *   AFS_SEL_2G  ����2g
 *   AFS_SEL_4G  ����4g
 *   AFS_SEL_8G  ����8g
 *   AFS_SEL_16G ����16g
 *
 * @param  afs_sel �����̷�Χѡ��ȡֵ0~3�������ú�AFS_SEL_xG��
 * @retval 0       ���óɹ�
 * @retval 1       ����ʧ��
 */

uint8_t mpu6050_set_accel_full_scale(uint8_t afs_sel);


/**
 * @brief  ��ȡMPU6050��������ԭʼ����
 * @param  gx ָ����X��ԭʼ���ٶ����ݵ�ָ�루��λ��LSB��δ���ţ�
 * @param  gy ָ����Y��ԭʼ���ٶ����ݵ�ָ�루��λ��LSB��δ���ţ�
 * @param  gz ָ����Z��ԭʼ���ٶ����ݵ�ָ�루��λ��LSB��δ���ţ�
 * @retval 0   ��ȡ�ɹ�
 * @retval 1   ��ȡʧ��
 */
uint8_t mpu6050_read_gyro(int16_t* gx, int16_t* gy, int16_t* gz);


/**
 * @brief  ��ȡMPU6050�ļ��ٶȼ�ԭʼ����
 * @param  ax ָ����X��ԭʼ���ٶ����ݵ�ָ�루��λ��LSB��δ���ţ�
 * @param  ay ָ����Y��ԭʼ���ٶ����ݵ�ָ�루��λ��LSB��δ���ţ�
 * @param  az ָ����Z��ԭʼ���ٶ����ݵ�ָ�루��λ��LSB��δ���ţ�
 * @retval 0   ��ȡ�ɹ�
 * @retval 1   ��ȡʧ��
 */
uint8_t mpu6050_read_accel(int16_t* ax, int16_t* ay, int16_t* az);

/**
 * @brief  ��ȡMPU6050�¶�ԭʼ���ݲ�ת��Ϊ�����¶�
 * @note   ���㹫ʽ��Temperature(��) = TEMP_OUT / 340 + 36.53
 * @param  temperature ָ���¶ȱ�����ָ�루��λ���棩
 * @retval 0   ��ȡ�ɹ�
 * @retval 1   ��ȡʧ��
 */
uint8_t mpu6050_read_temperature(float* temperature);


/**
 * @brief  ��ȡMPU6050оƬ��WHO_AM_I�Ĵ�������ȡ����ID
 * @note   WHO_AM_I�Ĵ���ֵͨ��Ϊ0x68�����ְ汾Ϊ0x69�������ڼ��оƬͨ���Ƿ�����
 * @retval ����WHO_AM_I�Ĵ�����ֵ����MPU6050��ID
 */
uint8_t mpu6050_get_id();

/**
 * @brief  ���������ǽ��ٶȻ��ָ���ŷ����
 * @note   �÷��������������ǣ��������л����ۼ�Ư��
 * @param  yaw   ָ��ƫ���Ǳ�����ָ�루��λ���㣩
 * @param  pitch ָ�����Ǳ�����ָ�루��λ���㣩
 * @param  roll  ָ�����Ǳ�����ָ�루��λ���㣩
 * @retval 0 ����ɹ�
 * @retval 1 ��ȡ������ʧ��
 */
uint8_t mpu6050_gyro_get_angles(float* yaw,float* pitch,float* roll);

/**
 * @brief  ���ڼ��ٶȼƹ���ŷ����
 * @note   �÷��������ھ�̬��̬���ƣ�yaw �޷��ɼ��ٶȼ�ֱ�Ӳ�ã�������ͨ����Ϊ0
 * @param  yaw   ָ��ƫ���Ǳ�����ָ�루��λ���㣩
 * @param  pitch ָ�����Ǳ�����ָ�루��λ���㣩
 * @param  roll  ָ�����Ǳ�����ָ�루��λ���㣩
 * @retval 0 ����ɹ�
 * @retval 1 ��ȡ���ٶȼ�ʧ��
 */
uint8_t mpu6050_accel_get_angles(float* yaw,float* pitch,float* roll);

/**
 * @brief  �ж��豸��ǰ�Ƿ��ڽ���ˮƽ״̬
 * @note   ͨ�����ڼ��ٶȼƼ������ pitch/roll ����ֵ�Ƚ����ж���yaw �������ж�
 * @retval 1 ˮƽ������ֵ��Χ�ڣ�
 * @retval 0 ��ˮƽ��������ֵ��Χ��
 */
uint8_t mpu6050_is_horizontal();

#endif