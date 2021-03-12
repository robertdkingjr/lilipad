/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __QSCI_H__
#define __QSCI_H__

#define QSCI_CONSOLE_INDEX     1	//QSCI1
#define QSCI_FREEMASTER_INDEX  0	//QSCI0

void QSCI0_Init(uint32 baudrate);
uint8 QSCI0_PutChar(uint8 byChar);
void QSCI1_Init(uint32 baudrate);
uint8 QSCI1_PutChar(uint8 byChar);

void QSCI_ISRAck(uint8 byId);
uint8 QSCI_CheckTxEmptyFlag(uint8 byId);
void QSCI_PutChar(uint8 byId, uint8 byChar);
void QSCI_EnableTXEmptyInterrupt(uint8 byId, uint8 byIsEn);
#endif
