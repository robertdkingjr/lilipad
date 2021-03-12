/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

typedef enum
{
	DISP_MODE_ON = 0,
	DISP_MODE_OFF,
	DISP_MODE_BLINK,
	DISP_MODE_TOG,
	DISP_MODE_DIM,
	DISP_MODE_MAX
}DISP_MODE_E;

void DISP_Init(void);
void DISP_SetMode(uint8 byDeviceId, HAL_LED_INDEX_E byLedId, DISP_MODE_E eMode, uint32 dwInterval);
void DISP_Handler(uint16 wTimePassedMs);

#endif
