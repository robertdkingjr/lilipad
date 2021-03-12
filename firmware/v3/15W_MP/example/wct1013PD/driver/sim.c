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

#include "sim.h"

void SIM_Init(void)
{
	SIM_CLKOUT = 0x1020U;	//Disable clock output
	SIM_GPSCL  = 0x0080U;   //GPIOC2/C3 = TXD0/RXD0
	SIM_GPSCH  = 0x0000U;   
	SIM_GPSFL  = 0x0050U;   //GPIOF4/F5 = TXD1/RXD1  F2/F3 = XBAR_OUT6/7
	SIM_PCE0   = 0xE07EU;
	SIM_PCE1   = 0x3840U;
	SIM_PCE2   = 0x168CU;
	SIM_PCE3   = 0x00E0U;
	SIM_SD2    = 0x0004U;   //PIT1 works in STOP mode
	SIM_IPS0   = 0x0101U;   //TA0 from XB_OUT49, PWM Fault0 from XB_OUT29
}
