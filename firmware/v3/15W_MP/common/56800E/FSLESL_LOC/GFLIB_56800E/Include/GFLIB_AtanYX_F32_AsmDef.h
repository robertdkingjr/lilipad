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
* @brief  Definition file for arcus tangent function based on the provided x,y 
* 		  coordinates as arguments
* 
*******************************************************************************/
#ifndef _GFLIB_ATANYX_F32_ASMDEF_H_
#define _GFLIB_ATANYX_F32_ASMDEF_H_

#if defined(__cplusplus) 
extern "C" { 
#endif 
/******************************************************************************
* Includes
******************************************************************************/
#include "GFLIB_AtanYX_F32_Asm.h"

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Macros 
******************************************************************************/
#define GFLIB_AtanYX_F16_Asm(f16Y, f16X, pbErrFlag) GFLIB_AtanYX_F16_FAsm(f16Y, f16X, &gsAtanCoef_F32, pbErrFlag)

/******************************************************************************
* Types
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/
extern GFLIB_CONST GFLIB_ATAN_T_F32 gsAtanCoef_F32;

/******************************************************************************
* Global functions
******************************************************************************/

/******************************************************************************
* Inline functions
******************************************************************************/
#if defined(__cplusplus) 
} 
#endif 

#endif /* _GFLIB_ATANYX_F32_ASMDEF_H_ */
