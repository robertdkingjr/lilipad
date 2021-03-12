/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __SYSTEM_CALLBACK_H__
#define __SYSTEM_CALLBACK_H__

void WCT_UpdateDevUsrIndication(uint8 byDeviceId);
uint32 WCT_SetReChargeTimeOnAbnormal(E_RECHARGETIME_SETTYPE eAbnormalType);
void WCT_OnWPCPacketRecv(uint8 byDeviceId, uint8 bySize, uint8 *pbyData);

#endif
