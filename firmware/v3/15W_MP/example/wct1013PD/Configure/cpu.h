/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __CPU_H__
#define __CPU_H__

//Note: This file defines specific mcu chip-related types, macros...

#include "MWCT1013.h"

#define CPU_IPBUS_CLK    100000000UL
#define CPU_TMR_CLK      CPU_IPBUS_CLK
#define CPU_PWM_CLK      3200000000UL
#define CPU_LP_CLK       32000U

#endif



