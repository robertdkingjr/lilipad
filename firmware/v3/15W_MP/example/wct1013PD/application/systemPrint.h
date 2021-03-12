/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/


#ifndef __SYSTEM_PRINT_H__
#define __SYSTEM_PRINT_H__

#if _DEBUG
#define APP_PrintChar(X)                 SPRT_PrintChar((uint8)(X))
#define APP_PrintString(X)               SPRT_PrintString((uint8*)(X))
#define APP_PrintDecChar(X)              SPRT_PrintDecChar(X)
#define APP_PrintHexChar(X)              SPRT_PrintHexChar(X)
#define APP_PrintSignedDecChar(X)        SPRT_PrintSignedDecChar(X)
#define APP_PrintSignedDecWord(X)        SPRT_PrintSignedDecWord(X)
#define APP_PrintDoubleWordValue(X)      SPRT_PrintDoubleWordValue(X)
#else
#define APP_PrintChar(X)
#define APP_PrintString(X)
#define APP_PrintDecChar(X)
#define APP_PrintHexChar(X)
#define APP_PrintSignedDecChar(X)
#define APP_PrintSignedDecWord(X)
#define APP_PrintDoubleWordValue(X)
#endif

void SPRT_PrintChar(uint8 byChar);
void SPRT_PrintString(uint8 *pbyStr);
void SPRT_PrintDecChar(uint8 byChar);
void SPRT_PrintHexChar(uint8 byChar);
void SPRT_PrintSignedDecChar(uint8 byChar);
void SPRT_PrintSignedDecWord(uint16 wValue);
void SPRT_PrintDoubleWordValue(uint32 dwValue);

#endif
