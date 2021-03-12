/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2007-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
****************************************************************************//*!
*
* @brief  GFLIB include file
* 
*******************************************************************************/
#ifndef _GFLIB_H_
#define _GFLIB_H_

#if defined(__cplusplus) 
extern "C" { 
#endif 

//#include "mlib.h"
#include "GFLIB_AsinAcos_F32_AsmDef.h"
#include "GFLIB_Atan_F32_AsmDef.h"
#include "GFLIB_AtanYX_F32_AsmDef.h"
#include "GFLIB_AtanYXShifted_F16_AsmDef.h"
#include "GFLIB_CtrlPIDp_A32_Asm.h"
#include "GFLIB_CtrlPIp_A32_Asm.h"
#include "GFLIB_DRamp_F16_Asm.h"
#include "GFLIB_DRamp_F32_Asm.h"
#include "GFLIB_Hyst_F16_Asm.h"
#include "GFLIB_Integrator_A32_Asm.h"
#include "GFLIB_Limit_F16_Asm.h"
#include "GFLIB_Limit_F32_Asm.h"
#include "GFLIB_VectorLimit_F16_Asm.h"
#include "GFLIB_Lut_F16_Asm.h"
#include "GFLIB_LutPer_F16_Asm.h"
#include "GFLIB_Ramp_F16_Asm.h"
#include "GFLIB_Ramp_F32_Asm.h"
#include "GFLIB_SinCos_F32_AsmDef.h"
#include "GFLIB_Sqrt_F32_AsmDef.h"
#include "GFLIB_Tan_F32_AsmDef.h"
#include "GFLIB_FlexRamp_A32_Asm.h"
#include "GFLIB_DFlexRamp_A32_Asm.h"

/* Redeclaration of GFLIB function */
#define GFLIB_Sin_F16(f16Angle) GFLIB_Sin_F16_Asm(f16Angle)
#define GFLIB_Cos_F16(f16Angle) GFLIB_Cos_F16_Asm(f16Angle)

#define GFLIB_Tan_F16(f16Angle) GFLIB_Tan_F16_Asm(f16Angle)

#define GFLIB_Asin_F16(f16Val) GFLIB_Asin_F16_Asm(f16Val)
#define GFLIB_Acos_F16(f16Val) GFLIB_Acos_F16_Asmi(f16Val)

#define GFLIB_Atan_F16(f16Val) GFLIB_Atan_F16_Asm(f16Val)

#define GFLIB_AtanYX_F16(f16Y, f16X, pbErrFlag) GFLIB_AtanYX_F16_Asm(f16Y, f16X, pbErrFlag)

#define GFLIB_AtanYXShifted_F16(f16Y, f16X, psParam) GFLIB_AtanYXShifted_F16_Asm(f16Y, f16X, psParam)

#define GFLIB_Lut1D_F16(f16X, pf16Table, u16TableSize) GFLIB_Lut1D_F16_Asm(f16X, pf16Table, u16TableSize)
#define GFLIB_LutPer1D_F16(f16X, pf16Table, u16TableSize) GFLIB_LutPer1D_F16_Asm(f16X, pf16Table, u16TableSize)

#define GFLIB_RampInit_F16(f16InitVal, psParam) GFLIB_RampInit_F16_Asm(f16InitVal, psParam)
#define GFLIB_RampInit_F32(f32InitVal, psParam) GFLIB_RampInit_F32_Asm(f32InitVal, psParam)
#define GFLIB_Ramp_F16(f16Target, psParam) GFLIB_Ramp_F16_Asm(f16Target, psParam)
#define GFLIB_Ramp_F32(f32Target, psParam) GFLIB_Ramp_F32_Asm(f32Target, psParam)

#define GFLIB_DRampInit_F16(f16InitVal, psParam) GFLIB_DRampInit_F16_Asm(f16InitVal, psParam)
#define GFLIB_DRampInit_F32(f32InitVal, psParam) GFLIB_DRampInit_F32_Asm(f32InitVal, psParam)
#define GFLIB_DRamp_F16(f16Target, f16Instant, pbStopFlag, psParam) GFLIB_DRamp_F16_Asm(f16Target, f16Instant, pbStopFlag, psParam)
#define GFLIB_DRamp_F32(f16Target, f16Instant, pbStopFlag, psParam) GFLIB_DRamp_F32_Asm(f16Target, f16Instant, pbStopFlag, psParam)

#define GFLIB_FlexRampInit_F16(f16InitVal, psParam) GFLIB_FlexRampInit_F16_Asm(f16InitVal, psParam)
#define GFLIB_FlexRampCalcIncr_F16(f16Target, a32Duration, psParam) GFLIB_FlexRampCalcIncr_F16_Asm(f16Target, a32Duration, psParam)
#define GFLIB_FlexRamp_F16(psParam) GFLIB_FlexRamp_F16_Asm(psParam)

#define GFLIB_DFlexRampInit_F16(f16InitVal, psParam) GFLIB_DFlexRampInit_F16_Asm(f16InitVal, psParam)
#define GFLIB_DFlexRampCalcIncr_F16(f16Target, a32Duration, f32IncrSatMot, f32IncrSatGen, psParam) GFLIB_DFlexRampCalcIncr_F16_Asm(f16Target, a32Duration, f32IncrSatMot, f32IncrSatGen, psParam)
#define GFLIB_DFlexRamp_F16(f16Instant, pbStopFlagMot, pbStopFlagGen, psParam) GFLIB_DFlexRamp_F16_Asm(f16Instant, pbStopFlagMot, pbStopFlagGen, psParam)

#define GFLIB_Limit_F16(f16Val, f16LLim, f16ULim) GFLIB_Limit_F16_Asmi(f16Val, f16LLim, f16ULim)
#define GFLIB_Limit_F32(f32Val, f32LLim, f32ULim) GFLIB_Limit_F32_Asmi(f32Val, f32LLim, f32ULim)
#define GFLIB_UpperLimit_F16(f16Val, f16ULim) GFLIB_UpperLimit_F16_Asmi(f16Val, f16ULim)
#define GFLIB_UpperLimit_F32(f32Val, f32ULim) GFLIB_UpperLimit_F32_Asmi(f32Val, f32ULim)
#define GFLIB_LowerLimit_F16(f16Val, f16LLim) GFLIB_LowerLimit_F16_Asmi(f16Val, f16LLim)
#define GFLIB_LowerLimit_F32(f32Val, f32LLim) GFLIB_LowerLimit_F32_Asmi(f32Val, f32LLim)
#define GFLIB_VectorLimit_F16(psVectorIn, f16Lim, psVectorOut) GFLIB_VectorLimit_F16_Asm(psVectorIn, f16Lim, psVectorOut)
#define GFLIB_VectorLimit1_F16(psVectorIn, f16Lim, psVectorOut) GFLIB_VectorLimit1_F16_Asm(psVectorIn, f16Lim, psVectorOut)
#define GFLIB_VectorLimit_F32(psVectorIn, f32Lim, psVectorOut) GFLIB_VectorLimit_F32_Asm(psVectorIn, f32Lim, psVectorOut)

#define GFLIB_Hyst_F16(f16Val, psParam) GFLIB_Hyst_F16_Asmi(f16Val, psParam)

#define GFLIB_CtrlPIpInit_F16(f16InitVal, psParam) GFLIB_CtrlPIpInit_F16_Asmi(f16InitVal, psParam)
#define GFLIB_CtrlPIpAWInit_F16(f16InitVal, psParam) GFLIB_CtrlPIpAWInit_F16_Asmi(f16InitVal, psParam)
#define GFLIB_CtrlPIp_F16(f16InErr, psParam) GFLIB_CtrlPIp_F16_Asm(f16InErr, psParam)
#define GFLIB_CtrlPIpAW_F16(f16InErr, pbStopIntegFlag, psParam) GFLIB_CtrlPIpAW_F16_Asm(f16InErr, pbStopIntegFlag, psParam)
#define GFLIB_CtrlPIDpInit_F16(f16InitVal, psParam) GFLIB_CtrlPIDpInit_F16_Asmi(f16InitVal, psParam)
#define GFLIB_CtrlPIDpAWInit_F16(f16InitVal, psParam) GFLIB_CtrlPIDpAWInit_F16_Asmi(f16InitVal, psParam)
#define GFLIB_CtrlPIDp_F16(f16InErr, f16InErrD, psParam) GFLIB_CtrlPIDp_F16_Asm(f16InErr, f16InErrD, psParam)
#define GFLIB_CtrlPIDpAW_F16(f16InErr, f16InErrD, pbStopIntegFlag, psParam) GFLIB_CtrlPIDpAW_F16_Asm(f16InErr, f16InErrD, pbStopIntegFlag, psParam)

#define GFLIB_IntegratorInit_F16(f16InitVal, psParam) GFLIB_IntegratorInit_F16_Asmi(f16InitVal, psParam)
#define GFLIB_Integrator_F16(f16InVal, psParam) GFLIB_Integrator_F16_Asmi(f16InVal, psParam)

#define GFLIB_Sqrt_F16(f16Val) GFLIB_Sqrt_F16_Asm(f16Val)
#define GFLIB_Sqrt_F16l(f32Val) GFLIB_Sqrt_F16l_Asm(f32Val)

#if defined(__cplusplus) 
} 
#endif 

#endif

