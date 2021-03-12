/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __TRIGGER_H__
#define __TRIGGER_H__

//CrossbarA OUT index
#define TRG_DST_TA0IN_TRIG    49
#define TRG_DST_ADCB_TRIG     13
#define TRG_DST_PWM_FAULT0    29

//CrossbarA IN index
#define TRG_SRC_NONE           0
#define TRG_SRC_PWMA1_TRG0    22
#define TRG_SRC_PWMA0_TRG0    20
#define TRG_SRC_CMPC_OUT      14
#define TRG_SRC_CMPD_OUT      15

void TRG_Init(void);
void TRG_SetTriggerSource(uint8 byTrgDst, uint8 byTrgSrc);
boolean TRG_CheckADCBTriggerSource(void);

#endif
