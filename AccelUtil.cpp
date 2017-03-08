#include <Wire.h>
#include <stdint.h>
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include "AccelUtil.h"

void readData() {


  //Define pointer Buffers
  uint8_t accelxHigh;
  uint8_t accelxLow;
  uint8_t accelyLow;
  uint8_t accelyHigh;
  uint8_t accelzLow;
  uint8_t accelzHigh;

  uint16_t gyroxHigh;
  uint16_t gyroxLow;
  uint16_t gyroyHigh;
  uint16_t gyroyLow;
  uint16_t gyrozHigh;
  uint16_t gyrozLow;

  // Read most recent measurements  from 12 registers
  // containing acceleration and gyro Data
  // With the default settings of the MPU-6050, Most registers 0x00
  //From setupMPU() the frequency of sampling is 8kHz
  //MPUread(MPU6050_TEMP_OUT_H, uint8_t accelData.x_high, sizeof(accelData.x_high));
  //MPUread(MPU6050_TEMP_OUT_L, uint8_t accelData.x_high, sizeof(accelData.x_high));

  MPUread(MPU6050_ACCEL_XOUT_H, &accelxHigh, 1);
  MPUread(MPU6050_ACCEL_XOUT_L, &accelxLow, 1);
  MPUread(MPU6050_ACCEL_YOUT_H, &accelyHigh, 1);
  MPUread(MPU6050_ACCEL_YOUT_L, &accelyLow, 1);
  MPUread(MPU6050_ACCEL_ZOUT_H, &accelzHigh, 1);
  MPUread(MPU6050_ACCEL_ZOUT_L, &accelzLow, 1);
  MPUread(MPU6050_GYRO_XOUT_H, &gyroxHigh, 1);
  MPUread(MPU6050_GYRO_XOUT_L, &gyroxLow, 1);
  MPUread(MPU6050_GYRO_YOUT_H, &gyroyHigh, 1);
  MPUread(MPU6050_GYRO_YOUT_L, &gyroyLow, 1);
  MPUread(MPU6050_GYRO_ZOUT_H, &gyrozHigh, 1);
  MPUread(MPU6050_GYRO_ZOUT_L, &gyrozLow, 1);

  //Shift lower 8
  accelxHigh <<= 8;
  accelyHigh <<= 8; 
  accelzHigh <<= 8; 
  gyroxHigh <<= 8; 
  gyroyHigh <<= 8;
  gyrozHigh <<= 8;

  //Make sure lower 8 bits are clear
  accelxHigh &= 0xFF00;
  accelyHigh &= 0xFF00;
  accelzHigh &= 0xFF00;
  gyroxHigh &= 0xFF00;
  gyroyHigh &= 0xFF00;
  gyrozHigh &= 0xFF00;
  

 accelDataX[0] = accelxHigh + accelxLow;
 accelDataY[0] = accelyHigh + accelyLow;
 accelDataZ[0] = accelzHigh + accelzLow;
 gyroDataX[0] = gyroxHigh + gyroxLow;
 gyroDataY[0] = gyroyHigh + gyroyLow;
 gyroDataZ[0] = gyrozHigh + gyrozLow;

}

int MPUread(uint16_t start, int *buffer, uint16_t sizeAdd)
{
  int inputByte;

  Wire.beginTransmission(MPUAddress);
  Wire.write(start);
  Wire.endTransmission(false);

  Wire.requestFrom(MPUAddress, sizeAdd, true);

  while (Wire.available())
  {
    inputByte = 0xFF & Wire.read();  
    *buffer = inputByte;
  } 
}

int MPUwrite(uint16_t start, const uint8_t *pData, uint16_t sizeAdd)
{

  Wire.beginTransmission(MPUAddress);
  Wire.write(start);        // write the start address
  Wire.write(pData, sizeAdd);  // write data bytes
  Wire.endTransmission(true); // release the I2C-bus

  return (0);         // return : no error
}

//Single register write
int MPUwrite_reg(uint16_t reg, uint8_t data)
{
  MPUwrite(reg, &data, 1);

}

