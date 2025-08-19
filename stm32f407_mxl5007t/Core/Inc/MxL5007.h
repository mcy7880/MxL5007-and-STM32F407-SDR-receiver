/*
 
 Driver APIs for MxL5007 Tuner
 
 Copyright, Maxlinear, Inc.
 All Rights Reserved
 
 File Name:      MxL5007.h

 18-08-2025 Maciej Fajfer - porting to STM32F407, modifications and improvements
 */
#ifndef __MxL5007_Common_H
#define __MxL5007_Common_H

#include "MxL5007_Common.h"


typedef struct
{
	uint8_t Num;	//Register number
	uint8_t Val;	//Register value
} IRVType, *PIRVType;


uint32_t MxL5007_Init(uint8_t* pArray,				// a array pointer that store the addr and data pairs for I2C write
					uint32_t* Array_Size,			// a integer pointer that store the number of element in above array
					uint8_t Mode,
					int32_t IF_Diff_Out_Level,
					uint32_t Xtal_Freq_Hz,
					uint32_t IF_Freq_Hz,
					uint8_t Invert_IF,
					uint8_t Clk_Out_Enable,
					uint8_t Clk_Out_Amp
					);
uint32_t MxL5007_RFTune(uint8_t* pArray, uint32_t* Array_Size,
					 uint32_t RF_Freq,			// RF Frequency in Hz
					 uint8_t BWMHz		// Bandwidth in MHz
					 );
uint32_t SetIRVBit(PIRVType pIRV, uint8_t Num, uint8_t Mask, uint8_t Val);

#endif //__MxL5007_Common_H
