/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/
//----------------------
// APPLICATION INCLUDES
//----------------------

#include "defines.h"

#include "appCfg.h"
#include "wct_lib.h"
#include "wct_LibParams.h"

#include "hal.h"

#include "systemPrint.h"
#include "systemTimers.h"
#include "systemDisplay.h"
#include "systemTask.h"

#include "systemCallback.h"

/*Called in DDM ISR. 
 *Caution:
 *1. Don't call function that not support reentry
 *2. The execution time should be as less as possible, say less than 100us.
 */
void WCT_OnWPCPacketRecv(uint8 byDeviceId, uint8 bySize, uint8 *pbyData)
{
	//HAL_SetTP(0, HAL_TP_TYPE_TOG);
		
	/*
	uint8 i;

	APP_PrintString( "\r\n" );
	APP_PrintString(", DT: ");
	for (i=0; i<bySize; i++)
	{
		APP_PrintHexChar( pbyData[ i ] );
		APP_PrintString(", ");
	}
	APP_PrintString( "\r\n" );
	*/
}

//Set the recharge retry time for abnormal cases.
//return time value in unit of ms; if equal 255 min, it means infinity
uint32 WCT_SetReChargeTimeOnAbnormal(E_RECHARGETIME_SETTYPE eAbnormalType)
{
	uint32 dwTime = 0;
	gWCT_Params.uCtrlBit.bQfactorRetry          = 0;
	switch (eAbnormalType)
	{
		case RECHARGETIME_RX_UNKNOWN:
			dwTime = 0UL;
			break;
		case RECHARGETIME_RX_CHARGE_COMPLETE:
			dwTime = 60000UL * 5UL;                 // 5 mins
			break;
		case RECHARGETIME_RX_INTERNAL_FAULT:
			dwTime = 0UL;
			break;
		case RECHARGETIME_RX_OVER_TEMP:
			dwTime = 60000UL * 5UL;
			break;
		case RECHARGETIME_RX_OVER_VOLT:
			dwTime = 0UL;
			break;
		case RECHARGETIME_RX_OVER_CURRENT:
			dwTime = 0UL;
			break;
		case RECHARGETIME_RX_BATTERY_FAILURE:
			dwTime = FATAL_FAULT_TIMEOUT;
			break;
		case RECHARGETIME_RX_NO_RESPONSE:
			dwTime = 0UL;
			break;
		case RECHARGETIME_RX_RESTART_POWERXFER:
			dwTime = 0UL;
			break;
		case RECHARGETIME_RX_NEGOTIATION_FAILURE:
			dwTime = 0UL;
			break;		
		case RECHARGETIME_TX_RCVPWR_TIMEOUT:
			dwTime = 0UL;
			break;
		case RECHARGETIME_RX_POWER_BEYOND_CAPABILITY:
			dwTime = 60000UL * 5UL;
			break;
		case RECHARGETIME_TX_FOD_ERROR:
			gWCT_Params.uCtrlBit.bQfactorRetry          = 1;
			dwTime = 60000UL * 5UL;
			break;
		case RECHARGETIME_TX_PREFOD_ERROR:
			gWCT_Params.uCtrlBit.bQfactorRetry          = 1;
			dwTime = FATAL_FAULT_TIMEOUT;
			break;
		case RECHARGETIME_TX_QFOD_ERROR:
			gWCT_Params.uCtrlBit.bQfactorRetry          = 1;
            dwTime = 60000UL * 5UL;
            break;
		case RECHARGETIME_TX_CHARGE_REPEATED_FAIL:
			dwTime = 60000UL * 5UL;
			break;
		default:
			dwTime = 0UL;
            break;
	}
	return dwTime;
}

void WCT_UpdateDevUsrIndication(uint8 byDeviceId)
{
    TX_CHARGING_STATUS TxStatus = WCT_GetTxStatus(byDeviceId);
    TX_CHARGING_ERRORS TxErrors = WCT_GetTxError(byDeviceId);
    RX_CHARGING_STATUS RxStatus = WCT_GetRxStatus(byDeviceId);
    RX_CHARGING_ERRORS RxErrors = WCT_GetRxError(byDeviceId);
    
    APP_ERROR_TYPE app_error = App_GetErr();
    if(app_error != APP_SUCCESS)
    {
		switch(app_error)
		{
			case APP_NVM_ERROR:
				DISP_SetMode(0, HAL_RED_LED_INDEX, DISP_MODE_ON, 0);
				DISP_SetMode(0, HAL_GRN_LED_INDEX, DISP_MODE_OFF, 0);
				break;
				
			case APP_OVP_ERROR:
			case APP_UVP_ERROR:				
			case APP_OTP_ERROR:
				DISP_SetMode(0, HAL_RED_LED_INDEX, DISP_MODE_BLINK, 1000);
				DISP_SetMode(0, HAL_GRN_LED_INDEX, DISP_MODE_OFF, 0);
				break;
				
			default:
				break;
		}
    }
    else if (TX_SUCCESS != TxErrors)
	{
		switch (TxErrors)
		{
			case TX_CHIP_ERROR:
			case TX_PRE_FOD_ERROR:
			case TX_FOD_ERROR:
			case TX_QFOD_ERROR:
			case TX_CHARGE_REPEATED_FAIL:
				DISP_SetMode(byDeviceId, HAL_RED_LED_INDEX, DISP_MODE_ON, 0);
				DISP_SetMode(byDeviceId, HAL_GRN_LED_INDEX, DISP_MODE_OFF, 0);
				break;
			default:
				break;
		}
	}
	else if (RX_SUCCESS != RxErrors)
	{
		switch (RxErrors)
		{
			default:
				DISP_SetMode(byDeviceId, HAL_RED_LED_INDEX, DISP_MODE_ON, 0);
				DISP_SetMode(byDeviceId, HAL_GRN_LED_INDEX, DISP_MODE_OFF, 0);
				break;
		}
	}
	else
	{
		//OK, everything goes normal, DO show something here
		switch (RxStatus)
		{
			case RX_PREPARE_CHARGE:   // used as indication for RX detected
				// set display mode for RX detected
				DISP_SetMode(byDeviceId, HAL_RED_LED_INDEX, DISP_MODE_BLINK, 2000);
				DISP_SetMode(byDeviceId, HAL_GRN_LED_INDEX, DISP_MODE_ON, 0);
				break;
			case RX_NONE:
				// set display mode for RX removed
				DISP_SetMode(byDeviceId, HAL_RED_LED_INDEX, DISP_MODE_OFF, 0);
				DISP_SetMode(byDeviceId, HAL_GRN_LED_INDEX, DISP_MODE_BLINK, 2000);
				break;
			case RX_CHARGED:
				DISP_SetMode(byDeviceId, HAL_RED_LED_INDEX, DISP_MODE_OFF, 0);
				DISP_SetMode(byDeviceId, HAL_GRN_LED_INDEX, DISP_MODE_ON, 0);
				break;
			default:
				break;
		}
	}
}

