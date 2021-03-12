/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __CMP_H__
#define __CMP_H__

#define CMP_GENERAL_CHANNEL_12BIT_DAC_INDEX   4

#define CMP_FRQ_INDEX            2
#define CMP_FRQ_CHANNEL_RESONANCE_INDEX  0

#define CMP_MP81_INDEX			 0

#define CMP_D_DCDC_CURRENT_LOW      3

#define CMP_POLARITY_NEGATIVE        0
#define CMP_POLARITY_POSITIVE        1


void CMP_Enable(uint8 byId, uint8 byIsEn);
void CMP_SetChannel(uint8 byId, uint8 byPolarity, uint8 byChannel);
void CMP_Init(void);
boolean CMP_CheckRaisingFlag(uint8 byId);
void CMP_ClearRaisingFlag(uint8 byId);
void CMP_SetInternalDAC(uint8 byId, uint16 wDACLevel);
void CMP_Configure(uint8 byId);

//To optimize the speed performance
#define CMP_Check_D_DCDC_Status()     ( CMPA_SCR&0x04 ? 1U : 0U)
#define CMP_Clean_D_DCDC_Flag()       {CMPA_SCR |= 0x04;}

#endif // __CMP_H__