void setupMPU() {

  //Function to set MPU registers
  //Set SampleRate Divider to 1/8 or 1000Hz
  MPUwrite_reg(MPU6050_SMPLRT_DIV, 0x07);
  //Disable FSync and DLPF (Digital LowPass Filter)
  //DLPF set to 00000100
  //1kHz sample period
  //8.5 and 8.3 delays for accel and gyro resp.
  MPUwrite_reg(MPU6050_CONFIG, 0x00);
  //Disable Gyro Self Tests, set range +-500 deg/sec
  MPUwrite_reg(MPU6050_GYRO_CONFIG, 0x07);//11101000
  //Disable Accel Self Tests, Set range to +-4g
  MPUwrite_reg(MPU6050_ACCEL_CONFIG, 0x07);

  //Disable a bunch of stuff
  MPUwrite_reg( MPU6050_FF_THR, 0x00);
  MPUwrite_reg(MPU6050_FF_DUR, 0x00);
  MPUwrite_reg(MPU6050_MOT_THR, 0x00);
  MPUwrite_reg(MPU6050_MOT_DUR, 0x00);
  MPUwrite_reg(MPU6050_ZRMOT_THR, 0x00);
  MPUwrite_reg(MPU6050_ZRMOT_DUR, 0x00);
  MPUwrite_reg(MPU6050_FIFO_EN, 0x00);
  MPUwrite_reg(MPU6050_ZRMOT_DUR, 0x00);
  MPUwrite_reg(MPU6050_ZRMOT_DUR, 0x00);
  MPUwrite_reg(MPU6050_ZRMOT_DUR, 0x00);

  //Master Control set to Zero
  MPUwrite_reg(MPU6050_I2C_MST_CTRL, 0x00);
  //Setup AUX I2C Slaves
  MPUwrite_reg(MPU6050_I2C_SLV0_ADDR, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV0_REG, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV0_CTRL, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV1_ADDR, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV1_REG, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV1_CTRL, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV2_ADDR, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV2_REG, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV2_CTRL, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV3_ADDR, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV3_REG, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV3_CTRL, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV4_ADDR, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV4_REG, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV4_DO, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV4_CTRL, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV4_DI, 0x00);
  MPUwrite_reg(MPU6050_I2C_MST_STATUS, 0x00);
  MPUwrite_reg(MPU6050_INT_PIN_CFG, 0x00);
  MPUwrite_reg(MPU6050_INT_ENABLE, 0x00);

  //READ ONLY REGISTERS
  /*
  MPUread(MPU6050_INT_STATUS, 0x00);
   MPUread(MPU6050_ACCEL_XOUT_H, 0x00);
   MPUread(MPU6050_ACCEL_XOUT_L, 0x00);
   MPUread(MPU6050_ACCEL_YOUT_H, 0x00);
   MPUread(MPU6050_ACCEL_YOUT_L, 0x00);
   MPUread(MPU6050_ACCEL_ZOUT_H, 0x00);
   MPUread(MPU6050_ACCEL_ZOUT_L, 0x00);
   MPUread(MPU6050_TEMP_OUT_H, 0x00);
   MPUread(MPU6050_TEMP_OUT_L, 0x00);
   MPUread(MPU6050_GYRO_XOUT_H, 0x00);
   MPUread(MPU6050_GYRO_XOUT_L, 0x00);
   MPUread(MPU6050_GYRO_YOUT_H, 0x00);
   MPUread(MPU6050_GYRO_YOUT_L, 0x00);
   MPUread(MPU6050_GYRO_ZOUT_H, 0x00);
   MPUread(MPU6050_GYRO_ZOUT_L, 0x00);
   
   */

  //Slave Data out and on
  MPUwrite_reg(MPU6050_I2C_SLV0_DO, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV1_DO, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV2_DO, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV3_DO, 0x00);
  MPUwrite_reg(MPU6050_I2C_MST_DELAY_CTRL, 0x00);
  MPUwrite_reg(MPU6050_SIGNAL_PATH_RESET, 0x00);
  MPUwrite_reg(MPU6050_MOT_DETECT_CTRL, 0x00);
  MPUwrite_reg(MPU6050_USER_CTRL, 0x00);
  MPUwrite_reg(MPU6050_PWR_MGMT_1, 0x00);
  MPUwrite_reg(MPU6050_PWR_MGMT_2, 0x00);
  MPUwrite_reg(MPU6050_FIFO_COUNTH, 0x00);
  MPUwrite_reg(MPU6050_FIFO_COUNTL, 0x00);
  MPUwrite_reg(MPU6050_FIFO_R_W, 0x00);

	return true;
}

