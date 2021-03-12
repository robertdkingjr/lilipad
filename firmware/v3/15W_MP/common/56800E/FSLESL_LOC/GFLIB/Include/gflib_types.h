/*******************************************************************************
 * This software is provided AS IS and without warranty of any kind. 
 * Your right to use this software is subject to separate terms and conditions entered between you and NXP.
 * If you have not entered into such terms, you have no license rights to use the code.
 * Copyright 2007-2016 Freescale Semiconductor, Inc.
 * Copyright 2017~2019 NXP.
 * All rights reserved.
****************************************************************************//*!
*
* @brief  Basic types for GFLIB
* 
*******************************************************************************/
#ifndef _GFLIB_TYPES_H_
#define _GFLIB_TYPES_H_

#if defined(__cplusplus) 
extern "C" { 
#endif 

#define GFLIB_CONST const

typedef unsigned short bool_t;

/* Fractional data types */
typedef signed long long frac64_t;
typedef signed long frac32_t;
typedef signed short frac16_t;

/* Accumulator data types */
typedef signed long long acc64_t;
typedef signed long acc32_t;
typedef signed short acc16_t;

#if !defined(FRAC16)
#define FRAC16(x) ((frac16_t)((x) < 0.999969482421875 ? ((x) >= -1 ? (x)*0x8000 : 0x8000) : 0x7FFF))
#endif //!FRAC16

#if !defined(ACC32)
#define ACC32(x) ((acc32_t)((x) < 65535.999969482421875 ? ((x) >= -65536 ? (x)*0x8000 : 0x80000000) : 0x7FFFFFFF))
#endif //!ACC32

#endif /* _GFLIB_TYPES_H_ */
