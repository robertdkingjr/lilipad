/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2007-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
****************************************************************************//*!
*
* @brief  Arcus tangent function
* 
*******************************************************************************/
#ifndef _GFLIB_ATAN_F32_ASM_H_
#define _GFLIB_ATAN_F32_ASM_H_

#if defined(__cplusplus) 
extern "C" { 
#endif 
/******************************************************************************
* Includes
******************************************************************************/
//#include "mlib.h"
#include "gflib_types.h"

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Macros 
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/
typedef struct
{
	frac32_t f32A[3];
} GFLIB_ATAN_COEF_T_F32;

typedef struct
{
	GFLIB_ATAN_COEF_T_F32 sAtanSector[8];
} GFLIB_ATAN_T_F32;

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

extern asm frac16_t GFLIB_Atan_F16_FAsm(frac16_t f16Angle, const GFLIB_ATAN_T_F32 *psParam);

/******************************************************************************
* Inline functions
******************************************************************************/
#if defined(__cplusplus) 
} 
#endif 

#endif /* _GFLIB_ATAN_F32_ASM_H_ */
