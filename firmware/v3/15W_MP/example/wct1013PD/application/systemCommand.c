/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#include "defines.h"

#include "appCfg.h"
#include "Wct_Lib.h"

#include "NVM.h"

#include "wct_debug.h"

#include "systemCommand.h"

CMD_TYPE_E gCMD_eCommand;

void CMD_Handler(void)
{
	if(WCT_CMD_NONE != gCMD_eCommand)
	{
		switch(gCMD_eCommand)
		{
		case WCT_CMD_STOP_LIB:   //1
			WCT_Stop();
			break;
			
		case WCT_CMD_START_LIB:  //2
			WCT_LibInit();
			break;
			
		case WCT_CMD_RE_CALIBRATION:  //3
		    //For debug purpose
			//This command should be issued after WCT STOP
			NVM_ReCalibration();
			break;
			
		case WCT_CMD_QF_COEFF_TEMP_CALIBRATION:
			/*
			* This command is intended to calibrate swImpedanceChangePerCelsius.
			* Steps below should be followed, and there should be no objects on Tx surface for all steps below.
			* 1. Make the board on temperature T1, enter WCT_CMD_STOP_LIB to gCmd in FM, which will stop WCT library
			* 2. Enter WCT_CMD_RE_CALIBRATION to gCmd in FM, which will re-do the calibration
			* 	 Note: step 1~2 may be skipped, since the calibration will be executed at first time power on after firmware downloaded
			* 3. Make the board on temperature T2, make sure |T2-T1| > 20, otherwise the coefficient accuracy can't be guaranteed
			* 4. Enter command WCT_CMD_STOP_LIB to gCmd in FM, which stops WCT library if step 1~2 are skipped
			* 5. Enter command WCT_CMD_QF_COEFF_TEMP_CALIBRATION to gCmd in FM, which will generate the coefficient swImpedanceChangePerCelsius
			* 6. Enter command WCT_CMD_START_LIB to gCmd in FM, which re-start WCT library
			* 
			* One simple example:
			* T1 = 15 celsius degree(ambient temperature) after firmware downloaded or do steps 1~2 above,
			* put a Rx on Tx, Rx with heavy loading, say 15W, make the charging keeps one hour or longer to heat the Tx board,
			* then remove the Rx and do steps 4~6 above immediately.
			*/
			WBG_QfTemperatureCoeffiecntCalibration();
			break;
		}
		gCMD_eCommand = WCT_CMD_NONE;
	}
}

//Should not be called in ISR, which leads critical status for gCMD_eCommand
void CMD_Notify(CMD_TYPE_E eCmd)
{
	gCMD_eCommand = eCmd;
}

