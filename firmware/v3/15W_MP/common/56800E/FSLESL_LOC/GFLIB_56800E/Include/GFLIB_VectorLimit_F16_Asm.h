/*******************************************************************************
*
* Copyright 2007-2015 Freescale Semiconductor, Inc.
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale License
* distributed with this Material.
* See the LICENSE file distributed for more details.
* 
*
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

