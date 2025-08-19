/*
 
 Driver APIs for MxL5007 Tuner
 
 Copyright, Maxlinear, Inc.
 All Rights Reserved
 
 File Name:      MxL_User_Define.c

 18-08-2025 Maciej Fajfer - porting to STM32F407, modifications and improvements
 */
#include "main.h"
#include "printf.h"
#include "MxL_User_Define.h"

#define I2C_time_out 50
extern I2C_HandleTypeDef hi2c3; //handle to I2C3

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//																		   //
//					I2C Functions (implement by customer)				   //
//																		   //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

/*
 * exception handler for I2C timeout
 */
void MxL_TIMEOUT_UserCallback()
{
	UART_printf("\r\nFailed to communicate with MXL5007T\r\n");
	/* Block communication and all processes */
	while (1);
}

/******************************************************************************
**
**  Name: MxL_I2C_Write
**
**  Description:    I2C write operations
**
**  Parameters:    	
**					DeviceAddr	- MxL5007 Device address
**					pArray		- Write data array pointer
**					count		- total number of array
**
**  Returns:        0 if success
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   12-16-2007   khuang initial release.
**
******************************************************************************/
uint32_t MxL_I2C_Write(uint8_t DeviceAddr, uint8_t* pArray, uint32_t count)
{
	//HAL_OK that is return by HAL_I2C_Master_Transmit is equal to zero so it's compliant with Maxlinear's driver implementation.
	return HAL_I2C_Master_Transmit(&hi2c3, DeviceAddr<<1, pArray, count, I2C_time_out);
}

/******************************************************************************
**
**  Name: MxL_I2C_Read
**
**  Description:    I2C read operations
**
**  Parameters:    	
**					DeviceAddr	- MxL5007 Device address
**					Addr		- register address for read
**					*Data		- data return
**
**  Returns:        0 if success
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   12-16-2007   khuang initial release.
**
******************************************************************************/
uint32_t MxL_I2C_Read(uint8_t DeviceAddr, uint8_t Addr, uint8_t* mData)
{
	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c3, DeviceAddr<<1, 0xFB, I2C_MEMADD_SIZE_8BIT, &Addr, 1, I2C_time_out);
	if (status != HAL_OK)
		return status;

	status = HAL_I2C_Master_Receive(&hi2c3, DeviceAddr<<1, mData, 1, I2C_time_out);
	if (status != HAL_OK)
			return status;

	return 0; //returns 0 if success
}

/******************************************************************************
**
**  Name: MxL_Delay
**
**  Description:    Delay function in milli-second
**
**  Parameters:    	
**					mSec		- milli-second to delay
**
**  Returns:        0
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   12-16-2007   khuang initial release.
**
******************************************************************************/
void MxL_Delay(uint32_t mSec)
{
	HAL_Delay(mSec);
}

/******************************************************************************
**
**  Name: MxL_Set_IF_AGC_Volatge
**
**  Description:    setting IF AGC voltage - it can be implemented as DAC or PWM with low pass filter -> 0...3.000 V
**
**  Parameters:
**					V_if_agc		- IF AGC voltage
**
**  Returns:        nothing
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   18-08-2025   Maciej Fajfer initial release.
**
******************************************************************************/
void MxL_Set_IF_AGC_Volatge(float V_if_agc)
{
	int32_t TIM4_CCR_val = V_if_agc * 8192.0/3.0 * K_corr_coeff; //calculating value for TIM4_CCR register
	if (TIM4_CCR_val < 0) TIM4_CCR_val = 0;
	if (TIM4_CCR_val > 8191) TIM4_CCR_val = 8191;
	TIM4->CCR2 =  TIM4_CCR_val; //duty cycle update
}
