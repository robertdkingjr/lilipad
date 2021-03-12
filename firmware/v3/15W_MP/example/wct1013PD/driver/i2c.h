/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
*******************************************************************************/

#ifndef __I2C_H__
#define __I2C_H__ 

void I2C_Init(void);
void I2C_Enable(uint8 byIsEn);
boolean I2C_Read(uint8 add, uint8 *pRsp, uint16 Rsp_size);
boolean I2C_Write(uint8 add, uint8 *pCmd, uint16 Cmd_size);
void I2C_ClearFlag(void);
#endif
