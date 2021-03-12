/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2007-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
****************************************************************************//*!
*
* @brief  Look-up table algorithm implemented in assembler 
* 
*******************************************************************************/
#ifndef _GFLIB_LUT_F16_ASM_H_
#define _GFLIB_LUT_F16_ASM_H_

#if defined(__cplusplus) 
extern "C" { 
#endif 
/******************************************************************************
* Includes
******************************************************************************/
//#include "mlib.h"

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Macros 
******************************************************************************/
#define GFLIB_Lut1D_F16_Asm(f16X, pf16Table, u16TableSize) GFLIB_Lut1D_F16_FAsm(f16X, pf16Table, u16TableSize)
#define GFLIB_Lut2D_F16_Asm(f16X, f16Y, psParam) GFLIB_Lut2D_F16_FAsm(f16X, f16Y, psParam)

/******************************************************************************
* Types
******************************************************************************/
typedef struct
{
	uint16_t  u16ShamOffset1;
	uint16_t  u16ShamOffset2;
	frac16_t *pf16Table;
} GFLIB_LUT2D_T_F16;

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

extern asm frac16_t GFLIB_Lut1D_F16_FAsm(frac16_t f16X, const frac16_t *pf16Table, uint16_t u16TableSize);

extern asm frac16_t GFLIB_Lut2D_F16_FAsm(frac16_t f16X, frac16_t f16Y, const GFLIB_LUT2D_T_F16 *psParam);

/******************************************************************************
* Inline functions
******************************************************************************/
#if defined(__cplusplus) 
} 
#endif 

#endif /* _GFLIB_LUT_F16_ASM_H_ */
