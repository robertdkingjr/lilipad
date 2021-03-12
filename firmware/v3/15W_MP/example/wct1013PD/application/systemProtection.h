/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/
#ifndef __SYSTEM_PROTECTION_H__
#define __SYSTEM_PROTECTION_H__

boolean PROT_CheckRunTimeParams(uint8 byDeviceId, uint8 byCoilId, TX_CHARGING_STATUS eState, uint16 wGuaranteedPower, uint16 wTimePassedMs);
boolean PROT_CheckBoardParams(uint16 wTimePassedMs);

uint16 PROT_GetRRQDFittingInputCurrent(uint8 byDeviceId, uint8 byCoilId, uint16 wRailVoltage, uint32 dwFreq);
uint16 PROT_GetRRQDFittingCoilCurrent(uint8 byDeviceId, uint8 byCoilId, uint16 wRailVoltage, uint32 dwFreq);
boolean PROT_SafeDigitalPingParamCheck(uint8 byDeviceId);

#endif
