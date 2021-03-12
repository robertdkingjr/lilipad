/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __DAC_H__
#define __DAC_H__

#define DAC_Q_MVL_INDEX     0

void DAC_Init(void);
void DAC_SetVoltage(uint8 byId, uint16 wVoltage);
void DAC_PowerDown(void);
void DAC_PowerUp(void);

#endif // __DAC_H__
