/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2007-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
****************************************************************************//*!
*
* @brief  Ramp functions with 32-bit fractional output in assembler
* 
*******************************************************************************/
#ifndef _GFLIB_RAMP_F32_ASM_H_
#define _GFLIB_RAMP_F32_ASM_H_

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
#define GFLIB_RampInit_F32_Asm(f32InitVal, psParam) GFLIB_RampInit_F32_FAsm(f32InitVal, psParam)

#define GFLIB_Ramp_F32_Asm(f32Target, psParam) GFLIB_Ramp_F32_FAsm(f32Target, psParam)

/******************************************************************************
* Types
******************************************************************************/
/* Ramp structure */
typedef struct
{
    frac32_t f32RampUp;
    frac32_t f32RampDown;
    frac32_t f32State;
} GFLIB_RAMP_T_F32;

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

extern asm frac32_t GFLIB_Ramp_F32_FAsm(frac32_t f32Target, GFLIB_RAMP_T_F32 *psParam);

/******************************************************************************
* Inline functions
******************************************************************************/

/***************************************************************************//*!
*
* @brief  The function initializes the actual value of Ramp_F32.
*
* @param  ptr   		GFLIB_RAMP_T_F32 *psParam
*						  - f32RampUp: Ramp-up increment
*						  - f32RampDown: Ramp-down increment
*						  - f32State: Previous ramp value
*
* @param  in    		frac16_t f16InitVal
* 							- Initial value
*
* @return N/A
*		
* @remarks 
*
****************************************************************************/
extern inline void GFLIB_RampInit_F32_FAsm(register frac32_t f32InitVal, register GFLIB_RAMP_T_F32 *psParam)
{
	asm(move.l f32InitVal,X:(psParam+4));
}

#if defined(__cplusplus) 
} 
#endif 

#endif /* _GFLIB_RAMP_F32_ASM_H_ */
