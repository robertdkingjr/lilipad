/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2007-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
****************************************************************************//*!
*
* @brief  Definition file for arcus sine and arcus cosine functions
* 
*******************************************************************************/
#ifndef _GFLIB_ASINACOS_F32_ASMDEF_H_
#define _GFLIB_ASINACOS_F32_ASMDEF_H_

#if defined(__cplusplus) 
extern "C" { 
#endif 
/******************************************************************************
* Includes
******************************************************************************/
#include "GFLIB_AsinAcos_F32_Asm.h"

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Macros 
******************************************************************************/
#define GFLIB_Asin_F16_Asm(f16Val) GFLIB_Asin_F16_FAsm(f16Val, &gsAsinCoef_F32)
#define GFLIB_Acos_F16_Asmi(f16Val) GFLIB_Acos_F16_FAsmi(f16Val, &gsAsinCoef_F32)

/******************************************************************************
* Types
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/
extern GFLIB_CONST GFLIB_ASIN_T_F32 gsAsinCoef_F32;

/*****************************************************************************
* Global functions
******************************************************************************/

/******************************************************************************
* Inline functions
******************************************************************************/
#if defined(__cplusplus) 
} 
#endif 

#endif /* _GFLIB_ASINACOS_F32_ASMDEF_H_ */
