/*

 Driver APIs for MxL5007 Tuner
 
 Copyright, Maxlinear, Inc.
 All Rights Reserved
 
 File Name:      MxL5007_API.c
 
 Version:    4.1.3

 18-08-2025 Maciej Fajfer - porting to STM32F407, modifications and improvements
*/


//#include "StdAfx.h"
#include <stdbool.h>
#include "MxL5007_API.h"
#include "MxL_User_Define.h"
#include "MxL5007.h"
#include "printf.h"


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//																		   //
//							Tuner Functions								   //
//																		   //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
MxL_ERR_MSG MxL_Set_Register(MxL5007_TunerConfigS* myTuner, uint8_t RegAddr, uint8_t RegData)
{
	uint32_t Status=0;
	uint8_t pArray[2];
	pArray[0] = RegAddr;
	pArray[1] = RegData;
	Status = MxL_I2C_Write((uint8_t)myTuner->I2C_Addr, pArray, 2);
	if(Status) return MxL_ERR_SET_REG;

	return MxL_OK;

}

MxL_ERR_MSG MxL_Get_Register(MxL5007_TunerConfigS* myTuner, uint8_t RegAddr, uint8_t *RegData)
{
	if(MxL_I2C_Read((uint8_t)myTuner->I2C_Addr, RegAddr, RegData))
		return MxL_ERR_GET_REG;
	return MxL_OK;

}

MxL_ERR_MSG MxL_Soft_Reset(MxL5007_TunerConfigS* myTuner)
{
	uint8_t reg_reset;
	reg_reset = 0xFF;
	if(MxL_I2C_Write((uint8_t)myTuner->I2C_Addr, &reg_reset, 1))
		return MxL_ERR_OTHERS;

	return MxL_OK;
}

MxL_ERR_MSG MxL_Loop_Through_On(MxL5007_TunerConfigS* myTuner, MxL5007_LoopThru isOn)
{	
	uint8_t pArray[2];	// a array pointer that store the addr and data pairs for I2C write
	
	pArray[0]=0x04;
	if(isOn)
	 pArray[1]= 0x01;
	else
	 pArray[1]= 0x0;

	if(MxL_I2C_Write((uint8_t)myTuner->I2C_Addr, pArray, 2))
		return MxL_ERR_OTHERS;

	return MxL_OK;
}

MxL_ERR_MSG MxL_Stand_By(MxL5007_TunerConfigS* myTuner)
{
	uint8_t pArray[4];	// a array pointer that store the addr and data pairs for I2C write
	
	pArray[0] = 0x01;
	pArray[1] = 0x0;
	pArray[2] = 0x0F;
	pArray[3] = 0x0;

	if(MxL_I2C_Write((uint8_t)myTuner->I2C_Addr, pArray, 4))
		return MxL_ERR_OTHERS;

	return MxL_OK;
}

MxL_ERR_MSG MxL_Wake_Up(MxL5007_TunerConfigS* myTuner)
{
	uint8_t pArray[2];	// a array pointer that store the addr and data pairs for I2C write
	
	pArray[0] = 0x01;
	pArray[1] = 0x01;

	if(MxL_I2C_Write((uint8_t)myTuner->I2C_Addr, pArray, 2))
		return MxL_ERR_OTHERS;

	if(MxL_Tuner_RFTune(myTuner, myTuner->RF_Freq_Hz, myTuner->BW_MHz))
		return MxL_ERR_RFTUNE;

	return MxL_OK;
}

MxL_ERR_MSG MxL_Tuner_Init(MxL5007_TunerConfigS* myTuner)
{	
	uint8_t pArray[MAX_ARRAY_SIZE];	// a array pointer that store the addr and data pairs for I2C write
	uint32_t Array_Size;							// a integer pointer that store the number of element in above array

	//Soft reset tuner
	if(MxL_Soft_Reset(myTuner))
		return MxL_ERR_INIT;

	//perform initialization calculation
	MxL5007_Init(pArray, &Array_Size, (uint8_t)myTuner->Mode, myTuner->IF_Diff_Out_Level, (uint32_t)myTuner->Xtal_Freq,
				(uint32_t)myTuner->IF_Freq, (uint8_t)myTuner->IF_Spectrum, (uint8_t)myTuner->ClkOut_Setting, (uint8_t)myTuner->ClkOut_Amp);

	//perform I2C write here
	if(MxL_I2C_Write((uint8_t)myTuner->I2C_Addr, pArray, Array_Size))
		return MxL_ERR_INIT;

	return MxL_OK;
}


MxL_ERR_MSG MxL_Tuner_RFTune(MxL5007_TunerConfigS* myTuner, uint32_t RF_Freq_Hz, MxL5007_BW_MHz BWMHz)
{
	uint8_t pArray[MAX_ARRAY_SIZE];	// a array pointer that store the addr and data pairs for I2C write
	uint32_t Array_Size;							// a integer pointer that store the number of element in above array

	//Store information into struc
	myTuner->RF_Freq_Hz = RF_Freq_Hz;
	myTuner->BW_MHz = BWMHz;

	//perform Channel Change calculation
	MxL5007_RFTune(pArray,&Array_Size,RF_Freq_Hz,BWMHz);

	//perform I2C write here
	if(MxL_I2C_Write((uint8_t)myTuner->I2C_Addr, pArray, Array_Size))
		return MxL_ERR_RFTUNE;

	//wait for 3ms
	MxL_Delay(3); 

	return MxL_OK;
}

