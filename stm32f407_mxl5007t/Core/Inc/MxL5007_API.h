/*
 
 Driver APIs for MxL5007 Tuner
 
 Copyright, Maxlinear, Inc.
 All Rights Reserved
 
 File Name:      MxL5007_API.h
 
 18-08-2025 Maciej Fajfer - porting to STM32F407, modifications and improvements
 */
#ifndef __MxL5007_API_H
#define __MxL5007_API_H

#include <stdint.h>
#include "MxL5007_Common.h"
#include "main.h"

//This coefficients were obtained experimentally based on Gain [dB] = f(V_if_agc) curve at f_rf=100 MHz and then least squares approximation.
//V_if_agc -> 0.3 ... 1.8 V gives about -9.5 dB ... 80 dB
//V_if_agc voltage was measured directly on the VAGC pin (27) of MxL5007.
//Gain [dB] = 65.441*V_if_agc [V] - 36.681  ->  V_if_agc [V] = 1.5281E-2*Gain [dB] + 5.6052E-1
//It's total tuner's gain (without IF outputs loading) so it's Gain=10*lo10(V_if_diff/V_rf) because with current registers settings first AGC is turned off.
#define A_MxL_V_if_agc 1.5281E-2
#define B_MxL_V_if_agc 5.6052E-1

/******************************************************************************
**
**  Name: MxL_Set_Register
**
**  Description:    Write one register to MxL5007
**
**  Parameters:    	myTuner				- Pointer to MxL5007_TunerConfigS
**					RegAddr				- Register address to be written
**					RegData				- Data to be written
**
**  Returns:        MxL_ERR_MSG			- MxL_OK if success	
**										- MxL_ERR_SET_REG if fail
**
******************************************************************************/
MxL_ERR_MSG MxL_Set_Register(MxL5007_TunerConfigS* myTuner, uint8_t RegAddr, uint8_t RegData);

/******************************************************************************
**
**  Name: MxL_Get_Register
**
**  Description:    Read one register from MxL5007
**
**  Parameters:    	myTuner				- Pointer to MxL5007_TunerConfigS
**					RegAddr				- Register address to be read
**					RegData				- Pointer to register read
**
**  Returns:        MxL_ERR_MSG			- MxL_OK if success	
**										- MxL_ERR_GET_REG if fail
**
******************************************************************************/
MxL_ERR_MSG MxL_Get_Register(MxL5007_TunerConfigS* myTuner, uint8_t RegAddr, uint8_t *RegData);

/******************************************************************************
**
**  Name: MxL_Tuner_Init
**
**  Description:    MxL5007 Initialization
**
**  Parameters:    	myTuner				- Pointer to MxL5007_TunerConfigS
**
**  Returns:        MxL_ERR_MSG			- MxL_OK if success	
**										- MxL_ERR_INIT if fail
**
******************************************************************************/
MxL_ERR_MSG MxL_Tuner_Init(MxL5007_TunerConfigS* );

/******************************************************************************
**
**  Name: MxL_Tuner_RFTune
**
**  Description:    Frequency tunning for channel
**
**  Parameters:    	myTuner				- Pointer to MxL5007_TunerConfigS
**					RF_Freq_Hz			- RF Frequency in Hz
**					BWMHz				- Bandwidth 6, 7 or 8 MHz
**
**  Returns:        MxL_ERR_MSG			- MxL_OK if success	
**										- MxL_ERR_RFTUNE if fail
**
******************************************************************************/
MxL_ERR_MSG MxL_Tuner_RFTune(MxL5007_TunerConfigS*, uint32_t RF_Freq_Hz, MxL5007_BW_MHz BWMHz);

/******************************************************************************
**
**  Name: MxL_Soft_Reset
**
**  Description:    Software Reset the MxL5007 Tuner
**
**  Parameters:    	myTuner				- Pointer to MxL5007_TunerConfigS
**
**  Returns:        MxL_ERR_MSG			- MxL_OK if success	
**										- MxL_ERR_OTHERS if fail
**
******************************************************************************/
MxL_ERR_MSG MxL_Soft_Reset(MxL5007_TunerConfigS*);

