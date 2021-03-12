/*******************************************************************************
*
* Copyright 2007-2014 Freescale Semiconductor, Inc.
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale License
* distributed with this Material.
* See the LICENSE file distributed for more details.
* 
*
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
