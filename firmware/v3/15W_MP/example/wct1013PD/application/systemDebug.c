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
#include "freemaster.h"

#include "hal.h"

#include "systemDebug.h"


//DEBUG MODULE

volatile uint32 gDBG_dwAssertCode, gDBG_dwAssertParam;
volatile uint32 gDBG_dwWarningCode, gDBG_dwWarningParam;

#ifdef _DEBUG

void DBG_Assert(uint8 byAssert, uint32 dwAssertCode, uint32 dwParameter)
{
	if(byAssert)
	{
		uint8 i;
		for(i=0; i<NUM_DEVICES; i++)
		{
			HAL_EnableWCT(i, 0);
		}
		gDBG_dwAssertCode  = dwAssertCode;
		gDBG_dwAssertParam = dwParameter;
		while(1) 
        {
#if FREEMASTER_SUPPORTED
            FMSTR_Poll();
#endif
        }
	}
}

void DBG_Warning(uint8 byWarning, uint32 dwWarningCode, uint32 dwParameter)
{
	if(byWarning)
	{
		gDBG_dwWarningCode  = dwWarningCode;
		gDBG_dwWarningParam = dwParameter;
	}
}

#endif

