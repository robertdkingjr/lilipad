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

#include "trigger.h"
              
void TRG_Init(void)
{
	XBARA_SEL2  = 0x1400U;  //PWMA0 trigger 0 route to XB_OUT5
	XBARA_SEL3  = 0x0014U;  //PWMA0 route to XB_OUT6
	XBARA_SEL6  = 0x0000U;  //0x1400U;	//PWMA0 trigger 0 route to ADCB trigger
	XBARA_SEL12 = 0x0E00U;	//CMPC OUT route to PWMA1 EXT SYNC
	XBARA_SEL24 = 0x0E00U;  //CMPC OUT route to TA0 IN
}


//Set byTrgSrc route to byTrgDst
void TRG_SetTriggerSource(uint8 byTrgDst, uint8 byTrgSrc)
{
	uint16 *pDst, index;
	uint16 sel;
	DBG_Assert(byTrgDst>58 , 0, byTrgDst);
	
	pDst = (uint16*)&XBARA_SEL0;
	
	index = byTrgDst / 2;
	
	sel   = pDst[index];
	
	if(byTrgDst & 0x01)
	{
		sel &= 0x00FF;
		sel |= (byTrgSrc<<8);
	}
	else
	{
		sel &= 0xFF00;
		sel |= byTrgSrc;
	}
	pDst[index] = sel;
}

//Check if there is valid trigger source for ADCB
boolean TRG_CheckADCBTriggerSource(void)
{
	boolean ret = 0;
	
	if(XBARA_SEL6 & 0x3F00)
	{
		ret = 1;
	}
	
	return ret;
}
