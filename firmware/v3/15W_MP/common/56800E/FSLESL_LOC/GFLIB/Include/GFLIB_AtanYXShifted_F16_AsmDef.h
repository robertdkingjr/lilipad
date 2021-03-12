/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2007-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
****************************************************************************//*!
*
* @brief  Definition file for AtanYXShifted function
* 
*******************************************************************************/
#ifndef _GFLIB_ATANYXSHIFTED_F16_ASMDEF_H_
#define _GFLIB_ATANYXSHIFTED_F16_ASMDEF_H_

#if defined(__cplusplus) 
extern "C" { 
#endif 
/******************************************************************************
* Includes
******************************************************************************/
#include "GFLIB_AtanYXShifted_F16_Asm.h"

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Macros 
******************************************************************************/

/******************************************************************************
* Types
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/
extern frac16_t GFLIB_AtanYXShifted_F16_Asm(frac16_t f16Y, 
											frac16_t f16X,
											const GFLIB_ATANYXSHIFTED_T_F16 *psParam);

/******************************************************************************
* Inline functions
******************************************************************************/
#if defined(__cplusplus) 
} 
#endif 

#endif /* _GFLIB_ATANYXSHIFTED_F32_ASMDEF_H_ */
