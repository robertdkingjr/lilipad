/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __SYSTEM_COMMAND_H__
#define __SYSTEM_COMMAND_H__

typedef enum
{
	WCT_CMD_NONE           = 0,
	WCT_CMD_STOP_LIB       = 1,
	WCT_CMD_START_LIB      = 2,
	WCT_CMD_RE_CALIBRATION = 3,
	WCT_CMD_QF_COEFF_TEMP_CALIBRATION = 4
}CMD_TYPE_E;

void CMD_Handler(void);
void CMD_Notify(CMD_TYPE_E eCmd);

#endif
