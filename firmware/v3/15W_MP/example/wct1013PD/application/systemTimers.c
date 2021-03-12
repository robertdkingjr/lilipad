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

#include "Wct_Lib.h"
#include "freemaster.h"

#include "hal.h"

#include "systemTimers.h"

volatile static uint16 gST_wTimerTick;

void ST_TimerUpdate(void)
{
	gST_wTimerTick++;
}

uint16 ST_GetTimerTick(void)
{
	return gST_wTimerTick;
}

void ST_AddTickTimer(uint16 wTicks)
{
	uint8 sts = HAL_DisableIRQ();
	gST_wTimerTick += wTicks;
	HAL_RestoreIRQ(sts);
}

uint16 ST_GetElapasedTime(uint16 wLastTick)
{
	uint16 ticks = ST_GetTimerTick();
	uint16 wElapsedTime;
	
	if (ticks >= wLastTick)
	{
		wElapsedTime = ticks - wLastTick;
	}
	else
	{
	    //counter wrap
		wElapsedTime = ticks + (65536UL - wLastTick);
	}
	
	return wElapsedTime;
}

void ST_WaitMs(uint16 wNumMs)
{
	uint16 ticks = ST_GetTimerTick();

	while (ST_GetElapasedTime(ticks) < wNumMs)
	{
	    //Kick the watch dog
		HAL_KickWatchDog();
#if FREEMASTER_SUPPORTED
        FMSTR_Poll();
#endif
	}
}

