/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#include "defines.h"
#include "cpu.h"

#include "wdg.h"

void WDG_Init(void)
{
#ifndef _DEBUG
    // Disable the timer
    COP_CTRL = 0u;
    // Set the timeout counter value
    COP_TOUT = 0xA4CCU;
    // Re-initialize the timer, 8M ROSC clock, divider=256, WDG enable, COP write protection disabled
    // timeout period = COP_TOUT/(8M/256) = 1.35s
    COP_CTRL = 0x0202U;
#else
    COP_CTRL = 0x0300U;
#endif
}

void WDG_Kick(void)
{
#ifndef _DEBUG
    //service sequence routine, 0x5555 followed by 0xAAAA
	COP_CNTR = 0x5555u;
	COP_CNTR = 0xAAAAu;
#endif
}

void WDG_Enable(uint8 byIsEn)
{
#ifndef _DEBUG
    //Note: the CWP bit in COP_CTRL must be 0
	if(byIsEn)
	{
		COP_CTRL |= COP_CTRL_CSEN;
	}
	else
	{
		COP_CTRL &= ~COP_CTRL_CSEN;
	}
#endif
}
