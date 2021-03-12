/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __WDG_H__
#define __WDG_H__


void WDG_Init(void);
void WDG_Kick(void);
void WDG_Enable(uint8 byIsEn);

#endif // __WDG_H__
