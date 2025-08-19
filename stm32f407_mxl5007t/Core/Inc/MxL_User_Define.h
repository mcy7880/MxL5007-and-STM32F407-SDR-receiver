/*

 Driver APIs for MxL5007 Tuner

 Copyright, Maxlinear, Inc.
 All Rights Reserved

 File Name:      MxL_User_Define.h

 18-08-2025 Maciej Fajfer - porting to STM32F407, modifications and improvements
 */
#ifndef __MxL_User_Define_H
#define __MxL_User_Define_H

#define K_corr_coeff   1.3119 //correction coefficient - it's important due to load that's contribute by IF_AGC block to the low pass filter for TIMER4 PWM

void MxL_TIMEOUT_UserCallback();
uint32_t MxL_I2C_Write(uint8_t DeviceAddr, uint8_t* pArray, uint32_t count);
uint32_t MxL_I2C_Read(uint8_t DeviceAddr, uint8_t Addr, uint8_t* mData);
void MxL_Delay(uint32_t mSec);
void MxL_Set_IF_AGC_Volatge(float V_if_agc);

#endif //__MxL_User_Define_H
