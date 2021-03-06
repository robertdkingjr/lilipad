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
#include "appcfg.h"

#include "systemDebug.h"

#include "pmc.h"

void PMC_Init(void)
{
	uint16 wtemp;
		
	PMC_STS = 0x3CU;
	PMC_CTRL &= ~PMC_CTRL_TRIM;
	wtemp = ((uint16) (SIM_NVMOPT2L & ((uint16) SIM_NVMOPT2L_PMC_BGTRIM) ));
	PMC_CTRL |= ((vuint16_t) wtemp);
}


