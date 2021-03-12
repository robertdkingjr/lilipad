/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __FLASH_H__
#define __FLASH_H__

boolean FLASH_SectorDataErase(uint32 dwAddr);
boolean FLASH_WriteData(uint32 dwAddr, uint32 dwData);
void FLASH_ReadData(uint8 * pbyBuf, NVM_BYTE_POINTER pbyAddr, uint16 wLen);
void FLASH_Init(void);

#endif
