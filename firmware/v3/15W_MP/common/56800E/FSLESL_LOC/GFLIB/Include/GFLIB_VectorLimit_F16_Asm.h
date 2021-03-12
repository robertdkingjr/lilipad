/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2007-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
****************************************************************************//*!
*
* @brief  Vector limit functions with 16-bit fractional output in assembler
* 
*******************************************************************************/
#ifndef _GFLIB_VECTORLIMIT_F16_ASM_H_
#define _GFLIB_VECTORLIMIT_F16_ASM_H_

#if defined(__cplusplus) 
extern "C" { 
#endif 
/******************************************************************************
* Includes
******************************************************************************/
//#include "mlib.h"
#include "GFLIB_Sqrt_F32_AsmDef.h"

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Macros 
******************************************************************************/
#define GFLIB_VectorLimit_F16_Asm(psVectorIn, f16Lim, psVectorOut) GFLIB_VectorLimit_F16_FAsm(psVectorIn, f16Lim, psVectorOut)
#define GFLIB_VectorLimit1_F16_Asm(psVectorIn, f16Lim, psVectorOut) GFLIB_VectorLimit1_F16_FAsm(psVectorIn, f16Lim, psVectorOut)

/******************************************************************************
* Types
******************************************************************************/
typedef struct
{
	frac16_t f16A;
	frac16_t f16B;
} GFLIB_VECTORLIMIT_T_F16;

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/
extern asm bool_t GFLIB_VectorLimit_F16_FAsm(const GFLIB_VECTORLIMIT_T_F16 *psVectorIn, frac16_t f16Lim, GFLIB_VECTORLIMIT_T_F16 *psVectorOut);
extern asm bool_t GFLIB_VectorLimit1_F16_FAsm(const GFLIB_VECTORLIMIT_T_F16 *psVectorIn, frac16_t f16Lim, GFLIB_VECTORLIMIT_T_F16 *psVectorOut);
/******************************************************************************
* Inline functions
******************************************************************************/
#if defined(__cplusplus) 
} 
#endif 

#endif /* _GFLIB_VECTORLIMIT_F16_ASM_H_ */

