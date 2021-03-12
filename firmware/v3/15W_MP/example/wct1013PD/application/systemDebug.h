/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __SYSTEM_DEBUG_H__
#define __SYSTEM_DEBUG_H__

#if _DEBUG
void DBG_Assert(uint8 byAssert, uint32 dwAssertCode, uint32 dwParameter);
void DBG_Warning(uint8 byWarn, uint32 dwAssertCode, uint32 dwParameter);
#else
#define DBG_Assert(...)
#define DBG_Warning(...)
#endif

#endif
