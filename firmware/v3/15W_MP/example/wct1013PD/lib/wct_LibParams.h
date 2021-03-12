/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __WCT_LIB_PARAMS_H__
#define __WCT_LIB_PARAMS_H__

void WCT_GetQFParams(uint8 byDeviceId, uint8 byCoilId, uint32 *pInitFreq, uint32 *pInitQlc);
FOD_CHARACTERIZATION_PARAMS* WCT_GetCharacterizatioinParams(uint8 byDeviceId, uint8 byCoilId, uint8 byControlType);
FOD_NORMALIZATION_PARAMS* WCT_GetNormalizationParams(uint8 byDeviceId, uint8 byCoilId, uint8 byControlType);

#endif