MxL5007_ChipVersion MxL_Check_ChipVersion(MxL5007_TunerConfigS* myTuner)
{	
	uint8_t Data;
	if(MxL_I2C_Read((uint8_t)myTuner->I2C_Addr, 0xD9, &Data)) //https://github.com/VPBox/Dev/blob/68c7654006f1c355b51d622398fc40884c6df2b7/kernel/drivers/media/tuners/mxl5007t.c#L822
		return MxL_GET_ID_FAIL;

	MxL5007_ChipVersion MxL_ChipVersion;
	switch (Data)
	{
		case MxL_5007_V1_F1:
			MxL_ChipVersion = MxL_5007_V1_F1;
			break;
		case MxL_5007_V1_F2:
			MxL_ChipVersion = MxL_5007_V1_F2;
			break;
		case MxL_5007_V2_100_F1:
			MxL_ChipVersion = MxL_5007_V2_100_F1;
			break;
		case MxL_5007_V2_100_F2:
			MxL_ChipVersion = MxL_5007_V2_100_F2;
			break;
		case MxL_5007_V2_200_F1:
			MxL_ChipVersion = MxL_5007_V2_200_F1;
			break;
		case MxL_5007_V2_200_F2:
			MxL_ChipVersion = MxL_5007_V2_200_F2;
			break;
		case MxL_5007_V4:
			MxL_ChipVersion = MxL_5007_V4;
			break;
		default:
			MxL_ChipVersion = MxL_UNKNOWN_ID;
		break;
	}

	return MxL_ChipVersion;
}

void MxL_Print_ChipVersion(MxL5007_ChipVersion MxL_ChipVersion)
{
	switch (MxL_ChipVersion)
	{
		case MxL_5007_V1_F1:
			UART_printf("MxL5007T_v1.f1 detected\r\n");
			break;

		case MxL_5007_V1_F2:
			UART_printf("MxL5007T_v1.f2 detected\r\n");
			break;

		case MxL_5007_V2_100_F1:
			UART_printf("MxL5007T_v2.100.f1 detected\r\n");
			break;

		case MxL_5007_V2_100_F2:
			UART_printf("MxL5007T_v2.100.f2 detected\r\n");
			break;

		case MxL_5007_V2_200_F1:
			UART_printf("MxL5007T_v2.200.f1 detected\r\n");
			break;

		case MxL_5007_V2_200_F2:
			UART_printf("MxL5007T_v2.200.f2 detected\r\n");
			break;

		case MxL_5007_V4:
			UART_printf("MxL5007T_v4 detected\r\n");
			break;

		default:
			UART_printf("it isn't MxL5007T chip");
			break;
	}
}

MxL_ERR_MSG MxL_RFSynth_Lock_Status(MxL5007_TunerConfigS* myTuner, bool* isLock)
{	
	uint8_t Data;
	*isLock = false;
	if(MxL_I2C_Read((uint8_t)myTuner->I2C_Addr, 0xD8, &Data))
		return MxL_ERR_OTHERS;
	Data &= 0x0C;
	if (Data == 0x0C)
		*isLock = true;  //RF Synthesizer is Lock
	return MxL_OK;
}

MxL_ERR_MSG MxL_REFSynth_Lock_Status(MxL5007_TunerConfigS* myTuner, bool* isLock)
{
	uint8_t Data;
	*isLock = false;
	if(MxL_I2C_Read((uint8_t)myTuner->I2C_Addr, 0xD8, &Data))
		return MxL_ERR_OTHERS;
	Data &= 0x03;
	if (Data == 0x03)
		*isLock = true;   //REF Synthesizer is Lock
	return MxL_OK;
}

void MxL_SetGain(float gain)
{
	float V_if_agc = A_MxL_V_if_agc*gain + B_MxL_V_if_agc;
	MxL_Set_IF_AGC_Volatge(V_if_agc);
}

//According to reverse engineering of the MxL5007 it looks like that 0xAD and 0xAE registers contains word that are
//dependents on both: V_if_agc voltage and RF input level. Most probably this experimental function is worthless.
MxL_ERR_MSG MxL_Get_RSSI(MxL5007_TunerConfigS* myTuner, uint16_t* RSSI_int)
{
	uint8_t d1, d2, i;
	uint16_t tmp = 0;

	for (i=0;i<10;i++) //getting max result from 10 readings
	{
		if(MxL_I2C_Read((uint8_t)myTuner->I2C_Addr, 0xAE, &d1))
			return MxL_ERR_OTHERS;

		if(MxL_I2C_Read((uint8_t)myTuner->I2C_Addr, 0xAD, &d2)) //it looks like 0xAD contains some flags
			return MxL_ERR_OTHERS;

		*RSSI_int = ((d2 & 0x3E)<<7) | d1;
		if (*RSSI_int > tmp) tmp = *RSSI_int;
	}
	*RSSI_int = tmp;

	return MxL_OK;
}
