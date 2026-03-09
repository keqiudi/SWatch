#ifndef __LSM303DLHC_H
#define __LSM303DLHC_H

#include "i2c_hal.h"

//磁场感应寄存器 Magnetic field sensing register
#define	LSM303_CRA_REG_M					0x00	//RW
#define	LSM303_CRB_REG_M					0x01	//RW
#define	LSM303_MR_REG_M						0x02	//RW
#define	LSM303_OUT_X_H_M					0x03	//R
#define	LSM303_OUT_X_L_M					0x04	//R
#define	LSM303_OUT_Z_H_M					0x05	//R
#define	LSM303_OUT_Z_L_M					0x06	//R
#define	LSM303_OUT_Y_H_M					0x07	//R
#define	LSM303_OUT_Y_L_M					0x08	//R
#define	LSM303_SR_REG_M						0x09	//R
#define	LSM303_IRA_REG_M					0x0A	//R
#define	LSM303_IRB_REG_M					0x0B	//R
#define	LSM303_IRC_REG_M					0x0C	//R

//线性加速度寄存器 Linear acceleration register 
#define	LSM303_CTRL_REG1_A					0x20	//RW
#define	LSM303_CTRL_REG2_A					0x21	//RW
#define	LSM303_CTRL_REG3_A					0x22	//RW
#define	LSM303_CTRL_REG4_A					0x23	//RW
#define	LSM303_CTRL_REG5_A					0x24	//RW
#define	LSM303_HP_FILTER_RESET_A		    0x25	//R
#define	LSM303_REFERENCE_A					0x26	//RW
#define	LSM303_STATUS_REG_A					0x27	//R
#define	LSM303_OUT_X_L_A					0x28	//R
#define	LSM303_OUT_X_H_A					0x29	//R
#define	LSM303_OUT_Y_L_A					0x2A	//R
#define	LSM303_OUT_Y_H_A					0x2B	//R
#define	LSM303_OUT_Z_L_A					0x2C	//R
#define	LSM303_OUT_Z_H_A					0x2D	//R
#define LSM303_FIFO_CTRL_REG_A				0x2E	//RW
#define LSM303_FIFO_SRC_REG_A				0x2F	//R
#define	LSM303_INT1_CFG_A					0x30	//RW
#define	LSM303_INT1_SOURCE_A				0x31	//R
#define	LSM303_INT1_THS_A					0x32	//RW
#define	LSM303_INT1_DURATION_A			    0x33	//RW
#define	LSM303_INT2_CFG_A					0x34	//RW
#define	LSM303_INT2_SOURCE_A				0x35	//R
#define	LSM303_INT2_THS_A					0x36	//RW
#define	LSM303_INT2_DURATION_A			    0x37	//RW
#define LMS303_CLICK_CFG_A					0x38	//RW
#define	LMS303_CLICK_SRC_A					0x39	//R
#define	LMS303_CLICK_THS_A					0x3A	//RW
#define	LMS303_TIME_LIMIT_A					0x3B	//RW
#define	LMS303_TIME_LATENCY_A				0x3C	//RW
#define	LMS303_TIME_WINDOW_A				0x3D	//RW


#define LSM303_TEMP_OUT_H_M                 0x31	//R
#define LSM303_TEMP_OUT_L_M                 0x32	//R

#define LSM303_ACCEL_ADDRESS				0x19	//加速度计I2C地址,7位0011001
#define LSM303_MAG_ADDRESS				    0x1E	//磁力计I2C地址,7位0011110

#define LSM303DLHC_SDA_PORT GPIOB
#define LSM303DLHC_SDA_PIN  GPIO_PIN_13
#define LSM303DLHC_SCL_PORT GPIOB
#define LSM303DLHC_SCL_PIN  GPIO_PIN_14


uint8_t lsm303dlhc_init(void);

void lsm303dlhc_sleep(void);
void lsm303dlhc_wakeup(void);

void lsm303dlhc_read_accel(int16_t* ax, int16_t* ay, int16_t* az);
void lsm303dlhc_read_magnetic(int16_t* mx, int16_t* my, int16_t* mz);

void lsm303dlhc_read_temperature(float* temperature);
int lsm303dlhc_calc_z_axis_angle(int16_t ax, int16_t ay, int16_t az);
int lsm303dlhc_calc_x_axis_angle(int16_t ax, int16_t ay, int16_t az);
float lsm303dlhc_calc_azimuth_angle(int ax,int ay,int az,int mx,int my,int mz);

#endif