/******************************************************************************
**
**  Name: MxL_Loop_Through_On
**
**  Description:    Turn On/Off on-chip Loop-through
**
**  Parameters:    	myTuner				- Pointer to MxL5007_TunerConfigS
**					isOn				- True to turn On Loop Through
**										- False to turn off Loop Through
**
**  Returns:        MxL_ERR_MSG			- MxL_OK if success	
**										- MxL_ERR_OTHERS if fail
**
******************************************************************************/
MxL_ERR_MSG MxL_Loop_Through_On(MxL5007_TunerConfigS*, MxL5007_LoopThru);

/******************************************************************************
**
**  Name: MxL_Standby
**
**  Description:    Enter Standby Mode
**
**  Parameters:    	myTuner				- Pointer to MxL5007_TunerConfigS
**
**  Returns:        MxL_ERR_MSG			- MxL_OK if success	
**										- MxL_ERR_OTHERS if fail
**
******************************************************************************/
MxL_ERR_MSG MxL_Stand_By(MxL5007_TunerConfigS*);

/******************************************************************************
**
**  Name: MxL_Wakeup
**
**  Description:    Wakeup from Standby Mode (Note: after wake up, please call RF_Tune again)
**
**  Parameters:    	myTuner				- Pointer to MxL5007_TunerConfigS
**
**  Returns:        MxL_ERR_MSG			- MxL_OK if success	
**										- MxL_ERR_OTHERS if fail
**
******************************************************************************/
MxL_ERR_MSG MxL_Wake_Up(MxL5007_TunerConfigS*);

/******************************************************************************
**
**  Name: MxL_Check_ChipVersion
**
**  Description:    Return the MxL5007 Chip ID
**
**  Parameters:    	myTuner				- Pointer to MxL5007_TunerConfigS
**			
**  Returns:        MxL_ChipVersion			
**
******************************************************************************/
MxL5007_ChipVersion MxL_Check_ChipVersion(MxL5007_TunerConfigS*);

/******************************************************************************
**
**  Name: MxL_Print_ChipVersion
**
**  Description:    Print the MxL5007 Chip Version in UART console
**
**  Parameters:    	MxL_ChipVersion				- value provided by function MxL_Check_ChipVersion
**
**  Returns:        void
**
******************************************************************************/
void MxL_Print_ChipVersion(MxL5007_ChipVersion MxL_ChipVersion);

/******************************************************************************
**
**  Name: MxL_RFSynth_Lock_Status
**
**  Description:    RF synthesizer lock status of MxL5007
**
**  Parameters:    	myTuner				- Pointer to MxL5007_TunerConfigS
**					isLock				- Pointer to Lock Status
**
**  Returns:        MxL_ERR_MSG			- MxL_OK if success	
**										- MxL_ERR_OTHERS if fail
**
******************************************************************************/
MxL_ERR_MSG MxL_RFSynth_Lock_Status(MxL5007_TunerConfigS* , bool* isLock);

/******************************************************************************
**
**  Name: MxL_REFSynth_Lock_Status
**
**  Description:    REF synthesizer lock status of MxL5007
**
**  Parameters:    	myTuner				- Pointer to MxL5007_TunerConfigS
**					isLock				- Pointer to Lock Status
**
**  Returns:        MxL_ERR_MSG			- MxL_OK if success	
**										- MxL_ERR_OTHERS if fail	
**
******************************************************************************/
MxL_ERR_MSG MxL_REFSynth_Lock_Status(MxL5007_TunerConfigS* , bool* isLock);

/******************************************************************************
**
**  Name: MxL_SetGain
**
**  Description:    REF synthesizer lock status of MxL5007
**
**  Parameters:    	gain				- overall tuner's gain in dB
**
**  Returns:        nothing
**
******************************************************************************/
void MxL_SetGain(float gain);

/******************************************************************************
**
**  Name: MxL_Get_RSSI
**
**  Description:    get RSSI value - Most probably this experimental function is worthless.
**
**  Parameters:    	myTuner				- Pointer to MxL5007_TunerConfigS
**  				RSSI_int			- Pointer to RSSI integer storage variable
**
**  Returns:        MxL_ERR_MSG			- MxL_OK if success
**										- MxL_ERR_OTHERS if fail
**
******************************************************************************/
MxL_ERR_MSG MxL_Get_RSSI(MxL5007_TunerConfigS* myTuner, uint16_t* RSSI_int);

#endif //__MxL5007_API_H
